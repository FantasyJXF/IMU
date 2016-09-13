/*
*********************************************************************************************************
*
*	ģ������ : RTC
*	�ļ����� : rtc.c
*	˵    �� : RTC�ײ�����
*********************************************************************************************************
*/

#include "STM32F4.h"


#define RTC_Debug   /* ����ѡ�����ģʽ */

/* ѡ��RTC��ʱ��Դ */
#define RTC_CLOCK_SOURCE_LSE       /* LSE */
//#define RTC_CLOCK_SOURCE_LSI     /* LSI */ 

/* ���� */
RTC_TimeTypeDef  RTC_TimeStructure;
RTC_InitTypeDef  RTC_InitStructure;
RTC_DateTypeDef  RTC_DateStructure;

RTC_TimeTypeDef  RTC_TimeStampStructure;
RTC_DateTypeDef  RTC_TimeStampDateStructure;

/* ��������RTC��Ƶ */
__IO uint32_t uwAsynchPrediv = 0;
__IO uint32_t uwSynchPrediv = 0;
/*
*********************************************************************************************************
*	�� �� ��: InitRTC
*	����˵��: ��ʼ��RTC
*	��    �Σ���
*	�� �� ֵ: ��		        
*********************************************************************************************************
*/
void nitRTC(void)
{
	
	/* ���ڼ���Ƿ��Ѿ����ù�RTC��������ù��Ļ����������ý���ʱ
	   ����RTC���ݼĴ���Ϊ0x32F2��������RTC���ݼĴ�������0x32F2
	   ��ô��ʾû�����ù�����Ҫ����RTC.
	*/
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
	{  
		/* RTC ����  */
		RTC_Config();
		
 		/* ��ӡ������Ϣ */
		#ifdef RTC_Debug
			printf("��һ��ʹ��RTC \n\r");
		#endif 	
		
		/* ����ϵ縴λ��־�Ƿ����� */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			/* �����ϵ縴λ */
			#ifdef RTC_Debug
				printf("�����ϵ縴λ \n\r");
			#endif
		}
	}
	else
	{
		/* ��ӡ������Ϣ */
		#ifdef RTC_Debug
			printf("��n��ʹ��RTC \n\r");
		#endif
		
		/* ����ϵ縴λ��־�Ƿ����� */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			/* �����ϵ縴λ */
			#ifdef RTC_Debug
				printf("�����ϵ縴λ \n\r");
			#endif
		}
		/* ������Ÿ�λ��־�Ƿ����� */
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			/* �������Ÿ�λ */
			#ifdef RTC_Debug
			printf("�������Ÿ�λ \n\r");
			#endif			
		}

		/* ʹ��PWRʱ�� */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

		/* �������RTC */
		PWR_BackupAccessCmd(ENABLE);

		/* �ȴ� RTC APB �Ĵ���ͬ�� */
		RTC_WaitForSynchro();
		
		/* ���RTC���ӱ�־ */
		RTC_ClearFlag(RTC_FLAG_ALRAF);

		/* ���RTC�����жϹ����־ */
		EXTI_ClearITPendingBit(EXTI_Line17);
	}
	
}
	
/*
*********************************************************************************************************
*	�� �� ��: RTC_Config
*	����˵��: ��������ʱ�������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RTC_Config(void)
{	
	/* ʹ��PWRʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/* �������RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* ѡ��LSI��Ϊʱ��Դ */
#if defined (RTC_CLOCK_SOURCE_LSI)  
	
	/* Enable the LSI OSC */ 
	RCC_LSICmd(ENABLE);

	/* Wait till LSI is ready */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{
	}

	/* ѡ��RTCʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	
	/* ck_spre(1Hz) = RTCCLK(LSE) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)*/
	uwSynchPrediv = 0xFF;
	uwAsynchPrediv = 0x7F;
	
	/* ѡ��LSE��ΪRTCʱ�� */
#elif defined (RTC_CLOCK_SOURCE_LSE)
	/* ʹ��LSE����  */
	RCC_LSEConfig(RCC_LSE_ON);

	/* �ȴ����� */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
	}

	/* ѡ��RTCʱ��Դ */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

	uwSynchPrediv = 0xFF;
	uwAsynchPrediv = 0x7F;

