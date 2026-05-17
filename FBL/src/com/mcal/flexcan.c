/******************************************************************************
 * @file    FlexCAN.c
 * @brief   CAN驱动
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#include "flexcan.h"

#define FLEXCAN0_CLK_HZ 8000000UL
#define FLEXCAN_TQ_NUM  16UL
#define MSG_BUF_SIZE    4UL

uint32_bl RxCODE;
uint32_bl RxTIMESTAMP;

/**
 * @brief CAN驱动初始化
 * @param[in] CAN设备指针
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl FLEXCAN0_Init(McalCanDevice_t *dev)
{
	if (NULL == dev)
	{
		return FBL_ERR_PTR;
	}

    /*
     * CAN baudrate = CAN_CLK / ((PRESDIV + 1) * TQ)
     * PRESDIV = CAN_CLK / (baudrate * TQ) - 1
     */
//  uint32_bl baudrate = dev->baudrate;
//  uint32_bl presdiv = (FLEXCAN0_CLK_HZ / (baudrate * FLEXCAN_TQ_NUM)) - 1U;

	PCC->PCCn[PCC_FlexCAN0_INDEX] |= PCC_PCCn_CGC_MASK; /* CGC=1: enable clock to FlexCAN0 */

	CAN0->MCR |= CAN_MCR_MDIS_MASK;        /* MDIS=1: Disable module before selecting clock */
	CAN0->CTRL1 &= ~CAN_CTRL1_CLKSRC_MASK; /* CLKSRC=0: Clock Source = SOSCDIV2 */
	CAN0->MCR &= ~CAN_MCR_MDIS_MASK;       /* MDIS=0; Enable module config. (Sets FRZ, HALT) */

	while (!((CAN0->MCR & CAN_MCR_FRZACK_MASK) >> CAN_MCR_FRZACK_SHIFT));
	/*!
	 * Good practice:
	 * ===================================================
	 * wait for FRZACK=1 on freeze mode entry/exit
	 */
	CAN0->CTRL1 = 0 //CAN_CTRL1_PRESDIV(presdiv)
				| CAN_CTRL1_PSEG2(3)   /* Configure for 500 KHz bit time */
                | CAN_CTRL1_PSEG1(3)   /* Time quanta freq = 16 time quanta x 500 KHz bit time= 8MHz */
                | CAN_CTRL1_PROPSEG(6) /* PRESDIV+1 = Fclksrc/Ftq = 8 MHz/8 MHz = 1 */
                | CAN_CTRL1_RJW(3)     /* so PRESDIV = 0 */
                | CAN_CTRL1_SMP(1);    /* PSEG2 = Phase_Seg2 - 1 = 4 - 1 = 3 */
                                       /* PSEG1 = PSEG2 = 3 */
                                       /* PROPSEG= Prop_Seg - 1 = 7 - 1 = 6 */
                                       /* RJW: since Phase_Seg2 >=4, RJW+1=4 so RJW=3. */
                                       /* SMP = 1: use 3 bits per CAN sample */
                                       /* CLKSRC=0 (unchanged): Fcanclk= Fosc= 8 MHz */

	uint32_bl i = 0;
	for(i = 0; i < 128; i++)
	{   					/* CAN0: clear 32 msg bufs x 4 words/msg buf = 128 words */
		CAN0->RAMn[i] = 0;  /* Clear msg buf word */
	}
	for(i = 0; i < 16; i++)
	{          						 /* In FRZ mode, init CAN0 16 msg buf filters */
		CAN0->RXIMR[i] = 0xFFFFFFFF; /* Check all ID bits for incoming messages */
	}
	CAN0->RXMGMASK = 0x1FFFFFFF;  				  /* Global acceptance mask: check all ID bits */
	CAN0->RAMn[ 4*MSG_BUF_SIZE + 0] = 0x04000000; /* Msg Buf 4, word 0: Enable for reception */
												  /* EDL,BRS,ESI=0: CANFD not used */
												  /* CODE=4: MB set to RX inactive */
												  /* IDE=0: Standard ID */
												  /* SRR, RTR, TIME STAMP = 0: not applicable */

	CAN0->RAMn[ 4*MSG_BUF_SIZE + 1] = 0x14440000; /* Msg Buf 4, word 1: Standard ID = 0x111 */
	CAN0->MCR = 0x0000001F; /* Negate FlexCAN 1 halt state for 32 MBs */

	while ((CAN0->MCR && CAN_MCR_FRZACK_MASK) >> CAN_MCR_FRZACK_SHIFT);
	/* Good practice: wait for FRZACK to clear (not in freeze mode) */

	while ((CAN0->MCR && CAN_MCR_NOTRDY_MASK) >> CAN_MCR_NOTRDY_SHIFT);
	/* Good practice: wait for NOTRDY to clear (module ready) */

	return FBL_OK;
}

