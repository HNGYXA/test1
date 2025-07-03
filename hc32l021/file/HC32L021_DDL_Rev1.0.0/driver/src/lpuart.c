/**
 *******************************************************************************
 * @file  lpuart.c
 * @brief This file provides firmware functions to manage the LPUART.
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "lpuart.h"

/**
 * @addtogroup HC32L021_DDL 驱动库
 * @{
 */

/**
 * @defgroup DDL_LPUART LPUART模块驱动库
 * @brief LPUART Driver Library LPUART模块驱动库
 * @{
 */
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/**
 * @defgroup LPUART_Local_Macros LPUART局部宏定义
 * @{
 */
#define LPUART_TIMEOUT (0xFFFFFFu) /* 超时保护计数值 */
/**
 * @}
 */
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @defgroup LPUART_Global_Functions LPUART全局函数定义
 * @{
 */

/**
 * @brief  LPUART初始化函数
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @param  [in] pstcLpuartInit LPUART初始化配置结构体 @ref stc_lpuart_init_t
 * @retval uint32_t
 *           - 最终波特率设定值
 */
uint32_t LPUART_Init(LPUART_TypeDef *LPUARTx, stc_lpuart_init_t *pstcLpuartInit)
{
    uint32_t  u32Bps  = 0;
    float32_t f32Scnt = 0;

    CLR_REG(LPUARTx->SCON);

    WRITE_REG(LPUARTx->SCON, pstcLpuartInit->u32StopBits | pstcLpuartInit->u32Parity | pstcLpuartInit->u32FrameLength | pstcLpuartInit->u32TransMode
                                 | pstcLpuartInit->u32HwControl | pstcLpuartInit->stcBaudRate.u32SclkSelect);

    MODIFY_REG(LPUARTx->SCON, LPUART_SCON_OVER, (0x2UL << LPUART_SCON_OVER_Pos));

    f32Scnt = (float32_t)(pstcLpuartInit->stcBaudRate.u32Sclk) / (float32_t)(pstcLpuartInit->stcBaudRate.u32Baud << 2);
    WRITE_REG(LPUARTx->SCNT, (uint16_t)(float32_t)(f32Scnt + 0.5f));

    u32Bps = (((pstcLpuartInit->stcBaudRate.u32Sclk) / ((uint16_t)(float32_t)(f32Scnt + 0.5f))) >> 2); /* 最终波特率设定值*/

    return u32Bps;
}

/**
 * @brief  LPUART XTL波特率产生寄存器
 * @note   当XTL时钟为32768时，调用此函数产生9600或者4800波特率
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @param  [in] u32BselConfig 波特率选择 @ref LPUART_XTL_32768_Baud_Select
 * @retval en_result_t
 *           - Ok: 配置成功
 *           - Error: 配置失败
 */
en_result_t LPUART_XtlBselSet(LPUART_TypeDef *LPUARTx, uint32_t u32BselConfig)
{
    en_result_t enRet = Ok;

    if (LPUART_SCLK_SEL_XTL == READ_REG32_BIT(LPUARTx->SCON, LPUART_SCON_SCLKSEL_Msk))
    {
        if (u32BselConfig == LPUART_XTL_BAUD_9600)
        {
            WRITE_REG32(LPUARTx->BSEL, LPUART_XTL_BAUD_9600);
            WRITE_REG32(LPUARTx->MODU, 0x54Au);
        }
        else if (u32BselConfig == LPUART_XTL_BAUD_4800)
        {
            WRITE_REG32(LPUARTx->BSEL, LPUART_XTL_BAUD_4800);
            WRITE_REG32(LPUARTx->MODU, 0xEFBu);
        }
        else
        {
            enRet = Error;
        }
    }
    else
    {
        enRet = Error;
    }

    return enRet;
}

/**
 * @brief  LPUART多机模式配置
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @param  [in] u8Addr    从机设备地址
 * @param  [in] u8AddrMsk 从机设备地址掩码
 * @retval None
 */
void LPUART_MultiModeConfig(LPUART_TypeDef *LPUARTx, uint8_t u8Addr, uint8_t u8AddrMsk)
{
    SET_REG_BIT(LPUARTx->SCON, LPUART_SCON_ADRDET);
    SET_REG_BIT(LPUARTx->SADDR, u8Addr);
    SET_REG_BIT(LPUARTx->SADEN, u8AddrMsk);
}

/**
 * @brief  LPUART单线半双工模式使能
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @retval None
 */
void LPUART_HdModeEnable(LPUART_TypeDef *LPUARTx)
{
    SET_REG32_BIT(LPUARTx->SCON, LPUART_SCON_HDSEL_Msk);
}

/**
 * @brief  LPUART单线半双工模式关闭
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @retval None
 */
void LPUART_HdModeDisable(LPUART_TypeDef *LPUARTx)
{
    CLR_REG32_BIT(LPUARTx->SCON, LPUART_SCON_HDSEL_Msk);
}

/**
 * @brief  LPUART发送或接收等功能使能设置
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @param  [in] u32Func 功能 @ref LPUART_Func_Enable
 * @retval None
 */
void LPUART_FuncEnable(LPUART_TypeDef *LPUARTx, uint32_t u32Func)
{
    SET_REG32_BIT(LPUARTx->SCON, u32Func);
}

/**
 * @brief  LPUART发送或接收等功能禁止设置
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @param  [in] u32Func 功能 @ref LPUART_Func_Enable
 * @retval None
 */
void LPUART_FuncDisable(LPUART_TypeDef *LPUARTx, uint32_t u32Func)
{
    CLR_REG32_BIT(LPUARTx->SCON, u32Func);
}

/**
 * @brief  LPUART通信中断使能函数设置
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @param  [in] u32IntSelect 发送or接收中断使能 @ref LPUART_Int_Select
 * @retval None
 */
void LPUART_IntEnable(LPUART_TypeDef *LPUARTx, uint32_t u32IntSelect)
{
    SET_REG32_BIT(LPUARTx->SCON, u32IntSelect);
}

/**
 * @brief  LPUART通信中断禁止函数设置
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @param  [in] u32IntSelect 发送or接收中断使能 @ref LPUART_Int_Select
 * @retval None
 */
void LPUART_IntDisable(LPUART_TypeDef *LPUARTx, uint32_t u32IntSelect)
{
    CLR_REG32_BIT(LPUARTx->SCON, u32IntSelect);
}

/**
 * @brief  LPUART总状态获取
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @retval None
 */
uint8_t LPUART_IntFlagGetAll(LPUART_TypeDef *LPUARTx)
{
    return (LPUARTx->ISR);
}

/**
 * @brief  LPUART状态全部清除
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @retval None
 */
void LPUART_IntFlagClearAll(LPUART_TypeDef *LPUARTx)
{
    LPUARTx->ICR = 0u;
}

/**
 * @brief  LPUART单个状态获取
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @param  [in] u32IntFlag 状态 @ref LPUART_Status_Select
 * @retval boolean_t
 *           - TRUE: 置位
 *           - FALSE: 未置位
 */
boolean_t LPUART_IntFlagGet(LPUART_TypeDef *LPUARTx, uint32_t u32IntFlag)
{
    return READ_REG32_BIT(LPUARTx->ISR, u32IntFlag) ? TRUE : FALSE;
}

/**
 * @brief  LPUART单个状态清除
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @param  [in] u32IntFlag 状态 @ref LPUART_Status_Select
 * @retval None
 */
void LPUART_IntFlagClear(LPUART_TypeDef *LPUARTx, uint32_t u32IntFlag)
{
    CLR_REG32_BIT(LPUARTx->ICR, u32IntFlag);
}

/**
 * @brief  LPUART发送数据函数，查询方式调用此函数，中断方式发送不适用
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @param  [in] u8Data 发送数据
 * @retval en_result_t
 *           - Ok: 配置成功
 *           - ErrorTimeout: 超时
 */
en_result_t LPUART_TransmitPoll(LPUART_TypeDef *LPUARTx, uint8_t *pu8Data, uint32_t u32Size)
{
    while (u32Size)
    {
        while (!READ_REG_BIT(LPUARTx->ISR, LPUART_ISR_TXE))
        {
            ;
        }
        WRITE_REG(LPUARTx->SBUF_f.DATA, *pu8Data);

        pu8Data++;
        u32Size--;
    }

    while (!READ_REG_BIT(LPUARTx->ISR, LPUART_ISR_TC))
    {
        ;
    }
    CLR_REG_BIT(LPUARTx->ICR, LPUART_FLAG_TC);

    return Ok;
}

/**
 * @brief  LPUART发送数据函数,查询方式调用此函数，中断方式发送不适用
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @param  [in] u8Data 发送数据
 * @param  [in] u32TimeOut 最长发送时间
 * @retval en_result_t
 *           - Ok: 配置成功
 *           - ErrorTimeout: 超时
 */
en_result_t LPUART_TransmitPollTimeOut(LPUART_TypeDef *LPUARTx, uint8_t *pu8Data, uint32_t u32Size, uint32_t u32TimeOut)
{
    while (u32Size)
    {
        while (!READ_REG_BIT(LPUARTx->ISR, LPUART_ISR_TXE))
        {
            if (0x0u == u32TimeOut--)
            {
                return ErrorTimeout;
            }
        }
        WRITE_REG(LPUARTx->SBUF_f.DATA, *pu8Data);

        pu8Data++;
        u32Size--;
    }

    while (!READ_REG_BIT(LPUARTx->ISR, LPUART_ISR_TC))
    {
        if (0x0u == u32TimeOut--)
        {
            return ErrorTimeout;
        }
    }
    CLR_REG_BIT(LPUARTx->ICR, LPUART_FLAG_TC);

    return Ok;
}

/**
 * @brief  LPUART发送数据函数，中断方式调用此函数
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @param  [in] u8Data 发送数据
 * @retval en_result_t
 *           - Ok: 发送完成
 *           - ErrorTimeout: 超时
 */
en_result_t LPUART_TransmitInt(LPUART_TypeDef *LPUARTx, uint8_t u8Data)
{
    while (FALSE == LPUART_IntFlagGet(LPUARTx, LPUART_FLAG_TXE))
    {
        ;
    }
    WRITE_REG(LPUARTx->SBUF_f.DATA, u8Data);
    return Ok;
}
/**
* @brief  LPUART查询接收数据函数
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @retval en_result_t
 *           - Ok: 接受完成
 *           - ErrorTimeout: 超时
 */
en_result_t LPUART_ReceivePoll(LPUART_TypeDef *LPUARTx, uint8_t *pu8Data, uint32_t u32Size)
{
    while (u32Size)
    {
        while (!READ_REG_BIT(LPUARTx->ISR, LPUART_ISR_RC))
        {
            ;
        }
        *pu8Data = READ_REG(LPUARTx->SBUF);
        CLR_REG_BIT(LPUARTx->ICR, LPUART_ISR_RC);
        pu8Data++;
        u32Size--;
    }

    return Ok;
}

/**
 * @brief  LPUART查询接收数据函数
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @retval en_result_t
 *           - Ok: 接收完成
 *           - ErrorTimeout: 超时
 */
en_result_t LPUART_ReceivePollTimeOut(LPUART_TypeDef *LPUARTx, uint8_t *pu8Data, uint32_t u32Size, uint32_t u32TimeOut)
{
    while (u32Size)
    {
        while (!READ_REG_BIT(LPUARTx->ISR, LPUART_ISR_RC))
        {
            if (0x0u == u32TimeOut--)
            {
                return ErrorTimeout;
            }
        }

        *pu8Data = READ_REG(LPUARTx->SBUF);
        CLR_REG_BIT(LPUARTx->ICR, LPUART_ISR_RC);

        pu8Data++;
        u32Size--;
    }

    return Ok;
}

/**
 * @brief  LPUART中断接收数据函数
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @retval uint8_t 接收数据
 */
uint8_t LPUART_ReceiveInt(LPUART_TypeDef *LPUARTx)
{
    return (uint8_t)(READ_REG32_BIT(LPUARTx->SBUF, LPUART_SBUF_DATA_Msk));
}

/**
 * @brief  LPUART DATA8置位
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @retval None
 */
void LPUART_Bit8Set(LPUART_TypeDef *LPUARTx)
{
    SET_REG_BIT(LPUARTx->SBUF, LPUART_SBUF_DATA8_Msk);
}

/**
 * @brief  LPUART DATA8清零
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @retval None
 */
void LPUART_Bit8Clear(LPUART_TypeDef *LPUARTx)
{
    CLR_REG_BIT(LPUARTx->SBUF, LPUART_SBUF_DATA8_Msk);
}

/**
 * @brief  获取RB8数值
 * @param  [in] LPUARTx LPUARTx通道 @ref LPUART_TypeDef
 * @retval boolean_t
 *           - TRUE: 置位
 *           - FALSE: 未置位
 */
boolean_t LPUART_Bit8Get(LPUART_TypeDef *LPUARTx)
{
    return READ_REG32_BIT(LPUARTx->SBUF, LPUART_SBUF_DATA8_Msk) ? TRUE : FALSE;
}

/**
 * @brief  结构体初始化
 * @param  [in] pstcInit 初始化结构体 @ref stc_lpuart_init_t
 * @retval None
 */
void LPUART_StcInit(stc_lpuart_init_t *pstcInit)
{
    pstcInit->u32TransMode              = LPUART_MODE_TX;
    pstcInit->u32Parity                 = LPUART_B8_MULTI_DataOrAddr;
    pstcInit->u32StopBits               = LPUART_STOPBITS_1;
    pstcInit->u32FrameLength            = LPUART_FRAME_LEN_8B_NOPAR;
    pstcInit->u32HwControl              = LPUART_HWCONTROL_NONE;
    pstcInit->stcBaudRate.u32SclkSelect = LPUART_SCLK_SEL_PCLK;
    pstcInit->stcBaudRate.u32Sclk       = SYSCTRL_HclkFreqGet();
    pstcInit->stcBaudRate.u32Baud       = 0;
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
