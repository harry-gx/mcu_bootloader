/******************************************************************************
* 文件名称: SID14_ClearDiagnosticInformation.c
* 内容摘要: 清除诊断信息
* 创建者の: 孔佳伟
* 个人主页: https://gitee.com/openes
* 修改记录:
******************************************************************************/


#include "uds_cfg.h"
#include "SID14_ClearDiagnosticInformation.h"
#include "uds.h"

#define UDS_DTC_GROUP_ALL	 0xFFFFFF

/******************************************************************************
* 函数名称: bool_bl service_14_check_len(const uint8_bl* msg_buf, uint16_bl msg_dlc)
* 功能说明: 检查 14 服务数据长度是否合法
* 输入参数: uint16_bl msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: TRUE: 合法; FALSE: 非法
* 其它说明: 无
******************************************************************************/
bool_bl service_14_check_len(const uint8_bl* msg_buf, uint16_bl msg_dlc)
{
	bool_bl ret = FALSE;

	(void)msg_buf;
	if(4 == msg_dlc)
		ret = TRUE;

	return ret;
}


/******************************************************************************
* 函数名称: void service_14_ClearDiagnosticInformation(const uint8_bl* msg_buf, uint16_bl msg_dlc)
* 功能说明: 14 服务 - 清除诊断信息
* 输入参数: uint8_bl*    msg_buf         --数据首地址
    　　　　uint8_bl     msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void service_14_ClearDiagnosticInformation(const uint8_bl* msg_buf, uint16_bl msg_dlc)
{
	uint8_bl rsp_buf[8];
	uint32_bl dtc_group = 0;

	dtc_group = 0;
	dtc_group |= ((uint32_bl)msg_buf[1]) << 16;
	dtc_group |= ((uint32_bl)msg_buf[2]) << 8;
	dtc_group |= ((uint32_bl)msg_buf[3]) << 0;

	if (dtc_group == UDS_DTC_GROUP_ALL)
	{
		// clear_dtc_by_group (dtc_group);
		rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_14);
		uds_positive_rsp(rsp_buf, 1);
	}
	else
	{
		uds_negative_rsp(SID_14, NRC_REQUEST_OUT_OF_RANGE);
	}
}


/****************EOF****************/
