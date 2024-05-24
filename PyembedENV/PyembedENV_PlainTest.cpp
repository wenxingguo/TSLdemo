#define PY_SSIZE_T_CLEAN
#include <Python.h>

int main(int argc, char* argv[])
{
    wchar_t* program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }
    Py_SetProgramName(program); /* optional but recommended */
    Py_Initialize();

    // PyRun_SimpleString("print(eval('1+1'))");

    PyObject* module_name_obj = PyUnicode_FromString("builtins");
    PyObject* module_builtins = PyImport_GetModule(module_name_obj);
    PyObject* func_eval = PyObject_GetAttrString(module_builtins, "eval");

    // 不用处理
    PyObject* m = PyImport_AddModule("__main__");
    PyObject* d1 = PyModule_GetDict(m);
    PyObject* d2 = PyModule_GetDict(m);
    // 手动添加引用
    Py_XINCREF(d1);
    Py_XINCREF(d2);

    PyObject* arg = PyUnicode_FromString("{'first':1, 'second':2}");
    PyObject* intuple = PyTuple_New(3);
    PyTuple_SetItem(intuple, 0, arg);
    PyTuple_SetItem(intuple, 1, d1);
    PyTuple_SetItem(intuple, 2, d2);

    // PyObject* return_data = PyObject_CallFunction()

    // PyObject* return_object = PyObject_CallFunctionObjArgs(func_eval, arg, d, d, NULL);
    PyObject* return_object = PyObject_CallObject(func_eval, intuple);
    assert(PyDict_Check(return_object) == 1);
    PyObject* error = PyErr_Occurred();

    if (error != NULL)
        PyErr_Print();
    assert(error == NULL);

    Py_XDECREF(module_builtins);
    Py_XDECREF(module_name_obj);
    // Py_XDECREF(m);
    Py_XDECREF(func_eval);
    Py_XDECREF(intuple);
    Py_XDECREF(return_object);

    PyRun_SimpleString("from time import time,ctime\n"
                       "print('Today is', ctime(time()))\n");
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    PyMem_RawFree(program);
    return 0;
}
