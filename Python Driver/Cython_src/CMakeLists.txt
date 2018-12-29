cmake_minimum_required(VERSION 3.7)
project(StaticDB)

set(CMAKE_C_COMPILER:FILEPATH "C:/Program Files (x86)/Dev-Cpp/MinGW64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER:FILEPATH "C:/Program Files (x86)/Dev-Cpp/MinGW64/bin/g++.exe")
set(CMAKE_CXX_STANDARD 11)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ./tmp)
set(EXECUTABLE_OUTPUT_PATH ./tmp)
set(LIBRARY_OUTPUT_PATH ./tmp)


include_directories(C:/Python27/include)

set_target_properties(staticDB PROPERTIES OUTPUT_NAME staticDB.pyd)

add_executable(staticDB)
target_link_libraries(staticDB C:/Python27/libs/libpython27.a)