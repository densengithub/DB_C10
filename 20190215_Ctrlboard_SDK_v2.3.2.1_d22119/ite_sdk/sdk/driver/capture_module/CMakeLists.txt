include_directories(
    .
    ..
    ${SMEDIA_INC_DIRS}
    ${PROJECT_SOURCE_DIR}/sdk/include/capture_module
)

add_library(capture_module STATIC
    capture_module.c 
    adv7180.c
    #cat9883_adc.c
    #cat9883_mcu.c
    #gc0308.c
    #gt5110e1.c
    ${CFG_CAPTURE_MODULE}
)
