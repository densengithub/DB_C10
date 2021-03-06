set(PROJ_NAME ${CMAKE_PROJECT_NAME})

if (DEFINED CFG_BOOTLOADER_ENABLE)
    set(CMAKE_PROJECT_NAME bootloader)
    add_subdirectory(../bootloader ../bootloader)
endif()

set(CMAKE_PROJECT_NAME ${PROJ_NAME})

if (DEFINED CFG_WATCHDOG_ENABLE)
    add_definitions(
        -DCFG_WATCHDOG_ENABLE
    )
endif()

add_definitions(
    -DCFG_HW_VERSION="${CFG_HW_VERSION}"
    )
    
# user app
include(${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/appdash.cmake)


if (DEFINED CFG_PLAY_VIDEO_ON_BOOTING)
    add_definitions(
        -DCFG_PLAY_VIDEO_ON_BOOTING
        -DCFG_BOOT_VIDEO_BGCOLOR=${CFG_BOOT_VIDEO_BGCOLOR}
        -DCFG_BOOT_VIDEO_VOLUME=${CFG_BOOT_VIDEO_VOLUME}
        )
endif()

if (DEFINED CFG_ENABLE_ROTATE)
    add_definitions(
        -DCFG_ENABLE_ROTATE
        )
endif()

if (DEFINED CFG_BOOT_VIDEO_ENABLE_WINDOW_MODE)
    add_definitions(
        -DCFG_BOOT_VIDEO_ENABLE_WINDOW_MODE
        -DCFG_VIDEO_WINDOW_X_POS=${CFG_VIDEO_WINDOW_X_POS}
        -DCFG_VIDEO_WINDOW_Y_POS=${CFG_VIDEO_WINDOW_Y_POS}
        -DCFG_VIDEO_WINDOW_WIDTH=${CFG_VIDEO_WINDOW_WIDTH}
        -DCFG_VIDEO_WINDOW_HEIGHT=${CFG_VIDEO_WINDOW_HEIGHT}
        )
endif()

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/sdk/driver/mmp/include
    ${PROJECT_SOURCE_DIR}/sdk/share/audio_mgr
    ${PROJECT_SOURCE_DIR}/sdk/share/ffmpeg
    ${PROJECT_SOURCE_DIR}/sdk/share/wifi_mgr/
    ${PROJECT_SOURCE_DIR}/sdk/driver/itv

if (CFG_CHIP_FAMILY STREQUAL 9850)
    include_directories(
        ${PROJECT_SOURCE_DIR}/sdk/share/itu
        ${PROJECT_SOURCE_DIR}/sdk/share/itu/it9850
    )
else()
    include_directories(
        ${PROJECT_SOURCE_DIR}/sdk/share/itu
        ${PROJECT_SOURCE_DIR}/sdk/share/itu/it9070
    )
endif()
    )

add_executable(${CMAKE_PROJECT_NAME}
    backup.c
    config.c
    function_table.c   
    main.c    
    scene.c
    upgrade.c 
    external.c
    storage.c 
    bsp_communication.c
    comm_mem_manage.c
    niu_db_data_ite.c
    )

if (DEFINED CFG_NET_ENABLE)
    add_executable(${CMAKE_PROJECT_NAME}
        network.c
        )
endif()

