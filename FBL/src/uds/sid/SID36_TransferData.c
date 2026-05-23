/******************************************************************************
* 文件名称: SID36_TransferData.c
* 内容摘要: 数据传输
* 创建者の: 孔佳伟
* 个人主页: https://gitee.com/openes
* 修改记录:
******************************************************************************/

#include <string.h>
#include "uds_cfg.h"
#include "fbl_cfg.h"
#include "SID36_TransferData.h"
#include "uds.h"
#include "uds_port.h"

uint8_bl diag_tansf_data_block_cnt = 1;
uint32_bl boot_wstart_address = 0u;
uint32_bl write_address_offset = 0u;
uint32_bl write_length_once = 0u;

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
void service_36_TransferData(const uint8_bl *msg_buf, uint16_bl msg_dlc)
{
	uint8_bl rsp_buff[8];

	boot_wstart_address = UdsPort_GetAppStartAddr();
	if (diag_tansf_data_block_cnt == msg_buf[1])
	{
		write_length_once = msg_dlc - 2u;

		if (BOOT_WRITE_BLOCK_SIZE < write_length_once)
		{
	        SetUdsProgramResult(UDS_PROGRAM_RESULT_FAIL);
	        uds_negative_rsp(SID_36, NRC_GENERAL_PROGRAMMING_FAILURE);
			return;
		}

		if (BOOT_WRITE_BLOCK_SIZE > write_length_once)
		{
			memset((uint8_bl *)&msg_buf[msg_dlc], 0xFF,
				  (BOOT_WRITE_BLOCK_SIZE - write_length_once));
			write_length_once = BOOT_WRITE_BLOCK_SIZE;
		}

		if (BOOT_WRITE_BLOCK_SIZE == write_length_once)
		{
			int32_bl ret = UdsPort_WriteAppFlash(boot_wstart_address + write_address_offset,
								                 write_length_once,
								                 &msg_buf[2]);
			if (FBL_OK != ret)
			{
		        SetUdsProgramResult(UDS_PROGRAM_RESULT_FAIL);
		        uds_negative_rsp(SID_36, NRC_GENERAL_PROGRAMMING_FAILURE);
				return;
			}

			diag_tansf_data_block_cnt++;
			write_address_offset += write_length_once;

			rsp_buff[0] = USD_GET_POSITIVE_RSP(SID_36);
			rsp_buff[1] = diag_tansf_data_block_cnt - 1;
		    uds_positive_rsp(rsp_buff, 2);
		}
	}
	else
	{
        SetUdsProgramResult(UDS_PROGRAM_RESULT_FAIL);
        uds_negative_rsp(SID_36, NRC_GENERAL_PROGRAMMING_FAILURE);
	}
}
