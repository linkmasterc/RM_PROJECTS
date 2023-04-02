#include "bsp_init.h"

void BSP_Init(void)
{
	TIM5_Configuration();
	TIM9_Configuration(1000-1,168-1);
	CAN1_Configuration();
	CAN2_Configuration();
	SysTick_init();
	gpio_init();
	usart1_init();
	usart2_init(115200);
	usart3_init(460800);
	uart4_init(115200);
	uart5_init(460800);
	usart6_init(460800);
	Encoder_Init();
	Motor_Init();
	NVIC_Configuration();

}


void Encoder_Init()
{
	for(u8 i=0;i<4;i++)
	{
		stServoEncoder[i].siNumber=8192;
	}
}

void Motor_Init()
{
	for(u8 i=0;i<4;i++)
	{
		stWheel_SpeedPid[i].m_fpKp=10;
		stWheel_SpeedPid[i].m_fpUMax=8000;
		stWheel_SpeedPid[i].m_fpUpMax=8000;
		
		stServoWheel_PosPid[i].m_fpKp=10;
		stServoWheel_SpeedPid[i].m_fpKp=100;
		stServoWheel_PosPid[i].m_fpUMax=2000;
		stServoWheel_PosPid[i].m_fpUpMax=2000;
		stServoWheel_SpeedPid[i].m_fpUMax=8000;
		stServoWheel_SpeedPid[i].m_fpUpMax=8000;
	}
}


