/*
*********************************************************************************************************
*
*	模块名称 : RTC
*	文件名称 : rtc.c
*	说    明 : RTC底层驱动
*********************************************************************************************************
*/

#include "STM32F4.h"


#define RTC_Debug   /* 用于选择调试模式 */

/* 选择RTC的时钟源 */
#define RTC_CLOCK_SOURCE_LSE       /* LSE */
//#define RTC_CLOCK_SOURCE_LSI     /* LSI */ 

/* 变量 */
RTC_TimeTypeDef  RTC_TimeStructure;
RTC_InitTypeDef  RTC_InitStructure;
RTC_DateTypeDef  RTC_DateStructure;

RTC_TimeTypeDef  RTC_TimeStampStructure;
RTC_DateTypeDef  RTC_TimeStampDateStructure;

/* 用于设置RTC分频 */
__IO uint32_t uwAsynchPrediv = 0;
__IO uint32_t uwSynchPrediv = 0;
/*
*********************************************************************************************************
*	函 数 名: InitRTC
*	功能说明: 初始化RTC
*	形    参：无
*	返 回 值: 无		        
*********************************************************************************************************
*/
void nitRTC(void)
{
	
	/* 用于检测是否已经配置过RTC，如果配置过的话，会在配置结束时
	   设置RTC备份寄存器为0x32F2。如果检测RTC备份寄存器不是0x32F2
	   那么表示没有配置过，需要配置RTC.
	*/
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
	{  
		/* RTC 配置  */
		RTC_Config();
		
 		/* 打印调试信息 */
		#ifdef RTC_Debug
			printf("第一次使用RTC \n\r");
		#endif 	
		
		/* 检测上电复位标志是否设置 */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			/* 发生上电复位 */
			#ifdef RTC_Debug
				printf("发生上电复位 \n\r");
			#endif
		}
	}
	else
	{
		/* 打印调试信息 */
		#ifdef RTC_Debug
			printf("第n次使用RTC \n\r");
		#endif
		
		/* 检测上电复位标志是否设置 */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			/* 发生上电复位 */
			#ifdef RTC_Debug
				printf("发生上电复位 \n\r");
			#endif
		}
		/* 检测引脚复位标志是否设置 */
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			/* 发生引脚复位 */
			#ifdef RTC_Debug
			printf("发生引脚复位 \n\r");
			#endif			
		}

		/* 使能PWR时钟 */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

		/* 允许访问RTC */
		PWR_BackupAccessCmd(ENABLE);

		/* 等待 RTC APB 寄存器同步 */
		RTC_WaitForSynchro();
		
		/* 清除RTC闹钟标志 */
		RTC_ClearFlag(RTC_FLAG_ALRAF);

		/* 清除RTC闹钟中断挂起标志 */
		EXTI_ClearITPendingBit(EXTI_Line17);
	}
	
}
	
/*
*********************************************************************************************************
*	函 数 名: RTC_Config
*	功能说明: 用于配置时间戳功能
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RTC_Config(void)
{	
	/* 使能PWR时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* 允许访问RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* 选择LSI作为时钟源 */
#if defined (RTC_CLOCK_SOURCE_LSI)  
	
	/* Enable the LSI OSC */ 
	RCC_LSICmd(ENABLE);

	/* Wait till LSI is ready */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{
	}

	/* 选择RTC时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	/* ck_spre(1Hz) = RTCCLK(LSE) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)*/
	uwSynchPrediv = 0xFF;
	uwAsynchPrediv = 0x7F;
	
	/* 选择LSE作为RTC时钟 */
#elif defined (RTC_CLOCK_SOURCE_LSE)
	/* 使能LSE振荡器  */
	RCC_LSEConfig(RCC_LSE_ON);

	/* 等待就绪 */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
	}

	/* 选择RTC时钟源 */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	uwSynchPrediv = 0xFF;
	uwAsynchPrediv = 0x7F;

