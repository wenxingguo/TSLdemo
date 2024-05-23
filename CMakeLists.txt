cmake_minimum_required (VERSION 3.8)

project (TSLDemo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(PyembedENV)

add_subdirectory(HttpEquipment)

add_subdirectory(TSLParserTest)


