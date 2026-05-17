/******************************************************************************
 * @file    port_if.c
 * @brief   PORT驱动抽象层
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#include "port_if.h"

/**
 * @brief PORT设备注册并初始化
 * @param[in] PORT设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_PortDevRegister(McalPortDevice_t *dev)
{
	int32_bl ret = FBL_OK;

	if (NULL == dev)
	{
		return FBL_ERR_PTR;
	}

	ret = dev->InitPort(dev);
	if (FBL_OK != ret)
	{
		return FBL_ERR;
	}

	return ret;
}