#else
#error Please select the RTC Clock source inside the main.c file
#endif 

	/* 使能RTC时钟 */
	RCC_RTCCLKCmd(ENABLE);

	/* 等待RTC APB寄存器同步 */
	RTC_WaitForSynchro();
	
	/* 使能时间戳 */
  RTC_TimeStampCmd(RTC_TimeStampEdge_Falling, ENABLE);  
	
	RTC_TimeStampPinSelection(RTC_TamperPin_PC13);	

	/* 配置RTC数据寄存器和分频器  */
	RTC_InitStructure.RTC_AsynchPrediv = uwAsynchPrediv;
	RTC_InitStructure.RTC_SynchPrediv = uwSynchPrediv;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	
	/* 检测RTC初始化 */
	if (RTC_Init(&RTC_InitStructure) == ERROR)
	{
		/* 打印调试信息 */
		#ifdef RTC_Debug
			printf("RTC初始化失败 \n\r");
		#endif
	}

	/* 设置年月日和星期 */
	RTC_DateStructure.RTC_Year = 0x13;
	RTC_DateStructure.RTC_Month = RTC_Month_January;
	RTC_DateStructure.RTC_Date = 0x11;
	RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Saturday;
	RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

	/* 设置时分秒，以及显示格式 */
	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours   = 0x05;
	RTC_TimeStructure.RTC_Minutes = 0x20;
	RTC_TimeStructure.RTC_Seconds = 0x00; 
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   
	
	/* 配置备份寄存器，表示已经设置过RTC */
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
}

/*
*********************************************************************************************************
*	函 数 名: RTC_TimeShow
*	功能说明: 显示时间
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RTC_TimeShow(void)
{
	/* 得到时间 */
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	printf("\n\r============== Current Time Display ============================\n\r");
	printf("\n\r  当前时间 :  %0.2d:%0.2d:%0.2d \n\r", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}

/*
*********************************************************************************************************
*	函 数 名: RTC_DateShow
*	功能说明: 显示时间
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RTC_DateShow(void)
{
	/* 得到日期 */
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	printf("\n\r============== Current Date Display ============================\n\r");
	printf("\n\r  当前日期 :  %0.2d-%0.2d-%0.2d-%0.2d \n\r", RTC_DateStructure.RTC_WeekDay, RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Year);
}

/*
*********************************************************************************************************
*	函 数 名: RTC_TimeStampShow
*	功能说明: 时间戳显示
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RTC_TimeStampShow(void)
{
	/* 得到当前时间戳 */
	RTC_GetTimeStamp(RTC_Format_BIN, &RTC_TimeStampStructure, &RTC_TimeStampDateStructure);
	printf("\n\r==============时间戳显示 (时间和日期)=================\n\r");
	printf("时间 :  %0.2d:%0.2d:%0.2d \n\r", RTC_TimeStampStructure.RTC_Hours, RTC_TimeStampStructure.RTC_Minutes, RTC_TimeStampStructure.RTC_Seconds);
	printf("日期 :  %0.2d-%0.2d-%0.2d \n\r", RTC_TimeStampDateStructure.RTC_WeekDay, RTC_TimeStampDateStructure.RTC_Date, RTC_TimeStampDateStructure.RTC_Month);
	printf("======================================================\n\r");
}

