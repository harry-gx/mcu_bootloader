/******************************************************************************
 * @file    crc32.h
 * @brief   校验镜像的工具
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#ifndef CRC_32_H
#define CRC_32_H

#include "std_type.h"

/**
 * @brief 计算镜像crc32校验
 * @param[in] 校验初始化数据
 * @param[in] 待校验数据
 * @param[in] 待校验数据大小
 * @return 校验码
 */
uint32_bl CRC_CalcImageCrc(uint32_bl crcInit,
                           const uint8_bl *buffer,
                           uint32_bl size);

#endif /* CRC_32_H */
