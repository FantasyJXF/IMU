#include "STM32F4.h"	

void Uart_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 					//使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);					//使能UART4时钟

	//串口4对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4); 			//GPIOA0复用为UART4
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4); 			//GPIOA1复用为UART4

	//UART4端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 			//GPIOA0与GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;									//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 								//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 									//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); 												//初始化PA0，PA1

	//UART4 初始化设置
	USART_InitStructure.USART_BaudRate = bound;										//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;						//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART4, &USART_InitStructure); 										//初始化串口4

	USART_Cmd(UART4, ENABLE);  																	//使能串口4

#if EN_UART4_RX	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);								//开启相关中断

	//Uart4 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;							//串口4中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;				//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;							//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);																//根据指定的参数初始化NVIC寄存器、
#endif	
}

//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口 */
	USART_SendData(UART4, (uint8_t) ch);
	
	/* 等待发送完毕 */
	while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);		

	return (ch);
}
