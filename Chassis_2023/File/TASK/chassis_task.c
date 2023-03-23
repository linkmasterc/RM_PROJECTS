#include "chassis_task.h"
#define ABS(x) ( (x)>0?(x):-(x) )
#define SpeedWheelRate 10
#define SafeType 		0x00
#define GyroType 		0x01
#define LineType 		0x02
#define ForceFBType 0x03

#define SpanSpeed 6000

bool GyroJudge=TRUE;

float InitAngle[4]={3530,4190,4113,4232};

float X_Vel;
float Y_Vel;
float W_Vel;

float Angle_I;
float Angle_II;
float Angle_III;
float Angle_IV;

float X_Max_Vel;
float Y_Max_Vel;
float W_Max_Vel;

s16 Raw_Sum_Current;
s16 Limit_Sum_Current;

float testkp=0;

float speed_rate=1;

void ForceFB()
{
	for(u8 i=0;i<4;i++)
	{
		if(fabs(stServoEncoder[i].uiRawValue-InitAngle[i])<=400)
		{
			stServoWheel_SpeedPid[i].m_fpUMax=3500;
		}
		else
		{
			stServoWheel_SpeedPid[i].m_fpUMax=0;
			stServoWheel_PosPid[i].m_fpDes=stServoWheel_PosPid[i].m_fpFB;
		}
	}
	
}
void ServoWheelControl()
{
	for(u8 i=0;i<4;i++)
	{
		CalIWeakenPID(&stServoWheel_PosPid[i]);
		stServoWheel_SpeedPid[i].m_fpDes=stServoWheel_PosPid[i].m_fpU;
		CalIWeakenPID(&stServoWheel_SpeedPid[i]);
	}
}
void SpeedWheelControl()
{
	for(u8 i=0;i<4;i++)
	{
		CalIWeakenPID(&stWheel_SpeedPid[i]);
	}
}
	

void ServoMode(u8 modechoice)
{
	switch (modechoice)
	{
		/*******************小陀螺模式下舵向轮控制************************/
		case GyroType:
		{
			for(u8 i=0;i<4;i++)
			{
				stServoWheel_SpeedPid[i].m_fpUpMax=8000;
				stServoWheel_SpeedPid[i].m_fpUMax=8000;

			}
			if(GyroJudge)
			{
				stServoWheel_PosPid[0].m_fpDes=stServoWheel_PosPid[0].m_fpDes+45;
				stServoWheel_PosPid[1].m_fpDes=stServoWheel_PosPid[1].m_fpDes-45;
				stServoWheel_PosPid[2].m_fpDes=stServoWheel_PosPid[2].m_fpDes+45;
				stServoWheel_PosPid[3].m_fpDes=stServoWheel_PosPid[3].m_fpDes-45;
				GyroJudge=FALSE;
			}			
		}break;
		/********************直线模式下舵向轮控制***********************/
		case LineType:
		{
			for(u8 i=0;i<4;i++)
			{
				stServoWheel_SpeedPid[i].m_fpUpMax=8000;
				stServoWheel_SpeedPid[i].m_fpUMax=8000;

			}
			if(!GyroJudge)
			{
				stServoWheel_PosPid[0].m_fpDes=stServoWheel_PosPid[0].m_fpDes-45;
				stServoWheel_PosPid[1].m_fpDes=stServoWheel_PosPid[1].m_fpDes+45;
				stServoWheel_PosPid[2].m_fpDes=stServoWheel_PosPid[2].m_fpDes-45;
				stServoWheel_PosPid[3].m_fpDes=stServoWheel_PosPid[3].m_fpDes+45;
				GyroJudge=TRUE;
			}	
		}break;
		/********************机械测试模式下舵向轮标定控制**********************/
		case ForceFBType:
		{
			ForceFB();
		}break;
		/********************安全模式下舵向轮控制***********************/
		case SafeType:
		{
			for(u8 i=0;i<4;i++)
			{
				stServoWheel_SpeedPid[i].m_fpUMax=0;
			}
		}break;
		
		default:
		break;
	}
	ServoWheelControl();
}


