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
include lex/CMakeFiles/lex.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lex/CMakeFiles/lex.dir/compiler_depend.make

# Include the progress variables for this target.
include lex/CMakeFiles/lex.dir/progress.make

# Include the compile flags for this target's objects.
include lex/CMakeFiles/lex.dir/flags.make

lex/CMakeFiles/lex.dir/lexical_analysis.cpp.obj: lex/CMakeFiles/lex.dir/flags.make
lex/CMakeFiles/lex.dir/lexical_analysis.cpp.obj: lex/CMakeFiles/lex.dir/includes_CXX.rsp
lex/CMakeFiles/lex.dir/lexical_analysis.cpp.obj: E:/code/C_C++/Compiler/src/lex/lexical_analysis.cpp
lex/CMakeFiles/lex.dir/lexical_analysis.cpp.obj: lex/CMakeFiles/lex.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\code\C_C++\Compiler\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lex/CMakeFiles/lex.dir/lexical_analysis.cpp.obj"
	cd /d E:\code\C_C++\Compiler\build\lex && D:\vscode\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lex/CMakeFiles/lex.dir/lexical_analysis.cpp.obj -MF CMakeFiles\lex.dir\lexical_analysis.cpp.obj.d -o CMakeFiles\lex.dir\lexical_analysis.cpp.obj -c E:\code\C_C++\Compiler\src\lex\lexical_analysis.cpp

lex/CMakeFiles/lex.dir/lexical_analysis.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lex.dir/lexical_analysis.cpp.i"
	cd /d E:\code\C_C++\Compiler\build\lex && D:\vscode\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E E:\code\C_C++\Compiler\src\lex\lexical_analysis.cpp > CMakeFiles\lex.dir\lexical_analysis.cpp.i

lex/CMakeFiles/lex.dir/lexical_analysis.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lex.dir/lexical_analysis.cpp.s"
	cd /d E:\code\C_C++\Compiler\build\lex && D:\vscode\mingw64\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S E:\code\C_C++\Compiler\src\lex\lexical_analysis.cpp -o CMakeFiles\lex.dir\lexical_analysis.cpp.s

# Object files for target lex
lex_OBJECTS = \
"CMakeFiles/lex.dir/lexical_analysis.cpp.obj"

# External object files for target lex
lex_EXTERNAL_OBJECTS =

lex/liblex.a: lex/CMakeFiles/lex.dir/lexical_analysis.cpp.obj
lex/liblex.a: lex/CMakeFiles/lex.dir/build.make
lex/liblex.a: lex/CMakeFiles/lex.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:\code\C_C++\Compiler\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library liblex.a"
	cd /d E:\code\C_C++\Compiler\build\lex && $(CMAKE_COMMAND) -P CMakeFiles\lex.dir\cmake_clean_target.cmake
	cd /d E:\code\C_C++\Compiler\build\lex && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\lex.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lex/CMakeFiles/lex.dir/build: lex/liblex.a
.PHONY : lex/CMakeFiles/lex.dir/build

lex/CMakeFiles/lex.dir/clean:
	cd /d E:\code\C_C++\Compiler\build\lex && $(CMAKE_COMMAND) -P CMakeFiles\lex.dir\cmake_clean.cmake
.PHONY : lex/CMakeFiles/lex.dir/clean

lex/CMakeFiles/lex.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\code\C_C++\Compiler\src E:\code\C_C++\Compiler\src\lex E:\code\C_C++\Compiler\build E:\code\C_C++\Compiler\build\lex E:\code\C_C++\Compiler\build\lex\CMakeFiles\lex.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : lex/CMakeFiles/lex.dir/depend

