# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2017.1.2\bin\cmake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2017.1.2\bin\cmake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Cloud\OneDrive\Programming\Projects\staticDB

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Cloud\OneDrive\Programming\Projects\staticDB\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/staticDB.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/staticDB.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/staticDB.dir/flags.make

CMakeFiles/staticDB.dir/main.cpp.obj: CMakeFiles/staticDB.dir/flags.make
CMakeFiles/staticDB.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Cloud\OneDrive\Programming\Projects\staticDB\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/staticDB.dir/main.cpp.obj"
	C:\PROGRA~2\Dev-Cpp\MinGW64\bin\G__~1.EXE   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\staticDB.dir\main.cpp.obj -c D:\Cloud\OneDrive\Programming\Projects\staticDB\main.cpp

CMakeFiles/staticDB.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/staticDB.dir/main.cpp.i"
	C:\PROGRA~2\Dev-Cpp\MinGW64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Cloud\OneDrive\Programming\Projects\staticDB\main.cpp > CMakeFiles\staticDB.dir\main.cpp.i

CMakeFiles/staticDB.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/staticDB.dir/main.cpp.s"
	C:\PROGRA~2\Dev-Cpp\MinGW64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Cloud\OneDrive\Programming\Projects\staticDB\main.cpp -o CMakeFiles\staticDB.dir\main.cpp.s

CMakeFiles/staticDB.dir/main.cpp.obj.requires:

.PHONY : CMakeFiles/staticDB.dir/main.cpp.obj.requires

CMakeFiles/staticDB.dir/main.cpp.obj.provides: CMakeFiles/staticDB.dir/main.cpp.obj.requires
	$(MAKE) -f CMakeFiles\staticDB.dir\build.make CMakeFiles/staticDB.dir/main.cpp.obj.provides.build
.PHONY : CMakeFiles/staticDB.dir/main.cpp.obj.provides

CMakeFiles/staticDB.dir/main.cpp.obj.provides.build: CMakeFiles/staticDB.dir/main.cpp.obj


# Object files for target staticDB
staticDB_OBJECTS = \
"CMakeFiles/staticDB.dir/main.cpp.obj"

# External object files for target staticDB
staticDB_EXTERNAL_OBJECTS =

staticDB.exe: CMakeFiles/staticDB.dir/main.cpp.obj
staticDB.exe: CMakeFiles/staticDB.dir/build.make
staticDB.exe: CMakeFiles/staticDB.dir/linklibs.rsp
staticDB.exe: CMakeFiles/staticDB.dir/objects1.rsp
staticDB.exe: CMakeFiles/staticDB.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Cloud\OneDrive\Programming\Projects\staticDB\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable staticDB.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\staticDB.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/staticDB.dir/build: staticDB.exe

.PHONY : CMakeFiles/staticDB.dir/build

CMakeFiles/staticDB.dir/requires: CMakeFiles/staticDB.dir/main.cpp.obj.requires

.PHONY : CMakeFiles/staticDB.dir/requires

CMakeFiles/staticDB.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\staticDB.dir\cmake_clean.cmake
.PHONY : CMakeFiles/staticDB.dir/clean

CMakeFiles/staticDB.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Cloud\OneDrive\Programming\Projects\staticDB D:\Cloud\OneDrive\Programming\Projects\staticDB D:\Cloud\OneDrive\Programming\Projects\staticDB\cmake-build-debug D:\Cloud\OneDrive\Programming\Projects\staticDB\cmake-build-debug D:\Cloud\OneDrive\Programming\Projects\staticDB\cmake-build-debug\CMakeFiles\staticDB.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/staticDB.dir/depend

