/********* For Study**************
 *	�ļ���				:				SysTick.c
 *	����					: 	ϵͳ�δ��жϳ�ʼ��
 *********(C) COPYRIGHT ************/

#include "STM32F4.h"

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
	
	/*
		����systic�ж�����Ϊ1ms��������systick�жϡ�

    	SystemCoreClock �ǹ̼��ж����ϵͳ�ں�ʱ�ӣ�����STM32F427,һ��Ϊ 168MHz

    	SysTick_Config() �������βα�ʾ�ں�ʱ�Ӷ��ٸ����ں󴥷�һ��Systick��ʱ�ж�.
	    	-- SystemCoreClock / 1000  ��ʾ��ʱƵ��Ϊ 1000Hz�� Ҳ���Ƕ�ʱ����Ϊ  1ms
	    	-- SystemCoreClock / 500   ��ʾ��ʱƵ��Ϊ 500Hz��  Ҳ���Ƕ�ʱ����Ϊ  2ms
	    	-- SystemCoreClock / 2000  ��ʾ��ʱƵ��Ϊ 2000Hz�� Ҳ���Ƕ�ʱ����Ϊ  500us

    	���ڳ����Ӧ�ã�����һ��ȡ��ʱ����1ms�����ڵ���CPU���ߵ͹���Ӧ�ã��������ö�ʱ����Ϊ 10ms
    */
//	SysTick_Config(SystemCoreClock / 1000);
}





