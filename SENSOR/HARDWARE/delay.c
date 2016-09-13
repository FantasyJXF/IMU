/********* For Study **************
 *	文件名				:				delay.c
 *	描述					: 		延时函数文件
 *********(C) COPYRIGHT************/

#include "STM32F4.h"
	 
static u8  fac_us=0;          //us延时倍乘数
static u16 fac_ms=0;          //ms延时倍乘数

//初始化延迟函数
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init()	 
{

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	fac_us=SystemCoreClock/8000000;	                     //为系统时钟的1/8  

	fac_ms=(u16)fac_us*1000;                            //代表每个ms需要的systick时钟数   
}								    

/*
 * 函数名：Delay_Us
 * 描述  ：毫秒延时函数
 * 输入  ：time：延时时间，单位：ms
 * 输出  ：无
 */ 	    								   
void Delay_Us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}

/*
 * 函数名：Delay_Ms
 * 描述  ：毫秒延时函数
 * 输入  ：time：延时时间，单位：ms
 * 输出  ：无
 */ 
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对168M条件下,nms<=798
void Delay_Ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;      //时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;                 //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;     //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
} 



