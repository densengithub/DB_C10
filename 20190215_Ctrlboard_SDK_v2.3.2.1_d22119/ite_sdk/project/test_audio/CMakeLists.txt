add_executable(${CMAKE_PROJECT_NAME}
    test_$ENV{CFG_PLATFORM}.c
    audio_ctrl.c
    )

    
if(DEFINED CFG_SIMPLE_IIS_TEST_play)
    add_executable(${CMAKE_PROJECT_NAME}
        test_main_simple.c
    )
elseif(DEFINED CFG_USB_WAV_FILE_TEST)
    add_executable(${CMAKE_PROJECT_NAME}
        gwavhead.c
        test_main_usb.c
    )
elseif(DEFINED CFG_USB_WAV_RECORD_FILE_TEST)
    add_executable(${CMAKE_PROJECT_NAME}
        gwavhead.c
        test_main_usb_record.c
    )
elseif(DEFINED CFG_AUDIOMGR_PLAY_TEST)
    include_directories(
        ${PROJECT_SOURCE_DIR}/sdk/share/audio_mgr
    )
    add_executable(${CMAKE_PROJECT_NAME}
        test_main_audiomgr.c
        sdl/audio.c
        sdl/main_processor_message_queue.c
    )
elseif(DEFINED CFG_AD_REC_DA_PLAY_TEST)
    add_executable(${CMAKE_PROJECT_NAME}
        test_main_adc_rec_play.c
    )
elseif(DEFINED CFG_AD_DA_LOUDER_TEST)
    add_executable(${CMAKE_PROJECT_NAME}
        test_main_louder.c
    )
elseif(DEFINED CFG_HDMIRX_DA_LOUDER_TEST)
    add_executable(${CMAKE_PROJECT_NAME}
        test_main_HDMIlouder.c
    )
    
endif()

# build
include(${PROJECT_SOURCE_DIR}/sdk/build.cmake)
