/******************************************************************************
 * @file    lpuart.c
 * @brief   串口驱动
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#include "lpuart.h"

/**
 * @brief 串口驱动初始化
 * @param[in] 串口驱动设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl LPUART1_Init(McalLpuartDevice_t *dev)
{
	if (NULL == dev)
	{
		return FBL_ERR_PTR;
	}

    uint32_bl uartClk = 8000000U;

    uint32_bl osr;
    uint32_bl sbr;

    uint32_bl bestOsr = 0;
    uint32_bl bestSbr = 0;

    uint32_bl calcBaud;
    uint32_bl error;
    uint32_bl minError = 0xFFFFFFFF;
    uint32_bl baudrate = dev->baudrate;

    /* 配置PCC */
    PCC->PCCn[PCC_LPUART1_INDEX] &= ~PCC_PCCn_CGC_MASK;

    PCC->PCCn[PCC_LPUART1_INDEX] = PCC_PCCn_PCS(0b010) |
                                   PCC_PCCn_CGC_MASK;

    /* 搜索最佳 OSR/SBR */
    for (osr = 4; osr <= 32; osr++)
    {
        sbr = uartClk / (baudrate * osr);

        if (sbr == 0)
        {
            continue;
        }

        calcBaud = uartClk / (osr * sbr);

        if (calcBaud > baudrate)
        {
            error = calcBaud - baudrate;
        }
        else
        {
            error = baudrate - calcBaud;
        }

        if (error < minError)
        {
            minError = error;

            bestOsr = osr;
            bestSbr = sbr;
        }
    }

    /* 配置BAUD */
    LPUART1->BAUD = LPUART_BAUD_OSR(bestOsr - 1U) |
                    LPUART_BAUD_SBR(bestSbr);

    /* OSR < 8 时建议双边采样 */
    if (bestOsr < 8)
    {
        LPUART1->BAUD |= LPUART_BAUD_BOTHEDGE_MASK;
    }

    /* Enable TX/RX */
    LPUART1->CTRL = LPUART_CTRL_TE_MASK |
                    LPUART_CTRL_RE_MASK;

    return FBL_OK;
}

/**
 * @brief 串口1 - 发送1字节数据
 *
 * @param[in] ch 1字节数据
 *
 * @return 无
 */
static void LPUART1_TransmitChar(uint8_t ch)
{
	/* Wait for transmit buffer to be empty */
	while ((LPUART1->STAT & LPUART_STAT_TDRE_MASK) >> LPUART_STAT_TDRE_SHIFT == 0);

	LPUART1->DATA = ch; /* Send data */
}

/**
 * @brief 串口1 - 接收1字节数据
 *
 * @param[in] 无
 *
 * @return 1字节数据
 */
static uint8_bl LPUART1_ReceiveChar(void)
{
	/* Wait for received buffer to be full */
	while ((LPUART1->STAT & LPUART_STAT_RDRF_MASK) >> LPUART_STAT_RDRF_SHIFT == 0);

	uint8_bl ch = LPUART1->DATA; /* Read received data*/
	return ch;
}

/**
 * @brief 串口重定向 - 发送数据
 *
 * @param[in] fd 句柄
 * @param[in] buf 数据
 * @param[in] len 数据长度
 *
 * @return 数据长度
 */
int _write(int fd, const char *buf, int len)
{
    (void)fd;

    uint32_bl i;
    for (i = 0; i < len; i++)
    {
    	LPUART1_TransmitChar(buf[i]);
    }

    return len;
}

/**
 * @brief 串口重定向 - 接收数据
 *
 * @param[in] fd 句柄
 * @param[in] buf 数据
 * @param[in] len 数据长度
 *
 * @return 数据长度
 */
int _read(int fd, char *buf, int len)
{
    (void)fd;

    uint32_bl i;
    for (i = 0; i < len; i++)
    {
    	uint8_bl ch = LPUART1_ReceiveChar();

        /* 回显 */
    	LPUART1_TransmitChar(ch);

        /* 回车结束 */
        if (ch == '\r' || ch == '\n')
        {
            buf[i] = '\n';
            return i + 1;
        }

        buf[i] = ch;
    }

    return len;
}
