/*********  For Study**************
 *	�ļ���				:		 mpu6000.c
 *	����					: 	 mpu6000�����ļ�
 *********(C) COPYRIGHT************/

#include "STM32F4.h"

u8	mpu6000_buf[14];					//spi��ȡMPU6000��������
//u8 offset_flag = 0;						//У׼ģʽ��־λ��Ϊ0δ����У׼��Ϊ1����У׼

S_INT16_XYZ	MPU6000_Acc_Offset	=	{0,0,0};		
S_INT16_XYZ	MPU6000_Gyro_Offset	=	{0,0,0};	
S_INT16_XYZ MPU6000_Acc = {0,0,0};
S_INT16_XYZ MPU6000_Gyro = {0,0,0};

float	mpu6000_tempreature = 0;
s16 mpu6000_tempreature_temp = 0;
s16 mpu6000_tempreature_Offset = 0;

///*
// * ��������MPU6000_Date_Offset
// * ����  ��MPU6000����У׼
// * ����  ��У׼����
// * ���  ����
// */ 
//void MPU6000_Date_Offset(u16 cnt)
//{
//	static S_INT32_XYZ Temp_Gyro , Temp_Acc;
//	int i = 0;
//	
//	Temp_Gyro.X =	0;
//	Temp_Gyro.Y =	0;
//	Temp_Gyro.Z =	0;
//	
//	Temp_Acc.X = 0;
//	Temp_Acc.Y = 0;
//	Temp_Acc.Z = 0;
//	
//	offset_flag = 1;//����MPU6000У׼ģʽ
//	for(i = cnt; i > 0; i--)
//	{
//    MPU6000_ReadValue();
//		
//		Temp_Acc.X	+=	MPU6000_Acc.X;
//		Temp_Acc.Y	+=	MPU6000_Acc.Y;		
//		//Temp_Acc.Z	+=	MPU6000_Acc.Z;
//		Temp_Gyro.X	+=	MPU6000_Gyro.X;
//		Temp_Gyro.Y	+=	MPU6000_Gyro.Y;
//		Temp_Gyro.Z	+=	MPU6000_Gyro.Z;

//	}
//	
//	MPU6000_Acc_Offset.X 	=	Temp_Acc.X	/	cnt;
//	MPU6000_Acc_Offset.Y 	=	Temp_Acc.Y	/	cnt;
//	//MPU6000_Acc_Offset.Z  =	Temp_Acc.Z	/	cnt;	
//	MPU6000_Gyro_Offset.X	= Temp_Gyro.X	/	cnt;
//	MPU6000_Gyro_Offset.Y	= Temp_Gyro.Y	/	cnt;
//  MPU6000_Gyro_Offset.Z =	Temp_Gyro.Z	/	cnt;

//	offset_flag = 0;//�˳�MPU6000У׼ģʽ
//}


/*
 * ��������MPU6000_ReadValue
 * ����  ����ȡMPU6000ԭʼ����
 * ����  ����
 * ���  ����
 */ 
void MPU6000_ReadValue(void)
{
	uint8_t i;
	
	MPU6000_CS(0); 																	//ʹ��SPI����

	SPI1_Read_Write_Byte(ACCEL_XOUT_H|0x80); 				//�Ӽ��ٶȼƵļĴ�����ʼ���ж�ȡ�����Ǻͼ��ٶȼƵ�ֵ//���Ͷ�����+�Ĵ�����
	
	for(i	=	0;i	<	14;i++)														//һ����ȡ14�ֽڵ�����
	{
		mpu6000_buf[i]	=	SPI1_Read_Write_Byte(0xff);	//����0xff,��Ϊslave��ʶ��
	}	
//	if(offset_flag == 0)
//	{
		MPU6000_Acc.X = BYTE16(s16, mpu6000_buf[0],  mpu6000_buf[1]) - MPU6000_Acc_Offset.X;
		MPU6000_Acc.Y = BYTE16(s16, mpu6000_buf[2],  mpu6000_buf[3]) - MPU6000_Acc_Offset.Y;
		MPU6000_Acc.Z = BYTE16(s16, mpu6000_buf[4],  mpu6000_buf[5]);
		MPU6000_Gyro.X = BYTE16(s16, mpu6000_buf[8],  mpu6000_buf[9]) - MPU6000_Gyro_Offset.X;
		MPU6000_Gyro.Y = BYTE16(s16, mpu6000_buf[10],  mpu6000_buf[11]) - MPU6000_Gyro_Offset.Y;
		MPU6000_Gyro.Z = BYTE16(s16, mpu6000_buf[12],  mpu6000_buf[13]) - MPU6000_Gyro_Offset.Z;
		
		mpu6000_tempreature_temp	=	BYTE16(s16, mpu6000_buf[6],  mpu6000_buf[7]);
		mpu6000_tempreature	=	(float)(35000+((521+mpu6000_tempreature_temp)*100)/34); // ԭ����ĸΪ340�����ڷ���*100����������1000����
		mpu6000_tempreature = mpu6000_tempreature/1000;                             
		if(( -4	<	MPU6000_Gyro.X ) && (MPU6000_Gyro.X < 4) ) MPU6000_Gyro.X = 0;
		if(( -4	<	MPU6000_Gyro.Y ) && (MPU6000_Gyro.Y < 4) ) MPU6000_Gyro.Y = 0;
		if(( -4	<	MPU6000_Gyro.Z ) && (MPU6000_Gyro.Z < 4) ) MPU6000_Gyro.Z = 0;
//	}
//	else if(offset_flag)  //MPU6000����У׼ģʽ
//	{
//		MPU6000_Acc.X = BYTE16(s16, mpu6000_buf[0],  mpu6000_buf[1]);
//		MPU6000_Acc.Y = BYTE16(s16, mpu6000_buf[2],  mpu6000_buf[3]);
//		MPU6000_Acc.Z = BYTE16(s16, mpu6000_buf[4],  mpu6000_buf[5]);
//		MPU6000_Gyro.X = BYTE16(s16, mpu6000_buf[8],  mpu6000_buf[9]);
//		MPU6000_Gyro.Y = BYTE16(s16, mpu6000_buf[10],  mpu6000_buf[11]);
//		MPU6000_Gyro.Z = BYTE16(s16, mpu6000_buf[12],  mpu6000_buf[13]);
//	}
	
	MPU6000_CS(1);  	    //��ֹSPI����
}

