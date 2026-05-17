/******************************************************************************
 * @file    flash.h
 * @brief   flash驱动
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#ifndef FLASH_H
#define FLASH_H

#include "fls_if.h"
#include "device_registers.h"
#include "flash_driver.h"
#include "status.h"

/**
 * @brief flash初始化
 *
 * @param[in] 无
 *
 * @return 无
 */
int32_bl FLASH_Init(McalFlsDevice_t *dev);

/**
 * @brief 从flash指定地址读数据
 *
 * @param[in] startAddr 起始地址
 * @param[out] size 数据大小
 * @param[out] pData 数据
 *
 * @return 错误状态
 */
int32_bl FLASH_Read(uint32_bl startAddr, uint32_bl size, uint8_bl *pData);

/**
 * @brief 向flash指定地址写数据
 *
 * @param[in] startAddr 起始地址
 * @param[in] size 数据大小
 * @param[in] pData 数据
 *
 * @return 错误状态
 */
int32_bl FLASH_Write(uint32_bl startAddr, uint32_bl size, const uint8_bl *pData);

/**
 * @brief 从flash指定地址擦除
 *
 * @param[in] startAddr 起始地址
 * @param[in] size 擦除大小
 *
 * @return 错误状态
 */
int32_bl FLASH_Erase(uint32_bl startAddr, uint32_bl size);

#endif /* FLASH_H */
