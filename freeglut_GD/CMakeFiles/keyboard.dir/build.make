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
include CMakeFiles/keyboard.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/keyboard.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/keyboard.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/keyboard.dir/flags.make

CMakeFiles/keyboard.dir/progs/demos/keyboard/keyboard.c.obj: CMakeFiles/keyboard.dir/flags.make
CMakeFiles/keyboard.dir/progs/demos/keyboard/keyboard.c.obj: CMakeFiles/keyboard.dir/includes_C.rsp
CMakeFiles/keyboard.dir/progs/demos/keyboard/keyboard.c.obj: C:/Users/Gerard/Documents/Programmation/Freeglut/freeglut_git/progs/demos/keyboard/keyboard.c
CMakeFiles/keyboard.dir/progs/demos/keyboard/keyboard.c.obj: CMakeFiles/keyboard.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/keyboard.dir/progs/demos/keyboard/keyboard.c.obj"
	C:\MinGW64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/keyboard.dir/progs/demos/keyboard/keyboard.c.obj -MF CMakeFiles\keyboard.dir\progs\demos\keyboard\keyboard.c.obj.d -o CMakeFiles\keyboard.dir\progs\demos\keyboard\keyboard.c.obj -c C:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\progs\demos\keyboard\keyboard.c

CMakeFiles/keyboard.dir/progs/demos/keyboard/keyboard.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/keyboard.dir/progs/demos/keyboard/keyboard.c.i"
	C:\MinGW64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\progs\demos\keyboard\keyboard.c > CMakeFiles\keyboard.dir\progs\demos\keyboard\keyboard.c.i

CMakeFiles/keyboard.dir/progs/demos/keyboard/keyboard.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/keyboard.dir/progs/demos/keyboard/keyboard.c.s"
	C:\MinGW64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\progs\demos\keyboard\keyboard.c -o CMakeFiles\keyboard.dir\progs\demos\keyboard\keyboard.c.s

# Object files for target keyboard
keyboard_OBJECTS = \
"CMakeFiles/keyboard.dir/progs/demos/keyboard/keyboard.c.obj"

# External object files for target keyboard
keyboard_EXTERNAL_OBJECTS =

bin/keyboard.exe: CMakeFiles/keyboard.dir/progs/demos/keyboard/keyboard.c.obj
bin/keyboard.exe: CMakeFiles/keyboard.dir/build.make
bin/keyboard.exe: lib/libfreeglut.dll.a
bin/keyboard.exe: CMakeFiles/keyboard.dir/linklibs.rsp
bin/keyboard.exe: CMakeFiles/keyboard.dir/objects1.rsp
bin/keyboard.exe: CMakeFiles/keyboard.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bin\keyboard.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\keyboard.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/keyboard.dir/build: bin/keyboard.exe
.PHONY : CMakeFiles/keyboard.dir/build

CMakeFiles/keyboard.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\keyboard.dir\cmake_clean.cmake
.PHONY : CMakeFiles/keyboard.dir/clean

CMakeFiles/keyboard.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git C:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git D:\Users\Gerard\Documents\Programmation\Freeglut\freeglut_git\CMakeFiles\keyboard.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/keyboard.dir/depend

