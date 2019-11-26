/*
 * Copyright (c) 2011 ITE Tech.Inc.All Rights Reserved.
 */
/** @file
 * Castor hardware abstraction layer definitions.
 *
 * @author Jim Tan
 * @version 1.0
 */
#ifndef ITH_DEFS_H
#define ITH_DEFS_H

// Base address
#define ITH_MMIO_BASE                   0xC0000000
#define ITH_MMIO_SIZE                   0x1EDFFFFF

#define ITH_HOST_BASE                   0xC0000000
#define ITH_SRAM_AUDIO_BASE             0xC0200000

#define ITH_SRAM_AUDIO_SIZE             8192

#define ITH_SRAM_ISP_BASE               0xC0100000
#define ITH_SRAM_ISP_SIZE               6144

#define ITH_MEM_BASE                    0x00000000
#define ITH_AHB0_BASE                   0xD0000000
#define ITH_DMA_BASE                    0xD0200000
#define ITH_USB0_BASE                   0xD0400000
#define ITH_USB1_BASE                   0xD0500000
#define ITH_OPENVG_BASE                 0xD0600000
#define ITH_NAND_BASE                   0xD0700000
#define ITH_ETHERNET_BASE               0xD0800000
#define ITH_DPU_BASE                    0xD0900000
#define ITH_GPIO_BASE                   0xDE000000
#define ITH_IIC_BASE                    0xDE100000

#define ITH_INTR_BASE                   0xDE200000

#define ITH_RTC_BASE                    0xDE500000
#define ITH_UART0_BASE                  0xDE600000
#define ITH_UART2_BASE                  0xDE600200
#define ITH_UART3_BASE                  0xDE600300

#define ITH_UART1_BASE                  0xDE700000
#define ITH_SSP0_BASE                   0xDE800000
#define ITH_SSP1_BASE                   0xDE900000
#define ITH_TIMER_BASE                  0xDEA00000
#define ITH_WD_BASE                     0xDEB00000
#define ITH_IR_BASE                     0xDEC00000

#define ITH_SD_BASE                     0xDEE00000

#define ITH_MS_BASE                     0xDE400000

#define N01_BITS_MSK                    (0x0001)        //                   1
#define N02_BITS_MSK                    (0x0003)        //                  11
#define N03_BITS_MSK                    (0x0007)        //                 111
#define N04_BITS_MSK                    (0x000F)        //                1111
#define N05_BITS_MSK                    (0x001F)        //              1 1111
#define N06_BITS_MSK                    (0x003F)        //             11 1111
#define N07_BITS_MSK                    (0x007F)        //            111 1111
#define N08_BITS_MSK                    (0x00FF)        //           1111 1111
#define N09_BITS_MSK                    (0x01FF)        //         1 1111 1111
#define N10_BITS_MSK                    (0x03FF)        //        11 1111 1111
#define N11_BITS_MSK                    (0x07FF)        //       111 1111 1111
#define N12_BITS_MSK                    (0x0FFF)        //      1111 1111 1111
#define N13_BITS_MSK                    (0x1FFF)        //    1 1111 1111 1111
#define N14_BITS_MSK                    (0x3FFF)        //   11 1111 1111 1111
#define N15_BITS_MSK                    (0x7FFF)        //  111 1111 1111 1111
#define N16_BITS_MSK                    (0xFFFF)        // 1111 1111 1111 1111

// Chip ID
#define ITH_HTRAP_REG                   0x0000
#define ITH_HTRAP_BIT                   12
#define ITH_HTRAP_MASK                  (N03_BITS_MSK << ITH_HTRAP_BIT)

#define ITH_DEVICE_ID_REG               0x0002
#define ITH_REVISION_ID_REG             0x0004

// CLOCK
#define ITH_PLL_CFG_REG                 0x0000
#define ITH_PLL_CFG_BIT                 7
#define ITH_PLL_CFG_MASK                (N02_BITS_MSK << ITH_PLL_CFG_BIT)

#define ITH_HOST_CLK1_REG               0x0010
#define ITH_BCLK_UPD_BIT                14
#define ITH_BCLK_UPD_MASK               (N01_BITS_MSK << ITH_BCLK_UPD_BIT)
#define ITH_BCLK_SRC_SEL_BIT            11
#define ITH_BCLK_SRC_SEL_MASK           (N03_BITS_MSK << ITH_BCLK_SRC_SEL_BIT)
#define ITH_BCLK_RATIO_BIT              0
#define ITH_BCLK_RATIO_MASK             (N10_BITS_MSK << ITH_BCLK_RATIO_BIT)

#define ITH_MEM_CLK1_REG                0x0014
#define ITH_EN_DIV_MCLK_BIT             15
#define ITH_MCLK_UPD_BIT                14
#define ITH_MCLK_UPD_MASK               (N01_BITS_MSK << ITH_MCLK_UPD_BIT)
#define ITH_MCLK_SRC_SEL_BIT            11
#define ITH_MCLK_SRC_SEL_MASK           (N03_BITS_MSK << ITH_MCLK_SRC_SEL_BIT)
#define ITH_MCLK_RATIO_BIT              0
#define ITH_MCLK_RATIO_MASK             (N10_BITS_MSK << ITH_MCLK_RATIO_BIT)

#define ITH_MEM_CLK2_REG                0x0016

#define ITH_AHB_CLK1_REG                0x0018
#define ITH_NCLK_UPD_BIT                14
#define ITH_NCLK_UPD_MASK               (N01_BITS_MSK << ITH_NCLK_UPD_BIT)
#define ITH_NCLK_SRC_SEL_BIT            11
#define ITH_NCLK_SRC_SEL_MASK           (N03_BITS_MSK << ITH_NCLK_SRC_SEL_BIT)
#define ITH_NCLK_RATIO_BIT              0
#define ITH_NCLK_RATIO_MASK             (N10_BITS_MSK << ITH_NCLK_RATIO_BIT)

#define ITH_AHB_CLK2_REG                0x001A
#define ITH_EN_N1CLK_BIT                3

#define ITH_APB_CLK1_REG                0x001C
#define ITH_WCLK_UPD_BIT                14
#define ITH_WCLK_UPD_MASK               (N01_BITS_MSK << ITH_WCLK_UPD_BIT)
#define ITH_WCLK_SRC_SEL_BIT            11
#define ITH_WCLK_SRC_SEL_MASK           (N03_BITS_MSK << ITH_WCLK_SRC_SEL_BIT)
#define ITH_WCLK_RATIO_BIT              0
#define ITH_WCLK_RATIO_MASK             (N10_BITS_MSK << ITH_WCLK_RATIO_BIT)

#define ITH_APB_CLK2_REG                0x001E
#define ITH_EN_W5CLK_BIT                11
#define ITH_EN_W4CLK_BIT                9
#define ITH_EN_W3CLK_BIT                7
#define ITH_EN_W2CLK_BIT                5
#define ITH_EN_W1CLK_BIT                3
#define ITH_EN_W0CLK_BIT                1

#define ITH_APB_CLK3_REG                0x0020
#define ITH_EN_W11CLK_BIT               11
#define ITH_EN_W10CLK_BIT               9
#define ITH_EN_W9CLK_BIT                7
#define ITH_EN_W8CLK_BIT                5
#define ITH_EN_W7CLK_BIT                3
#define ITH_EN_W6CLK_BIT                1

#define ITH_APB_CLK4_REG                0x0022
#define ITH_RST_SDIP_BIT                8

