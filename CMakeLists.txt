cmake_minimum_required(VERSION 3.7)
project(staticDB)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        include/PhysicalLayer/file/File.hpp
        include/PhysicalLayer/file/File_SDL.hpp
        include/PhysicalLayer/file/File_stdio.hpp
        include/PhysicalLayer/file/File_Struct.hpp
        include/PhysicalLayer/Base.h
        include/PhysicalLayer/DataManager.h
        include/PhysicalLayer/DataNode.h
        include/PhysicalLayer/fileObj.h
        include/PhysicalLayer/FreeList.h
        include/PhysicalLayer/FreeNode.h
        include/PhysicalLayer/SDB_Error.h
        include/PhysicalLayer/Utility.h
        include/json.hpp
        include/JsonStore.h
        main.cpp
        version.h include/Parser.h)

add_executable(staticDB ${SOURCE_FILES})