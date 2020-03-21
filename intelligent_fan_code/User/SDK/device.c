/**
 * Copyright (c), 2012~2019 iot.10086.cn All Rights Reserved
 * @file    device.c
 * @date    2019/04/22
 * @brief   设备接口，包括设备初始化、连接平台、消息处理等
 */

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/
#include "err.h"
#include "osl.h"

#include "product_def.h"
#include "func_ops.h"
#include "protocol.h"
#include "device.h"

/*****************************************************************************/
/* Local Definitions ( Constant and Macro )                                  */
/*****************************************************************************/
#define SDK_VERSION "1.1.2"

#define DEVICE_IS_OFFLINE() ((device.module_status & 0x02) ? 0 : 1)

/** 串口命令超时时间, 预设200ms*/
#define PROTOCOL_FRAME_TIMEOUT_SEC  200

/** 串口心跳命令间隔, 预设60s*/
#define DEVICE_KEEPALIVE_INTERVAL  60000

/** 设备SN最大长度*/
#define DEV_SN_LEN  32

/** 模组ID最大长度（根据模组不同，用户可修改）*/
#define MODULE_ID_LEN  32

/** 模组版本号最大长度*/
#define MODULE_VER_LEN  30

/*****************************************************************************/
/* Structures, Enum and Typedefs                                             */
/*****************************************************************************/
enum device_state_t
{
    DEVICE_STATE_CONFIG_MODULE = 0,
    DEVICE_STATE_RUNNING,
    DEVICE_STATE_FACTORY_TEST,
    DEVICE_STATE_DUMMY = 0x7FFFFFFF
};

struct device_t
{
    int8 *sn;
    int8 *module_id;
    int8 *module_ver;
    device_event_cb evt_cb;
    uint32 last_frame_time;
    uint8 module_status;
    enum device_state_t state;
};

/*****************************************************************************/
/* Local Function Prototype                                                  */
/*****************************************************************************/

/*****************************************************************************/
/* Local Variables                                                           */
/*****************************************************************************/
static struct device_t device;

static int8 sn_buf[DEV_SN_LEN + 1];
static int8 id_buf[MODULE_ID_LEN + 1];
static int8 ver_buf[MODULE_VER_LEN + 1];

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* External Functions and Variables                                          */
/*****************************************************************************/

/*****************************************************************************/
/* Function Implementation                                                   */
/*****************************************************************************/
static uint16 set_lv_str(uint8 *data, const int8 *str)
{
    uint16 str_len = 0;

    if (!str)
        str_len = 0;
    else
        str_len = strlen(str);
    *data++ = str_len & 0xFF;
    *data++ = (str_len >> 8) & 0xFF;
    osl_memcpy(data, str, str_len);
    return (2 + str_len);
}

static uint8 check_timeout(uint32 start, uint32 timeout)
{
    uint32 curr_time = osl_time_ms();
    uint32 interval = 0;

    interval = (curr_time >= start) ? (curr_time - start) : (0xFFFFFFFF + curr_time - start);
    return ((interval >= timeout) ? 1 : 0);
}

static int32 send_response_frame(enum protocol_frame_type_t type, uint8 *data, uint16 data_len)
{
    device.last_frame_time = osl_time_ms();
    return protocol_frame_pack_and_send(type, PROT_RESPONSE, data, data_len);
}

int32 device_data_recv(uint8 *data, uint16 data_len)
{
    protocol_data_recv(data, data_len);
    return ERR_OK;
}

int32 device_cmd_handle(uint8 *payload, uint16 payload_len)
{
    int32 ret = ERR_OK;
    uint8 *val_ptr = NULL;
    uint16 val_len = 0;
    uint16 cmd_uuid_len = 0;
    uint8 *resp_buf = NULL;
    uint16 resp_buf_len = 0;
    uint16 func_id = 0;
    uint8 val = 0;

    cmd_uuid_len = (payload[1] << 8) | payload[0];
    val_ptr = payload + 2 + cmd_uuid_len + 7 + 3;
    val_len = payload_len - 2 - cmd_uuid_len - 7 - 3;
    func_id = ((payload + 2 + cmd_uuid_len)[4] << 8) | ((payload + 2 + cmd_uuid_len)[3]);
    ret = func_set_val(func_id, val_ptr, val_len);
    resp_buf_len = 2 + cmd_uuid_len + 3 + 1;
    resp_buf = protocol_send_buf_malloc(PROT_FRAME_LEN_SEND);
    osl_memcpy(resp_buf, payload, 2 + cmd_uuid_len);
    val = (ERR_OK == ret) ? 1 : 0;
    tlv_pack((resp_buf + 2 + cmd_uuid_len), TLV_BOOL, &val, sizeof(boolean));
    send_response_frame(FRAME_TYPE_FUNCS_CMD, resp_buf, resp_buf_len);
    return ret;
}

