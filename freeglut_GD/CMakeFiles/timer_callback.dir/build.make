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
CMAKE_SOURCE_DIR = C:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git

# Include any dependencies generated for this target.
include CMakeFiles/timer_callback.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/timer_callback.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/timer_callback.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/timer_callback.dir/flags.make

CMakeFiles/timer_callback.dir/progs/demos/timer_callback/timer.c.obj: CMakeFiles/timer_callback.dir/flags.make
CMakeFiles/timer_callback.dir/progs/demos/timer_callback/timer.c.obj: CMakeFiles/timer_callback.dir/includes_C.rsp
CMakeFiles/timer_callback.dir/progs/demos/timer_callback/timer.c.obj: C:/Users/Gerard/Documents/Programmation/Freeglut/freeglut_git/progs/demos/timer_callback/timer.c
CMakeFiles/timer_callback.dir/progs/demos/timer_callback/timer.c.obj: CMakeFiles/timer_callback.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/timer_callback.dir/progs/demos/timer_callback/timer.c.obj"
	C:\MinGW64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/timer_callback.dir/progs/demos/timer_callback/timer.c.obj -MF CMakeFiles\timer_callback.dir\progs\demos\timer_callback\timer.c.obj.d -o CMakeFiles\timer_callback.dir\progs\demos\timer_callback\timer.c.obj -c C:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\progs\demos\timer_callback\timer.c

CMakeFiles/timer_callback.dir/progs/demos/timer_callback/timer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/timer_callback.dir/progs/demos/timer_callback/timer.c.i"
	C:\MinGW64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\progs\demos\timer_callback\timer.c > CMakeFiles\timer_callback.dir\progs\demos\timer_callback\timer.c.i

CMakeFiles/timer_callback.dir/progs/demos/timer_callback/timer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/timer_callback.dir/progs/demos/timer_callback/timer.c.s"
	C:\MinGW64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\progs\demos\timer_callback\timer.c -o CMakeFiles\timer_callback.dir\progs\demos\timer_callback\timer.c.s

# Object files for target timer_callback
timer_callback_OBJECTS = \
"CMakeFiles/timer_callback.dir/progs/demos/timer_callback/timer.c.obj"

# External object files for target timer_callback
timer_callback_EXTERNAL_OBJECTS =

bin/timer_callback.exe: CMakeFiles/timer_callback.dir/progs/demos/timer_callback/timer.c.obj
bin/timer_callback.exe: CMakeFiles/timer_callback.dir/build.make
bin/timer_callback.exe: lib/libfreeglut.dll.a
bin/timer_callback.exe: CMakeFiles/timer_callback.dir/linklibs.rsp
bin/timer_callback.exe: CMakeFiles/timer_callback.dir/objects1.rsp
bin/timer_callback.exe: CMakeFiles/timer_callback.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bin\timer_callback.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\timer_callback.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/timer_callback.dir/build: bin/timer_callback.exe
.PHONY : CMakeFiles/timer_callback.dir/build

CMakeFiles/timer_callback.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\timer_callback.dir\cmake_clean.cmake
.PHONY : CMakeFiles/timer_callback.dir/clean

CMakeFiles/timer_callback.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git C:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\CMakeFiles\timer_callback.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/timer_callback.dir/depend
