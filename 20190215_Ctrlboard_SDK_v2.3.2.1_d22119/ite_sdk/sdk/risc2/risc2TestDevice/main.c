#include <stdarg.h>

#include "risc2/risc2TestDevice/risc2TestDevice.h"

#define ENDIAN_SWAP16(x) \
        (((x & 0x00FF) << 8) | ((x & 0xFF00) >> 8))

#define ENDIAN_SWAP32(x) \
        (((x & 0x000000FF) << 24) | \
        ((x & 0x0000FF00) <<  8) | \
        ((x & 0x00FF0000) >>  8) | \
        ((x & 0xFF000000) >> 24))

uint8_t* gpCmdExchangeAddr = NULL;
extern unsigned char risc2_start_addr[];
uint32_t gRiscCpuClock = 0;

static void risc2TestDeviceProcessInitCmd(void)
{
    TEST_DEVICE_INIT_DATA* ptInitData = (TEST_DEVICE_INIT_DATA*) gpCmdExchangeAddr;
    gRiscCpuClock =  ENDIAN_SWAP32(ptInitData->cpuClock);
}

static void risc2TestDeviceProcessReadCmd(void)
{
    TEST_DEVICE_READ_DATA* ptReadData = (TEST_DEVICE_READ_DATA*) gpCmdExchangeAddr;
    int i = 0;

    //reply certain pattern for test..
    for (i = 0; i < 64; i++)
    {
        ptReadData->pBuffer[i] = i;
    }
    ptReadData->retSize = ENDIAN_SWAP32(64);
}

static void risc2TestDeviceProcessGetClockCmd(void)
{
    TEST_DEVICE_GET_CPU_CLOCK* ptGetClockData = (TEST_DEVICE_GET_CPU_CLOCK*) gpCmdExchangeAddr;
    ptGetClockData->cpuClock = ENDIAN_SWAP32(gRiscCpuClock);
}


int main(int argc, char **argv)
{
    //Set GPIO and Clock Setting
    int inputCmd = 0;
    struct _risc2_header *ptRisc2Header = (struct _risc2_header*) risc2_start_addr;
    gpCmdExchangeAddr = (uint8_t*) ENDIAN_SWAP32((uint32_t)ptRisc2Header->cmd_exchange_addr);

    //Start Timer
    startTimer(0);

    while(1)
    {
        inputCmd = RISC2_COMMAND_REG_READ(REQUEST_CMD_REG);
        if (inputCmd && RISC2_COMMAND_REG_READ(RESPONSE_CMD_REG) == 0)
        {
            //need to read data from meomery instead of cache
            dc_invalidate();
            switch(inputCmd)
            {
                case INIT_CMD_ID:
                    risc2TestDeviceProcessInitCmd();
                    break;
                case READ_CMD_ID:
                    risc2TestDeviceProcessReadCmd();
                    break;
                case GET_CLOCK_CMD_ID:
                    risc2TestDeviceProcessGetClockCmd();
                    break;
                default:
                    break;
            }
            RISC2_COMMAND_REG_WRITE(RESPONSE_CMD_REG, (uint16_t) inputCmd);
        }
    }
}
