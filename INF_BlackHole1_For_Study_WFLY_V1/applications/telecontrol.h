#ifndef  _TELECONTROL_H_
#define  _TELECONTROL_H_
#include "stm32f10x.h"

u8 Is_WFLY_Lock(void);
u8 Is_WFLY_Unlock(void);
u8 Is_WFLY_Connected(void);
void WFLY_PWM_Convert(void);

#endif


