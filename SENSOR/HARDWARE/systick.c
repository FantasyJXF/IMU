/********* For Study**************
 *	文件名				:				SysTick.c
 *	描述					: 	系统滴答中断初始化
 *********(C) COPYRIGHT ************/

#include "STM32F4.h"

/*
 * 函数名：SysTick_Init
 * 描述  ：启动系统滴答定时器 SysTick
 * 输入  ：无
 * 输出  ：无
 */
void Systick_Init(void)
{
	//SystemFrequency / 1000    1ms中断一次
	//SystemFrequency / 500	    2ms中断一次
	 
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
		while (1);
	}
	
	/*
		配置systic中断周期为1ms，并启动systick中断。

    	SystemCoreClock 是固件中定义的系统内核时钟，对于STM32F427,一般为 168MHz

    	SysTick_Config() 函数的形参表示内核时钟多少个周期后触发一次Systick定时中断.
	    	-- SystemCoreClock / 1000  表示定时频率为 1000Hz， 也就是定时周期为  1ms
	    	-- SystemCoreClock / 500   表示定时频率为 500Hz，  也就是定时周期为  2ms
	    	-- SystemCoreClock / 2000  表示定时频率为 2000Hz， 也就是定时周期为  500us

    	对于常规的应用，我们一般取定时周期1ms。对于低速CPU或者低功耗应用，可以设置定时周期为 10ms
    */
//	SysTick_Config(SystemCoreClock / 1000);
}





