if (CFG_CHIP_FAMILY STREQUAL 9910)   #9910 and 9070 use the same isp driver
    add_subdirectory(it9070)
elseif (CFG_CHIP_FAMILY STREQUAL 970 OR CFG_CHIP_FAMILY STREQUAL 960)
    add_subdirectory(it970)
else()
    add_subdirectory(it${CFG_CHIP_FAMILY})
endif()
