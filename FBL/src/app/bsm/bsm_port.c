/*
 * 文件名称: boot_port.c
 * 文件说明: Boot 平台适配层实现
 * 职责说明:
 * 1. 负责读取升级标志、APP 校验、APP 跳转等平台行为。
 * 2. 负责与 HAL/UDS 交互，向状态机提供统一接口。
 */

#include "bsm_port.h"
#include "fbl_cfg.h"
#include "uds_port.h"
#include "device_registers.h"
#include "lpit_if.h"
#include "fls_if.h"
#include "crc32.h"

/*
 * 结构体名称: BootUpdateFlagU
 * 功能说明: 升级标志读取联合体（按 D-Flash 存储格式读取）
 */
typedef union __BootUpdateFlagU
{
    uint16_bl updateFlag;     /* 16位升级标志值 */
    uint8_bl updateFlagArr[8];/* 保留字节视图 */
} BootUpdateFlagU;

/*
 * 函数名称: GetAppHeader
 * 功能说明: 获取 APP 头部指针
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: APP 头部常量指针
 */
static const BootAppHeader *GetAppHeader(void)
{
    return (const BootAppHeader *)BOOT_APP_START_ADDR;
}

/*
 * 函数名称: BSM_Take1msTick
 * 功能说明: 读取并清除 LPIT 1ms 标志
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: TRUE=有tick, FALSE=无tick
 */
bool_bl BSM_Take1msTick(void)
{
    if (TRUE == IF_LpitGet1MsFlag())
    {
    	IF_LpitClean1MsFlag();
        return TRUE;
    }

    return FALSE;
}

/*
 * 函数名称: BSM_ReadUpdateFlag
 * 功能说明: 读取升级标志
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: 1=已置位，0=未置位
 */
uint8_bl BSM_ReadUpdateFlag(void)
{
    const BootUpdateFlagU *p = (const BootUpdateFlagU *)BOOT_UPDATE_FLAG_ADDR;
    return (p->updateFlag == BOOT_UPDATE_FLAG_VALUE) ? BOOT_FLAG_SET_VALUE : BOOT_FLAG_CLEAR_VALUE;
}

/*
 * 函数名称: BSM_ClearUpdateFlag
 * 功能说明: 清除升级标志
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: 清除后标志状态（1=仍置位，0=已清除）
 */
uint8_bl BSM_ClearUpdateFlag(void)
{
    BootUpdateFlagU clearFlag = {0};
    int32_bl writeOk;
    int32_bl eraseOk;
    uint8_bl flagNow;

    eraseOk = IF_FlsErase(BOOT_UPDATE_FLAG_ADDR, BOOT_UPDATE_FLAG_ERASE_SIZE);
    if (FBL_OK != eraseOk)
    {
        return BSM_ReadUpdateFlag();
    }

    writeOk = IF_FlsWrite(BOOT_UPDATE_FLAG_ADDR,
    					 (uint32_bl)sizeof(clearFlag.updateFlagArr),
						  clearFlag.updateFlagArr);
    flagNow = BSM_ReadUpdateFlag();

    if ((FBL_OK != writeOk) && (BOOT_FLAG_SET_VALUE == flagNow))
    {
        return BOOT_FLAG_SET_VALUE;
    }

    return flagNow;
}

/*
 * 函数名称: BSM_AppExist
 * 功能说明: 判断 APP 是否存在（通过 magic）
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: 1=存在，0=不存在
 */
uint8_bl BSM_AppExist(void)
{
    const BootAppHeader *hdr = GetAppHeader();
    return (hdr->magic == BOOT_APP_MAGIC) ? BOOT_APP_VALID_VALUE : BOOT_APP_INVALID_VALUE;
}

/*
 * 函数名称: BSM_AppValid
 * 功能说明: 判断 APP 是否有效（magic/长度边界/CRC）
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: 1=有效，0=无效
 */
