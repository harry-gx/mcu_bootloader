/******************************************************************************
* 文件名称: SID36_TransferData.c
* 内容摘要: 数据传输
* 创建者の: 孔佳伟
* 个人主页: https://gitee.com/openes
* 修改记录: 
******************************************************************************/

#include "uds_cfg.h"
#include "SID36_TransferData.h"
#include "uds.h"
#include "uds_port.h"

/******************************************************************************
* 函数名称: bool_t service_36_check_len(const uint8_t* msg_buf, uint16_t msg_dlc)
* 功能说明: 检查 36 服务数据长度是否合法
* 输入参数: uint16_t msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: TRUE: 合法; FALSE: 非法
* 其它说明: 无
******************************************************************************/
bool_bl service_36_check_len(const uint8_bl* msg_buf, uint16_bl msg_dlc)
{
    (void)msg_buf;
    if ((msg_dlc > 2u) && (msg_dlc <= 514u))
    {
        return TRUE;
    }

    return FALSE;
}

/******************************************************************************
* 函数名称: void service_36_TransferData(const uint8_t* msg_buf, uint16_t msg_dlc)
* 功能说明: 36 服务 - 数据传输
* 输入参数: uint8_t*    msg_buf         --数据首地址
    　　　　uint8_t     msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void service_36_TransferData(const uint8_bl* msg_buf, uint16_bl msg_dlc)
{
    uint8_bl rsp_buf[8];
    uint8_bl bn;
    uint8_bl wr_ok;
    uint16_bl data_len;
    uint16_bl write_len;
    uint16_bl i;
    const uint8_bl *data_ptr;
    const uint8_bl *write_ptr;
    uint32_bl flash_addr;
    uint32_bl app_start_addr;
    static uint8_bl aligned_buf[528];
    static uint32_bl stream_offset = 0u;

    bn = msg_buf[1];
    app_start_addr = UdsPort_GetAppStartAddr();
    if (bn == 1u)
    {
        stream_offset = 0u;
    }

    if (msg_dlc <= 2u)
    {
        SetUdsProgramResult(UDS_PROGRAM_RESULT_FAIL);
        uds_negative_rsp(SID_36, NRC_INVALID_MESSAGE_LENGTH_OR_FORMAT);
        return;
    }

    data_ptr = &msg_buf[2];
    data_len = (uint16_bl)(msg_dlc - 2u);

    if ((bn == 1u) && (data_len >= UDS_HEADER_PREFIX_SIZE))
    {
        StoreUdsHeaderPrefix(data_ptr, data_len);
    }

    /* Keep first 16 header bytes erased, patch them in 31 FF01 later. */
    if (stream_offset == 0u)
    {
        if (data_len <= UDS_HEADER_PREFIX_SIZE)
        {
            stream_offset += data_len;
            rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_36);
            rsp_buf[1] = bn;
            uds_positive_rsp(rsp_buf, 2);
            return;
        }

        data_ptr += UDS_HEADER_PREFIX_SIZE;
        data_len = (uint16_bl)(data_len - UDS_HEADER_PREFIX_SIZE);
        flash_addr = app_start_addr + UDS_HEADER_PREFIX_SIZE;
    }
    else
    {
        flash_addr = app_start_addr + stream_offset;
    }

    write_len = data_len;
    write_ptr = data_ptr;

    if ((data_len < 512u) && ((data_len % 16u) != 0u))
    {
        write_len = (uint16_bl)((data_len + 15u) & (~15u));
        for (i = 0u; i < data_len; i++)
        {
            aligned_buf[i] = data_ptr[i];
        }
        for (; i < write_len; i++)
        {
            aligned_buf[i] = 0xFFu;
        }
        write_ptr = aligned_buf;
    }

    wr_ok = UdsPort_WriteAppFlash(flash_addr, write_len, write_ptr);
    if (wr_ok != 0u)
    {
        SetUdsProgramResult(UDS_PROGRAM_RESULT_FAIL);
        uds_negative_rsp(SID_36, NRC_GENERAL_PROGRAMMING_FAILURE);
        return;
    }

    stream_offset += (uint32_bl)(msg_dlc - 2u);

    rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_36);
    rsp_buf[1] = bn;
    uds_positive_rsp(rsp_buf, 2);
}
