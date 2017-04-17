/*********BlackHole1 For Study**************
 *	文件名					:			 control.c
 *	描述					:  	  电机控制算法
 *	版本					:	BlackHole1_For_Study_V1
 *	作者					:	 	【INF】无穷创新		
 *	技术论坛				:		 www.infcre.com
 *	QQ群					:   	  168590475
 *  源码状态				:         开源
 *			For infinite creativity
 *********(C) COPYRIGHT 2015 INF ************/

/****************************
					 Y(Roll)
     顺时针转 | 逆时针转
      motor1  |  motor4
       正桨   |   反桨
    --------------------X(Pitch)          
     逆时针转 | 顺时针转 
      motor2  |  motor3
       反桨   |   正桨 
              |
****************************/

#include "control.h"
#include "telecontrol.h"

volatile u16 motor[5];
vs16 throttle = 0;

PID PID_Roll_Angle,PID_Pitch_Angle,PID_Yaw_Angle;	//外环：角度PID环
PID PID_Roll_Rate,PID_Pitch_Rate,PID_Yaw_Rate;		//内环：角速度PID环
S_FLOAT_XYZ Exp_Angle;

extern u8 unlock_flag;//解锁标志位，为0未解锁，为1解锁	
extern volatile float angle_z;

/*
 * 函数名：Control
 * 描述  ：根据姿态信息控制四旋翼
 * 输入  ：Now_Angle：当前姿态；Exp_Angle：期望姿态
 * 输出  ：无
 */ 
void Control(S_FLOAT_XYZ	Now_Angle, S_FLOAT_XYZ Exp_Angle)
{	
	//期望角+测量角，两者符号相反，实质为偏差
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
	
	if((throttle	>	LAUNCH_THROTTLE) && (unlock_flag) && (Is_WFLY_Connected()))//当油门大于100，解锁了，且遥控器连接正常情况下
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
参数整定找最佳，从小到大顺序查
先是比例后积分，最后再把微分加
曲线振荡很频繁，比例度盘要放大
曲线漂浮绕大湾，比例度盘往小扳
曲线偏离回复慢，积分时间往下降
曲线波动周期长，积分时间再加长
曲线振荡频率快，先把微分降下来
动差大来波动慢。微分时间应加长
理想曲线两个波，前高后低4比1
一看二调多分析，调节质量不会低
****************************/

/*
 * 函数名：PID_Init
 * 描述  ：PID参数初始化函数
 * 输入  ：无
 * 输出  ：无
 */ 
void PID_Init(void)
{
//角度环	
	PID_Roll_Angle.P = 14;
	PID_Roll_Angle.I = 0;
	PID_Roll_Angle.D = 0.25;
	
	PID_Pitch_Angle.P = 14;
	PID_Pitch_Angle.I = 0;
	PID_Pitch_Angle.D = 0.25;
	
	PID_Yaw_Angle.P = 20;
	PID_Yaw_Angle.I = 0;
	PID_Yaw_Angle.D = 1;
		
//角速度环	
	PID_Roll_Rate.P = 0;
	PID_Roll_Rate.I = 0;
	PID_Roll_Rate.D = 0;
	
	PID_Pitch_Rate.P = 0;
	PID_Pitch_Rate.I = 0;
	PID_Pitch_Rate.D = 0;
	
	PID_Yaw_Rate.P = 0;
	PID_Yaw_Rate.I = 0;
	PID_Yaw_Rate.D = 0;


//初始化清零
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
												
						