/**
 * @brief CAN发送数据
 * @param[in] 协议数据单元
 * @return FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl FLEXCAN0_TransmitMsg(McalCanDevice_t *dev, McalCanPduType_t *pdu)
{
	if ((NULL == dev) || (NULL == pdu))
	{
		return FBL_ERR_PTR;
	}

	CAN0->IFLAG1 = 0x00000001;
	CAN0->RAMn[ 0*MSG_BUF_SIZE + 1] = pdu->canId << 18;
	CAN0->RAMn[ 0*MSG_BUF_SIZE + 2] = pdu->data[0] << 24 | pdu->data[1] << 16 | pdu->data[2] << 8 | pdu->data[3];
	CAN0->RAMn[ 0*MSG_BUF_SIZE + 3] = pdu->data[4] << 24 | pdu->data[5] << 16 | pdu->data[6] << 8 | pdu->data[7];
	CAN0->RAMn[ 0*MSG_BUF_SIZE + 0] = 0x0C400000 | pdu->dlc << CAN_WMBn_CS_DLC_SHIFT;

	return FBL_OK;
}

/**
 * @brief CAN接收数据
 * @param[out] 协议数据单元
 * @return FBL_NOT_RECV:未接收到数据
 *         FBL_OK:成功
 *         FBL_ERR:错误
 *         FBL_ERR_PTR:错误的指针
 */
int32_bl FLEXCAN0_ReceiveMsg(McalCanDevice_t *dev, McalCanPduType_t *pdu)
{
	/* 未接收到消息，直接退出 */
	if (0 == ((CAN0->IFLAG1 >> 4) & 1))
	{
		return FBL_NOT_RECV;
	}

	if ((NULL == dev) || (NULL == pdu))
	{
		return FBL_ERR_PTR;
	}

	/* Read CODE field */
	RxCODE = (CAN0->RAMn[ 4*MSG_BUF_SIZE + 0] & 0x07000000) >> 24;
	/* Read ID */
	uint32_bl RxID = (CAN0->RAMn[ 4*MSG_BUF_SIZE + 1] & CAN_WMBn_ID_ID_MASK) >> CAN_WMBn_ID_ID_SHIFT;
	pdu->canId = RxID >> 18;
	/* Read Message Length */
	pdu->dlc = (CAN0->RAMn[ 4*MSG_BUF_SIZE + 0] & CAN_WMBn_CS_DLC_MASK) >> CAN_WMBn_CS_DLC_SHIFT;

	/* 读取 8 个字节数据到 buf中 */
	pdu->data[0] = CAN0->RAMn[ 4*MSG_BUF_SIZE + 2 + 0] >> 24;
	pdu->data[1] = CAN0->RAMn[ 4*MSG_BUF_SIZE + 2 + 0] >> 16;
	pdu->data[2] = CAN0->RAMn[ 4*MSG_BUF_SIZE + 2 + 0] >> 8;
	pdu->data[3] = CAN0->RAMn[ 4*MSG_BUF_SIZE + 2 + 0] >> 0;
	pdu->data[4] = CAN0->RAMn[ 4*MSG_BUF_SIZE + 2 + 1] >> 24;
	pdu->data[5] = CAN0->RAMn[ 4*MSG_BUF_SIZE + 2 + 1] >> 16;
	pdu->data[6] = CAN0->RAMn[ 4*MSG_BUF_SIZE + 2 + 1] >> 8;
	pdu->data[7] = CAN0->RAMn[ 4*MSG_BUF_SIZE + 2 + 1] >> 0;

	RxTIMESTAMP = (CAN0->RAMn[ 0*MSG_BUF_SIZE + 0] & 0x000FFFF);
	/* Clear CAN 0 MB 4 flag without clearing others*/
	CAN0->IFLAG1 = 0x00000010;

	return FBL_OK;
}
