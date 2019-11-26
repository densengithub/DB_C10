if (DEFINED CFG_ARM_NOR_WRITER)
    add_definitions(
        -DARM_NOR_WRITER
    )
endif()

include_directories(
    .
    ..
    ${PROJECT_SOURCE_DIR}/sdk/include/
    ${PROJECT_SOURCE_DIR}/sdk/include/nor
    ${SMEDIA_INC_DIRS}
)

add_library(spi STATIC
    mmp_spi.c
    armNorWriterUtility.c
)
