/******************************************************************************
 * @file    clock_mode.c
 * @brief   系统时钟和运行模式驱动
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#include "clock_mode.h"

/**
 * @brief 初始化 8MHz 系统外部振荡器(外部晶振)
 *
 * @param[in] 无
 *
 * @return 无
 */
static void CLOCK_Init8MHzSOSC(void)
{
	/*!
	 * SOSC Initialization (8 MHz):
	 * ===================================================
	 */
	SCG->SOSCDIV = SCG_SOSCDIV_SOSCDIV1(1)|
                   SCG_SOSCDIV_SOSCDIV2(1);    /* SOSCDIV1 & SOSCDIV2 =1: divide by 1 */
	SCG->SOSCCFG = SCG_SOSCCFG_RANGE(2)|       /* Range=2: Medium freq (SOSC betw 1MHz-8MHz) */
                   SCG_SOSCCFG_EREFS_MASK;     /* HGO=0:   Config xtal osc for low power */
                                               /* EREFS=1: Input is external XTAL */

	while(SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK); /* Ensure SOSCCSR unlocked */
	SCG->SOSCCSR = SCG_SOSCCSR_SOSCEN_MASK;    /* LK=0:          SOSCCSR can be written */
                                               /* SOSCCMRE=0:    OSC CLK monitor IRQ if enabled */
                                               /* SOSCCM=0:      OSC CLK monitor disabled */
                                               /* SOSCERCLKEN=0: Sys OSC 3V ERCLK output clk disabled */
                                               /* SOSCLPEN=0:    Sys OSC disabled in VLP modes */
                                               /* SOSCSTEN=0:    Sys OSC disabled in Stop modes */
                                               /* SOSCEN=1:      Enable oscillator */

	while(!(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK)); /* Wait for sys OSC clk valid */
}

/**
 * @brief 初始化 160MHz 系统锁相环(把 8MHz 外部晶振倍频成 160MHz)
 *
 * @param[in] 无
 *
 * @return 无
 */
static void CLOCK_Init160MHzSPLL(void)
{
	/*!
	 * SPLL Initialization (160 MHz):
	 * ===================================================
	 */
	while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); /* Ensure SPLLCSR unlocked */
	SCG->SPLLCSR &= ~SCG_SPLLCSR_SPLLEN_MASK;  /* SPLLEN=0: SPLL is disabled (default) */

	SCG->SPLLDIV |= SCG_SPLLDIV_SPLLDIV1(2)|   /* SPLLDIV1 divide by 2 */
					SCG_SPLLDIV_SPLLDIV2(3);   /* SPLLDIV2 divide by 4 */

	SCG->SPLLCFG = SCG_SPLLCFG_MULT(24);       /* PREDIV=0: Divide SOSC_CLK by 0+1=1 */
                                               /* MULT=24:  Multiply sys pll by 4+24=40 */
                                               /* SPLL_CLK = 8MHz / 1 * 40 / 2 = 160 MHz */

	while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); /* Ensure SPLLCSR unlocked */
	SCG->SPLLCSR |= SCG_SPLLCSR_SPLLEN_MASK;   /* LK=0:        SPLLCSR can be written */
                                               /* SPLLCMRE=0:  SPLL CLK monitor IRQ if enabled */
                                               /* SPLLCM=0:    SPLL CLK monitor disabled */
                                               /* SPLLSTEN=0:  SPLL disabled in Stop modes */
                                               /* SPLLEN=1:    Enable SPLL */

	while(!(SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK)); /* Wait for SPLL valid */
}

/**
 * @brief 切换系统主时钟到 SPLL，并配置成正常 RUN 模式下 Core 跑 80MHz
 *
 * @param[in] 无
 *
 * @return 无
 */
static void CLOCK_NormalRunMode80MHz(void)
{
	/*! Slow IRC is enabled with high range (8 MHz) in reset.
	*	Enable SIRCDIV2_CLK and SIRCDIV1_CLK, divide by 1 = 8MHz
	*   asynchronous clock source.
	* ==========================================
	*/
	SCG->SIRCDIV = SCG_SIRCDIV_SIRCDIV1(1) |
			       SCG_SIRCDIV_SIRCDIV2(1);

	/*!
	*  Change to normal RUN mode with 8MHz SOSC, 80 MHz PLL:
	*  ====================================================
	*/
	SCG->RCCR = SCG_RCCR_SCS(6) |        /* Select PLL as clock source */
				SCG_RCCR_DIVCORE(0b01) | /* DIVCORE=1, div. by 2: Core clock = 160/2 MHz = 80 MHz */
				SCG_RCCR_DIVBUS(0b01) |  /* DIVBUS=1, div. by 2: bus clock = 40 MHz */
				SCG_RCCR_DIVSLOW(0b10);  /* DIVSLOW=2, div. by 2: SCG slow, flash clock= 26 2/3 MHz	*/

	/* Wait for sys clk src = SPLL */
	while (((SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT ) != 6);
}

/**
 * @brief 系统时钟设备初始化
 * @param[in] 系统时钟设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl CLOCK_InitSysClock(McalSysClockDevice_t *dev)
{
	int32_bl ret = FBL_OK;

	if (NULL == dev)
	{
		return FBL_ERR_PTR;
	}

	CLOCK_Init8MHzSOSC();
	CLOCK_Init160MHzSPLL();
	CLOCK_NormalRunMode80MHz();

	return ret;
}
