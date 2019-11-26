include_directories(
    .
    ..
    ${SMEDIA_INC_DIRS}
    ${PROJECT_SOURCE_DIR}/sdk/include
    ${PROJECT_SOURCE_DIR}/sdk/include/capture/capture_960
)

add_library(capture STATIC
    mmp_capture.c
    capture_hw.c
    capture_hw.h
    capture_config.h
    capture_reg.h
    capture_util.h
    capture.c
    capture.h
    cap_util.c
)
if(DEFINED CFG_SENSOR_ENABLE)
    add_library(capture STATIC
        device_sensor.c
	)
elseif(DEFINED CFG_HDMI_RX_ENABLE)
    add_library(capture STATIC
        device_hdmirx.c
	)
else()
    add_library(capture STATIC
        device_modules.c
	)
endif()

if(NOT DEFINED CFG_CAPTURE_DEFINE)
   set(CFG_CAPTURE_DEFINE "DEMO_976_DoorCamera_NT99141.txt")
endif()

configure_file(${PROJECT_SOURCE_DIR}/sdk/target/defcap/${CFG_CAPTURE_DEFINE} ${CMAKE_CURRENT_BINARY_DIR}/defcap_config.h)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
)