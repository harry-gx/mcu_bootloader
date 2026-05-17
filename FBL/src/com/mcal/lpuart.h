/******************************************************************************
 * @file    lpuart.h
 * @brief   串口驱动
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#ifndef LPUART_H_
#define LPUART_H_

#include "lpuart_if.h"
#include "device_registers.h"

/**
 * @brief 串口驱动初始化
 * @param[in] 串口驱动设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl LPUART1_Init(McalLpuartDevice_t *dev);

#endif /* LPUART_H_ */
