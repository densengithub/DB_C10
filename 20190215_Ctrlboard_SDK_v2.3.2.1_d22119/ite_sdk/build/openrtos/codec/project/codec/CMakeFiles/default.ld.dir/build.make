# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = C:/Maksim_user/DB_C10/20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/tool/bin/cmake.exe

# The command to remove a file.
RM = C:/Maksim_user/DB_C10/20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/tool/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/Maksim_user/DB_C10/20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/Maksim_user/DB_C10/20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/codec

# Utility rule file for default.ld.

# Include the progress variables for this target.
include project/codec/CMakeFiles/default.ld.dir/progress.make

project/codec/CMakeFiles/default.ld: lib/sm32/default.ld


lib/sm32/default.ld: ../../../project/codec/default.lds
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:/Maksim_user/DB_C10/20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/codec/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ../../lib/sm32/default.ld"
	cd C:/Maksim_user/DB_C10/20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/codec/project/codec && sm32-elf-cpp -IC:/Maksim_user/DB_C10/20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/sdk/include -E -P -o C:/Maksim_user/DB_C10/20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/codec/lib/sm32/default.ld C:/Maksim_user/DB_C10/20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/project/codec/default.lds

default.ld: project/codec/CMakeFiles/default.ld
default.ld: lib/sm32/default.ld
default.ld: project/codec/CMakeFiles/default.ld.dir/build.make

.PHONY : default.ld

# Rule to build all files generated by this target.
project/codec/CMakeFiles/default.ld.dir/build: default.ld

.PHONY : project/codec/CMakeFiles/default.ld.dir/build

project/codec/CMakeFiles/default.ld.dir/clean:
	cd C:/Maksim_user/DB_C10/20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/codec/project/codec && $(CMAKE_COMMAND) -P CMakeFiles/default.ld.dir/cmake_clean.cmake
.PHONY : project/codec/CMakeFiles/default.ld.dir/clean

project/codec/CMakeFiles/default.ld.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/Maksim_user/DB_C10/20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk C:/Maksim_user/DB_C10/20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/project/codec C:/Maksim_user/DB_C10/20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/codec C:/Maksim_user/DB_C10/20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/codec/project/codec C:/Maksim_user/DB_C10/20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/codec/project/codec/CMakeFiles/default.ld.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : project/codec/CMakeFiles/default.ld.dir/depend