// CQ 2D CLOCK
#define ITH_CQ_CLK_REG                  0x0026
#define ITH_CQ_RST_BIT                  13
#define ITH_EN_N2CLK_BIT                3
#define ITH_EN_M3CLK_BIT                5

// LCD CLOCK
#define ITH_LCD_CLK1_REG                0x0028
#define ITH_EN_DIV_DCLK_BIT             15

#define ITH_LCD_CLK2_REG                0x002A
#define ITH_LCD_REG_RST_BIT             13
#define ITH_LCD_RST_BIT                 12
#define ITH_EN_DHCLK_BIT                7
#define ITH_EN_M4CLK_BIT                5
#define ITH_EN_LCLK_BIT                 3
#define ITH_EN_DCLK_BIT                 1

// TVE CLOCK
#define ITH_TVE_CLK1_REG                0x002C
#define ITH_EN_DIV_ECLK_BIT             15

#define ITH_TVE_CLK2_REG                0x002E
#define ITH_TVE_YRST_BIT                13
#define ITH_TVE_ERST_BIT                12
#define ITH_EN_YCLK_BIT                 3
#define ITH_EN_ECLK_BIT                 1

// ISP CLOCK
#define ITH_EN_N5CLK_BIT                5

#define ITH_ISP_CLK1_REG                0x0030
#define ITH_EN_DIV_ICLK_BIT             15
#define ITH_ICLK_SRC_SEL_BIT            2
#define ITH_ICLK_SRC_SEL_MASK           (N03_BITS_MSK << ITH_ICLK_SRC_SEL_BIT)

#define ITH_ISP_CLK2_REG                0x0032
#define ITH_ISPQ_RST_BIT                14
#define ITH_ISP_REG_RST_BIT             13
#define ITH_ISP_RST_BIT                 12
#define ITH_EN_M5CLK_BIT                3
#define ITH_EN_ICLK_BIT                 1
#define ITH_EN_I1CLK_BIT                0

// VIDEO CLOCK
#define ITH_VIDEO_CLK1_REG              0x0034
#define ITH_EN_DIV_XCLK_BIT             15

#define ITH_VIDEO_CLK2_REG              0x0036
#define ITH_JPEG_REG_RST_BIT            15
#define ITH_JPEG_RST_BIT                14
#define ITH_VIDEO_REG_RST_BIT           13
#define ITH_VIDEO_RST_BIT               12
#define ITH_EN_M7CLK_BIT                11
#define ITH_EN_XCLK_BIT                 1

// JPEG CLOCK
#define ITH_JPEG_CLK_REG                0x0038
#define ITH_EN_DIV_JCLK_BIT             15
#define ITH_EN_JCLK_BIT                 11

// AUDIO CLOCK
#define ITH_AUDIO_AMCLK_REG             (0x003A)
#define ITH_AUDIO_EN_AMCLK_DIV_POS      (15u)       // 0: disable; 1: enable;
#define ITH_AUDIO_EN_AMCLK_DIV_MSK      (N01_BITS_MSK << ITH_AUDIO_EN_AMCLK_DIV_POS)
#define ITH_AUDIO_SEL_AMCLK_SRC_POS     (12u)       // 0: from PLL1 output1 (default)
                                                    // 1: from PLL1 output2
                                                    // 2: from PLL2 output1
                                                    // 3: from PLL2 output2
                                                    // 4: from PLL3 output1
                                                    // 5: from PLL3 output2
                                                    // 6: from CKSYS (12MHz)
                                                    // 7: from Ring OSC (200KHz)
#define ITH_AUDIO_SEL_AMCLK_SRC_MSK     (N03_BITS_MSK << ITH_AUDIO_SEL_AMCLK_SRC_POS)
#define ITH_AUDIO_AMCLK_DIV_RATIO_POS   (0u)    // 0: [1:1] ~ 127: [128:1]
#define ITH_AUDIO_AMCLK_DIV_RATIO_MSK   (N07_BITS_MSK << ITH_AUDIO_AMCLK_DIV_RATIO_POS)

#define ITH_AUDIO_ZCLK_REG              (0x003C)
#define ITH_AUDIO_EN_ZCLK_DIV_POS       (15u)   // 0: disable; 1: enable;
#define ITH_AUDIO_EN_ZCLK_DIV_MSK       (N01_BITS_MSK << ITH_AUDIO_EN_ZCLK_DIV_POS)
#define ITH_AUDIO_ZCLK_DIV_RATIO_POS    (0u)    // 0: [1:1] ~ 1023: [1024:1]
#define ITH_AUDIO_ZCLK_DIV_RATIO_MSK    (N10_BITS_MSK << ITH_AUDIO_ZCLK_DIV_RATIO_POS)

#define ITH_AUDIO_CLK_REG               (0x003E)
#define ITH_AUDIO_RESET_I2S_POS         (12u)   // 0: normal; 1: reset
#define ITH_AUDIO_RESET_I2S_MSK         (N01_BITS_MSK << ITH_AUDIO_RESET_I2S_POS)
#define ITH_AUDIO_EN_M9CLK_POS          (7u)    // 0: disable; 1: enable
#define ITH_AUDIO_EN_M9CLK_MSK          (N01_BITS_MSK << ITH_AUDIO_EN_M9CLK_POS)
#define ITH_AUDIO_EN_M8CLK_POS          (5u)    // 0: disable; 1: enable
#define ITH_AUDIO_EN_M8CLK_MSK          (N01_BITS_MSK << ITH_AUDIO_EN_M8CLK_POS)
#define ITH_AUDIO_EN_ZCLK_POS           (3u)    // 0: disable; 1: enable
#define ITH_AUDIO_EN_ZCLK_MSK           (N01_BITS_MSK << ITH_AUDIO_EN_ZCLK_POS)
#define ITH_AUDIO_EN_AMCLK_POS          (1u)    // 0: disable; 1: enable
#define ITH_AUDIO_EN_AMCLK_MSK          (N01_BITS_MSK << ITH_AUDIO_EN_AMCLK_POS)

// USB CLOCK
#define ITH_USB_CLK_REG                 0x0046
#define ITH_EN_N6CLK_BIT                3
#define ITH_EN_M11CLK_BIT               1

// UIENC CLOCK
#define ITH_UIENC_CLK_REG               0x004A
#define ITH_EN_UIENC_RST_BIT            12
#define ITH_EN_M14CLK_BIT               1

// LCD CLOCK REG4
#define ITH_LCD_CLK_REG4                0x0054
#define ITH_EN_DHCLK_DIV_BIT            15

// CAP CLOCK
#define ITH_CAP_CLK_REG                 0x0062
#define ITH_EN_CAPC_RST_BIT             3
#define ITH_EN_CAP_REG_RST_BIT          2
#define ITH_EN_M17CLK_BIT               0

// SENSOR CLOCK
#define ITH_SENSOR_CLK_REG              0x0064
#define ITH_SENSOR_CLK_DIV_EN_BIT       11
#define ITH_SENSOR_CLK_DIV_EN_MASK      (N01_BITS_MSK << ITH_SENSOR_CLK_DIV_EN_BIT)
#define ITH_SENSOR_CLK_SEL_BIT          8
#define ITH_SENSOR_CLK_SEL_MASK         (N03_BITS_MSK << ITH_SENSOR_CLK_SEL_BIT)
#define ITH_SENSOR_CLK_RATIO_BIT        0
#define ITH_SENSOR_CLK_RATIO_MASK       (N04_BITS_MSK << ITH_SENSOR_CLK_RATIO_BIT)

