/*
 * Copyright (c) 2007 ITE Corp. All Rights Reserved.
 */
/** @file
 * Used as TSI HW configure header file.
 *
 * @author I-Chun Lai
 */
#ifndef TSI_HW_H
#define TSI_HW_H

#define MMP_GENERAL_INTERRUPT_REG_06        ITH_HOST_BASE + 0x0006
#define MMP_GENERAL_INTERRUPT_REG_0A        ITH_HOST_BASE + 0x000A
#define MMP_GENERAL_INTERRUPT_REG_0C        ITH_HOST_BASE + 0x000C
#define MMP_TSI_CLOCK_REG_48                ITH_HOST_BASE + 0x0048
#define MMP_PCR_CLOCK_REG_40                ITH_HOST_BASE + 0x0040
#define MMP_PCR_CLOCK_REG_42                ITH_HOST_BASE + 0x0042
#define MMP_PCR_EN_M12CLK                   ITH_HOST_BASE + 0x0002          // memory clock for TSI0 controller
#define MMP_PCR_EN_M13CLK                   ITH_HOST_BASE + 0x0008          // memory clock for TSI1 controller
#define MMP_PCR_EN_PCLK                     ITH_HOST_BASE + 0x0002          // [ 1] PCR clock
#define MMP_PCR_EN_DIV_PCLK                 ITH_HOST_BASE + 0x8002          // [15]
#define MMP_HOST_BUS_EN_MMIO_FPC            ITH_HOST_BASE + 0x0400
#define MMP_HOST_BUS_CONTROLLER_REG_202     ITH_HOST_BASE + 0x0202

//Define Register
#define REG_TSP_0_BASE                      (0xD0400000)    /* Base Register Address */
#define REG_TSP_1_BASE                      (0xD0400080)    /* Base Register Address */
#define REG_TSI_MEMORY_BASE_L_OFFSET        (0x0)
#define REG_TSI_MEMORY_BASE_H_OFFSET        (0x2)
#define REG_TSI_MEMORY_SIZE_L_OFFSET        (0x4)
#define REG_TSI_MEMORY_SIZE_H_OFFSET        (0x6)
#define REG_TSI_READ_POINTER_L_OFFSET       (0x8)
#define REG_TSI_READ_POINTER_H_OFFSET       (0xA)
#define REG_TSI_SETTING_OFFSET              (0xC)
#define REG_TSI_SET_PCR_PID_FILTER_OFFSET   (0xE)
#define REG_TSI_PCR_CNT_THRESHOLD_OFFSET    (0x10)
#define REG_TSI_GET_PCR_PID_OFFSET          (0x12)
#define REG_TSI_WRITE_POINTER_L_OFFSET      (0x14)
#define REG_TSI_WRITE_POINTER_H_OFFSET      (0x16)
#define REG_TSI_PCR_L_OFFSET                (0x18)
#define REG_TSI_PCR_M_OFFSET                (0x1A)
#define REG_TSI_STATUS_OFFSET               (0x1E)
#define REG_TSI_SYNC_BYTE_AF_LENGTH_OFFSET  (0x22)
#define REG_TSI_PCR_CNT_L_OFFSET            (0x24)
#define REG_TSI_PCR_CNT_H_OFFSET            (0x26)
#define REG_TSI_FREQUENCY_RATIO_OFFSET      (0x28)
#define REG_TSI_IO_SRC_AND_FALL_SAMP_OFFSET (0x2A)
#define REG_TSI_PID_FILTER_BASE_OFFSET      (0x2C)
#define REG_TSI_CLEAL_INTR_OFFSET           (0x6C)

#endif