#ifdef FUNC_QUERY_ENABLE
int32 device_query_handle(uint8 *payload, uint16 payload_len)
{
    int32 ret = ERR_OK;
    uint16 cmd_uuid_len = 0;
    uint8 *resp_buf = NULL;
    uint16 payload_offset = 0;
    uint16 resp_buf_offset = 0;
    uint16 func_id = 0;
    uint8 handle_status = 1;
    
    cmd_uuid_len = (payload[1] << 8) | payload[0];
    resp_buf = protocol_send_buf_malloc(PROT_FRAME_LEN_SEND);
    payload_offset = 2 + cmd_uuid_len;
    resp_buf_offset = 2 + cmd_uuid_len + 4;
    for (; 0 < (payload_len - payload_offset); payload_offset += 7)
    {
        func_id = payload[payload_offset + 3] | (payload[payload_offset + 4] << 8);
        ret = func_get_val(func_id, resp_buf + resp_buf_offset, PROT_FRAME_LEN_SEND
                - PROT_FRAME_LEN_HEAD - PROT_FRAME_LEN_CHECKSUM - resp_buf_offset
                , FUNC_LEVEL_QUERY);
        if (0 == ret)
        {
            handle_status = 0;
            resp_buf_offset = 2 + cmd_uuid_len + 4;
            break;
        }
        resp_buf_offset += ret;
    }
    osl_memcpy(resp_buf, payload, 2 + cmd_uuid_len);
    tlv_pack(resp_buf + 2 + cmd_uuid_len, TLV_BOOL, &handle_status, 1);
    send_response_frame(FRAME_TYPE_FUNCS_QUERY, resp_buf, resp_buf_offset);
    return ret;
}
#endif

static void device_event_handle(struct device_event_t evt, enum device_evt_id_t id,
        uint8 *module_status, uint16 val)
{
    evt.evt_id = id;
    evt.evt_data = module_status;
    evt.evt_data_len = val;
    device.evt_cb(&evt);
}

static void handle_module_status(uint8 *module_status)
{
    struct device_event_t evt;
    uint8 tmp = (*(module_status)) ^ device.module_status;

    device.module_status = *module_status;
    if (tmp & 0x20)
    {
        device_event_handle(evt, DEVICE_EVT_ID_FACTORY_TEST_OVER, module_status + 1, 1);
    }
    if (tmp & 0x02)
    {
        device_event_handle(evt, ((*module_status) & 0x2) ? DEVICE_EVT_ID_ONENET_CONNECTED
                : DEVICE_EVT_ID_ONENET_DISCONNECTED, NULL, 0);
    }
    if (tmp & 0x01)
    {
        device_event_handle(evt, ((*module_status) & 0x1) ? DEVICE_EVT_ID_NET_CONNECTED
                : DEVICE_EVT_ID_NET_DISCONNECTED, module_status + 1, 1);
    }
    if (tmp & 0x04)
    {
        device_event_handle(evt, DEVICE_EVT_ID_SMARTCONFIG_TIMEOUT, NULL, 0);
    }
    if (tmp & 0x40)
    {
        device_event_handle(evt, DEVICE_EVT_ID_MODULE_RESET, NULL, 0);
    }
}

