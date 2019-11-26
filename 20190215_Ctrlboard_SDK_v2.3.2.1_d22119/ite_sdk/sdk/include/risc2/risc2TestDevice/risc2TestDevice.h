#ifndef __RSL_SLAVE__
#define __RSL_SLAVE__

#ifdef __cplusplus
extern "C" {
#endif

#include "risc2/risc2_device.h"

//=============================================================================
//                Constant Definition
//=============================================================================
#define INIT_CMD_ID                 1
#define READ_CMD_ID                 2
#define GET_CLOCK_CMD_ID            3

#define ITP_IOCTL_INIT_TEST_DEVICE_PARAM            ITP_IOCTL_CUSTOM_CTL_ID1
#define ITP_IOCTL_READ_TEST_DEVICE_DATA             ITP_IOCTL_CUSTOM_CTL_ID2
#define ITP_IOCTL_GET_TEST_DEVICE_CLOCK             ITP_IOCTL_CUSTOM_CTL_ID3

//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================
typedef struct
{
    uint32_t cpuClock;
} TEST_DEVICE_INIT_DATA;

typedef struct
{
    uint8_t     pBuffer[128];
    uint32_t    retSize;
} TEST_DEVICE_READ_DATA;

typedef struct
{
    uint32_t cpuClock;
} TEST_DEVICE_GET_CPU_CLOCK;


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

#endif





