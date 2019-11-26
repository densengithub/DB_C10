if (DEFINED CFG_ITU_ERR)
    add_definitions(
        -DCFG_ITU_ERR
   )
endif()

if (DEFINED CFG_ITU_WARN)
    add_definitions(
        -DCFG_ITU_WARN
   )
endif()

if (DEFINED CFG_ITU_INFO)
    add_definitions(
        -DCFG_ITU_INFO
    )
endif()

if (DEFINED CFG_ITU_DBG)
    add_definitions(
        -DCFG_ITU_DBG
    )
endif()

if (DEFINED CFG_ITU_LOAD)
    add_definitions(
        -DCFG_ITU_LOAD
    )
endif()

if (DEFINED CFG_ITU_UPDATE)
    add_definitions(
        -DCFG_ITU_UPDATE
    )
endif()

if (DEFINED CFG_ITU_DRAW)
    add_definitions(
        -DCFG_ITU_DRAW
    )
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/sdk/share/itu_renderer
)

if (CFG_CHIP_FAMILY STREQUAL 9070)
    include_directories(
        ${PROJECT_SOURCE_DIR}/sdk/share/itu/it9070
    )
elseif (CFG_CHIP_FAMILY STREQUAL 9850)
    include_directories(
        ${PROJECT_SOURCE_DIR}/sdk/share/itu/it9850
    )
else()
    include_directories(
        ${PROJECT_SOURCE_DIR}/sdk/share/itu/it970
    )
endif()

add_library(itu_renderer STATIC
    ${PROJECT_SOURCE_DIR}/sdk/include/ite/itu.h
    itu_base.c
    itu_cfg.h
    itu_surface.c
    itu_utility.c
)

if (DEFINED CFG_VIDEO_ENABLE)
    include_directories(
        ${PROJECT_SOURCE_DIR}/sdk/driver/itv
    )
    if (CFG_CHIP_FAMILY STREQUAL 9070)
        add_library(itu_renderer STATIC
             it9070/itu_framefunc.c
        )
    elseif (CFG_CHIP_FAMILY STREQUAL 9850)
        if (CFG_VIDEO_FRAME_FUN_ENABLE)
             add_library(itu_renderer STATIC
                it9850/itu_framefunc_ff.c
             )
         else()
            add_library(itu_renderer STATIC
                it9850/itu_framefunc.c
            )
        endif()
    else()
        if (CFG_VIDEO_FRAME_FUN_ENABLE)
             add_library(itu_renderer STATIC
                it970/itu_framefunc_ff.c
             )
         else()
            add_library(itu_renderer STATIC
                it970/itu_framefunc.c
            )
        endif()
    endif()
endif()

if (DEFINED CFG_LCD_ENABLE)
    if (CFG_CHIP_FAMILY STREQUAL 9070)
        add_library(itu_renderer STATIC
            it9070/itu_lcd.c
        )
    elseif (CFG_CHIP_FAMILY STREQUAL 9850)
        add_library(itu_renderer STATIC
            it9850/itu_lcd.c
        )
    else()
        add_library(itu_renderer STATIC
            it970/itu_lcd.c
        )
    endif()

endif()

if (DEFINED CFG_M2D_ENABLE)
    if (CFG_CHIP_FAMILY STREQUAL 9070)
        add_library(itu_renderer STATIC
            it9070/itu_m2d.c
            it9070/itu_private.h
        )
    elseif (CFG_CHIP_FAMILY STREQUAL 9850)
        add_library(itu_renderer STATIC
            it9850/itu_m2d.c
            it9850/itu_private.h
        )
    else()
        add_library(itu_renderer STATIC
            it970/itu_m2d.c
            it970/itu_private.h
        )
    endif()
else()
    add_library(itu_renderer STATIC
        itu_sw.c
    )
endif()

if (DEFINED CFG_STNLCD_ENABLE)
    add_library(itu_renderer STATIC
        itu_stnlcd.c
   )
endif()
