INCLUDE_DIRECTORIES(
    .
    ..
    ${PROJECT_SOURCE_DIR}/sdk/include/
    ${PROJECT_SOURCE_DIR}/sdk/include/saradc/saradc_${CFG_CHIP_FAMILY}
)

add_library(saradc STATIC
    ./saradc.c
    ./saradc_hw.c
)
