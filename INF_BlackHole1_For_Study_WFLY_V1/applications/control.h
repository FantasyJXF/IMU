#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "stm32f10x.h"
#include "mpu6500.h"
#include "pwm_out.h"
#include "ahrs.h"

#define LAUNCH_THROTTLE	200 			//∆∑…”Õ√≈
#define ADD_THROTTLE		1500 			//≤π≥‰”Õ√≈

typedef struct PID{float P,Pout,I,Iout,D,Dout,I_Max,Out;}PID;

void Control(S_FLOAT_XYZ	Now_Angle, S_FLOAT_XYZ Tar_Angle);
void PID_Init(void);

#endif
