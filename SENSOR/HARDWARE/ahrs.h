#ifndef __AHRS_H
#define __AHRS_H

#define SAMPLE_HALF_T 0.0005f     // 采样周期的一半,单位:s   采样频率为1KHz，采样周期0.001s
#define FILTER_LENGTH 20					// 华东滤波窗口长度

#define GYRO_G 	0.0610351f				// 角速度变成度/S   此参数对应陀螺2000度每秒  Gyro_G=1/16.384=0.06103515625
#define GYRO_GR	0.0010653f				// 角速度变成弧度/S	 此参数对应陀螺2000度每秒 GYRO_GR=0.06103515625/57.2958=0.0010652641

#define IMU_KP 1.5f     					// 比例
#define IMU_KI 0.0005f 						// 积分


typedef struct
{
	float X;
	float Y;
	float Z;
}S_FLOAT_XYZ;

//extern volatile S_FLOAT_XYZ Angle;

void Geographic_Coordinate_ReadValue(void);
void Prepare_Data(void);
void IMU_Update(void);
//void AHRS_Date_Init(void);

#endif


