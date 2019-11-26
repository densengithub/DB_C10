#include <sys/time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/ioctl.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>
#include <sys/statvfs.h>
#include "saradc/saradc_970/saradc.h"
#include "ite/itp.h"
#include "scene.h"
#include "project.h"

// #define _MGT_Color_
#define _NGT_Color_
#define SARADC_output_set_byte       8
#define SARADC_output_set_valid_byte 6
#define SARADC_output_unit2_byte     3

int testSARADC(int channel,int* adc_value)
{
    SARADC_RESULT result = SARADC_SUCCESS;
    uint32_t writeBuffer_len = 128;
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

    // if (writeBuffer_len < 8)
    // {
    //     printf("writeBuffer_len can not be less than 8 !!\n");
    //     while(1) sleep(1);
    // }

    result = mmpSAREnableXAIN(0x1 << channel);
    if (result)
    {
        printf("mmpSAREnableXAIN() error (0x%x) !!\n", result);
        while(1) sleep(1);
    }

    result = mmpSARSetWriteBufferSize(writeBuffer_len);
    if (result)
        printf("mmpSARSetWriteBufferSize() error (0x%x) !!\n", result);

    // printf("[SARADC] writeBuffer_len: %d\n", writeBuffer_len);
    writeBuffer = (uint8_t*)malloc(writeBuffer_len);
    if (!writeBuffer)
    {
        printf("malloc fail\n");
        while(1) sleep(1);
    }
    else
    {
        // printf("[SARADC] writeBuffer_addr: %x\n", writeBuffer);
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

    // printf("[SARADC] read pointer setting: %d\n", read_ptr);

    range_min = 0x03f;
    range_max = 0xfff;    

    result = mmpSARSetAVGDetectRule(channel, range_max, range_min);
    if (result)
        printf("mmpSARSetAVGDetectRule() error (0x%x) !!\n", result);

    // printf("[SARADC] channel%d enable!\n", channel);

    result = mmpSARFire();
    if (result)
        printf("mmpSARFire() error (0x%x) !!\n", result);

    // printf("[SARADC] SARADC is writing memory ");
    printf_count = 0;
    while (mmpSARIsOverwritingMEM(channel) != true)
    {
        // if (printf_count % (1 * 100 * 1000) == 0)
        // {
        //     printf(".");
        //     fflush(stdout);
        // }
        // printf_count++;
        usleep(1);
    }
    // printf("\n");
    // printf("[SARADC] finish!\n");

    // printf("[SARADC] channel%d disable!\n", channel);

    result = mmpSARStop();
    if (result)
        printf("mmpSARStop() error (0x%x) !!\n", result);

    result = mmpSARReadWritePointer(channel, &write_ptr);
    if (result)
        printf("mmpSARReadWritePointer() error (0x%x) !!\n", result);

    // printf("[SARADC] write pointer setting: %d\n", write_ptr);

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
    *adc_value = average / index;
    // printf("\n%d outputs,min:0x%x,max:0x%x,avg:0x%x\t\n", index, mininum, maximum, *adc_value);
    

    // printf("mmpSARIsOverwritingMEM: %d\n", mmpSARIsOverwritingMEM(channel));
    // printf("mmpSARIsOutOfRange: %d\n", mmpSARIsOutOfRange(channel));
    // if (mmpSARIsOverwritingMEM(channel))
    // {
    //     printf("Continue(overwrite) ...\n");
    //     //while(1) sleep(1);
    // }
    // if (mmpSARIsOutOfRange(channel))
    // {
    //     printf("Error(out of range) ...\n");
    //     while(1) sleep(1);
    // }

    if (writeBuffer) free(writeBuffer);

    return 1;
}
int buff_filter[10] = {0};
void AD_Filter(int ad_value,int* filter_value)
{
    int i = 0, sum = 0,j=0;
    static int first_d = 1;
    if (first_d == 1)
    {
        for (i = 0; i < 6;i++)
        {
            buff_filter[i] = ad_value;
            // sum += ad_value;
        }
        // *filt_value = sum / 10;
    }
    else
    {
        for (i = 0; i < 6;i++)
        {
            if (i == 5)
            {
                buff_filter[i] = ad_value;
            }
            else
            {
                buff_filter[i] = buff_filter[i + 1];
            }  
        }
        for (i = 0; i < 5; i++)
        {
            for (j = 0; j < 5; j++)
            {
                if (buff_filter[j] < buff_filter[j + 1])
                {
                    sum = buff_filter[j];
                    buff_filter[j] = buff_filter[j + 1];
                    buff_filter[j + 1] = sum;
                }
            }
        }       
    }
    sum = 0;
    for (i = 1; i < 5; i++)
    {
        sum += buff_filter[i];
    }
    *filter_value = sum >> 2;
}

void* AppPsensorTask(void* arg)
{
    SARADC_RESULT result = SARADC_SUCCESS;
    int channel = 0;
    int Adc_value = 0,Adc_value_file = 0;
    printf("saradc project!\n");  

    result = mmpSARInitialize(SARADC_MODE_SRAM_INTR, SARADC_AMPLIFY_1X, SARADC_CLK_DIV_9);
    if (result)
        printf("mmpSARInitialize() error (0x%x) !!\n", result);

    result = mmpSARTerminate();
    if (result)
        printf("mmpSARTerminate() error (0x%x) !!\n", result);

    result = mmpSARInitialize(SARADC_MODE_SRAM_INTR, SARADC_AMPLIFY_1X, SARADC_CLK_DIV_9);
    if (result)
        printf("mmpSARInitialize() error (0x%x) !!\n", result);
 	    
    while(!SceneReadReady())
    {
        usleep(10*1000);
    }   
    while (1)
    {
        // printf("================= test ch:%d =================\n", channel);
        if (testSARADC(channel,&Adc_value))
        {            
            AD_Filter(Adc_value, &Adc_value_file);
            if (testmode_status)
            {
                ev_layer2.layer_displaydata.DashBoardInfo.test_psensor1_ad = (unsigned short)Adc_value;
            }
            
            #ifdef _MGT_Color_            
            Adc_value_file *= 100;           
            Adc_value_file /= (0x0fff);
            printf("Adc_value=%d\t\n",Adc_value_file);

            if (Adc_value_file < 5)
            {
                Adc_value_file = 5;
            }            
            #elif defined(_NGT_Color_)
            // printf("Adc_value=0x%x,",Adc_value_file);
            Adc_value_file *= 100;            
            Adc_value_file /= ((0xfff >> 3) + 200);
            #endif
            // printf("%d%%\t\n",Adc_value_file);
            ScreenSetBrightness(Adc_value_file);
        }
        else
        {
            // printf("Fail...\n");            
        }
        sleep(1);        
    }
    
    return 0;
}

int CreateAppPsensorTask(void)
{
#ifndef _WIN32
    
    int res = 0;
	pthread_t task_psensor;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	// attr.stacksize = TEST_STACK_SIZE;	

	printf("Create AppPsensorTask!.......\n");
	res = pthread_create(&task_psensor, &attr, AppPsensorTask, NULL);
	if (res)
	{
		printf("[TouchPanel]%s() L#%ld: ERROR, create UartttlComunicationTask() thread fail! res=%ld\n", res);
		return 1;
	}
	// printf("Creat AppPsensorTask  finished!.......\n");
	return 0;
#else

    return 0;
#endif
}