/*********BlackHole1 For Study**************
 *	文件名					:				main.c
 *	描述					: 主程序，初始化寄存器
 *  编译环境				:			  MDK5.14
 *	版本					:	BlackHole1_For_Study_V1
 *	作者					:	 	【INF】无穷创新
 *	最后更新				:			  2015.9.21								
 *	技术论坛				:		 www.infcre.com
 *	QQ群					:   	  168590475
 *  源码状态				:         开源
 *			For infinite creativity
 *********(C) COPYRIGHT 2015 INF ************/

#include "main.h"

/*
 * 函数名：System_Init
 * 描述  ：系统初始化函数
 * 输入  ：无
 * 输出  ：无
 */  
void System_Init(void)
{
	LED_Init();													//LED状态灯初始化
	PID_Init();													//PID参数初始化
	PWM_Out_Init(3999,71,0,0,0,0);	//PWM输出初始化
	USART3_Init(460800);								//串口初始化
	while(!MPU6500_Init());							//等待MPU6500初始化成功
	LED_StartShow();										//两个作用：一是延时，二是状态显示
	Systick_Init();											//开启1ms中断
	PWM_In_Init(0xffff,72	-	1);       	//开启输入捕获，定时精度为1us
}

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */  
int main(void)
{			
	System_Init();										//系统初始化		
	while(1)													//空循环
	{					
  }	 			  
}
		
		
		





#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
		/* User can add his own implementation to report the file name and line number,
			 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

		/* Infinite loop */
		while (1)
		{
		}
}

#endif

