#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H
#include "mongoose.h"
#include <functional>
#include <string>

typedef std::function<std::string(const std::string&)> ReturnMsgHandle;

// typedef std::string (*ReturnMsgHandle)(const std::string&);

struct ev_handle_data {
    bool done;
    std::string url;
    std::string return_msg;
    ReturnMsgHandle return_msg_handle;
};

static void http_en_handle(mg_connection* c, int ev, void* ev_data)
{
    ev_handle_data* data = (ev_handle_data*)c->fn_data;
    if (ev == MG_EV_OPEN) {
    } else if (ev == MG_EV_POLL) {
    } else if (ev == MG_EV_CONNECT) {
        mg_str host = mg_url_host(data->url.c_str());
        std::string return_ms;
        mg_printf(c, "%s %s HTTP/1.0\r\n"
                     "Host: %.*s\r\n"
                     "Content-Type: octet-stream\r\n"
                     "Content-Length: %d\r\n"
                     "\r\n",
            "GET", mg_url_uri(data->url.c_str()), (int)host.len, host.buf, 0);
        mg_send(c, NULL, 0);
    } else if (ev == MG_EV_HTTP_MSG) {

        // 处理返回值
        mg_http_message* hm = (mg_http_message*)ev_data;
        // printf("%s\n", hm->body.buf);

        data->return_msg = std::string(hm->body.buf);
        if (data->return_msg_handle) {
            data->return_msg = data->return_msg_handle(data->return_msg);
        }
        c->is_draining = 1;
        // std::cout << "msg" << std::endl;
        data->done = true;
    } else if (ev == MG_EV_ERROR) {
        data->return_msg = "MG_EV_ERROR";
        // std::cout << "error" << std::endl;
        data->done = true;
    }
}

// struct HttpEquipmentInfo {
//     std::string
//};

class HttpClient {
public:
    static std::string connect_once(const std::string& url, ReturnMsgHandle ret_handle = nullptr)
    {
        mg_mgr mgr;
        mg_mgr_init(&mgr);
        ev_handle_data ev_data;
        ev_data.done = false;
        ev_data.url = url;
        ev_data.return_msg_handle = ret_handle;
        mg_connection* conn = mg_http_connect(&mgr, url.c_str(), http_en_handle, (void*)&ev_data);
        if (conn == NULL) {
            // std::cerr << "connect ERROR" << std::endl;
            return { "connect ERROR" };
        }
        while (!ev_data.done) {
            mg_mgr_poll(&mgr, 0);
        }
        mg_mgr_free(&mgr);
        return ev_data.return_msg;
    }

    HttpClient() {};

    virtual ~HttpClient() {};
};

#endif /* HTTPCLIENT_H */
