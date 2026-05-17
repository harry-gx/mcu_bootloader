/******************************************************************************
 * @file    fls_if.c
 * @brief   flash驱动抽象层
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#include "fls_if.h"

static McalFlsDevice_t *g_flsDevice = NULL;

/**
 * @brief flash设备注册并初始化
 * @param[in] flash设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_FlsDevRegister(McalFlsDevice_t *dev)
{
	int32_bl ret = FBL_OK;

	g_flsDevice = dev;
	if ((NULL == g_flsDevice) || (NULL == dev))
	{
		return FBL_ERR_PTR;
	}

	ret = dev->FlsDevInit(dev);
	if (FBL_OK != ret)
	{
		return FBL_ERR;
	}

	return ret;
}

/**
 * @brief flash读数据
 * @param[in] startAddr 起始地址
 * @param[out] size 数据大小
 * @param[out] pData 数据
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_FlsRead(uint32_bl startAddr, uint32_bl size, uint8_bl *pData)
{
	int32_bl ret = FBL_OK;

	if (NULL == pData)
	{
		return FBL_ERR_PTR;
	}

	ret = g_flsDevice->FlsReadFunc(startAddr, size, pData);
	if (FBL_OK != ret)
	{
		return FBL_ERR;
	}

	return ret;
}

/**
 * @brief flash写数据
 * @param[in] startAddr 起始地址
 * @param[in] size 数据大小
 * @param[in] pData 数据
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_FlsWrite(uint32_bl startAddr, uint32_bl size, const uint8_bl *pData)
{
	int32_bl ret = FBL_OK;

	if (NULL == pData)
	{
		return FBL_ERR_PTR;
	}

	ret = g_flsDevice->FlsWriteFunc(startAddr, size, pData);
	if (FBL_OK != ret)
	{
		return FBL_ERR;
	}

	return ret;
}

/**
 * @brief flash擦除
 * @param[in] startAddr 起始地址
 * @param[in] size 数据大小
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_FlsErase(uint32_bl startAddr, uint32_bl size)
{
	int32_bl ret = FBL_OK;

	ret = g_flsDevice->FlsEraseFunc(startAddr, size);
	if (FBL_OK != ret)
	{
		return FBL_ERR;
	}

	return ret;
}