#else
#error Please select the RTC Clock source inside the main.c file
#endif 

	/* ʹ��RTCʱ�� */
	RCC_RTCCLKCmd(ENABLE);

	/* �ȴ�RTC APB�Ĵ���ͬ�� */
	RTC_WaitForSynchro();
	
	/* ʹ��ʱ��� */
  RTC_TimeStampCmd(RTC_TimeStampEdge_Falling, ENABLE);  
	
	RTC_TimeStampPinSelection(RTC_TamperPin_PC13);	

	/* ����RTC���ݼĴ����ͷ�Ƶ��  */
	RTC_InitStructure.RTC_AsynchPrediv = uwAsynchPrediv;
	RTC_InitStructure.RTC_SynchPrediv = uwSynchPrediv;
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	
	/* ���RTC��ʼ�� */
	if (RTC_Init(&RTC_InitStructure) == ERROR)
	{
		/* ��ӡ������Ϣ */
		#ifdef RTC_Debug
			printf("RTC��ʼ��ʧ�� \n\r");
		#endif
	}

	/* ���������պ����� */
	RTC_DateStructure.RTC_Year = 0x13;
	RTC_DateStructure.RTC_Month = RTC_Month_January;
	RTC_DateStructure.RTC_Date = 0x11;
	RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Saturday;
	RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

	/* ����ʱ���룬�Լ���ʾ��ʽ */
	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours   = 0x05;
	RTC_TimeStructure.RTC_Minutes = 0x20;
	RTC_TimeStructure.RTC_Seconds = 0x00; 
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   
	
	/* ���ñ��ݼĴ�������ʾ�Ѿ����ù�RTC */
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
}

/*
*********************************************************************************************************
*	�� �� ��: RTC_TimeShow
*	����˵��: ��ʾʱ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RTC_TimeShow(void)
{
	/* �õ�ʱ�� */
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	printf("\n\r============== Current Time Display ============================\n\r");
	printf("\n\r  ��ǰʱ�� :  %0.2d:%0.2d:%0.2d \n\r", RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}

/*
*********************************************************************************************************
*	�� �� ��: RTC_DateShow
*	����˵��: ��ʾʱ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RTC_DateShow(void)
{
	/* �õ����� */
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	printf("\n\r============== Current Date Display ============================\n\r");
	printf("\n\r  ��ǰ���� :  %0.2d-%0.2d-%0.2d-%0.2d \n\r", RTC_DateStructure.RTC_WeekDay, RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Year);
}

/*
*********************************************************************************************************
*	�� �� ��: RTC_TimeStampShow
*	����˵��: ʱ�����ʾ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RTC_TimeStampShow(void)
{
	/* �õ���ǰʱ��� */
	RTC_GetTimeStamp(RTC_Format_BIN, &RTC_TimeStampStructure, &RTC_TimeStampDateStructure);
	printf("\n\r==============ʱ�����ʾ (ʱ�������)=================\n\r");
	printf("ʱ�� :  %0.2d:%0.2d:%0.2d \n\r", RTC_TimeStampStructure.RTC_Hours, RTC_TimeStampStructure.RTC_Minutes, RTC_TimeStampStructure.RTC_Seconds);
	printf("���� :  %0.2d-%0.2d-%0.2d \n\r", RTC_TimeStampDateStructure.RTC_WeekDay, RTC_TimeStampDateStructure.RTC_Date, RTC_TimeStampDateStructure.RTC_Month);
	printf("======================================================\n\r");
}

