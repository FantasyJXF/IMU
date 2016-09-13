/*
*********************************************************************************************************
*
*	ģ������ : ����������MPU-6000֮SPIģ��
*	�ļ����� : mpu6000_spi.c
*	˵    �� : ʵ��MPU-6000��SPI���ò�����
*********************************************************************************************************
*/

#include "STM32F4.h"

/*
*********************************************************************************************************
*	�� �� ��: IMU_CfgSpiHard
*	����˵��: ����STM32�ڲ�SPIӲ���Ĺ���ģʽ���ٶȵȲ��������ڷ��ʴ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void MPU6000_CfgSpiHard(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	/*
	STM32F4XX ʱ�Ӽ���.
		HCLK = 168M
		PCLK1 = HCLK / 4 = 42M
		PCLK2 = HCLK / 2 = 84M

		SPI2��SPI3 �� PCLK1, ʱ��42M
		SPI1       �� PCLK2, ʱ��84M

		STM32F4 ֧�ֵ����SPIʱ��Ϊ 37.5 Mbits/S, �����Ҫ��Ƶ������ʹ�õ���SPI1��
	*/
	/* ����SPIӲ������ */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  	/* ���ݷ���2��ȫ˫�� */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;													/* STM32��SPI����ģʽ ������ģʽ */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;											/* ����λ���� �� 8λ */
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;														//ʱ�����յ�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;													//���ݲ����ڵ�1��ʱ����
		/* SPI_CPOL��SPI_CPHA���ʹ�þ���ʱ�Ӻ����ݲ��������λ��ϵ��
	   ��������: ���߿����ǵ͵�ƽ,��1�����ز������ݡ�
	*/
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														   /* Ƭѡ���Ʒ�ʽ��������� */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;					
/*   MPU6000��SPI�������Ϊ1MHz
     ���ò�����Ԥ��Ƶϵ�� 84MHz / 256= 0.328125MHz
	   ����ѡ��SPI_BaudRatePrescaler_8���׳���
		 ����84MHz��ָͨ��AHB���߷�Ƶ��APB2���֧��84MHz��APB1���֧��42MHz
	*/
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;													//���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;																		//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure); 																			  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	
	SPI_Cmd(SPI1, ENABLE);				                                              /* ʹ��SPI  */
}
	
/*
 * ��������SPI1_Init
 * ����  ��SPI1��ʼ��
 * ����  ����
 * ���  ����
 */ 
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//  SPI_InitTypeDef  SPI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC,ENABLE);	//GPIOA|Cʱ��ʹ�� 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE );												//SPI1ʱ��ʹ�� 	
	
	  /*!< ���� MPU6000 ����: CS --> PC2 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//�������Ÿ���
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
	
	/*!< ���� MPU6000 ����: SCK  --> PA5 */
	/*!< ���� MPU6000 ����: MISO --> PA6*/
	/*!< ���� MPU6000 ����: MOSI --> PA7 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��
	
  MPU6000_CfgSpiHard();

//	MPU6000_CS(1); 			 																											  //MPU6000Ƭѡȡ��
//	GPIO_SetBits(GPIOA,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);  									//PA5|6|7����
//	
//	SPI_Cmd(SPI1,DISABLE);  																										//SPI1ʧ��
	
//	/* MPU6000 SPI ģʽ���� */
//  // MPU6000 ֧��SPIģʽ0��ģʽ3���ݴ�����CPOL CPHA
//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  				//SPI����Ϊ˫��˫��ȫ˫��
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;																//SPI����
//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;														//���ͽ���8λ֡�ṹ
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;																	//ʱ�����յ�
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;																//���ݲ����ڵ�1��ʱ����
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;																		//NSS�ź����������
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;					
///*   MPU6000��SPI�������Ϊ1MHz
//     ���ò�����Ԥ��Ƶϵ�� 84MHz / 256= 0.328125MHz
//	   ����ѡ��SPI_BaudRatePrescaler_8���׳���
//		 ����84MHz��ָͨ��AHB���߷�Ƶ��APB2���֧��84MHz��APB1���֧��42MHz
//	*/
//	
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;													//���ݴ����MSBλ��ʼ
//	SPI_InitStructure.SPI_CRCPolynomial = 7;																		//CRCֵ����Ķ���ʽ
//	SPI_Init(SPI1, &SPI_InitStructure); 																			  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	
//	SPI_Cmd(SPI1,ENABLE);																												//ʹ��SPI1
	
	//SPI1_Read_Write_Byte(0xff);																									//��������	 
	
	/* ��������CS���ţ�NRF�������״̬ */
	MPU6000_CS(1);  
}


/*
 * ��������MPU6000_Write_Reg
 * ����  ��������MPU6000�ض��ļĴ���д������
 * ����  ��reg:ָ���ļĴ�����ַ��value��д���ֵ
 * ���  ��status������״ֵ̬
 */ 
u8 MPU6000_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;
	MPU6000_CS(0);  										//ʹ��SPI����
	status = SPI1_Read_Write_Byte(reg); //����д����+�Ĵ�����
	SPI1_Read_Write_Byte(value);				//д��Ĵ���ֵ
	MPU6000_CS(1);  										//��ֹMPU6000
	return(status);											//����״ֵ̬
}


/*
 * ��������MPU6000_Read_Reg
 * ����  ��SPI��ȡ�Ĵ���
 * ����  ��reg:ָ���ļĴ�����ַ
 * ���  ��reg_val���Ĵ����е�����
 */ 
u8 MPU6000_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
	MPU6000_CS(0);  										   //ʹ��SPI����
//	SPI1_Read_Write_Byte(reg | 0x80); 	   //���Ͷ�����+�Ĵ�����
	SPI1_Read_Write_Byte(reg);
	//reg_val = SPI1_Read_Write_Byte(0xff);  //��ȡ�Ĵ���ֵ
	reg_val = SPI1_Read_Write_Byte(NOP);
	MPU6000_CS(1);  									     //��ֹMPU6000
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
	u16 received;
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
	received=SPI_I2S_ReceiveData(SPI1);
	return received;
//	return SPI_I2S_ReceiveData(SPI1); 															//����ͨ��SPIx������յ�����					    
}

