# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git

# Include any dependencies generated for this target.
include CMakeFiles/indexed_color.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/indexed_color.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/indexed_color.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/indexed_color.dir/flags.make

CMakeFiles/indexed_color.dir/progs/demos/indexed_color/idxcol.c.obj: CMakeFiles/indexed_color.dir/flags.make
CMakeFiles/indexed_color.dir/progs/demos/indexed_color/idxcol.c.obj: CMakeFiles/indexed_color.dir/includes_C.rsp
CMakeFiles/indexed_color.dir/progs/demos/indexed_color/idxcol.c.obj: progs/demos/indexed_color/idxcol.c
CMakeFiles/indexed_color.dir/progs/demos/indexed_color/idxcol.c.obj: CMakeFiles/indexed_color.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/indexed_color.dir/progs/demos/indexed_color/idxcol.c.obj"
	C:\MinGW64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/indexed_color.dir/progs/demos/indexed_color/idxcol.c.obj -MF CMakeFiles\indexed_color.dir\progs\demos\indexed_color\idxcol.c.obj.d -o CMakeFiles\indexed_color.dir\progs\demos\indexed_color\idxcol.c.obj -c D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\progs\demos\indexed_color\idxcol.c

CMakeFiles/indexed_color.dir/progs/demos/indexed_color/idxcol.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/indexed_color.dir/progs/demos/indexed_color/idxcol.c.i"
	C:\MinGW64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\progs\demos\indexed_color\idxcol.c > CMakeFiles\indexed_color.dir\progs\demos\indexed_color\idxcol.c.i

CMakeFiles/indexed_color.dir/progs/demos/indexed_color/idxcol.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/indexed_color.dir/progs/demos/indexed_color/idxcol.c.s"
	C:\MinGW64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\progs\demos\indexed_color\idxcol.c -o CMakeFiles\indexed_color.dir\progs\demos\indexed_color\idxcol.c.s

# Object files for target indexed_color
indexed_color_OBJECTS = \
"CMakeFiles/indexed_color.dir/progs/demos/indexed_color/idxcol.c.obj"

# External object files for target indexed_color
indexed_color_EXTERNAL_OBJECTS =

bin/indexed_color.exe: CMakeFiles/indexed_color.dir/progs/demos/indexed_color/idxcol.c.obj
bin/indexed_color.exe: CMakeFiles/indexed_color.dir/build.make
bin/indexed_color.exe: lib/libfreeglut.dll.a
bin/indexed_color.exe: CMakeFiles/indexed_color.dir/linklibs.rsp
bin/indexed_color.exe: CMakeFiles/indexed_color.dir/objects1.rsp
bin/indexed_color.exe: CMakeFiles/indexed_color.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bin\indexed_color.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\indexed_color.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/indexed_color.dir/build: bin/indexed_color.exe
.PHONY : CMakeFiles/indexed_color.dir/build

CMakeFiles/indexed_color.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\indexed_color.dir\cmake_clean.cmake
.PHONY : CMakeFiles/indexed_color.dir/clean

CMakeFiles/indexed_color.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\CMakeFiles\indexed_color.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/indexed_color.dir/depend

