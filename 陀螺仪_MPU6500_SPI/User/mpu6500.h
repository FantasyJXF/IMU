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
// ����MPU6500�ڲ���ַ
/*****************************************************************/
#define	SMPLRT_DIV		                      0x19	//�����ǲ�����
#define	CONFIG			                        0x1A	//��ͨ�˲���  ����ֵ0x06 5hz
#define	GYRO_CONFIG		                      0x1B	//�����ǲ�����Χ 0X18 ����2000��
#define	ACCEL_CONFIG	                      0x1C	//���ٶȼƲ�����Χ 0X18 ����16g
#define	ACCEL_CONFIG_2                      0x1D  //���ٶȼƵ�ͨ�˲��� 0x06 5hz

#define INT_PIN_CFG                         0x37 //�ж�����
#define USER_CTRL                           0x6a

//--------------------9axis  reg addr-----------------------//
#define	ACCEL_XOUT_H	0x3B //���ٶȼ��������
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41   //temperture
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43 //�������������
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

//--------------------other reg addr-----------------------//
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I		  0x75	//ID��ַ�Ĵ���(��ȷ��ֵ0x70��ֻ��)

extern void MPU6500_Init(void);
//void READ_MPU6500_ACCEL(void);//��ȡ���ٶ�
//void READ_MPU6500_GYRO(void);//��ȡ������
extern void MPU6500_ReadValue(void);
void MPU6500_Date_Offset(u16 cnt);

#endif



