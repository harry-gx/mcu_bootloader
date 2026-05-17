/******************************************************************************
 * @file    lpuart_if.h
 * @brief   串口驱动抽象层
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#ifndef LPUART_IF_H
#define LPUART_IF_H

#include "std_type.h"

struct McalLpuartDevice_s;
typedef struct McalLpuartDevice_s McalLpuartDevice_t;

/* 串口驱动抽象层结构体定义*/
struct McalLpuartDevice_s
{
    /* 波特率 */
    uint32_bl baudrate;
	/* 初始化串口函数 */
	int32_bl (*InitLpuart)(McalLpuartDevice_t *);
};

/**
 * @brief 串口驱动设备注册并初始化
 * @param[in] 串口驱动设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_LpuartDevRegister(McalLpuartDevice_t *dev);

#endif /* LPUART_IF_H */
