project($ENV{CFG_PROJECT})
cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0002 OLD)
cmake_policy(SET CMP0014 OLD)

include(global_func.cmake)

#
# usage example:
#   ITE_LINK_LIBRARY_IF_DEFINED_CFG_BUILD_LIB(mad)
#
#   equals to
#
#   if (DEFINED(CFG_BUILD_MAD)
#       target_link_libraries(${CMAKE_PROJECT_NAME}
#           mad
#       )
#   endif()
#
function(ITE_LINK_LIBRARY_IF_DEFINED_CFG_BUILD_LIB lib)
    string(TOUPPER CFG_BUILD_${lib} cfg)
    #message("CFG:${cfg}")

    if (DEFINED ${cfg})
        #message("LINK:${cfg}")
        target_link_libraries(${CMAKE_PROJECT_NAME}
            ${lib}
        )
    endif()
endfunction()

#
# usage example:
#   ITE_LINK_LIBRARY_IF_DEFINED(CFG_BUILD_AUDIO_PREPROCESS voipdsp eigens)
#
#   equals to
#
#   if (DEFINED(CFG_BUILD_AUDIO_PREPROCESS))
#       target_link_libraries(${CMAKE_PROJECT_NAME}
#           voipdsp
#           eigens
#       )
#   endif()
#
function(ITE_LINK_LIBRARY_IF_DEFINED cfg)
    if (DEFINED ${cfg})
        foreach(lib IN LISTS ARGN)
            #message("LINK:${lib}")
            target_link_libraries(${CMAKE_PROJECT_NAME}
                ${lib}
            )
        endforeach()
    endif()
endfunction()

#
# usage example:
#   ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DOORBELL_INDOOR)
#
#   equals to
#
#   if (DEFINED(CFG_DOORBELL_INDOOR))
#       add_definitions(
#           -DCFG_DOORBELL_INDOOR
#       )
#   endif()
#
function(ITE_ADD_DEFINITIONS_IF_DEFINED def)
    if (DEFINED ${def})
        add_definitions(
            -D${def}
        )
    endif()
endfunction()

if (EXISTS ${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/config.cmake)
    include(project/${CMAKE_PROJECT_NAME}/config.cmake)
else()
    include(build/$ENV{CFG_BUILDPLATFORM}/${CMAKE_PROJECT_NAME}/config.cmake)
endif()

if (EXISTS ${PROJECT_SOURCE_DIR}/project/${CMAKE_PROJECT_NAME}/project.cmake)
    include(project/${CMAKE_PROJECT_NAME}/project.cmake)
endif()

include(sdk/def.cmake)
include($ENV{CFG_PLATFORM}/def.cmake)

if ($ENV{CFG_PROJECT} STREQUAL codec)
    add_subdirectory(project/codec)
elseif ($ENV{CFG_PROJECT} STREQUAL alt_cpu)
    add_subdirectory(project/alt_cpu)
elseif ($ENV{CFG_PROJECT} STREQUAL risc2)
    add_subdirectory(project/risc2)
else()
    add_subdirectory(sdk)
    add_subdirectory($ENV{CFG_PLATFORM})
    add_subdirectory(project)
endif()
