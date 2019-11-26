if (CFG_CHIP_FAMILY STREQUAL 970 OR CFG_CHIP_FAMILY STREQUAL 960)

if (DEFINED CFG_M2D_MEMPOOL_ENABLE)
    add_definitions(
        -DCFG_M2D_MEMPOOL_ENABLE
        -DCFG_M2D_MEMPOOL_SIZE=${CFG_M2D_MEMPOOL_SIZE}
    )
endif()


    include_directories(
    	.
    	..
        ${SMEDIA_INC_DIRS}
        ${PROJECT_SOURCE_DIR}/sdk/include/gfx
        ${PROJECT_SOURCE_DIR}/sdk/share/nedmalloc
    )

    add_library(m2d STATIC
        it970/driver.c
        it970/surface.c
        it970/hw.c
        it970/gfx_mem.c
        it970/gfx_math.c
    )
elseif (CFG_CHIP_FAMILY STREQUAL 9850)

if (DEFINED CFG_M2D_MEMPOOL_ENABLE)
    add_definitions(
        -DCFG_M2D_MEMPOOL_ENABLE
        -DCFG_M2D_MEMPOOL_SIZE=${CFG_M2D_MEMPOOL_SIZE}
    )
endif()


    include_directories(
    	.
    	..
        ${SMEDIA_INC_DIRS}
        ${PROJECT_SOURCE_DIR}/sdk/include/gfx
        ${PROJECT_SOURCE_DIR}/sdk/share/nedmalloc
    )

    add_library(m2d STATIC
        it9850/driver.c
        it9850/surface.c
        it9850/hw.c
        it9850/gfx_mem.c
        it9850/gfx_math.c
    )
else()	
    INCLUDE_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}/sdk/include/m2d
    )
    
    ADD_LIBRARY(m2d
        it9070/m2d_aafont.c
        it9070/m2d_alphablend.c
        it9070/m2d_bitblt.c
        it9070/m2d_brush.c
        it9070/m2d_clipping.c
        it9070/m2d_dispctxt.c
        it9070/m2d_enable.c
        it9070/m2d_engine.c
        it9070/m2d_font.c
        it9070/m2d_gradientfill.c
        it9070/m2d_line.c
        it9070/m2d_misc.c
        it9070/m2d_pen.c
        it9070/m2d_rotate.c
        it9070/m2d_stretchblt.c
        it9070/m2d_transparentblt.c
        it9070/m2d_util.c
        it9070/m2d_cmp_font.c
        it9070/m2d_cmp_aafont.c
        it9070/iteM2dArrays.c
        it9070/iteM2dVectors.c
        it9070/iteM2dUtility.c
        it9070/iteM2dPath.c
        it9070/iteM2dGeometry.c
    )
endif()
