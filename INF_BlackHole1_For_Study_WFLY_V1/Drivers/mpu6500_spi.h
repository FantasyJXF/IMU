#ifndef __MPU6500_SPI_H
#define __MPU6500_SPI_H

#include "stm32f10x.h"


#define MPU6500_CS(X)			(X==0)?GPIO_ResetBits(GPIOB,GPIO_Pin_0):GPIO_SetBits(GPIOB,GPIO_Pin_0) //MPU6500Ƭѡ�ź�

extern void SPI1_Init(void);

extern u8 MPU6500_Write_Reg(uint8_t reg,uint8_t value);
extern u8 MPU6500_Read_Reg(uint8_t reg);
extern u8 SPI1_Read_Write_Byte(uint8_t TxData);//SPI���߶�дһ���ֽ�


#endif