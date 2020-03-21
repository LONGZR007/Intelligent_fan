/**
 * Copyright (c), 2012~2019 iot.10086.cn All Rights Reserved
 * @file    c_types.h
 * @date    2019/04/22
 * @brief   数据类型定义
 */

#ifndef __C_TYPES_H__
#define __C_TYPES_H__

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/

#ifdef _cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* External Definition ( Constant and Macro )                                */
/*****************************************************************************/
#ifndef NULL
#define NULL 0
#endif

#undef BOOL
    
/*****************************************************************************/
/* External Structures, Enum and Typedefs                                    */
/*****************************************************************************/
typedef _Bool boolean;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef char int8;
typedef short int16;
typedef int int32;
typedef long long int64;

typedef float float32;
typedef double float64;

/*****************************************************************************/
/* External Function Prototypes                                              */
/*****************************************************************************/

#ifdef _cplusplus
}
#endif

#endif
