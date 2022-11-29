#include "motor.h"

void shoot_frequency(u8 fre_Hz)
{
	static u32 shoot_time=0;
	if(system_monitor.SysTickTime-shoot_time>(1000/fre_Hz))
	{
		bullet_num++;
		shoot_time=system_monitor.SysTickTime;
	}
}
/*******************************************
函数名：Get_Encoder_Number
作用	：获取码盘值
备注	：
*******************************************/
s16 Get_Encoder_Number(CanRxMsg* pCanRxMsg)
{
	s16 encoder_number=0;
	encoder_number=(pCanRxMsg->Data[0]<<8)|pCanRxMsg->Data[1];
	return encoder_number;
}


/*******************************************
函数名：Get_Speed
作用	：获得电机的转速(rpm)
备注	：
*******************************************/
s16 Get_Speed(CanRxMsg* pCanRxMsg)
{
	s16 speed;
	speed=(pCanRxMsg->Data[2]<<8)|pCanRxMsg->Data[3];
	return speed;
}


/*******************************************
函数名：Abs_Encoder_Process
作用	：获得码盘值的累计值
备注	：有过零判断
*******************************************/
s32 Abs_Encoder_Process(ST_ENCODER* pEncoder, s32 value)
{
	pEncoder->PreValue=pEncoder->RawValue;
	pEncoder->RawValue=value;
	pEncoder->diff=pEncoder->RawValue-pEncoder->PreValue;
	if(pEncoder->diff<-pEncoder->Number/2)
	{
		pEncoder->diff+=pEncoder->Number;
	}
	else if(pEncoder->diff>pEncoder->Number/2)
	{
		pEncoder->diff-=pEncoder->Number;
	}
	pEncoder->Sumvalue+=pEncoder->diff;
	return pEncoder->Sumvalue;
}
float Abs_Angle_Process(ST_ANGLE* pANGLE,float angle)
{
	pANGLE->PreValue=pANGLE->RawValue;
	pANGLE->RawValue=angle;
	pANGLE->diff=pANGLE->RawValue-pANGLE->PreValue;
	if(pANGLE->diff<-pANGLE->deg/2)
		pANGLE->diff+=pANGLE->deg;
	else if(pANGLE->diff>pANGLE->deg/2)
		pANGLE->diff-=pANGLE->deg;
	pANGLE->SumValue+=pANGLE->diff;
	return pANGLE->SumValue;
}

/*******************************************
函数名：CAN_SendCurrent
作用	：通过can线发送电流(电压)
备注	：
*******************************************/
void CAN_SendCurrent(CAN_TypeDef* CANx,u32 id,s16 current1,s16 current2,s16 current3,s16 current4)
{
	static CanTxMsg TxMessage;
	TxMessage.StdId=id;
	TxMessage.IDE=CAN_Id_Standard;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.DLC=8;
	TxMessage.Data[0]=(current1>>8);
	TxMessage.Data[1]=current1;
	TxMessage.Data[2]=(current2>>8);
	TxMessage.Data[3]=current2;
	TxMessage.Data[4]=(current3>>8);
	TxMessage.Data[5]=current3;
	TxMessage.Data[6]=(current4>>8);
	TxMessage.Data[7]=current4;
	CAN_Transmit(CANx,&TxMessage);
}
u8 judge_TD=1;
/*******************************************
函数名：YAW_TD_PID
作用	：YAW轴的PID控制
备注	：
*******************************************/
void YAW_TD_PID()
{
	if(judge_TD==1)
	{
		YAW_POS_TD.m_aim=YAW_POS_DES;
		CalTD(&YAW_POS_TD);
		YAW_POS_PID.fpDes=YAW_POS_TD.m_x1;
	}
	else 
		YAW_POS_PID.fpDes=YAW_POS_DES-10.3;

	YAW_POS_PID.fpFB=-Abs_Angle_Process(&YAW_ANGLE,BMIYawAngle);
	if(YAW_POS_PID.fpDes-YAW_POS_PID.fpFB>=180)
		YAW_POS_PID.fpFB+=360;
	else if(YAW_POS_PID.fpDes-YAW_POS_PID.fpFB<=-180)
		YAW_POS_PID.fpFB-=360;
	PID_Control(&YAW_POS_PID);
	YAW_SPEED_PID.fpDes=YAW_POS_PID.fpU+YAW_POS_TD.m_x2*COE*judge_TD;
	YAW_SPEED_PID.fpFB=BMIYawSpeed;
	PID_LPF_Control(&YAW_SPEED_PID,&YAW_LPF);
	YAW_SPEED_PID.fpU=-YAW_SPEED_PID.fpU;
	CAN_SendCurrent(CAN2,0x1ff,YAW_SPEED_PID.fpU,0,0,0);
}
/*******************************************
函数名：CAN_SendCurrent
作用	：通过can线发送电流(电压)
备注	：
*******************************************/

