#ifndef _MPU6500_H
#define _MPU6500_H

#include "stm32f10x.h"
#include "bsp_spi.h"

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

extern S_INT16_XYZ MPU6500_Acc;
extern S_INT16_XYZ MPU6500_Gyro;
extern S_INT16_XYZ	MPU6500_Acc_Offset;		
extern float MPU6500_tempreature;
// 定义MPU6500内部地址
/*****************************************************************/
#define	SMPLRT_DIV		                      0x19	//陀螺仪采样率
#define	CONFIG			                        0x1A	//低通滤波器  典型值0x06 5hz
#define	GYRO_CONFIG		                      0x1B	//陀螺仪测量范围 0X18 正负2000度
#define	ACCEL_CONFIG	                      0x1C	//加速度计测量范围 0X18 正负16g
#define	ACCEL_CONFIG_2                      0x1D  //加速度计低通滤波器 0x06 5hz

#define INT_PIN_CFG                         0x37 //中断配置
#define USER_CTRL                           0x6a

//--------------------9axis  reg addr-----------------------//
#define	ACCEL_XOUT_H	0x3B //加速度计输出数据
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41   //temperture
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43 //陀螺仪输出数据
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

//--------------------other reg addr-----------------------//
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I		  0x75	//ID地址寄存器(正确数值0x70，只读)

extern void MPU6500_Init(void);
//void READ_MPU6500_ACCEL(void);//读取加速度
//void READ_MPU6500_GYRO(void);//读取陀螺仪
extern void MPU6500_ReadValue(void);
void MPU6500_Date_Offset(u16 cnt);

#endif



