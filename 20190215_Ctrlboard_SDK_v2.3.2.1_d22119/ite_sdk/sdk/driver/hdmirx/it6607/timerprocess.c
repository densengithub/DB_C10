///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <TimerProcess.c>
//   @author jau-chih.tseng@ite.com.tw
//   @date   2012/05/16
//   @fileversion: HDMIRX_SAMPLE_3.11
//******************************************/
#include <sys/time.h>
#include <ite/mmp_types.h>
#include "hdmirx/it6607/TimerProcess.h"
#include "ite/mmp_types.h"
#include "linux/timer.h"
//#include "IO.h"
//volatile unsigned int idata ucTickCount;
unsigned int prevTickCount = 0 ;
//volatile TimerTask_Description idata TimerTask[MAX_TimerTaskC];
static unsigned int PalGetClock(void)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) != 0)
        printf("gettimeofday failed!\n");
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
static unsigned long PalGetDuration(unsigned int clock)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) != 0)
        printf("gettimeofday failed!\n");
    return (unsigned int)(tv.tv_sec * 1000 + tv.tv_usec / 1000) - clock;
}

unsigned int GetCurrentVirtualTime()
{
    return PalGetClock();
}

BOOL IsTimeOut(unsigned int x)
{
    if(PalGetDuration(prevTickCount) >= x )
    {
        prevTickCount = PalGetClock(); ;
        return TRUE ;
    }
    return FALSE ;
}

BOOL TimeOutCheck(unsigned int timer, unsigned int x)
{
    if(PalGetDuration(timer)>=x)
    {
        return TRUE ;
    }
    return FALSE ;
}

void delay1ms(unsigned int ms)
{
    unsigned int tickCount = PalGetClock();
    while(PalGetDuration(tickCount) < ms ) ;
}
