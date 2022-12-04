#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f4xx.h"


#define Trigger_HIGH()  	GPIOA->BSRRL = GPIO_Pin_8
#define Trigger_LOW()		GPIOA->BSRRH = GPIO_Pin_8
#define Trigger_TOGGLE()   	GPIOA->ODR  ^= GPIO_Pin_8


extern u32 TriggerState;
void gpio_init(void);

#endif
