/**
 *******************************************************************************
 * @file  main.c
 * @brief This file provides example of ADC
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
#include "adc.h"
#include "ctrim.h"
#include "ddl.h"
#include "gpio.h"
#include "myiic.h"
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void GpioConfig(void);
static void CtrimConfig(void);
static void AdcConfig(void);
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
uint32_t AdcResult1;
uint32_t AdcResult2;
float VO;
float IO;
/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief  Main function
 * @retval int32_t return value, if needed
 */
int32_t main(void)
{
		
		
    /* CTRIM配置 */
    CtrimConfig();
		
		/* GPIO配置 */
		GpioConfig();
		
    /* ADC配置 */
    AdcConfig();

    /* 使能CTRIM */
    CTRIM_Enable();
		
		//
		IIC_WR(0x01,0xff);
    while (1)
		{
				
			DDL_Delay100us(1000);
			IO = AdcResult1 *3.3 / 4096 ;
			VO = AdcResult2 *3.3 / 4096 ;
			DDL_Delay100us(1000);
			IIC_WR(0x01,0xff);
				
    }
}

/**
 * @brief  GPIO_SDA配置
 * @retval None
 */


/**
 * @brief  CTRIM 中断服务程序
 * @retval None
 */
void Ctrim_Clkdet_IRQHandler(void)
{
    if (TRUE == CTRIM_FlagGet(CTRIM_FLAG_UD))
    {
        /* 等待转换完成 */
        if (ADC_IntFlagGet(ADC_FLAG_SQR))
        {
            /* 清除中断标志 */
            ADC_IntFlagClear(ADC_FLAG_SQR);
						
            /* 获取采样值 */
            AdcResult1 = ADC_SqrResultGet(ADC_SQR_CH0_MUX);
            AdcResult2 = ADC_SqrResultGet(ADC_SQR_CH1_MUX);

            CTRIM_FlagClear(CTRIM_FLAG_UD); /* 清除标志位 */
        }
    }
}

/**
 * @brief  ADC相关配置
 * @retval None
 */
static void AdcConfig(void)
{
    stc_adc_sqr_init_t stcAdcSqrConfig = {0};

    SYSCTRL_PeriphClockEnable(PeriphClockAdc);

    ADC_Enable();

    /* ADC 初始化配置 */
    ADC_SqrStcInit(&stcAdcSqrConfig);
    stcAdcSqrConfig.u32SampCycle     = ADC_SAMPLE_CYCLE_127; /* ADC采样周期选择 */
    stcAdcSqrConfig.u32RefVoltage    = ADC_REF_VOL_AVCC;    /* ADC参考电压选择 */
    stcAdcSqrConfig.u32ClockDiv      = ADC_CLK_DIV8;        /* ADC时钟分频选择 */
    stcAdcSqrConfig.u32CurrentSelect = ADC_IBAS_LOW_POWER; /* ADCIBAS电流选择 */
    stcAdcSqrConfig.u32SqrCount      = 2;                   /* ADC转换次数配置 */

    ADC_SqrInit(&stcAdcSqrConfig); /* 初始化配置 */

    /* 配置通道和通道输入源 */
		GPIO_PA09_ANALOG_SET();
    GPIO_PA10_ANALOG_SET();
    ADC_ConfigSqrCh(ADC_SQR_CH0_MUX, ADC_INPUT_CH4); /* 配置通道0的输入源来自PA09 */
    ADC_ConfigSqrCh(ADC_SQR_CH1_MUX, ADC_INPUT_CH5); /* 配置通道1的输入源来自PA10 */
    /* 配置外部触发源 */
    ADC_ExternTrigEnable(ADC_TRIG_CTRIM);
    /* 配置中断 */
    ADC_IntEnable();
    EnableNvic(ADC_IRQn, IrqPriorityLevel3, TRUE);
}

/**
 * @brief  CTRIM配置
 * @retval None
 */
static void CtrimConfig(void)
{
    stc_ctrim_timer_init_t stcCtrimInit = {0};

    /* 开启CTRIM 外设时钟 */
    SYSCTRL_PeriphClockEnable(PeriphClockCtrim);

    /* 复位CTRIM模块 */
    SYSCTRL_PeriphReset(PeriphResetCtrim);

    /* CTRIM 初始化配置 */
    CTRIM_TimerStcInit(&stcCtrimInit);                       /* 结构体变量初始化               */
    stcCtrimInit.u32Clock       = CTRIM_ACCURATE_CLOCK_PCLK; /* 时钟源选择                    */
    stcCtrimInit.u32ClockDiv    = CTRIM_REF_CLOCK_DIV128;    /* 分频128                       */
    stcCtrimInit.u16ReloadValue = 31250u - 1u;               /* 定时约1s，4000000/128 = 31250 */
    CTRIM_TimerInit(&stcCtrimInit);

    /* 清除所有标志位 */
    CTRIM_FlagClearALL();

    /* 使能中断 */
    CTRIM_IntEnable(CTRIM_INT_UD);                          /* 计数器下溢中断 使能      */
    EnableNvic(CTRIM_CLKDET_IRQn, IrqPriorityLevel1, TRUE); /* 使能并配置CTRIM 系统中断 */
}

static void GpioConfig(void)
{
    stc_gpio_init_t stcGpioInit = {0};

    SYSCTRL_PeriphClockEnable(PeriphClockGpio); /*开启GPIO时钟*/

    GPIO_StcInit(&stcGpioInit);                   /* 结构体变量初始值初始化 */
    stcGpioInit.bOutputValue = TRUE;              /* 设置GPIO输出初始值为高 */
    stcGpioInit.u32Mode      = GPIO_MD_OUTPUT_OD; /* 端口方向配置 */
    stcGpioInit.u32PullUp    = GPIO_PULL_NONE;    /* 端口上拉配置 */
    stcGpioInit.u32Pin       = GPIO_PIN_00;       /* 端口引脚配置 */
    GPIOA_Init(&stcGpioInit);                     /* GPIO端口初始化 */
		
	  GPIO_StcInit(&stcGpioInit);                   /* 结构体变量初始值初始化 */
    stcGpioInit.bOutputValue = TRUE;              /* 设置GPIO输出初始值为高 */
    stcGpioInit.u32Mode      = GPIO_MD_OUTPUT_OD; /* 端口方向配置 */
    stcGpioInit.u32PullUp    = GPIO_PULL_NONE;    /* 端口上拉配置 */
    stcGpioInit.u32Pin       = GPIO_PIN_15;       /* 端口引脚配置 */
    GPIOA_Init(&stcGpioInit);                     /* GPIO端口初始化 */	
}
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
