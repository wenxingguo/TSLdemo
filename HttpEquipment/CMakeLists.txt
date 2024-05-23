project(HttpEquipment)


find_package(PythonLibs REQUIRED)
add_executable(HttpEquipmentTest HttpEquipment.cpp HttpEquipmentTest.cpp mongoose.c ../Equipment.cpp ../PyembedENV/PyembedENV.cpp)
target_include_directories(HttpEquipmentTest PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(HttpEquipmentTest PRIVATE ${PYTHON_LIBRARIES})


add_subdirectory(HttpEquipmentServer)
