#ifndef BOOT_PORT_H
#define BOOT_PORT_H

/*
 * 文件名称: boot_port.h
 * 文件说明: Boot 平台适配层接口声明（与硬件、驱动、协议栈交互）
 */

#include "std_type.h"

/*
 * 枚举名称: BootPortUdsRetE
 * 功能说明: 平台层返回给状态机的 UDS 流程结果
 */
typedef enum __BootPortUdsRetE
{
    BOOT_PORT_UDS_IDLE = 0,        /* 无升级动作 */
    BOOT_PORT_UDS_UPDATE_REQUEST,  /* 收到升级请求 */
    BOOT_PORT_UDS_PROGRAM_DONE,    /* 刷写完成 */
    BOOT_PORT_UDS_PROGRAM_FAIL     /* 刷写失败 */
} BootPortUdsRetE;

/*
 * 函数名称: BOOT_PortTake1msTick
 * 功能说明: 读取并清除 LPIT 1ms 标志
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: TRUE=有tick, FALSE=无tick
 */
bool_bl BOOT_PortTake1msTick(void);

/*
 * 函数名称: BOOT_PortReadUpdateFlag
 * 功能说明: 读取升级标志
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: 1=已置位，0=未置位
 */
uint8_bl BOOT_PortReadUpdateFlag(void);

/*
 * 函数名称: BOOT_PortClearUpdateFlag
 * 功能说明: 清除升级标志
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: 清除后标志状态（1=仍置位，0=已清除）
 */
uint8_bl BOOT_PortClearUpdateFlag(void);

/*
 * 函数名称: BOOT_PortAppExist
 * 功能说明: 判断 APP 是否存在（通过 magic）
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: 1=存在，0=不存在
 */
uint8_bl BOOT_PortAppExist(void);

/*
 * 函数名称: BOOT_PortAppValid
 * 功能说明: 判断 APP 是否有效（magic/长度边界/CRC）
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: 1=有效，0=无效
 */
uint8_bl BOOT_PortAppValid(void);

/*
 * 函数名称: BOOT_PortPollUpdateReq
 * 功能说明: 轮询是否收到升级请求
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: UDS 状态码
 */
BootPortUdsRetE BOOT_PortPollUpdateReq(void);

/*
 * 函数名称: BOOT_PortProgramProcess
 * 功能说明: 执行升级过程处理
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: UDS 状态码
 */
BootPortUdsRetE BOOT_PortProgramProcess(void);

/*
 * 函数名称: BOOT_PortJumpToApp
 * 功能说明: 标准 Cortex-M 跳转 APP
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: 无（成功通常不返回）
 */
void BOOT_PortJumpToApp(void);

#endif
