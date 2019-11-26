# Install script for directory: C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/sdk/alt_cpu

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/alt_cpu")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/alt_cpu/sdk/alt_cpu/oledCtrl/cmake_install.cmake")
  include("C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/alt_cpu/sdk/alt_cpu/patternGen/cmake_install.cmake")
  include("C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/alt_cpu/sdk/alt_cpu/rslMaster/cmake_install.cmake")
  include("C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/alt_cpu/sdk/alt_cpu/rslSlave/cmake_install.cmake")
  include("C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/alt_cpu/sdk/alt_cpu/swPwm/cmake_install.cmake")
  include("C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/alt_cpu/sdk/alt_cpu/swSerialPort/cmake_install.cmake")
  include("C:/workspace/IT970_Series/Software_Design_Kit/SDK_R/set20190215_Ctrlboard_SDK_v2.3.2.1_d22119/ite_sdk/build/openrtos/alt_cpu/sdk/alt_cpu/swUart/cmake_install.cmake")

endif()

