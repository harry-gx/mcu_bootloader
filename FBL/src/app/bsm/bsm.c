/*
 * 文件名称: boot_sm.c
 * 文件说明: Boot 状态机核心实现（动作表 + 转移表）
 */

#include "bsm.h"
#include "bsm_cfg.h"
#include "bsm_port.h"
#include "uds_port.h"

/*
 * 函数名称: BootSm_WaitTimeout
 * 功能说明: 判断 WAIT_UPDATE 是否超时
 */
static uint32_bl BootSm_WaitTimeout(const boot_context_t *ctx)
{
    return (ctx->wait_tick >= ctx->wait_timeout_tick) ? BOOT_FLAG_SET_VALUE : BOOT_FLAG_CLEAR_VALUE;
}

static boot_event_t BootSm_ActionCheckRecovery(boot_context_t *ctx)
{
	uint8_bl ret = UDS_GetRecoveryFlag();

	ctx->wait_tick++;
    if ((BOOT_RECOVERY_NO_VALUE == ret) &&
    	(BootSm_WaitTimeout(ctx) == BOOT_FLAG_SET_VALUE))
    {
        return BOOT_EVT_WAIT_TIMEOUT;
    }
    if (BOOT_RECOVERY_YES_VALUE == ret)
    {
    	return BOOT_EVT_RECOVERY_YES;
    }

    return BOOT_EVT_NONE;
}

static boot_event_t BootSm_ActionCheckFlag(boot_context_t *ctx)
{
    ctx->update_flag = BSM_ReadUpdateFlag();
    return (ctx->update_flag == BOOT_FLAG_SET_VALUE) ? BOOT_EVT_FLAG_SET : BOOT_EVT_FLAG_CLEAR;
}

static boot_event_t BootSm_ActionCheckApp(boot_context_t *ctx)
{
    if (BSM_AppExist() == BOOT_APP_INVALID_VALUE)
    {
        return BOOT_EVT_APP_NOT_EXIST;
    }

    if (BSM_AppValid() == BOOT_APP_INVALID_VALUE)
    {
        return BOOT_EVT_APP_INVALID;
    }

    ctx->app_valid = BOOT_APP_VALID_VALUE;
    return BOOT_EVT_APP_VALID;
}

static boot_event_t BootSm_ActionWaitUpdate(boot_context_t *ctx)
{
    if (BSM_PollUpdateReq() == BSM_UDS_UPDATE_REQUEST)
    {
        return BOOT_EVT_UPDATE_REQUEST;
    }

    return BOOT_EVT_NONE;
}

static boot_event_t BootSm_ActionProgramming(boot_context_t *ctx)
{
	BsmUdsRetE ret;

    ret = BSM_ProgramProcess();
    if (ret == BSM_UDS_PROGRAM_DONE)
    {
        return BOOT_EVT_PROGRAM_DONE;
    }

    if (ret == BSM_UDS_PROGRAM_FAIL)
    {
        ctx->error_code = BOOT_ERROR_PROGRAM_FAILED;
        return BOOT_EVT_PROGRAM_FAIL;
    }

    return BOOT_EVT_NONE;
}

static boot_event_t BootSm_ActionVerifyApp(boot_context_t *ctx)
{
    if (BSM_AppValid() != BOOT_APP_INVALID_VALUE)
    {
        if (BSM_ClearUpdateFlag() != BOOT_FLAG_CLEAR_VALUE)
        {
            ctx->error_code = BOOT_ERROR_CLEAR_FLAG_FAILED;
        }
        ctx->app_valid = BOOT_APP_VALID_VALUE;
        return BOOT_EVT_VERIFY_OK;
    }

    ctx->error_code = BOOT_ERROR_VERIFY_FAILED;
    return BOOT_EVT_VERIFY_FAIL;
}

static boot_event_t BootSm_ActionJumpApp(boot_context_t *ctx)
{
	BSM_JumpToApp();
    ctx->error_code = BOOT_ERROR_JUMP_RETURNED;
    return BOOT_EVT_JUMP_FAIL;
}

static boot_event_t BootSm_ActionError(boot_context_t *ctx)
{
    (void)ctx;
    if (BSM_PollUpdateReq() == BSM_UDS_UPDATE_REQUEST)
    {
        return BOOT_EVT_UPDATE_REQUEST;
    }

    return BOOT_EVT_ERROR;
}

static const boot_state_action_entry_t g_action_table[] =
{
	{ BOOT_STATE_CHECK_RECOVERY, BootSm_ActionCheckRecovery },
    { BOOT_STATE_CHECK_FLAG,     BootSm_ActionCheckFlag },
    { BOOT_STATE_CHECK_APP,      BootSm_ActionCheckApp },
    { BOOT_STATE_WAIT_UPDATE,    BootSm_ActionWaitUpdate },
    { BOOT_STATE_PROGRAMMING,    BootSm_ActionProgramming },
    { BOOT_STATE_VERIFY_APP,     BootSm_ActionVerifyApp },
    { BOOT_STATE_JUMP_APP,       BootSm_ActionJumpApp },
    { BOOT_STATE_ERROR,          BootSm_ActionError }
};
#define BOOT_ACTION_TABLE_SIZE ((uint32_bl)(sizeof(g_action_table) / sizeof(g_action_table[0])))

