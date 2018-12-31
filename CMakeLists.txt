cmake_minimum_required(VERSION 3.7)
project(staticDB)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/Engine/file/File.hpp
        src/Engine/file/File_SDL.hpp
        src/Engine/file/File_stdio.hpp
        src/Engine/file/File_Struct.hpp
        src/Engine/Base.h
        src/Engine/DataManager.h
        src/Engine/DataNode.h
        src/Engine/fileObj.h
        src/Engine/FreeList.h
        src/Engine/FreeNode.h
        src/Engine/SDB_Error.h
        src/Engine/Utility.h
        src/Compiler.h
        src/Database.h
        src/json.hpp
        src/JsonStore.h
        src/Parser.h
        src/StringMapper.h
        src/version.h
        main.cpp
        src/Constants.h
        src/SymbolTree.h
        src/Intefaces/TokenInterface.h
        src/Tokens/BasicTokens.h
        src/CommonUtil.h
        src/Tokens/TokenDeclarations.h src/Structures/SymbolTreeNode.h)

add_executable(staticDB ${SOURCE_FILES})