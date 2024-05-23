#include "HttpEquipment.h"
#include "../PyembedENV/PyembedENV.h"
#include <iostream>

void My_Python_MainEVN_Manager_init()
{
    PyConfig config;
    PyConfig_InitPythonConfig(&config);
    PyConfig_SetString(&config, &config.executable,
        L"/opt/python-d/bin/python3.10");
    Py_InitializeFromConfig(&config);
    Add_Module_Search_Path("../../HttpEquipment/pymoudle");
}

std::string ret_msg_handle(const std::string& in_data, const std::string& module_name, const std::string& attr_name)
{
    std::future<std::string> result;
    async_Call_By_Name(result, module_name, attr_name, in_data);
    std::string out = result.get();
    return out;
}

int main(int argc, char* argv[])
{
    if (!python_main_thread_state) {
        python_main_thread_state.reset(new Python_MainENV_Manager(My_Python_MainEVN_Manager_init));
    }

    ReturnMsgHandle S_retmsg_handle = std::bind(ret_msg_handle, std::placeholders::_1, "RetmsgHandle", "ServiceRetmsdHandle");
    ReturnMsgHandle P_retmsg_handle = std::bind(ret_msg_handle, std::placeholders::_1, "RetmsgHandle", "PropertyRetmsdHandle");

    Equipment* eq1 = new HttpEquipment("../../TSLParserTest/light.json", "localhost:8000", S_retmsg_handle, P_retmsg_handle);
    // std::cout << eq1->getUrlbyIdentifier(argv[1]) << std::endl;
    std::vector<std::string> argvs;
    for (int i = 2; i < argc; ++i) {
        argvs.push_back(argv[i]);
    }

    std::cout << eq1->invokeService(argv[1], argvs) << std::endl;

    // std::cout << eq1->getProPerty(argv[1]) << std::endl;

    return 0;
}
