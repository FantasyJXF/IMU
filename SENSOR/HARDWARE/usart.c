#include "STM32F4.h"	

void Uart_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 					//ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);					//ʹ��UART4ʱ��

	//����4��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4); 			//GPIOA0����ΪUART4
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4); 			//GPIOA1����ΪUART4

	//UART4�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 			//GPIOA0��GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;									//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 								//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 									//����
	GPIO_Init(GPIOA,&GPIO_InitStructure); 												//��ʼ��PA0��PA1

	//UART4 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;										//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;						//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(UART4, &USART_InitStructure); 										//��ʼ������4

	USART_Cmd(UART4, ENABLE);  																	//ʹ�ܴ���4

#if EN_UART4_RX	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);								//��������ж�

	//Uart4 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;							//����4�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;				//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;							//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);																//����ָ���Ĳ�����ʼ��NVIC�Ĵ�����
#endif	
}

//�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(UART4, (uint8_t) ch);
	
	/* �ȴ�������� */
	while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);		

	return (ch);
}