static const boot_transition_t g_transition_table[] =
{
	{ BOOT_STATE_CHECK_RECOVERY,  BOOT_EVT_RECOVERY_YES,   BOOT_STATE_PROGRAMMING },
	{ BOOT_STATE_CHECK_RECOVERY,  BOOT_EVT_WAIT_TIMEOUT,   BOOT_STATE_CHECK_FLAG },

    { BOOT_STATE_CHECK_FLAG,      BOOT_EVT_FLAG_SET,       BOOT_STATE_WAIT_UPDATE },
    { BOOT_STATE_CHECK_FLAG,      BOOT_EVT_FLAG_CLEAR,     BOOT_STATE_CHECK_APP },

    { BOOT_STATE_CHECK_APP,       BOOT_EVT_APP_VALID,      BOOT_STATE_JUMP_APP },
    { BOOT_STATE_CHECK_APP,       BOOT_EVT_APP_INVALID,    BOOT_STATE_WAIT_UPDATE },
    { BOOT_STATE_CHECK_APP,       BOOT_EVT_APP_NOT_EXIST,  BOOT_STATE_WAIT_UPDATE },

    { BOOT_STATE_WAIT_UPDATE,     BOOT_EVT_UPDATE_REQUEST, BOOT_STATE_PROGRAMMING },

    { BOOT_STATE_PROGRAMMING,     BOOT_EVT_PROGRAM_DONE,   BOOT_STATE_VERIFY_APP },
    { BOOT_STATE_PROGRAMMING,     BOOT_EVT_PROGRAM_FAIL,   BOOT_STATE_WAIT_UPDATE },

    { BOOT_STATE_VERIFY_APP,      BOOT_EVT_VERIFY_OK,      BOOT_STATE_JUMP_APP },
    { BOOT_STATE_VERIFY_APP,      BOOT_EVT_VERIFY_FAIL,    BOOT_STATE_WAIT_UPDATE },

    { BOOT_STATE_JUMP_APP,        BOOT_EVT_JUMP_FAIL,      BOOT_STATE_WAIT_UPDATE },

    { BOOT_STATE_ERROR,           BOOT_EVT_UPDATE_REQUEST, BOOT_STATE_PROGRAMMING },
    { BOOT_STATE_ERROR,           BOOT_EVT_ERROR,          BOOT_STATE_ERROR }
};
#define BOOT_TRANSITION_TABLE_SIZE ((uint32_bl)(sizeof(g_transition_table) / sizeof(g_transition_table[0])))

static boot_state_action_t BootSm_FindAction(boot_state_t state)
{
    uint32_bl i;
    for (i = 0u; i < BOOT_ACTION_TABLE_SIZE; i++)
    {
        if (g_action_table[i].state == state)
        {
            return g_action_table[i].action;
        }
    }
    return (boot_state_action_t)0;
}

static bool_bl BootSm_FindNextState(boot_state_t state, boot_event_t event, boot_state_t *next)
{
    uint32_bl i;
    for (i = 0u; i < BOOT_TRANSITION_TABLE_SIZE; i++)
    {
        if ((g_transition_table[i].current_state == state) &&
            (g_transition_table[i].event == event))
        {
            *next = g_transition_table[i].next_state;
            return TRUE;
        }
    }
    return FALSE;
}

/*
 * 函数名称: BSM_Init
 * 功能说明: 初始化状态机上下文
 * 输入参数: ctx - 状态机上下文指针
 * 输出参数: ctx - 初始化后的上下文
 * 返回值: 无
 */
void BSM_Init(boot_context_t *ctx)
{
    if (ctx == (boot_context_t *)0)
    {
        return;
    }

    ctx->state = BOOT_STATE_CHECK_RECOVERY;
    ctx->error_code = BOOT_ERROR_NONE;
    ctx->update_flag = BOOT_FLAG_CLEAR_VALUE;
    ctx->app_valid = BOOT_APP_INVALID_VALUE;
    ctx->wait_tick = 0u;
    ctx->wait_timeout_tick = BOOT_WAIT_TIMEOUT_TICK_DEFAULT;
}

/*
 * 函数名称: BSM_RunStep
 * 功能说明: 执行一次状态机步进
 * 输入参数: ctx - 状态机上下文指针
 * 输出参数: ctx - 可能被更新的上下文
 * 返回值: 无
 */
void BSM_RunStep(boot_context_t *ctx)
{
    boot_state_action_t action;
    boot_event_t event;
    boot_state_t next_state;

    if (ctx == (boot_context_t *)0)
    {
        return;
    }

    action = BootSm_FindAction(ctx->state);
    if (action == (boot_state_action_t)0)
    {
        ctx->error_code = BOOT_ERROR_ACTION_NOT_FOUND;
        ctx->state = BOOT_STATE_ERROR;
        return;
    }

    event = action(ctx);
    if (event == BOOT_EVT_NONE)
    {
        return;
    }

    if (BootSm_FindNextState(ctx->state, event, &next_state))
    {
        ctx->state = next_state;
        return;
    }

    ctx->error_code = BOOT_ERROR_TRANSITION_NOT_FOUND;
    ctx->state = BOOT_STATE_ERROR;
}
