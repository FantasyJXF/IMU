/*
*********************************************************************************************************
*
*	ģ������ : RTC
*	�ļ����� : rtc.h
*	��    �� : V1.0
*
*********************************************************************************************************
*/

#ifndef __RTC_H
#define __RTC_H
#include "STM32F4.h"

void RTC_TimeShow(void);
void RTC_AlarmShow(void);
void RTC_TimeRegulate(void);
uint8_t USART_Scanf(uint32_t MinValue, uint32_t MaxValue);
void RTC_DateShow(void);
void InitRTC(void);
void RTC_Config(void);
void RTC_TimeStampShow(void);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

