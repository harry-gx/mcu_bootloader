/******************************************************************************
* 文件名称: SID19_ReadDTCInformation.c
* 内容摘要: 读取故障码信息
* 创建者の: 孔佳伟
* 个人主页: https://gitee.com/openes
* 修改记录:
******************************************************************************/

#include "uds_cfg.h"
#include "SID19_ReadDTCInformation.h"
#include "uds.h"


#define REPORT_DTC_NUMBER_BY_STATUS_MASK  (0x01)
#define REPORT_DTC_BY_STATUS_MASK         (0x02)
#define REPORT_DTC_SNOPSHOT_BY_DTC_NUMBER (0x04)
#define REPORT_DTC_EXTENDED_DATA_RECORD_BY_DTC_NUMBER (0x06)
#define REPORT_SUPPORTED_DTC              (0x0a)


/******************************************************************************
* 函数名称: bool_bl service_19_check_len(const uint8_bl* msg_buf, uint16_bl msg_dlc)
* 功能说明: 检查 19 服务数据长度是否合法
* 输入参数: uint16_bl msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: TRUE: 合法; FALSE: 非法
* 其它说明: 无
******************************************************************************/
bool_bl service_19_check_len(const uint8_bl* msg_buf, uint16_bl msg_dlc)
{
	bool_bl ret = FALSE;


	return ret;
}


/******************************************************************************
* 函数名称: void service_19_ReadDTCInformation(const uint8_bl* msg_buf, uint16_bl msg_dlc)
* 功能说明: 19 服务 - 读取故障码信息
* 输入参数: uint8_bl*    msg_buf         --数据首地址
    　　　　uint8_bl     msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void service_19_ReadDTCInformation(const uint8_bl* msg_buf, uint16_bl msg_dlc)
{
	uint8_bl subfunction;
    uint8_bl rsp_buf[64];

	subfunction = UDS_GET_SUB_FUNCTION (msg_buf[1]);

    switch (subfunction)
	{
		case REPORT_DTC_NUMBER_BY_STATUS_MASK:
		{
			uint16_bl dtc_count;
			dtc_count = 5; // example
			rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_19);
			rsp_buf[1] = subfunction;
			rsp_buf[2] = 0x7F;
			rsp_buf[3] = 1; // 14229
			rsp_buf[4] = (uint8_bl)dtc_count;
			rsp_buf[5] = (dtc_count >> 8) & 0xFF;
			uds_positive_rsp (rsp_buf, 6);
		    break;
		}

		default:
	        uds_negative_rsp(SID_19, NRC_SUBFUNCTION_NOT_SUPPORTED);
		    break;
	}
}


/****************EOF****************/
