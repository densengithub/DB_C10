#ifndef SARADC_TYPE_H
#define SARADC_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

//=============================================================================
//                Constant Definition
//=============================================================================
typedef enum SARADC_PORT_TAG
{
    SARADC_0,
    SARADC_1,
    SARADC_2,
    SARADC_3
} SARADC_PORT;

typedef enum SARADC_AMPLIFY_GAIN_TAG
{
    SARADC_AMPLIFY_1X,    // no amplified
    SARADC_AMPLIFY_2X,
    SARADC_AMPLIFY_4X,
    SARADC_AMPLIFY_8X
} SARADC_AMPLIFY_GAIN;

typedef enum SARADC_MODE_TAG
{
    SARADC_MODE_SRAM_INTR = 0,    // message notification(SRAM + INTR)
    SARADC_MODE_SRAM = 2,
    SARADC_MODE_INTR
} SARADC_MODE;

typedef enum SARADC_CLK_DIV_TAG
{
    SARADC_CLK_DIV_0,
    SARADC_CLK_DIV_1,
    SARADC_CLK_DIV_2,
    SARADC_CLK_DIV_3,
    SARADC_CLK_DIV_4,
    SARADC_CLK_DIV_5,
    SARADC_CLK_DIV_6,
    SARADC_CLK_DIV_7,
    SARADC_CLK_DIV_8,
    SARADC_CLK_DIV_9,
    SARADC_CLK_DIV_10,
    SARADC_CLK_DIV_11,
    SARADC_CLK_DIV_12,
    SARADC_CLK_DIV_13,
    SARADC_CLK_DIV_14,
    SARADC_CLK_DIV_15,
    SARADC_CLK_DIV_16,
    SARADC_CLK_DIV_17,
    SARADC_CLK_DIV_18,
    SARADC_CLK_DIV_19,
    SARADC_CLK_DIV_20,
    SARADC_CLK_DIV_21,
    SARADC_CLK_DIV_22,
    SARADC_CLK_DIV_23,
    SARADC_CLK_DIV_24,
    SARADC_CLK_DIV_25,
    SARADC_CLK_DIV_26,
    SARADC_CLK_DIV_27,
    SARADC_CLK_DIV_28,
    SARADC_CLK_DIV_29,
    SARADC_CLK_DIV_30,
    SARADC_CLK_DIV_31
} SARADC_CLK_DIV;

typedef enum SARADC_AVG_CAL_COUNT_TAG
{
    SARADC_AVG_CAL_COUNT_0,     // 2^0: 1 count
    SARADC_AVG_CAL_COUNT_1,     // 2^1: 2 counts
    SARADC_AVG_CAL_COUNT_2,
    SARADC_AVG_CAL_COUNT_3,
    SARADC_AVG_CAL_COUNT_4,
    SARADC_AVG_CAL_COUNT_5,
    SARADC_AVG_CAL_COUNT_6,
    SARADC_AVG_CAL_COUNT_7,
    SARADC_AVG_CAL_COUNT_8,
    SARADC_AVG_CAL_COUNT_9,
    SARADC_AVG_CAL_COUNT_10,    // 2^10: 1024 counts
    SARADC_AVG_CAL_COUNT_11,
    SARADC_AVG_CAL_COUNT_12,
    SARADC_AVG_CAL_COUNT_13,
    SARADC_AVG_CAL_COUNT_14,
    SARADC_AVG_CAL_COUNT_15,
    SARADC_AVG_CAL_COUNT_16,
    SARADC_AVG_CAL_COUNT_17,
    SARADC_AVG_CAL_COUNT_18,
    SARADC_AVG_CAL_COUNT_19,
    SARADC_AVG_CAL_COUNT_20,
    SARADC_AVG_CAL_COUNT_21,
    SARADC_AVG_CAL_COUNT_22,
    SARADC_AVG_CAL_COUNT_23,
    SARADC_AVG_CAL_COUNT_24,
    SARADC_AVG_CAL_COUNT_25,
    SARADC_AVG_CAL_COUNT_26,
    SARADC_AVG_CAL_COUNT_27,
    SARADC_AVG_CAL_COUNT_28,
    SARADC_AVG_CAL_COUNT_29,
    SARADC_AVG_CAL_COUNT_30,
    SARADC_AVG_CAL_COUNT_31
} SARADC_AVG_CAL_COUNT;

typedef enum SARADC_INTR_AVG_TAG
{
    SARADC_INTR_AVG_VALID,
    SARADC_INTR_AVG_ABOMAX,
    SARADC_INTR_AVG_UNDMIN
} SARADC_INTR_AVG;

//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================
typedef struct XAIN_INFO_TAG
{
    uint8_t  *baseAddr;
    uint16_t rptr;
    uint16_t maxAVGDetect;
    uint16_t minAVGDetect;
} XAIN_INFO;

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

#endif /* SARADC_TYPE_H */
