/**
 * Copyright (c), 2012~2018 iot.10086.cn All Rights Reserved
 * @file    err.h
 * @date    2018/07/20
 * @brief   错误码定义
 */

#ifndef __ERR_H__
#define __ERR_H__

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
/** 成功*/
#define ERR_OK 0

/** 内存不足*/
#define ERR_NO_MEM -1
/** 接口参数错误*/
#define ERR_INVALID_PARAM -2
/** 数据错误*/
#define ERR_INVALID_DATA -3
/** 资源忙*/
#define ERR_RESOURCE_BUSY -4
/** 设备未初始化*/
#define ERR_UNINITIALIZED -5
/** 设备初始化未完成*/
#define ERR_INITIALIZING -6

/** 模组通信超时*/
#define ERR_MOD_TIMEOUT -100
/** 功能不支持*/
#define ERR_MOD_NOT_SUPPORT -101
/** 功能参数错误*/
#define ERR_MOD_INVALID_PARAM -102
/** 重复的操作*/
#define ERR_MOD_REPEAT_OPERATE -103
/** 模组未初始化*/
#define ERR_MOD_UNINITIALIZED -104
/** 设备不在线*/
#define ERR_MOD_OFFLINE -105
/** 模组忙*/
#define ERR_MOD_BUSY -106

/*****************************************************************************/
/* External Structures, Enum and Typedefs                                    */
/*****************************************************************************/

/*****************************************************************************/
/* External Function Prototypes                                              */
/*****************************************************************************/

#ifdef _cplusplus
}
#endif

#endif