// ARM CLOCK
#define ITH_ARM_CLK1_REG                0x004C
#define ITH_FCLK_UPD_BIT                14
#define ITH_FCLK_UPD_MASK               (N01_BITS_MSK << ITH_FCLK_UPD_BIT)
#define ITH_FCLK_SRC_SEL_BIT            11
#define ITH_FCLK_SRC_SEL_MASK           (N03_BITS_MSK << ITH_FCLK_SRC_SEL_BIT)
#define ITH_FCLK_RATIO_BIT              0
#define ITH_FCLK_RATIO_MASK             (N10_BITS_MSK << ITH_FCLK_RATIO_BIT)

// OPENVG CLOCK
#define ITH_OPVG_CLK1_REG               0x0056
#define ITH_OPVG_RST_BIT                15
#define ITH_EN_N9CLK_BIT                7
#define ITH_EN_M16CLK_BIT               5
#define ITH_EN_DG_M16CLK_BIT            4
#define ITH_EN_VCLK_BIT                 3
#define ITH_EN_DG_VCLK_BIT              2
#define ITH_EN_TCLK_BIT                 1
#define ITH_EN_DG_TCLK_BIT              0

#define ITH_OPVG_CLK2_REG               0x0058
#define ITH_EN_DIV_TCLK_BIT             15

#define ITH_OPVG_CLK3_REG               0x005A
#define ITH_EN_DIV_VCLK_BIT             15

// DFT
#define ITH_GEN_DFT1_REG                0x0072

// PLL
#define ITH_PLL1_SET1_REG               0x00A0
#define ITH_PLL1_SET2_REG               0x00A2

#define ITH_PLL1_SET3_REG               0x00A4
#define ITH_PLL1_PWR_STATE_POS          (15u)          // 0: power on; 1: power off
#define ITH_PLL1_PWR_STATE_MSK          (N01_BITS_MSK << ITH_PLL1_PWR_STATE_POS)
#define ITH_PLL1_BYPASS_BIT             14
#define ITH_PLL1_START_BIT              13
#define ITH_PLL1_UPDATE_BIT             12
#define ITH_PLL1_UPDATE_MASK            (N01_BITS_MSK << ITH_PLL1_UPDATE_BIT)

#define ITH_PLL1_SET4_REG               0x00A6

#define ITH_PLL1_SET5_REG               0x00A8
#define ITH_PLL1_PD_LEVEL_BIT           0
#define ITH_PLL1_PD_LEVEL_MASK          (N09_BITS_MSK << ITH_PLL1_PD_LEVEL_BIT)

#define ITH_PLL1_SET6_REG               0x00AA
#define ITH_PLL1_SP_EN_BIT              15
#define ITH_PLL1_SP_STATE_BIT           14
#define ITH_PLL1_SP_MODE_BIT            12
#define ITH_PLL1_SP_MODE_MASK           (N02_BITS_MSK << ITH_PLL1_SP_MODE_BIT)
#define ITH_PLL1_SP_STEPX_BIT           6
#define ITH_PLL1_SP_STEPX_MASK          (N06_BITS_MSK << ITH_PLL1_SP_STEPX_BIT)
#define ITH_PLL1_SP_STEPY_BIT           0
#define ITH_PLL1_SP_STEPY_MASK          (N06_BITS_MSK << ITH_PLL1_SP_STEPY_BIT)

#define ITH_PLL2_SET1_REG               0x00B0
#define ITH_PLL2_SET2_REG               0x00B2
#define ITH_PLL2_SET3_REG               0x00B4
#define ITH_PLL2_PWR_STATE_POS          (15u)          // 0: power on; 1: power off
#define ITH_PLL2_PWR_STATE_MSK          (N01_BITS_MSK << ITH_PLL2_PWR_STATE_POS)
#define ITH_PLL2_BYPASS_BIT             (14u)
#define ITH_PLL2_START_BIT              (13u)
#define ITH_PLL2_UPDATE_BIT             (12u)
#define ITH_PLL2_UPDATE_MASK            (N01_BITS_MSK << ITH_PLL2_UPDATE_BIT)
#define ITH_PLL2_SET4_REG               0x00B6

#define ITH_PLL3_SET1_REG               0x00C0
#define ITH_PLL3_SET2_REG               0x00C2
#define ITH_PLL3_SET3_REG               0x00C4
#define ITH_PLL3_PWR_STATE_POS          (15u)
#define ITH_PLL3_PWR_STATE_MSK          (N01_BITS_MSK << ITH_PLL3_PWR_STATE_POS)
#define ITH_PLL3_BYPASS_BIT             (14u)
#define ITH_PLL3_START_BIT              (13u)
#define ITH_PLL3_UPDATE_BIT             (12u)
#define ITH_PLL3_UPDATE_MASK            (N01_BITS_MSK << ITH_PLL3_UPDATE_BIT)

#define ITH_PLL3_SET4_REG               0x00C6

#define ITH_PLL_GEN_SET_REG             (0x00D2)

#define ITH_PLL_LOCK_STATUS_REG         (0x00D4)
#define ITH_PLL3_LOCK_POS               (2u)    // 0: unlocked; 1: locked
#define ITH_PLL3_LOCK_MSK               (N01_BITS_MSK << ITH_PLL3_LOCK_POS)
#define ITH_PLL2_LOCK_POS               (1u)    // 0: unlocked; 1: locked
#define ITH_PLL2_LOCK_MSK               (N01_BITS_MSK << ITH_PLL2_LOCK_POS)
#define ITH_PLL1_LOCK_POS               (0u)    // 0: unlocked; 1: locked
#define ITH_PLL1_LOCK_MSK               (N01_BITS_MSK << ITH_PLL1_LOCK_POS)

// HOST
#define ITH_HOST_BUS_CTRL1_REG          0x0200

#define ITH_EN_MMIO_REG                 0x0202
#define ITH_EN_MMIO_BIT                 0
#define ITH_EN_MMIO_MASK                (N15_BITS_MSK << ITH_EN_MMIO_BIT)

#define ITH_EN_I2S_MMIO_BIT             0
#define ITH_EN_JPEG_MMIO_BIT            1
#define ITH_EN_MPEG_MMIO_BIT            2
#define ITH_EN_VIDEO_MMIO_BIT           3
#define ITH_EN_TVE_MMIO_BIT             4
#define ITH_EN_TSI0_MMIO_BIT            5
#define ITH_EN_TSI1_MMIO_BIT            6
#define ITH_EN_CQ_MMIO_BIT              7
#define ITH_EN_RISC0_MMIO_BIT           8
#define ITH_EN_2D_MMIO_BIT              9
#define ITH_EN_FPC_MMIO_BIT             10
#define ITH_EN_NFC_MMIO_BIT             11
#define ITH_EN_USB_MMIO_BIT             12
#define ITH_EN_ISP_MMIO_BIT             13
#define ITH_EN_LCD_MMIO_BIT             14
#define ITH_EN_CAP_MMIO_BIT             15

#define ITH_HOST_BUS_CTRL18_REG         0x0222

// AHB0
#define ITH_AHB0_CTRL_REG               0x88
#define ITH_AHB0_CTRL_REG_DEFAULT       0x00100F01

// MEM
#define ITH_MEM_CTRL5_REG               0x0308
#define ITH_SELF_REFRESH_BIT            12

#define ITH_MEM_CTRL6_REG               0x030A
#define ITH_B0_RQ_SEV_STOP_BIT          15
#define ITH_B0_RQ_EMPTY_BIT             14
#define ITH_B0_REF_PERIOD_CHG_BIT       13

