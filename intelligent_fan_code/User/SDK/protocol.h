/**
 * Copyright (c), 2012~2019 iot.10086.cn All Rights Reserved
 * @file    protocol.h
 * @date    2019/04/22
 * @brief   串口协议封包解包接口文件
 */

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/
#include "c_types.h"

#ifdef _cplusplus
extern "C" {
#endif   

/*****************************************************************************/
/* External Definition ( Constant and Macro )                                */
/*****************************************************************************/
#define PROT_FRAME_SYNC   0x5A5A
    
#define PROT_FRAME_LEN_HEAD	     5
#define PROT_FRAME_LEN_CHECKSUM	 1

#define PROT_REQUEST    0
#define PROT_RESPONSE   1
    
#define TLV_TYPE_LEN    1
#define TLV_LENGTH_LEN  2

/** 数据接收缓冲区大小（默认88字节，用户可自定义）*/
#define PROT_FRAME_LEN_RECV  88

/** 数据发送缓冲区大小（默认88字节，用户可自定义）*/
#define PROT_FRAME_LEN_SEND  88

/*****************************************************************************/
/* External Structures, Enum and Typedefs                                    */
/*****************************************************************************/
enum protocol_resp_code_t
{
    RESPONSE_CODE_OK = 0,
    RESPONSE_CODE_NOT_SUPPORT,
    RESPONSE_CDDE_INVALID_PARAM,
    RESPONSE_CODE_REPEAT_OPS,
    RESPONSE_CODE_OFFLINE,
    RESPONSE_CODE_BUSY
};

enum protocol_frame_type_t
{
    FRAME_TYPE_NONE = 0x0,
    FRAME_TYPE_GET_MODULE_INFO = 0x1,
    FRAME_TYPE_CONFIG_MODULE,
    FRAME_TYPE_RESET_MODULE,
    FRAME_TYPE_KEEPALIVE,
    FRAME_TYPE_START_SMARTCONFIG,
    FRAME_TYPE_GET_MODULE_STATUS = 0x8,
    FRAME_TYPE_GET_NET_TIME,
    FRAME_TYPE_NOTIFY_MODULE_STATUS,
    FRAME_TYPE_TEST,
    FRAME_TYPE_UPGRADE_REQUEST = 0x10,
    FRAME_TYPE_UPGRADE_DATA,
    FRAME_TYPE_FUNCS_UPLOAD = 0x13,
    FRAME_TYPE_FUNCS_CMD,
    FRAME_TYPE_FUNCS_QUERY
};

enum tlv_type_t
{
    TLV_BOOL = 1,
    TLV_ENUM,
    TLV_INT,
    TLV_FLOAT,
    TLV_BINARY,
    TLV_ERROR,
    TLV_STRING
};

typedef int32 (*protocol_frame_send_cb)(uint8 *frame, uint16 frame_len);

/*****************************************************************************/
/* External Function Prototypes                                              */
/*****************************************************************************/
void protocol_data_recv(uint8 *data, uint16 data_len);

int32 protocol_frame_pack_and_send(enum protocol_frame_type_t type, uint8 is_resp,
        uint8 *data, uint16 data_len);

enum protocol_frame_type_t protocol_frame_parse(uint8 *data, uint16 *data_len);

int32 protocol_init(protocol_frame_send_cb callback);

uint8 *protocol_send_buf_malloc(uint16 data_len);

uint16 tlv_pack(uint8 *buf, enum tlv_type_t type, uint8 *val, uint16 val_len);

uint16 tlv_unpack(uint8 *buf, enum tlv_type_t *type, uint8 *val, uint16 *val_len);

#ifdef _cplusplus
}
#endif   

#endif
