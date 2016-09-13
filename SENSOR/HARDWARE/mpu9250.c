/*********  For Study**************
 *	�ļ���				:		 mpu9250.c
 *	����					: 	 mpu9250�����ļ�
 *********(C) COPYRIGHT************/

#include "STM32F4.h"

u8	mpu9250_buf[14];				//spi��ȡMPU9250��������
u8 offset_flag = 0;				   //У׼ģʽ��־λ��Ϊ0δ����У׼��Ϊ1����У׼

S_INT16_XYZ	MPU9250_Acc_Offset	=	{0,0,0};		
S_INT16_XYZ	MPU9250_Gyro_Offset	=	{0,0,0};	
S_INT16_XYZ MPU9250_Acc = {0,0,0};
S_INT16_XYZ MPU9250_Gyro = {0,0,0};

float	mpu9250_tempreature = 0;
s16 mpu9250_tempreature_temp = 0;
s16 mpu9250_tempreature_Offset = 0;

/*
 * ��������MPU9250_Date_Offset
 * ����  ��MPU9250����У׼
 * ����  ��У׼����
 * ���  ����
 */ 
void MPU9250_Date_Offset(u16 cnt)
{
	static S_INT32_XYZ Temp_Gyro , Temp_Acc;
	int i = 0;
	
	Temp_Gyro.X =	0;
	Temp_Gyro.Y =	0;
	Temp_Gyro.Z =	0;
	
	Temp_Acc.X = 0;
	Temp_Acc.Y = 0;
	Temp_Acc.Z = 0;
	
	offset_flag = 1;  //����MPU9250У׼ģʽ
	for(i = cnt; i > 0; i--)
	{
    MPU9250_ReadValue();
		
		Temp_Acc.X	+=	MPU9250_Acc.X;
		Temp_Acc.Y	+=	MPU9250_Acc.Y;		
		//Temp_Acc.Z	+=	MPU9250_Acc.Z;
		Temp_Gyro.X	+=	MPU9250_Gyro.X;
		Temp_Gyro.Y	+=	MPU9250_Gyro.Y;
		Temp_Gyro.Z	+=	MPU9250_Gyro.Z;

	}
	
	MPU9250_Acc_Offset.X 	=	Temp_Acc.X	/	cnt;
	MPU9250_Acc_Offset.Y 	=	Temp_Acc.Y	/	cnt;
	//MPU9250_Acc_Offset.Z  =	Temp_Acc.Z	/	cnt;	
	MPU9250_Gyro_Offset.X	= Temp_Gyro.X	/	cnt;
	MPU9250_Gyro_Offset.Y	= Temp_Gyro.Y	/	cnt;
	MPU9250_Gyro_Offset.Z =	Temp_Gyro.Z	/	cnt;

	offset_flag = 0;//�˳�MPU9250У׼ģʽ
}

/*
 * ��������MPU9250_Init
 * ����  ��MPU9250��ʼ������
 * ����  ����
 * ���  ��0����ʼ��ʧ�� 1����ʼ���ɹ�
 */ 
void MPU9250_Init(void)
{	
	u16 adress;
	adress=MPU9250_Read_Reg(WHO_AM_I);
	MPU9250_Write_Reg(PWR_MGMT_1, 0x00);	//�������״̬
	MPU9250_Write_Reg(CONFIG, 0x07);      //��ͨ�˲�Ƶ�ʣ�����ֵ��0x07(3600Hz)�˼Ĵ����ھ���Internal_Sample_Rate==8K
	
/**********************Init SLV0 i2c**********************************/	
//Use SPI-bus read slave0
	MPU9250_Write_Reg(INT_PIN_CFG ,0x30);// INT Pin / Bypass Enable Configuration  
	MPU9250_Write_Reg(I2C_MST_CTRL,0x4d);//I2C MAster mode and Speed 400 kHz
	MPU9250_Write_Reg(USER_CTRL ,0x20); // I2C_MST _EN 
	MPU9250_Write_Reg(I2C_MST_DELAY_CTRL ,0x01);//��ʱʹ��I2C_SLV0 _DLY_ enable 	
	MPU9250_Write_Reg(I2C_SLV0_CTRL ,0x81); //enable IIC	and EXT_SENS_DATA==1 Byte
	
/*******************Init GYRO and ACCEL******************************/	
	MPU9250_Write_Reg(SMPLRT_DIV, 0x07);  //�����ǲ����ʣ�����ֵ��0x07(1kHz) (SAMPLE_RATE= Internal_Sample_Rate / (1 + SMPLRT_DIV) )
	MPU9250_Write_Reg(GYRO_CONFIG, 0x18); //�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)   16.4LSB/(��/s)
	MPU9250_Write_Reg(ACCEL_CONFIG, 0x18);//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x18(���Լ죬16G)  2048/ g
	MPU9250_Write_Reg(ACCEL_CONFIG_2, 0x08);//���ټƸ�ͨ�˲�Ƶ�� ����ֵ ��0x08  ��1.13kHz��	
		
/**********************Init MAG **********************************/
	i2c_Mag_write(AK8963_CNTL2_REG,AK8963_CNTL2_SRST); // Reset AK8963
	i2c_Mag_write(AK8963_CNTL1_REG,0x12); // use i2c to set AK8963 working on Continuous measurement mode1 & 16-bit output	
}


