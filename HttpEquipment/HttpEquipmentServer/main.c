#include "../mongoose.h"
#include <signal.h>

static int s_debug_level = MG_LL_INFO;
static const char* s_listening_address = "http://0.0.0.0:8000";
static const char* s_enable_hexdump = "no";

static int s_signo;
static void signal_handler(int signo)
{
    s_signo = signo;
}

static char light_status[5] = "OFF";
static char color[3] = "R"; // R G B W A

static void cb(struct mg_connection* c, int ev, void* ev_data)
{
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message* hm = (struct mg_http_message*)ev_data;
        if (mg_match(hm->uri, mg_str("/lightStatus"), NULL)) {

            mg_http_reply(c, 200, "", "{ \"lightStatus\":\"%s\"}", light_status);

        } else if (mg_match(hm->uri, mg_str("/lightColor"), NULL)) {

            mg_http_reply(c, 200, "", "{ \"lightColor\":\"%s\"}", color);

        } else if (mg_match(hm->uri, mg_str("/lightIP"), NULL)) {

            mg_http_reply(c, 200, "", "{ \"IP\":\"%s\"}", s_listening_address);

        } else if (mg_match(hm->uri, mg_str("/Setcolor"), NULL)) {

            char query_decode[3];
            int size = mg_http_get_var(&hm->query, "color", query_decode, hm->query.len);
            if (size != 1) {
                mg_http_reply(c, 200, "", "ERROR get size = %d", size);
            } else if (query_decode[0] == 'R' || query_decode[0] == 'G' || query_decode[0] == 'B' || query_decode[0] == 'W' || query_decode[0] == 'A') {
                color[0] = query_decode[0];
                mg_http_reply(c, 200, "", "{ \"color\" : \"%s\"}", color);
            } else {
                mg_http_reply(c, 200, "", "ERROR color %s", query_decode);
            }

        } else if (mg_match(hm->uri, mg_str("/lightON"), NULL)) {
            light_status[0] = 'O';
            light_status[1] = 'N';
            light_status[2] = '\0';
            mg_http_reply(c, 200, "", "{ \"lightStatus\":\"%s\"}", light_status);
        } else if (mg_match(hm->uri, mg_str("/lightOFF"), NULL)) {
            light_status[0] = 'O';
            light_status[1] = 'F';
            light_status[2] = 'F';
            light_status[3] = '\0';
            mg_http_reply(c, 200, "", "{ \"lightStatus\":\"%s\"}", light_status);
        } else if (mg_match(hm->uri, mg_str("/lightInfo"), NULL)) {
            mg_http_reply(c, 200, "", "{ \"lightStatus\":\"%s\", \"color\":\"%s\", \"IP\": \"%s\"}", light_status, color, s_listening_address);
        } else {
            mg_http_reply(c, 404, "", "NOT FOUND");
        }

        // Log request
        MG_INFO(("%.*s %.*s %lu -> %.*s %lu", hm->method.len, hm->method.buf,
            hm->uri.len, hm->uri.buf, hm->body.len, 3, c->send.buf + 9,
            c->send.len));
    }
}

int main(int argc, char* argv[])
{
    struct mg_mgr mgr;
    struct mg_connection* c;

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    mg_log_set(s_debug_level);
    mg_mgr_init(&mgr);
    if ((c = mg_http_listen(&mgr, s_listening_address, cb, &mgr)) == NULL) {
        MG_ERROR(("Cannot listen on %s. Use http://ADDR:PORT or :PORT",
            s_listening_address));
        exit(EXIT_FAILURE);
    }
    if (mg_casecmp(s_enable_hexdump, "yes") == 0)
        c->is_hexdumping = 1;

    MG_INFO(("Listening on     : %s", s_listening_address));
    while (s_signo == 0)
        mg_mgr_poll(&mgr, 1000);
    mg_mgr_free(&mgr);
    MG_INFO(("Exiting on signal %d", s_signo));
    return 0;
}