// MEM DEBUG
#define ITH_MEMDBG0_BASE_REG            0x03C8
#define ITH_MEMDBG1_BASE_REG            0x03D0

#define ITH_MEMDBG_TOP_ADDR_LO_REG      0x00

#define ITH_MEMDBG_TOP_ADDR_HI_REG      0x02
#define ITH_MEMDBG_TOP_ADDR_HI_BIT      0
#define ITH_MEMDBG_TOP_ADDR_HI_MASK     (N10_BITS_MSK << ITH_MEMDBG_TOP_ADDR_HI_BIT)

#define ITH_MEMDBG_BOT_ADDR_LO_REG      0x04

#define ITH_MEMDBG_BOT_ADDR_HI_REG      0x06
#define ITH_MEMDBG_BOT_ADDR_HI_BIT      0
#define ITH_MEMDBG_BOT_ADDR_HI_MASK     (N10_BITS_MSK << ITH_MEMDBG_BOT_ADDR_HI_BIT)

#define ITH_MEMDBG_EN_REG               0x06
#define ITH_MEMDBG_EN_BIT               15

#define ITH_MEMDBG_MODE_REG             0x06
#define ITH_MEMDBG_MODE_BIT             13
#define ITH_MEMDBG_MODE_MASK            (N02_BITS_MSK << ITH_MEMDBG_MODE_BIT)

#define ITH_MEMDBG_FLAG_REG             0x03D8
#define ITH_MEMDBG_FLAG_STATUS0_REG     13
#define ITH_MEMDBG_FLAG_STATUS0_BIT     7
#define ITH_MEMDBG_FLAG_STATUS0_MASK    (N06_BITS_MSK << ITH_MEMDBG_FLAG_STATUS0_BIT)
#define ITH_MEMDBG_FLAG_STATUS1_REG     6
#define ITH_MEMDBG_FLAG_STATUS1_BIT     0
#define ITH_MEMDBG_FLAG_STATUS1_MASK    (N06_BITS_MSK << ITH_MEMDBG_FLAG_STATUS1_BIT)

// MEM STAT
#define ITH_MEMSTAT_SERV_PERIOD_REG     0x03E0

#define ITH_MEMSTAT_SERV_CR_REG         0x03E2
#define ITH_MEMSTAT_SERV_EN_BIT         15
#define ITH_MEMSTAT_SERV_DONE_BIT       12
#define ITH_MEMSTAT_SERV1_RQ_BIT        5
#define ITH_MEMSTAT_SERV1_RQ_MASK       (N05_BITS_MSK << ITH_MEMSTAT_SERV1_RQ_BIT)
#define ITH_MEMSTAT_SERV0_RQ_BIT        0
#define ITH_MEMSTAT_SERV0_RQ_MASK       (N05_BITS_MSK << ITH_MEMSTAT_SERV0_RQ_BIT)

#define ITH_MEMSTAT_SERVALL_NUM_LO_REG  0x03E4
#define ITH_MEMSTAT_SERVALL_NUM_HI_REG  0x03E6
#define ITH_MEMSTAT_SERV0_NUM_LO_REG    0x03E8
#define ITH_MEMSTAT_SERV0_NUM_HI_REG    0x03EA
#define ITH_MEMSTAT_SERV1_NUM_LO_REG    0x03EC
#define ITH_MEMSTAT_SERV1_NUM_HI_REG    0x03EE
#define ITH_MEMSTAT_SERVALL_CNT_LO_REG  0x03F0
#define ITH_MEMSTAT_SERVALL_CNT_HI_REG  0x03F2
#define ITH_MEMSTAT_SERVTIME_CNT_LO_REG 0x03F4
#define ITH_MEMSTAT_SERVTIME_CNT_HI_REG 0x03F6

#define ITH_MEMSTAT_INTR_REG            0x03F8
#define ITH_MEMSTAT_INTR_WP1_BIT        10
#define ITH_MEMSTAT_INTR_WP0_BIT        9
#define ITH_MEMSTAT_INTR_EN_BIT         8
#define ITH_MEMSTAT_INTR_WC_BIT         0

// USB
#define ITH_USB0_PHY_CTRL_REG           0x0904
#define ITH_USB0_PHY_OSC_OUT_EN_BIT     1
#define ITH_USB0_PHY_PLL_ALIV_BIT       0

#define ITH_USB1_PHY_CTRL_REG           0x090C
#define ITH_USB1_PHY_OSC_OUT_EN_BIT     1
#define ITH_USB1_PHY_PLL_ALIV_BIT       0
#define ITH_USB_HC_MISC_REG             0x40
#define ITH_USB_HOSTPHY_SUSPEND_BIT     6

// TV ENCODER
#define ITH_TVE_NTSC_PAL_SYS_REG        (0x1A00)

#define ITH_TVE_DAC_OUT_REG             (0x1A02)
#define ITH_TVE_B0DAC1OUT_BIT           0
#define ITH_TVE_B0DAC1OUT_MASK          (N04_BITS_MSK << ITH_TVE_B0DAC1OUT_BIT)
#define ITH_TVE_B0DAC2OUT_BIT           4
#define ITH_TVE_B0DAC2OUT_MASK          (N04_BITS_MSK << ITH_TVE_B0DAC2OUT_BIT)
#define ITH_TVE_B0DAC3OUT_BIT           8
#define ITH_TVE_B0DAC3OUT_MASK          (N04_BITS_MSK << ITH_TVE_B0DAC3OUT_BIT)
#define ITH_TVE_B0DAC4OUT_BIT           12
#define ITH_TVE_B0DAC4OUT_MASK          (N04_BITS_MSK << ITH_TVE_B0DAC4OUT_BIT)

#define ITH_TVE_B0TCYCSTEP_REG          (0x1CEC)

// FPC & STC
#define ITH_STC_CTRL_REG                (0x1D00)

#define ITH_STC_VIDEO_OUT_FREQ_REG      (0x1D02)
#define ITH_STC_VIDEO_OUT_FREQ_INT_BIT  2

#define ITH_STC_TIMER_INTR_NUM_REG      (0x1D04)
#define ITH_STC_TIMER_INTR_NUM_BIT      0
#define ITH_STC_TIMER_INTR_NUM_MASK     (N14_BITS_MSK << ITH_STC_TIMER_INTR_NUM_BIT)

#define ITH_FPC_INTR_SETTING_REG        (0x1D22)
#define ITH_STC_BASECNT_LO_REG          (0x1D24)
#define ITH_STC_BASECNT_HI_REG          (0x1D26)

#define ITH_STC_EXTCNT_REG              (0x1D28)
#define ITH_STC_EXTCNT_BIT              0
#define ITH_STC_EXTCNT_MASK             (N09_BITS_MSK << ITH_STC_EXTCNT_BIT)

#define ITH_STC_TIMERCNT_REG            (0x1D2A)
#define ITH_STC_TIMERCNT_BIT            0
#define ITH_STC_TIMERCNT_MASK           (N14_BITS_MSK << ITH_STC_TIMERCNT_BIT)

#define ITH_FPC_INTR_READBACK_REG       (0x1D32)

// LCD
#define ITH_LCD_BASEA_HI_REG            0x1114
#define ITH_LCD_BASEA_HI_BIT            0
#define ITH_LCD_BASEA_HI_MASK           (0xFFFF << ITH_LCD_BASEA_HI_BIT)

#define ITH_LCD_BASEA_LO_REG            0x1112
#define ITH_LCD_BASEA_LO_BIT            0
#define ITH_LCD_BASEA_LO_MASK           (0xFFF8 << ITH_LCD_BASEA_LO_BIT)

