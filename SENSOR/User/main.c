/******************** (C) COPYRIGHT 2016 Fantasy **************************
 * �ļ���  ��main.c
 * ����    : ��ȡ����������    
 * ��汾  ��ST3.5.0
 * ����    ��Fantasy
**********************************************************************************/	
#include "STM32F4.h"

/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */  
//extern MPU_value mpu_value;        //9������
 
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
		
