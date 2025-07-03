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
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
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

    printf("@ LPUART Printf Example:\r\n");
    printf("  ==>> HC32L021 STK \r\n");
    printf("  ==>> www.xhsc.com.cn\r\n\r\n");

    while (1)
    {
        ;
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
    LPUART_StcInit(&stcLpuartInit);                                    /* 结构体初始化        */
    stcLpuartInit.u32StopBits               = LPUART_STOPBITS_1;       /* 1停止位             */
    stcLpuartInit.u32FrameLength            = LPUART_FRAME_LEN_8B_PAR; /* 数据8位，奇偶校验1位 */
    stcLpuartInit.u32Parity                 = LPUART_B8_PARITY_EVEN;   /* 偶校验              */
    stcLpuartInit.u32TransMode              = LPUART_MODE_TX_RX;       /* 收发模式            */
    stcLpuartInit.stcBaudRate.u32SclkSelect = LPUART_SCLK_SEL_PCLK;    /* 传输时钟源          */
    stcLpuartInit.stcBaudRate.u32Sclk       = SYSCTRL_HclkFreqGet();   /* HCLK获取            */
    stcLpuartInit.stcBaudRate.u32Baud       = 9600;                    /* 波特率              */
    LPUART_Init(LPUART1, &stcLpuartInit);
    LPUART_IntFlagClearAll(LPUART1); /* 清除所有状态标志 */
}

/**
 * @brief  端口配置
 * @retval None
 */
static void GpioConfig(void)
{
    stc_gpio_init_t stcGpioInit = {0};

    SYSCTRL_PeriphClockEnable(PeriphClockGpio);

    /* 配置PA01为LPUART1_TX */
    GPIO_StcInit(&stcGpioInit);
    stcGpioInit.u32Mode      = GPIO_MD_OUTPUT_PP;
    stcGpioInit.u32PullUp    = GPIO_PULL_UP;
    stcGpioInit.bOutputValue = TRUE;
    stcGpioInit.u32Pin       = GPIO_PIN_01;
    GPIO_Init(GPIOA, &stcGpioInit);
    GPIO_PA01_AF_LPUART1_TXD();
}

PUTCHAR_PROTOTYPE
{
    LPUART_TransmitInt(LPUART1, (uint8_t)(ch));
    return ch;
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
