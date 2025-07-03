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
#include "gpio.h"
#include "lpuart.h"
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define SLAVEADDR 0xc0
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void GpioConfig(void);
void LpUartConfig(void);
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
volatile uint8_t u8MasterTxData[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
volatile uint8_t u8MasterRxData[10] = {0x00};
uint8_t          u8TxCnt            = 0;
uint8_t          u8RxCnt            = 0;
/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  Main function
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
    /* 串口引脚配置 */
    GpioConfig();

    /* 串口模块配置 */
    LpUartConfig();

    /* 主机发送从机地址，接下来中断中发送数据 */
    LPUART1->SBUF = (1 << 8) | SLAVEADDR;

    while (1)
    {
        ;
    }
}

/**
 * @brief  LPUART1中断函数
 * @retval None
 */
void LpUart1_IRQHandler(void)
{
    /* 主机数据发送 */
    if (LPUART_IntFlagGet(LPUART1, LPUART_FLAG_TC))
    {
        if (u8TxCnt > 9) /* 如果已经发送10个数据字节 */
        {
            LPUART_IntDisable(LPUART1, LPUART_INT_TC); /* 禁止发送中断 */
            LPUART_IntEnable(LPUART1, LPUART_INT_RC);  /* 使能接收中断 */
        }
        else
        {
            LPUART_TransmitInt(LPUART1, u8MasterTxData[u8TxCnt++]); /* 发送数据 */
        }
        LPUART_IntFlagClear(LPUART1, LPUART_FLAG_TC);
    }

    /* 主机数据接收 */
    if ((LPUART_IntFlagGet(LPUART1, LPUART_FLAG_RC)))
    {
        u8MasterRxData[u8RxCnt++] = LPUART_ReceiveInt(LPUART1); /* 接收数据 */
        if (u8RxCnt > 9)                                        /* 如果已经接收到10个数据 */
        {
            LPUART_IntDisable(LPUART1, LPUART_INT_RC); /* 禁止接收中断 */
        }
        LPUART_IntFlagClear(LPUART1, LPUART_FLAG_RC);
    }
}

/**
 * @brief  串口引脚配置
 * @retval None
 */
void GpioConfig(void)
{
    stc_gpio_init_t stcGpioInit = {0};

    /* 外设模块时钟使能 */
    SYSCTRL_PeriphClockEnable(PeriphClockGpio);

    /* 配置PA01为LPUART1_TX */
    GPIO_StcInit(&stcGpioInit);
    stcGpioInit.u32Mode      = GPIO_MD_OUTPUT_PP;
    stcGpioInit.u32PullUp    = GPIO_PULL_UP;
    stcGpioInit.bOutputValue = TRUE;
    stcGpioInit.u32Pin       = GPIO_PIN_01;
    GPIO_Init(GPIOA, &stcGpioInit);
    GPIO_PA01_AF_LPUART1_TXD();

    /* 配置PA02为LPUART1_RX */
    GPIO_StcInit(&stcGpioInit);
    stcGpioInit.u32Mode = GPIO_MD_INPUT;
    stcGpioInit.u32Pin  = GPIO_PIN_02;
    GPIO_Init(GPIOA, &stcGpioInit);
    GPIO_PA02_AF_LPUART1_RXD();
}

/**
 * @brief  串口模块配置
 * @retval None
 */
void LpUartConfig(void)
{
    stc_lpuart_init_t stcLpuartInit = {0};

    /* 外设模块时钟使能 */
    SYSCTRL_PeriphClockEnable(PeriphClockLpuart1);

    /* LPUART 初始化 */
    LPUART_StcInit(&stcLpuartInit);                                       /* 结构体初始化          */
    stcLpuartInit.u32StopBits               = LPUART_STOPBITS_1;          /* 1停止位               */
    stcLpuartInit.u32FrameLength            = LPUART_FRAME_LEN_8B_PAR;    /* 数据8位、奇偶校验1位  */
    stcLpuartInit.u32Parity                 = LPUART_B8_MULTI_DataOrAddr; /* 多机模式              */
    stcLpuartInit.u32TransMode              = LPUART_MODE_TX_RX;          /* 收发模式              */
    stcLpuartInit.stcBaudRate.u32SclkSelect = LPUART_SCLK_SEL_PCLK;       /* 传输时钟源            */
    stcLpuartInit.stcBaudRate.u32Sclk       = SYSCTRL_HclkFreqGet();      /* HCLK获取              */
    stcLpuartInit.stcBaudRate.u32Baud       = 9600;                       /* 波特率                */
    stcLpuartInit.u32HwControl              = LPUART_HWCONTROL_NONE;      /* 无硬件流控            */
    LPUART_Init(LPUART1, &stcLpuartInit);

    LPUART_IntFlagClearAll(LPUART1);                   /* 清除所有状态标志 */
    LPUART_IntEnable(LPUART1, LPUART_INT_TC);          /* <使能串口发送中断 */
    EnableNvic(LPUART1_IRQn, IrqPriorityLevel3, TRUE); /* 系统中断使能 */
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
