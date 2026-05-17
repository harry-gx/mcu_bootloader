/******************************************************************************
 * @file    can_if.c
 * @brief   CAN驱动抽象层
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#include "can_if.h"

static McalCanDevice_t *g_canDevice = NULL;

/**
 * @brief CAN设备注册并初始化
 * @param[in] CAN设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_CanDevRegister(McalCanDevice_t *dev)
{
	int32_bl ret = FBL_OK;

	g_canDevice = dev;
	if ((NULL == g_canDevice) || (NULL == dev))
	{
		return FBL_ERR_PTR;
	}

	ret = dev->CanDevInit(dev);
	if (FBL_OK != ret)
	{
		return FBL_ERR;
	}

	return ret;
}

/**
 * @brief CAN接收数据
 * @param[out] 协议数据单元
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_CanRead(McalCanPduType_t *pdu)
{
	int32_bl ret = FBL_OK;

	if (NULL == pdu)
	{
		return FBL_ERR_PTR;
	}

	ret = g_canDevice->CanReadFunc(g_canDevice, pdu);
	if (FBL_OK != ret)
	{
		if (FBL_NOT_RECV == ret)
		{
			return FBL_NOT_RECV;
		}
		return FBL_ERR;
	}

	return ret;
}

/**
 * @brief CAN发送数据
 * @param[in] 协议数据单元
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_CanWrite(McalCanPduType_t *pdu)
{
	int32_bl ret = FBL_OK;

	if (NULL == pdu)
	{
		return FBL_ERR_PTR;
	}

	ret = g_canDevice->CanWriteFunc(g_canDevice, pdu);
	if (FBL_OK != ret)
	{
		return FBL_ERR;
	}

	return ret;
}
