#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "stm32f10x.h"


#define MPU6500_CS(X)			(X==0)?GPIO_ResetBits(GPIOA,GPIO_Pin_4):GPIO_SetBits(GPIOA,GPIO_Pin_4) //MPU6500片选信号

extern void SPI1_Init(void);

extern u8 MPU6500_Write_Reg(uint8_t reg,uint8_t value);
extern u8 MPU6500_Read_Reg(uint8_t reg);
extern u8 SPI1_Read_Write_Byte(uint8_t TxData);//SPI总线读写


#endif
