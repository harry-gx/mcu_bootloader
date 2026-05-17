/******************************************************************************
 * @file    port.h
 * @brief   PORT驱动
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#include "port_if.h"
#include "device_registers.h"

#ifndef PORT_H
#define PORT_H

/**
 * @brief PORT驱动设备初始化
 * @param[in] PORT驱动设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl PORT_InitPort(McalPortDevice_t *dev);

#endif /* PORT_H */
