#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f4xx.h"


#define Trigger_HIGH()  	GPIO_SetBits(GPIOA,GPIO_Pin_3);
#define Trigger_LOW()			GPIO_ResetBits(GPIOA, GPIO_Pin_3);
#define Trigger_TOGGLE()   	GPIOD->ODR  ^= GPIO_Pin_9


extern u32 TriggerState;
void gpio_init(void);

#endif
