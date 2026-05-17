/******************************************************************************
 * @file    lpit_if.h
 * @brief   LPIT驱动抽象层
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#ifndef LPIT_IF_H
#define LPIT_IF_H

#include "std_type.h"

struct McalLpitDevice_s;
typedef struct McalLpitDevice_s McalLpitDevice_t;

/* LPIT驱动抽象层结构体定义*/
struct McalLpitDevice_s
{
	/* 私有数据指针 */
	void *priv;
	/* 初始化LPIT函数 */
	int32_bl (*InitLpit)(McalLpitDevice_t *);
	/* 获取1ms标志函数 */
	uint8_bl (*Get1MsFlag)(void);
	/* 清理1ms标志函数 */
	void (*Clean1MsFlag)(void);
};

/**
 * @brief LPIT设备注册并初始化
 * @param[in] LPIT设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_LpitDevRegister(McalLpitDevice_t *dev);

/**
 * @brief 获取1ms标志
 * @param[in] 无
 * @return 1ms标志
 */
uint8_bl IF_LpitGet1MsFlag(void);

/**
 * @brief 清理1ms标志
 * @param[in] 无
 * @return 无
 */
void IF_LpitClean1MsFlag(void);

#endif /* LPIT_IF_H */
