/*
*********************************************************************************************************
*
*	ģ������ : ����������MPU-9250����ģ��
*	�ļ����� : mpu9250_spi.h
*	˵    �� : ͷ�ļ�
* ��    �� ��Fantasy
*********************************************************************************************************
*/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                         MPU9250 SPI Interface                              //
//      SCK  --> PA5                                                          //
//      MISO --> PA6                                                          //
//      MOSI --> PA7                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef __MPU9250_SPI_H
#define __MPU9250_SPI_H
#include "STM32F4.h"


#define MPU9250_CS(X)			(X==0)?GPIO_ResetBits(GPIOC,GPIO_Pin_2):GPIO_SetBits(GPIOC,GPIO_Pin_2) //MPU9250Ƭѡ�ź�

///* ���Ų��������üĴ�������ʽ���ӿ�����ٶ� */
//#define MPU9250_CS_1()    MPU9250_PORT_CSN->BSRRL = GPIO_Pin_2
//#define MPU9250_CS_0()    MPU9250_PORT_CSN->BSRRH = GPIO_Pin_2


extern void SPI1_Init(void);
extern u8 MPU9250_Write_Reg(uint8_t reg,uint8_t value);
extern u8 MPU9250_Read_Reg(uint8_t reg);
extern u8 SPI1_Read_Write_Byte(uint8_t TxData);//SPI���߶�дһ���ֽ�
extern void i2c_Mag_write(u8 reg,u8 value);
extern u8 i2c_Mag_read(u8 reg);
void MPU9250_CfgSpiHard(void);

#endif










