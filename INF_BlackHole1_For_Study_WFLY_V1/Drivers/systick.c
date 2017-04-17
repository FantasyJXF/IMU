/*********BlackHole1 For Study**************
 *	文件名					:				systick.c
 *	描述					: 	系统滴答中断初始化
 *	版本					:	BlackHole1_For_Study_V1
 *	作者					:	 	【INF】无穷创新		
 *	技术论坛				:		 www.infcre.com
 *	QQ群					:   	  168590475
 *  源码状态				:         开源
 *			For infinite creativity
 *********(C) COPYRIGHT 2015 INF ************/

#include "systick.h"

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
}




