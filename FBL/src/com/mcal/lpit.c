/******************************************************************************
 * @file    lpit.c
 * @brief   定时器驱动
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#include "lpit.h"

/**
 * @brief 定时器0 - 初始化
 *
 * @param[in] 无
 *
 * @return 无
 */
int32_bl LPIT0_Init(McalLpitDevice_t *dev)
{
	int32_bl ret = FBL_OK;

	if (NULL == dev)
	{
		return FBL_ERR_PTR;
	}

	/*!
	* LPIT Clocking:
	* ==============================
	*/
	PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6);    /* Clock Src = 6 (SPLL2_DIV2_CLK)*/
	PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clk to LPIT0 regs 		*/

	/*!
	* LPIT Initialization:
	*/

	/* DBG_EN-0: Timer chans stop in Debug mode */
	/* DOZE_EN=0: Timer chans are stopped in DOZE mode */
	/* SW_RST=0: SW reset does not reset timer chans, regs */
	/* M_CEN=1: enable module clk (allows writing other LPIT0 regs) */
	LPIT0->MCR |= LPIT_MCR_M_CEN_MASK;

	/* Chan 0 Timeout period: 40M clocks */ // 40000000 -> 40000
	LPIT0->TMR[0].TVAL = 40000;

	/* T_EN=1: Timer channel is enabled */
	/* CHAIN=0: channel chaining is disabled */
	/* MODE=0: 32 periodic counter mode */
	/* TSOT=0: Timer decrements immediately based on restart */
	/* TSOI=0: Timer does not stop after timeout */
	/* TROT=0 Timer will not reload on trigger */
	/* TRG_SRC=0: External trigger soruce */
	/* TRG_SEL=0: Timer chan 0 trigger source is selected*/
	LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;

	return ret;
}

/**
 * @brief 定时器0 - 获取1ms标志
 *
 * @param[in] 无
 *
 * @return 1ms标志
 */
uint8_bl LPIT0_Get1MsFlag(void)
{
	return (LPIT0->MSR & LPIT_MSR_TIF0_MASK);
}

/**
 * @brief 定时器0 - 清理1ms标志
 *
 * @param[in] 无
 *
 * @return 无
 */
void LPIT0_Clean1MsFlag(void)
{
	LPIT0->MSR |= LPIT_MSR_TIF0_MASK;
}
