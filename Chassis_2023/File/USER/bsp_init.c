#include "bsp_init.h"

void BSP_Init(void)
{
	TIM5_Configuration();
	CAN1_Configuration();
	CAN2_Configuration();
	usart3_init(460800);
	NVIC_Configuration();

}
