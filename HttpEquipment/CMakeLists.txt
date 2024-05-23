project(HttpEquipment)

add_executable(HttpEquipmentTest HttpEquipment.cpp HttpEquipmentTest.cpp mongoose.c ../Equipment.cpp)

add_subdirectory(HttpEquipmentServer)
