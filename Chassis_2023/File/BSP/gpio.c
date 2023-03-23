#include "gpio.h"


u32 TriggerState = 0;


void gpio_init(void)
{	
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);

	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType 	= GPIO_OType_OD;
	GPIO_InitStruct.GPIO_Pin 	= GPIO_Pin_3;
	GPIO_InitStruct.GPIO_PuPd 	= GPIO_PuPd_UP;			//ÉÏÀ­µç×è
	GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_OType =GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|
															GPIO_Pin_14|GPIO_Pin_15;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
}