void SpeedMode(u8 modechoice)
{
	switch (modechoice)
	{
		/**********************小陀螺模式下速度轮的控制*********************/
		case GyroType:
		{
			for(u8 i=0;i<4;i++)
				stWheel_SpeedPid[i].m_fpUMax=8000;
			for(u8 i=0;i<4;i++)
				FPRampSignal(&stWheel_SpeedPid[i].m_fpDes,SpanSpeed,10);
		}break;
		/**********************直线模式下速度轮的控制*********************/		
		case LineType:
		{
			for(u8 i=0;i<4;i++)
				stWheel_SpeedPid[i].m_fpUMax=8000;
			
			stWheel_SpeedPid[0].m_fpDes=-(g_StDbus.stRC.Ch1-1024);
			stWheel_SpeedPid[1].m_fpDes=(g_StDbus.stRC.Ch1-1024);
			stWheel_SpeedPid[2].m_fpDes=(g_StDbus.stRC.Ch1-1024);
			stWheel_SpeedPid[3].m_fpDes=-(g_StDbus.stRC.Ch1-1024);
			if(g_StDbus.stRC.Ch0-1024<0)
			{
				stWheel_SpeedPid[0].m_fpDes+=(g_StDbus.stRC.Ch0-1024);
				stWheel_SpeedPid[3].m_fpDes=stWheel_SpeedPid[0].m_fpDes;
			}
			else if(g_StDbus.stRC.Ch0-1024>0)
			{
				stWheel_SpeedPid[1].m_fpDes=(g_StDbus.stRC.Ch0-1024);
				stWheel_SpeedPid[2].m_fpDes=stWheel_SpeedPid[1].m_fpDes;
			}	
			for(u8 i=0;i<4;i++)
			{
				stWheel_SpeedPid[i].m_fpDes*=SpeedWheelRate;
			}
		}break;
		/*********************安全模式下速度轮的控制**********************/
		case SafeType:
		{
			for(u8 i=0;i<4;i++)
			{
				stWheel_SpeedPid[i].m_fpUMax=0;
			}
		}break;
		
		default:
		break;
	}
	SpeedWheelControl();
}


void ChassisModeChosse()
{
	switch(ControlMode)
	{
		case 0x00:
		{
			ServoMode(SafeType);
			SpeedMode(SafeType);
		}
		break;
		case 0x01:
		{
			ServoMode(ForceFBType);
			SpeedMode(SafeType);
		}
		break;
	
		case 0x02:
		{
			ServoMode(LineType);
			SpeedMode(LineType);
		}
		break;

		case 0x03:
		{
			ServoMode(GyroType);
			SpeedMode(GyroType);
		}	
		break;	
	
		case 0x04:
		{

		}
		break;
		

		
		default:
		break;
	}
	CAN_SendData(CAN1,0x0200,stServoWheel_SpeedPid[0].m_fpU,stWheel_SpeedPid[1].m_fpU,
													 stWheel_SpeedPid[2].m_fpU,stServoWheel_SpeedPid[3].m_fpU);
	CAN_SendData(CAN2,0x0200,stWheel_SpeedPid[0].m_fpU,stServoWheel_SpeedPid[1].m_fpU,
													 stServoWheel_SpeedPid[2].m_fpU,stWheel_SpeedPid[3].m_fpU);
}



void PowerLoopControl()
{
	capacitor_msg.TxPower=MaxPower;
	CAN_SendData(CAN1,0x1FF,0,capacitor_msg.TxPower*100,0,0);
	ChassisPowerPid.m_fpDes=capacitor_msg.Pow_In;
	ChassisPowerPid.m_fpFB=capacitor_msg.Pow_Out;
	
	CalIWeakenPID(&ChassisPowerPid);
	speed_rate=ChassisPowerPid.m_fpU/MaxPower+1;
	for(u8 i=0;i<4;i++)
		stWheel_SpeedPid[i].m_fpDes*=speed_rate;
	for(u8 i=0;i<4;i++)
		CalIWeakenPID(&stWheel_SpeedPid[i]);

	CAN_SendData(CAN1,0x200,stWheel_SpeedPid[0].m_fpU,stWheel_SpeedPid[1].m_fpU,stWheel_SpeedPid[2].m_fpU,stWheel_SpeedPid[3].m_fpU);

}


