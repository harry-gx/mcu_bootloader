/******************************************************************************
 * @file    main.c
 * @brief   Bootloader 主入口
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#include <stdio.h>
#include "com.h"
#include "bsm_port.h"
#include "bsm.h"
#include "uds_port.h"

int main(void)
{
    static boot_context_t boot_ctx;

    /* 板卡驱动设备注册并初始化 */
    COM_DeviceInit();

    /* UDS初始化 */
    UDS_Init();

    /* 初始化 Boot 状态机上下文 */
    BSM_Init(&boot_ctx);

    printf("\033[2J\033[H");
    printf("---[TEST][main]---\r\n");

    for (;;)
    {
    	/* 执行一次主循环计数 */
    	UDS_MainLoopCnt();

        /* 高频轮询通信接收 */
    	UDS_RecvFrame();

        /* 1ms 周期任务(UDS服务) */
        if (TRUE == BSM_Take1msTick())
        {
        	/* 执行UDS周期任务 */
        	UDS_1MsTask();

            /* 执行一次状态机步进 */
        	BSM_RunStep(&boot_ctx);
        }
    }
}
