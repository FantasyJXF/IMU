/*********BlackHole1 For Study**************
 *	文件名					:			 telecontrol.c
 *	描述					:  	 遥控器数据处理文件
 *	版本					:	BlackHole1_For_Study_V1
 *	作者					:	 	【INF】无穷创新		
 *	技术论坛				:		 www.infcre.com
 *	QQ群					:   	  168590475
 *  源码状态				:         开源
 *			For infinite creativity
 *********(C) COPYRIGHT 2015 INF ************/

#include "telecontrol.h"
#include "ahrs.h"
#include "control.h"
#include "mpu6500.h"
#include "led.h"

u8 unlock_flag = 0 ;					//解锁标志位，为0未解锁，为1解锁	
extern vs16 throttle;
extern S_FLOAT_XYZ Exp_Angle;
extern u16 Rc_Channel_Val[6];

/*
 * 函数名：Is_WFLY_Unlock
 * 描述  ：判断是否执行解锁动作
 * 输入  ：无
 * 输出  ：0：未解锁；1：解锁成功
 */  
u8 Is_WFLY_Unlock(void)
{
	static u16	cnt_unlock = 0;
	
	if( !unlock_flag )	//如果未解锁
	{
		if((Rc_Channel_Val[0]	<	1200) && (Rc_Channel_Val[1] < 1200) )			//左摇杆打到右下角
		{	
			if(((Rc_Channel_Val[2]	<	1200) && (Rc_Channel_Val[3]) > 1800) )//右摇杆打到左下角
			{	
				cnt_unlock++;
				if( (cnt_unlock % 20) == 0)								 //解锁过程中，每隔40ms闪烁一次
				{
					LED_LEFT_BACK_TOGGLE;
					LED_RIGHT_BACK_TOGGLE;
				}
			}
		}
		else cnt_unlock = 0;													 //如果未解锁或解锁中断，则解锁时间计数清零
			
		if( cnt_unlock > 1000)												 //解锁动作持续1秒，则解锁
			{
				cnt_unlock = 0;
				SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;//关闭中断
				AHRS_Date_Init(); 										 	 //每次解锁后都先初始化导航数据
				MPU6500_Date_Offset(5000);								 //校准陀螺仪
				SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk; //开启中断
				unlock_flag = 1;													 //成功解锁标志
				return 1;
			}	
		 else return 0;		
	}
	else return 1;//如果解锁了，则返回1
}

/*
 * 函数名：Is_WFLY_Unlock
 * 描述  ：判断是否执行上锁动作
 * 输入  ：无
 * 输出  ：0：上锁成功；1：未上锁
 */  
u8 Is_WFLY_Lock(void)
{
	static u16	cnt_lock = 0;
	if( unlock_flag )	//如果解锁了
	{
		if((Rc_Channel_Val[0]	> 1800) && (Rc_Channel_Val[1] < 1200) )//左摇杆打到左下角
		{		
				cnt_lock++;
				if( (cnt_lock % 20) == 0)//上锁过程中，每隔40ms闪烁一次
				{
					LED_LEFT_BACK_TOGGLE;
					LED_RIGHT_BACK_TOGGLE;
				}
		}
		else cnt_lock = 0;		//如果未解锁或解锁中断，则解锁时间计数清零
			
		if( cnt_lock == 500)	//解锁动作持续500ms，则解锁
			{
				cnt_lock = 0;
				unlock_flag = 0;	//成功上锁标志
				return 1;
			}	
		 else return 0;		
	}
	else return 1;					//如果解锁了，则返回1
}

/*
 * 函数名：Is_WFLY_Connected
 * 描述  ：判断遥控器是否正常连接
 * 输入  ：无
 * 输出  ：0：连接失败；1：正常连接
 */  
u8 Is_WFLY_Connected(void)
{
	if((Rc_Channel_Val[0]	<	1100) && (Rc_Channel_Val[1] > 1900) && (Rc_Channel_Val[2]	>	1900) && (Rc_Channel_Val[3] > 1900))
  return 0;
	else return 1;
}


volatile float angle_z;
/*
 * 函数名：WFLY_PWM_Convert
 * 描述  ：遥控器信号转换
 * 输入  ：无
 * 输出  ：无
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
		if((Rc_Channel_Val[0] > 1600) || (Rc_Channel_Val[0] < 1400))//防止死区累计误差
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
