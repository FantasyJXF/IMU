/*********BlackHole1 For Study**************
 *	�ļ���					:				main.c
 *	����					: �����򣬳�ʼ���Ĵ���
 *  ���뻷��				:			  MDK5.14
 *	�汾					:	BlackHole1_For_Study_V1
 *	����					:	 	��INF�������
 *	������				:			  2015.9.21								
 *	������̳				:		 www.infcre.com
 *	QQȺ					:   	  168590475
 *  Դ��״̬				:         ��Դ
 *			For infinite creativity
 *********(C) COPYRIGHT 2015 INF ************/

#include "main.h"

/*
 * ��������System_Init
 * ����  ��ϵͳ��ʼ������
 * ����  ����
 * ���  ����
 */  
void System_Init(void)
{
	LED_Init();													//LED״̬�Ƴ�ʼ��
	PID_Init();													//PID������ʼ��
	PWM_Out_Init(3999,71,0,0,0,0);	//PWM�����ʼ��
	USART3_Init(460800);								//���ڳ�ʼ��
	while(!MPU6500_Init());							//�ȴ�MPU6500��ʼ���ɹ�
	LED_StartShow();										//�������ã�һ����ʱ������״̬��ʾ
	Systick_Init();											//����1ms�ж�
	PWM_In_Init(0xffff,72	-	1);       	//�������벶�񣬶�ʱ����Ϊ1us
}

/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */  
int main(void)
{			
	System_Init();										//ϵͳ��ʼ��		
	while(1)													//��ѭ��
	{					
  }	 			  
}
		
		
		





#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
		/* User can add his own implementation to report the file name and line number,
			 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

		/* Infinite loop */
		while (1)
		{
		}
}

#endif

