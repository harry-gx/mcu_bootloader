/******************************************************************************
* 文件名称: SID31_RoutineControl.c
* 内容摘要: 例程控制
* 创建者の: 孔佳伟
* 个人主页: https://gitee.com/openes
* 修改记录:
******************************************************************************/

#include <string.h>
#include "SID31_RoutineControl.h"
#include "uds_cfg.h"
#include "fbl_cfg.h"
#include "uds.h"
#include "uds_port.h"
#include "crc32.h"

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
		SetUdsProgramResult(UDS_PROGRAM_RESULT_FAIL);
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
				uint8_bl eraseOk = UdsPort_EraseAppImage();
				if (0u != eraseOk)
				{
					SetUdsProgramResult(UDS_PROGRAM_RESULT_FAIL);
					uds_negative_rsp(SID_31, NRC_GENERAL_PROGRAMMING_FAILURE);
					return;
				}
				else
				{
					SetUdsProgramResult(UDS_PROGRAM_RESULT_DONE);
					uds_positive_rsp(rsp_buf, 4);
				}
			}

			if(0xFF01 == rid) // CRC 校验
			{
				uint32_bl firLen = (msg_buf[4] << 24) |
								   (msg_buf[5] << 16) |
								   (msg_buf[6] << 8) |
								   (msg_buf[7]);

				uint32_bl firCrc = (msg_buf[8] << 24) |
								   (msg_buf[9] << 16) |
								   (msg_buf[10] << 8) |
								   (msg_buf[11]);

			    uint32_bl crc = CRC_CalcImageCrc(0xFFFFFFFFu,
											    (const uint8_bl *)BOOT_APP_START_ADDR,
												firLen);
			    if (firCrc == crc)
			    {
			    	/* 固件更新完成后校验通过，才保存固件长度和校验码 */
			    	uint8_bl writeHead[16] = {0};
			    	memcpy(writeHead, (uint8_bl *)BOOT_APP_START_ADDR, 4);
			    	memcpy(&writeHead[4], &firLen, 4);
			    	memcpy(&writeHead[8], &firCrc, 4);
			    	int32_bl ret = UdsPort_WriteAppFlash(BOOT_APP_START_ADDR, 16u, (const uint8_bl *)writeHead);
					if (FBL_OK != ret)
					{
				        SetUdsProgramResult(UDS_PROGRAM_RESULT_FAIL);
				        uds_negative_rsp(SID_31, NRC_GENERAL_PROGRAMMING_FAILURE);
						return;
					}

					SetUdsProgramResult(UDS_PROGRAM_RESULT_DONE);
					uds_positive_rsp(rsp_buf, 4);
			    }
			    else
			    {
			    	SetUdsProgramResult(UDS_PROGRAM_RESULT_FAIL);
			    	uds_negative_rsp(SID_31, NRC_GENERAL_PROGRAMMING_FAILURE);
			    }
			}
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
