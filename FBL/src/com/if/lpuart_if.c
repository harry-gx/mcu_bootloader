/******************************************************************************
 * @file    lpuart_if.c
 * @brief   串口驱动抽象层
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#include "lpuart_if.h"

/**
 * @brief 串口驱动设备注册并初始化
 * @param[in] 串口驱动设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_LpuartDevRegister(McalLpuartDevice_t *dev)
{
	int32_bl ret = FBL_OK;

	if (NULL == dev)
	{
		return FBL_ERR_PTR;
	}

	ret = dev->InitLpuart(dev);
	if (FBL_OK != ret)
	{
		return FBL_ERR;
	}

	return ret;
}
