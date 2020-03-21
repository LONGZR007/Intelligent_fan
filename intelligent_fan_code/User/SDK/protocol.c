/**
 * Copyright (c), 2012~2019 iot.10086.cn All Rights Reserved
 * @file    protocol.c
 * @date    2019/04/22
 * @brief   ����Э��������ӿ��ļ�
 */

/*****************************************************************************/
/* Includes                                                                  */
/*****************************************************************************/
#include "err.h"
#include "osl.h"
#include "protocol.h"

/*****************************************************************************/
/* Local Definitions ( Constant and Macro )                                  */
/*****************************************************************************/

/*****************************************************************************/
/* Structures, Enum and Typedefs                                             */
/*****************************************************************************/
#pragma pack(1)
struct prot_frame_header_t
{
    uint16 sync;
    uint16 len;
    uint8 type: 5;
    uint8 ver: 2;
    uint8 response: 1;
};
#pragma pack()

struct prot_frame_parser_t
{
    uint8 *recv_ptr;
    uint8 *send_ptr;
    uint16 r_oft;
    uint16 w_oft;
    uint16 frame_len;
    uint16 found_frame_head;
    protocol_frame_send_cb send_cb;
};

/*****************************************************************************/
/* Local Function Prototype                                                  */
/*****************************************************************************/

/*****************************************************************************/
/* Local Variables                                                           */
/*****************************************************************************/
static struct prot_frame_parser_t parser;

static uint8 recv_buf[PROT_FRAME_LEN_RECV];
static uint8 send_buf[PROT_FRAME_LEN_SEND];

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* External Functions and Variables                                          */
/*****************************************************************************/

/*****************************************************************************/
/* Function Implementation                                                   */
/*****************************************************************************/
static uint8 cal_checksum(uint8 *data, uint32 data_len)
{
    uint32 checksum = 0;
    uint32 i = 0;

    for (i = 0; i < data_len; i++)
    {
        checksum += data[i];
    }
    return (uint8)(checksum & 0xFF);
}

static uint8 get_frame_type(uint8 *frame, uint16 head_oft)
{
    return (frame[(head_oft + 4) % PROT_FRAME_LEN_RECV] & 0x1F);
}

static uint16 get_frame_len(uint8 *frame, uint16 head_oft)
{
    return (frame[(head_oft + 3) % PROT_FRAME_LEN_RECV] << 8) |
            (frame[(head_oft + 2) % PROT_FRAME_LEN_RECV]);
}

static uint8 get_frame_checksum(uint8 *frame, uint16 head_oft, uint16 frame_len)
{
    return (frame[(head_oft + frame_len - 1) % PROT_FRAME_LEN_RECV]);
}

static int32 recvbuf_find_header(uint8 *buf, uint16 ring_buf_len, uint16 start, uint16 len)
{
    uint16 i = 0;

    for (i = 0; i < (len - 1); i++)
    {
        if (((PROT_FRAME_SYNC & 0xFF) == buf[(start + i) % ring_buf_len]) && 
                (((PROT_FRAME_SYNC >> 8) & 0xFF) == buf[(start + i + 1) % ring_buf_len])) 
        {
            return ((start + i) % ring_buf_len);
        }
    }
    return -1;
}

static int32 recvbuf_get_len_to_parse(uint16 frame_len, uint16 ring_buf_len,
        uint16 start, uint16 end)
{
    uint16 unparsed_data_len = 0;

    if (start <= end)
        unparsed_data_len = end - start;
    else
        unparsed_data_len = ring_buf_len - start + end;

    if ((frame_len > unparsed_data_len) || (unparsed_data_len < 
            (sizeof(struct prot_frame_header_t) + PROT_FRAME_LEN_CHECKSUM)))
        return 0;
    else
        return unparsed_data_len;
}

