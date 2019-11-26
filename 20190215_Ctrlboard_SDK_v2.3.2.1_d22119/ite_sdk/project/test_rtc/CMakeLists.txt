add_executable(${CMAKE_PROJECT_NAME}
    test_$ENV{CFG_PLATFORM}.c  
)

add_executable(${CMAKE_PROJECT_NAME}
    test_rtc.c
)

# build
include(${PROJECT_SOURCE_DIR}/sdk/build.cmake)	
