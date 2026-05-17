/******************************************************************************
 * @file    port_if.h
 * @brief   PORT驱动抽象层
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#ifndef PORT_IF_H
#define PORT_IF_H

#include "std_type.h"

struct McalPortDevice_s;
typedef struct McalPortDevice_s McalPortDevice_t;

/* PORT驱动抽象层结构体定义*/
struct McalPortDevice_s
{
	/* 私有数据指针 */
	void *priv;
	/* 初始化PORT函数 */
	int32_bl (*InitPort)(McalPortDevice_t *);
};

/**
 * @brief PORT设备注册并初始化
 * @param[in] PORT设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl IF_PortDevRegister(McalPortDevice_t *dev);

#endif /* PORT_IF_H */
