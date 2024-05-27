#ifndef PYEMBED_ENV_H
#define PYEMBED_ENV_H
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <functional>
#include <future>
#include <memory>
#include <string>
#include <thread>

class GIL_EnSure {
public:
    GIL_EnSure()
    {
        if (PyGILState_Check() == 1) { // 该线程已经获得GIL锁
            GIL_already_hold = true;
            return;
        }
        GIL_state = PyGILState_Ensure();
        GIL_already_hold = false;
    }

    ~GIL_EnSure()
    {
        if (GIL_already_hold == false) {
            PyGILState_Release(GIL_state);
        }
    }

private:
    PyGILState_STATE GIL_state;
    bool GIL_already_hold;
};

/*************************************************************
 * 一般多线程程序互斥量初始化是未加锁
 * Python3 解释器的GIL通常属于未加锁状态 即初始化后Python主线程释放GIL
 * 需要执行python脚本的线程主动对申请对GIL加锁
 *************************************************************/
class Python_MainENV_Manager { // 初始化Python3主解释器

    typedef void (*custom_init_fn)();
    Python_MainENV_Manager(const Python_MainENV_Manager&) = delete;
    Python_MainENV_Manager& operator=(Python_MainENV_Manager&) = delete;

public:
    explicit Python_MainENV_Manager(custom_init_fn custom_init_ = nullptr)
        : custom_init(custom_init_)
    {
        if (custom_init_) { // 自定义配置
            custom_init_();
        } else { // 基本配置
            PyConfig config;
            PyConfig_InitPythonConfig(&config);
            PyConfig_SetString(&config, &config.executable,
                L"../python-embed/bin/python3");
            Py_InitializeFromConfig(&config);
        }
        //释放锁
        main_thread_state = PyEval_SaveThread();
    }

    void RestoreMainThread()
    {
        PyEval_RestoreThread(main_thread_state);
    }

    void SaveMainThead()
    {
        main_thread_state = PyEval_SaveThread();
    }

    virtual ~Python_MainENV_Manager()
    {
        // 恢复主线程环境
        PyEval_RestoreThread(main_thread_state);
        // 退出python解释器
        Py_FinalizeEx();
    }

private:
    PyThreadState* main_thread_state;
    custom_init_fn custom_init;
};

extern std::shared_ptr<Python_MainENV_Manager> python_main_thread_state; // 此变量不能在子线程中使用

PyObject* Get_Module_by_Name(const std::string& module_name);

PyObject* Get_Attr_by_Name(PyObject* module, const std::string& attr_name);

void Add_Module_Search_Path(const std::string& module_path);

template <typename T>
T call_object(PyObject* callable, PyObject* args_tuple)
{
    assert(PyCallable_Check(callable) == 1);
    assert(PyTuple_Check(args_tuple) == 1);
    GIL_EnSure s;

    // 调用参数为PyTuple
    // 对于builtins的内的函数，建议在脚本中进行调用
    /*
     * 如eval()
     * def module_eval(in_str : str):
     *   return eval(in_str)
     * 通过模块调用module_eval()
     * */

    if constexpr (std::is_same_v<std::void_t<>, T>) {
        PyObject_CallObject(callable, args_tuple);
        PyObject* err = PyErr_Occurred();
        if (err != NULL)
            PyErr_Print();
        assert(err == NULL);

        Py_XDECREF(args_tuple);
    } else {
        PyObject* return_data = PyObject_CallObject(callable, args_tuple);
        PyObject* err = PyErr_Occurred();
        if (err != NULL)
            PyErr_Print();
        assert(err == NULL);
        T result;
        if (return_data) { // 类型需要完备
            if constexpr (std::is_same_v<int, T>) {
                result = (T)PyLong_AsLong(return_data);
                Py_XDECREF(return_data);
            }
            if constexpr (std::is_same_v<std::string, T>) {
                result = std::string(PyUnicode_AsUTF8(return_data));
                Py_XDECREF(return_data);
            }
            if constexpr (std::is_same_v<PyObject*, T>) {
                result = return_data;
            }
        }
        Py_XDECREF(args_tuple);
        return result;
    }
}

static void _Gen_Tuple_from_Args(PyObject* py_tuple, int index)
{
    assert(PyTuple_Size(py_tuple) == index);
    return;
}

