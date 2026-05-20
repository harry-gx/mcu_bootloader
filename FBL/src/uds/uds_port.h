/******************************************************************************
* 文件名称: uds_port.h
* 内容摘要: UDS 协议栈移植接口头文件
* 创建者の: 孔佳伟
* 个人主页: https://gitee.com/openes
* 修改记录:
******************************************************************************/

#ifndef _UDS_PORT_H_
#define _UDS_PORT_H_

#include "std_type.h"

#define FRAME_SIZE      8               // 帧长度

#define REQUEST_ID      0x511           // 请求 ID
#define FUNCTION_ID     0x7DF           // 功能 ID
#define RESPONSE_ID     0x666           // 应答 ID

#define UDS_RX_MAX      1024            // 接收缓冲区长度 --最大支持 4095 字节，可根据实际需求改动，以免资源浪费
#define UDS_TX_MAX      128             // 发送缓冲区长度 --最大支持 4095 字节，可根据实际需求改动，以免资源浪费

#define PADDING_VAL     0xAA            // 填充值，如果发送的有效数据不满一帧，则用该值填充


/******************************************************************************
* 函数名称: void UDS_RecvFrame(uint32_t id, uint8_t* frame_buf, uint8_t frame_dlc)
* 功能说明: 接收到一帧报文
* 输入参数: uint32_t    id              --消息帧 ID
    　　　　uint8_t*    frame_buf       --接收报文帧数据首地址
    　　　　uint8_t     frame_dlc       --接收报文帧数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: frame_dlc 长度必须等于 FRAME_SIZE，否则会被判断为无效帧
******************************************************************************/
void UDS_RecvFrame(void);


/******************************************************************************
* 函数名称: void uds_send_frame(uint32_t id, uint8_t* frame_buf, uint8_t frame_dlc)
* 功能说明: 发送一帧报文
* 输入参数: uint8_t     response_id     --应答 ID
    　　　　uint8_t*    frame_buf       --发送报文帧数据首地址
    　　　　uint8_t     frame_dlc       --发送报文帧数据长度
* 输出参数: 无
* 函数返回: 无
* 其它说明: frame_dlc 长度应当等于 FRAME_SIZE
******************************************************************************/
void uds_send_frame(uint32_bl response_id, uint8_bl *frame_buf, uint8_bl frame_dlc);


/******************************************************************************
* 函数名称: void UDS_Init(void)
* 功能说明: UDS 初始化
* 输入参数: 无
* 输出参数: 无
* 函数返回: 无
* 其它说明: 无
******************************************************************************/
void UDS_Init(void);


/******************************************************************************
* 函数名称: void UDS_1MsTask(void)
* 功能说明: UDS 周期任务
* 输入参数: 无
* 输出参数: 无
* 函数返回: 无
* 其它说明: 该函数需要被 1ms 周期调用
******************************************************************************/
void UDS_1MsTask(void);

uint8_bl UDS_GetRecoveryFlag(void);
uint32_bl GetMainLoopCnt(void);
void UDS_MainLoopCnt(void);

/* UDS programming flow state flags */
#define UDS_PROGRAM_RESULT_IDLE   (0u)
#define UDS_PROGRAM_RESULT_DONE   (1u)
#define UDS_PROGRAM_RESULT_FAIL   (2u)
#define UDS_HEADER_PREFIX_SIZE    (16u)

void SetUpdateRequestFlag(uint8_bl flag);
uint8_bl GetUpdateRequestFlag(void);

void SetUdsProgramResult(uint8_bl result);
uint8_bl GetUdsProgramResult(void);

void ClearUdsHeaderPrefix(void);
void StoreUdsHeaderPrefix(const uint8_bl *data, uint32_bl len);
uint8_bl LoadUdsHeaderPrefix(uint8_bl *out, uint32_bl len);

uint32_bl UdsPort_GetAppStartAddr(void);
int32_bl UdsPort_EraseAppImage(void);
int32_bl UdsPort_WriteAppFlash(uint32_bl addr, uint32_bl len, const uint8_bl *data);
uint8_bl UdsPort_CheckFlashProgramCompatible(uint32_bl addr, const uint8_bl *data, uint32_bl len);

#endif

