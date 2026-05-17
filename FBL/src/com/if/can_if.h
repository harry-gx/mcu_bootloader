/******************************************************************************
 * @file    can_if.h
 * @brief   CAN驱动抽象层
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#ifndef CAN_IF_H
#define CAN_IF_H

#include "std_type.h"

#define CAN_MSG_LENGTH 8U

struct McalCanDevice_s;
typedef struct McalCanDevice_s McalCanDevice_t;

/* 本结构体定义MCAL层数据传输的基本单元，作为底层驱动与上层协议的桥梁 */
typedef struct
{
	/* CAN ID */
	uint32_bl canId;
	/* 帧长度 */
	uint8_bl dlc;
	/* 存放一帧数据的缓冲区 */
	uint8_bl data[CAN_MSG_LENGTH];
} McalCanPduType_t;

/* CAN MCAL驱动抽象层结构体定义*/
struct McalCanDevice_s
{
    /* 波特率 */
    uint32_bl baudrate;
	/* 设备初始化函数 */
	int32_bl (*CanDevInit)(McalCanDevice_t *);
	/* 读取数据函数 */
	int32_bl (*CanReadFunc)(McalCanDevice_t *, McalCanPduType_t *);
	/* 发送数据函数 */
	int32_bl (*CanWriteFunc)(McalCanDevice_t *, McalCanPduType_t *);
};

/**
 * @brief CAN设备注册并初始化
 * @param[in] CAN设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_CanDevRegister(McalCanDevice_t *dev);

/**
 * @brief CAN接收数据
 * @param[out] 协议数据单元
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_CanRead(McalCanPduType_t *pdu);

/**
 * @brief CAN发送数据
 * @param[in] 协议数据单元
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_CanWrite(McalCanPduType_t *pdu);

#endif /* CAN_IF_H */