/*
*********************************************************************************************************
*	�� �� ��: RTC_TimeRegulate
*	����˵��: 
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RTC_TimeRegulate(void)
{
	uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;
	
    /*************************ʱ������******************************************/
	printf("\n\r==============Time Settings=====================================\n\r");
	RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	
	/***************************Сʱ����***********************************************/
	
	printf("  Please Set Hours\n\r");
	while (tmp_hh == 0xFF)
	{
		tmp_hh = USART_Scanf(0, 23);
		RTC_TimeStructure.RTC_Hours = tmp_hh;
	}
	printf(":  %0.2d\n\r", tmp_hh);
	
	/**************************��������************************************************/

	printf("  Please Set Minutes\n\r");
	while (tmp_mm == 0xFF)
	{
		tmp_mm = USART_Scanf(0, 59);
		RTC_TimeStructure.RTC_Minutes = tmp_mm;
	}
	printf(":  %0.2d\n\r", tmp_mm);
	
	/***************************������*********************************************/

	printf("  Please Set Seconds\n\r");
	while (tmp_ss == 0xFF)
	{
		tmp_ss = USART_Scanf(0, 59);
		RTC_TimeStructure.RTC_Seconds = tmp_ss;
	}
	printf(":  %0.2d\n\r", tmp_ss);
	
	/**************************����ʱ����ؼĴ���**************************************************/

	/* Configure the RTC time register */
	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
	{
		printf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
	} 
	else
	{
		printf("\n\r>> !! RTC Set Time success. !! <<\n\r");
		RTC_TimeShow();
		/* ����RTC���ݼĴ���0����ֵ */
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
	}

	/*****************************��������********************************************/
	tmp_hh = 0xFF;
	tmp_mm = 0xFF;
	tmp_ss = 0xFF;
    
	printf("\n\r==============Date Settings=====================================\n\r");

	/*****************************��������********************************************/
	
	printf("  Please Set WeekDay (01-07)\n\r");
	while (tmp_hh == 0xFF)
	{
		tmp_hh = USART_Scanf(1, 7);
		RTC_DateStructure.RTC_WeekDay = tmp_hh;
	}
	printf(":  %0.2d\n\r", tmp_hh);
	
	/*****************************������***********************************************/
	
	tmp_hh = 0xFF;
	
	printf("  Please Set Date (01-31)\n\r");
	while (tmp_hh == 0xFF)
	{
		tmp_hh = USART_Scanf(1, 31);
		RTC_DateStructure.RTC_Date = tmp_hh;
	}
	printf(":  %0.2d\n\r", tmp_hh);
	
	/*****************************������************************************************/

	printf("  Please Set Month (01-12)\n\r");
	while (tmp_mm == 0xFF)
	{
		tmp_mm = USART_Scanf(1, 12);
		RTC_DateStructure.RTC_Month = tmp_mm;
	}
	printf(":  %0.2d\n\r", tmp_mm);
	
	/**************************������****************************************************/

	printf("  Please Set Year (00-99)\n\r");
	while (tmp_ss == 0xFF)
	{
		tmp_ss = USART_Scanf(0, 99);
		RTC_DateStructure.RTC_Year = tmp_ss;
	}
	printf(":  %0.2d\n\r", tmp_ss);
	
	/**************************����������ؼĴ���**************************************************/
	
	/* Configure the RTC date register */
	if(RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure) == ERROR)
	{
		printf("\n\r>> !! RTC Set Date failed. !! <<\n\r");
	} 
	else
	{
		printf("\n\r>> !! RTC Set Date success. !! <<\n\r");
		
		RTC_DateShow();
		
		/* ����RTC���ݼĴ���0����ֵ */
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: USART_Scanf
*	����˵��: ���մ�������
*	��    �Σ�MinValue ��ʾ�����������Сֵ
*             MaxValue ��ʾ������������ֵ
*             
*	�� �� ֵ: ���ش��ڽ��յ�����ֵ
*********************************************************************************************************
*/
uint8_t USART_Scanf(uint32_t MinValue, uint32_t MaxValue)
{
	uint32_t index = 0;
	uint32_t tmp[2] = {0, 0};

	while (index < 2)
	{
		/* �ȴ�ֱ�� RXNE = 1 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		{}
			
		tmp[index++] = (USART_ReceiveData(USART1));
		if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
		{
			printf("\n\r Please enter valid number between 0 and 9\n\r");
			index--;
		}
	}
	
	/* ������Ӧ����ֵ */
	index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
	/* �����ֵ�Ƿ�����Ҫ�� */
	if ((index < MinValue) || (index > MaxValue))
	{
		printf("\n\r Please enter valid number between %d and %d\n\r", MinValue, MaxValue);
		return 0xFF;
	}
	
	return index;
}

