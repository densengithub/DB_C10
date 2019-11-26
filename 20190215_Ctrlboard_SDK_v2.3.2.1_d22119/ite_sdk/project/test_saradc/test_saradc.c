#include <sys/ioctl.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>
#include "ite/itp.h"
#include <sys/statvfs.h>
#include "saradc.h"

//#define USB_RECORD

#define SARADC_output_set_byte       8
#define SARADC_output_set_valid_byte 6
#define SARADC_output_unit2_byte     3

#ifdef USB_RECORD
FILE *SARADC_FD = NULL;
#endif

int testSARADC(int channel)
{
    SARADC_RESULT result = SARADC_SUCCESS;
    uint32_t writeBuffer_len = 4096;
    uint16_t range_min = 0, range_max = 0;
    uint8_t* writeBuffer = NULL;
    int check_times = 1;
    uint16_t printf_chk = 0;
    uint16_t buffer = 0;
    int unit_start = 0;
    int index = 0;
    uint16_t mininum = 0x0;
    uint16_t maximum = 0xfff;
    uint32_t average = 0x0;
    uint8_t temp = 0;
    uint16_t write_ptr = 0;
    uint16_t read_ptr = 0;
    uint32_t printf_count = 0;
    int check_break = 0, tail_check = 0;

    if (writeBuffer_len < 8)
    {
        printf("writeBuffer_len can not be less than 8 !!\n");
        while(1) sleep(1);
    }

    result = mmpSAREnableXAIN(0x1 << channel);
    if (result)
    {
        printf("mmpSAREnableXAIN() error (0x%x) !!\n", result);
        while(1) sleep(1);
    }

    result = mmpSARSetWriteBufferSize(writeBuffer_len);
    if (result)
        printf("mmpSARSetWriteBufferSize() error (0x%x) !!\n", result);

    printf("[SARADC] writeBuffer_len: %d\n", writeBuffer_len);
    writeBuffer = (uint8_t*)malloc(writeBuffer_len);
    if (!writeBuffer)
    {
        printf("malloc fail\n");
        while(1) sleep(1);
    }
    else
    {
        printf("[SARADC] writeBuffer_addr: %x\n", writeBuffer);
        memset(writeBuffer, 0, writeBuffer_len);
    }

    result = mmpSARSetMEMBase(channel, writeBuffer);
    if (result)
        printf("mmpSARSetMEMBase() error (0x%x) !!\n", result);

    result = mmpSARReadWritePointer(channel, &read_ptr);
    if (result)
        printf("mmpSARReadWritePointer() error (0x%x) !!\n", result);

    result = mmpSARSetReadPointer(channel, read_ptr);
    if (result)
        printf("mmpSARSetReadPointer() error (0x%x) !!\n", result);

    printf("[SARADC] read pointer setting: %d\n", read_ptr);

    switch (channel)
    {
        case 0:
            range_min = 0x559;
            range_max = 0x6e9;
            break;

        case 1:
            range_min = 0x45d;
            range_max = 0x5df;
            break;

        case 2:
            range_min = 0x4e5;
            range_max = 0x66c;
            break;

        case 3:
            range_min = 0x66c;
            range_max = 0x7f6;
            break;

        case 4:
            range_min = 0x9ff;
            range_max = 0xa98;
            break;

        case 5:
            range_min = 0xb1d;
            range_max = 0xb98;
            break;

        case 6:
            range_min = 0x887;
            range_max = 0x8ff;
            break;

        case 7:
            range_min = 0xf36;
            range_max = 0xfff;
            break;

        default:
            break;
    }

    result = mmpSARSetAVGDetectRule(channel, range_max, range_min);
    if (result)
        printf("mmpSARSetAVGDetectRule() error (0x%x) !!\n", result);

    printf("[SARADC] channel%d enable!\n", channel);

    result = mmpSARFire();
    if (result)
        printf("mmpSARFire() error (0x%x) !!\n", result);

    printf("[SARADC] SARADC is writing memory ");
    printf_count = 0;
    while (mmpSARIsOverwritingMEM(channel) != true)
    {
        if (printf_count % (1 * 100 * 1000) == 0)
        {
            printf(".");
            fflush(stdout);
        }
        printf_count++;
        usleep(1);
    }
    printf("\n");
    printf("[SARADC] finish!\n");

    printf("[SARADC] channel%d disable!\n", channel);

    result = mmpSARStop();
    if (result)
        printf("mmpSARStop() error (0x%x) !!\n", result);

    result = mmpSARReadWritePointer(channel, &write_ptr);
    if (result)
        printf("mmpSARReadWritePointer() error (0x%x) !!\n", result);

    printf("[SARADC] write pointer setting: %d\n", write_ptr);

    ithInvalidateDCacheRange((void *)writeBuffer, writeBuffer_len);

    printf_chk = write_ptr + 8;
    if (printf_chk == writeBuffer_len)
    {
        printf_chk = 0;
        tail_check = 1;
    }
    else if (printf_chk > writeBuffer_len)
    {
        printf("check data error 1(out of range)\n");
        while(1) sleep(1);
    }

    while (check_break == 0)
    {
        if ((printf_chk % SARADC_output_set_byte == SARADC_output_set_valid_byte) ||
                (printf_chk % SARADC_output_set_byte == SARADC_output_set_valid_byte + 1))
            goto end;
        else if (printf_chk % SARADC_output_set_byte == 0)
            unit_start = printf_chk % SARADC_output_unit2_byte;

        temp = *(writeBuffer + printf_chk);
        if ((printf_chk - unit_start) % SARADC_output_unit2_byte == 0)
        {
            buffer = temp;
        }
        else if ((printf_chk - unit_start) % SARADC_output_unit2_byte == 1)
        {
            buffer |= (temp & 0xf) << 8;
            index++;
            //printf("[%d,%d]:%x\n", index++, printf_chk, buffer);

            if (printf_chk == 1)
            {
                mininum = buffer;
                maximum = buffer;
                average += buffer;
            }
            else
            {
                if (buffer < mininum) mininum = buffer;
                else if (buffer > maximum) maximum = buffer;
                average += buffer;
            }

            buffer = (temp & 0xf0) >> 4;
        }
        else
        {
            buffer |= temp << 4;
            index++;
            //printf("[%d,%d]:%x\n", index++, printf_chk, buffer);

            if (buffer < mininum) mininum = buffer;
            else if (buffer > maximum) maximum = buffer;

            average += buffer;
        }

end:
        printf_chk++;
        if (tail_check)
        {
            if (printf_chk == writeBuffer_len)
            {
                check_break = 1;
            }
            else if (printf_chk > writeBuffer_len)
            {
                printf("check data error 2(out of range)\n");
                while(1) sleep(1);
            }
        }
        else
        {
            if (printf_chk == writeBuffer_len)
            {
                printf_chk = 0;
            }
            else if (printf_chk == write_ptr + 8)
            {
                check_break = 1;
            }
            else if (printf_chk > writeBuffer_len)
            {
                printf("check data error 3(out of range)\n");
                while(1) sleep(1);
            }
        }
    }

    printf("%d outputs\nmin:%x\nmax:%x\naverage:%x\n", index, mininum, maximum, average/index);

#ifdef USB_RECORD
    SARADC_FD = fopen("A:/test_saradc.txt", "a");
    if (SARADC_FD == NULL)
    {
        printf("SARADC_FD fail\n");
        while(1) sleep(1);
    }
    else
    {
        unsigned char temp[16] = {0};
        sprintf(temp, "ch%d:%x\r\n", channel, average/index);
        fwrite(temp, strlen(temp), 1, SARADC_FD);
        fclose(SARADC_FD);
    }
#endif

    printf("mmpSARIsOverwritingMEM: %d\n", mmpSARIsOverwritingMEM(channel));
    printf("mmpSARIsOutOfRange: %d\n", mmpSARIsOutOfRange(channel));
    if (mmpSARIsOverwritingMEM(channel))
    {
        printf("Continue(overwrite) ...\n");
        //while(1) sleep(1);
    }
    if (mmpSARIsOutOfRange(channel))
    {
        printf("Error(out of range) ...\n");
        while(1) sleep(1);
    }

    if (writeBuffer) free(writeBuffer);

    return 1;
}

