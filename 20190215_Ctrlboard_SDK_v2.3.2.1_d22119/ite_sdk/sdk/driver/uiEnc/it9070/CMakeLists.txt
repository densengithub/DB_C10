if (CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(
        -DDEBUG
    )
endif()

INCLUDE_DIRECTORIES(
    .
    ${PROJECT_SOURCE_DIR}/sdk/include/uiEnc
    ${SMEDIA_INC_DIRS}
)

add_definitions(
    -DCFG_ALPHA_ENABLE
)

ADD_LIBRARY(uiEnc STATIC
    uiEnc_hw.c
    mmp_uiEnc.c

    # include file
    ${PROJECT_SOURCE_DIR}/sdk/include/uiEnc/mmp_uiEnc.h
    ${PROJECT_SOURCE_DIR}/sdk/include/uiEnc/uiEnc_err.h
)
