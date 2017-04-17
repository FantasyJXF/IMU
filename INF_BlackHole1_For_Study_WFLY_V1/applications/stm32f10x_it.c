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
 * ��������SysTick_Handler
 * ����  ���δ��жϺ���
 * ����  ����
 * ���  ����
 */  
void SysTick_Handler(void)
{	
	static u16 cnt = 0;
	static u8 ms = 0;
	
	cnt++;

	Prepare_Data();  
	if(Is_WFLY_Unlock())//�ж��Ƿ�������Ƿ���ִ�н�������	
	{		
		Is_WFLY_Lock();		//�ж��Ƿ���ִ����������	
		IMU_Update();			//��̬����Ƶ��Ϊ1KHz
		ms++;
		if(ms == 2)	 			//����Ƶ��500Hz
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
	else//δ����
	{
		Motor_PWM_Update(0,0,0,0);//ȷ����ȫ
		if(cnt % 1000 == 0)	
		{	
			LED_LEFT_BACK_TOGGLE;
			LED_RIGHT_BACK_TOGGLE;
			cnt = 0;
		}	
	}

}


u8 	Rc_Capture_Sta[6] = {0};//6��ͨ���Ĳ���״̬
u16 Rc_Channel_Val[6] = {0};//6��ͨ���Ĳ���ֵ

/*
 * ��������TIM2_IRQHandler
 * ����  ����ʱ��2���벶���жϺ���
 * ����  ����
 * ���  ����
 */ 
void TIM2_IRQHandler(void)
{
//**************************����1���������¼�**************************	
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1)	!= RESET)
		{	
			if(Rc_Capture_Sta[0])																				//�����½��� 		
			{	  			
				Rc_Channel_Val[0]	=	TIM_GetCapture1(TIM2);								//��ȡ����ֵ������ΪPWM�ߵ�ƽʱ��
			 	Rc_Capture_Sta[0]	=	0;																		//��ǲ���������
				TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); 				//����Ϊ�����ز���
			}
			else  																											//����������
			{
				TIM_SetCounter(TIM2,0);																		//�����������������
				Rc_Capture_Sta[0]	=	1;																		//��ǲ�����������
				TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);				//����Ϊ�½��ز���
			}	
      TIM_ClearITPendingBit(TIM2, TIM_IT_CC1	|	TIM_IT_Update ); 	//���ͨ��1�жϱ�־λ			
		}			     	   
	
//**************************����2���������¼�**************************			
	if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET)
	{	
		if(Rc_Capture_Sta[1])																					//�����½��� 		
		{	  			
			Rc_Channel_Val[1]	=	TIM_GetCapture2(TIM2);									//��ȡ����ֵ������ΪPWM�ߵ�ƽʱ��	
			Rc_Capture_Sta[1]	=	0;																			//��ǲ��������� 		
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); 					//����Ϊ�����ز��� 
		}
		else																													//����������
		{
			Rc_Capture_Sta[1]	=	1;																			//�����������������
			TIM_SetCounter(TIM2,0);																			//��ǲ�����������
			TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);					//����Ϊ�½��ز���
		}		 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC2 ); 			 							//���ͨ��2�жϱ�־λ	
	}			 	
		
//**************************����3���������¼�**************************			
	if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
	{	
		if(Rc_Capture_Sta[2])																					//�����½��� 				 		
		{	  			
			Rc_Channel_Val[2]=TIM_GetCapture3(TIM2);										//��ȡ����ֵ������ΪPWM�ߵ�ƽʱ��
			Rc_Capture_Sta[2]=0;																				//��ǲ���������
		  TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Rising); 					//����Ϊ�����ز���  
		}
		else  																												//����������
		{
			Rc_Capture_Sta[2]=1;																				//�����������������
	 		TIM_SetCounter(TIM2,0);																			//��ǲ�����������
			TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Falling);					//����Ϊ�½��ز���
		}		 
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC3 ); 			 							//���ͨ��3�жϱ�־λ	
	}			 

	
//**************************����4���������¼�**************************	
	if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET)
	{	
		if(Rc_Capture_Sta[3])																					//�����½��� 		
		{	  			
			Rc_Channel_Val[3]=TIM_GetCapture4(TIM2);										//��ȡ����ֵ������ΪPWM�ߵ�ƽʱ��
			Rc_Capture_Sta[3]=0;																				//��ǲ���������
			TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Rising);  				//����Ϊ�����ز���
		}
		else  																												//����������
		{
			Rc_Capture_Sta[3]=1;																				//�����������������
	 		TIM_SetCounter(TIM2,0);																			//��ǲ�����������
	 		TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Falling);					//����Ϊ�½��ز���
		}	
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC4 ); 			     					//���ͨ��4�жϱ�־λ	
	}			 
}

/*
 * ��������TIM3_IRQHandler
 * ����  ����ʱ��3���벶���жϺ���
 * ����  ����
 * ���  ����
 */ 
void TIM3_IRQHandler(void)
{
	//**************************����1���������¼�**************************	
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1)	!= RESET)
		{	
			if(Rc_Capture_Sta[4])																				//�����½��� 		
			{	  			
				Rc_Channel_Val[4]	=	TIM_GetCapture1(TIM3);								//��ȡ����ֵ������ΪPWM�ߵ�ƽʱ��
			 	Rc_Capture_Sta[4]	=	0;																		//��ǲ���������
				TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising); 				//����Ϊ�����ز���
			}
			else  																											//����������
			{
				TIM_SetCounter(TIM3,0);																		//�����������������
				Rc_Capture_Sta[4]	=	1;																		//��ǲ�����������
				TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);				//����Ϊ�½��ز���
			}	
      TIM_ClearITPendingBit(TIM3, TIM_IT_CC1	|	TIM_IT_Update ); 	//���ͨ��1�жϱ�־λ			
		}			     	   
	
//**************************����2���������¼�**************************			
	if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)
	{	
		if(Rc_Capture_Sta[5])																					//�����½��� 		
		{	  			
			Rc_Channel_Val[5]	=	TIM_GetCapture2(TIM3);									//��ȡ����ֵ������ΪPWM�ߵ�ƽʱ��	
			Rc_Capture_Sta[5]	=	0;																			//��ǲ��������� 		
			TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Rising); 					//����Ϊ�����ز��� 
		}
		else																													//����������
		{
			Rc_Capture_Sta[5]	=	1;																			//�����������������
			TIM_SetCounter(TIM3,0);																			//��ǲ�����������
			TIM_OC2PolarityConfig(TIM3,TIM_ICPolarity_Falling);					//����Ϊ�½��ز���
		}		 
    TIM_ClearITPendingBit(TIM3, TIM_IT_CC2 ); 			 							//���ͨ��2�жϱ�־λ	
	}		
}




