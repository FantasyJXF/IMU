/*
*********************************************************************************************************
*
*	模块名称 : 三轴陀螺仪MPU-9250驱动模块
*	文件名称 : mpu9250_spi.h
*	说    明 : 头文件
* 作    者 ：Fantasy
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


#define MPU9250_CS(X)			(X==0)?GPIO_ResetBits(GPIOC,GPIO_Pin_2):GPIO_SetBits(GPIOC,GPIO_Pin_2) //MPU9250片选信号

///* 引脚操作，采用寄存器的形式，加快操作速度 */
//#define MPU9250_CS_1()    MPU9250_PORT_CSN->BSRRL = GPIO_Pin_2
//#define MPU9250_CS_0()    MPU9250_PORT_CSN->BSRRH = GPIO_Pin_2


extern void SPI1_Init(void);
extern u8 MPU9250_Write_Reg(uint8_t reg,uint8_t value);
extern u8 MPU9250_Read_Reg(uint8_t reg);
extern u8 SPI1_Read_Write_Byte(uint8_t TxData);//SPI总线读写一个字节
extern void i2c_Mag_write(u8 reg,u8 value);
extern u8 i2c_Mag_read(u8 reg);
void MPU9250_CfgSpiHard(void);

#endif










