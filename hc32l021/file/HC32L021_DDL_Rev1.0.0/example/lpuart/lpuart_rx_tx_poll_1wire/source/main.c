/**
 *******************************************************************************
 * @file  main.c
 * @brief This file provides example of LPUART
 @verbatim
   Change Logs:
   Date             Author          Notes
   2024-12-02       MADS            First version
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
#include "ddl.h"
#include "gpio.h"
#include "lpm.h"
#include "lpuart.h"
#include "sysctrl.h"
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define RX_TX_FRAME_LEN 10u /* 通信帧长度 */
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void GpioConfig(void);
static void LpUartConfig(void);
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
uint8_t u8RxData[RX_TX_FRAME_LEN] = {0};
/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  Main function
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* 端口配置 */
    GpioConfig();

    /* LPUART配置 */
    LpUartConfig();

    while (1)
    {
        /* 处理错误请求 */
        if (LPUART_IntFlagGet(LPUART1, LPUART_FLAG_PE))
        {
            LPUART_IntFlagClear(LPUART1, LPUART_FLAG_PE); /* 清除奇偶校验错误标记 */
        }
        else if (LPUART_IntFlagGet(LPUART1, LPUART_FLAG_FE))
        {
            LPUART_IntFlagClear(LPUART1, LPUART_FLAG_FE); /* 清除帧错误标记 */
        }
        else
        {
            ;
        }

        if (Ok == (LPUART_ReceivePollTimeOut(LPUART1, u8RxData, RX_TX_FRAME_LEN, 0xFFFFF))) /* 从PC接收RX_TX_FRAME_LEN长度数据完成 */
        {
            LPUART_TransmitPollTimeOut(LPUART1, u8RxData, RX_TX_FRAME_LEN, 0xFFFFF); /* 将收到的数据发送给PC */
        }
    }
}

/**
 * @brief  LPUART配置
 * @retval None
 */
static void LpUartConfig(void)
{
    stc_lpuart_init_t stcLpuartInit;

    /* 外设模块时钟使能 */
    SYSCTRL_PeriphClockEnable(PeriphClockLpuart1);

    /* LPUART 初始化 */
    LPUART_StcInit(&stcLpuartInit);                                    /* 结构体初始化         */
    stcLpuartInit.u32StopBits               = LPUART_STOPBITS_1;       /* 1停止位              */
    stcLpuartInit.u32FrameLength            = LPUART_FRAME_LEN_8B_PAR; /* 数据8位，奇偶校验1位 */
    stcLpuartInit.u32Parity                 = LPUART_B8_PARITY_EVEN;   /* 偶校验               */
    stcLpuartInit.u32TransMode              = LPUART_MODE_TX_RX;       /* 收发模式             */
    stcLpuartInit.stcBaudRate.u32SclkSelect = LPUART_SCLK_SEL_PCLK;    /* 传输时钟源           */
    stcLpuartInit.stcBaudRate.u32Sclk       = SYSCTRL_HclkFreqGet();   /* HCLK获取             */
    stcLpuartInit.stcBaudRate.u32Baud       = 9600;                    /* 波特率               */
    LPUART_Init(LPUART1, &stcLpuartInit);

    LPUART_IntFlagClearAll(LPUART1); /* 清除所有状态标志 */

    LPUART_HdModeEnable(LPUART1); /* 单线模式使能 */
}

/**
 * @brief  端口配置
 * @retval None
 */
static void GpioConfig(void)
{
    stc_gpio_init_t stcGpioInit = {0};

    /* 外设模块时钟使能 */
    SYSCTRL_PeriphClockEnable(PeriphClockGpio);

    /* 配置PA01为LPUART1_TX */
    GPIO_StcInit(&stcGpioInit);
    stcGpioInit.u32Mode      = GPIO_MD_OUTPUT_OD;
    stcGpioInit.u32PullUp    = GPIO_PULL_UP;
    stcGpioInit.bOutputValue = TRUE;
    stcGpioInit.u32Pin       = GPIO_PIN_01;
    GPIO_Init(GPIOA, &stcGpioInit);
    GPIO_PA01_AF_LPUART1_TXD();
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
