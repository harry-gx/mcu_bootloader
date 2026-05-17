/******************************************************************************
 * @file    fls_if.c
 * @brief   flash驱动抽象层
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#ifndef FLS_IF_H
#define FLS_IF_H

#include "std_type.h"

struct McalFlsDevice_s;
typedef struct McalFlsDevice_s McalFlsDevice_t;

/* CAN MCAL驱动抽象层结构体定义*/
struct McalFlsDevice_s
{
	/* 私有数据指针 */
	void *priv;
	/* flash设备初始化函数 */
	int32_bl (*FlsDevInit)(McalFlsDevice_t *);
	/* flash读数据函数 */
	int32_bl (*FlsReadFunc)(uint32_bl startAddr, uint32_bl size, uint8_bl *pData);
	/* flash写数据函数 */
	int32_bl (*FlsWriteFunc)(uint32_bl startAddr, uint32_bl size, const uint8_bl *pData);
	/* flash擦除函数 */
	int32_bl (*FlsEraseFunc)(uint32_bl startAddr, uint32_bl size);
};

/**
 * @brief flash设备注册并初始化
 * @param[in] flash设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_FlsDevRegister(McalFlsDevice_t *dev);

/**
 * @brief flash读数据
 * @param[in] startAddr 起始地址
 * @param[out] size 数据大小
 * @param[out] pData 数据
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_FlsRead(uint32_bl startAddr, uint32_bl size, uint8_bl *pData);

/**
 * @brief flash写数据
 * @param[in] startAddr 起始地址
 * @param[in] size 数据大小
 * @param[in] pData 数据
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_FlsWrite(uint32_bl startAddr, uint32_bl size, const uint8_bl *pData);

/**
 * @brief flash擦除
 * @param[in] startAddr 起始地址
 * @param[in] size 数据大小
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_FlsErase(uint32_bl startAddr, uint32_bl size);

#endif /* FLS_IF_H */
