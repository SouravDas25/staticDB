cmake_minimum_required(VERSION 3.7)
project(staticDB)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/PhysicalLayer/file/File.hpp
        src/PhysicalLayer/file/File_SDL.hpp
        src/PhysicalLayer/file/File_stdio.hpp
        src/PhysicalLayer/file/File_Struct.hpp
        src/PhysicalLayer/Base.h
        src/PhysicalLayer/DataManager.h
        src/PhysicalLayer/DataNode.h
        src/PhysicalLayer/fileObj.h
        src/PhysicalLayer/FreeList.h
        src/PhysicalLayer/FreeNode.h
        src/PhysicalLayer/SDB_Error.h
        src/PhysicalLayer/Utility.h
        src/json.hpp
        src/JsonStore.h
        main.cpp
        src/version.h src/Parser.h src/StringMapper.h src/Database.h src/Compiler.h)

add_executable(staticDB ${SOURCE_FILES})