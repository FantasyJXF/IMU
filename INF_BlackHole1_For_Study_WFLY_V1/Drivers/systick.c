/*********BlackHole1 For Study**************
 *	�ļ���					:				systick.c
 *	����					: 	ϵͳ�δ��жϳ�ʼ��
 *	�汾					:	BlackHole1_For_Study_V1
 *	����					:	 	��INF�������		
 *	������̳				:		 www.infcre.com
 *	QQȺ					:   	  168590475
 *  Դ��״̬				:         ��Դ
 *			For infinite creativity
 *********(C) COPYRIGHT 2015 INF ************/

#include "systick.h"

/*
 * ��������SysTick_Init
 * ����  ������ϵͳ�δ�ʱ�� SysTick
 * ����  ����
 * ���  ����
 */
void Systick_Init(void)
{
	//SystemFrequency / 1000    1ms�ж�һ��
	//SystemFrequency / 500	    2ms�ж�һ��
	 
	if (SysTick_Config(SystemCoreClock / 1000))
	{ 
		while (1);
	}
}




