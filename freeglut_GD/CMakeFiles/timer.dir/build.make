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
include CMakeFiles/timer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/timer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/timer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/timer.dir/flags.make

CMakeFiles/timer.dir/progs/demos/timer/timer.c.obj: CMakeFiles/timer.dir/flags.make
CMakeFiles/timer.dir/progs/demos/timer/timer.c.obj: CMakeFiles/timer.dir/includes_C.rsp
CMakeFiles/timer.dir/progs/demos/timer/timer.c.obj: progs/demos/timer/timer.c
CMakeFiles/timer.dir/progs/demos/timer/timer.c.obj: CMakeFiles/timer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/timer.dir/progs/demos/timer/timer.c.obj"
	C:\MinGW64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/timer.dir/progs/demos/timer/timer.c.obj -MF CMakeFiles\timer.dir\progs\demos\timer\timer.c.obj.d -o CMakeFiles\timer.dir\progs\demos\timer\timer.c.obj -c D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\progs\demos\timer\timer.c

CMakeFiles/timer.dir/progs/demos/timer/timer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/timer.dir/progs/demos/timer/timer.c.i"
	C:\MinGW64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\progs\demos\timer\timer.c > CMakeFiles\timer.dir\progs\demos\timer\timer.c.i

CMakeFiles/timer.dir/progs/demos/timer/timer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/timer.dir/progs/demos/timer/timer.c.s"
	C:\MinGW64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\progs\demos\timer\timer.c -o CMakeFiles\timer.dir\progs\demos\timer\timer.c.s

# Object files for target timer
timer_OBJECTS = \
"CMakeFiles/timer.dir/progs/demos/timer/timer.c.obj"

# External object files for target timer
timer_EXTERNAL_OBJECTS =

bin/timer.exe: CMakeFiles/timer.dir/progs/demos/timer/timer.c.obj
bin/timer.exe: CMakeFiles/timer.dir/build.make
bin/timer.exe: lib/libfreeglut.dll.a
bin/timer.exe: CMakeFiles/timer.dir/linklibs.rsp
bin/timer.exe: CMakeFiles/timer.dir/objects1.rsp
bin/timer.exe: CMakeFiles/timer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bin\timer.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\timer.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/timer.dir/build: bin/timer.exe
.PHONY : CMakeFiles/timer.dir/build

CMakeFiles/timer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\timer.dir\cmake_clean.cmake
.PHONY : CMakeFiles/timer.dir/clean

CMakeFiles/timer.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\CMakeFiles\timer.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/timer.dir/depend

