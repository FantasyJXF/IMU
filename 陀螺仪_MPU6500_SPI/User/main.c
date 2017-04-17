/**
  ******************************************************************************
  * @file    main.c
  * @author  LYC
  * @version V1.0
  * @date    2014-04-22
  * @brief   ��ֲҰ������I2C
  ******************************************************************************
  * @attention
  * ʵ��ƽ̨:Ұ�� iSO STM32 ������ 
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "bsp_SysTick.h"
#include "bsp_led.h"
#include "bsp_usart1.h"
#include "mpu6500.h"
#include "bsp_spi.h"

#define TASK_ENABLE 0
extern unsigned int Task_Delay[NumOfTask];
/*
 * t : ��ʱʱ�� 
 * Ticks : ���ٸ�ʱ�����ڲ���һ���ж� 
 * f : ʱ��Ƶ�� 72000000
 * t = Ticks * 1/f = (72000000/100000) * (1/72000000) = 10us 
 */ 

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{
		u16 adress;
  
  //��ʼ��systick
	SysTick_Init();
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();
	/* ����1ͨ�ų�ʼ�� */
	USARTx_Config();

	//SPI��ʼ��
	SPI1_Init();
  //MPU6050��ʼ��
	MPU6500_Init();
	
//  adress = MPU6500_Read_Reg(WHO_AM_I);

//  printf("WhoamI? -> %d \n",adress);

if (MPU6500_Read_Reg(WHO_AM_I) == 112)
	{	
	
		while(1)
		{
			if(Task_Delay[0]==TASK_ENABLE)
			{
				LED1_TOGGLE;
				Task_Delay[0]=1000;
			}
			
			if(Task_Delay[1]==0)
			{

				printf("С���ӣ��Ҵ���\n");
				MPU6500_ReadValue();
			
				printf("\r\n���ٶȣ� %8d%8d%8d    ",MPU6500_Acc.X,MPU6500_Acc.Y,MPU6500_Acc.Z);

				printf("�����ǣ� %8d%8d%8d    ",MPU6500_Gyro.X,MPU6500_Gyro.Y,MPU6500_Gyro.Z);
				
				printf("�¶ȣ� %5.2f",MPU6500_tempreature);
				
				Task_Delay[1]=100;//��ֵÿ1ms���1������0�ſ������½����������ִ�е�������100ms
			}
		}   
	}
	else
		while(1);	
}

		

/*********************************************END OF FILE**********************/