static void recvbuf_put_data(uint8 *buf, uint16 ring_buf_len, uint16 w_oft,
        uint8 *data, uint16 data_len)
{
    if ((w_oft + data_len) > ring_buf_len) 
    {
        uint16 data_len_part = ring_buf_len - w_oft;

        /* ���ݷ�����д�뻺����*/
        osl_memcpy(buf + w_oft, data, data_len_part);
        osl_memcpy(buf, data + data_len_part, data_len - data_len_part);
    }
    else
        osl_memcpy(buf + w_oft, data, data_len);
}

void protocol_data_recv(uint8 *data, uint16 data_len)
{
    recvbuf_put_data(parser.recv_ptr, PROT_FRAME_LEN_RECV, parser.w_oft, data, data_len);
    parser.w_oft = (parser.w_oft + data_len) % PROT_FRAME_LEN_RECV;
}

int32 protocol_frame_pack_and_send(enum protocol_frame_type_t type, uint8 is_resp,
        uint8 *data, uint16 data_len)
{
    uint8 *send_frame = NULL;
    uint8 *checksum = NULL;
    struct prot_frame_header_t *header;

    if (!data_len)
    {
        data = protocol_send_buf_malloc(PROT_FRAME_LEN_SEND);
    }
    send_frame = data - sizeof(struct prot_frame_header_t);  //send_frameָ��ָ��֡ͷ
    /** Э����*/
    header = (struct prot_frame_header_t *)send_frame;
    header->sync = PROT_FRAME_SYNC;
    header->len = sizeof(struct prot_frame_header_t) + data_len + PROT_FRAME_LEN_CHECKSUM;
    header->response = is_resp;
    header->ver = 0x01;
    header->type = type;
    checksum = (uint8 *)(send_frame + sizeof(struct prot_frame_header_t) + data_len);
    *checksum = cal_checksum((uint8 *)send_frame, header->len - PROT_FRAME_LEN_CHECKSUM);

    parser.send_cb(send_frame, header->len);  //���ô��ڷ��ͻص���������������  
    return ERR_OK;
}

enum protocol_frame_type_t protocol_frame_parse(uint8 *data, uint16 *data_len)
{
    enum protocol_frame_type_t frame_type = FRAME_TYPE_NONE;
    uint16 need_to_parse_len = 0;
    uint16 data_start_oft = 0;
    int16 header_oft = -1;
    uint8 checksum = 0;
    uint16 payload_len = 0;
    
    need_to_parse_len = recvbuf_get_len_to_parse(parser.frame_len, PROT_FRAME_LEN_RECV,
            parser.r_oft, parser.w_oft);
    if (!need_to_parse_len)
        return frame_type;

    /* ��δ�ҵ�֡ͷ����Ҫ���в���*/
    if (0 == parser.found_frame_head)
    {
        /* ͬ��ͷΪ���ֽڣ����ܴ���δ���������������һ���ֽڸպ�Ϊͬ��ͷ��һ���ֽڵ������
           ��˲���ͬ��ͷʱ�����һ���ֽڽ���������Ҳ���ᱻ����*/
        header_oft = recvbuf_find_header(parser.recv_ptr, PROT_FRAME_LEN_RECV,
                parser.r_oft, need_to_parse_len);
        if (0 <= header_oft)
        {
            /* ���ҵ�֡ͷ*/
            parser.found_frame_head = 1;
            parser.r_oft = header_oft;
            
            /* ȷ���Ƿ���Լ���֡��*/
            if (!recvbuf_get_len_to_parse(parser.frame_len, PROT_FRAME_LEN_RECV,
                    parser.r_oft, parser.w_oft))
                return frame_type;
        }
        else 
        {
            /* δ��������������Ȼδ�ҵ�֡ͷ�������˴ν���������������*/
            parser.r_oft = ((parser.r_oft + need_to_parse_len - 1) % PROT_FRAME_LEN_RECV);
            return frame_type;
        }
    }
    
    /* ����֡������ȷ���Ƿ���Խ������ݽ���*/
    if (0 == parser.frame_len) 
    {
        parser.frame_len = get_frame_len(parser.recv_ptr, parser.r_oft);
        if(need_to_parse_len < parser.frame_len)
            return frame_type;
    }

