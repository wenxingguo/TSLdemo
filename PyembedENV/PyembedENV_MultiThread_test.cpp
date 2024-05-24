#include "PyembedENV.h"
#include <functional>
#include <future>
#include <iostream>
#include <thread>
#include <type_traits>
#include <vector>

void My_Python_MainEVN_Manager_init()
{
    PyConfig config;
    PyConfig_InitPythonConfig(&config);
    PyConfig_SetString(&config, &config.executable,
        L"/opt/python-d/bin/python3.10");
    Py_InitializeFromConfig(&config);
    Add_Module_Search_Path("../../PyembedENV");
    PyImport_AddModule("__main__");
}

int main(int argc, char* argv[])
{
    if (!python_main_thread_state) {
        python_main_thread_state.reset(new Python_MainENV_Manager(My_Python_MainEVN_Manager_init));
    }

    // 无返回值
    // async_Call_By_Name(std::future<std::void_t<>>(), "testmodule", "echo", "{\"first\":1, \"second\":2}");

    // 返回PyObject*
    PyObject* m = Get_Module_by_Name("__main__");
    PyObject* d1 = PyModule_GetDict(m);
    Py_XINCREF(d1);
    Py_XINCREF(d1);

    // 调用 builtins 的eval
    std::future<PyObject*> call_result;
    // async_Call_By_Name(call_result, "testmodule", "EVAL", "{\"first\":1, \"second\":2}");
    async_Call_By_Name(call_result, "builtins", "eval", "{\"first\":1, \"second\":2}", d1, d1);

    // 确保异步调用完成，防止死锁
    PyObject* result = call_result.get();
    {
        GIL_EnSure s;
        if (PyDict_Check(result) == 1) {
            PyObject* info = PyObject_Str(result);
            std::cout << PyUnicode_AsUTF8(info) << std::endl;
            Py_XDECREF(info);

            PyObject* first = PyDict_GetItemString(result, "first");
            PyObject* second = PyDict_GetItemString(result, "second");
            std::cout << PyLong_AsLong(first) << " " << PyLong_AsLong(second) << std::endl;
        }
        Py_XDECREF(result);
    }
    // std::cout << call_result.get() << std::endl;
    // Py_XDECREF(test_module);
    // Py_XDECREF(test_func);
    // Py_XDECREF(echo_func);

    // PyObject*

    return 0;
}
