/*********BlackHole1 For Study**************
 *	�ļ���					:			 pwm_in.c
 *	����					:  	pwm���벶���ļ�
 *	�汾					:	BlackHole1_For_Study_V1
 *	����					:	 	��INF�������		
 *	������̳				:		 www.infcre.com
 *	QQȺ					:   	  168590475
 *  Դ��״̬				:         ��Դ
 *			For infinite creativity
 *********(C) COPYRIGHT 2015 INF ************/

#include "pwm_in.h"

/*
 * ��������Pwm_In_Init
 * ����  ��ң�������벶���ʼ������
 * ����  ��arr���Զ���װ�ؼĴ������ڵ�ֵ��psc��ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
 * ���  ����
 */ 
void PWM_In_Init(u16 arr,u16 psc)
{	 
   TIM2_PWM_In_Init(arr,psc);
	 TIM3_PWM_In_Init(arr,psc);
}

/*
 * ��������TIM2_PWM_In_Init
 * ����  ����ʱ��2���벶���ʼ������
 * ����  ��arr���Զ���װ�ؼĴ������ڵ�ֵ��psc��ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
 * ���  ����
 */ 
void TIM2_PWM_In_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM2_ICInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);													//ʹ��TIM2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   											//ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;  	//PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;           											//�����ز��񣬹�����Ϊ��������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	 
	TIM_TimeBaseStructure.TIM_Period = arr;     																	//�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 																		//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 											//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  									//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 															//��ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM2ͨ��1���벶�����
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1 ;  													//ѡ������� IC1ӳ�䵽TI1��
  TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;									//�����ز���
  TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 							//ӳ�䵽TI1��
 	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;											  //���������Ƶ,����Ƶ 
 	TIM2_ICInitStructure.TIM_ICFilter = 0x00;																			//���������˲��������˲�
  TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	//��ʼ��TIM2ͨ��2���벶�����
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_2 ; 														//ѡ������� IC1ӳ�䵽TI1��
  TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;									//�����ز���
  TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 							//ӳ�䵽TI1��
  TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 											//���������Ƶ,����Ƶ 
  TIM2_ICInitStructure.TIM_ICFilter = 0x00;																			//���������˲���,���˲�
  TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	//��ʼ��TIM2ͨ��3���벶�����
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_3 ; 														//ѡ������� IC1ӳ�䵽TI1��
  TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;									//�����ز���
  TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 							//ӳ�䵽TI1��
  TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 											//���������Ƶ,����Ƶ 
 	TIM2_ICInitStructure.TIM_ICFilter = 0x00;																			//���������˲��������˲�
 	TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	//��ʼ��TIM2ͨ��4���벶�����
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_4;															//ѡ������� IC1ӳ�䵽TI1��
	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;									//�����ز���
	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 							//ӳ�䵽TI1��
	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 											//���������Ƶ,����Ƶ 
	TIM2_ICInitStructure.TIM_ICFilter = 0x00;																			//���������˲��������˲�
	TIM_ICInit(TIM2, &TIM2_ICInitStructure);
		
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn  ;  														//TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  										//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  													//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 															//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  																							//��ʼ������NVIC�Ĵ��� 

	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4 ,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
 	TIM_Cmd(TIM2,ENABLE ); 																												//ʹ�ܶ�ʱ��2
}

/*
 * ��������TIM3_PWM_In_Init
 * ����  ����ʱ��3���벶���ʼ������
 * ����  ��arr���Զ���װ�ؼĴ������ڵ�ֵ��psc��ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
 * ���  ����
 */ 
void TIM3_PWM_In_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM3_ICInitStructure;

  NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);													//ʹ��TIM3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  											  //ʹ��GPIOBʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;  												
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           										
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);

	//��ʼ��TIM3	 
	TIM_TimeBaseStructure.TIM_Period = arr;    																		 //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 																		 //Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 											 //����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 									 //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                                //��ʼ��TIMx��ʱ�������λ
  
	//��ʼ��TIM3ͨ��1���벶�����
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1 ; 														 //ѡ������� IC1ӳ�䵽TI1��
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;									 //�����ز���
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 							 //ӳ�䵽TI1��
 	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	                       //���������Ƶ,����Ƶ 
 	TIM3_ICInitStructure.TIM_ICFilter = 0x00;																			 //���������˲���,���˲�
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);
//��ʼ��TIM3ͨ��2���벶�����
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2 ; 														 //ѡ������� IC1ӳ�䵽TI1��
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;									 //�����ز���
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 							 //ӳ�䵽TI1��
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 											 //���������Ƶ,����Ƶ 
  TIM3_ICInitStructure.TIM_ICFilter = 0x00;																			 //���������˲��� ���˲�
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
		
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn  ;  														 //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  										 //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  												 	 //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 															 //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  																							 //��ʼ������NVIC�Ĵ��� 

	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2,ENABLE);								 //��������ж� ,����CC1IE�����ж�	
	
 	TIM_Cmd(TIM3,ENABLE ); 																												 //ʹ�ܶ�ʱ��3
}
