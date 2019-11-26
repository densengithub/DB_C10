/*
 * MemManage.c
 *
 *  Created on: 2018年9月12日
 *      Author: 陆相如
 */
#include "stdlib.h"
#include "string.h"

void nmemset(void *_Dst, int _Val, size_t _Size)
{
    memset(_Dst, _Val, _Size);
}

int nmemcmp(const void *_Buf1, const void *_Buf2, size_t _Size)
{
    return memcmp(_Buf1, _Buf2, _Size);
}

void * nmemcpy(void * _Dst, const void * _Src, size_t _Size)
{
    return memcpy(_Dst, _Src, _Size);
}
