#ifndef __EXTI_H
#define	__EXTI_H


#include "stm32f10x.h"


/*外部中断EXIT相关宏定义*/
#define             macEXTI_GPIO_CLK                        (RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO)     
#define             macEXTI_GPIO_PORT                       GPIOB   
#define             macEXTI_GPIO_PIN                        GPIO_Pin_10
#define             macEXTI_SOURCE_PORT                     GPIO_PortSourceGPIOB
#define             macEXTI_SOURCE_PIN                      GPIO_PinSource10
#define             macEXTI_LINE                            EXTI_Line10
#define             macEXTI_IRQ                             EXTI15_10_IRQn
#define             macEXTI_INT_FUNCTION                    EXTI15_10_IRQHandler


#define ENABLE_INV_INTERRUPTS  EnableInvInterrupt()
#define DISABLE_INV_INTERRUPTS DisableInvInterrupt()


void EXTI_Pxy_Config(void);
void EnableInvInterrupt(void);
void DisableInvInterrupt(void);


#endif /* __EXTI_H */

