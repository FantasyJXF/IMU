/*
*********************************************************************************************************
*
*	ģ������ : MPU9250��SPI����ģ��
*	�ļ����� : mpu9250_spi.c
*	˵    �� : ʵ�ִ�������SPI���ò�����
*********************************************************************************************************
*/

#include "STM32F4.h"
	
/*
 * ��������SPI1_Init
 * ����  ��SPI1��ʼ��
 * ����  ����
 * ���  ����
 */ 
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC,ENABLE);	//GPIOA|Cʱ��ʹ�� 
	
		/*!< ���� MPU9250 ����: CS --> PC2 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//�������Ÿ���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE );												//SPI1ʱ��ʹ�� 	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
	
	/*!< ���� MPU9250 ����: SCK  --> PA5 */
	/*!< ���� MPU9250 ����: MISO --> PA6*/
	/*!< ���� MPU9250 ����: MOSI --> PA7 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;  //PA5|6|7����
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��	
	
	/* ���� SPI1����ģʽ */
	MPU9250_CfgSpiHard();

 /* ��������CS���ţ�MPU9250�������״̬ */
 // MPU9250_CS(1);  	    //��ֹSPI����
}	

/*
*********************************************************************************************************
*	�� �� ��: MPU9250_CfgSpiHard
*	����˵��: ����MPU9250�ڲ�SPIӲ���Ĺ���ģʽ���ٶȵȲ��������ڷ��ʴ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void MPU9250_CfgSpiHard(void)
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
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;					
/*   
     ���ò�����Ԥ��Ƶϵ�� 84MHz / 16= 5.25MHz
	   ����ѡ��SPI_BaudRatePrescaler_8���׳���
		 ����84MHz��ָͨ��AHB���߷�Ƶ��APB2���֧��84MHz��APB1���֧��42MHz
	*/
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;													//���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;																		//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure); 		//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���																	  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	
	SPI_Cmd(SPI1, ENABLE);				                                              /* ʹ��SPI  */
	
  SPI1_Read_Write_Byte(0xff);																									//��������	 
}

/*
 * ��������MPU9250_Write_Reg
 * ����  ��������MPU9250�ض��ļĴ���д������
 * ����  ��reg:ָ���ļĴ�����ַ��value��д���ֵ
 * ���  ��status������״ֵ̬
 */ 
u8 MPU9250_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;
	MPU9250_CS(0);  										//ʹ��SPI����
	status = SPI1_Read_Write_Byte(reg); //����д����+�Ĵ�����
	SPI1_Read_Write_Byte(value);				//д��Ĵ���ֵ
	MPU9250_CS(1);  										//��ֹMPU9250
	return(status);											//����״ֵ̬
}


/*
 * ��������MPU9250_Read_Reg
 * ����  ��SPI��ȡ�Ĵ���
 * ����  ��reg:ָ���ļĴ�����ַ
 * ���  ��reg_val���Ĵ����е�����
 */ 
u8 MPU9250_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
  MPU9250_CS(0);  										   //ʹ��SPI����
	SPI1_Read_Write_Byte(reg | 0x80); 	   //���Ͷ�����+�Ĵ�����  reg��ַ+������
//	SPI1_Read_Write_Byte(reg);
	reg_val = SPI1_Read_Write_Byte(0xff);  //��ȡ�Ĵ���ֵ  ��������
  MPU9250_CS(1);  									     //��ֹMPU9250
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

/***************************************************************/
// MPU�ڲ�i2c д��
//I2C_SLVx_ADDR:  MPU9250_AK8963_ADDR
//I2C_SLVx_REG:   reg
//I2C_SLVx_Data out:  value
/***************************************************************/
//static void i2c_Mag_write(u8 reg,u8 value)
void i2c_Mag_write(u8 reg,u8 value)
{
	u16 j=500;
	MPU9250_Write_Reg(I2C_SLV0_ADDR ,MPU9250_AK8963_ADDR);//���ô����Ƶ�ַ,mode: write
	MPU9250_Write_Reg(I2C_SLV0_REG ,reg);//set reg addr
	MPU9250_Write_Reg(I2C_SLV0_DO ,value);//send value	
	while(j--);//�˴���ΪMPU�ڲ�I2C��ȡ�ٶȽ�������ʱ�ȴ��ڲ�д���
}
/***************************************************************/
// MPU�ڲ�i2c ��ȡ
//I2C_SLVx_ADDR:  MPU9250_AK8963_ADDR
//I2C_SLVx_REG:   reg
//return value:   EXT_SENS_DATA_00 register value
/***************************************************************/
//static u8 i2c_Mag_read(u8 reg)
u8 i2c_Mag_read(u8 reg)
{
	u16 j=5000;
	MPU9250_Write_Reg(I2C_SLV0_ADDR ,MPU9250_AK8963_ADDR|0x80); //���ô����Ƶ�ַ��mode��read
	MPU9250_Write_Reg(I2C_SLV0_REG ,reg);// set reg addr
	MPU9250_Write_Reg(I2C_SLV0_DO ,0xff);//read
	while(j--);//�˴���ΪMPU�ڲ�I2C��ȡ�ٶȽ�����������ʱ�ȴ��ڲ���ȡ���
	return MPU9250_Read_Reg(EXT_SENS_DATA_00);
}

