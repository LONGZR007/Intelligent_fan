/**
 * Copyright (c), 2012~2019 iot.10086.cn All Rights Reserved
 * @file    device.h
 * @date    2019/04/22
 * @brief   设备接口，包括设备初始化、连接平台、消息处理等
 */

#ifndef __DEVICE_H__
#define __DEVICE_H__ 

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/
#include "func_ops.h"

#ifdef _cplusplus
extern "C"{
#endif   

/*****************************************************************************/
/* External Structures, Enum and Typedefs                                    */
/*****************************************************************************/
enum device_evt_id_t
{   
    /** 设备连接到无线网络*/
    DEVICE_EVT_ID_NET_CONNECTED = 0,
    /** 设备从无线网络断开*/
    DEVICE_EVT_ID_NET_DISCONNECTED,
    /** 设备连接到OneNET平台*/
    DEVICE_EVT_ID_ONENET_CONNECTED,
    /** 设备与OneNET平台断开*/
    DEVICE_EVT_ID_ONENET_DISCONNECTED,
    /** 模组被远程复位*/
    DEVICE_EVT_ID_MODULE_RESET,
    /** 智能配网超时，默认时间2分钟*/
    DEVICE_EVT_ID_SMARTCONFIG_TIMEOUT,
    /** 产测完成*/
    DEVICE_EVT_ID_FACTORY_TEST_OVER,
    /** 网络时间同步*/
    DEVICE_EVT_ID_SYNC_TIME,
    /** 暂不支持*/
    DEVICE_EVT_ID_FOTA_REQUEST,
    /** 暂不支持*/
    DEVICE_EVT_ID_FOTA_DATA,
    DEVICE_EVT_ID_DUMMY = 0x7FFFFFFF
};

struct device_event_t
{
    enum device_evt_id_t evt_id;
    uint8 *evt_data;
    uint16 evt_data_len;
    uint16 reserved;
};

enum smartconfig_type_t
{
    /** 智能模式*/
    SMARTCONFIG_AUTO = 0,
    /** SoftAP模式*/
    SMARTCONFIG_SOFTAP
};

/*****************************************************************************/
/* CallBack Function Prototypes                                              */
/*****************************************************************************/
// ---------------------------------------------------------------------------------------
// |	         evt.evt_id     		|  evt.evt_data	|             返回值    			 |
// ---------------------------------------------------------------------------------------
// |            NET_CONNECTED    		|  1字节信号强度	|               0                |
// |           NET_DISCONNECTED  		|      无       	|               0                |
// |           ONENET_CONNECTED  		|      无       	|               0                |
// |         ONENET_DISCONNECTED 		|      无       	|               0                |
// |     DEVICE_EVT_ID_MODULE_RESET  	|      无       	|               0                |
// |  DEVICE_EVT_ID_SMARTCONFIG_TIMEOUT	|      无       	|               0                |
// |   DEVICE_EVT_ID_FACTORY_TEST_OVER	|  1字节信号强度  	|               0                |
// |             SYNC_TIME     			| 4字节Unix时间戳	|               0                |
// |            FOTA_REQUEST   			|      无       	|0 - 不升级；其它 - 升级数据的最大分片 |
// |             FOTA_DATA     			|  参见串口协议   	|          参见串口协议            |
// ---------------------------------------------------------------------------------------
/**
 * 设备事件回调函数接口
 * @param evt 事件内容
 * @return 参见上表
 */
typedef int32 (*device_event_cb)(struct device_event_t *evt);

/**
 * 通信串口数据发送接口
 * @param data 需要发送的数据
 * @param data_len 需要发送的数据长度
 * @return 参见err.h
 */
typedef int32 (*data_send_cb)(uint8 *data, uint16 data_len);

/*****************************************************************************/
/* External Function Prototypes                                              */
/*****************************************************************************/
/**
 * 通信串口数据接收函数
 * @param data 接收到的数据
 * @param data_len 接收到的数据长度
 * @return 参见err.h
 */
int32 device_data_recv(uint8 *data, uint16 data_len);

/**
 * 设备数据上传接口
 * @param func_id 功能点ID
 * @param flag 功能点类型：0:属性, 1:事件
 * @return 参见err.h
 */
int32 device_upload(uint16 func_id , uint8 flag);

/**
 * 和物设备初始化
 * @param sn 设备唯一标识，产品下唯一
 * @param event_cb 设备事件回调函数
 * @param send_cb 通信串口数据发送接口
 * @return 参见err.h
 */
int32 device_init(const int8 *sn, device_event_cb event_cb,
        data_send_cb send_cb);

/**
 * 设备消息处理接口
 * @return 参见err.h
 */
int32 device_step(void);
    
/**
 * 启动设备配网
 * @param type 配网模式
 * @return 参见err.h
 */
int32 device_smartconfig(enum smartconfig_type_t type);

/**
 * 启动模组产测，产测结果通过ID为DEVICE_EVT_ID_FACTORY_TEST_OVER的设备事件进行通知
 * @return 参见err.h
 */
int32 device_factory_test(void);

/**
 * 同步网络时间，当前时间通过ID为DEVICE_EVT_SYNC_TIME的设备事件进行通知
 * @return 参见err.h
 */
int32 device_sync_time(void);

/**
 * 模组重置
 * @return 参见err.h
 */
int32 device_reset_module(void);

/**
 * 获取模组信息
 * @param id 模组唯一ID
 * @param version 模组固件版本
 * @return
 */
int32 device_get_module_info(int8 *id, int8 *version);

/**
 * 更新模组状态，不会直接返回结果，在事件回调中通知
 * @return
 */
int32 device_get_module_status(void);


#ifdef _cplusplus
}
#endif

#endif