///*----------------------------------------------------------------------------------------
//                                功率限制变量
//----------------------------------------------------------------------------------------*/
//Enum_CAP_STATE CAP_STATE ;                       //电容功率判断
//Enum_CAP_STATE CAP_STATE_Pre;

//int Super_CAP_Change_Cnt;
//u8 Super_CAP_Change_Flag;
//u8 Super_CAP_Break_Flag = 0 ;

//bool Is_Tank;
//bool FLAG_CAP_Vol_Warning;
//bool FLAG_CAP_Vol_Dangerous;
//bool Cap_Is_Wraning;
//bool Chassis_Is_Start_Wraning;
//bool Chassis_Is_Starting;
//bool Chassis_Is_Climbing_Wraning;
//bool Chassis_Is_Rushing_Wraning;
//bool FLAG_CAP_Pow;

//float PrePower_mes = 0;
//float nextPowerBuffer_mes = 0;                   //下一时刻的底盘功率缓冲的预测
//float my_Watch_Coe = 0.93;
//float cur_limit;

//double CAP_Cur_Coe;
//double CAP_POW_USE;
///*----------------------------------------------------------------------------------------
//函数功能：底盘遥控器模式
//----------------------------------------------------------------------------------------*/
//float fpVx_pow ;   //X方向三次幂速度
//float fpVy_pow ;   //Y方向三次幂速度
//float fpW_pow ;    //角速度三次幂
//void Chassis_RC_Mode(void)
//{
//	X_Offset = g_stDBus.stRC.Ch2 - RC_CH_VALUE_OFFSET;        //通道值减去中间摇杆值
//  Y_Offset = g_stDBus.stRC.Ch3 - RC_CH_VALUE_OFFSET;
//	
//	RC_Simulation_RampSignal();                               //遥控器模拟斜坡信号
// 
//	RC_Velt_X_RampSignal();                                   //X速度斜坡信号
//  RC_Velt_Y_RampSignal();                                   //Y速度斜坡信号
// 		
//	//小陀螺模式
//	if( ( g_stDBus.stRC.SW_L == RC_SW_MID && abs(X_Offset) > RC_CH_VALUE_CHASSIS_DEAD ) )  //左边开关在中间且X限定值大于一个区域
//  {
//		Chassis_RC_TOP();
//  }
//	else                                                                                   //底盘跟随模式
//  {
//		Chassis_Follow();                                                                    
//  }		
//}
//ST_PID Pow_ControlPID = QuickPID(1,0.1,0,10,200,200,0,1.0);
///*----------------------------------------------------------------------------------------
//                           所有速度处理函数
//----------------------------------------------------------------------------------------*/
///*-------------------------------------------------------------------------------------------------
//函数功能：底盘速度闭环函数
//-------------------------------------------------------------------------------------------------*/
//void Chassis_Velocity_Loop(void)
//{	
//		CalIWeakenPID(&g_stCM1_SpeedPID);
//		CalIWeakenPID(&g_stCM2_SpeedPID);
//		CalIWeakenPID(&g_stCM3_SpeedPID);
//		CalIWeakenPID(&g_stCM4_SpeedPID);
//}
///*-------------------------------------------------------------------------------------------------
//函数功能：底盘速度赋值
//-------------------------------------------------------------------------------------------------*/
//void Velt_Distribute(float fpVx,float fpVy,float fpW)
//{
//	  g_stCM1_SpeedPID.m_fpDes = -(-fpVx * Mec_Weel_Coe + fpVy * Mec_Weel_Coe + fpW * Mec_Weel_Row_Front_Left);   //单位rpm
//    g_stCM2_SpeedPID.m_fpDes =  ( fpVx * Mec_Weel_Coe + fpVy * Mec_Weel_Coe - fpW * Mec_Weel_Row_Front_Right);  //单位rpm
//    g_stCM3_SpeedPID.m_fpDes =  (-fpVx * Mec_Weel_Coe + fpVy * Mec_Weel_Coe - fpW * Mec_Weel_Row_Back_Left);    //单位rpm
//    g_stCM4_SpeedPID.m_fpDes = -( fpVx * Mec_Weel_Coe + fpVy * Mec_Weel_Coe + fpW * Mec_Weel_Row_Back_Right);   //单位rpm
//}
///*-------------------------------------------------------------------------------------------------
//函数功能：底盘中心实际速度计算
//-------------------------------------------------------------------------------------------------*/
//void Chassis_Real_Velt_Calculate(void)
//{
//    W_Real_Pre = W_Real ;
//		Vx_Real =( g_stCM1_SpeedPID.m_fpFB + g_stCM2_SpeedPID.m_fpFB - g_stCM3_SpeedPID.m_fpFB - g_stCM4_SpeedPID.m_fpFB ) / ( 4 * Mec_Weel_Coe ) ;                                                                                         //Vx=（v1+v2-v3-v4)/(4*轮子周长)
//		Vy_Real =( g_stCM2_SpeedPID.m_fpFB + g_stCM3_SpeedPID.m_fpFB - g_stCM1_SpeedPID.m_fpFB - g_stCM4_SpeedPID.m_fpFB ) / ( 4 * Mec_Weel_Coe ) ;
//		W_Real = g_stCM1_SpeedPID.m_fpFB/(-4*Mec_Weel_Row_Front_Left)+g_stCM2_SpeedPID.m_fpFB/(-4*Mec_Weel_Row_Front_Right)+g_stCM3_SpeedPID.m_fpFB/(-4*Mec_Weel_Row_Back_Left)+g_stCM4_SpeedPID.m_fpFB/(-4* Mec_Weel_Row_Back_Right );			//由4个轮子的真实速度解算出车的Vx，Vy，Vz
//}
///*-------------------------------------------------------------------------------------------------
//函数功能：底盘安全模式，速度全为0
//-------------------------------------------------------------------------------------------------*/
//void Chassis_Safe_Stop(void)
//{
//		st_velt.fpVx=0;
//		st_velt.fpVy=0;
//		st_velt.fpW=0;
//		fpVx_pow=0;
//		fpVy_pow=0;
//		fpW_pow =0;
//}
///*----------------------------------------------------------------------------------------
//函数功能：功率限制算法
//----------------------------------------------------------------------------------------*/
//float sum_cur = 0;
//float total_cur;
//float total_cur_limit = 0;
//float sum_p=0;
//float sum_i=0;
//float limit_coef=0;
//float temp_des1 =0;
//float temp_des2 =0;
//float temp_des3 =0;
//float temp_des4 =0;
//void Power_Limit_algorithm(float limit_cur,int limit_v)
//{
//	if( Power_mes > MaxPower )
//	{
//		 nextPowerBuffer_mes = PowerBuffer_mes - ( Power_mes - MaxPower ) * 0.1f ; //超功率，缓冲值减少(系数取0.1是因为要自己估算当前功率在100ms内的能量消耗)
//	}
//	else
//	{
//		 nextPowerBuffer_mes = PowerBuffer_mes ;                                   //不超功率，缓冲值不变（保守计算）
//	}
//	
//	total_cur_limit = limit_cur;
//  total_cur = fabs(g_stCM1_SpeedPID.m_fpU) + fabs(g_stCM2_SpeedPID.m_fpU)\
//              + fabs(g_stCM3_SpeedPID.m_fpU) + fabs(g_stCM4_SpeedPID.m_fpU);
//	sum_cur = total_cur;
//  sum_p = 0 ;
//	sum_i = 0 ;
//	limit_coef = 0 ;
//	temp_des1  = 0 ;
//	temp_des2  = 0 ;
//	temp_des3  = 0 ;
//	temp_des4  = 0 ;				                                                      //将这些数都归零
//	
//	if( total_cur > total_cur_limit )
//	{
//			if(    fabs( g_stCM1_SpeedPID.m_fpDes ) > 100.0f && fabs( g_stCM2_SpeedPID.m_fpDes ) > 100.0f \
//					&& fabs( g_stCM3_SpeedPID.m_fpDes ) > 100.0f && fabs( g_stCM4_SpeedPID.m_fpDes ) > 100.0f )//当有一个轮子的速度目标值到100时重新分配速度目标值
//			{
//					sum_p =   g_stCM1_SpeedPID.m_fpKp * g_stCM1_SpeedPID.m_fpDes * Sgn( g_stCM1_SpeedPID.m_fpE )\
//									+ g_stCM2_SpeedPID.m_fpKp * g_stCM2_SpeedPID.m_fpDes * Sgn( g_stCM2_SpeedPID.m_fpE )\
//									+ g_stCM3_SpeedPID.m_fpKp * g_stCM3_SpeedPID.m_fpDes * Sgn( g_stCM3_SpeedPID.m_fpE )\
//									+ g_stCM4_SpeedPID.m_fpKp * g_stCM4_SpeedPID.m_fpDes * Sgn( g_stCM4_SpeedPID.m_fpE ) ;
//					sum_i =   ( g_stCM1_SpeedPID.m_fpUi - g_stCM1_SpeedPID.m_fpKp * g_stCM1_SpeedPID.m_fpFB ) * Sgn( g_stCM1_SpeedPID.m_fpE )\
//									+ ( g_stCM2_SpeedPID.m_fpUi - g_stCM2_SpeedPID.m_fpKp * g_stCM2_SpeedPID.m_fpFB ) * Sgn( g_stCM2_SpeedPID.m_fpE )\
//									+ ( g_stCM3_SpeedPID.m_fpUi - g_stCM3_SpeedPID.m_fpKp * g_stCM3_SpeedPID.m_fpFB ) * Sgn( g_stCM3_SpeedPID.m_fpE )\
//									+ ( g_stCM4_SpeedPID.m_fpUi - g_stCM4_SpeedPID.m_fpKp * g_stCM4_SpeedPID.m_fpFB ) * Sgn( g_stCM4_SpeedPID.m_fpE );					
//					limit_coef = (total_cur_limit - sum_i)/sum_p;
//					
//					temp_des1 = limit_coef * g_stCM1_SpeedPID.m_fpDes;
//					temp_des2 = limit_coef * g_stCM2_SpeedPID.m_fpDes;
//					temp_des3 = limit_coef * g_stCM3_SpeedPID.m_fpDes;
//					temp_des4 = limit_coef * g_stCM4_SpeedPID.m_fpDes;
//					
//					g_stCM1_SpeedPID.m_fpDes = Clip( temp_des1 , -limit_v , limit_v ) ;
//					g_stCM2_SpeedPID.m_fpDes = Clip( temp_des2 , -limit_v , limit_v ) ;
//					g_stCM3_SpeedPID.m_fpDes = Clip( temp_des3 , -limit_v , limit_v ) ;
//					g_stCM4_SpeedPID.m_fpDes = Clip( temp_des4 , -limit_v , limit_v ) ;
//					Chassis_Velocity_Loop();	           //PID计算
//			}
//			else
//			{
//					Chassis_Velocity_Loop();	           //PID计算
//					g_stCM1_SpeedPID.m_fpU = g_stCM1_SpeedPID.m_fpU / total_cur * total_cur_limit;
//					g_stCM2_SpeedPID.m_fpU = g_stCM2_SpeedPID.m_fpU / total_cur * total_cur_limit;
//					g_stCM3_SpeedPID.m_fpU = g_stCM3_SpeedPID.m_fpU / total_cur * total_cur_limit;
//					g_stCM4_SpeedPID.m_fpU = g_stCM4_SpeedPID.m_fpU / total_cur * total_cur_limit;
//			}
//  }
//	
//	if( nextPowerBuffer_mes <= 15 )          //缓冲值不足15J
//	{
//			g_stCM1_SpeedPID.m_fpU = 0;
//			g_stCM2_SpeedPID.m_fpU = 0;
//			g_stCM3_SpeedPID.m_fpU = 0;
//			g_stCM4_SpeedPID.m_fpU = 0;
//	}
//	else
//	{
//			g_stCM1_SpeedPID.fpUMax = MECANUM_CURRENT_MAX;
//			g_stCM2_SpeedPID.fpUMax = MECANUM_CURRENT_MAX;
//			g_stCM3_SpeedPID.fpUMax = MECANUM_CURRENT_MAX;
//			g_stCM4_SpeedPID.fpUMax = MECANUM_CURRENT_MAX;
//	}
//}
///*----------------------------------------------------------------------------------------
//函数功能：底盘速度功率Vx,Vy,Vw限制算法（优化）
//----------------------------------------------------------------------------------------*/
//float Limit_fpVx;
//float Limit_fpVy;
//float Limit_fpW;
//ST_PID g_stCM1_Limit_SpeedPID = ST_PID_INIT(120, 0.1, 0, MECANUM_CURRENT_MAX, 2000, 0, 30);
//ST_PID g_stCM2_Limit_SpeedPID = ST_PID_INIT(120, 0.1, 0, MECANUM_CURRENT_MAX, 2000, 0, 30);
//ST_PID g_stCM3_Limit_SpeedPID = ST_PID_INIT(120, 0.1, 0, MECANUM_CURRENT_MAX, 2000, 0, 30);
//ST_PID g_stCM4_Limit_SpeedPID = ST_PID_INIT(120, 0.1, 0, MECANUM_CURRENT_MAX, 2000, 0, 30);
//double CM1_Limit_Speed_Coe;
//double CM2_Limit_Speed_Coe;
//double CM3_Limit_Speed_Coe;
//double CM4_Limit_Speed_Coe;
//void Velt_Limit_Pow_Control(void)
//{
//	
//	  Limit_fpVx = CAP_Cur_Coe*st_velt.fpVx;
//	  Limit_fpVy = CAP_Cur_Coe*st_velt.fpVy;
//	  Limit_fpW  = CAP_Cur_Coe*st_velt.fpW ;
//	
//	  g_stCM1_Limit_SpeedPID.m_fpDes = -(-Limit_fpVx * Mec_Weel_Coe + Limit_fpVy * Mec_Weel_Coe + Limit_fpW * Mec_Weel_Row_Front_Left);   //单位rpm
//    g_stCM2_Limit_SpeedPID.m_fpDes =  ( Limit_fpVx * Mec_Weel_Coe + Limit_fpVy * Mec_Weel_Coe - Limit_fpW * Mec_Weel_Row_Front_Right);  //单位rpm
//    g_stCM3_Limit_SpeedPID.m_fpDes =  (-Limit_fpVx * Mec_Weel_Coe + Limit_fpVy * Mec_Weel_Coe - Limit_fpW * Mec_Weel_Row_Back_Left);    //单位rpm
//    g_stCM4_Limit_SpeedPID.m_fpDes = -( Limit_fpVx * Mec_Weel_Coe + Limit_fpVy * Mec_Weel_Coe + Limit_fpW * Mec_Weel_Row_Back_Right);   //单位rpm
//	
//    g_stCM1_Limit_SpeedPID.m_fpFB = g_stCM1_SpeedPID.m_fpFB;
//    g_stCM2_Limit_SpeedPID.m_fpFB = g_stCM2_SpeedPID.m_fpFB;
//    g_stCM3_Limit_SpeedPID.m_fpFB = g_stCM3_SpeedPID.m_fpFB;
//    g_stCM4_Limit_SpeedPID.m_fpFB = g_stCM4_SpeedPID.m_fpFB;

