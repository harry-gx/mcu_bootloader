/******************************************************************************
 * @file    flash.c
 * @brief   flash驱动
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#include "flash.h"

static flash_ssd_config_t flashSSDConfig;
static const flash_user_config_t Flash_InitConfig = {
    .PFlashBase  = 0x00000000U, /* Base address of Program Flash block */
    .PFlashSize  = 0x00080000U, /* Size of Program Flash block */
    .DFlashBase  = 0x10000000U, /* Base address of Data Flash block */
    .EERAMBase   = 0x14000000U, /* Base address of FlexRAM block */
    .CallBack    = NULL_CALLBACK
};

/**
 * @brief flash初始化
 *
 * @param[in] 无
 *
 * @return 无
 */
int32_bl FLASH_Init(McalFlsDevice_t *dev)
{
	if (NULL == dev)
	{
		return FBL_ERR_PTR;
	}

	status_t ret = STATUS_SUCCESS;
	ret = FLASH_DRV_Init(&Flash_InitConfig, &flashSSDConfig);
	if (STATUS_SUCCESS != ret)
	{
		return FBL_ERR;
	}

    return FBL_OK;
}

/**
 * @brief 从flash指定地址读数据
 *
 * @param[in] startAddr 起始地址
 * @param[out] size 数据大小
 * @param[out] pData 数据
 *
 * @return 错误状态
 */
int32_bl FLASH_Read(uint32_bl startAddr, uint32_bl size, uint8_bl *pData)
{
	if (NULL == pData)
	{
		return FBL_ERR_PTR;
	}

	/* Flash地址转指针 */
	const uint8_bl *pFlash = (const uint8_bl *)startAddr;

    uint32_bl i;
    for (i = 0; i < size; i++)
    {
    	pData[i] = pFlash[i];
    }

    return FBL_OK;
}

/**
 * @brief 向flash指定地址写数据
 *
 * @param[in] startAddr 起始地址
 * @param[in] size 数据大小
 * @param[in] pData 数据
 *
 * @return 错误状态
 */
int32_bl FLASH_Write(uint32_bl startAddr, uint32_bl size, const uint8_bl *pData)
{
	if (NULL == pData)
	{
		return FBL_ERR_PTR;
	}

	status_t ret = STATUS_SUCCESS;
    ret = FLASH_DRV_Program(&flashSSDConfig, startAddr, size, pData);
	if (STATUS_SUCCESS != ret)
	{
		return FBL_ERR;
	}

    return FBL_OK;
}

/**
 * @brief 从flash指定地址擦除
 *
 * @param[in] startAddr 起始地址
 * @param[in] size 擦除大小
 *
 * @return 错误状态
 */
int32_bl FLASH_Erase(uint32_bl startAddr, uint32_bl size)
{
	status_t ret = STATUS_SUCCESS;
    ret = FLASH_DRV_EraseSector(&flashSSDConfig, startAddr, size);
	if (STATUS_SUCCESS != ret)
	{
		return FBL_ERR;
	}

    return FBL_OK;
}