#define ITH_LCD_WIDTH_REG               0x110C
#define ITH_LCD_WIDTH_BIT               0
#define ITH_LCD_WIDTH_MASK              (N12_BITS_MSK << ITH_LCD_WIDTH_BIT)

#define ITH_LCD_HEIGHT_REG              0x110E
#define ITH_LCD_HEIGHT_BIT              0
#define ITH_LCD_HEIGHT_MASK             (N12_BITS_MSK << ITH_LCD_HEIGHT_BIT)

#define ITH_LCD_PITCH_REG               0x1110
#define ITH_LCD_PITCH_BIT               0
#define ITH_LCD_PITCH_MASK              (N14_BITS_MSK << ITH_LCD_HEIGHT_BIT)

// Command queue
#define ITH_CMDQ_BASE_LO_REG            0x1400
#define ITH_CMDQ_BASE_LO_BIT            0
#define ITH_CMDQ_BASE_LO_MASK           (0xFFF8 << ITH_CMDQ_BASE_LO_BIT)

#define ITH_CMDQ_BASE_HI_REG            0x1402
#define ITH_CMDQ_BASE_HI_BIT            0
#define ITH_CMDQ_BASE_HI_MASK           (N13_BITS_MSK << ITH_CMDQ_BASE_HI_BIT)

#define ITH_CMDQ_LEN_REG                0x1404
#define ITH_CMDQ_LEN_BIT                0
#define ITH_CMDQ_LEN_MASK               (N09_BITS_MSK << ITH_CMDQ_LEN_BIT)

#define ITH_CMDQ_WR_LO_REG              0x1406
#define ITH_CMDQ_WR_LO_BIT              0
#define ITH_CMDQ_WR_LO_MASK             (0xFFF8 << ITH_CMDQ_WR_LO_BIT)

#define ITH_CMDQ_WR_HI_REG              0x1408
#define ITH_CMDQ_WR_HI_BIT              0
#define ITH_CMDQ_WR_HI_MASK             (N03_BITS_MSK << ITH_CMDQ_WR_HI_BIT)

#define ITH_CMDQ_FLIPIDX_REG            0x140A
#define ITH_CMDQ_FLIPIDX_BIT            0
#define ITH_CMDQ_FLIPIDX_MASK           (N02_BITS_MSK << ITH_CMDQ_FLIPIDX_BIT)

#define ITH_CMDQ_CR_REG                 0x140C
#define ITH_CMDQ_CR_BIT                 0
#define ITH_CMDQ_CR_MASK                (N13_BITS_MSK << ITH_CMDQ_CR_BIT)

#define ITH_CMDQ_ENWAITISPBUSYINAHB_BIT 15
#define ITH_CMDQ_ENISPCMDTOAHB_BIT      14
#define ITH_CMDQ_ENINTR_BIT             13
#define ITH_CMDQ_ENBIGENDIAN_BIT        12
#define ITH_CMDQ_ENCMDERRSTOP_BIT       3
#define ITH_CMDQ_TURBOFLIP_BIT          2
#define ITH_CMDQ_FLIPBUFMODE_BIT        1

#define ITH_CMDQ_RD_LO_REG              0x140E
#define ITH_CMDQ_RD_LO_BIT              0
#define ITH_CMDQ_RD_LO_MASK             (0xFFF8 << ITH_CMDQ_RD_LO_BIT)

#define ITH_CMDQ_RD_HI_REG              0x1410
#define ITH_CMDQ_RD_HI_BIT              0
#define ITH_CMDQ_RD_HI_MASK             (N03_BITS_MSK << ITH_CMDQ_RD_HI_BIT)

#define ITH_CMDQ_SR1_REG                0x1412
#define ITH_CMDQ_SR1_BIT                0
#define ITH_CMDQ_SR1_MASK               (N13_BITS_MSK << ITH_CMDQ_SR1_BIT)

#define ITH_CMDQ_INTSTATUS_BIT          12
#define ITH_CMDQ_AHBEMTPY_BIT           11
#define ITH_CMDQ_FLIPCMDLOAD_BIT        10
#define ITH_CMDQ_OVGCMDLOAD_BIT         9
#define ITH_CMDQ_ISPCMDLOAD_BIT         8
#define ITH_CMDQ_FLIPEMPTY_BIT          7
#define ITH_CMDQ_ISPBUSY_BIT            6
#define ITH_CMDQ_DPUBUSY_BIT            5
#define ITH_CMDQ_OVGBUSY_BIT            4
#define ITH_CMDQ_CMQFAIL_BIT            3
#define ITH_CMDQ_ALLIDLE_BIT            2
#define ITH_CMDQ_HQEMPTY_BIT            1
#define ITH_CMDQ_SQEMPTY_BIT            0

#define ITH_CMDQ_SR2_REG                0x1414
#define ITH_CMDQ_SR3_REG                0x1416
#define ITH_CMDQ_SR4_REG                0x1418
#define ITH_CMDQ_SR5_REG                0x141A

#define ITH_CMDQ_IR1_REG                0x141C

#define ITH_CMDQ_INTCLR_BIT             15

#define ITH_CMDQ_INTCFGMODE_BIT         4
#define ITH_CMDQ_INTCFGMODE_MASK        (N04_BITS_MSK << ITH_CMDQ_INTCFGMODE_BIT)
#define ITH_CMDQ_INTCFGMODE_INTRDPTR    0
#define ITH_CMDQ_INTCFGMODE_ALLEMPTY    1
#define ITH_CMDQ_INTCFGMODE_HQEMPTY     2
#define ITH_CMDQ_INTCFGMODE_SQEMPTY     3

#define ITH_CMDQ_INTCFGUPDATE_BIT       0

#define ITH_CMDQ_INTRDPTR_LO_REG        0x141E

#define ITH_CMDQ_INTRDPTR_HI_REG        0x1420
#define ITH_CMDQ_INTRDPTR_HI_BIT        0
#define ITH_CMDQ_INTRDPTR_HI_MASK       (N03_BITS_MSK << ITH_CMDQ_INTRDPTR_HI_BIT)

#define ITH_CMDQ_DCR_REG                0x1422
#define ITH_CMDQ_DCR_BIT                0
#define ITH_CMDQ_DCR_MASK               (N08_BITS_MSK << ITH_CMDQ_DCR_BIT)

// OPENVG
#define ITH_OPVG_BID1_REG               0x1FC
#define ITH_OPVG_BID2_REG               0x200
#define ITH_OPVG_BID3_REG               0x204

// NAND
#define ITH_NAND_AUTOBOOTCFG_REG        0x38
#define ITH_NAND_AUTOBOOTCFG_BIT        8
#define ITH_NAND_AUTOBOOTCFG_MASK       (N02_BITS_MSK << ITH_NAND_AUTOBOOTCFG_BIT)

