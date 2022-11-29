#include "shoot_task.h"
u8 fre_Hz=1;
void SHOOT_TASK()
{
	if(DOWN_SHOOT_STATUS==3)
	{
		TRIGGER_SPEED_PID.fpUMax=8000;
		shoot_frequency(DOWN_SHOOT_FREQ+1);
		TRIGGER_PID();
		SHOOTER_PID();
	}
	else
	{
		TRIGGER_SPEED_PID.fpUMax=0;
		TRIGGER_PID();
	}
}	
