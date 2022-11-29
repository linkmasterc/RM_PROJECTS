#include "bsp_init.h"

void BSP_Init()
{
  TIM5_Configuration();
	CAN1_Configuration();
	CAN2_Configuration();
	SPI_Configuration();
//	usart2_init(115200);
	SysTick_init();													// ϵͳʱ���ж�ʱ������Ϊ1ms
	usart3_init(460800);
	uart4_init(115200);
	bmi088_init();
	NVIC_Configuration();
}
