/******************************************************************************
 * @file    com.h
 * @brief   驱动注册
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#ifndef HAL_H
#define HAL_H

#include "std_type.h"

#include "port.h"
#include "clock_mode.h"
#include "lpit.h"
#include "flash.h"
#include "lpuart.h"
#include "flexcan.h"

/**
 * @brief 板卡驱动设备注册并初始化
 * @param[in] 无
 * @return 无
 */
void COM_DeviceInit(void);

#endif /* HAL_H */
