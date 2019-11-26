if (CFG_CHIP_FAMILY STREQUAL 9910)
    ADD_LIBRARY(video_encoder STATIC
        video_encoder_it9910.c
    )

    TARGET_LINK_LIBRARIES(video_encoder
        encoder
        sensor
        capture
        vp
    )
elseif (CFG_CHIP_FAMILY STREQUAL 970)
    include_directories(
        ${PROJECT_SOURCE_DIR}/sdk/driver/h264_encoder/inc
        ${PROJECT_SOURCE_DIR}/sdk/driver/h264_encoder/h264
        ${PROJECT_SOURCE_DIR}/sdk/driver/h264_encoder/common
        ${PROJECT_SOURCE_DIR}/sdk/driver/h264_encoder/camstab
        ${PROJECT_SOURCE_DIR}/sdk/include/capture
        ${PROJECT_SOURCE_DIR}/sdk/include/sensor
    )

    ADD_LIBRARY(video_encoder STATIC
        video_encoder_it970.c
    )

    TARGET_LINK_LIBRARIES(video_encoder
        h264_encoder
        sensor
        capture
        isp
    )
else()
    ADD_LIBRARY(video_encoder STATIC
        video_encoder.c
    )
endif()