// GPIO
#define ITH_GPIO1_DATAOUT_REG           0x00
#define ITH_GPIO1_DATAIN_REG            0x04
#define ITH_GPIO1_PINDIR_REG            0x08
#define ITH_GPIO1_DATASET_REG           0x0C
#define ITH_GPIO1_DATACLR_REG           0x10
#define ITH_GPIO1_PULLEN_REG            0x14
#define ITH_GPIO1_PULLTYPE_REG          0x18
#define ITH_GPIO1_INTREN_REG            0x1C
#define ITH_GPIO1_INTRRAWSTATE_REG      0x20
#define ITH_GPIO1_INTRMASKSTATE_REG     0x24
#define ITH_GPIO1_INTRMASK_REG          0x28
#define ITH_GPIO1_INTRCLR_REG           0x2C
#define ITH_GPIO1_INTRTRIG_REG          0x30
#define ITH_GPIO1_INTRBOTH_REG          0x34
#define ITH_GPIO1_INTRRISENEG_REG       0x38
#define ITH_GPIO1_BOUNCEEN_REG          0x3C
#define ITH_GPIO2_DATAOUT_REG           0x40
#define ITH_GPIO2_DATAIN_REG            0x44
#define ITH_GPIO2_PINDIR_REG            0x48
#define ITH_GPIO2_DATASET_REG           0x4C
#define ITH_GPIO2_DATACLR_REG           0x50
#define ITH_GPIO2_PULLEN_REG            0x54
#define ITH_GPIO2_PULLTYPE_REG          0x58
#define ITH_GPIO2_INTREN_REG            0x5C
#define ITH_GPIO2_INTRRAWSTATE_REG      0x60
#define ITH_GPIO2_INTRMASKSTATE_REG     0x64
#define ITH_GPIO2_INTRMASK_REG          0x68
#define ITH_GPIO2_INTRCLR_REG           0x6C
#define ITH_GPIO2_INTRTRIG_REG          0x70
#define ITH_GPIO2_INTRBOTH_REG          0x74
#define ITH_GPIO2_INTRRISENEG_REG       0x78
#define ITH_GPIO2_BOUNCEEN_REG          0x7C

#define ITH_GPIO2_BOUNCEPRESCALE_REG    0x80
#define ITH_GPIO_FEATURE_REG            0x84
#define ITH_GPIO_REV_REG                0x88
#define ITH_GPIO1_MODE_REG              0x90
#define ITH_GPIO2_MODE_REG              0x94
#define ITH_GPIO3_MODE_REG              0x98
#define ITH_GPIO4_MODE_REG              0x9C

#define ITH_GPIO1_DRIVING_SET_REG       0xA0
#define ITH_GPIO2_DRIVING_SET_REG       0xA4
#define ITH_GPIO3_DRIVING_SET_REG       0xA8
#define ITH_GPIO4_DRIVING_SET_REG       0xAC

#define ITH_GPIO_MISC_SET_REG           0xD0
#define ITH_GPIO_IIC_MODE_POS           (28u)   // 0: external IIC + internal HDMI + IIC controller
                                                // 1: external IIC + IIC controller
                                                // 2: external IIC + internal HDMI
                                                // 3: internal HDMI + IIC controller
#define ITH_GPIO_IIC_MODE_MSK           (N02_BITS_MSK << ITH_GPIO_IIC_MODE_POS)
#define ITH_GPIO_SSP1_MODE_POS          (27u)   // 0: master; 1: slave
#define ITH_GPIO_SSP1_MODE_MSK          (N01_BITS_MSK << ITH_GPIO_IIC_MODE_MSK)
#define ITH_GPIO_SSP_CLK_SEL_POS        (7u)    // 0: from IIS AMCLK; 1: from APB WCLK
#define ITH_GPIO_SSP_CLK_SEL_MSK        (N01_BITS_MSK << ITH_GPIO_SSP_CLK_SEL_POS)
#define ITH_GPIO_HOST_SEL_POS           (4u)
#define ITH_GPIO_HOST_SEL_MSK           (N03_BITS_MSK << ITH_GPIO_HOST_SEL_POS)
#define ITH_GPIO_STOR_SEL_POS           (0u)
#define ITH_GPIO_STOR_SEL_MSK           (N03_BITS_MSK << ITH_GPIO_STOR_SEL_POS)

#define ITH_GPIO_HOSTSEL_HOSTCFG        0x0
#define ITH_GPIO_HOSTSEL_GPIO           0x4
#define ITH_GPIO_HOSTSEL_ARMJTAG        0x5
#define ITH_GPIO_HOSTSEL_RISCJTAG       0x6
#define ITH_GPIO_HOSTSEL_ARMRISCJTAG    0x7

#define ITH_GPIO_HTRAP_REG              0xD4

#define ITH_GPIO_HTRAP_PULLDOWN_BIT     8
#define ITH_GPIO_HTRAP_PULLDOWN_MASK    (N04_BITS_MSK << ITH_GPIO_HTRAP_PULLDOWN_BIT)

#define ITH_GPIO_HTRAP_PULLUP_BIT       4
#define ITH_GPIO_HTRAP_PULLUP_MASK      (N04_BITS_MSK << ITH_GPIO_HTRAP_PULLUP_BIT)

#define ITH_GPIO_SD_SEL_BIT             2

#define ITH_GPIO_URTXSEL1_REG           0xD8
#define ITH_GPIO_URTXSEL2_REG           0xDC
#define ITH_GPIO_URRXSEL1_REG           0xE0
#define ITH_GPIO_URRXSEL2_REG           0xE4

// Interrupt
#define ITH_INTR_IRQ1_SRC_REG           0x00
#define ITH_INTR_IRQ1_EN_REG            0x04
#define ITH_INTR_IRQ1_CLR_REG           0x08
#define ITH_INTR_IRQ1_TRIGMODE_REG      0x0C
#define ITH_INTR_IRQ1_TRIGLEVEL_REG     0x10
#define ITH_INTR_IRQ1_STATUS_REG        0x14
#define ITH_INTR_IRQ1_SWINTR_REG        0x18
#define ITH_INTR_FIQ1_SRC_REG           0x20
#define ITH_INTR_FIQ1_EN_REG            0x24
#define ITH_INTR_FIQ1_CLR_REG           0x28
#define ITH_INTR_FIQ1_TRIGMODE_REG      0x2C
#define ITH_INTR_FIQ1_TRIGLEVEL_REG     0x30
#define ITH_INTR_FIQ1_STATUS_REG        0x34
#define ITH_INTR_FIQ1_SWINTR_REG        0x38
#define ITH_INTR_IRQ2_SRC_REG           0x100
#define ITH_INTR_IRQ2_EN_REG            0x104
#define ITH_INTR_IRQ2_CLR_REG           0x108
#define ITH_INTR_IRQ2_TRIGMODE_REG      0x10C
#define ITH_INTR_IRQ2_TRIGLEVEL_REG     0x110
#define ITH_INTR_IRQ2_STATUS_REG        0x114
#define ITH_INTR_IRQ2_SWINTR_REG        0x118
#define ITH_INTR_FIQ2_SRC_REG           0x120
#define ITH_INTR_FIQ2_EN_REG            0x124
#define ITH_INTR_FIQ2_CLR_REG           0x128
#define ITH_INTR_FIQ2_TRIGMODE_REG      0x12C
#define ITH_INTR_FIQ2_TRIGLEVEL_REG     0x130
#define ITH_INTR_FIQ2_STATUS_REG        0x134
#define ITH_INTR_FIQ2_SWINTR_REG        0x138

#define ITH_INTR_SWINT_BIT              0

// RTC
#define ITH_RTC_SEC_REG                 0x00
#define ITH_RTC_SEC_MASK                0x3F

#define ITH_RTC_MIN_REG                 0x04
#define ITH_RTC_MIN_MASK                0x3F

#define ITH_RTC_HOUR_REG                0x08
#define ITH_RTC_HOUR_MASK               0x1F

#define ITH_RTC_DAY_REG                 0x0C
#define ITH_RTC_DAY_MASK                0xFFFF

#define ITH_RTC_WEEK_REG                0x10
#define ITH_RTC_WEEK_MASK               0x7

