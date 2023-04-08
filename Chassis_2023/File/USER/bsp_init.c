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
	uart4_init(460800);
	uart5_init(460800);
	usart6_init(460800);
	Encoder_Init();
	Motor_Init();
	RSStructAddress_Init();
	NVIC_Configuration();

}


void Encoder_Init()
{
	for(u32 i=0;i<4;i++)
	{
		stServoEncoder[i].siNumber=8192;
	}
}

void Motor_Init()
{
	for(u32 i=0;i<4;i++)
	{
		stWheel_SpeedPid[i].m_fpKp=1000;
		stWheel_SpeedPid[i].m_fpUMax=8000;
		stWheel_SpeedPid[i].m_fpUpMax=8000;
		
		stServoWheel_PosPid[i].m_fpKp=20;
		stServoWheel_SpeedPid[i].m_fpKp=100;
		stServoWheel_PosPid[i].m_fpUMax=2000;
		stServoWheel_PosPid[i].m_fpUpMax=2000;
		stServoWheel_SpeedPid[i].m_fpUMax=8000;
		stServoWheel_SpeedPid[i].m_fpUpMax=8000;
	}
}

void RSStructAddress_Init()
{
		RSStructAddressBuf[0]=(u32)(&GameStatus);
		RSStructAddressBuf[1]=(u32)(&GameResult);
		RSStructAddressBuf[2]=(u32)(&GameRobotHP);
		RSStructAddressBuf[3]=(u32)(&EventData);
		RSStructAddressBuf[4]=(u32)(&SupplyProjectileAction);
		RSStructAddressBuf[5]=(u32)(&RefereeWarning);
		RSStructAddressBuf[6]=(u32)(&DartRemainingTime);
		RSStructAddressBuf[7]=(u32)(&GameRobotStatus);
		RSStructAddressBuf[8]=(u32)(&PowerHeatData);
		RSStructAddressBuf[9]=(u32)(&GameRobotPos);
		RSStructAddressBuf[10]=(u32)(&Buff);
		RSStructAddressBuf[11]=(u32)(&AerialRobotEnergy);
		RSStructAddressBuf[12]=(u32)(&RobotHurt);
		RSStructAddressBuf[13]=(u32)(&ShootData);
		RSStructAddressBuf[14]=(u32)(&BulletRemaining);
		RSStructAddressBuf[15]=(u32)(&RFIDStatus);


}
	
	


