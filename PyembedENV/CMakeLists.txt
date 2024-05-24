project(PyembedENV)

find_package(Python3 REQUIRED)
find_package(PythonLibs REQUIRED)

add_executable(PyembedENV_SingleThread PyembedENV.cpp PyembedENV_SingleThread_test.cpp)
target_include_directories(PyembedENV_SingleThread PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(PyembedENV_SingleThread PRIVATE ${PYTHON_LIBRARIES})

add_executable(PyembedENV_MultiThread PyembedENV.cpp PyembedENV_MultiThread_test.cpp)
target_include_directories(PyembedENV_MultiThread PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(PyembedENV_MultiThread PRIVATE ${PYTHON_LIBRARIES})

add_executable(PyembedENV_PlainTest PyembedENV_PlainTest.cpp)
target_include_directories(PyembedENV_PlainTest PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(PyembedENV_PlainTest PRIVATE ${PYTHON_LIBRARIES})



#add_subdirectory(HttpEquipmentServer)