/*
*********************************************************************************************************
*	函 数 名: RTC_TimeRegulate
*	功能说明: 
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void RTC_TimeRegulate(void)
{
	uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;
	
    /*************************时间设置******************************************/
	printf("\n\r==============Time Settings=====================================\n\r");
	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	
	/***************************小时设置***********************************************/
	
	printf("  Please Set Hours\n\r");
	while (tmp_hh == 0xFF)
	{
		tmp_hh = USART_Scanf(0, 23);
		RTC_TimeStructure.RTC_Hours = tmp_hh;
	}
	printf(":  %0.2d\n\r", tmp_hh);
	
	/**************************分钟设置************************************************/

	printf("  Please Set Minutes\n\r");
	while (tmp_mm == 0xFF)
	{
		tmp_mm = USART_Scanf(0, 59);
		RTC_TimeStructure.RTC_Minutes = tmp_mm;
	}
	printf(":  %0.2d\n\r", tmp_mm);
	
	/***************************秒设置*********************************************/

	printf("  Please Set Seconds\n\r");
	while (tmp_ss == 0xFF)
	{
		tmp_ss = USART_Scanf(0, 59);
		RTC_TimeStructure.RTC_Seconds = tmp_ss;
	}
	printf(":  %0.2d\n\r", tmp_ss);
	
	/**************************配置时间相关寄存器**************************************************/

	/* Configure the RTC time register */
	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
	{
		printf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
	} 
	else
	{
		printf("\n\r>> !! RTC Set Time success. !! <<\n\r");
		RTC_TimeShow();
		/* 设置RTC备份寄存器0的数值 */
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
	}

	/*****************************日期设置********************************************/
	tmp_hh = 0xFF;
	tmp_mm = 0xFF;
	tmp_ss = 0xFF;
    
	printf("\n\r==============Date Settings=====================================\n\r");

	/*****************************星期设置********************************************/
	
	printf("  Please Set WeekDay (01-07)\n\r");
	while (tmp_hh == 0xFF)
	{
		tmp_hh = USART_Scanf(1, 7);
		RTC_DateStructure.RTC_WeekDay = tmp_hh;
	}
	printf(":  %0.2d\n\r", tmp_hh);
	
	/*****************************日设置***********************************************/
	
	tmp_hh = 0xFF;
	
	printf("  Please Set Date (01-31)\n\r");
	while (tmp_hh == 0xFF)
	{
		tmp_hh = USART_Scanf(1, 31);
		RTC_DateStructure.RTC_Date = tmp_hh;
	}
	printf(":  %0.2d\n\r", tmp_hh);
	
	/*****************************月设置************************************************/

	printf("  Please Set Month (01-12)\n\r");
	while (tmp_mm == 0xFF)
	{
		tmp_mm = USART_Scanf(1, 12);
		RTC_DateStructure.RTC_Month = tmp_mm;
	}
	printf(":  %0.2d\n\r", tmp_mm);
	
	/**************************年设置****************************************************/

	printf("  Please Set Year (00-99)\n\r");
	while (tmp_ss == 0xFF)
	{
		tmp_ss = USART_Scanf(0, 99);
		RTC_DateStructure.RTC_Year = tmp_ss;
	}
	printf(":  %0.2d\n\r", tmp_ss);
	
	/**************************设置日期相关寄存器**************************************************/
	
	/* Configure the RTC date register */
	if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR)
	{
		printf("\n\r>> !! RTC Set Date failed. !! <<\n\r");
	} 
	else
	{
		printf("\n\r>> !! RTC Set Date success. !! <<\n\r");
		
		RTC_DateShow();
		
		/* 设置RTC备份寄存器0的数值 */
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
	}
}

/*
*********************************************************************************************************
*	函 数 名: USART_Scanf
*	功能说明: 接收串口数据
*	形    参：MinValue 表示允许输入的最小值
*             MaxValue 表示允许输入的最大值
*             
*	返 回 值: 返回串口接收到的数值
*********************************************************************************************************
*/
uint8_t USART_Scanf(uint32_t MinValue, uint32_t MaxValue)
{
	uint32_t index = 0;
	uint32_t tmp[2] = {0, 0};

	while (index < 2)
	{
		/* 等待直到 RXNE = 1 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		{}
			
		tmp[index++] = (USART_ReceiveData(USART1));
		if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
		{
			printf("\n\r Please enter valid number between 0 and 9\n\r");
			index--;
		}
	}
	
	/* 计算相应的数值 */
	index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
	/* 检测数值是否满足要求 */
	if ((index < MinValue) || (index > MaxValue))
	{
		printf("\n\r Please enter valid number between %d and %d\n\r", MinValue, MaxValue);
		return 0xFF;
	}
	
	return index;
}