#define ITH_RTC_REC_REG                 0x28
#define ITH_RTC_CR_REG                  0x2C

#define ITH_RTC_PWREN_IOSEL_REG         0x2C
#define ITH_RTC_PWREN_IOSEL_BIT         22
#define ITH_RTC_PWREN_IOSEL_MASK        (N02_BITS_MSK << ITH_RTC_PWREN_IOSEL_BIT)

#define ITH_RTC_STATE_REG               0x2C
#define ITH_RTC_STATE_BIT               25
#define ITH_RTC_STATE_MASK              (N06_BITS_MSK << ITH_RTC_STATE_BIT)

#define ITH_RTC_WSEC_REG                0x30
#define ITH_RTC_WSEC_MASK               0x3F

#define ITH_RTC_WMIN_REG                0x34
#define ITH_RTC_WMIN_MASK               0x3F

#define ITH_RTC_WHOUR_REG               0x38
#define ITH_RTC_WHOUR_MASK              0x1F

#define ITH_RTC_WDAY_REG                0x3C
#define ITH_RTC_WDAY_MASK               0xFFFF

#define ITH_RTC_INTRSTATE_REG           0x44

#define ITH_RTC_DIV_REG                 0x48
#define ITH_RTC_DIV_EN_BIT              31
#define ITH_RTC_DIV_EN_MASK             (N01_BITS_MSK << ITH_RTC_DIV_EN_BIT)
#define ITH_RTC_DIV_SRC_BIT             30
#define ITH_RTC_DIV_SRC_MASK            (N01_BITS_MSK << ITH_RTC_DIV_SRC_BIT)
#define ITH_RTC_DIV_CYCLE_BIT           0
#define ITH_RTC_DIV_CYCLE_MASK          (0x3FFFFFFF << ITH_RTC_DIV_CYCLE_BIT)

// UART
#define ITH_UART_DLL_REG                0x0                     // Divisor Register LSB
#define ITH_UART_THR_REG                0x00                    // Transmitter Holding Register(Write)
#define ITH_UART_RBR_REG                0x00                    // Receive Buffer register (Read)

#define ITH_UART_IER_REG                0x04                    // Interrupt Enable Register(Write)
#define ITH_UART_DLM_REG                0x4                     // Divisor Register MSB

#define ITH_UART_RECV_READY             0x04
#define ITH_UART_THR_EMPTY              0x02

#define ITH_UART_IIR_REG                0x08                    // Interrupt Identification Register (Read)
#define ITH_UART_IIR_TXFIFOFULL         0x10                    // This bit is set as 1 when TX FIFO is full

#define ITH_UART_FCR_REG                0x08                    // FIFO control register(Write)
#define ITH_UART_FCR_FIFO_EN_BIT        0                       // FIFO Enable
#define ITH_UART_FCR_RXFIFO_RESET_BIT   1                       // Rx FIFO Reset
#define ITH_UART_FCR_TXFIFO_RESET_BIT   2                       // Tx FIFO Reset
#define ITH_UART_FCR_TXFIFO_TRGL_BIT    4
#define ITH_UART_FCR_TXFIFO_TRGL_MASK   (N02_BITS_MSK << ITH_UART_FCR_TXFIFO_TRGL_BIT)
#define ITH_UART_FCR_RXFIFO_TRGL_BIT    6
#define ITH_UART_FCR_RXFIFO_TRGL_MASK   (N02_BITS_MSK << ITH_UART_FCR_RXFIFO_TRGL_BIT)

#define ITH_UART_LCR_REG                0x0C                    // Line Control register
#define ITH_UART_LCR_STOP               0x4
#define ITH_UART_LCR_EVEN               0x18                    // Even Parity
#define ITH_UART_LCR_ODD                0x8                     // Odd Parity
#define ITH_UART_LCR_PE                 0x8                     // Parity Enable
#define ITH_UART_LCR_SETBREAK           0x40                    // Set Break condition
#define ITH_UART_LCR_STICKPARITY        0x20                    // Stick Parity Enable
#define ITH_UART_LCR_DLAB               0x80                    // Divisor Latch Access Bit

#define ITH_UART_DLH_REG                0x10                    // Fraction Divisor Register
#define ITH_UART_MCR_REG                0x10                    // Modem Control Register
#define ITH_UART_MCR_DMAMODE2           0x20                    // DMA mode2

#define ITH_UART_LSR_REG                0x14                    // Line status register(Read)
#define ITH_UART_LSR_DR                 0x1                     // Data Ready
#define ITH_UART_LSR_THRE               0x20                    // THR Empty
#define ITH_UART_LSR_TRANS_EMPTY        0x40

#define ITH_UART_MDR_REG                0x20
#define ITH_UART_MDR_MODE_SEL_MASK      0x03

#define ITH_UART_CLK_REG                0x22
#define ITH_UART_CLK_BIT                15
#define ITH_UART_CLK_SRC_BIT            14

#define ITH_UART_FEATURE_REG            0x98
#define ITH_UART_FIFO_DEPTH_BIT         0
#define ITH_UART_FIFO_DEPTH_MASK        (N04_BITS_MSK << ITH_UART_FIFO_DEPTH_BIT)

// Timer
#define ITH_TIMER1_CNT_REG              0x0
#define ITH_TIMER1_LOAD_REG             0x4
#define ITH_TIMER1_MATCH1_REG           0x8
#define ITH_TIMER1_MATCH2_REG           0xC

#define ITH_TIMER1_CR_REG               0x60

#define ITH_TIMER_EN_BIT                0
#define ITH_TIMER_CLK_BIT               1
#define ITH_TIMER_UPDOWN_BIT            2
#define ITH_TIMER_ONESHOT_BIT           3
#define ITH_TIMER_MODE_BIT              4
#define ITH_TIMER_PWMEN_BIT             5
#define ITH_TIMER_EN64_BIT              6

#define ITH_TIMER_INTRRAWSTATE_REG      0x78
#define ITH_TIMER_INTRSTATE_REG         0x7C
#define ITH_TIMER_INTRMASK_REG          0x80

// Watch Dog
#define ITH_WD_COUNTER_REG              0x00
#define ITH_WD_LOAD_REG                 0x04
#define ITH_WD_RESTART_REG              0x08
#define ITH_WD_CR_REG                   0x0C

#define ITH_WD_AUTORELOAD               0x5AB9

// Remote IR
#define ITH_IR_CAP_CTRL_REG             0x00
#define ITH_IR_CAPMODE_BIT              6
#define ITH_IR_CAPMODE_MASK             (N02_BITS_MSK << ITH_IR_CAPMODE_BIT)

#define ITH_IR_CAP_STATUS_REG           0x04
#define ITH_IR_DATAREADY_BIT            0
#define ITH_IR_OE_BIT                   3
#define ITH_IR_CLEAR_BIT                9

#define ITH_IR_CAP_PRESCALE_REG         0x08
#define ITH_IR_CAP_DATA_REG             0x0C

#define ITH_IR_HWCFG_REG                0x10

#define ITH_IR_WIDTH_BIT                8
#define ITH_IR_WIDTH_MASK               (N08_BITS_MSK << ITH_IR_WIDTH_BIT)

// DMA
#if defined(CFG_AXI_DMA)

    #include "ith_defs_dma_axi.h"