# boot video
if (DEFINED CFG_PLAY_VIDEO_ON_BOOTING)
	add_executable(${CMAKE_PROJECT_NAME}
		video.c
		)
		
	file(COPY
		${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/media/boot/boot.mp4
		DESTINATION ${CMAKE_BINARY_DIR}/data/public/media
		)
endif()

if (DEFINED CFG_PLAY_MJPEG_ON_BOOTING)
	add_executable(${CMAKE_PROJECT_NAME}
		video_mjpeg.c
		)
		
	file(COPY
        ${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/media/boot/${CFG_BOOT_MJPEG_FILE}
		DESTINATION ${CMAKE_BINARY_DIR}/data/public/media
		)
endif()

configure_file(${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/config.ini ${CMAKE_BINARY_DIR}/data/public/config.ini COPYONLY)

# key sound


# itu
file(COPY
    ${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/${CFG_ITU_PATH}/template.itu
    DESTINATION ${CMAKE_BINARY_DIR}/data/private
    )

# file(COPY
#     ${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/${CFG_ITU_PATH}/spcurve.png
#     ${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/${CFG_ITU_PATH}/escurve.png
#     DESTINATION ${CMAKE_BINARY_DIR}/data/private
#     )

# font
file(COPY
    # ${PROJECT_SOURCE_DIR}/data/font/${CFG_FONT_FILENAME}
    ${PROJECT_SOURCE_DIR}/data/font/DINNextLTPro-Medium.ttf
    ${PROJECT_SOURCE_DIR}/data/font/DINNextLTPro-Regular.ttf
    ${PROJECT_SOURCE_DIR}/data/font/DINNextLTPro-BoldItalic.ttf
    ${PROJECT_SOURCE_DIR}/data/font/DINNextLTPro-Bold.ttf
    ${PROJECT_SOURCE_DIR}/data/font/DINNextLTPro-MediumItalic.ttf
    # ${PROJECT_SOURCE_DIR}/data/font/SourceHanSansCN-Medium.ttf
    # ${PROJECT_SOURCE_DIR}/data/font/SourceHanSansCN-Normal.ttf
    DESTINATION ${CMAKE_BINARY_DIR}/data/private/font
    )

# web
# file(GLOB files ${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/web/*.*)
# foreach (src ${files})
#     string(REPLACE "${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/web/" "${CMAKE_BINARY_DIR}/data/private/web/" dest ${src})
#     configure_file(${src} ${dest} COPYONLY)
# endforeach()

# screen saver
# file(COPY
#     ${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/screensaver.jpg
#     DESTINATION ${CMAKE_BINARY_DIR}/data/public
#     )

# codec
if (DEFINED CFG_COMPRESS_AUDIO_PLUGIN)
    if (CFG_CHIP_FAMILY STREQUAL 9850)
    configure_file(${PROJECT_SOURCE_DIR}/build/$ENV{CFG_BUILDPLATFORM}/codec_it9850/lib/sm32/mp3.codecs ${CMAKE_BINARY_DIR}/data/private/codec/mp3.codecs COPYONLY)
    configure_file(${PROJECT_SOURCE_DIR}/build/$ENV{CFG_BUILDPLATFORM}/codec_it9850/lib/sm32/aac.codecs ${CMAKE_BINARY_DIR}/data/private/codec/aac.codecs COPYONLY)    
    configure_file(${PROJECT_SOURCE_DIR}/build/$ENV{CFG_BUILDPLATFORM}/codec_it9850/lib/sm32/wma.codecs ${CMAKE_BINARY_DIR}/data/private/codec/wma.codecs COPYONLY)
    configure_file(${PROJECT_SOURCE_DIR}/build/$ENV{CFG_BUILDPLATFORM}/codec_it9850/lib/sm32/wave.codecs ${CMAKE_BINARY_DIR}/data/private/codec/wave.codecs COPYONLY)
    else()
    configure_file(${PROJECT_SOURCE_DIR}/build/$ENV{CFG_BUILDPLATFORM}/codec/lib/sm32/mp3.codecs ${CMAKE_BINARY_DIR}/data/private/codec/mp3.codecs COPYONLY)
    configure_file(${PROJECT_SOURCE_DIR}/build/$ENV{CFG_BUILDPLATFORM}/codec/lib/sm32/aac.codecs ${CMAKE_BINARY_DIR}/data/private/codec/aac.codecs COPYONLY)    
    configure_file(${PROJECT_SOURCE_DIR}/build/$ENV{CFG_BUILDPLATFORM}/codec/lib/sm32/wma.codecs ${CMAKE_BINARY_DIR}/data/private/codec/wma.codecs COPYONLY)
    configure_file(${PROJECT_SOURCE_DIR}/build/$ENV{CFG_BUILDPLATFORM}/codec/lib/sm32/wave.codecs ${CMAKE_BINARY_DIR}/data/private/codec/wave.codecs COPYONLY)
    endif()
endif()

# media
# file(GLOB files ${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/media/*.*)
# foreach (src ${files})
#    string(REPLACE "${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/media/" "${CMAKE_BINARY_DIR}/data/public/media/" dest ${src})
#    configure_file(${src} ${dest} COPYONLY)
# endforeach()

# if (DEFINED CFG_DYNAMIC_LOAD_TP_MODULE)
#     add_executable(${CMAKE_PROJECT_NAME}
#         load_tp_module.c 
#         )
# endif()

file(COPY
        ${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/media/boot/boot.mp4
		DESTINATION ${CMAKE_BINARY_DIR}/data/public/media/boot/
		)
		
# layer		
include(${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/layer.cmake)

# build
include(${PROJECT_SOURCE_DIR}/sdk/build.cmake)
