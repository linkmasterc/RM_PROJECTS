#ifndef __BSP_INIT_H__
#define __BSP_INIT_H__
#include "tim.h"
#include "can.h"
#include "usart.h"
#include "nvic.h"
#include "systick.h"
#include "gpio.h"
#include "global_declare.h"
void Encoder_Init(void);
void Motor_Init(void);
void BSP_Init(void);
#endif
