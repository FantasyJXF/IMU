/*********BlackHole1 For Study**************
 *	�ļ���					:				usart.c
 *	����					: 	 ���ڳ�ʼ���ļ�
 *	�汾					:	BlackHole1_For_Study_V1
 *	����					:	 	��INF�������		
 *	������̳				:		 www.infcre.com
 *	QQȺ					:   	  168590475
 *  Դ��״̬				:         ��Դ
 *			For infinite creativity
 *********(C) COPYRIGHT 2015 INF ************/

#include "usart.h"	  

/*
 * ��������USART3_Init
 * ����  ������3��ʼ������
 * ����  ��������
 * ���  ����
 */  	 
void USART3_Init(u32 bound)
{
	//GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																
	GPIO_Init(GPIOB, &GPIO_InitStructure); 																				
  
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;																		 //������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;										 //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;											   //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;														 //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode =  USART_Mode_Tx | USART_Mode_Rx;							 //�շ�ģʽ

  USART_Init(USART3, &USART_InitStructure); 																		 //��ʼ������

  USART_Cmd(USART3, ENABLE);                																		 //ʹ�ܴ��� 
}



//*****************************************************
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); 
    USART_SendData(USART3,(uint8_t)ch);   
	return ch;
}
#endif 
//*****************************************************

