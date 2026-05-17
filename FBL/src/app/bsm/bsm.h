#ifndef BOOT_SM_H
#define BOOT_SM_H

/*
 * 文件名称: boot_sm.h
 * 文件说明: Boot 状态机模块对外接口与数据结构定义
 */

#include "std_type.h"

/*
 * 枚举名称: boot_mode_t
 * 功能说明: Boot 运行模式（语义视图）
 */
typedef enum
{
    BOOT_MODE_NORMAL = 0,
    BOOT_MODE_UPDATE,
    BOOT_MODE_RECOVERY,
    BOOT_MODE_ERROR
} boot_mode_t;

/*
 * 枚举名称: boot_state_t
 * 功能说明: Boot 状态机主状态
 */
typedef enum
{
    BOOT_STATE_CHECK_FLAG = 0,
    BOOT_STATE_CHECK_APP,
    BOOT_STATE_WAIT_UPDATE,
    BOOT_STATE_PROGRAMMING,
    BOOT_STATE_VERIFY_APP,
    BOOT_STATE_JUMP_APP,
    BOOT_STATE_ERROR
} boot_state_t;

/*
 * 枚举名称: boot_event_t
 * 功能说明: 状态机事件
 */
typedef enum
{
    BOOT_EVT_NONE = 0,
    BOOT_EVT_FLAG_SET,
    BOOT_EVT_FLAG_CLEAR,
    BOOT_EVT_APP_VALID,
    BOOT_EVT_APP_INVALID,
    BOOT_EVT_APP_NOT_EXIST,
    BOOT_EVT_UPDATE_REQUEST,
    BOOT_EVT_WAIT_TIMEOUT,
    BOOT_EVT_PROGRAM_DONE,
    BOOT_EVT_PROGRAM_FAIL,
    BOOT_EVT_VERIFY_OK,
    BOOT_EVT_VERIFY_FAIL,
    BOOT_EVT_JUMP_FAIL,
    BOOT_EVT_ERROR
} boot_event_t;

/*
 * 结构体名称: boot_context_t
 * 功能说明: Boot 状态机上下文
 */
typedef struct
{
    boot_state_t state;           /* 当前状态 */
    boot_mode_t mode;             /* 当前模式（由状态派生） */
    uint32_bl error_code;          /* 当前错误码 */

    uint32_bl update_flag;         /* 升级标志缓存 */
    uint32_bl app_valid;           /* APP 有效标记 */

    uint32_bl wait_tick;           /* 等待计数 */
    uint32_bl wait_timeout_tick;   /* 等待超时阈值 */
} boot_context_t;

void BootSm_Init(boot_context_t *ctx);
void BootSm_RunStep(boot_context_t *ctx);

#endif
