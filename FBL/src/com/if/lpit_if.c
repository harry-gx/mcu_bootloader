/******************************************************************************
 * @file    lpit_if.c
 * @brief   LPIT驱动抽象层
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#include "lpit_if.h"

static McalLpitDevice_t *g_lpitDevice = NULL;

/**
 * @brief LPIT设备注册并初始化
 * @param[in] LPIT设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_LpitDevRegister(McalLpitDevice_t *dev)
{
	int32_bl ret = FBL_OK;

	g_lpitDevice = dev;
	if ((NULL == g_lpitDevice) || (NULL == dev))
	{
		return FBL_ERR_PTR;
	}

	ret = dev->InitLpit(dev);
	if (FBL_OK != ret)
	{
		return FBL_ERR;
	}

	return ret;
}

/**
 * @brief 获取1ms标志
 * @param[in] 无
 * @return 1ms标志
 */
uint8_bl IF_LpitGet1MsFlag(void)
{
	return g_lpitDevice->Get1MsFlag();
}

/**
 * @brief 清理1ms标志
 * @param[in] 无
 * @return 无
 */
void IF_LpitClean1MsFlag(void)
{
	g_lpitDevice->Clean1MsFlag();
}