/*
 * ��������MPU6000_Init
 * ����  ��MPU6000��ʼ������
 * ����  ����
 * ���  ��0����ʼ��ʧ�� 1����ʼ���ɹ�
 */ 
u8 MPU6000_Init(void)
{
	u16 adress=0;
	u16 MGMT=0;
	u16 SIGNAL=0;
	SPI1_Init();																//MPU6000 IO�ں�SPI��ʼ��
	adress=MPU6000_Read_Reg(WHO_AM_I);
	if(MPU6000_Read_Reg(WHO_AM_I) == 0X00)			//��ȷ��ȡ��MPU6000�ĵ�ַ
	{		
		MPU6000_Write_Reg(PWR_MGMT_1,0X80);   		//��Դ����,��λMPU6000
	  MGMT=MPU6000_Read_Reg(PWR_MGMT_1);
		Delay_Ms(100);
		MPU6000_Write_Reg(SIGNAL_PATH_RESET,0X07);//�����ǡ����ٶȼơ��¶ȼƸ�λ
		SIGNAL=MPU6000_Read_Reg(SIGNAL_PATH_RESET);
		Delay_Ms(100);
		MPU6000_Write_Reg(PWR_MGMT_1,0X01);   		//ѡ��ʱ��Դ
		MPU6000_Write_Reg(PWR_MGMT_2,0X00);   		//ʹ�ܼ��ٶȼƺ�������
		MPU6000_Write_Reg(CONFIG,0X02);						//��ͨ�˲��� 0x02 92hz (3.9ms delay) fs=1khz
		MPU6000_Write_Reg(SMPLRT_DIV,0X00);				//������1000/(1+0)=1000HZ
		MPU6000_Write_Reg(GYRO_CONFIG,0X18);  		//�����ǲ�����Χ 0X18 ����2000��
		MPU6000_Write_Reg(ACCEL_CONFIG,0x10); 		//���ٶȼƲ�����Χ 0X00 ����8g
		
		return 1;
	}
	else return 0;
}

void MPU6000_Init(void)
{
	SPI1_Init();																//MPU6000 IO�ں�SPI��ʼ��
	
	MPU6000_Write_Reg(PWR_MGMT_1,0X00);   		//��Դ����,��λMPU6000
	Delay_Ms(100);
	MPU6000_Write_Reg(SIGNAL_PATH_RESET,0X07);//�����ǡ����ٶȼơ��¶ȼƸ�λ
	Delay_Ms(100);
	MPU6000_Write_Reg(PWR_MGMT_1,0X01);   		//ѡ��ʱ��Դ
	MPU6000_Write_Reg(PWR_MGMT_2,0X00);   		//ʹ�ܼ��ٶȼƺ�������
	MPU6000_Write_Reg(CONFIG,0X02);						//��ͨ�˲��� 0x02 92hz (3.9ms delay) fs=1khz
	MPU6000_Write_Reg(SMPLRT_DIV,0X00);				//������1000/(1+0)=1000HZ
	MPU6000_Write_Reg(GYRO_CONFIG,0X18);  		//�����ǲ�����Χ 0X18 ����2000��
	MPU6000_Write_Reg(ACCEL_CONFIG,0x10); 		//���ٶȼƲ�����Χ 0X00 ����8g
}

