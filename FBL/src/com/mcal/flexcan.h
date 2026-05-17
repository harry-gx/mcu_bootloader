/******************************************************************************
 * @file    FlexCAN.h
 * @brief   CAN驱动
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#ifndef FLEXCAN_H
#define FLEXCAN_H

#include "can_if.h"
#include "device_registers.h"

/**
 * @brief CAN驱动初始化
 * @param[in] CAN设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl FLEXCAN0_Init(McalCanDevice_t *dev);

/**
 * @brief CAN发送数据
 * @param[in] 协议数据单元
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl FLEXCAN0_TransmitMsg(McalCanDevice_t *dev, McalCanPduType_t *pdu);

/**
 * @brief CAN接收数据
 * @param[out] 协议数据单元
 * @return FBL_NOT_RECV:未接收到数据
 *         FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl FLEXCAN0_ReceiveMsg(McalCanDevice_t *dev, McalCanPduType_t *pdu);

#endif /* FLEXCAN_H */
