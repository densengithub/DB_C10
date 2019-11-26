#ifndef SARADC_REG_H
#define SARADC_REG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ite/ith_defs.h"

//=============================================================================
//                Constant Definition
//=============================================================================
#define SARADC_REG_BASE                       ITH_SARADC_BASE

//================================================================
// Controller Setting Register 0
//================================================================
#define SARADC_REG_CR0                        (SARADC_REG_BASE + 0x00000000)

#define SARADC_BIT_DRIVER_FIRE_EN             0x00000001    //1000 0000 0000 0000 0000 0000 0000 0000
#define SARADC_BIT_PHY_FIRE_EN                0x00000001    //0100 0000 0000 0000 0000 0000 0000 0000
#define SARADC_BIT_GAIN_SETTING               0x00000003    //0011 0000 0000 0000 0000 0000 0000 0000
#define SARADC_BIT_MODE                       0x00000003    //0000 0011 0000 0000 0000 0000 0000 0000
#define SARADC_BIT_XAIN_VALID                 0x000000FF    //0000 0000 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_INTR_VALID                 0x000000FF    //0000 0000 0000 0000 1111 1111 0000 0000
#define SARADC_BIT_CLK_DIV                    0x0000001F    //0000 0000 0000 0000 0000 0000 0001 1111

#define SARADC_SHT_DRIVER_FIRE_EN             31
#define SARADC_SHT_PHY_FIRE_EN                30
#define SARADC_SHT_GAIN_SETTING               28
#define SARADC_SHT_MODE                       24
#define SARADC_SHT_XAIN_VALID                 16
#define SARADC_SHT_INTR_VALID                 8
#define SARADC_SHT_CLK_DIV                    0

//================================================================
// Controller Setting Register 1
//================================================================
#define SARADC_REG_CR1                        (SARADC_REG_BASE + 0x00000004)

#define SARADC_BIT_AVG_COUNT                  0x0000001F    //0001 1111 0000 0000 0000 0000 0000 0000
#define SARADC_BIT_BUFFER_LEN                 0x0000FFFF    //0000 0000 0000 0000 1111 1111 1111 1111

#define SARADC_SHT_AVG_COUNT                  24
#define SARADC_SHT_BUFFER_LEN                 0

//================================================================
// XAIN Base Address
//================================================================
#define SARADC_REG_XAIN_BASEADDR_0            (SARADC_REG_BASE + 0x00000008)

#define SARADC_BIT_BASEADDR_0                 0xFFFFFFFF    //1111 1111 1111 1111 1111 1111 1111 1111

#define SARADC_SHT_BASEADDR_0                 0

#define SARADC_REG_XAIN_BASEADDR_1            (SARADC_REG_BASE + 0x0000000C)

#define SARADC_BIT_BASEADDR_1                 0xFFFFFFFF    //1111 1111 1111 1111 1111 1111 1111 1111

#define SARADC_SHT_BASEADDR_1                 0

#define SARADC_REG_XAIN_BASEADDR_2            (SARADC_REG_BASE + 0x00000010)

#define SARADC_BIT_BASEADDR_2                 0xFFFFFFFF    //1111 1111 1111 1111 1111 1111 1111 1111

#define SARADC_SHT_BASEADDR_2                 0

#define SARADC_REG_XAIN_BASEADDR_3            (SARADC_REG_BASE + 0x00000014)

#define SARADC_BIT_BASEADDR_3                 0xFFFFFFFF    //1111 1111 1111 1111 1111 1111 1111 1111

#define SARADC_SHT_BASEADDR_3                 0

#define SARADC_REG_XAIN_BASEADDR_4            (SARADC_REG_BASE + 0x00000018)

#define SARADC_BIT_BASEADDR_4                 0xFFFFFFFF    //1111 1111 1111 1111 1111 1111 1111 1111

#define SARADC_SHT_BASEADDR_4                 0

#define SARADC_REG_XAIN_BASEADDR_5            (SARADC_REG_BASE + 0x0000001C)

#define SARADC_BIT_BASEADDR_5                 0xFFFFFFFF    //1111 1111 1111 1111 1111 1111 1111 1111

#define SARADC_SHT_BASEADDR_5                 0

#define SARADC_REG_XAIN_BASEADDR_6            (SARADC_REG_BASE + 0x00000020)

#define SARADC_BIT_BASEADDR_6                 0xFFFFFFFF    //1111 1111 1111 1111 1111 1111 1111 1111

#define SARADC_SHT_BASEADDR_6                 0

