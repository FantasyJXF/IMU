/******************** (C) COPYRIGHT 2016 Fantasy **************************
 * 文件名  ：main.c
 * 描述    : 读取传感器数据    
 * 库版本  ：ST3.5.0
 * 作者    ：Fantasy
**********************************************************************************/	
#include "STM32F4.h"

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */  
//extern MPU_value mpu_value;        //9轴数据
 
int main(void)
{			
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Uart_init(115200);
  SPI1_Init();
	MPU9250_Init();
	while(1)													
	{					
		MPU9250_ReadValue();
		Delay_Ms(500);
  }	 			  
}
		
