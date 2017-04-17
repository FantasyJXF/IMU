/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_it.h"
#include "led.h"
#include "ahrs.h"
#include "control.h"
#include "systick.h"
#include "telecontrol.h"

extern S_FLOAT_XYZ Exp_Angle;

/*
 * 函数名：SysTick_Handler
 * 描述  ：滴答中断函数
 * 输入  ：无
 * 输出  ：无
 */  
void SysTick_Handler(void)
{	
	static u16 cnt = 0;
	static u8 ms = 0;
	
	cnt++;

	Prepare_Data();  
	if(Is_WFLY_Unlock())//判断是否解锁与是否在执行解锁动作	
	{		
		Is_WFLY_Lock();		//判断是否在执行上锁动作	
		IMU_Update();			//姿态更新频率为1KHz
		ms++;
		if(ms == 2)	 			//控制频率500Hz
		{ 
			ms = 0;			
			WFLY_PWM_Convert();
			Control(Angle,Exp_Angle);		
		}			
		if(cnt % 500 == 0)	
		{	
			LED_LEFT_BACK_TOGGLE;
			LED_RIGHT_BACK_TOGGLE;
			cnt = 0;
		}	
	}
	else//未解锁
	{
		Motor_PWM_Update(0,0,0,0);//确保安全
		if(cnt % 1000 == 0)	
		{	
			LED_LEFT_BACK_TOGGLE;
			LED_RIGHT_BACK_TOGGLE;
			cnt = 0;
		}	
	}

}


u8 	Rc_Capture_Sta[6] = {0};//6个通道的捕获状态
u16 Rc_Channel_Val[6] = {0};//6个通道的捕获值

/*
 * 函数名：TIM2_IRQHandler
 * 描述  ：定时器2输入捕获中断函数
 * 输入  ：无
 * 输出  ：无
 */ 
void TIM2_IRQHandler(void)
{
//**************************捕获1发生捕获事件**************************	
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1)	!= RESET)
		{	
			if(Rc_Capture_Sta[0])																				//捕获到下降沿 		
			{	  			
				Rc_Channel_Val[0]	=	TIM_GetCapture1(TIM2);								//读取捕获值，这里为PWM高电平时间
			 	Rc_Capture_Sta[0]	=	0;																		//标记捕获到下升沿
				TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); 				//设置为上升沿捕获
			}
			else  																											//捕获到上升沿
			{
				TIM_SetCounter(TIM2,0);																		//计数器立刻清零计数
				Rc_Capture_Sta[0]	=	1;																		//标记捕获到了上升沿
				TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);				//设置为下降沿捕获
			}	
      TIM_ClearITPendingBit(TIM2, TIM_IT_CC1	|	TIM_IT_Update ); 	//清除通道1中断标志位			
		}			     	   
	
//**************************捕获2发生捕获事件**************************			
	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
	{	
		if(Rc_Capture_Sta[1])																					//捕获到下降沿 		
		{	  			
			Rc_Channel_Val[1]	=	TIM_GetCapture2(TIM2);									//读取捕获值，这里为PWM高电平时间	
			Rc_Capture_Sta[1]	=	0;																			//标记捕获到下升沿 		
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); 					//设置为上升沿捕获 
		}
		else																													//捕获到上升沿
		{
			Rc_Capture_Sta[1]	=	1;																			//计数器立刻清零计数
			TIM_SetCounter(TIM2,0);																			//标记捕获到了上升沿
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);					//设置为下降沿捕获
		}		 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC2 ); 			 							//清除通道2中断标志位	
	}			 	
		
//**************************捕获3发生捕获事件**************************			
	if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
	{	
		if(Rc_Capture_Sta[2])																					//捕获到下降沿 				 		
		{	  			
			Rc_Channel_Val[2]=TIM_GetCapture3(TIM2);										//读取捕获值，这里为PWM高电平时间
			Rc_Capture_Sta[2]=0;																				//标记捕获到下升沿
		  TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Rising); 					//设置为上升沿捕获  
		}
		else  																												//捕获到上升沿
		{
			Rc_Capture_Sta[2]=1;																				//计数器立刻清零计数
	 		TIM_SetCounter(TIM2,0);																			//标记捕获到了上升沿
			TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Falling);					//设置为下降沿捕获
		}		 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC3 ); 			 							//清除通道3中断标志位	
	}			 

	
//**************************捕获4发生捕获事件**************************	
	if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
	{	
		if(Rc_Capture_Sta[3])																					//捕获到下降沿 		
		{	  			
			Rc_Channel_Val[3]=TIM_GetCapture4(TIM2);										//读取捕获值，这里为PWM高电平时间
			Rc_Capture_Sta[3]=0;																				//标记捕获到下升沿
			TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Rising);  				//设置为上升沿捕获
		}
		else  																												//捕获到上升沿
		{
			Rc_Capture_Sta[3]=1;																				//计数器立刻清零计数
	 		TIM_SetCounter(TIM2,0);																			//标记捕获到了上升沿
	 		TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Falling);					//设置为下降沿捕获
		}	
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC4 ); 			     					//清除通道4中断标志位	
	}			 
}

/*
 * 函数名：TIM3_IRQHandler
 * 描述  ：定时器3输入捕获中断函数
 * 输入  ：无
 * 输出  ：无
 */ 
void TIM3_IRQHandler(void)
{
	//**************************捕获1发生捕获事件**************************	
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1)	!= RESET)
		{	
			if(Rc_Capture_Sta[4])																				//捕获到下降沿 		
			{	  			
				Rc_Channel_Val[4]	=	TIM_GetCapture1(TIM3);								//读取捕获值，这里为PWM高电平时间
			 	Rc_Capture_Sta[4]	=	0;																		//标记捕获到下升沿
				TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising); 				//设置为上升沿捕获
			}
			else  																											//捕获到上升沿
			{
				TIM_SetCounter(TIM3,0);																		//计数器立刻清零计数
				Rc_Capture_Sta[4]	=	1;																		//标记捕获到了上升沿
				TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);				//设置为下降沿捕获
			}	
      TIM_ClearITPendingBit(TIM3, TIM_IT_CC1	|	TIM_IT_Update ); 	//清除通道1中断标志位			
		}			     	   
	
//**************************捕获2发生捕获事件**************************			
	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
	{	
		if(Rc_Capture_Sta[5])																					//捕获到下降沿 		
		{	  			
			Rc_Channel_Val[5]	=	TIM_GetCapture2(TIM3);									//读取捕获值，这里为PWM高电平时间	
			Rc_Capture_Sta[5]	=	0;																			//标记捕获到下升沿 		
			TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Rising); 					//设置为上升沿捕获 
		}
		else																													//捕获到上升沿
		{
			Rc_Capture_Sta[5]	=	1;																			//计数器立刻清零计数
			TIM_SetCounter(TIM3,0);																			//标记捕获到了上升沿
			TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Falling);					//设置为下降沿捕获
		}		 
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2 ); 			 							//清除通道2中断标志位	
	}		
}