uint8_bl BSM_AppValid(void)
{
    const BootAppHeader *hdr = GetAppHeader();
    uint32_bl imageEnd;

    if (BSM_AppExist() == BOOT_APP_INVALID_VALUE)
    {
        return BOOT_APP_INVALID_VALUE;
    }

    if ((hdr->length == 0xFFFFFFFFu) || (hdr->crc32 == 0xFFFFFFFFu) || (hdr->length == 0u))
    {
        return BOOT_APP_INVALID_VALUE;
    }

    if (BOOT_APP_START_ADDR > (0xFFFFFFFFu - hdr->length))
    {
        return BOOT_APP_INVALID_VALUE;
    }

    imageEnd = BOOT_APP_START_ADDR + hdr->length;
    if (imageEnd > BOOT_APP_END_ADDR)
    {
        return BOOT_APP_INVALID_VALUE;
    }

    uint32_bl crc = CRC_CalcImageCrc(0xFFFFFFFFu,
								    (const uint8_bl *)BOOT_APP_START_ADDR,
								    hdr->length);
    if (crc == hdr->crc32)
    {
    	return BOOT_APP_VALID_VALUE;
    }

    return BOOT_APP_INVALID_VALUE;
}

/*
 * 函数名称: BSM_PollUpdateReq
 * 功能说明: 轮询是否收到升级请求
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: UDS 状态码
 */
BsmUdsRetE BSM_PollUpdateReq(void)
{
	if (1 == GetUpdateRequestFlag())
	{
		SetUpdateRequestFlag(0);
		return BSM_UDS_UPDATE_REQUEST;
	}

    return BSM_UDS_IDLE;
}

/*
 * 函数名称: BSM_ProgramProcess
 * 功能说明: 执行升级过程处理
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: UDS 状态码
 */
BsmUdsRetE BSM_ProgramProcess(void)
{
    uint8_bl result = GetUdsProgramResult();

    if (result == UDS_PROGRAM_RESULT_DONE)
    {
    	SetUdsProgramResult(UDS_PROGRAM_RESULT_IDLE);
        return BSM_UDS_PROGRAM_DONE;
    }

    if (result == UDS_PROGRAM_RESULT_FAIL)
    {
    	SetUdsProgramResult(UDS_PROGRAM_RESULT_IDLE);
        return BSM_UDS_PROGRAM_FAIL;
    }

    return BSM_UDS_IDLE;
}

/*
 * 函数名称: BSM_JumpToApp
 * 功能说明: 标准 Cortex-M 跳转 APP
 * 输入参数: 无
 * 输出参数: 无
 * 返回值: 无（成功通常不返回）
 */
void BSM_JumpToApp(void)
{
    typedef void (*app_entry_t)(void);

    /* 跳过app固件头 */
    uint32_bl appVectorBase = BOOT_APP_START_ADDR + BOOT_APP_HEADER_SIZE;
    uint32_bl appMsp = *(uint32_bl *)(appVectorBase + 0u);
    uint32_bl appReset = *(uint32_bl *)(appVectorBase + 4u);

    if ((appMsp == 0xFFFFFFFFu) || (appReset == 0xFFFFFFFFu))
    {
        return;
    }

    /* 1. 关中断 */
    __asm volatile ("cpsid i" ::: "memory");

    /* 2. 关闭 + 清除 NVIC */
    uint8_bl i;
    for (i = 0u; i < S32_NVIC_ICER_COUNT; i++)
    {
        S32_NVIC->ICER[i] = 0xFFFFFFFFu;
        S32_NVIC->ICPR[i] = 0xFFFFFFFFu;
    }

    /* 3. 设置中断向量表 */
    S32_SCB->VTOR = appVectorBase;

    /* 4. 设置 MSP */
    __asm volatile ("msr msp, %0" :: "r"(appMsp) : );

    /* 5. 跳转 */
    app_entry_t JumpToApplication = (app_entry_t)appReset;
    JumpToApplication();
}
