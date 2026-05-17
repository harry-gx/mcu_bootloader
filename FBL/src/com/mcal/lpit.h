/******************************************************************************
 * @file    lpit.h
 * @brief   定时器驱动
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#ifndef LPIT_H
#define LPIT_H

#include "lpit_if.h"
#include "device_registers.h"

/**
 * @brief 定时器0 - 初始化
 *
 * @param[in] 无
 *
 * @return 无
 */
int32_bl LPIT0_Init(McalLpitDevice_t *dev);

/**
 * @brief 定时器0 - 获取1ms标志
 *
 * @param[in] 无
 *
 * @return 1ms标志
 */
uint8_bl LPIT0_Get1MsFlag(void);

/**
 * @brief 定时器0 - 清理1ms标志
 *
 * @param[in] 无
 *
 * @return 无
 */
void LPIT0_Clean1MsFlag(void);

#endif /* LPIT_H */
