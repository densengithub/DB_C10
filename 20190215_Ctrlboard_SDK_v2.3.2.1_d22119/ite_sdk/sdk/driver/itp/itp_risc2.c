/** @file
 * PAL RISC2 CPU functions.
 *
 * @author Steven Hsiao
 * @version 1.0
 * @date 2018/09/26
 * @copyright ITE Tech. Inc. All Rights Reserved.
 */
#include <errno.h>
#include "openrtos/FreeRTOS.h"
#include "risc2/risc2_device.h"


#ifdef CFG_RISC2_TEST_DEVICE
extern ITPDevice itpDeviceRisc2Test;
#endif
#ifdef CFG_RISC2_OPUS_CODEC
extern ITPDevice itpDeviceOpusCodec;
#endif

typedef struct
{
    int         engineMode;
    ITPDevice*  ptDevice;
} RISC2_ENGINE;

static pthread_mutex_t gRisc2Mutex  = PTHREAD_MUTEX_INITIALIZER;
static RISC2_ENGINE gtCurDevice = { 0 };

static RISC2_ENGINE gptRisc2EngineArray[] =
{
#ifdef CFG_RISC2_TEST_DEVICE
    {RISC2_TEST_DEVICE, &itpDeviceRisc2Test},
#endif
#ifdef CFG_RISC2_OPUS_CODEC
    {RISC2_OPUS_CODEC, &itpDeviceOpusCodec},
#endif
};

static int risc2Ioctl(int file, unsigned long request, void *ptr, void *info)
{
    int i = 0;
    int newEngineMode = 0;
    ITPDevice *pNewDevice = NULL;
    int ret = 0;
    
    pthread_mutex_lock(&gRisc2Mutex);
    switch (request)
    {
    	case ITP_IOCTL_RISC2_SWITCH_ENG:
    	    //Engine is running, therefore, need to reload new engine image.
    	    newEngineMode = *(int*)ptr;
            if (gtCurDevice.engineMode == RISC2_UNKNOWN_DEVICE || gtCurDevice.engineMode != newEngineMode)
            {
                iteRiscResetCpu(RISC2_CPU);
                for (i = 0; i < sizeof(gptRisc2EngineArray) / sizeof(RISC2_ENGINE); i++)
                {
                    if (newEngineMode == gptRisc2EngineArray[i].engineMode)
                    {
                        pNewDevice = gptRisc2EngineArray[i].ptDevice;
                        gtCurDevice.ptDevice = pNewDevice;
                        break;
                    }
                }

                if (i == sizeof(gptRisc2EngineArray) / sizeof(RISC2_ENGINE))
                {
                    printf("itp_risc2.c(%d), requested RISC2 engine is not exited\n", __LINE__);
                }
            }
    		break;
        default:
            ret = gtCurDevice.ptDevice->ioctl(file, request, ptr, info);
            break;
    }    
    pthread_mutex_unlock(&gRisc2Mutex);
    return ret;
}

static int risc2Read(int file, char *ptr, int len, void* info)
{
    int ret = 0;
    if (gtCurDevice.ptDevice)
    {
        pthread_mutex_lock(&gRisc2Mutex);
        ret =  gtCurDevice.ptDevice->read(file, ptr, len, info);
        pthread_mutex_unlock(&gRisc2Mutex);
    }
    return ret;
}

static int risc2Write(int file, char *ptr, int len, void* info)
{
    int ret = 0;
    if (gtCurDevice.ptDevice)
    {
        pthread_mutex_lock(&gRisc2Mutex);
        ret =  gtCurDevice.ptDevice->write(file, ptr, len, info);
        pthread_mutex_unlock(&gRisc2Mutex);
    }
    return ret;
}

static int risc2Seek(int file, int ptr, int dir, void *info)
{
    int ret = 0;

    if (gtCurDevice.ptDevice)
    {
        pthread_mutex_lock(&gRisc2Mutex);
        ret =  gtCurDevice.ptDevice->lseek(file, ptr, dir, info);
        pthread_mutex_unlock(&gRisc2Mutex);
    }
    return ret;
}

const ITPDevice itpDeviceRisc2 =
{
    ":risc2",
    itpOpenDefault,
    itpCloseDefault,
    risc2Read,
    risc2Write,
    risc2Seek,
    risc2Ioctl,
    NULL
};
