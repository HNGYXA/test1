/**
 *******************************************************************************
 * @file  base_types.h
 * @brief This file contains all the types definition for the driver library
 @verbatim
   Change Logs:
   Date             Author          Notes
   2024-09-18       MADS            First version
 @endverbatim
 *******************************************************************************
 * Copyright (C) 2024, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 *******************************************************************************
 */

#ifndef __BASE_TYPES_H__
#define __BASE_TYPES_H__

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/**
 * @addtogroup HC32L021_DDL 驱动库
 * @{
 */

/**
 * @addtogroup DDL_BASE_TYPES BASETYPE驱动库
 * @{
 */
/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/
/**
 * @defgroup BASE_TPYE_Global_Types BASETYPE全局类型定义
 * @{
 */

/**
 * @brief  logical datatype (only values are TRUE and FALSE)
 */
typedef uint8_t boolean_t;

/**
 * @brief  single precision floating point number (4 byte)
 */
typedef float float32_t;

/**
 * @brief  double precision floating point number (8 byte)
 */
typedef double float64_t;

/**
 * @brief  ASCII character for string generation (8 bit)
 */
typedef char char_t;

/**
 * @brief  function pointer type to void/void function
 */
typedef void (*func_ptr_t)(void);

/**
 * @brief  function pointer type to void/uint8_t function
 */
typedef void (*func_ptr_arg1_t)(uint8_t u8Param);

/**
 * @brief  generic error codes
 */
typedef enum
{
    Ok                       = 0u,  /* No error */
    Error                    = 1u,  /* Non-specific error code */
    ErrorAddressAlignment    = 2u,  /* Address alignment does not match */
    ErrorAccessRights        = 3u,  /* Wrong mode (e.g. user/system) mode is set */
    ErrorInvalidParameter    = 4u,  /* Provided parameter is not valid */
    ErrorOperationInProgress = 5u,  /* A conflicting or requested operation is still in progress */
    ErrorInvalidMode         = 6u,  /* Operation not allowed in current mode */
    ErrorUninitialized       = 7u,  /* Module (or part of it) was not initialized properly */
    ErrorBufferFull          = 8u,  /* Circular buffer can not be written because the buffer is full */
    ErrorTimeout             = 9u,  /* Time Out error occurred (e.g. I2C arbitration lost, Flash time-out, etc.) */
    ErrorNotReady            = 10u, /* A requested final state is not reached */
    OperationInProgress      = 11u  /* Indicator for operation in progress */
} en_result_t;
/**
 * @}
 */
/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup BASETYPE_Global_Macros BASETYPE全局宏定义
 * @{
 */

#ifndef TRUE
/** Value is true (boolean_t type) */
#define TRUE ((boolean_t)1u)
#endif

#ifndef FALSE
/** Value is false (boolean_t type) */
#define FALSE ((boolean_t)0u)
#endif

/** Returns the minimum value out of two values */
#define MINIMUM(X, Y) ((X) < (Y) ? (X) : (Y))

/** Returns the maximum value out of two values */
#define MAXIMUM(X, Y) ((X) > (Y) ? (X) : (Y))

/** Returns the dimension of an array */
#define ARRAY_SZ(X) (sizeof(X) / sizeof((X)[0]))

#ifdef __DEBUG_ASSERT
#define ASSERT(x) \
    do \
    { \
        assert((x) > 0u); \
    } while (0);
#else
#define ASSERT(x) \
    { \
    }
#endif
/**
 * @}
 */
/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/
/*******************************************************************************
  Global function prototypes (definition in C source)
 ******************************************************************************/
/**
 * @}
 */

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif /* __BASE_TYPES_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
