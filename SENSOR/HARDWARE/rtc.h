/*
*********************************************************************************************************
*
*	模块名称 : RTC
*	文件名称 : rtc.h
*	版    本 : V1.0
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

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