////************************���ٶȶ�ȡ**************************/
//void READ_MPU9250_ACCEL(void)
//{ 

//   BUF[0]=MPU9250_Read_Reg(ACCEL_XOUT_L); 
//   BUF[1]=MPU9250_Read_Reg(ACCEL_XOUT_H);
//   mpu_value.Accel[0]=	(BUF[1]<<8)|BUF[0];
//   mpu_value.Accel[0]/=164; 						   //��ȡ����X������
//   BUF[2]=MPU9250_Read_Reg(ACCEL_YOUT_L);
//   BUF[3]=MPU9250_Read_Reg(ACCEL_YOUT_H);
//   mpu_value.Accel[1]=	(BUF[3]<<8)|BUF[2];
//   mpu_value.Accel[1]/=164; 						   //��ȡ����Y������
//   BUF[4]=MPU9250_Read_Reg(ACCEL_ZOUT_L); 
//   BUF[5]=MPU9250_Read_Reg(ACCEL_ZOUT_H);
//   mpu_value.Accel[2]=  (BUF[5]<<8)|BUF[4];
//   mpu_value.Accel[2]/=164; 					      //��ȡ����Z������ 
//	
//}
////**********************�����Ƕ�ȡ*****************************/
//void READ_MPU9250_GYRO(void)
//{ 

//   BUF[0]=MPU9250_Read_Reg(GYRO_XOUT_L); 
//   BUF[1]=MPU9250_Read_Reg(GYRO_XOUT_H);
//   mpu_value.Gyro[0]=	(BUF[1]<<8)|BUF[0];
//   mpu_value.Gyro[0]/=16.4; 						   //��ȡ����X������

//   BUF[2]=MPU9250_Read_Reg(GYRO_YOUT_L);
//   BUF[3]=MPU9250_Read_Reg(GYRO_YOUT_H);
//   mpu_value.Gyro[1]=	(BUF[3]<<8)|BUF[2];
//   mpu_value.Gyro[1]/=16.4; 						   //��ȡ����Y������
//   BUF[4]=MPU9250_Read_Reg(GYRO_ZOUT_L);
//   BUF[5]=MPU9250_Read_Reg(GYRO_ZOUT_H);
//   mpu_value.Gyro[2]=	(BUF[5]<<8)|BUF[4];
//   mpu_value.Gyro[2]/=16.4; 					       //��ȡ����Z������
//}


////**********************�����ƶ�ȡ***************************/
////i2c_Mag_read(AK8963_ST2_REG) �˲���ȡ����ʡ��
////���ݶ�ȡ�����Ĵ�����reading this register means data reading end
////AK8963_ST2_REG ͬʱ�������ݷ����������⹦��
////����ο� MPU9250 PDF
///**********************************************************/
//void READ_MPU9250_MAG(void)
//{ 	
//	u8 x_axis,y_axis,z_axis; 
//	
//	x_axis=i2c_Mag_read(AK8963_ASAX);// X�������ȵ���ֵ
//	y_axis=i2c_Mag_read(AK8963_ASAY);
//	z_axis=i2c_Mag_read(AK8963_ASAZ);
//	
//	if((i2c_Mag_read(AK8963_ST1_REG)&AK8963_ST1_DOR)==0)//data ready
//	{
//			//��ȡ����X������
//		 BUF[0]=i2c_Mag_read(MAG_XOUT_L); //Low data	
//		 if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register & check Magnetic sensor overflow occurred 
//		 {
//			 BUF[0]=i2c_Mag_read(MAG_XOUT_L);//reload data
//		 } 
//		 BUF[1]=i2c_Mag_read(MAG_XOUT_H); //High data	
//		 if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register
//		 {
//			 BUF[1]=i2c_Mag_read(MAG_XOUT_H);
//		 }
//		 mpu_value.Mag[0]=((BUF[1]<<8)|BUF[0])*(((x_axis-128)>>8)+1);		//�����Ⱦ��� ��ʽ��/RM-MPU-9250A-00 PDF/ 5.13	
//		 
//		//��ȡ����Y������
//			BUF[2]=i2c_Mag_read(MAG_YOUT_L); //Low data	
//		 if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register
//		 {
//			 BUF[2]=i2c_Mag_read(MAG_YOUT_L);
//		 }		 
//		 BUF[3]=i2c_Mag_read(MAG_YOUT_H); //High data	
//		 if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register
//		 {
//			 BUF[3]=i2c_Mag_read(MAG_YOUT_H);
//		 }
//		  mpu_value.Mag[1]=((BUF[3]<<8)|BUF[2])*(((y_axis-128)>>8)+1);	
//		 
//		//��ȡ����Z������
//		 BUF[4]=i2c_Mag_read(MAG_ZOUT_L); //Low data	
//		 if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register
//		 {
//			 BUF[4]=i2c_Mag_read(MAG_ZOUT_L);
//		 }	 
//		 BUF[5]=i2c_Mag_read(MAG_ZOUT_H); //High data	
//		 if((i2c_Mag_read(AK8963_ST2_REG)&AK8963_ST2_HOFL)==1)// data reading end register
//		 {
//			 BUF[5]=i2c_Mag_read(MAG_ZOUT_H);
//		 }
//		  mpu_value.Mag[2]=((BUF[5]<<8)|BUF[4])*(((z_axis-128)>>8)+1);	
//	}					       
//}

