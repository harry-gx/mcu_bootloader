/******************************************************************************
 * @file    clock_if.h
 * @brief   系统时钟和运行模式驱动抽象层
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#ifndef CLOCK_IF_H
#define CLOCK_IF_H

#include "std_type.h"

struct McalSysClockDevice_s;
typedef struct McalSysClockDevice_s McalSysClockDevice_t;

/* 系统时钟驱动抽象层结构体定义*/
struct McalSysClockDevice_s
{
	/* 私有数据指针 */
	void *priv;
	/* 初始化 8MHz 系统外部振荡器(外部晶振)函数 */
	int32_bl (*InitSysClock)(McalSysClockDevice_t *);
};

/**
 * @brief 系统时钟设备注册并初始化
 * @param[in] 系统时钟设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_SysClockDevRegister(McalSysClockDevice_t *dev);

#endif /* CLOCK_IF_H */
