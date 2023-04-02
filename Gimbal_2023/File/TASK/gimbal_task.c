#include "gimbal_task.h"
#include "global_declare.h"
#include "general_math.h"
#include "rm_algorithm.h"
#include "can.h"
u8 test=1;
u16 Barrel1_Angle=0;
u16 Barrel2_Angle=0;

u8	DNPITCHANGLEBOXSIZE = 1;
float DNPitchAnglebox[100] = {0};
float Shooter_Speed_K=0;

float Angle_180_To_Inf(float angle_input, ST_ANGLE* st_angle)
{
    st_angle->angle_180 = angle_input;

    if( st_angle->angle_180_pre - st_angle->angle_180 > 180 )
        st_angle->angle_inf += (st_angle->angle_180 - st_angle->angle_180_pre) + 360;
    else if( st_angle->angle_180 - st_angle->angle_180_pre > 180 )
        st_angle->angle_inf += (st_angle->angle_180 - st_angle->angle_180_pre) - 360;
    else
        st_angle->angle_inf += (st_angle->angle_180 - st_angle->angle_180_pre);

    st_angle->angle_180_pre = angle_input;

    return st_angle->angle_inf;
}

static u8 Friction_State_Flag = 0x00;
float SMC_J;
float SMC_f;
u32 Inc_time;
u32 Dec_time;
void Friction_Control(float speed1, float speed2)
{
	static u8 Auto_Phase;
	static u32 time_tick;
	
	float Inc_k;
	float Dec_k;
	
	static u8 Diff_Coe1;
	static u8 Diff_Coe2;
	static u8 Last_CM1_TEMP;
	static u8 Last_CM2_TEMP;
	static bool RISE_0_FALL1;
	static bool RISE_0_FALL2;
	
	static u32 tick1;
	static u32 tick2;
	static u32 Last_Time1;
	static u32 Last_Time2;
	
	
	/*温度矫正*/
	if(Last_CM1_TEMP < Friction1_Temp)
	{
		Diff_Coe1 = Friction1_Temp - Last_CM1_TEMP;
		Last_Time1 = tick1;
		tick1 = 0;
		Real_Friction1_Temp = Friction1_Temp;
		RISE_0_FALL1 = TRUE;
	}
	else if(Last_CM1_TEMP > Friction1_Temp)
	{
		Diff_Coe1 = Friction1_Temp - Last_CM1_TEMP;
		Last_Time1 = tick1;
		tick1 = 0;
		Real_Friction1_Temp = Friction1_Temp;
		RISE_0_FALL1 = FALSE;
	}
	else
	{
		tick1++;
		if(RISE_0_FALL1)
		{
			if(Real_Friction1_Temp < Friction1_Temp+1)
				Real_Friction1_Temp += (FP32)Diff_Coe1/(FP32)Last_Time1;
			else
				Real_Friction1_Temp  = Friction1_Temp+1.1f;
		}
		else
		{
			if(Real_Friction1_Temp > Friction1_Temp-1)
				Real_Friction1_Temp -= (FP32)Diff_Coe1/(FP32)Last_Time1;
			else
				Real_Friction1_Temp  = Friction1_Temp-1.1f;
		}
	}
	
	if(Last_CM2_TEMP < Friction2_Temp)
	{
		Diff_Coe2 = Friction2_Temp - Last_CM2_TEMP;
		Last_Time2 = tick2;
		tick2 = 0;
		Real_Friction2_Temp = Friction2_Temp;
		RISE_0_FALL2 = TRUE;
	}
	else if(Last_CM2_TEMP > Friction2_Temp)
	{
		Diff_Coe2 = Friction2_Temp - Last_CM2_TEMP;
		Last_Time2 = tick2;
		tick2 = 0;
		Real_Friction2_Temp = Friction2_Temp;
		RISE_0_FALL2 = FALSE;
	}
	else
	{
		tick2++;
		if(RISE_0_FALL2)
		{
			if(Real_Friction2_Temp < Friction2_Temp+1)
				Real_Friction2_Temp += (FP32)Diff_Coe2/(FP32)Last_Time2;
			else
				Real_Friction2_Temp  = Friction2_Temp+1.1f;
		}
		else
		{
			if(Real_Friction2_Temp > Friction2_Temp-1)
				Real_Friction2_Temp -= (FP32)Diff_Coe2/(FP32)Last_Time2;
			else
				Real_Friction2_Temp  = Friction2_Temp-1.1f;
		}
	}		
		/*速度控制*/
	if(Friction_State_Flag == 1)										
	{
		if(Auto_Phase == 1)
		{
			if(g_stFriction1SMC.m_fpFB > 6000)
			{
				Inc_time = time_tick;
				time_tick = 0;
				Auto_Phase = 2;
			}
			else
			{
				g_stFriction1SMC.m_fpU = 500;
				time_tick++;
			}
		}
		else if(Auto_Phase == 2)
		{
			if(g_stFriction1SMC.m_fpFB < 100)
			{
				Dec_time = time_tick;
				time_tick = 0;
				Auto_Phase = 0;
			}
			else
			{
				g_stFriction1SMC.m_fpU = 0;
				time_tick++;
			}
		}
		else
		{
			Inc_k = 6000.0f*1000.0f/Inc_time;
			Dec_k = 5900.0f*1000.0f/Dec_time;
			
			SMC_J = 500/(Inc_k + Dec_k);
			SMC_f = SMC_J*Dec_k;
		}
		
		CAN_SendData(CAN1,0x1ff,-(s16)(g_stFriction2SMC.m_fpU),0,0,(s16)(g_stFriction1SMC.m_fpU));
	}
	
	else if(Friction_State_Flag == 2)							//辨识模式
	{
		time_tick = 0;
		Auto_Phase = 1;
		
		g_stFriction1SMC.m_fpDes = -speed1;
		g_stFriction2SMC.m_fpDes = +speed2;
		
		CalSMC(&g_stFriction1SMC);
		CalSMC(&g_stFriction2SMC);
		
		CAN_SendData(CAN1,0x1ff,((s16)(g_stFriction2SMC.m_fpU)-Fric),0,0,((s16)(g_stFriction1SMC.m_fpU)+Fric));
	}
	else																	//空闲模式
	{
		time_tick = 0;
		Auto_Phase = 1;
		
		CAN_SendData(CAN1,0x1ff,0,0,0,0);
	}
	
	Last_CM1_TEMP = Friction1_Temp;
	Last_CM2_TEMP = Friction2_Temp;

	
}

