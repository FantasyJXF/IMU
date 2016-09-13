#ifndef _MPU6000_H
#define _MPU6000_H

#include "STM32F4.h"

#define BYTE16(Type, ByteH, ByteL)  ((Type)((((u16)(ByteH))<<8) | ((u16)(ByteL))))

typedef struct
{
	s16 X;
	s16 Y;
	s16 Z;
}S_INT16_XYZ;

typedef struct
{
	s32 X;
	s32 Y;
	s32 Z;
}S_INT32_XYZ;

extern S_INT16_XYZ MPU6000_Acc;
extern S_INT16_XYZ MPU6000_Gyro;
extern S_INT16_XYZ	MPU6000_Acc_Offset;		
extern float	mpu6000_tempreature;

//寄存器定义

#define SELF_TEST_X         0X0D
#define SELF_TEST_Y         0X0E
#define SELF_TEST_Z         0X0F
#define SELF_TEST_A         0X10

#define SMPLRT_DIV					0X19 //陀螺仪采样率
#define CONFIG							0X1A //低通滤波器  典型值0x06 5hz
#define GYRO_CONFIG					0X1B //陀螺仪测量范围 0X18 正负2000度
#define ACCEL_CONFIG				0X1C //加速度计测量范围 0X18 正负16g

#define MOT_THR							0X1F
#define FIFO_EN							0X23

#define ACCEL_XOUT_H				0X3B  //加速度计输出数据
#define ACCEL_XOUT_L				0X3C
#define ACCEL_YOUT_H				0X3D
#define ACCEL_YOUT_L				0X3E
#define ACCEL_ZOUT_H				0X3F
#define ACCEL_ZOUT_L				0X40

#define TEMP_OUT_H					0X41  //温度计输出数据
#define TEMP_OUT_L					0X42

#define GYRO_XOUT_H					0X43  //陀螺仪输出数据
#define GYRO_XOUT_L					0X44
#define GYRO_YOUT_H					0X45
#define GYRO_YOUT_L					0X46
#define GYRO_ZOUT_H					0X47
#define GYRO_ZOUT_L					0X48

#define SIGNAL_PATH_RESET   0X68 //陀螺仪、加速度计、温度传感器信号复位
#define USER_CTRL						0X6A //用户配置 当为0X10时使用SPI模式
#define PWR_MGMT_1					0X6B //电源管理1 典型值为0x00
#define PWR_MGMT_2					0X6C //电源管理2 典型值为0X00

#define WHO_AM_I						0X75 //器件ID MPU6000默认ID为0X68


void MPU6000_Date_Offset(u16 cnt);
extern void MPU6000_ReadValue(void);
//extern u8 MPU6000_Init(void);
extern void MPU6000_Init(void);

#endif



