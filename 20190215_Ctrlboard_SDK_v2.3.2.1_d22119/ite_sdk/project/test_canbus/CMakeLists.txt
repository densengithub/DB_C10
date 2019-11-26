if (CFG_CHIP_FAMILY STREQUAL 970)
add_executable(${CMAKE_PROJECT_NAME}
    test_$ENV{CFG_PLATFORM}.c
    test_canbus.c
    )
include_directories(
    ${PROJECT_SOURCE_DIR}/sdk/include/
    ${PROJECT_SOURCE_DIR}/sdk/include/can_bus
)
target_link_libraries(${CMAKE_PROJECT_NAME}
    spi
    )
else()
add_executable(${CMAKE_PROJECT_NAME}
    test_$ENV{CFG_PLATFORM}.c
    test_canbus_960.c
    )

include_directories(
    ${PROJECT_SOURCE_DIR}/sdk/include/
)
endif()
# build
include(${PROJECT_SOURCE_DIR}/sdk/build.cmake)
