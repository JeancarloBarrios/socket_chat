# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = /home/ebon1/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/182.4505.18/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/ebon1/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/182.4505.18/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ebon1/Development/OS/socket_chat

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ebon1/Development/OS/socket_chat

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/home/ebon1/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/182.4505.18/bin/cmake/linux/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/home/ebon1/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/182.4505.18/bin/cmake/linux/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/ebon1/Development/OS/socket_chat/CMakeFiles /home/ebon1/Development/OS/socket_chat/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/ebon1/Development/OS/socket_chat/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named server

# Build rule for target.
server: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 server
.PHONY : server

# fast build rule for target.
server/fast:
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/build
.PHONY : server/fast

poll.o: poll.c.o

.PHONY : poll.o

# target to build an object file
poll.c.o:
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/poll.c.o
.PHONY : poll.c.o

poll.i: poll.c.i

.PHONY : poll.i

# target to preprocess a source file
poll.c.i:
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/poll.c.i
.PHONY : poll.c.i

poll.s: poll.c.s

.PHONY : poll.s

# target to generate assembly for a file
poll.c.s:
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/poll.c.s
.PHONY : poll.c.s

server.o: server.c.o

.PHONY : server.o

# target to build an object file
server.c.o:
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/server.c.o
.PHONY : server.c.o

server.i: server.c.i

.PHONY : server.i

# target to preprocess a source file
server.c.i:
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/server.c.i
.PHONY : server.c.i

server.s: server.c.s

.PHONY : server.s

# target to generate assembly for a file
server.c.s:
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/server.c.s
.PHONY : server.c.s

server_@.o: server_@.c.o

.PHONY : server_@.o

# target to build an object file
server_@.c.o:
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/server_@.c.o
.PHONY : server_@.c.o

server_@.i: server_@.c.i

.PHONY : server_@.i

# target to preprocess a source file
server_@.c.i:
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/server_@.c.i
.PHONY : server_@.c.i

server_@.s: server_@.c.s

.PHONY : server_@.s

# target to generate assembly for a file
server_@.c.s:
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/server_@.c.s
.PHONY : server_@.c.s

utils.o: utils.c.o

.PHONY : utils.o

# target to build an object file
utils.c.o:
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/utils.c.o
.PHONY : utils.c.o

utils.i: utils.c.i

.PHONY : utils.i

# target to preprocess a source file
utils.c.i:
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/utils.c.i
.PHONY : utils.c.i

utils.s: utils.c.s

.PHONY : utils.s

# target to generate assembly for a file
utils.c.s:
	$(MAKE) -f CMakeFiles/server.dir/build.make CMakeFiles/server.dir/utils.c.s
.PHONY : utils.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... rebuild_cache"
	@echo "... edit_cache"
	@echo "... server"
	@echo "... poll.o"
	@echo "... poll.i"
	@echo "... poll.s"
	@echo "... server.o"
	@echo "... server.i"
	@echo "... server.s"
	@echo "... server_@.o"
	@echo "... server_@.i"
	@echo "... server_@.s"
	@echo "... utils.o"
	@echo "... utils.i"
	@echo "... utils.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