float Pitch_Current = 0;
void GimbalControl(void)
{
	static bool Pre_RunFlag = FALSE;
	static u8 Clk_Div = 0;
	float FricSpeed1,FricSpeed2;
	
	Clk_Div++;
	
	#ifdef PITCH_SPEED_FEEDBACK_ENCODER
	g_stPitchSpeedPID.m_fpFB = Encoder_Pitch_speed;
	#elif defined(PITCH_SPEED_FEEDBACK_GRYOSCOPE)
	g_stPitchSpeedPID.m_fpFB = Gyro_Y_Speed;
	#endif
	
	if(Clk_Div == 4)
	{
		if(stGimbalFlag.RunFlag == TRUE)
		{
			g_stPitchPosPID.m_fpDes = ChassisData.Receive.PitAngleDes;
		}
		
		//Pitch电机控制
		#ifdef PITCH_ANGLE_FEEDBACK_ENCODER
		g_stPitchPosPID.m_fpFB = DataSmoothFilter(DNPitchAnglebox,DNPITCHANGLEBOXSIZE,Pitch_Encoder_angle);
//		g_stPitchPosPID.m_fpFB = Pitch_Encoder_angle;
		#elif defined(PITCH_ANGLE_FEEDBACK_GRYOSCOPE)
		g_stPitchPosPID.m_fpFB = GetBMIPitchAngle() + PitchDiff;
		#endif
		
		if(stGimbalFlag.RunFlag == TRUE)
		{
			
			g_stPitchTD.m_aim = Clip(ChassisData.Receive.PitAngleDes, GBDN_PITCH_MIN, GBDN_PITCH_MAX);
//			g_stPitchTD.m_aim = Clip(PitchTest, GBDN_PITCH_MIN, GBDN_PITCH_MAX);
			g_stPitchPosPID.m_fpDes = Clip(ChassisData.Receive.PitAngleDes, GBDN_PITCH_MIN, GBDN_PITCH_MAX);
			GimbalSecondPosPid.m_fpDes=GimbalYawPosPid.m_fpDes;
//			g_stPitchPosPID.m_fpDes = Clip(PitchTest, GBDN_PITCH_MIN, GBDN_PITCH_MAX);
		}
		
		if(test==0){
		CalTD(&g_stPitchTD);
		g_stPitchPosPID.m_fpDes = g_stPitchTD.m_x1;}
//		else g_stPitchPosPID.m_fpDes = PitchTest;
		CalIWeakenPID(&g_stPitchPosPID);
		PitchSpeedCompensate = PitchCoe * g_stPitchTD.m_x2;
		g_stPitchSpeedPID.m_fpDes = g_stPitchPosPID.m_fpU - PitchSpeedCompensate;
		CalIWeakenPID(&g_stPitchSpeedPID);
		
		CalIWeakenPID(&g_stPitchPosPID);
		g_stPitchSpeedPID.m_fpDes = g_stPitchPosPID.m_fpU;
		CalIWeakenPID(&g_stPitchSpeedPID);
		
		//二级云台电机控制
		CalIWeakenPID(&GimbalSecondPosPid);
		GimbalSecondSpeedPid.m_fpDes=GimbalSecondPosPid.m_fpU;
		CalIWeakenPID(&GimbalSecondSpeedPid);
		
		
		//拨弹电机闭环控制
		CalIResistedPID(&g_stShooterPosPID);
		g_stShooterSpeedPID.m_fpDes = g_stShooterPosPID.m_fpU / 8192 * 360.0f;
		CalIWeakenPID(&g_stShooterSpeedPID);
		
		//摩擦轮闭环控制
		if(stGimbalFlag.FrictionFlag == TRUE&&stGimbalFlag.RunFlag==TRUE)
		{
			Friction_State_Flag = 0x02;
			if(ChassisData.Receive.Friction_Send_Des == 700)
			{
				FricSpeed1 = fric_speed1;
				FricSpeed2 = fric_speed1;
			}
			else if(ChassisData.Receive.Friction_Send_Des == 600)
			{
				FricSpeed1 = fric_speed2;
				FricSpeed2 = fric_speed2;
			}
		}
		else
		{
			FricSpeed1 = 0;
			FricSpeed2 = 0;
			if(g_stFriction2SMC.m_fpFB < 100)
				Friction_State_Flag = 0x00;
		}
		Friction_Control(FricSpeed1, FricSpeed2);
		
		if(stGimbalFlag.RunFlag == FALSE && Pre_RunFlag == FALSE)
		{
			S16RampSignal(&G_Compensate,0,2);
			FPRampSignal(&g_stPitchSpeedPID.m_fpUMax,0,2);
			//积分清零
			g_stPitchPosPID.m_fpSumE = 0;
			g_stPitchSpeedPID.m_fpSumE = 0;
		}
		
			//重力补偿
		if(g_stPitchSpeedPID.m_fpU + G_Compensate * cos((g_stPitchPosPID.m_fpFB + COMP)/360*2*PI) > 30000)
			Pitch_Current =  30000;
		else if(g_stPitchSpeedPID.m_fpU + G_Compensate * cos((g_stPitchPosPID.m_fpFB +COMP)/360*2*PI) < -30000)
			Pitch_Current = -30000;
		else
			Pitch_Current = g_stPitchSpeedPID.m_fpU + G_Compensate * cos((g_stPitchPosPID.m_fpFB + COMP)/360*2*PI);

		if(g_stShooterSpeedPID.m_fpE >30000)
		{
			g_stShooterSpeedPID.m_fpU = 0;
			Shooter_BLOCK = TRUE;
		}
		
		if(stGimbalFlag.PitchProtectFlag)
		{	Pitch_Current	= 0;}
		CAN_SendData(CAN2,0x1ff,0,(s16)(Pitch_Current),0,GimbalSecondSpeedPid.m_fpU);
		Pre_RunFlag = stGimbalFlag.RunFlag;
		Clk_Div = 0;
	}
}

void BarrelTranfer()
{
	
}

