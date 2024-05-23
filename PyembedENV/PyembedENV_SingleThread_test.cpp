#include "PyembedENV.h"

void My_Python_MainEVN_Manager_init()
{
    PyConfig config;
    PyConfig_InitPythonConfig(&config);
    PyConfig_SetString(&config, &config.executable,
        L"./pyembed-venv/bin/python3");
    Py_InitializeFromConfig(&config);
    Add_Module_Search_Path("./");
}

int main(int argc, char* argv[])
{
    if (!python_main_thread_state) {
        python_main_thread_state.reset(new Python_MainENV_Manager(My_Python_MainEVN_Manager_init));
    }
    {
        GIL_EnSure s;
        PyRun_SimpleString("print(\"hello Python!\")");
        FILE* file = fopen("./testplot.py", "r");
        if (file)
            PyRun_SimpleFile(file, "./testplot.py");
        // GIL解锁
    }

    return 0;
}
