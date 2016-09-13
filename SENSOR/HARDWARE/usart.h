#ifndef __USART_H
#define __USART_H

#include "STM32F4.h"  

#define EN_UART4_RX 				1						//使能（1）/禁止（0）串口2接收

void Uart_init(u32 bound);

#endif
