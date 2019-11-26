file(COPY
    ${PROJECT_SOURCE_DIR}/build/$ENV{CFG_BUILDPLATFORM}/risc2/lib/sm32/opuscodec.hex
    DESTINATION ${PROJECT_SOURCE_DIR}/sdk/driver/risc2/opuscodec/.
)

if (DEFINED CFG_RISC2_OPUS_CODEC AND NOT $ENV{CFG_PLATFORM} STREQUAL win32)
    add_library(opuscodec STATIC
        opuscodec.c
    )
endif()