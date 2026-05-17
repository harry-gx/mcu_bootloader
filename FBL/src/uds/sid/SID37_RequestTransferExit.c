/******************************************************************************
* 文件名称: SID36_TransferData.c
* 内容摘要: 数据传输
* 创建者の: 孔佳伟
* 个人主页: https://gitee.com/openes
* 修改记录:
******************************************************************************/

#include "uds_cfg.h"
#include "SID37_RequestTransferExit.h"
#include "uds.h"
#include "uds_port.h"



/******************************************************************************
* 函数名称: bool_bl service_37_check_len(const uint8_bl* msg_buf, uint16_bl msg_dlc)
* 功能说明: 检查 37 服务数据长度是否合法
* 输入参数: uint16_bl msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: TRUE: 合法; FALSE: 非法
* 其它说明: 无
******************************************************************************/
bool_bl service_37_check_len(const uint8_bl* msg_buf, uint16_bl msg_dlc)
{
	bool_bl ret = FALSE;

	ret = TRUE;

	return ret;
}


/******************************************************************************
* 函数名称: void service_37_RequestTransferExit(const uint8_bl* msg_buf, uint16_bl msg_dlc)
* 功能说明: 37 服务 - 请求退出传输
* 输入参数: uint8_bl*    msg_buf         --数据首地址
    　　　　uint8_bl     msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void service_37_RequestTransferExit(const uint8_bl* msg_buf, uint16_bl msg_dlc)
{
	uint8_bl rsp_buf[8];

	if (GetUdsProgramResult() == UDS_PROGRAM_RESULT_FAIL)
	{
		uds_negative_rsp(SID_37, NRC_GENERAL_PROGRAMMING_FAILURE);
		return;
	}

	rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_37);
	uds_positive_rsp(rsp_buf, 1);
}


/****************EOF****************/