template <typename T, typename... Args>
void _Gen_Tuple_from_Args(PyObject* py_tuple, int index, T&& head, Args&&... args)
{
    PyObject* item = NULL;
    using D_T = std::decay_t<T>;
    if constexpr (std::is_same_v<int, D_T> || std::is_same_v<std::size_t, D_T>) {
        item = PyLong_FromLong(head);
    } else if constexpr (std::is_same_v<double, D_T> || std::is_same_v<float, D_T>) {
        item = PyFloat_FromDouble(head);
    } else if constexpr (std::is_same_v<std::string, D_T>) {
        item = PyUnicode_FromString(head.c_str());
    } else if constexpr (std::is_same_v<char const*, D_T> || std::is_same_v<char*, D_T>) {
        item = PyUnicode_FromString(head);
    } else if constexpr (std::is_same_v<PyObject*, D_T>) {
        // 支持PyObject*
        item = head;
    }
    assert(item != NULL);
    // item的所有权转移
    PyTuple_SetItem(py_tuple, index, item);
    _Gen_Tuple_from_Args(py_tuple, index + 1, std::forward<Args>(args)...);
}

template <typename... Args>
PyObject* Gen_Tuple_from_Args(Args&&... args)
{
    GIL_EnSure s;
    Py_ssize_t total_size = sizeof...(args);

    PyObject* py_tuple = PyTuple_New(total_size);
    _Gen_Tuple_from_Args(py_tuple, 0, std::forward<Args>(args)...);
    return py_tuple;
}

// 同步调用
template <typename T, typename... Args>
T sync_Call_Object(PyObject* callable, Args&&... args)
{
    PyObject* args_tuple = Gen_Tuple_from_Args(std::forward<Args>(args)...);
    T result = call_object<T>(callable, args_tuple);
    return result;
}

template <typename T, typename... Args>
T sync_Call_By_Name(const std::string& module_name, const std::string& object_name, Args&&... args)
{
    PyObject* module_Obj = Get_Module_by_Name(module_name);
    PyObject* func_Obj = Get_Attr_by_Name(module_Obj, object_name);
    T result = sync_Call_Object<T>(func_Obj, std::forward<Args>(args)...);
    Py_XDECREF(module_Obj);
    Py_XDECREF(func_Obj);
    return result;
}

// 异步调用
template <typename RT, template <typename RT_> typename T, typename... Args>
void async_Call_Object(T<RT>&& future_out, Args&&... args) //-> std::future<T>
{
    if constexpr (std::is_same_v<std::future<RT>, T<RT>>) {
        std::function<RT()> func = std::bind(&call_object<RT>, std::forward<Args>(args)...);
        future_out = std::async(std::launch::async, func);
    } else if constexpr (std::is_same_v<std::future<std::void_t<>>, T<RT>>) {
        std::function<void()> func = std::bind(&call_object<std::void_t<>>, std::forward<Args>(args)...);
        std::thread thread_id(func);
        thread_id.detach();
    }
}

template <typename RT, template <typename RT_> typename T, typename... Args>
void async_Call_Object(T<RT>& future_out, Args&&... args) //-> std::future<T>
{
    async_Call_Object(std::move(future_out), std::forward<Args>(args)...);
}

template <typename RT, template <typename RT_> typename T, typename... Args>
void async_Call_By_Name(T<RT>&& future_out,
    const std::string& module_name,
    const std::string& object_name,
    Args&&... args) // args 为python方法调用的参数
{
    PyObject* module_Obj = Get_Module_by_Name(module_name);
    assert(module_Obj != NULL);
    PyObject* func_Obj = Get_Attr_by_Name(module_Obj, object_name);
    assert(func_Obj != NULL && PyCallable_Check(func_Obj) == 1);

    // args_tuple是新引用，在call_object中释放, 以确保其调用时有效
    // 该函数存在所有权的转移
    PyObject* args_tuple = Gen_Tuple_from_Args(std::forward<Args>(args)...);

    async_Call_Object(future_out, func_Obj, args_tuple);
    Py_XDECREF(func_Obj);
    Py_XDECREF(module_Obj);
}

template <typename RT, template <typename RT_> typename T, typename... Args>
void async_Call_By_Name(T<RT>& future_out,
    const std::string& module_name,
    const std::string& object_name,
    Args&&... args)
{
    async_Call_By_Name(std::move(future_out), module_name, object_name, std::forward<Args>(args)...);
}

#endif /* ifndef PYEMBED_ENV_H */
