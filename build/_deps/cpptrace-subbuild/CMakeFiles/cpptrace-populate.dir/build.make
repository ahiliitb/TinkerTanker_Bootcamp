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
CMAKE_SOURCE_DIR = /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild

# Utility rule file for cpptrace-populate.

# Include any custom commands dependencies for this target.
include CMakeFiles/cpptrace-populate.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/cpptrace-populate.dir/progress.make

CMakeFiles/cpptrace-populate: CMakeFiles/cpptrace-populate-complete

CMakeFiles/cpptrace-populate-complete: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-install
CMakeFiles/cpptrace-populate-complete: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-mkdir
CMakeFiles/cpptrace-populate-complete: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-download
CMakeFiles/cpptrace-populate-complete: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-update
CMakeFiles/cpptrace-populate-complete: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-patch
CMakeFiles/cpptrace-populate-complete: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-configure
CMakeFiles/cpptrace-populate-complete: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-build
CMakeFiles/cpptrace-populate-complete: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-install
CMakeFiles/cpptrace-populate-complete: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-test
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'cpptrace-populate'"
	/usr/bin/cmake -E make_directory /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/CMakeFiles
	/usr/bin/cmake -E touch /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/CMakeFiles/cpptrace-populate-complete
	/usr/bin/cmake -E touch /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-done

cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-update:
.PHONY : cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-update

cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-build: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No build step for 'cpptrace-populate'"
	cd /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-build && /usr/bin/cmake -E echo_append
	cd /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-build && /usr/bin/cmake -E touch /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-build

cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-configure: cpptrace-populate-prefix/tmp/cpptrace-populate-cfgcmd.txt
cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-configure: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "No configure step for 'cpptrace-populate'"
	cd /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-build && /usr/bin/cmake -E echo_append
	cd /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-build && /usr/bin/cmake -E touch /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-configure

cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-download: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-gitinfo.txt
cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-download: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (git clone) for 'cpptrace-populate'"
	cd /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps && /usr/bin/cmake -P /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/cpptrace-populate-prefix/tmp/cpptrace-populate-gitclone.cmake
	cd /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps && /usr/bin/cmake -E touch /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-download

cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-install: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-build
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No install step for 'cpptrace-populate'"
	cd /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-build && /usr/bin/cmake -E echo_append
	cd /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-build && /usr/bin/cmake -E touch /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-install

cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'cpptrace-populate'"
	/usr/bin/cmake -Dcfgdir= -P /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/cpptrace-populate-prefix/tmp/cpptrace-populate-mkdirs.cmake
	/usr/bin/cmake -E touch /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-mkdir

cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-patch: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-patch-info.txt
cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-patch: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-update
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'cpptrace-populate'"
	/usr/bin/cmake -E echo_append
	/usr/bin/cmake -E touch /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-patch

cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-update:
.PHONY : cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-update

cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-test: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-install
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No test step for 'cpptrace-populate'"
	cd /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-build && /usr/bin/cmake -E echo_append
	cd /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-build && /usr/bin/cmake -E touch /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-test

cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-update: cpptrace-populate-prefix/tmp/cpptrace-populate-gitupdate.cmake
cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-update: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-update-info.txt
cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-update: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-download
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Performing update step for 'cpptrace-populate'"
	cd /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-src && /usr/bin/cmake -Dcan_fetch=YES -P /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/cpptrace-populate-prefix/tmp/cpptrace-populate-gitupdate.cmake

cpptrace-populate: CMakeFiles/cpptrace-populate
cpptrace-populate: CMakeFiles/cpptrace-populate-complete
cpptrace-populate: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-build
cpptrace-populate: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-configure
cpptrace-populate: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-download
cpptrace-populate: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-install
cpptrace-populate: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-mkdir
cpptrace-populate: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-patch
cpptrace-populate: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-test
cpptrace-populate: cpptrace-populate-prefix/src/cpptrace-populate-stamp/cpptrace-populate-update
cpptrace-populate: CMakeFiles/cpptrace-populate.dir/build.make
.PHONY : cpptrace-populate

# Rule to build all files generated by this target.
CMakeFiles/cpptrace-populate.dir/build: cpptrace-populate
.PHONY : CMakeFiles/cpptrace-populate.dir/build

CMakeFiles/cpptrace-populate.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cpptrace-populate.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cpptrace-populate.dir/clean

CMakeFiles/cpptrace-populate.dir/depend:
	cd /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild /home/ahilkhanjnv/TinkerTanker/team1-jun24/build/_deps/cpptrace-subbuild/CMakeFiles/cpptrace-populate.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/cpptrace-populate.dir/depend
