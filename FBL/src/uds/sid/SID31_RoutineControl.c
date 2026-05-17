/******************************************************************************
* 文件名称: SID31_RoutineControl.c
* 内容摘要: 例程控制
* 创建者の: 孔佳伟
* 个人主页: https://gitee.com/openes
* 修改记录: 
******************************************************************************/

#include "uds_cfg.h"
#include "SID31_RoutineControl.h"
#include "uds.h"
#include "uds_port.h"

/*
 * 31 01 ff 00 擦除内存
 * 31 01 02 02 检查传输数据完整性
 * 31 01 02 03 更新条件查询
 * 31 01 ff 01 软硬件一致性检查
 * 31 01 df e0 从节点模式选择
 * 31 01 02 04 数字签名
*/


typedef enum __UDS_ROUTINE_CTRL_TYPE__
{
	UDS_ROUTINE_CTRL_NONE = 0,
	UDS_ROUTINE_CTRL_START = 0x01,
	UDS_ROUTINE_CTRL_STOP = 0x02,
	UDS_ROUTINE_CTRL_REQUEST_RESULT = 0x03
}uds_routine_ctrl_type;


/******************************************************************************
* 函数名称: bool_t service_31_check_len(const uint8_t* msg_buf, uint16_t msg_dlc)
* 功能说明: 检查 31 服务数据长度是否合法
* 输入参数: uint16_t msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: TRUE: 合法; FALSE: 非法
* 其它说明: 无
******************************************************************************/
bool_bl service_31_check_len(const uint8_bl* msg_buf, uint16_bl msg_dlc)
{
	bool_bl ret = FALSE;

	(void)msg_buf;
	if (msg_dlc >= 4u)
	{
		ret = TRUE;
	}

	return ret;
}


/******************************************************************************
* 函数名称: void service_31_RoutineControl(const uint8_t* msg_buf, uint16_t msg_dlc)
* 功能说明: 31 服务 - 例程控制
* 输入参数: uint8_t*    msg_buf         --数据首地址
    　　　　uint8_t     msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void service_31_RoutineControl(const uint8_bl* msg_buf, uint16_bl msg_dlc)
{
    uint8_bl subfunction;
	uint8_bl rsp_buf[8];
	uint16_bl rid;

	if (msg_dlc < 4u)
	{
		uds_negative_rsp(SID_31, NRC_INVALID_MESSAGE_LENGTH_OR_FORMAT);
		return;
	}

    subfunction = UDS_GET_SUB_FUNCTION (msg_buf[1]);
	rid = ((uint16_bl)msg_buf[2]) << 8;
	rid |= msg_buf[3];

	rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_31);
	rsp_buf[1] = msg_buf[1];
	rsp_buf[2] = msg_buf[2];
	rsp_buf[3] = msg_buf[3];

    switch (subfunction)
	{
		case UDS_ROUTINE_CTRL_START: // 01
			if(0xFF00 == rid) // 擦除 Flash
			{
				if (msg_dlc < 12u)
				{
					uds_negative_rsp(SID_31, NRC_INVALID_MESSAGE_LENGTH_OR_FORMAT);
					return;
				}

				uint8_bl eraseOk = UdsPort_EraseAppImage();
				if (0u != eraseOk)
				{
					SetUdsProgramResult(UDS_PROGRAM_RESULT_FAIL);
					uds_negative_rsp(SID_31, NRC_GENERAL_PROGRAMMING_FAILURE);
					return;
				}
			}
			if(0xFF01 == rid) // CRC 校验
			{
				uint8_bl len_buf[4];
				uint8_bl crc_buf[4];
				uint8_bl header_prefix[UDS_HEADER_PREFIX_SIZE];
				uint8_bl phrase0_buf[8];
				uint8_bl phrase1_buf[8];
				uint8_bl i;
				uint8_bl wr_len_ok;
				uint8_bl wr_crc_ok;
				uint32_bl app_start_addr;

				if (msg_dlc < 12u)
				{
					uds_negative_rsp(SID_31, NRC_INVALID_MESSAGE_LENGTH_OR_FORMAT);
					return;
				}

				len_buf[0] = msg_buf[4];
				len_buf[1] = msg_buf[5];
				len_buf[2] = msg_buf[6];
				len_buf[3] = msg_buf[7];

				crc_buf[0] = msg_buf[8];
				crc_buf[1] = msg_buf[9];
				crc_buf[2] = msg_buf[10];
				crc_buf[3] = msg_buf[11];

				if (LoadUdsHeaderPrefix(header_prefix, UDS_HEADER_PREFIX_SIZE) == 0u)
				{
					SetUdsProgramResult(UDS_PROGRAM_RESULT_FAIL);
					uds_negative_rsp(SID_31, NRC_GENERAL_PROGRAMMING_FAILURE);
					return;
				}

				app_start_addr = UdsPort_GetAppStartAddr();

				for (i = 0u; i < 8u; i++)
				{
					phrase0_buf[i] = header_prefix[i];
					phrase1_buf[i] = header_prefix[8u + i];
				}

				phrase0_buf[4] = len_buf[3];
				phrase0_buf[5] = len_buf[2];
				phrase0_buf[6] = len_buf[1];
				phrase0_buf[7] = len_buf[0];

				phrase1_buf[0] = crc_buf[3];
				phrase1_buf[1] = crc_buf[2];
				phrase1_buf[2] = crc_buf[1];
				phrase1_buf[3] = crc_buf[0];

				if (UdsPort_CheckFlashProgramCompatible(app_start_addr, phrase0_buf, 8u) == 0u)
				{
					SetUdsProgramResult(UDS_PROGRAM_RESULT_FAIL);
					uds_negative_rsp(SID_31, NRC_GENERAL_PROGRAMMING_FAILURE);
					return;
				}

				if (UdsPort_CheckFlashProgramCompatible(app_start_addr + 8u, phrase1_buf, 8u) == 0u)
				{
					SetUdsProgramResult(UDS_PROGRAM_RESULT_FAIL);
					uds_negative_rsp(SID_31, NRC_GENERAL_PROGRAMMING_FAILURE);
					return;
				}

				wr_len_ok = UdsPort_WriteAppFlash(app_start_addr, 8u, phrase0_buf);
				wr_crc_ok = UdsPort_WriteAppFlash(app_start_addr + 8u, 8u, phrase1_buf);
				if ((wr_len_ok != 0u) || (wr_crc_ok != 0u))
				{
					SetUdsProgramResult(UDS_PROGRAM_RESULT_FAIL);
					uds_negative_rsp(SID_31, NRC_GENERAL_PROGRAMMING_FAILURE);
					return;
				}

				SetUdsProgramResult(UDS_PROGRAM_RESULT_DONE);

			}

			uds_positive_rsp (rsp_buf, 4);
		    break;
		case UDS_ROUTINE_CTRL_STOP: // 02
		    uds_positive_rsp (rsp_buf,4);
		    break;
		case UDS_ROUTINE_CTRL_REQUEST_RESULT: // 03
		    uds_positive_rsp (rsp_buf,4);
		    break;
		default:
		    uds_negative_rsp (SID_31, NRC_SUBFUNCTION_NOT_SUPPORTED);
		    break;
	}
}


/****************EOF****************/
