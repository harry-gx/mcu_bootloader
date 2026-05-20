#ifndef BOOT_PORT_H
#define BOOT_PORT_H

/*
 * 文件名称: boot_port.h
 * 文件说明: Boot 平台适配层接口声明（与硬件、驱动、协议栈交互）
 */

#include "std_type.h"
#include "bsm.h"

/*
 * 枚举名称: BsmUdsRetE
 * 功能说明: 平台层返回给状态机的 UDS 流程结果
 */
typedef enum __BsmUdsRetE
{
	BSM_UDS_IDLE = 0,       /* 无升级动作 */
	BSM_UDS_UPDATE_REQUEST, /* 收到升级请求 */
	BSM_UDS_PROGRAM_DONE,   /* 刷写完成 */
	BSM_UDS_PROGRAM_FAIL    /* 刷写失败 */
} BsmUdsRetE;

/*
 * 函数名称: BSM_Init
 * 功能说明: 初始化状态机上下文
 * 输入参数: ctx - 状态机上下文指针
 * 输出参数: ctx - 初始化后的上下文
 * 返回值: 无
 */
void BSM_Init(boot_context_t *ctx);

/*
 * 函数名称: BSM_RunStep
 * 功能说明: 执行一次状态机步进
 * 输入参数: ctx - 状态机上下文指针
 * 输出参数: ctx - 可能被更新的上下文
 * 返回值: 无
 */
void BSM_RunStep(boot_context_t *ctx);

/*
 * 函数名称: BSM_Take1msTick
 * 功能说明: 读取并清除 LPIT 1ms 标志
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: TRUE=有tick, FALSE=无tick
 */
bool_bl BSM_Take1msTick(void);

/*
 * 函数名称: BSM_ReadUpdateFlag
 * 功能说明: 读取升级标志
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: 1=已置位，0=未置位
 */
uint8_bl BSM_ReadUpdateFlag(void);

/*
 * 函数名称: BSM_ClearUpdateFlag
 * 功能说明: 清除升级标志
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: 清除后标志状态（1=仍置位，0=已清除）
 */
uint8_bl BSM_ClearUpdateFlag(void);

/*
 * 函数名称: BSM_AppExist
 * 功能说明: 判断 APP 是否存在（通过 magic）
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: 1=存在，0=不存在
 */
uint8_bl BSM_AppExist(void);

/*
 * 函数名称: BSM_AppValid
 * 功能说明: 判断 APP 是否有效（magic/长度边界/CRC）
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: 1=有效，0=无效
 */
uint8_bl BSM_AppValid(void);

/*
 * 函数名称: BSM_PollUpdateReq
 * 功能说明: 轮询是否收到升级请求
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: UDS 状态码
 */
BsmUdsRetE BSM_PollUpdateReq(void);

/*
 * 函数名称: BSM_ProgramProcess
 * 功能说明: 执行升级过程处理
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: UDS 状态码
 */
BsmUdsRetE BSM_ProgramProcess(void);

/*
 * 函数名称: BSM_JumpToApp
 * 功能说明: 标准 Cortex-M 跳转 APP
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: 无（成功通常不返回）
 */
void BSM_JumpToApp(void);

#endif
