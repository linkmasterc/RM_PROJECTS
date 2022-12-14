#include "bsp_init.h"

void BSP_Init(void)
{
	TIM5_Configuration();
	CAN1_Configuration();
	CAN2_Configuration();
	SysTick_init();
	usart1_init();
	usart2_init(115200);
	usart3_init(460800);
	usart6_init(921600);
	NVIC_Configuration();

}
