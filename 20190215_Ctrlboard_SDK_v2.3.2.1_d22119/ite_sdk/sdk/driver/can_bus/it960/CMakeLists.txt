include_directories(
    .
    ..
    ${SMEDIA_INC_DIRS}
    ${PROJECT_SOURCE_DIR}/sdk/include/can_bus/it960
)

ADD_LIBRARY(can_bus STATIC
    can_api.c
    can_hw.c
)
