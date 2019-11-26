if (CFG_CHIP_FAMILY STREQUAL 9910)    #IT9070 and IT9910 use the same driver
    add_subdirectory(it9070)
elseif (CFG_CHIP_FAMILY STREQUAL 960)    #IT960 and IT970 use the same driver
    add_subdirectory(it970)
else()
    add_subdirectory(it${CFG_CHIP_FAMILY})
endif()
