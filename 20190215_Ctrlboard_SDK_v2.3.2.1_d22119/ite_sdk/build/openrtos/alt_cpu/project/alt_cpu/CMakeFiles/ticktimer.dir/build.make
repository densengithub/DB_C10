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
CMAKE_COMMAND = C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/tool/bin/cmake.exe

# The command to remove a file.
RM = C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/tool/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/alt_cpu

# Utility rule file for ticktimer.

# Include the progress variables for this target.
include project/alt_cpu/CMakeFiles/ticktimer.dir/progress.make

project/alt_cpu/CMakeFiles/ticktimer: lib/sm32/alt_cpu_ticktimer.o


lib/sm32/alt_cpu_ticktimer.o: ../../../project/alt_cpu/ticktimer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/alt_cpu/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ../../lib/sm32/alt_cpu_ticktimer.o"
	cd C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/alt_cpu/project/alt_cpu && C:/ITEGCC/bin/sm32-elf-gcc.exe -gstabs -O3 -g3 -mno-delay -msoft-div -msoft-float -msoft-mul -mno-ror -mno-cmov -mno-sext -fomit-frame-pointer -fno-short-enums -DNDEBUG -DCFG_CHIP_REV_A1 -DCFG_CHIP_FAMILY=970 -DCFG_RAM_SIZE=0x4000000 -DCFG_MEMDBG_ENABLE -DCFG_WT_SIZE=0 -DCFG_MMAP_SIZE=0 -DCFG_LCD_WIDTH=0 -DCFG_LCD_HEIGHT=0 -DCFG_LCD_PITCH=0 -DCFG_LCD_BPP=0 -DCFG_CMDQ_SIZE=0 -DCFG_FONT_FILENAME="WenQuanYiMicroHeiMono.ttf" -DCFG_PRIVATE_DRIVE="A" -DCFG_PUBLIC_DRIVE="B" -DCFG_TEMP_DRIVE="C" -DCFG_BACKUP_DRIVE="" -DCFG_RTC_DEFAULT_TIMESTAMP=1325376000 -DCFG_INTERNAL_RTC_TIMER= -DCFG_RISC_ENABLE -DCFG_ALT_CPU_ENABLE -DCFG_GPIO_ETHERNET_LINK=-1 -DCFG_I2S_USE_GPIO_MODE_2=y -DCFG_NET_ETHERNET_PHY_ADDR=-1 -DCFG_NET_ETHERNET_COUNT=1 -DCFG_DBG_PRINTBUF_SIZE=0x10000 -DCFG_DBG_PRINTBUF -DCFG_UPGRADE_FILENAME="ITEPKG03.PKG" -DCFG_UPGRADE_FILENAME_LIST="" -DCFG_UPGRADE_ENC_KEY=0 -DCFG_NORMAL_BUFFER_MODE -DCFG_BUILD_RELEASE -DCFG_VERSION_MAJOR=0 -DCFG_VERSION_MINOR=1 -DCFG_VERSION_PATCH=0 -DCFG_VERSION_CUSTOM=0 -DCFG_VERSION_TWEAK= -DCFG_VERSION_MAJOR_STR="0" -DCFG_VERSION_MINOR_STR="1" -DCFG_VERSION_PATCH_STR="0" -DCFG_VERSION_CUSTOM_STR="0" -DCFG_VERSION_TWEAK_STR="" -DCFG_SYSTEM_NAME="ITE Castor3" -DCFG_MANUFACTURER="www.ite.com.tw" -DCFG_PROJECT_NAME="alt_cpu" -D__DYNAMIC_REENT__ -D__OPENRTOS__=1 -D_DEFAULT_SOURCE -D_GNU_SOURCE -D_POSIX_BARRIERS -D_POSIX_C_SOURCE=200809L -D_POSIX_MONOTONIC_CLOCK -D_POSIX_PRIORITY_SCHEDULING=1 -D_POSIX_READER_WRITER_LOCKS -D_POSIX_THREADS -D_POSIX_TIMERS -D_UNIX98_THREAD_MUTEX_ATTRIBUTES -DOSIP_MT -DORTP_BIGENDIAN -DCFG_OPENRTOS_HEAP_SIZE=0 -IC:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/sdk/include -c -o C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/alt_cpu/lib/sm32/alt_cpu_ticktimer.o C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/project/alt_cpu/ticktimer.c

ticktimer: project/alt_cpu/CMakeFiles/ticktimer
ticktimer: lib/sm32/alt_cpu_ticktimer.o
ticktimer: project/alt_cpu/CMakeFiles/ticktimer.dir/build.make

.PHONY : ticktimer

# Rule to build all files generated by this target.
project/alt_cpu/CMakeFiles/ticktimer.dir/build: ticktimer

.PHONY : project/alt_cpu/CMakeFiles/ticktimer.dir/build

project/alt_cpu/CMakeFiles/ticktimer.dir/clean:
	cd C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/alt_cpu/project/alt_cpu && $(CMAKE_COMMAND) -P CMakeFiles/ticktimer.dir/cmake_clean.cmake
.PHONY : project/alt_cpu/CMakeFiles/ticktimer.dir/clean

project/alt_cpu/CMakeFiles/ticktimer.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/project/alt_cpu C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/alt_cpu C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/alt_cpu/project/alt_cpu C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/alt_cpu/project/alt_cpu/CMakeFiles/ticktimer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : project/alt_cpu/CMakeFiles/ticktimer.dir/depend

