#include "stm32f4xx_it.h"

#if EN_USART1_RX																								//���ʹ���˴��ڽ���

u8 Readbuf;

void USART1_IRQHandler(void)                										//����1�жϷ������
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  				//�����ж�
	{
		Readbuf =USART_ReceiveData(USART1);													//��ȡ���յ�������
		/*-   Add Ur Code    -*/
  } 
} 

#endif	
