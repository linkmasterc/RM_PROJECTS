#include "motor_task.h"
float PITCH_RAMP_SIGNAL;
float YAW_RAMP_SIGNAL;
u8 span_type=1;

float ramp_Hz=0.01;
void MOTOR_TASK()
{
	if(DOWN_PLATFOM_STATUS==1)
		PITCH_SPEED_PID.fpUMax=0;
	else if(DOWN_PLATFOM_STATUS==2)
	{
		PITCH_SPEED_PID.fpUMax=28000;
		PITCH_POS_TD.m_aim=PITCH_POS_DES;
		PITCH_TD_PID();
	}
	else if(DOWN_PLATFOM_STATUS==3)
	{
		PITCH_SPEED_PID.fpUMax=28000;
		if(span_type==1)
		{
			if(PITCH_POS_DES<0)
			{
				Ramp_Signal(&PITCH_POS_DES,0,ramp_Hz);
				PITCH_TD_PID();
			}
			else if(PITCH_POS_DES>=0)
				span_type=0;
		}
		else if(span_type==0)
		{
			if(PITCH_POS_DES>-45)
			{
				Ramp_Signal(&PITCH_POS_DES,-45,ramp_Hz);
				PITCH_TD_PID();
			}
			else if(PITCH_POS_DES<=45)
				span_type=1;
		}
	}

	
}



