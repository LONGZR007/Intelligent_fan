/**
 * Copyright (c), 2012~2019 iot.10086.cn All Rights Reserved
 * @file    func_ops.c
 * @date    2019/04/22
 * @brief   功能点操作接口，用于功能点值的设置及获取。接口列表由平台根
 *          据用户定义的产品功能点自动生成，用户需在接口内实现操作逻辑
 */

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/
#include "func_ops.h"
#include "protocol.h"
#include "device.h"
#include "err.h"
#include "osl.h"

/*****************************************************************************/
/* Local Definitions ( Constant and Macro )                                  */
/*****************************************************************************/
#define SET_BIT_64(x, offset) (x | ((uint64)1 << (offset)))
#define CLEAR_BIT_64(x, offset) (x & ~((uint64)1 << (offset)))

#define FAULT_STATUS(x, y, z) (x = (z) ? SET_BIT_64(x, y) : CLEAR_BIT_64(x, y))

/*****************************************************************************/
/* Structures, Enum and Typedefs                                             */
/*****************************************************************************/

/*****************************************************************************/
/* Local Function Prototype                                                  */
/*****************************************************************************/

/*****************************************************************************/
/* Local Variables                                                           */
/*****************************************************************************/
/****************************** Func List !!! ********************************/
static struct device_func_handle_t dev_func_list[] = {
    FUNC_HANDLE_BOTH(switch, 1, BOOL), 
    FUNC_HANDLE_BOTH(windspeed, 2, ENUM), 
    FUNC_HANDLE_BOTH(countdown, 4, ENUM), 
    FUNC_HANDLE_UP(countdown_left, 5, INT) 
};
/***************************** Auto Generated !!! ****************************/

/******************************* Error Code !!! ******************************/
/***************************** Auto Generated !!! ****************************/

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* Function Implementation                                                   */
/*****************************************************************************/
int32 func_set_val(uint16 func_id, uint8 *val, uint16 val_len)
{
    uint16 i = 0;
    
    for (i = 0; i < ARRAY_SIZE(dev_func_list); i++)
    {
        if (func_id == dev_func_list[i].info.func_id)
        {
            return dev_func_list[i].func_set(val, val_len);
        }
    }
    return ERR_INVALID_PARAM;
}

int32 func_get_val(uint16 func_id, uint8 *buf, uint16 buf_len, uint16 func_lv)
{
    uint16 query_len = 0;
    uint16 offset = 0;
    uint16 i = 0;

    for (i = 0; i < ARRAY_SIZE(dev_func_list); i++)
    {
        if (((0 == func_id) || (func_id == dev_func_list[i].info.func_id)) && dev_func_list[i].func_get)
        {
            uint32 func_info = (func_lv << 16) | dev_func_list[i].info.func_id;

            /** 最短的数据（BOOL型）打包长度需要11字节*/
            if ((buf_len - query_len) < 11)
                return 0;

            /** 打包功能点信息*/
            query_len += tlv_pack(buf + query_len, TLV_INT, (uint8 *)&func_info, 4);

            /** 打包值*/
            buf[query_len++] = dev_func_list[i].info.data_type;
            if (0 == (offset = dev_func_list[i].func_get(buf + query_len + 2, buf_len - query_len - 2)))
                return 0;
            buf[query_len++] = offset & 0xFF;
            buf[query_len++] = (offset >> 8) & 0xFF;
            query_len += offset;
        }
    }
    return query_len;
}

int32 binary_to_int(uint8 *val)
{
    int32 value = 0;
    osl_memcpy(&value, val, sizeof(int32));
    return value;
}

float64 binary_to_float(uint8 *val)
{
    float64 value = 0;
    osl_memcpy(&value, val, sizeof(float64));
    return value;
}

static int32 set_by_binary(void *value, uint8 *val, uint16 buf_len, uint16 val_len)
{
    if (buf_len < val_len)
        return 0; 
    osl_memcpy(val, value, val_len);
    return val_len;
}

