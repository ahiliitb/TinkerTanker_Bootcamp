# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.29

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ahilkhanjnv/TinkerTanker/team1-jun24

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ahilkhanjnv/TinkerTanker/team1-jun24/build

# Utility rule file for clean-all.

# Include any custom commands dependencies for this target.
include _deps/zstd-build/CMakeFiles/clean-all.dir/compiler_depend.make

# Include the progress variables for this target.
include _deps/zstd-build/CMakeFiles/clean-all.dir/progress.make

_deps/zstd-build/CMakeFiles/clean-all:
	cd /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/zstd-build && /usr/bin/gmake clean
	cd /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/zstd-build && rm -rf /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/

clean-all: _deps/zstd-build/CMakeFiles/clean-all
clean-all: _deps/zstd-build/CMakeFiles/clean-all.dir/build.make
.PHONY : clean-all

# Rule to build all files generated by this target.
_deps/zstd-build/CMakeFiles/clean-all.dir/build: clean-all
.PHONY : _deps/zstd-build/CMakeFiles/clean-all.dir/build

_deps/zstd-build/CMakeFiles/clean-all.dir/clean:
	cd /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/zstd-build && $(CMAKE_COMMAND) -P CMakeFiles/clean-all.dir/cmake_clean.cmake
.PHONY : _deps/zstd-build/CMakeFiles/clean-all.dir/clean

_deps/zstd-build/CMakeFiles/clean-all.dir/depend:
	cd /home/ahilkhanjnv/TinkerTanker/team1-jun24/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ahilkhanjnv/TinkerTanker/team1-jun24 /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/zstd-src/build/cmake /home/ahilkhanjnv/TinkerTanker/team1-jun24/build /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/zstd-build /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/zstd-build/CMakeFiles/clean-all.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : _deps/zstd-build/CMakeFiles/clean-all.dir/depend

