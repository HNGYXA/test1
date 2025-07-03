# Update History
------
## Rev1.0.0  Apr. 8, 2025

### documents
-  **更新hc32l021_ddl.chm文件**

### drivers
-  **atim.h & atim.c**
  - 修改ATIM3_Mode23_CHxA_Match_INT、ATIM3_Mode23_CHxB_Match_INT、ATIM3_Mode23_UND_Repeat_Cnt和ATIM3_Mode23_OVF_Repeat_Cnt的宏定义和调用
-  **ctrim.h & ctrim.c**
  - 修改ctrim.h注释
  - 修改CTRIM_REF_Clock_Div_Select的宏定义
  - 修改CTRIM_CalibStcInit()和CTRIM_TimerStcInit()函数
-  **lpuart.c**
  - 删除LPUART_Init()多余代码
-  **sysctrl.h & sysctrl.c**
  - 删除SYSCTRL_RCL_Mode_Select宏定义
  - 删除SYSCTRL_RCLModeSelect()函数定义和申明
  - sysctrl.h中，SPI0和HSI2C0相关定义修改为SPI和HSI2C

### example
-  **startup_hc32l021.s**
  - SPI0和HSI2C0的中断相关定义修改为SPI和HSI2C
-  **atim3**
  - 修改atimer3部分样例readme
  - 修改因宏定义变化相关的调用
-  **hsi2c**
  - HSI2C0的模块名和中断相关定义修改为HSI2C
-  **lvd**
  - 修改lvd_detect_reset样例中main函数中判断语句
-  **spi**
  - SPI0的模块名和中断相关定义修改为SPI

### mcu
- **common**
  - interrupts_hc32l021.c中, SPI0和HSI2C0的中断相关定义修改为SPI和HSI2C
  - 头文件hc32l021.h中，SPI0和HSI2C0的模块名和中断相关定义修改为SPI和HSI2C
  - 修改HC32L021.svd和HC32L021.sfr

------
## Beta1.2.0  Mar 18, 2025

### documents
-  **更新hc32l021_ddl.chm文件**

### drivers
-  **各模块驱动**
  - 修改lpuart.h、iwdt.h和rtc.h注释
-  **atim.h**
  - 修改ATIM3_Mode23_Filter的宏定义
-  **flash.h & flash.c**
  - 修改条件编译中编译器版本号  
-  **gpio.h**
  - 修改和GPIOAUX相关的宏定义
-  **vc.h**
  - 修改空白窗口源选择源宏定义
-  **spi.h**
  - 修改SPI中断标志位宏定义
-  **sysctrl.h & sysctrl.c**
  - 修改和头文件相关的宏定义和枚举常量
  - 修改SYSCTRL_ClockSrcEnable()中时钟使能宏定义值

### example
-  **各模块样例**
  - 修改adc、atimer3、ctimer、ctrim、gpio、iwdt、rtc、spi、trng、stk_test和hsi2c部分样例readme和注释
-  **adc**
  - 修改adc_scan_sqr_hw_trig样例中读取转换结果代码
-  **ctimer**
  - 修改gtimer_cascade_timer样例Atim3Config()定义和调用、修改btimer_toggle样例Btim3Config()函数
  - 修改gtimer_capture_rising样例变量定义和赋值顺序
-  **iwdt**
  - 删除样例中IWDT_Start()调用
-  **spi**
  - 添加主从机单线半双工和单工通信样例
  - 修改spi_slave_poll样例中片选信号判断  

### mcu
- **common**
  - 修改board_stkhc32l021.c中UID地址
  - 修改头文件hc32l021.h
  - 修改HC32L021.svd和HC32L021.sfr

------
## Beta1.1.0  Feb 24, 2025

### documents
- **更新hc32l021_ddl.chm文件**

### drivers
-  **各模块**
   - 添加结构体初始化函数
   - 修改和头文件相关的宏定义
   - 修改部分模块的注释问题
-  **gpio.c**
  - GPIO_Init()调整模拟数字功能的设置顺序和初始电平设置顺序
-  **rtc.c**
  - 添加RTC_CompValueCalculate()函数
-  **iwdt.c**
  - 添加对WWDT功能的处理
-  **hsi2c.c & hsi2c.h**
  - 删除Doze、MCFGR0相关功能代码
  - 修改宏定义命名
  - 修改HSI2C_MasterStatusMachineSubAddr()函数
  - 删除HSI2C_MasterMcfgr0Config()函数
  - 修改HSI2C_SlaveTransAbort()函数
  - 增加主机对于ALF和PLTF错误处理
-  **sysctrl.c & sysctrl.h**
  - 修改宏定义命名
  - 增加XTL驱动能力宏定义
  - 修改SYSCTRL_DebugActiveEnable()函数
  - 修改SYSCTRL_DebugActiveDisable()函数
-  **vc.h**
  - 修改宏定义命名
-  **trng.c**
  - 修改宏定义命名
-  **flash.c & flash.h**
  - 添加FLASH Readonly使能和禁止API
-  **lpuart.c & lpuart.h**
  - 修改初始化函数中波特率计算
  - 修改宏定义命名
  
### example
-  **各模块样例**
   - 修改初始化结构体清零为调用结构体初始化函数
   - 修改部分样例的注释问题
-  **adc**
   - 添加轮询样例
-  **ctrim**
   - 修改ctrim_rc48m_cal样例代码中trim值处理
-  **flash**
   - 添加擦写操作前解除FLASH Read Only处理
-  **gpio**
   - gpio_deepsleep_wakeup样例删除按键进入DeepSleep操作
-  **hsi2c**
  - 增加主机对于ALF和PLTF错误处理
-  **iwdt**
  - 增加DeepSleep下IWDT功能样例和WWDT功能样例
-  **lpuart**
  - 修改中断标志位清除调用函数
-  **lvd**
   - 修改样例中端口配置为模拟端口
-  **rtc**
  - 添加RTC_CompValueCalculate()调用
-  **spi**
  - 修改从机样例中端口配置方向
  
### mcu
- **common**
   - 添加结构体初始化函数
   - 修改和头文件相关的宏定义  
   - 修改interrupts_hc32l021.h宏定义命名
   - 修改头文件hc32l021.h
   - 修改HC32L021.svd和HC32L021.sfr
   - system_hc32l021.h中SystemInit()添加FLASH Read Only配置

------
## Beta1.0.0  Jan 5, 2025
- Initial release.
