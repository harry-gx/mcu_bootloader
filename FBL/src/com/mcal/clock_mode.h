/******************************************************************************
 * @file    clock_mode.h
 * @brief   系统时钟和运行模式驱动
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#include "clock_if.h"
#include "device_registers.h"

#ifndef CLOCK_MODE_H
#define CLOCK_MODE_H

/**
 * @brief 系统时钟设备初始化
 * @param[in] 系统时钟设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl CLOCK_InitSysClock(McalSysClockDevice_t *dev);

#endif /* CLOCK_MODE_H */
