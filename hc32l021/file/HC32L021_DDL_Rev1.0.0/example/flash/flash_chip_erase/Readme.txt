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
本样例展示FLASH的整片擦除功能

================================================================================
测试环境
================================================================================
测试用板
--------------------------------------------------------------------------------
SK_L021_TSSOP20_Rev1.0

管脚    作用                    测试连接
--------------------------------------------------------------------------------
PA00    LED                     
PA12    USER KEY                

辅助工具
--------------------------------------------------------------------------------
无

辅助软件
--------------------------------------------------------------------------------
无

================================================================================
使用步骤
================================================================================
1、准备测试环境，打开样例工程编译下载
   注：当使用KEIL时，需在编译前将工程的.sct文件替换成source目录下的ram_code.sct
2、在while(1)处设置断点，运行程序
3、当擦除完成后，可打开调试器Memory窗口查看整个FLASH的数据为xFF

================================================================================
注意事项
================================================================================
1、当使用KEIL时，需将工程的.sct文件替换成source目录下的ram_code.sct
