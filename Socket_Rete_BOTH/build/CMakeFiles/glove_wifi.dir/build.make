# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/cmake-gui

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pacmanpc/Socket_Rete_pc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pacmanpc/Socket_Rete_pc/build

# Include any dependencies generated for this target.
include CMakeFiles/glove_wifi.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/glove_wifi.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/glove_wifi.dir/flags.make

CMakeFiles/glove_wifi.dir/src/main.cpp.o: CMakeFiles/glove_wifi.dir/flags.make
CMakeFiles/glove_wifi.dir/src/main.cpp.o: ../src/main.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pacmanpc/Socket_Rete_pc/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/glove_wifi.dir/src/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/glove_wifi.dir/src/main.cpp.o -c /home/pacmanpc/Socket_Rete_pc/src/main.cpp

CMakeFiles/glove_wifi.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/glove_wifi.dir/src/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pacmanpc/Socket_Rete_pc/src/main.cpp > CMakeFiles/glove_wifi.dir/src/main.cpp.i

CMakeFiles/glove_wifi.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/glove_wifi.dir/src/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pacmanpc/Socket_Rete_pc/src/main.cpp -o CMakeFiles/glove_wifi.dir/src/main.cpp.s

CMakeFiles/glove_wifi.dir/src/main.cpp.o.requires:
.PHONY : CMakeFiles/glove_wifi.dir/src/main.cpp.o.requires

CMakeFiles/glove_wifi.dir/src/main.cpp.o.provides: CMakeFiles/glove_wifi.dir/src/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/glove_wifi.dir/build.make CMakeFiles/glove_wifi.dir/src/main.cpp.o.provides.build
.PHONY : CMakeFiles/glove_wifi.dir/src/main.cpp.o.provides

CMakeFiles/glove_wifi.dir/src/main.cpp.o.provides.build: CMakeFiles/glove_wifi.dir/src/main.cpp.o

CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.o: CMakeFiles/glove_wifi.dir/flags.make
CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.o: ../src/qbmove_communications.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pacmanpc/Socket_Rete_pc/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.o -c /home/pacmanpc/Socket_Rete_pc/src/qbmove_communications.cpp

CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pacmanpc/Socket_Rete_pc/src/qbmove_communications.cpp > CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.i

CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pacmanpc/Socket_Rete_pc/src/qbmove_communications.cpp -o CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.s

CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.o.requires:
.PHONY : CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.o.requires

CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.o.provides: CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.o.requires
	$(MAKE) -f CMakeFiles/glove_wifi.dir/build.make CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.o.provides.build
.PHONY : CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.o.provides

CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.o.provides.build: CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.o

CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.o: CMakeFiles/glove_wifi.dir/flags.make
CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.o: ../src/imuboard_communications.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pacmanpc/Socket_Rete_pc/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.o -c /home/pacmanpc/Socket_Rete_pc/src/imuboard_communications.cpp

CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pacmanpc/Socket_Rete_pc/src/imuboard_communications.cpp > CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.i

CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pacmanpc/Socket_Rete_pc/src/imuboard_communications.cpp -o CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.s

CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.o.requires:
.PHONY : CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.o.requires

CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.o.provides: CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.o.requires
	$(MAKE) -f CMakeFiles/glove_wifi.dir/build.make CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.o.provides.build
.PHONY : CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.o.provides

CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.o.provides.build: CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.o

CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.o: CMakeFiles/glove_wifi.dir/flags.make
CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.o: ../src/wireless_communicator.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pacmanpc/Socket_Rete_pc/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.o -c /home/pacmanpc/Socket_Rete_pc/src/wireless_communicator.cpp

CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/pacmanpc/Socket_Rete_pc/src/wireless_communicator.cpp > CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.i

CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/pacmanpc/Socket_Rete_pc/src/wireless_communicator.cpp -o CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.s

CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.o.requires:
.PHONY : CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.o.requires

CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.o.provides: CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.o.requires
	$(MAKE) -f CMakeFiles/glove_wifi.dir/build.make CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.o.provides.build
.PHONY : CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.o.provides

CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.o.provides.build: CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.o

# Object files for target glove_wifi
glove_wifi_OBJECTS = \
"CMakeFiles/glove_wifi.dir/src/main.cpp.o" \
"CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.o" \
"CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.o" \
"CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.o"

# External object files for target glove_wifi
glove_wifi_EXTERNAL_OBJECTS =

glove_wifi: CMakeFiles/glove_wifi.dir/src/main.cpp.o
glove_wifi: CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.o
glove_wifi: CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.o
glove_wifi: CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.o
glove_wifi: CMakeFiles/glove_wifi.dir/build.make
glove_wifi: CMakeFiles/glove_wifi.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable glove_wifi"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/glove_wifi.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/glove_wifi.dir/build: glove_wifi
.PHONY : CMakeFiles/glove_wifi.dir/build

CMakeFiles/glove_wifi.dir/requires: CMakeFiles/glove_wifi.dir/src/main.cpp.o.requires
CMakeFiles/glove_wifi.dir/requires: CMakeFiles/glove_wifi.dir/src/qbmove_communications.cpp.o.requires
CMakeFiles/glove_wifi.dir/requires: CMakeFiles/glove_wifi.dir/src/imuboard_communications.cpp.o.requires
CMakeFiles/glove_wifi.dir/requires: CMakeFiles/glove_wifi.dir/src/wireless_communicator.cpp.o.requires
.PHONY : CMakeFiles/glove_wifi.dir/requires

CMakeFiles/glove_wifi.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/glove_wifi.dir/cmake_clean.cmake
.PHONY : CMakeFiles/glove_wifi.dir/clean

CMakeFiles/glove_wifi.dir/depend:
	cd /home/pacmanpc/Socket_Rete_pc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pacmanpc/Socket_Rete_pc /home/pacmanpc/Socket_Rete_pc /home/pacmanpc/Socket_Rete_pc/build /home/pacmanpc/Socket_Rete_pc/build /home/pacmanpc/Socket_Rete_pc/build/CMakeFiles/glove_wifi.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/glove_wifi.dir/depend