#define SARADC_REG_XAIN_BASEADDR_7            (SARADC_REG_BASE + 0x00000024)

#define SARADC_BIT_BASEADDR_7                 0xFFFFFFFF    //1111 1111 1111 1111 1111 1111 1111 1111

#define SARADC_SHT_BASEADDR_7                 0

//================================================================
// XAIN Read and Write Pointer
//================================================================
#define SARADC_REG_XAIN_RWPTR_0               (SARADC_REG_BASE + 0x00000028)

#define SARADC_BIT_XAIN_RPTR_0                0x00000FFFF   //1111 1111 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_XAIN_WPTR_0                0x00000FFFF   //0000 0000 0000 0000 1111 1111 1111 1111

#define SARADC_SHT_XAIN_RPTR_0                16
#define SARADC_SHT_XAIN_WPTR_0                0

#define SARADC_REG_XAIN_RWPTR_1               (SARADC_REG_BASE + 0x0000002C)

#define SARADC_BIT_XAIN_RPTR_1                0x00000FFFF   //1111 1111 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_XAIN_WPTR_1                0x00000FFFF   //0000 0000 0000 0000 1111 1111 1111 1111

#define SARADC_SHT_XAIN_RPTR_1                16
#define SARADC_SHT_XAIN_WPTR_1                0

#define SARADC_REG_XAIN_RWPTR_2               (SARADC_REG_BASE + 0x00000030)

#define SARADC_BIT_XAIN_RPTR_2                0x00000FFFF   //1111 1111 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_XAIN_WPTR_2                0x00000FFFF   //0000 0000 0000 0000 1111 1111 1111 1111

#define SARADC_SHT_XAIN_RPTR_2                16
#define SARADC_SHT_XAIN_WPTR_2                0

#define SARADC_REG_XAIN_RWPTR_3               (SARADC_REG_BASE + 0x00000034)

#define SARADC_BIT_XAIN_RPTR_3                0x00000FFFF   //1111 1111 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_XAIN_WPTR_3                0x00000FFFF   //0000 0000 0000 0000 1111 1111 1111 1111

#define SARADC_SHT_XAIN_RPTR_3                16
#define SARADC_SHT_XAIN_WPTR_3                0

#define SARADC_REG_XAIN_RWPTR_4               (SARADC_REG_BASE + 0x00000038)

#define SARADC_BIT_XAIN_RPTR_4                0x00000FFFF   //1111 1111 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_XAIN_WPTR_4                0x00000FFFF   //0000 0000 0000 0000 1111 1111 1111 1111

#define SARADC_SHT_XAIN_RPTR_4                16
#define SARADC_SHT_XAIN_WPTR_4                0

#define SARADC_REG_XAIN_RWPTR_5               (SARADC_REG_BASE + 0x0000003C)

#define SARADC_BIT_XAIN_RPTR_5                0x00000FFFF   //1111 1111 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_XAIN_WPTR_5                0x00000FFFF   //0000 0000 0000 0000 1111 1111 1111 1111

#define SARADC_SHT_XAIN_RPTR_5                16
#define SARADC_SHT_XAIN_WPTR_5                0

#define SARADC_REG_XAIN_RWPTR_6               (SARADC_REG_BASE + 0x00000040)

#define SARADC_BIT_XAIN_RPTR_6                0x00000FFFF   //1111 1111 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_XAIN_WPTR_6                0x00000FFFF   //0000 0000 0000 0000 1111 1111 1111 1111

#define SARADC_SHT_XAIN_RPTR_6                16
#define SARADC_SHT_XAIN_WPTR_6                0

#define SARADC_REG_XAIN_RWPTR_7               (SARADC_REG_BASE + 0x00000044)

#define SARADC_BIT_XAIN_RPTR_7                0x00000FFFF   //1111 1111 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_XAIN_WPTR_7                0x00000FFFF   //0000 0000 0000 0000 1111 1111 1111 1111

#define SARADC_SHT_XAIN_RPTR_7                16
#define SARADC_SHT_XAIN_WPTR_7                0

//================================================================
// XAIN Interrupt Message
//================================================================
#define SARADC_REG_XAIN_INTR                  (SARADC_REG_BASE + 0x00000048)

