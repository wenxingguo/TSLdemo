#include "PyembedENV.h"

void My_Python_MainEVN_Manager_init()
{
    PyConfig config;
    PyConfig_InitPythonConfig(&config);
    PyConfig_SetString(&config, &config.executable,
        L"./pyembed-venv/bin/python3");
    Py_InitializeFromConfig(&config);
    Add_Module_Search_Path("../../PyembedENV");
    PyImport_AddModule("__main__");
}

int main(int argc, char* argv[])
{
    if (!python_main_thread_state) {
        python_main_thread_state.reset(new Python_MainENV_Manager(My_Python_MainEVN_Manager_init));
    }

    PyObject* dict_obj = sync_Call_By_Name<PyObject*>("testmodule", "EVAL", "{\"first\":1, \"second\":2}");
    assert(PyDict_Check(dict_obj) == 1);

    {
        GIL_EnSure s;
        PyRun_SimpleString("print(\"hello Python!\")");
        FILE* file = fopen("./testplot.py", "r");
        if (file) {
            PyRun_SimpleFile(file, "./testplot.py");
            fclose(file);
        }
        // GIL解锁
    }

    return 0;
}