#ifdef USB_RECORD
void ShowDriveTable()
{
    ITPDriveStatus* driveStatusTable;
    int i = 0;
    ioctl(ITP_DEVICE_DRIVE, ITP_IOCTL_GET_TABLE, &driveStatusTable);
    printf("================= ShowDriveTable =================\n");
    for (i = 0; i < ITP_MAX_DRIVE; i++)
    {
        ITPDriveStatus* driveStatus = &driveStatusTable[i];
        printf("disc: %d, device: %d, ava: %d, name: %s\n", driveStatus->disk, driveStatus->device, driveStatus->avail, driveStatus->name);
    }
}
#endif

void* TestFunc(void* arg)
{
    SARADC_RESULT result = SARADC_SUCCESS;
    int channel = 0;

    printf("test saradc project!\n");
    itpInit();

#ifdef USB_RECORD
    ShowDriveTable();
#endif

    result = mmpSARInitialize(SARADC_MODE_SRAM_INTR, SARADC_AMPLIFY_1X, SARADC_CLK_DIV_9);
    if (result)
        printf("mmpSARInitialize() error (0x%x) !!\n", result);

    result = mmpSARTerminate();
    if (result)
        printf("mmpSARTerminate() error (0x%x) !!\n", result);

    result = mmpSARInitialize(SARADC_MODE_SRAM_INTR, SARADC_AMPLIFY_1X, SARADC_CLK_DIV_9);
    if (result)
        printf("mmpSARInitialize() error (0x%x) !!\n", result);

    while (channel != 4)
    {
        printf("================= test ch:%d =================\n", channel);
        if (testSARADC(channel))
        {
            printf("Success...\n");
            sleep(1);
        }
        else
        {
            printf("Fail...\n");
            break;
        }
        channel++;
        if (channel == 4) channel = 0;
    }

    return 0;
}
