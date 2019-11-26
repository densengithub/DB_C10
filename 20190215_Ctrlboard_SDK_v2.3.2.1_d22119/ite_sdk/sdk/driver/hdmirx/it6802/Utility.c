///*****************************************
//  Copyright (C) 2009-2018
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <Utility.c>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2018/05/10
//   @fileversion: ITE_MHLRX_SAMPLE_V1.27
//******************************************/
#include <sys/time.h>
#include "stdio.h"
#include "hdmirx/it6802/config.h"
#include "hdmirx/it6802/IO.h"
#include "hdmirx/it6802/Utility.h"


unsigned int prevTickCount;

unsigned int CalTimer(void)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) != 0)
        printf("gettimeofday failed!\n");
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
    
}
unsigned int CalTimerDuration(unsigned int clock)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) != 0)
        printf("gettimeofday failed!\n");
    return tv.tv_sec * 1000 + tv.tv_usec / 1000 - clock;
    
}

int TimeOutCheck(unsigned int timer, unsigned int x)
{
    if(CalTimerDuration(timer)>=x)
    {
        return TRUE;
    }
    return FALSE;
}


BOOL IsTimeOut(unsigned int x)
{
    if(CalTimerDuration(prevTickCount) >= x )
    {
        prevTickCount = CalTimer();
        return TRUE;
    }
    return FALSE;
}

unsigned int GetCurrentVirtualTime()
{
    return CalTimer();
}


void delay1ms(unsigned int ms)
{
    unsigned int tickCount = CalTimer();
    while(CalTimerDuration(tickCount) < ms ) ;
}

