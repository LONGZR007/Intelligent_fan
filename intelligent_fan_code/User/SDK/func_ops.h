/**
 * Copyright (c), 2012~2019 iot.10086.cn All Rights Reserved
 * @file    func_ops.h
 * @date    2019/04/22
 * @brief   功能点操作接口文件，接口列表由平台根据用户的功能点定义自动生成
 */

#ifndef __FUNC_OPS_H__
#define __FUNC_OPS_H__

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/
#include "c_types.h"

#ifdef _cplusplus
extern "C"
{
#endif

/*****************************************************************************/
/* External Definition ( Constant and Macro )                                */
/*****************************************************************************/
#define FUNC_LEVEL_UPLOAD     0x03
#define FUNC_LEVEL_QUERY      0x05
#define FUNC_LEVEL_EVENT      0x06
    
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define FUNC_INFO(y, z)             \
    {                               \
        y, TYPE_##z                 \
    }

/** 可上报可下发*/
#define FUNC_HANDLE_BOTH(x, y, z)                           \
    {                                                       \
        FUNC_INFO(y, z), func_##x##_set, func_##x##_get     \
    }

/** 只上报*/
#define FUNC_HANDLE_UP(x, y, z)                             \
    {                                                       \
        FUNC_INFO(y, z), NULL, func_##x##_get               \
    }

/** 只下发*/
#define FUNC_HANDLE_DOWN(x, y, z)                           \
    {                                                       \
        FUNC_INFO(y, z), func_##x##_set, NULL               \
    }

/*****************************************************************************/
/* External Structures, Enum and Typedefs                                    */
/*****************************************************************************/    
enum device_data_type_t
{
    /** 布尔型*/
    TYPE_BOOL = 1,
    /** 枚举型*/
    TYPE_ENUM,
    /** 数值型 - 整数*/
    TYPE_INT,
    /** 数值型 - 浮点数*/
    TYPE_FLOAT,
    /** 透传型*/
    TYPE_BINARY,
    /** 故障型*/
    TYPE_ERROR,
    /** 字符型*/
    TYPE_STRING,
    /** 事件型*/
    TYPE_EVENT,
    TYPE_DUMMY = 0x7FFFFFFF
};

struct device_func_info_t
{
    uint32 func_id;
    enum device_data_type_t data_type;
};

typedef int32 (*device_func_set)(uint8 *val, uint16 val_len);
typedef int32 (*device_func_get)(uint8 *val, uint16 buf_len);

struct device_func_handle_t
{
    struct device_func_info_t info;
    device_func_set func_set;
    device_func_get func_get;
};

/***************************** Function Type !!! *****************************/
enum windspeed_enum_t
{
    WINDSPEED_ENUM_0,
    WINDSPEED_ENUM_1
};

enum countdown_enum_t
{
    COUNTDOWN_ENUM_0,
    COUNTDOWN_ENUM_1,
    COUNTDOWN_ENUM_2,
    COUNTDOWN_ENUM_3,
    COUNTDOWN_ENUM_4,
    COUNTDOWN_ENUM_5,
    COUNTDOWN_ENUM_6,
    COUNTDOWN_ENUM_7
};

/***************************   Auto Generated !!! ****************************/

/*****************************************************************************/
/* External Variables and Functions                                          */
/*****************************************************************************/
int32 func_set_val(uint16 func_id, uint8 *val, uint16 val_len);

int32 func_get_val(uint16 func_id, uint8 *buf, uint16 buf_len, uint16 func_lv);

/******************************** Cmd API !!! ********************************/
int32 func_switch_set(uint8 *val, uint16 val_len);

int32 func_windspeed_set(uint8 *val, uint16 val_len);

int32 func_countdown_set(uint8 *val, uint16 val_len);

/***************************** Auto Generated !!! ****************************/

/*********************** Get Upload And Query Value !!! **********************/
int32 func_switch_get(uint8 *val, uint16 buf_len);

int32 func_windspeed_get(uint8 *val, uint16 buf_len);

int32 func_countdown_get(uint8 *val, uint16 buf_len);

int32 func_countdown_left_get(uint8 *val, uint16 buf_len);

/***************************** Auto Generated !!! ****************************/

/******************************** Upload API !!! *****************************/
int32 func_switch_upload(void);

int32 func_windspeed_upload(void);

int32 func_countdown_upload(void);

int32 func_countdown_left_upload(void);

/***************************** Auto Generated !!! ****************************/

#ifdef _cplusplus
}
#endif

#endif
