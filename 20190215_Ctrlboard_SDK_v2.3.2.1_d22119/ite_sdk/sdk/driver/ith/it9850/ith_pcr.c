/*
 * Copyright (c) 2011 ITE Technology Corp. All Rights Reserved.
 *
 * @description Used as System Time Clock in front panel control.
 * @file ${Program}/ith_stc.c
 * @author Irene Wang
 * @version 1.0.0
 */
#include <pthread.h>
#include "ite/ith.h"

static pthread_mutex_t stc_mutex = PTHREAD_MUTEX_INITIALIZER;
static uint32_t stcBaseCountLo = 0;

uint64_t ithStcGetBaseClock64(STCInfo *pstc_info)
{
    uint32_t baseCount;
    uint64_t tempBaseCount;
    
    pthread_mutex_lock(&stc_mutex);
    baseCount = ithStcGetBaseClock();
    if(baseCount < stcBaseCountLo)
            {
        pstc_info->stcBaseCountHi++;
    }
    stcBaseCountLo = baseCount;
    //printf("[%s] hi=%u lo = %u\n", __FUNCTION__, pstc_info->stcBaseCountHi, stcBaseCountLo);
    tempBaseCount = ((uint64_t)pstc_info->stcBaseCountHi << 32) | stcBaseCountLo;
    tempBaseCount = (tempBaseCount * 9)/10;
    pthread_mutex_unlock(&stc_mutex);
    return tempBaseCount;
}

