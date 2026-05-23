/******************************************************************************
* 文件名称: SID34_RequestDownload.c
* 内容摘要: 请求下载
* 创建者の: 孔佳伟
* 个人主页: https://gitee.com/openes
* 修改记录:
******************************************************************************/

#include "uds_cfg.h"
#include "fbl_cfg.h"
#include "SID34_RequestDownload.h"
#include "uds.h"
#include "uds_port.h"

// 36 服务数据传输报文总大小
#define TOTAL_LEN_36 (BOOT_WRITE_BLOCK_SIZE + 2)

extern uint8_bl diag_tansf_data_block_cnt;
extern uint32_bl boot_wstart_address;
extern uint32_bl write_address_offset;
extern uint32_bl write_length_once;

/******************************************************************************
* 函数名称: bool_bl service_34_check_len(const uint8_bl* msg_buf, uint16_bl msg_dlc)
* 功能说明: 检查 34 服务数据长度是否合法
* 输入参数: uint16_bl msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: TRUE: 合法; FALSE: 非法
* 其它说明: 无
******************************************************************************/
bool_bl service_34_check_len(const uint8_bl* msg_buf, uint16_bl msg_dlc)
{
	bool_bl ret = FALSE;

	ret = TRUE;

	return ret;
}


/******************************************************************************
* 函数名称: void service_34_RequestDownload(const uint8_bl* msg_buf, uint16_bl msg_dlc)
* 功能说明: 34 服务 - 请求下载
* 输入参数: uint8_bl*    msg_buf         --数据首地址
    　　　　uint8_bl     msg_dlc         --数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void service_34_RequestDownload(const uint8_bl* msg_buf, uint16_bl msg_dlc)
{
	uint8_bl rsp_buf[8];

	SetUpdateRequestFlag(1);
	SetUdsProgramResult(UDS_PROGRAM_RESULT_IDLE);

	diag_tansf_data_block_cnt = 1;
	boot_wstart_address = 0u;
	write_address_offset = 0u;
	write_length_once = 0u;

	// 这里需要解析 34 服务报文

	rsp_buf[0] = USD_GET_POSITIVE_RSP(SID_34);
	rsp_buf[1] = 0x20;
	rsp_buf[2] = (uint8_bl)(TOTAL_LEN_36 >> 8);
	rsp_buf[3] = (uint8_bl)(TOTAL_LEN_36 >> 0);
	uds_positive_rsp(rsp_buf, 4);
}


/****************EOF****************/