void PITCH_TD_PID()
{
	if(judge_TD==1)
	{
		PITCH_POS_TD.m_aim=PITCH_POS_DES;
		CalTD(&PITCH_POS_TD);
		PITCH_POS_PID.fpDes=PITCH_POS_TD.m_x1;
	}
	else
		PITCH_POS_PID.fpDes=PITCH_POS_DES;
	
	PITCH_POS_PID.fpFB=BMIPitchAngle;
//	PITCH_POS_PID.fpKp=step_kp(&PITCH_POS_PID);
	PID_Control(&PITCH_POS_PID);
	PITCH_SPEED_PID.fpDes=PITCH_POS_PID.fpU+PITCH_POS_TD.m_x2*COE*judge_TD;
	PITCH_SPEED_PID.fpFB=BMIPitchSpeed;
	PID_LPF_Control(&PITCH_SPEED_PID,&PITCH_LPF);
	PITCH_SPEED_PID.fpU=-PITCH_SPEED_PID.fpU;
//	if(PITCH_POS_DES>0)
		PITCH_SPEED_PID.fpU+=gravity_fb;
//	else if(PITCH_POS_DES<0)
//		PITCH_SPEED_PID.fpU-=gravity_fb*cos(BMIPitchAngle*PI/180);
	PITCH_SPEED_PID.fpU=value_ctrl(PITCH_SPEED_PID.fpU,PITCH_SPEED_PID.fpUMax,-PITCH_SPEED_PID.fpUMax);
	CAN_SendCurrent(CAN2,0x1ff,0,0,PITCH_SPEED_PID.fpU,0);
}

void TRIGGER_PID()
{
	static u16 last_buller_num=0;
	TRIGGER_POS_PID.fpDes+=Bottom_SupplyStep*(bullet_num-last_buller_num);
	last_buller_num=bullet_num;
	TRIGGER_POS_PID.fpFB=motor_wheel[11].sum_value;
	PID_Control(&TRIGGER_POS_PID);
	TRIGGER_SPEED_PID.fpDes=TRIGGER_POS_PID.fpU/8192.0*360.0;
	TRIGGER_SPEED_PID.fpFB=motor_wheel[11].speed/36.0;
	PID_Control(&TRIGGER_SPEED_PID);
	CAN_SendCurrent(CAN2,0x1ff,TRIGGER_SPEED_PID.fpU,0,0,0);
}

void SHOOTER_PID()
{
	SHOOTER_SPEED_PID_L.fpFB=motor_wheel[1].speed;
	SHOOTER_SPEED_PID_R.fpFB=motor_wheel[2].speed;
	PID_Control(&SHOOTER_SPEED_PID_L);
	PID_Control(&SHOOTER_SPEED_PID_R);
	CAN_SendCurrent(CAN1,0x200,SHOOTER_SPEED_PID_L.fpU,SHOOTER_SPEED_PID_R.fpU,0,0);
}