static int32 process_frame(enum protocol_frame_type_t type, uint32 timeout)
{
    int32 ret = ERR_OK;
    enum protocol_frame_type_t frame_type = FRAME_TYPE_NONE;
    enum protocol_resp_code_t resp_code = RESPONSE_CODE_OK;
    uint8 payload[PROT_FRAME_LEN_RECV - PROT_FRAME_LEN_HEAD - PROT_FRAME_LEN_CHECKSUM];
    uint16 payload_len = 0;
    struct device_event_t evt;
    uint32 curr_time = osl_time_ms();

    osl_memset(&evt, 0, sizeof(evt));

    /** 解析收到的协议帧*/
    while (1)
    {
        frame_type = protocol_frame_parse(payload, &payload_len);
        switch (frame_type)
        {
            case FRAME_TYPE_NONE:
            {
                if (check_timeout(curr_time, timeout))
                    return ERR_MOD_TIMEOUT;
                break;
            }
            case FRAME_TYPE_GET_MODULE_INFO:
            {
                uint16 str_len_id = *(uint16 *)payload;
                osl_memcpy(id_buf, (payload + 2), str_len_id);
                device.module_id = id_buf;
                uint16 str_len_ver = *(uint16 *)(payload + 2 + str_len_id);
                osl_memcpy(ver_buf, (payload + 2 + str_len_id + 2), str_len_ver);
                device.module_ver = ver_buf;
                break;
            }
            case FRAME_TYPE_GET_MODULE_STATUS:
            {
                handle_module_status(payload);
                break;
            }
            case FRAME_TYPE_CONFIG_MODULE:
            {
                resp_code = (enum protocol_resp_code_t)(*payload);
                if (RESPONSE_CODE_OK == resp_code)
                {
                    ret = ERR_OK;
                    device.state = DEVICE_STATE_RUNNING;
                }
                else
                    ret = ERR_INITIALIZING;
                break;
            }
            case FRAME_TYPE_FUNCS_UPLOAD:
            case FRAME_TYPE_START_SMARTCONFIG:
            {
                resp_code = (enum protocol_resp_code_t)(*payload);
                if (RESPONSE_CODE_OK == resp_code)
                    ret = ERR_OK;
                else
                    ret = ERR_MOD_TIMEOUT - resp_code;
                break;
            }
            case FRAME_TYPE_GET_NET_TIME:
            {
                device_event_handle(evt, DEVICE_EVT_ID_SYNC_TIME, payload, payload_len);
                break;
            }
            case FRAME_TYPE_RESET_MODULE:
            case FRAME_TYPE_KEEPALIVE:
                break;               
            case FRAME_TYPE_NOTIFY_MODULE_STATUS:
            {
                handle_module_status(payload);
                ret = send_response_frame(frame_type, NULL, 0);
                type = frame_type;
                break;
            }
            case FRAME_TYPE_FUNCS_CMD:
            {
                ret = device_cmd_handle(payload, payload_len);
                type = frame_type;
                break;
            }
#ifdef FUNC_QUERY_ENABLE
            case FRAME_TYPE_FUNCS_QUERY:
            {
                ret = device_query_handle(payload, payload_len);
                type = frame_type;
                break;
            }
#endif
            case FRAME_TYPE_UPGRADE_REQUEST:
            {
                uint8 *resp = NULL;
                uint16 resp_len = 2;

                evt.evt_id = DEVICE_EVT_ID_FOTA_REQUEST;
                evt.evt_data = payload;
                evt.evt_data_len = payload_len;
                resp = protocol_send_buf_malloc(PROT_FRAME_LEN_SEND);
                *((uint16 *)resp) = device.evt_cb(&evt);
                ret = send_response_frame(frame_type, resp, resp_len);
                type = frame_type;
                break;
            }
            case FRAME_TYPE_UPGRADE_DATA:
            {
                uint8 *resp = NULL;
                uint16 resp_len = 1;

                evt.evt_id = DEVICE_EVT_ID_FOTA_DATA;
                evt.evt_data = payload;
                evt.evt_data_len = payload_len;
                resp = protocol_send_buf_malloc(PROT_FRAME_LEN_SEND);
                *((uint8 *)resp) = device.evt_cb(&evt);
                ret = send_response_frame(frame_type, resp, resp_len);
                type = frame_type;
                break;
            }
            default: break;
        }
        if (frame_type == type)
            break;
    }
    return ret;
}

static int32 send_request_frame(enum protocol_frame_type_t type, uint8 *data,
        uint16 data_len)
{
    int32 ret = ERR_OK;

    device.last_frame_time = osl_time_ms();
    ret = protocol_frame_pack_and_send(type, PROT_REQUEST, data, data_len);
    if (ERR_OK != ret)
        return ret;
    return process_frame(type, PROTOCOL_FRAME_TIMEOUT_SEC);
}