#define SARADC_BIT_XAIN_OVERWT_7              0x000000001   //1000 0000 0000 0000 0000 0000 0000 0000
#define SARADC_BIT_XAIN_OVERWT_6              0x000000001   //0100 0000 0000 0000 0000 0000 0000 0000
#define SARADC_BIT_XAIN_OVERWT_5              0x000000001   //0010 0000 0000 0000 0000 0000 0000 0000
#define SARADC_BIT_XAIN_OVERWT_4              0x000000001   //0001 0000 0000 0000 0000 0000 0000 0000
#define SARADC_BIT_XAIN_OVERWT_3              0x000000001   //0000 1000 0000 0000 0000 0000 0000 0000
#define SARADC_BIT_XAIN_OVERWT_2              0x000000001   //0000 0100 0000 0000 0000 0000 0000 0000
#define SARADC_BIT_XAIN_OVERWT_1              0x000000001   //0000 0010 0000 0000 0000 0000 0000 0000
#define SARADC_BIT_XAIN_OVERWT_0              0x000000001   //0000 0001 0000 0000 0000 0000 0000 0000
#define SARADC_BIT_XAIN_ABOMAX_7              0x000000001   //0000 0000 0000 0000 1000 0000 0000 0000
#define SARADC_BIT_XAIN_UNDMIN_7              0x000000001   //0000 0000 0000 0000 0100 0000 0000 0000
#define SARADC_BIT_XAIN_ABOMAX_6              0x000000001   //0000 0000 0000 0000 0010 0000 0000 0000
#define SARADC_BIT_XAIN_UNDMIN_6              0x000000001   //0000 0000 0000 0000 0001 0000 0000 0000
#define SARADC_BIT_XAIN_ABOMAX_5              0x000000001   //0000 0000 0000 0000 0000 1000 0000 0000
#define SARADC_BIT_XAIN_UNDMIN_5              0x000000001   //0000 0000 0000 0000 0000 0100 0000 0000
#define SARADC_BIT_XAIN_ABOMAX_4              0x000000001   //0000 0000 0000 0000 0000 0010 0000 0000
#define SARADC_BIT_XAIN_UNDMIN_4              0x000000001   //0000 0000 0000 0000 0000 0001 0000 0000
#define SARADC_BIT_XAIN_ABOMAX_3              0x000000001   //0000 0000 0000 0000 0000 0000 1000 0000
#define SARADC_BIT_XAIN_UNDMIN_3              0x000000001   //0000 0000 0000 0000 0000 0000 0100 0000
#define SARADC_BIT_XAIN_ABOMAX_2              0x000000001   //0000 0000 0000 0000 0000 0000 0010 0000
#define SARADC_BIT_XAIN_UNDMIN_2              0x000000001   //0000 0000 0000 0000 0000 0000 0001 0000
#define SARADC_BIT_XAIN_ABOMAX_1              0x000000001   //0000 0000 0000 0000 0000 0000 0000 1000
#define SARADC_BIT_XAIN_UNDMIN_1              0x000000001   //0000 0000 0000 0000 0000 0000 0000 0100
#define SARADC_BIT_XAIN_ABOMAX_0              0x000000001   //0000 0000 0000 0000 0000 0000 0000 0010
#define SARADC_BIT_XAIN_UNDMIN_0              0x000000001   //0000 0000 0000 0000 0000 0000 0000 0001

#define SARADC_SHT_XAIN_OVERWT_7              31
#define SARADC_SHT_XAIN_OVERWT_6              30
#define SARADC_SHT_XAIN_OVERWT_5              29
#define SARADC_SHT_XAIN_OVERWT_4              28
#define SARADC_SHT_XAIN_OVERWT_3              27
#define SARADC_SHT_XAIN_OVERWT_2              26
#define SARADC_SHT_XAIN_OVERWT_1              25
#define SARADC_SHT_XAIN_OVERWT_0              24
#define SARADC_SHT_XAIN_ABOMAX_7              15
#define SARADC_SHT_XAIN_UNDMIN_7              14
#define SARADC_SHT_XAIN_ABOMAX_6              13
#define SARADC_SHT_XAIN_UNDMIN_6              12
#define SARADC_SHT_XAIN_ABOMAX_5              11
#define SARADC_SHT_XAIN_UNDMIN_5              10
#define SARADC_SHT_XAIN_ABOMAX_4              9
#define SARADC_SHT_XAIN_UNDMIN_4              8
#define SARADC_SHT_XAIN_ABOMAX_3              7
#define SARADC_SHT_XAIN_UNDMIN_3              6
#define SARADC_SHT_XAIN_ABOMAX_2              5
#define SARADC_SHT_XAIN_UNDMIN_2              4
#define SARADC_SHT_XAIN_ABOMAX_1              3
#define SARADC_SHT_XAIN_UNDMIN_1              2
#define SARADC_SHT_XAIN_ABOMAX_0              1
#define SARADC_SHT_XAIN_UNDMIN_0              0

