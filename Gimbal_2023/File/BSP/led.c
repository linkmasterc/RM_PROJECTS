#include "led.h"

void LED_Init(void)
{    	 
	GPIO_SetBits(GPIOA,GPIO_Pin_13 | GPIO_Pin_14);//GPIOF9,F10…Ë÷√∏ﬂ£¨µ∆√
}
