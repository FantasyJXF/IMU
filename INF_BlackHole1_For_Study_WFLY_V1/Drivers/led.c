/*********BlackHole1 For Study**************
 *	�ļ���					:				led.c
 *	����					: 	led��ʼ���ļ�
 *	�汾					:	BlackHole1_For_Study_V1
 *	����					:	 	��INF�������		
 *	������̳				:		 www.infcre.com
 *	QQȺ					:   	  168590475
 *  Դ��״̬				:         ��Դ
 *			For infinite creativity
 *********(C) COPYRIGHT 2015 INF ************/

#include "led.h"
#include "delay.h"

/*
 * ��������LED_Init
 * ����  ��LED��GPIO�ڳ�ʼ��
 * ����  ����
 * ���  ����
 */  	 
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
}

/*
 * ��������LED_StartShow
 * ����  ����ʱ��״̬��ʾ
 * ����  ����
 * ���  ����
 */  	 
void LED_StartShow(void)
{
	int i = 0;
	
	for(; i<5 ; i++)
	{
		LED_LEFT_BACK(1);
		LED_RIGHT_BACK(0);
		Delay_Ms(200);
		LED_LEFT_BACK(0);
		LED_RIGHT_BACK(1);
		Delay_Ms(200);
	}
	
	LED_LEFT_BACK(1);
	LED_RIGHT_BACK(1);
}

