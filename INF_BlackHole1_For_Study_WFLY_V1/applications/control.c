/*********BlackHole1 For Study**************
 *	�ļ���					:			 control.c
 *	����					:  	  ��������㷨
 *	�汾					:	BlackHole1_For_Study_V1
 *	����					:	 	��INF�������		
 *	������̳				:		 www.infcre.com
 *	QQȺ					:   	  168590475
 *  Դ��״̬				:         ��Դ
 *			For infinite creativity
 *********(C) COPYRIGHT 2015 INF ************/

/****************************
					 Y(Roll)
     ˳ʱ��ת | ��ʱ��ת
      motor1  |  motor4
       ����   |   ����
    --------------------X(Pitch)          
     ��ʱ��ת | ˳ʱ��ת 
      motor2  |  motor3
       ����   |   ���� 
              |
****************************/

#include "control.h"
#include "telecontrol.h"

volatile u16 motor[5];
vs16 throttle = 0;

PID PID_Roll_Angle,PID_Pitch_Angle,PID_Yaw_Angle;	//�⻷���Ƕ�PID��
PID PID_Roll_Rate,PID_Pitch_Rate,PID_Yaw_Rate;		//�ڻ������ٶ�PID��
S_FLOAT_XYZ Exp_Angle;

extern u8 unlock_flag;//������־λ��Ϊ0δ������Ϊ1����	
extern volatile float angle_z;

/*
 * ��������Control
 * ����  ��������̬��Ϣ����������
 * ����  ��Now_Angle����ǰ��̬��Exp_Angle��������̬
 * ���  ����
 */ 
void Control(S_FLOAT_XYZ	Now_Angle, S_FLOAT_XYZ Exp_Angle)
{	
	//������+�����ǣ����߷����෴��ʵ��Ϊƫ��
	float rol = Exp_Angle.Y + Now_Angle.Y;
	float pit = Exp_Angle.X + Now_Angle.X;
	float yaw = Exp_Angle.Z + angle_z;

	PID_Roll_Angle.Pout = PID_Roll_Angle.P * rol;
	PID_Pitch_Angle.Pout = PID_Pitch_Angle.P * pit;
	PID_Yaw_Angle.Pout = PID_Yaw_Angle.P * yaw;		
	
	PID_Pitch_Angle.Dout = PID_Pitch_Angle.D * MPU6500_Gyro.X;	
	PID_Roll_Angle.Dout = PID_Roll_Angle.D * MPU6500_Gyro.Y;
	PID_Yaw_Angle.Dout = PID_Yaw_Angle.D * MPU6500_Gyro.Z;
	
	PID_Roll_Angle.Out = PID_Roll_Angle.Pout + PID_Roll_Angle.Iout + PID_Roll_Angle.Dout;
	PID_Pitch_Angle.Out = PID_Pitch_Angle.Pout + PID_Pitch_Angle.Iout + PID_Pitch_Angle.Dout;
	PID_Yaw_Angle.Out = PID_Yaw_Angle.Pout + PID_Yaw_Angle.Iout + PID_Yaw_Angle.Dout;
	
	if((throttle	>	LAUNCH_THROTTLE) && (unlock_flag) && (Is_WFLY_Connected()))//�����Ŵ���100�������ˣ���ң�����������������
	{
		motor[1] = ADD_THROTTLE + throttle - PID_Roll_Angle.Out - PID_Pitch_Angle.Out - PID_Yaw_Angle.Out;
		motor[2] = ADD_THROTTLE + throttle - PID_Roll_Angle.Out + PID_Pitch_Angle.Out + PID_Yaw_Angle.Out;
		motor[3] = ADD_THROTTLE + throttle + PID_Roll_Angle.Out + PID_Pitch_Angle.Out - PID_Yaw_Angle.Out;
		motor[4] = ADD_THROTTLE + throttle + PID_Roll_Angle.Out - PID_Pitch_Angle.Out + PID_Yaw_Angle.Out;
	}
	else
	{
		motor[1]  = 0;
		motor[2]  = 0;
		motor[3]  = 0;
		motor[4]  = 0;
	}
	
	Motor_PWM_Update(motor[1] ,motor[2] ,motor[3] ,motor[4]);

}


/****************************
������������ѣ���С����˳���
���Ǳ�������֣�����ٰ�΢�ּ�
�����񵴺�Ƶ������������Ҫ�Ŵ�
����Ư���ƴ��壬����������С��
����ƫ��ظ���������ʱ�����½�
���߲������ڳ�������ʱ���ټӳ�
������Ƶ�ʿ죬�Ȱ�΢�ֽ�����
���������������΢��ʱ��Ӧ�ӳ�
����������������ǰ�ߺ��4��1
һ��������������������������
****************************/

/*
 * ��������PID_Init
 * ����  ��PID������ʼ������
 * ����  ����
 * ���  ����
 */ 
void PID_Init(void)
{
//�ǶȻ�	
	PID_Roll_Angle.P = 14;
	PID_Roll_Angle.I = 0;
	PID_Roll_Angle.D = 0.25;
	
	PID_Pitch_Angle.P = 14;
	PID_Pitch_Angle.I = 0;
	PID_Pitch_Angle.D = 0.25;
	
	PID_Yaw_Angle.P = 20;
	PID_Yaw_Angle.I = 0;
	PID_Yaw_Angle.D = 1;
		
//���ٶȻ�	
	PID_Roll_Rate.P = 0;
	PID_Roll_Rate.I = 0;
	PID_Roll_Rate.D = 0;
	
	PID_Pitch_Rate.P = 0;
	PID_Pitch_Rate.I = 0;
	PID_Pitch_Rate.D = 0;
	
	PID_Yaw_Rate.P = 0;
	PID_Yaw_Rate.I = 0;
	PID_Yaw_Rate.D = 0;


//��ʼ������
	PID_Roll_Angle.Pout = 0;
	PID_Roll_Angle.Iout = 0;
	PID_Roll_Angle.Dout = 0;
	
	PID_Pitch_Angle.Pout = 0;
	PID_Pitch_Angle.Iout = 0;
	PID_Pitch_Angle.Dout = 0;
	
	PID_Yaw_Angle.Pout = 0;
	PID_Yaw_Angle.Iout = 0;
	PID_Yaw_Angle.Dout = 0;
	
	PID_Roll_Rate.Pout = 0;
	PID_Roll_Rate.Iout = 0;
	PID_Roll_Rate.Dout = 0;
	
	PID_Pitch_Rate.Pout = 0;
	PID_Pitch_Rate.Iout = 0;
	PID_Pitch_Rate.Dout = 0;
	
	PID_Yaw_Rate.Pout = 0;
	PID_Yaw_Rate.Iout = 0;
	PID_Yaw_Rate.Dout = 0;
}
												
						

