/******************************************************************************
* 文件名称: uds_port.c
* 内容摘要: UDS 协议栈移植接口
* 创建者の: 孔佳伟
* 个人主页: https://gitee.com/openes
* 修改记录:
******************************************************************************/

#include <string.h>
#include "uds_port.h"
#include "SID10_SessionControl.h"
#include "fbl_cfg.h"
#include "uds.h"
#include "can_tp.h"
#include "can_if.h"
#include "fls_if.h"

static volatile uint8_bl updateRequestFlag = 0u;
static volatile uint8_bl udsProgramResultFlag = UDS_PROGRAM_RESULT_IDLE;
static volatile uint8_bl udsHeaderPrefixValid = 0u;
static volatile uint32_bl mainLoopCnt = 0u;
static uint8_bl recoveryData[CAN_MSG_LENGTH] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
static uint8_bl recoveryFlag = 0;

/******************************************************************************
* 函数名称: void UDS_RecvFrame(uint32_bl id, uint8_bl* frame_buf, uint8_bl frame_dlc)
* 功能说明: 接收到一帧报文
* 输入参数: uint32_bl    id              --消息帧 ID
    　　　　uint8_bl*    frame_buf       --接收报文帧数据首地址
    　　　　uint8_bl     frame_dlc       --接收报文帧数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: frame_dlc 长度必须等于 FRAME_SIZE，否则会被判断为无效帧
******************************************************************************/
void UDS_RecvFrame(void)
{
	static McalCanPduType_t pdu;
	int32_bl ret = IF_CanRead(&pdu);
	if (FBL_OK == ret)
	{
    	/* 检查是否进入强刷状态 */
    	if ((8U == pdu.dlc) &&
			(0U == memcmp(recoveryData, pdu.data, pdu.dlc)))
    	{
    		recoveryFlag = 1U;
    		return;
    	}

    	recoveryFlag = 0U;
	    if (REQUEST_ID == pdu.canId)
	    {
	        uds_tp_recv_frame(0, pdu.data, pdu.dlc);
	    }

	    if (FUNCTION_ID == pdu.canId)
	    {
	        uds_tp_recv_frame(1, pdu.data, pdu.dlc);
	    }
	}
}


/******************************************************************************
* 函数名称: void uds_send_frame(uint32_bl id, uint8_bl* frame_buf, uint8_bl frame_dlc)
* 功能说明: 发送一帧报文
* 输入参数: uint8_bl     response_id     --应答 ID
    　　　　uint8_bl*    frame_buf       --发送报文帧数据首地址
    　　　　uint8_bl     frame_dlc       --发送报文帧数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: frame_dlc 长度应当等于 FRAME_SIZE
******************************************************************************/
void uds_send_frame(uint32_bl response_id, uint8_bl *frame_buf, uint8_bl frame_dlc)
{
	static McalCanPduType_t pdu;
    pdu.canId = response_id;
    pdu.dlc = frame_dlc;
    uint8_bl i;
    for (i = 0; i < CAN_MSG_LENGTH; i++)
    {
        pdu.data[i] = frame_buf[i];
    }

	IF_CanWrite(&pdu);
}


/******************************************************************************
* 函数名称: void UDS_Init(void)
* 功能说明: UDS 初始化
* 输入参数: 无
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void UDS_Init(void)
{
    service_init();
    set_current_session(UDS_SESSION_PROG);
}


/******************************************************************************
* 函数名称: void uds_1ms_task(void)
* 功能说明: UDS 周期任务
* 输入参数: 无
* 输出参数: 无
* 函数返回: 无
* 其它说明: 该函数需要被 1ms 周期调用
******************************************************************************/
void UDS_1MsTask(void)
{
    network_task();
    service_task();
}

uint8_bl UDS_GetRecoveryFlag(void)
{
    return recoveryFlag;
}

uint32_bl GetMainLoopCnt(void)
{
    return mainLoopCnt;
}

void UDS_MainLoopCnt(void)
{
    mainLoopCnt++;
}

void SetUpdateRequestFlag(uint8_bl flag)
{
    updateRequestFlag = flag;
}

uint8_bl GetUpdateRequestFlag(void)
{
    return updateRequestFlag;
}

void SetUdsProgramResult(uint8_bl result)
{
    udsProgramResultFlag = result;
}

uint8_bl GetUdsProgramResult(void)
{
    return udsProgramResultFlag;
}

uint32_bl UdsPort_GetAppStartAddr(void)
{
    return BOOT_APP_START_ADDR;
}

int32_bl UdsPort_EraseAppImage(void)
{
    return IF_FlsErase(BOOT_APP_START_ADDR, (BOOT_APP_END_ADDR - BOOT_APP_START_ADDR));
}

int32_bl UdsPort_WriteAppFlash(uint32_bl addr, uint32_bl len, const uint8_bl *data)
{
    if ((data == (const uint8_bl *)0) || (len == 0u))
    {
        return 0u;
    }

    return IF_FlsWrite(addr, len, data);
}
