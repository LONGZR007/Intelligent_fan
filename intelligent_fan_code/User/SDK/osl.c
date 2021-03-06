/**
 * Copyright (c), 2012~2019 iot.10086.cn All Rights Reserved
 * @file    osl.c
 * @date    2019/04/22
 * @brief   系统接口封装
 */

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/
#include <stdlib.h>
#include "osl.h"

/*****************************************************************************/
/* Local Definitions ( Constant and Macro )                                  */
/*****************************************************************************/

/*****************************************************************************/
/* Structures, Enum and Typedefs                                             */
/*****************************************************************************/

/*****************************************************************************/
/* Local Function Prototype                                                  */
/*****************************************************************************/

/*****************************************************************************/
/* Local Variables                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* External Functions and Variables                                          */
/*****************************************************************************/

/*****************************************************************************/
/* Function Implementation                                                   */
/*****************************************************************************/
void *osl_memcpy(void *dest, const void *src, uint16 n)
{
    uint16 i = 0;
    uint8 *dest_ptr = (uint8 *)dest;
    const uint8 *src_prt = (const uint8 *)src;

    if ((NULL == dest_ptr) || (NULL == src_prt))
        return NULL;

    if ((dest_ptr > src_prt) && (dest_ptr < src_prt + n))
    {
        for (i = n; i > 0; i--)
        {
            dest_ptr[i - 1] = src_prt[i - 1];
        }
    }
    else
    {
        for (i = 0; i < n; i++)
        {
            dest_ptr[i] = src_prt[i];
        }
    }

    return dest;
}

void *osl_memset(void *dest, uint8 val, uint16 n)
{
    uint16 i = 0;
    uint8 *dest_ptr = (uint8 *)dest;

    if (NULL == dest_ptr)
        return NULL;

    for (i = 0; i < n; i++)
    {
        dest_ptr[i] = val;
    }

    return dest;
}

/**
 * @brief 获取以毫秒为单位的时间计数，范围为 0 ~ 0xFFFFFFFF
 * @return 返回以毫秒为单位的计数值，计数值随时间每毫秒增加1
 */
uint32 osl_time_ms(void)
{
	uint32 HAL_GetTick(void);
	
	return HAL_GetTick();
//    #error "返回以毫秒为单位的计数值，返回后删除改行"
}
