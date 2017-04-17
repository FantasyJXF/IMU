/*********BlackHole1 For Study**************
 *	�ļ���					:			 telecontrol.c
 *	����					:  	 ң�������ݴ����ļ�
 *	�汾					:	BlackHole1_For_Study_V1
 *	����					:	 	��INF�������		
 *	������̳				:		 www.infcre.com
 *	QQȺ					:   	  168590475
 *  Դ��״̬				:         ��Դ
 *			For infinite creativity
 *********(C) COPYRIGHT 2015 INF ************/

#include "telecontrol.h"
#include "ahrs.h"
#include "control.h"
#include "mpu6500.h"
#include "led.h"

u8 unlock_flag = 0 ;					//������־λ��Ϊ0δ������Ϊ1����	
extern vs16 throttle;
extern S_FLOAT_XYZ Exp_Angle;
extern u16 Rc_Channel_Val[6];

/*
 * ��������Is_WFLY_Unlock
 * ����  ���ж��Ƿ�ִ�н�������
 * ����  ����
 * ���  ��0��δ������1�������ɹ�
 */  
u8 Is_WFLY_Unlock(void)
{
	static u16	cnt_unlock = 0;
	
	if( !unlock_flag )	//���δ����
	{
		if((Rc_Channel_Val[0]	<	1200) && (Rc_Channel_Val[1] < 1200) )			//��ҡ�˴����½�
		{	
			if(((Rc_Channel_Val[2]	<	1200) && (Rc_Channel_Val[3]) > 1800) )//��ҡ�˴����½�
			{	
				cnt_unlock++;
				if( (cnt_unlock % 20) == 0)								 //���������У�ÿ��40ms��˸һ��
				{
					LED_LEFT_BACK_TOGGLE;
					LED_RIGHT_BACK_TOGGLE;
				}
			}
		}
		else cnt_unlock = 0;													 //���δ����������жϣ������ʱ���������
			
		if( cnt_unlock > 1000)												 //������������1�룬�����
			{
				cnt_unlock = 0;
				SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;//�ر��ж�
				AHRS_Date_Init(); 										 	 //ÿ�ν������ȳ�ʼ����������
				MPU6500_Date_Offset(5000);								 //У׼������
				SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk; //�����ж�
				unlock_flag = 1;													 //�ɹ�������־
				return 1;
			}	
		 else return 0;		
	}
	else return 1;//��������ˣ��򷵻�1
}

/*
 * ��������Is_WFLY_Unlock
 * ����  ���ж��Ƿ�ִ����������
 * ����  ����
 * ���  ��0�������ɹ���1��δ����
 */  
u8 Is_WFLY_Lock(void)
{
	static u16	cnt_lock = 0;
	if( unlock_flag )	//���������
	{
		if((Rc_Channel_Val[0]	> 1800) && (Rc_Channel_Val[1] < 1200) )//��ҡ�˴����½�
		{		
				cnt_lock++;
				if( (cnt_lock % 20) == 0)//���������У�ÿ��40ms��˸һ��
				{
					LED_LEFT_BACK_TOGGLE;
					LED_RIGHT_BACK_TOGGLE;
				}
		}
		else cnt_lock = 0;		//���δ����������жϣ������ʱ���������
			
		if( cnt_lock == 500)	//������������500ms�������
			{
				cnt_lock = 0;
				unlock_flag = 0;	//�ɹ�������־
				return 1;
			}	
		 else return 0;		
	}
	else return 1;					//��������ˣ��򷵻�1
}

/*
 * ��������Is_WFLY_Connected
 * ����  ���ж�ң�����Ƿ���������
 * ����  ����
 * ���  ��0������ʧ�ܣ�1����������
 */  
u8 Is_WFLY_Connected(void)
{
	if((Rc_Channel_Val[0]	<	1100) && (Rc_Channel_Val[1] > 1900) && (Rc_Channel_Val[2]	>	1900) && (Rc_Channel_Val[3] > 1900))
  return 0;
	else return 1;
}


volatile float angle_z;
/*
 * ��������WFLY_PWM_Convert
 * ����  ��ң�����ź�ת��
 * ����  ����
 * ���  ����
 */ 
void WFLY_PWM_Convert(void)
{
	static u16 cnt;
	
	cnt++;
	
	Exp_Angle.X = (float)(Rc_Channel_Val[2]- 1500) * 0.07f;
	if((Exp_Angle.X > -2) && (Exp_Angle.X < 2))	Exp_Angle.X = 0;
	
	Exp_Angle.Y = (float)(Rc_Channel_Val[3]- 1500) * 0.07f;
	if((Exp_Angle.Y > -2) && (Exp_Angle.Y < 2))	Exp_Angle.Y = 0;
	
	throttle	=	(vs16)(Rc_Channel_Val[1] - 1000) * 2;
	if(cnt == 20)
	{
		cnt = 0;	
		if((Rc_Channel_Val[0] > 1600) || (Rc_Channel_Val[0] < 1400))//��ֹ�����ۼ����
		{
			if(throttle > LAUNCH_THROTTLE)	Exp_Angle.Z += -(float)(Rc_Channel_Val[0]- 1500) * 0.006f;
		}		
		if((Exp_Angle.Z > 180) && (Angle.Z > 0))
		{
			angle_z = Angle.Z - 360;
		}
		else if((Exp_Angle.Z < -180) && (Angle.Z < 0))
		{
			angle_z = Angle.Z + 360;
		}
		else angle_z = Angle.Z;
				
		if(Exp_Angle.Z > 360)  Exp_Angle.Z = (float)((s32)Exp_Angle.Z % 360);
		if(Exp_Angle.Z < -360) Exp_Angle.Z = (float)((s32)Exp_Angle.Z % - 360);		
	}
}