#else     // #if defined(CFG_AXI_DMA)

    #define ITH_DMA_INT_REG             0x00
    #define ITH_DMA_INT_TC_REG          0x04
    #define ITH_DMA_INT_TC_CLR_REG      0x08

    #define ITH_DMA_INT_ERRABT_REG      0x0C
    #define ITH_DMA_INT_ERRABT_CLR_REG  0x10

    #define ITH_DMA_TC_REG              0x14
    #define ITH_DMA_ERRABT_REG          0x18
    #define ITH_DMA_CH_EN_REG           0x1C
    #define ITH_DMA_CH_BUSY_REG         0x20

    #define ITH_DMA_CSR_REG             0x24                // main configuration status register
    #define ITH_DMA_M1_BIG_ENDIAN_BIT   2
    #define ITH_DMA_M0_BIG_ENDIAN_BIT   1
    #define ITH_DMA_EN_BIT              0

    #define ITH_DMA_SYNC_REG            0x28
    #define ITH_DMA_REVISION_REG        0x30

    #define ITH_DMA_FEATURE_REG         0x34
    #define ITH_DMA_MAX_CHNO_N_BIT      12
    #define ITH_DMA_MAX_CHNO_N_MASK     (N04_BITS_MSK << ITH_DMA_MAX_CHNO_N_BIT)

    #define ITH_DMA_C0_CSR_REG          0x100               // channel control register
    #define ITH_DMA_CUR_TC_MSK_BIT      31
    #define ITH_DMA_CUR_TC_MASK         (N01_BITS_MSK << ITH_DMA_CUR_TC_MSK_BIT)

    #define ITH_DMA_FF_TH_BIT           24
    #define ITH_DMA_FF_TH_MASK          (N03_BITS_MSK << ITH_DMA_FF_TH_BIT)
enum
{
    ITH_DMA_FF_TH_1     = 0x0,
    ITH_DMA_FF_TH_2     = 0x1,
    ITH_DMA_FF_TH_4     = 0x2,
    ITH_DMA_FF_TH_8     = 0x3,
    ITH_DMA_FF_TH_16    = 0x4
};

    #define ITH_DMA_CH_PRIO_BIT     22
    #define ITH_DMA_CH_PRIO_MASK    (N02_BITS_MSK << ITH_DMA_CH_PRIO_BIT)
typedef enum
{
    ITH_DMA_CH_PRIO_LOWEST  = 0x0,
    ITH_DMA_CH_PRIO_HIGH_3  = 0x1,
    ITH_DMA_CH_PRIO_HIGH_2  = 0x2,
    ITH_DMA_CH_PRIO_HIGHEST = 0x3
} ITHDmaPriority;

    #define ITH_DMA_BURST_SIZE_BIT  16
    #define ITH_DMA_BURST_SIZE_MASK (N03_BITS_MSK << ITH_DMA_BURSTC_SIZE_BIT)
typedef enum
{
    ITH_DMA_BURST_1     = 0,
    ITH_DMA_BURST_4     = 1,
    ITH_DMA_BURST_8     = 2,
    ITH_DMA_BURST_16    = 3,
    ITH_DMA_BURST_32    = 4,
    ITH_DMA_BURST_64    = 5,
    ITH_DMA_BURST_128   = 6,
    ITH_DMA_BURST_256   = 7
} ITHDmaBurst;

    #define ITH_DMA_ABT_BIT         15
    #define ITH_DMA_ABT_MASK        (N01_BITS_MSK << ITH_DMA_ABT_BIT)

    #define ITH_DMA_SRC_WIDTH_BIT   11
    #define ITH_DMA_SRC_WIDTH_MASK  (N03_BITS_MSK << ITH_DMA_SRC_WIDTH_BIT)
    #define ITH_DMA_DST_WIDTH_BIT   8
    #define ITH_DMA_DST_WIDTH_MASK  (N03_BITS_MSK << ITH_DMA_DST_WIDTH_BIT)
typedef enum
{
    ITH_DMA_WIDTH_8     = 0,
    ITH_DMA_WIDTH_16    = 1,
    ITH_DMA_WIDTH_32    = 2
} ITHDmaWidth;

    #define ITH_DMA_MODE_BIT    7
    #define ITH_DMA_MODE_MASK   (N01_BITS_MSK << ITH_DMA_MODE_BIT)
typedef enum
{
    ITH_DMA_NORMAL_MODE         = 0,
    ITH_DMA_HW_HANDSHAKE_MODE   = 1
} ITHDmaMode;

    #define ITH_DMA_SRCAD_CTRL_BIT  5
    #define ITH_DMA_SRCAD_CTRL_MASK (N02_BITS_MSK << ITH_DMA_SRCAD_CTRL_BIT)
    #define ITH_DMA_DSTAD_CTRL_BIT  3
    #define ITH_DMA_DSTAD_CTRL_MASK (N02_BITS_MSK << ITH_DMA_DSTAD_CTRL_BIT)
typedef enum
{
    ITH_DMA_CTRL_INC    = 0,
    ITH_DMA_CTRL_DEC    = 1,
    ITH_DMA_CTRL_FIX    = 2
} ITHDmaAddrCtl;

    #define ITH_DMA_SRC_SEL_BIT 2
    #define ITH_DMA_DST_SEL_BIT 1
typedef enum
{
    ITH_DMA_MASTER_0    = 0,
    ITH_DMA_MASTER_1    = 1
} ITHDmaDataMaster;

    #define ITH_DMA_CH_EN_MASK  0x1

    #define ITH_DMA_C0_CFG_REG  0x104                       // channel configuration register
    #define ITH_DST_HE_BIT      13
    #define ITH_DST_HE_MASK     (N01_BITS_MSK << ITH_DST_HE_BIT)
    #define ITH_DST_RS_BIT      9
    #define ITH_DST_RS_MASK     (N04_BITS_MSK << ITH_DST_RS_BIT)
    #define ITH_SRC_HE_BIT      7
    #define ITH_SRC_HE_MASK     (N01_BITS_MSK << ITH_SRC_HE_BIT)
    #define ITH_SRC_RS_BIT      3
    #define ITH_SRC_RS_MASK     (N04_BITS_MSK << ITH_SRC_RS_BIT)
typedef enum
{
    ITH_DMA_MEM         = 0,
    ITH_DMA_HW_MS       = 0,
    ITH_DMA_HW_IR_Cap   = 1,
    ITH_DMA_HW_SSP_TX   = 2,
    ITH_DMA_HW_SSP_RX   = 3,
    ITH_DMA_HW_SSP2_TX  = 4,
    ITH_DMA_HW_SPDIF    = ITH_DMA_HW_SSP2_TX,
    ITH_DMA_HW_SSP2_RX  = 5,
    ITH_DMA_HW_UART_TX  = 6,
    ITH_DMA_HW_UART_RX  = 7,
    ITH_DMA_HW_UART_FIR = 8,
    ITH_DMA_HW_UART2_TX = 9,
    ITH_DMA_HW_UART2_RX = 10,
    ITH_DMA_HW_CF       = 12,
    ITH_DMA_HW_SD       = ITH_DMA_HW_CF,
    ITH_DMA_HW_XD       = ITH_DMA_HW_CF
} ITHDmaRequest;

    #define ITH_DMA_INT_MASK        0x7         // D[2]:abort, D[1]:error, D[0]:tc

    #define ITH_DMA_C0_SRCADR_REG   0x108       // channel source address
    #define ITH_DMA_C0_DSTADR_REG   0x10C       // channel destination address
    #define ITH_DMA_C0_LLP_REG      0x110       // linked list descriptor pointer
    #define ITH_DMA_C0_TX_SIZE_REG  0x114       // transfer size

    #define ITH_DMA_CH_OFFSET       0x20

#endif      // #if defined(CFG_AXI_DMA)

#endif      // ITH_DEFS_H