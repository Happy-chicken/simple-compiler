# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = D:\CMake\bin\cmake.exe

# The command to remove a file.
RM = D:\CMake\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\code\C_C++\Compiler\src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\code\C_C++\Compiler\build

# Include any dependencies generated for this target.
include syn/CMakeFiles/syn.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include syn/CMakeFiles/syn.dir/compiler_depend.make

# Include the progress variables for this target.
include syn/CMakeFiles/syn.dir/progress.make

# Include the compile flags for this target's objects.
include syn/CMakeFiles/syn.dir/flags.make

syn/CMakeFiles/syn.dir/syntax_analysis.cpp.obj: syn/CMakeFiles/syn.dir/flags.make
syn/CMakeFiles/syn.dir/syntax_analysis.cpp.obj: syn/CMakeFiles/syn.dir/includes_CXX.rsp
syn/CMakeFiles/syn.dir/syntax_analysis.cpp.obj: E:/code/C_C++/Compiler/src/syn/syntax_analysis.cpp
syn/CMakeFiles/syn.dir/syntax_analysis.cpp.obj: syn/CMakeFiles/syn.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\code\C_C++\Compiler\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object syn/CMakeFiles/syn.dir/syntax_analysis.cpp.obj"
	cd /d E:\code\C_C++\Compiler\build\syn && D:\vscode\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT syn/CMakeFiles/syn.dir/syntax_analysis.cpp.obj -MF CMakeFiles\syn.dir\syntax_analysis.cpp.obj.d -o CMakeFiles\syn.dir\syntax_analysis.cpp.obj -c E:\code\C_C++\Compiler\src\syn\syntax_analysis.cpp

syn/CMakeFiles/syn.dir/syntax_analysis.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/syn.dir/syntax_analysis.cpp.i"
	cd /d E:\code\C_C++\Compiler\build\syn && D:\vscode\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\code\C_C++\Compiler\src\syn\syntax_analysis.cpp > CMakeFiles\syn.dir\syntax_analysis.cpp.i

syn/CMakeFiles/syn.dir/syntax_analysis.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/syn.dir/syntax_analysis.cpp.s"
	cd /d E:\code\C_C++\Compiler\build\syn && D:\vscode\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\code\C_C++\Compiler\src\syn\syntax_analysis.cpp -o CMakeFiles\syn.dir\syntax_analysis.cpp.s

# Object files for target syn
syn_OBJECTS = \
"CMakeFiles/syn.dir/syntax_analysis.cpp.obj"

# External object files for target syn
syn_EXTERNAL_OBJECTS =

syn/libsyn.a: syn/CMakeFiles/syn.dir/syntax_analysis.cpp.obj
syn/libsyn.a: syn/CMakeFiles/syn.dir/build.make
syn/libsyn.a: syn/CMakeFiles/syn.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:\code\C_C++\Compiler\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libsyn.a"
	cd /d E:\code\C_C++\Compiler\build\syn && $(CMAKE_COMMAND) -P CMakeFiles\syn.dir\cmake_clean_target.cmake
	cd /d E:\code\C_C++\Compiler\build\syn && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\syn.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
syn/CMakeFiles/syn.dir/build: syn/libsyn.a
.PHONY : syn/CMakeFiles/syn.dir/build

syn/CMakeFiles/syn.dir/clean:
	cd /d E:\code\C_C++\Compiler\build\syn && $(CMAKE_COMMAND) -P CMakeFiles\syn.dir\cmake_clean.cmake
.PHONY : syn/CMakeFiles/syn.dir/clean

syn/CMakeFiles/syn.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\code\C_C++\Compiler\src E:\code\C_C++\Compiler\src\syn E:\code\C_C++\Compiler\build E:\code\C_C++\Compiler\build\syn E:\code\C_C++\Compiler\build\syn\CMakeFiles\syn.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : syn/CMakeFiles/syn.dir/depend

