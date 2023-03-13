#include "gpio.h"


u32 TriggerState = 0;


void gpio_init(void)
{	
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType 	= GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_3;
	GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP;			//ÉÏÀ­µç×è
	GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	

}