/******************************** Cmd API !!! ********************************/
#include "./fan/app_fan.h"
int32 func_switch_set(uint8 *val, uint16 val_len)
{
    boolean value = (boolean)(*val);
    /** 根据变量value的值，填入下发控制逻辑 */

	if (value == 1)
	{
		FAN_OPEN();
	}
	else if (value == 0)
	{
		FAN_CLOSE();
	}
    
    /***/
    return 0;
}

int32 func_windspeed_set(uint8 *val, uint16 val_len)
{
    enum windspeed_enum_t value = (enum windspeed_enum_t)(*val);
    /** 根据变量value的值，填入下发控制逻辑 */

    if (value == WINDSPEED_ENUM_1)
	{
		FAN_OPEN();
	}
	else if (value == WINDSPEED_ENUM_0)
	{
		FAN_CLOSE();
	}
    /***/
    return 0;
}

int32 func_countdown_set(uint8 *val, uint16 val_len)
{
    enum countdown_enum_t value = (enum countdown_enum_t)(*val);
    /** 根据变量value的值，填入下发控制逻辑 */

    Countdown = value * 60 * 60;    // 将小时转换成秒
    /***/
    return 0;
}

/***************************** Auto Generated !!! ****************************/

/*********************** Get Upload And Query Value !!! **********************/
int32 func_switch_get(uint8 *val, uint16 buf_len)
{
    boolean value;          
    /** 请填入功能点值的获取逻辑，并将值赋给变量value */

	if(FAN_GPIO_PORT->ODR & FAN_PIN)
	{
		value = 1;
	}
	else
	{
		value = 0;
	}
    
    /***/
    return set_by_binary(&value, val, buf_len, sizeof(boolean));
}

int32 func_windspeed_get(uint8 *val, uint16 buf_len)
{
    enum windspeed_enum_t value;
    /** 请填入功能点值的获取逻辑，并将值赋给变量value */

	if(FAN_GPIO_PORT->ODR & FAN_PIN)
	{
		value = WINDSPEED_ENUM_1;
	}
	else
	{
		value = WINDSPEED_ENUM_0;
	}
    
    /***/
    return set_by_binary(&value, val, buf_len, sizeof(uint8));
}

int32 func_countdown_get(uint8 *val, uint16 buf_len)
{
    enum countdown_enum_t value;
    /** 请填入功能点值的获取逻辑，并将值赋给变量value */
	
	uint8_t value_temp = Countdown_Init / 60 / 60;
	
	if (value_temp == 0 && Countdown_Init > 0)
	{
		value_temp = COUNTDOWN_ENUM_1;
	}
	
	switch(value_temp)
	{
		case 0:
		{
			value = COUNTDOWN_ENUM_0;
		}
		break;
		
		case 1:
		{
			value = COUNTDOWN_ENUM_1;
		}
		break;
		
		case 2:
		{
			value = COUNTDOWN_ENUM_2;
		}
		break;
		
		case 3:
		{
			value = COUNTDOWN_ENUM_3;
		}
		break;
		
		case 4:
		{
			value = COUNTDOWN_ENUM_4;
		}
		break;
		
		case 5:
		{
			value = COUNTDOWN_ENUM_5;
		}
		break;
		
		case 6:
		{
			value = COUNTDOWN_ENUM_6;
		}
		break;
		
		case 7:
		{
			value = COUNTDOWN_ENUM_7;
		}
		break;
		
		default:
			value = COUNTDOWN_ENUM_0;
	}

    /***/
    return set_by_binary(&value, val, buf_len, sizeof(uint8));
}

int32 func_countdown_left_get(uint8 *val, uint16 buf_len)
{
    int32 value;
    /** 请填入功能点值的获取逻辑，并将值赋给变量value */

    value = Countdown/60;
	
    /***/
    return set_by_binary(&value, val, buf_len, sizeof(int32));
}

/***************************** Auto Generated !!! ****************************/

/******************************** Upload API !!! *****************************/
int32 func_switch_upload(void)
{
	func_windspeed_upload();
    return device_upload(1, 0);
}

int32 func_windspeed_upload(void)
{
    return device_upload(2, 0);
}

int32 func_countdown_upload(void)
{
    return device_upload(4, 0);
}

int32 func_countdown_left_upload(void)
{
    return device_upload(5, 0);
}

/***************************** Auto Generated !!! ****************************/
