#include "stm32f4xx_it.h"

#if EN_USART1_RX																								//如果使能了串口接收

u8 Readbuf;

void USART1_IRQHandler(void)                										//串口1中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  				//接收中断
	{
		Readbuf =USART_ReceiveData(USART1);													//读取接收到的数据
		/*-   Add Ur Code    -*/
  } 
} 

#endif	
