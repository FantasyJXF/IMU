/*
*********************************************************************************************************
*
*	模块名称 : 三轴陀螺仪MPU-6000之SPI模块
*	文件名称 : mpu6000_spi.c
*	说    明 : 实现MPU-6000的SPI配置操作。
*********************************************************************************************************
*/

#include "STM32F4.h"

/*
*********************************************************************************************************
*	函 数 名: IMU_CfgSpiHard
*	功能说明: 配置STM32内部SPI硬件的工作模式、速度等参数，用于访问传感器
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void MPU6000_CfgSpiHard(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	/*
	STM32F4XX 时钟计算.
		HCLK = 168M
		PCLK1 = HCLK / 4 = 42M
		PCLK2 = HCLK / 2 = 84M

		SPI2、SPI3 在 PCLK1, 时钟42M
		SPI1       在 PCLK2, 时钟84M

		STM32F4 支持的最大SPI时钟为 37.5 Mbits/S, 因此需要分频。下面使用的是SPI1。
	*/
	/* 配置SPI硬件参数 */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  	/* 数据方向：2线全双工 */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;													/* STM32的SPI工作模式 ：主机模式 */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;											/* 数据位长度 ： 8位 */
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;														//时钟悬空低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;													//数据捕获于第1个时钟沿
		/* SPI_CPOL和SPI_CPHA结合使用决定时钟和数据采样点的相位关系、
	   这里配置: 总线空闲是低电平,第1个边沿采样数据。
	*/
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;														   /* 片选控制方式：软件控制 */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;					
/*   MPU6000的SPI最大速率为1MHz
     设置波特率预分频系数 84MHz / 256= 0.328125MHz
	   这里选择SPI_BaudRatePrescaler_8容易出错。
		 这里84MHz是指通过AHB总线分频，APB2最高支持84MHz，APB1最高支持42MHz
	*/
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;													//数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;																		//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure); 																			  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	
	SPI_Cmd(SPI1, ENABLE);				                                              /* 使能SPI  */
}
	
/*
 * 函数名：SPI1_Init
 * 描述  ：SPI1初始化
 * 输入  ：无
 * 输出  ：无
 */ 
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//  SPI_InitTypeDef  SPI_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC,ENABLE);	//GPIOA|C时钟使能 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE );												//SPI1时钟使能 	
	
	  /*!< 配置 MPU6000 引脚: CS --> PC2 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//设置引脚复用
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
	
	/*!< 配置 MPU6000 引脚: SCK  --> PA5 */
	/*!< 配置 MPU6000 引脚: MISO --> PA6*/
	/*!< 配置 MPU6000 引脚: MOSI --> PA7 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化
	
  MPU6000_CfgSpiHard();

//	MPU6000_CS(1); 			 																											  //MPU6000片选取消
//	GPIO_SetBits(GPIOA,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);  									//PA5|6|7上拉
//	
//	SPI_Cmd(SPI1,DISABLE);  																										//SPI1失能
	
//	/* MPU6000 SPI 模式配置 */
//  // MPU6000 支持SPI模式0及模式3，据此设置CPOL CPHA
//	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  				//SPI设置为双线双向全双工
//	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;																//SPI主机
//	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;														//发送接收8位帧结构
//	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;																	//时钟悬空低
//	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;																//数据捕获于第1个时钟沿
//	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;																		//NSS信号由软件控制
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;					
///*   MPU6000的SPI最大速率为1MHz
//     设置波特率预分频系数 84MHz / 256= 0.328125MHz
//	   这里选择SPI_BaudRatePrescaler_8容易出错。
//		 这里84MHz是指通过AHB总线分频，APB2最高支持84MHz，APB1最高支持42MHz
//	*/
//	
//	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;													//数据传输从MSB位开始
//	SPI_InitStructure.SPI_CRCPolynomial = 7;																		//CRC值计算的多项式
//	SPI_Init(SPI1, &SPI_InitStructure); 																			  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	
//	SPI_Cmd(SPI1,ENABLE);																												//使能SPI1
	
	//SPI1_Read_Write_Byte(0xff);																									//启动传输	 
	
	/* 用于拉高CS引脚，NRF进入空闲状态 */
	MPU6000_CS(1);  
}


/*
 * 函数名：MPU6000_Write_Reg
 * 描述  ：用于向MPU6000特定的寄存器写入数据
 * 输入  ：reg:指定的寄存器地址；value：写入的值
 * 输出  ：status：返回状态值
 */ 
u8 MPU6000_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;
	MPU6000_CS(0);  										//使能SPI传输
	status = SPI1_Read_Write_Byte(reg); //发送写命令+寄存器号
	SPI1_Read_Write_Byte(value);				//写入寄存器值
	MPU6000_CS(1);  										//禁止MPU6000
	return(status);											//返回状态值
}


/*
 * 函数名：MPU6000_Read_Reg
 * 描述  ：SPI读取寄存器
 * 输入  ：reg:指定的寄存器地址
 * 输出  ：reg_val：寄存器中的数据
 */ 
u8 MPU6000_Read_Reg(uint8_t reg)
{
	uint8_t reg_val;
	MPU6000_CS(0);  										   //使能SPI传输
//	SPI1_Read_Write_Byte(reg | 0x80); 	   //发送读命令+寄存器号
	SPI1_Read_Write_Byte(reg);
	//reg_val = SPI1_Read_Write_Byte(0xff);  //读取寄存器值
	reg_val = SPI1_Read_Write_Byte(NOP);
	MPU6000_CS(1);  									     //禁止MPU6000
	return(reg_val);
}

/*
 * 函数名：SPI1_Read_Write_Byte
 * 描述  ：读写一个字节
 * 输入  ：TxData:要写入的字节
 * 输出  ：读取到的字节
 */ 
u8 SPI1_Read_Write_Byte(uint8_t TxData)
{		
	u8 retry = 0;				 
	u16 received;
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) 	//检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry > 250)	return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); 																//通过外设SPIx发送一个数据
	retry = 0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry > 250) return 0;
	}	  						    
	received=SPI_I2S_ReceiveData(SPI1);
	return received;
//	return SPI_I2S_ReceiveData(SPI1); 															//返回通过SPIx最近接收的数据					    
}