int32 device_upload(uint16 func_id , uint8 flag)
{
    int32 ret = ERR_OK;
    uint8 *payload = NULL;
    uint16 payload_len = 0;

    if (DEVICE_IS_OFFLINE())
        return ERR_MOD_OFFLINE; 
    payload = protocol_send_buf_malloc(PROT_FRAME_LEN_SEND);
    payload_len = func_get_val(func_id, payload, PROT_FRAME_LEN_SEND - PROT_FRAME_LEN_HEAD
            - PROT_FRAME_LEN_CHECKSUM, flag ? FUNC_LEVEL_EVENT : FUNC_LEVEL_UPLOAD);
    ret = send_request_frame(FRAME_TYPE_FUNCS_UPLOAD, payload, payload_len);
    return ret;
}

int32 device_get_module_info(int8 *id, int8 *version)
{
    int32 ret = ERR_OK;

    if (!device.module_id && !device.module_ver)
    {
        ret = send_request_frame(FRAME_TYPE_GET_MODULE_INFO, NULL, 0);
        if (ERR_OK != ret)
            return ret;
    }
    strcpy(id, device.module_id);
    strcpy(version, device.module_ver);
    return ret;
}

static int32 device_set_module_param(const int8 *sn)
{
    int32 ret = ERR_OK;
    uint16 payload_len = 0;
    uint16 offset = 0;
    uint16 sn_len = strlen(sn);
    uint16 product_id_len = strlen(PRODUCT_ID);
    uint16 device_ver_len = strlen(DEV_VERSION);
    uint8 *payload = protocol_send_buf_malloc(PROT_FRAME_LEN_SEND);

#ifdef GSM_MODE
    payload_len = 4 + product_id_len + device_ver_len;
    offset += set_lv_str(payload + offset, PRODUCT_ID);
    offset += set_lv_str(payload + offset, DEV_VERSION);
#else
    payload_len = 6 + sn_len + product_id_len + device_ver_len;
    offset += set_lv_str(payload + offset, PRODUCT_ID);
    offset += set_lv_str(payload + offset, sn);
    offset += set_lv_str(payload + offset, DEV_VERSION);
#endif
    ret = send_request_frame(FRAME_TYPE_CONFIG_MODULE, payload, payload_len);
    return ret;
}

int32 device_reset_module(void)
{
    return send_request_frame(FRAME_TYPE_RESET_MODULE, NULL, 0);
}

static int32 device_keep_alive(void)
{
    return send_request_frame(FRAME_TYPE_KEEPALIVE, NULL, 0);
}

int32 device_get_module_status(void)
{
    return send_request_frame(FRAME_TYPE_GET_MODULE_STATUS, NULL, 0);
}

int32 device_sync_time(void)
{
    return send_request_frame(FRAME_TYPE_GET_NET_TIME, NULL, 0);
}

int32 device_init(const int8 *sn, device_event_cb event_cb, data_send_cb send_cb)
{
    int32 ret = ERR_OK;

    osl_memset(&device, 0, sizeof(struct device_t));
    device.evt_cb = event_cb;
    osl_memcpy(sn_buf, sn, strlen(sn));
    device.sn = sn_buf;
    ret = protocol_init(send_cb);
    return ret;
}

int32 device_step(void)
{
    int32 ret = ERR_OK;

    switch (device.state)
    {
        case DEVICE_STATE_CONFIG_MODULE:
        {
            ret = device_set_module_param(device.sn);
            break;
        }
        case DEVICE_STATE_RUNNING:
        case DEVICE_STATE_FACTORY_TEST:
        {
            ret = process_frame(FRAME_TYPE_NONE, PROTOCOL_FRAME_TIMEOUT_SEC);
            if(check_timeout(device.last_frame_time, DEVICE_KEEPALIVE_INTERVAL))
            {
                return device_keep_alive();
            }
            break;
        }
        default: break;
    }
    return ret;
}

int32 device_smartconfig(enum smartconfig_type_t type)
{  
    int32 ret = ERR_OK;
    uint8 *payload = NULL;
    uint8 payload_len = 0;
    
    switch (type)
    {
        case SMARTCONFIG_AUTO:
        case SMARTCONFIG_SOFTAP:
        {
            payload = protocol_send_buf_malloc(PROT_FRAME_LEN_SEND);
            *payload = type;
            payload_len = 1;
            break;
        }
        default: break;
    }
    ret = send_request_frame(FRAME_TYPE_START_SMARTCONFIG, payload, payload_len);
    return ret;
}

int32 device_factory_test(void)
{
    device.state = DEVICE_STATE_FACTORY_TEST;
    return send_request_frame(FRAME_TYPE_TEST, NULL, 0);
}
