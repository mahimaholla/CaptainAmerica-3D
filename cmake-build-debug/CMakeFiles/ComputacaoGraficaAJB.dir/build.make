# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/anajbellini/Softwares/CLion/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/anajbellini/Softwares/CLion/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/anajbellini/CLionProjects/ComputacaoGraficaAJB

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/anajbellini/CLionProjects/ComputacaoGraficaAJB/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ComputacaoGraficaAJB.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ComputacaoGraficaAJB.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ComputacaoGraficaAJB.dir/flags.make

CMakeFiles/ComputacaoGraficaAJB.dir/capitao.c.o: CMakeFiles/ComputacaoGraficaAJB.dir/flags.make
CMakeFiles/ComputacaoGraficaAJB.dir/capitao.c.o: ../capitao.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/anajbellini/CLionProjects/ComputacaoGraficaAJB/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/ComputacaoGraficaAJB.dir/capitao.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/ComputacaoGraficaAJB.dir/capitao.c.o   -c /home/anajbellini/CLionProjects/ComputacaoGraficaAJB/capitao.c

CMakeFiles/ComputacaoGraficaAJB.dir/capitao.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/ComputacaoGraficaAJB.dir/capitao.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/anajbellini/CLionProjects/ComputacaoGraficaAJB/capitao.c > CMakeFiles/ComputacaoGraficaAJB.dir/capitao.c.i

CMakeFiles/ComputacaoGraficaAJB.dir/capitao.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/ComputacaoGraficaAJB.dir/capitao.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/anajbellini/CLionProjects/ComputacaoGraficaAJB/capitao.c -o CMakeFiles/ComputacaoGraficaAJB.dir/capitao.c.s

# Object files for target ComputacaoGraficaAJB
ComputacaoGraficaAJB_OBJECTS = \
"CMakeFiles/ComputacaoGraficaAJB.dir/capitao.c.o"

# External object files for target ComputacaoGraficaAJB
ComputacaoGraficaAJB_EXTERNAL_OBJECTS =

ComputacaoGraficaAJB: CMakeFiles/ComputacaoGraficaAJB.dir/capitao.c.o
ComputacaoGraficaAJB: CMakeFiles/ComputacaoGraficaAJB.dir/build.make
ComputacaoGraficaAJB: CMakeFiles/ComputacaoGraficaAJB.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/anajbellini/CLionProjects/ComputacaoGraficaAJB/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable ComputacaoGraficaAJB"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ComputacaoGraficaAJB.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ComputacaoGraficaAJB.dir/build: ComputacaoGraficaAJB

.PHONY : CMakeFiles/ComputacaoGraficaAJB.dir/build

CMakeFiles/ComputacaoGraficaAJB.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ComputacaoGraficaAJB.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ComputacaoGraficaAJB.dir/clean

CMakeFiles/ComputacaoGraficaAJB.dir/depend:
	cd /home/anajbellini/CLionProjects/ComputacaoGraficaAJB/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/anajbellini/CLionProjects/ComputacaoGraficaAJB /home/anajbellini/CLionProjects/ComputacaoGraficaAJB /home/anajbellini/CLionProjects/ComputacaoGraficaAJB/cmake-build-debug /home/anajbellini/CLionProjects/ComputacaoGraficaAJB/cmake-build-debug /home/anajbellini/CLionProjects/ComputacaoGraficaAJB/cmake-build-debug/CMakeFiles/ComputacaoGraficaAJB.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ComputacaoGraficaAJB.dir/depend

