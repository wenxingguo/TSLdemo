#include "PyembedENV.h"

//全局变量
std::shared_ptr<Python_MainENV_Manager> python_main_thread_state(nullptr);

PyObject* Get_Module_by_Name(const std::string& module_name)
{
    GIL_EnSure s;
    PyObject* module_name_obj = PyUnicode_FromString(module_name.c_str());
    PyObject* result = PyImport_GetModule(module_name_obj);
    if (!result) {
        result = PyImport_ImportModule(module_name.c_str()); // 返回新的引用
    }
    Py_XDECREF(module_name_obj);
    return result;
}

PyObject* Get_Attr_by_Name(PyObject* module, const std::string& attr_name)
{
    GIL_EnSure s;
    assert(PyModule_Check(module) == 1);
    PyObject* attr_obj = PyObject_GetAttrString(module, attr_name.c_str());
    assert(attr_obj != NULL);
    return attr_obj;
}

void Add_Module_Search_Path(const std::string& module_path)
{
    // 查看GIL锁
    // 以下代码需要在GIL加锁的情况下执行
    GIL_EnSure s;
    PyObject* sys_module = Get_Module_by_Name("sys");

    assert(sys_module != 0);
    PyObject* sys_path = PyObject_GetAttrString(sys_module, "path");
    assert(PyList_Check(sys_path) == 1);
    PyObject* module_path_obj = PyUnicode_FromString(module_path.c_str());
    PyList_Append(sys_path, module_path_obj);
    // Py_XDECREF(sys_module_name);
    Py_XDECREF(sys_module);
    Py_XDECREF(sys_path);
    Py_XDECREF(module_path_obj);
}