//================================================================
// XAIN Detection Rule
//================================================================
#define SARADC_REG_XAIN_DECTRULE_0            (SARADC_REG_BASE + 0x0000004C)

#define SARADC_BIT_XAIN_RULE_MAX_0            0x00000FFF    //0000 1111 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_XAIN_RULE_MIN_0            0x00000FFF    //0000 0000 0000 0000 0000 1111 1111 1111

#define SARADC_SHT_XAIN_RULE_MAX_0            16
#define SARADC_SHT_XAIN_RULE_MIN_0            0

#define SARADC_REG_XAIN_DECTRULE_1            (SARADC_REG_BASE + 0x00000050)

#define SARADC_BIT_XAIN_RULE_MAX_1            0x00000FFF    //0000 1111 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_XAIN_RULE_MIN_1            0x00000FFF    //0000 0000 0000 0000 0000 1111 1111 1111

#define SARADC_SHT_XAIN_RULE_MAX_1            16
#define SARADC_SHT_XAIN_RULE_MIN_1            0

#define SARADC_REG_XAIN_DECTRULE_2            (SARADC_REG_BASE + 0x00000054)

#define SARADC_BIT_XAIN_RULE_MAX_2            0x00000FFF    //0000 1111 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_XAIN_RULE_MIN_2            0x00000FFF    //0000 0000 0000 0000 0000 1111 1111 1111

#define SARADC_SHT_XAIN_RULE_MAX_2            16
#define SARADC_SHT_XAIN_RULE_MIN_2            0

#define SARADC_REG_XAIN_DECTRULE_3            (SARADC_REG_BASE + 0x00000058)

#define SARADC_BIT_XAIN_RULE_MAX_3            0x00000FFF    //0000 1111 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_XAIN_RULE_MIN_3            0x00000FFF    //0000 0000 0000 0000 0000 1111 1111 1111

#define SARADC_SHT_XAIN_RULE_MAX_3            16
#define SARADC_SHT_XAIN_RULE_MIN_3            0

#define SARADC_REG_XAIN_DECTRULE_4            (SARADC_REG_BASE + 0x0000005C)

#define SARADC_BIT_XAIN_RULE_MAX_4            0x00000FFF    //0000 1111 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_XAIN_RULE_MIN_4            0x00000FFF    //0000 0000 0000 0000 0000 1111 1111 1111

#define SARADC_SHT_XAIN_RULE_MAX_4            16
#define SARADC_SHT_XAIN_RULE_MIN_4            0

#define SARADC_REG_XAIN_DECTRULE_5            (SARADC_REG_BASE + 0x00000060)

#define SARADC_BIT_XAIN_RULE_MAX_5            0x00000FFF    //0000 1111 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_XAIN_RULE_MIN_5            0x00000FFF    //0000 0000 0000 0000 0000 1111 1111 1111

#define SARADC_SHT_XAIN_RULE_MAX_5            16
#define SARADC_SHT_XAIN_RULE_MIN_5            0

#define SARADC_REG_XAIN_DECTRULE_6            (SARADC_REG_BASE + 0x00000064)

#define SARADC_BIT_XAIN_RULE_MAX_6            0x00000FFF    //0000 1111 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_XAIN_RULE_MIN_6            0x00000FFF    //0000 0000 0000 0000 0000 1111 1111 1111

#define SARADC_SHT_XAIN_RULE_MAX_6            16
#define SARADC_SHT_XAIN_RULE_MIN_6            0

#define SARADC_REG_XAIN_DECTRULE_7            (SARADC_REG_BASE + 0x00000068)

#define SARADC_BIT_XAIN_RULE_MAX_7            0x00000FFF    //0000 1111 1111 1111 0000 0000 0000 0000
#define SARADC_BIT_XAIN_RULE_MIN_7            0x00000FFF    //0000 0000 0000 0000 0000 1111 1111 1111

#define SARADC_SHT_XAIN_RULE_MAX_7            16
#define SARADC_SHT_XAIN_RULE_MIN_7            0

//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================

//=============================================================================
//                Global Data Definition
//=============================================================================

//=============================================================================
//                Private Function Definition
//=============================================================================

//=============================================================================
//                Public Function Definition
//=============================================================================

#ifdef __cplusplus
}
#endif

#endif /* SARADC_REG_H */