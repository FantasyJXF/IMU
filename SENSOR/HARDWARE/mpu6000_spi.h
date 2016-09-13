/*
*********************************************************************************************************
*
*	ģ������ : ����������MPU-6000����ģ��
*	�ļ����� : mpu6000.h
*	˵    �� : ͷ�ļ�
* ��    �� ��Fantasy
*********************************************************************************************************
*/

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                         MPU6000 SPI Interface                              //
//      SCK  --> PA5                                                          //
//      MISO --> PA6                                                          //
//      MOSI --> PA7                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifndef __MPU6000_SPI_H
#define __MPU6000_SPI_H
#include "STM32F4.h"


#define MPU6000_CS(X)			(X==0)?GPIO_ResetBits(GPIOC,GPIO_Pin_2):GPIO_SetBits(GPIOC,GPIO_Pin_2) //MPU6000Ƭѡ�ź�
#define NOP         	0xFF  // Define No Operation, might be used to read status register

extern void SPI1_Init(void);

extern u8 MPU6000_Write_Reg(uint8_t reg,uint8_t value);
extern u8 MPU6000_Read_Reg(uint8_t reg);
extern u8 SPI1_Read_Write_Byte(uint8_t TxData);//SPI���߶�дһ���ֽ�


#endif