void MPU9250_ReadValue(void)
{
	uint8_t i;
	
	MPU9250_CS(0); 																	//ʹ��SPI����

	SPI1_Read_Write_Byte(ACCEL_XOUT_H|0x80); 				//�Ӽ��ٶȼƵļĴ�����ʼ���ж�ȡ�����Ǻͼ��ٶȼƵ�ֵ//���Ͷ�����+�Ĵ�����
	
	for(i	=	0;i	<	14;i++)														//һ����ȡ14�ֽڵ�����
	{
		mpu9250_buf[i]	=	SPI1_Read_Write_Byte(0xff);	//����0xff,��Ϊslave��ʶ��
	}	
	if(offset_flag == 0)
	{
		MPU9250_Acc.X = BYTE16(s16, mpu9250_buf[0],  mpu9250_buf[1]) - MPU9250_Acc_Offset.X;
		MPU9250_Acc.Y = BYTE16(s16, mpu9250_buf[2],  mpu9250_buf[3]) - MPU9250_Acc_Offset.Y;
		MPU9250_Acc.Z = BYTE16(s16, mpu9250_buf[4],  mpu9250_buf[5]);
		MPU9250_Gyro.X = BYTE16(s16, mpu9250_buf[8],  mpu9250_buf[9]) - MPU9250_Gyro_Offset.X;
		MPU9250_Gyro.Y = BYTE16(s16, mpu9250_buf[10],  mpu9250_buf[11]) - MPU9250_Gyro_Offset.Y;
		MPU9250_Gyro.Z = BYTE16(s16, mpu9250_buf[12],  mpu9250_buf[13]) - MPU9250_Gyro_Offset.Z;
		
		mpu9250_tempreature_temp	=	BYTE16(s16, mpu9250_buf[6],  mpu9250_buf[7]);
		mpu9250_tempreature	=	(float)(35000+((521+mpu9250_tempreature_temp)*100)/34); // ԭ����ĸΪ340�����ڷ���*100����������1000����
		mpu9250_tempreature = mpu9250_tempreature/1000;                             
		if(( -4	<	MPU9250_Gyro.X ) && (MPU9250_Gyro.X < 4) ) MPU9250_Gyro.X = 0;
		if(( -4	<	MPU9250_Gyro.Y ) && (MPU9250_Gyro.Y < 4) ) MPU9250_Gyro.Y = 0;
		if(( -4	<	MPU9250_Gyro.Z ) && (MPU9250_Gyro.Z < 4) ) MPU9250_Gyro.Z = 0;
	}
	else if(offset_flag)  //MPU9250����У׼ģʽ
	{
		MPU9250_Acc.X = BYTE16(s16, mpu9250_buf[0],  mpu9250_buf[1]);
		MPU9250_Acc.Y = BYTE16(s16, mpu9250_buf[2],  mpu9250_buf[3]);
		MPU9250_Acc.Z = BYTE16(s16, mpu9250_buf[4],  mpu9250_buf[5]);
		MPU9250_Gyro.X = BYTE16(s16, mpu9250_buf[8],  mpu9250_buf[9]);
		MPU9250_Gyro.Y = BYTE16(s16, mpu9250_buf[10],  mpu9250_buf[11]);
		MPU9250_Gyro.Z = BYTE16(s16, mpu9250_buf[12],  mpu9250_buf[13]);
	}
	
	MPU9250_CS(1);  	    //��ֹSPI����
}
