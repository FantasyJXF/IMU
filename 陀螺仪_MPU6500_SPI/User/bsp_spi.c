/*********BlackHole1 For Study**************
 *	�ļ���					:			mpu6500_spi.c
 *	����					:  mpu6500��SPI�˿ڳ�ʼ��
 *	�汾					:	BlackHole1_For_Study_V1
 *	����					:	 	��INF�������		
 *	������̳				:		 www.infcre.com
 *	QQȺ					:   	  168590475
 *  Դ��״̬				:         ��Դ
 *			For infinite creativity
 *********(C) COPYRIGHT 2015 INF ************/

#include "bsp_spi.h"
#include "stm32f10x.h"

/*
 * ��������SPI1_Init
 * ����  ��SPI1��ʼ��
 * ����  ����
 * ���  ����
 */ 
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	//GPIOAʱ��ʹ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE );												//SPI1ʱ��ʹ�� 	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 																	//MPU6500Ƭѡ�ź�
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 											
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);																

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	MPU6500_CS(1); 			 																											//MPU6500Ƭѡȡ��
	GPIO_SetBits(GPIOA,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);  									//PA5/6/7����
	
	SPI_Cmd(SPI1,DISABLE);  																										//SPI1ʧ��
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  				//SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;																//SPI����
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;														//���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;																	//ʱ�����յ�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;																//���ݲ����ڵ�1��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;																		//NSS�ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;					//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16��72M/8=9M��
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;													//���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;																		//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure); 																			  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	
	SPI_Cmd(SPI1,ENABLE);																												//ʹ��SPI1
	SPI1_Read_Write_Byte(0xff);																									//��������	 
}


/*
 * ��������MPU6500_Write_Reg
 * ����  ��SPIд��Ĵ���
 * ����  ��reg:ָ���ļĴ�����ַ��value��д���ֵ
 * ���  ��status������״ֵ̬
 */ 
u8 MPU6500_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;
	MPU6500_CS(0);  										//ʹ��SPI����
	status = SPI1_Read_Write_Byte(reg); //����д����+�Ĵ�����
	SPI1_Read_Write_Byte(value);				//д��Ĵ���ֵ
	MPU6500_CS(1);  										//��ֹMPU6500
	return(status);											//����״ֵ̬
}


/*
 * ��������MPU6500_Read_Reg
 * ����  ��SPI��ȡ�Ĵ���
 * ����  ��reg:ָ���ļĴ�����ַ
 * ���  ��reg_val��reg�Ĵ�����ַ��Ӧ��ֵ
 */ 
u8 MPU6500_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
	MPU6500_CS(0);  										//ʹ��SPI����
	SPI1_Read_Write_Byte(reg | 0x80); 	//���Ͷ�����+�Ĵ�����
	reg_val = SPI1_Read_Write_Byte(0xff); //��ȡ�Ĵ���ֵ
	MPU6500_CS(1);  									  //��ֹMPU6500
	return(reg_val);
}

/*
 * ��������SPI1_Read_Write_Byte
 * ����  ����дһ���ֽ�
 * ����  ��TxData:Ҫд����ֽ�
 * ���  ����ȡ�����ֽ�
 */ 
u8 SPI1_Read_Write_Byte(uint8_t TxData)
{		
	u8 retry = 0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) 	//���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry > 250)	return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); 																//ͨ������SPIx����һ������
	retry = 0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry > 250) return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI1); 															//����ͨ��SPIx������յ�����					    
}

