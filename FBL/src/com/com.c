/******************************************************************************
 * @file    com.c
 * @brief   驱动注册
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#include "com.h"
#include "device_registers.h"

static McalPortDevice_t g_portDevice = {
	.priv = NULL,
	.InitPort = PORT_InitPort
};

static McalCanDevice_t g_canDevice = {
	.baudrate = 500,
	.CanDevInit = FLEXCAN0_Init,
	.CanReadFunc = FLEXCAN0_ReceiveMsg,
	.CanWriteFunc = FLEXCAN0_TransmitMsg
};

static McalSysClockDevice_t g_sysClockDevice = {
	.priv = NULL,
	.InitSysClock = CLOCK_InitSysClock
};

static McalLpitDevice_t g_lpitDevice = {
	.priv = NULL,
	.InitLpit = LPIT0_Init,
	.Get1MsFlag = LPIT0_Get1MsFlag,
	.Clean1MsFlag = LPIT0_Clean1MsFlag
};

static McalFlsDevice_t g_flsDevice = {
	.priv = NULL,
	.FlsDevInit = FLASH_Init,
	.FlsReadFunc = FLASH_Read,
	.FlsWriteFunc = FLASH_Write,
	.FlsEraseFunc = FLASH_Erase
};

static McalLpuartDevice_t g_lpuartDevice = {
	.baudrate = 9600,
	.InitLpuart = LPUART1_Init
};

/**
 * @brief 板卡驱动设备注册并初始化
 * @param[in] 无
 * @return 无
 */
void COM_DeviceInit(void)
{
	IF_PortDevRegister(&g_portDevice);
	IF_SysClockDevRegister(&g_sysClockDevice);
	IF_LpitDevRegister(&g_lpitDevice);
	IF_FlsDevRegister(&g_flsDevice);
	IF_CanDevRegister(&g_canDevice);
	IF_LpuartDevRegister(&g_lpuartDevice);
}
