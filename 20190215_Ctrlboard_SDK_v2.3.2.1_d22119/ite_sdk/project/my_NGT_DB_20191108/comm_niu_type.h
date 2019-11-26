/*
 * niu_type.h
 *
 *  Created on: 2018年8月9日
 *      Author: 陆相如
 */

#ifndef USER_NIU_TYPE_H_
#define USER_NIU_TYPE_H_

#include "stdint.h"
#include "stdbool.h"
#include "float.h"
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef unsigned char NUINT8;
typedef unsigned short NUINT16;
typedef unsigned int  NUINT32;
typedef unsigned long NUINT64;

typedef signed char NINT8;
typedef signed short NINT16;
typedef signed int NINT32;
typedef signed long NINT64;

typedef unsigned char NCHAR;
typedef float NFLOAT;
typedef double NDOUBLE;
typedef void NVOID;
typedef unsigned char NARRAY;

typedef bool NBOOL;

#define NTRUE   true
#define NFALSE  false
#define NNULL   (void*)0

typedef enum {NDISABLE = 0, NENABLE = !NDISABLE} FunState;

typedef struct _BUS_BUFF
{
    NUINT16 len;
    NUINT8 *buff;
} NIU_BUS_BUFF;

//#define PM_V4_0

#ifdef __cplusplus
{
#endif

#endif /* USER_NIU_TYPE_H_ */
