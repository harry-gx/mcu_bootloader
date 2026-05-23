/******************************************************************************
 * @file    std_type.h
 * @brief   一些常用的类型定义头文件
 * @author  Xin.Gao
 * @date    2026-05-13
 *****************************************************************************/

#ifndef STD_TYPE_H
#define STD_TYPE_H

#ifndef TRUE
#define	TRUE (1U)
#endif

#ifndef FALSE
#define FALSE (0U)
#endif

#ifndef NULL
#define NULL ((void *) 0)
#endif

typedef unsigned char          bool_bl;
typedef float			       float_bl;
typedef double			       double_bl;
typedef char			       int8_bl;
typedef short int		       int16_bl;
typedef int				       int32_bl;
typedef long long int	       int64_bl;
typedef unsigned char          uint8_bl;
typedef unsigned short int     uint16_bl;
typedef unsigned int           uint32_bl;
typedef unsigned long long int uint64_bl;

typedef enum
{
    FBL_NOT_RECV = 1,  /* 未接收到数据 */
    FBL_OK       = 0,  /* 成功 */
    FBL_ERR      = -1, /* 错误 */
    FBL_ERR_PTR  = -2, /* 输入错误的指针值 */
} FblRetCode_t;

#endif /* STD_TYPE_H */
