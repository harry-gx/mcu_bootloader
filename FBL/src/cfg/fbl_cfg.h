#ifndef BOOT_CFG_H
#define BOOT_CFG_H

/*
 * 文件名称: boot_cfg.h
 * 文件说明: Boot 模块集中配置文件（地址、偏移、错误码、默认策略）
 * 设计原则: 与平台和协议强相关的常量统一在此管理，便于维护与审计。
 */

#include "std_type.h"

/* ==================== 通用标记定义 ==================== */
#define BOOT_RECOVERY_NO_VALUE                (0u)
#define BOOT_RECOVERY_YES_VALUE               (1u)

#define BOOT_FLAG_CLEAR_VALUE                 (0u)
#define BOOT_FLAG_SET_VALUE                   (1u)

#define BOOT_APP_INVALID_VALUE                (0u)
#define BOOT_APP_VALID_VALUE                  (1u)

/* ==================== Boot 行为配置 ==================== */
#define BOOT_WAIT_TIMEOUT_TICK_DEFAULT        (6000u)
#define BOOT_WRITE_BLOCK_SIZE	              (512)

/* ==================== 错误码配置 ==================== */
#define BOOT_ERROR_NONE                       (0u)
#define BOOT_ERROR_ACTION_NOT_FOUND           (0xE100u)
#define BOOT_ERROR_TRANSITION_NOT_FOUND       (0xE101u)
#define BOOT_ERROR_PROGRAM_FAILED             (0xE001u)
#define BOOT_ERROR_VERIFY_FAILED              (0xE002u)
#define BOOT_ERROR_JUMP_RETURNED              (0xE003u)
#define BOOT_ERROR_CLEAR_FLAG_FAILED          (0xE004u)

/* ==================== 存储映射与头部配置 ==================== */
#define BOOT_UPDATE_FLAG_ADDR                 (0x10000000u)
#define BOOT_UPDATE_FLAG_VALUE                (0x55AAu)
#define BOOT_UPDATE_FLAG_ERASE_SIZE           (2048u)

#define BOOT_APP_START_ADDR                   (0x00014000u)
#define BOOT_APP_END_ADDR                     (0x00080000u)
#define BOOT_APP_HEADER_SIZE                  (0x60u)
#define BOOT_APP_MAGIC                        (0xA55AA55Au)

/*
 * 结构体名称: BootAppHeader
 * 功能说明: APP 固件头结构定义
 */
typedef struct __BootAppHeader
{
    uint32_bl magic;         /* 固件魔术字 */
    uint32_bl length;        /* 固件有效载荷长度 */
    uint32_bl crc32;         /* 固件有效载荷 CRC32 */
    uint32_bl reserved0;     /* 预留字段 */

    uint8_bl type[16];       /* 固件类型 */
    uint8_bl board[16];      /* 板卡型号 */
    uint8_bl version[16];    /* 版本字符串 */
    uint8_bl timestamp[16];  /* 时间戳字符串 */

    uint32_bl reserved1;     /* 预留字段 */
    uint32_bl reserved2;     /* 预留字段 */
    uint32_bl reserved3;     /* 预留字段 */
    uint32_bl reserved4;     /* 预留字段 */
} BootAppHeader;

#endif /* BOOT_CFG_H */
