/*********BlackHole1 For Study**************
 *	文件名					:				led.c
 *	描述					: 	led初始化文件
 *	版本					:	BlackHole1_For_Study_V1
 *	作者					:	 	【INF】无穷创新		
 *	技术论坛				:		 www.infcre.com
 *	QQ群					:   	  168590475
 *  源码状态				:         开源
 *			For infinite creativity
 *********(C) COPYRIGHT 2015 INF ************/

#include "led.h"
#include "delay.h"

/*
 * 函数名：LED_Init
 * 描述  ：LED灯GPIO口初始化
 * 输入  ：无
 * 输出  ：无
 */  	 
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
}

/*
 * 函数名：LED_StartShow
 * 描述  ：延时与状态显示
 * 输入  ：无
 * 输出  ：无
 */  	 
void LED_StartShow(void)
{
	int i = 0;
	
	for(; i<5 ; i++)
	{
		LED_LEFT_BACK(1);
		LED_RIGHT_BACK(0);
		Delay_Ms(200);
		LED_LEFT_BACK(0);
		LED_RIGHT_BACK(1);
		Delay_Ms(200);
	}
	
	LED_LEFT_BACK(1);
	LED_RIGHT_BACK(1);
}

