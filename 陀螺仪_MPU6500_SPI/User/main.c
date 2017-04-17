/**
  ******************************************************************************
  * @file    main.c
  * @author  LYC
  * @version V1.0
  * @date    2014-04-22
  * @brief   移植野火的软件I2C
  ******************************************************************************
  * @attention
  * 实验平台:野火 iSO STM32 开发板 
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
 * t : 定时时间 
 * Ticks : 多少个时钟周期产生一次中断 
 * f : 时钟频率 72000000
 * t = Ticks * 1/f = (72000000/100000) * (1/72000000) = 10us 
 */ 

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{
		u16 adress;
  
  //初始化systick
	SysTick_Init();
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	/* LED 端口初始化 */
	LED_GPIO_Config();
	/* 串口1通信初始化 */
	USARTx_Config();

	//SPI初始化
	SPI1_Init();
  //MPU6050初始化
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

				printf("小东子，我错了\n");
				MPU6500_ReadValue();
			
				printf("\r\n加速度： %8d%8d%8d    ",MPU6500_Acc.X,MPU6500_Acc.Y,MPU6500_Acc.Z);

				printf("陀螺仪： %8d%8d%8d    ",MPU6500_Gyro.X,MPU6500_Gyro.Y,MPU6500_Gyro.Z);
				
				printf("温度： %5.2f",MPU6500_tempreature);
				
				Task_Delay[1]=100;//此值每1ms会减1，减到0才可以重新进来这里，所以执行的周期是100ms
			}
		}   
	}
	else
		while(1);	
}

		

/*********************************************END OF FILE**********************/
