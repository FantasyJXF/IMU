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
#include "mpu6050.h"
#include "bsp_i2c.h"

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
	short Accel[3];
	short Gyro[3];
	short Temp;
  
  //初始化systick
	SysTick_Init();
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	/* LED 端口初始化 */
	LED_GPIO_Config();
	/* 串口1通信初始化 */
	USARTx_Config();

	//I2C初始化
	i2c_GPIO_Config();
  //MPU6050初始化
	MPU6050_Init();

	//检测MPU6050
	if (MPU6050ReadID() == 1)
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
				MPU6050ReadAcc(Accel);			
				printf("\r\n加速度： %8d%8d%8d    ",Accel[0],Accel[1],Accel[2]);
				MPU6050ReadGyro(Gyro);
				printf("陀螺仪： %8d%8d%8d    ",Gyro[0],Gyro[1],Gyro[2]);
				
				MPU6050_ReturnTemp(&Temp);
				printf("温度： %d",Temp);
				
				Task_Delay[1]=100;//此值每1ms会减1，减到0才可以重新进来这里，所以执行的周期是100ms
			}

			//*************************************	下面是增加任务的格式************************************//
	//		if(Task_Delay[i]==0)
	//		{
	//			Task(i);
	//			Task_Delay[i]=;
	//		}

		}   
	}
	else
		while(1);	
}

/*********************************************END OF FILE**********************/
