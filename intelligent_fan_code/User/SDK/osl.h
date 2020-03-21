/**
 * Copyright (c), 2012~2019 iot.10086.cn All Rights Reserved
 * @file    osl.h
 * @date    2019/04/22
 * @brief   系统接口封装
 */

#ifndef __OSL_H__
#define __OSL_H__

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/
/** 平台编译工具包含LibC库*/
#define HAS_LIBC

/** 平台编译工具不包含LibC库*/
//#undef HAS_LIBC

#ifdef HAS_LIBC
#include <stdio.h>
#include <string.h>
#endif

#include "c_types.h"

#ifdef _cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* External Definition ( Constant and Macro )                                */
/*****************************************************************************/

/*****************************************************************************/
/* External Structures, Enum and Typedefs                                    */
/*****************************************************************************/

/*****************************************************************************/
/* External Function Prototypes                                              */
/*****************************************************************************/
void *osl_memcpy(void *dest, const void *src, uint16 n);
void *osl_memset(void *dest, uint8 val, uint16 n);
uint32 osl_time_ms(void);

#ifndef HAS_LIBC
uint32 strlen(const int8 *s);
int32 strcmp(const int8 *s1, const int8 *s2);
int32 sprintf(int8 *str, const int8 *format, ...);
#endif

#ifdef _cplusplus
}
#endif

#endif
