/*
 * MemManage.h
 *
 *  Created on: 2018年9月12日
 *      Author: 陆相如
 */

#ifndef USER_MEMMANAGE_H_
#define USER_MEMMANAGE_H_

#include "stddef.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void nmemset(void *_Dst, int _Val, size_t _Size);
    int nmemcmp(const void *_Buf1, const void *_Buf2, size_t _Size);
    void *nmemcpy(void *_Dst, const void *_Src, size_t _Size);

#ifdef __cplusplus
}
#endif

#endif /* USER_MEMMANAGE_H_ */
