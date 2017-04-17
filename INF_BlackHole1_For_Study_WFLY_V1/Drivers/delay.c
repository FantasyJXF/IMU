/*********BlackHole1 For Study**************
 *	文件名					:				delay.c
 *	描述					: 		延时函数文件
 *	版本					:	BlackHole1_For_Study_V1
 *	作者					:	 	【INF】无穷创新		
 *	技术论坛				:		 www.infcre.com
 *	QQ群					:   	  168590475
 *  源码状态				:         开源
 *			For infinite creativity
 *********(C) COPYRIGHT 2015 INF ************/

#include "delay.h"
	 
/*
 * 函数名：Delay_Us
 * 描述  ：微秒粗延时函数
 * 输入  ：time：延时时间，单位：us
 * 输出  ：无
 */  	 
void Delay_Us(u16 us)
{    
   u16 i=0;  
   while(us--)
   {
      i=10;  
      while(i--) ;    
   }
}

/*
 * 函数名：Delay_Ms
 * 描述  ：毫秒延时函数
 * 输入  ：time：延时时间，单位：ms
 * 输出  ：无
 */  
void Delay_Ms(u16 ms)
{    
   u16 i=0;  
   while(ms--)
   {
      i=12000;
      while(i--) ;    
   }
 }



