//		CalIWeakenPID(&g_stCM1_Limit_SpeedPID);
//		CalIWeakenPID(&g_stCM2_Limit_SpeedPID);
//		CalIWeakenPID(&g_stCM3_Limit_SpeedPID);
//		CalIWeakenPID(&g_stCM4_Limit_SpeedPID);
//	
//    CM1_Limit_Speed_Coe = g_stCM1_Limit_SpeedPID.m_fpU/g_stCM1_SpeedPID.m_fpU ;
//    CM2_Limit_Speed_Coe = g_stCM2_Limit_SpeedPID.m_fpU/g_stCM2_SpeedPID.m_fpU ;
//		CM3_Limit_Speed_Coe = g_stCM3_Limit_SpeedPID.m_fpU/g_stCM3_SpeedPID.m_fpU ;
//		CM4_Limit_Speed_Coe = g_stCM4_Limit_SpeedPID.m_fpU/g_stCM4_SpeedPID.m_fpU ;
//	
//	  g_stCM1_SpeedPID.m_fpU *= CM1_Limit_Speed_Coe ;
//	  g_stCM2_SpeedPID.m_fpU *= CM2_Limit_Speed_Coe ;
//	  g_stCM3_SpeedPID.m_fpU *= CM3_Limit_Speed_Coe ;
//	  g_stCM4_SpeedPID.m_fpU *= CM4_Limit_Speed_Coe ;
//	

//}


