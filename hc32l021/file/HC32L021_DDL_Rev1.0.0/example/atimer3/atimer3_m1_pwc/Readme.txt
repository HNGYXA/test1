================================================================================
                               样例使用说明
================================================================================
版本历史 
================================================================================
Data           Author   IAR         MDK         Note
2024-12-02     MADS     7.70        5.26        first version

================================================================================
功能描述
================================================================================
本样例主要展示Atimer3的脉宽测量功能
说明：
本样例使用Atimer3的PWC循环脉宽测量功能

================================================================================
测试环境
================================================================================
测试用板
--------------------------------------------------------------------------------
SK_L021_TSSOP20_Rev1.0

管脚    作用                    测试连接
--------------------------------------------------------------------------------
PA11   TIM3 CH0A               接方波信号

辅助工具
--------------------------------------------------------------------------------
示波器

辅助软件
--------------------------------------------------------------------------------
无

================================================================================
使用步骤
================================================================================
1、准备测试环境，打开样例工程编译，使用debug模式下载后运行
2、atimer在第一个上升沿的时候自动清零计数器，并开始计数，到第二个上升沿的时候
   停止计数并捕获计数值测量PWM的指定起始和结束边沿（上升沿到上升沿）的计数值
3、观察u32PwcCapValue的值，根据计数时钟可计算出被捕获信号的频率值

================================================================================
注意事项
================================================================================
无
