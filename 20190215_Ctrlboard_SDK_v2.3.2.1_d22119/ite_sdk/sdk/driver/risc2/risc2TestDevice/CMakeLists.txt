file(COPY
    ${PROJECT_SOURCE_DIR}/build/$ENV{CFG_BUILDPLATFORM}/risc2/lib/sm32/risc2TestDevice.hex
    DESTINATION ${PROJECT_SOURCE_DIR}/sdk/driver/risc2/risc2TestDevice/.
)

if (DEFINED CFG_RISC2_TEST_DEVICE AND NOT $ENV{CFG_PLATFORM} STREQUAL win32)
    add_library(risc2TestDevice STATIC
        risc2TestDevice.c
    )
endif()