    /* ֡ͷλ��ȷ�ϣ���δ���������ݳ���֡�������Լ���У���*/
    if ((parser.frame_len + parser.r_oft) > PROT_FRAME_LEN_RECV)
    {
        /* ����֡����Ϊ�����֣�һ�����ڻ�����β��һ�����ڻ�����ͷ*/
        checksum = cal_checksum(parser.recv_ptr + parser.r_oft, 
                PROT_FRAME_LEN_RECV - parser.r_oft);
        checksum += cal_checksum(parser.recv_ptr, parser.frame_len -
                PROT_FRAME_LEN_CHECKSUM + parser.r_oft - PROT_FRAME_LEN_RECV);
    }
    else 
    {
        /* ����֡����һ����ȡ��*/
        checksum = cal_checksum(parser.recv_ptr + parser.r_oft, parser.frame_len - 
                PROT_FRAME_LEN_CHECKSUM);
    }

    if (checksum == get_frame_checksum(parser.recv_ptr, parser.r_oft, parser.frame_len))
    {
        /* У��ɹ�*/
        payload_len = parser.frame_len - sizeof(struct prot_frame_header_t) - PROT_FRAME_LEN_CHECKSUM;
        data_start_oft = (parser.r_oft + sizeof(struct prot_frame_header_t)) % PROT_FRAME_LEN_RECV;

        if ((data_start_oft + payload_len) > PROT_FRAME_LEN_RECV) 
        {
            /* ����֡����Ϊ�����֣�һ�����ڻ�����β��һ�����ڻ�����ͷ*/
            uint16 data_len_part = PROT_FRAME_LEN_RECV - data_start_oft;
            osl_memcpy(data, parser.recv_ptr + data_start_oft, data_len_part);
            osl_memcpy(data + data_len_part, parser.recv_ptr, payload_len - data_len_part);
        }
        else 
        {
            /* ����֡����һ����ȡ��*/
            osl_memcpy(data, parser.recv_ptr + data_start_oft, payload_len);
        }
        *data_len = payload_len;
        frame_type = (enum protocol_frame_type_t)get_frame_type(parser.recv_ptr, parser.r_oft);

        /* �����������е�����֡*/
        parser.r_oft = (parser.r_oft + parser.frame_len) % PROT_FRAME_LEN_RECV;
    }
    else
    {
        /* У�����˵��֮ǰ�ҵ���֡ͷֻ��żȻ���ֵķ�����*/
        parser.r_oft = (parser.r_oft + 1) % PROT_FRAME_LEN_RECV;
    }
    parser.frame_len = 0;
    parser.found_frame_head = 0;

    return frame_type;
}

int32 protocol_init(protocol_frame_send_cb callback)
{
    osl_memset(&parser, 0, sizeof(struct prot_frame_parser_t));
    
    /* ��ʼ���������ݽ��������������*/
    parser.recv_ptr = recv_buf;
    
    /* ��ʼ���������ݷ��ͻ�����*/
    parser.send_ptr = send_buf;
    
    parser.send_cb = callback;  //���뷢�ͻص������ӿ�
    return ERR_OK;
}

uint8 *protocol_send_buf_malloc(uint16 data_len)
{
    uint8 *send = NULL;
    
    if (data_len > PROT_FRAME_LEN_SEND)
        return NULL;
    send = parser.send_ptr;
    return (send + sizeof(struct prot_frame_header_t));  //�������ݶ��׵�ַ
}

/** TLV���ݷ��*/
uint16 tlv_pack(uint8 *buf, enum tlv_type_t type, uint8 *val, uint16 val_len)
{
    buf[0] = type;
    osl_memcpy(buf + 1, &val_len, 2);
    osl_memcpy(buf + 3, val, val_len);
    
    return (3 + val_len);
}

uint16 tlv_unpack(uint8 *buf, enum tlv_type_t *type, uint8 *val, uint16 *val_len)
{
    *type = (enum tlv_type_t)buf[0];
    osl_memcpy(val_len, buf + 1, 2); 
    osl_memcpy(val, buf + 3, *val_len);
    return (3 + *val_len);
}
