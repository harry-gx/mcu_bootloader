/******************************************************************************
 * @file    port.c
 * @brief   PORT驱动
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#include "port.h"

/**
 * @brief 禁用看门狗
 *
 * @param[in] 无
 *
 * @return 无
 */
static void WdogDisable(void)
{
	WDOG->CNT=0xD928C520;   /* Unlock watchdog */
	WDOG->TOVAL=0x0000FFFF; /* Maximum timeout value */
	WDOG->CS = 0x00002100;  /* Disable watchdog */
}

/**
 * @brief 初始化GPIO
 *
 * @param[in] 无
 *
 * @return 无
 */
static void PortInit(void)
{
	/*!
	 * Pins definitions
	 * ===================================================
	 *
	 * Pin number        | Function
	 * ----------------- |------------------
	 * PTD0              | GPIO [BLUE LED]
	 */
	PCC-> PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK; /* Enable clock for PORT D */
	PTD->PDDR |= 1<<0;                               /* Port D0:  Data Direction= output */
	PORTD->PCR[0] |=  PORT_PCR_MUX(1);               /* Port D0:  MUX = ALT1, GPIO (to blue LED on EVB) */

	/*!
	 * Pins definitions
	 * ===================================================
	 *
	 * Pin number        | Function
	 * ----------------- |------------------
	 * PTC6              | UART1 TX
	 * PTC7              | UART1 RX
	 */
	PCC->PCCn[PCC_PORTC_INDEX ]|=PCC_PCCn_CGC_MASK; /* Enable clock for PORTC */
	PORTC->PCR[6]|=PORT_PCR_MUX(2);                 /* Port C6: MUX = ALT2, UART1 TX */
	PORTC->PCR[7]|=PORT_PCR_MUX(2);                 /* Port C7: MUX = ALT2, UART1 RX */

	/*!
	 * Pins definitions
	 * ===================================================
	 *
	 * Pin number        | Function
	 * ----------------- |------------------
	 * PTE4              | CAN0_RX
	 * PTE5              | CAN0_TX
	 */
	PCC->PCCn[PCC_PORTE_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clock for PORTE */
	PORTE->PCR[4] |= PORT_PCR_MUX(5);                /* Port E4: MUX = ALT5, CAN0_RX */
	PORTE->PCR[5] |= PORT_PCR_MUX(5);                /* Port E5: MUX = ALT5, CAN0_TX */
}

/**
 * @brief PORT驱动设备初始化
 * @param[in] PORT驱动设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl PORT_InitPort(McalPortDevice_t *dev)
{
	int32_bl ret = FBL_OK;

	if (NULL == dev)
	{
		return FBL_ERR_PTR;
	}

	WdogDisable();
	PortInit();

	return ret;
}
