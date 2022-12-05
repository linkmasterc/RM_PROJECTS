#include "chassis_task.h"
#define ABS(x) ( (x)>0?(x):-(x) )
float X_Vel;
float Y_Vel;
float W_Vel;

float X_Max_Vel;
float Y_Max_Vel;
float W_Max_Vel;

float Raw_Power;
float Max_Power;

s16 Raw_Sum_Current;
s16 Limit_Sum_Current;
void ChassisRCMode()
{
	//g_StDbus.stRC.Ch3;//y
	//g_StDbus.stRC.Ch2;//x
		/* ң����ͨ���������� */
	if(ABS(g_StDbus.stRC.Ch3 - RC_CH_VALUE_OFFSET) < RC_CH_VALUE_DEAD)
	{	g_StDbus.stRC.Ch0 = RC_CH_VALUE_OFFSET;}
	if(ABS(g_StDbus.stRC.Ch2 - RC_CH_VALUE_OFFSET) < RC_CH_VALUE_DEAD)
	{	g_StDbus.stRC.Ch1 = RC_CH_VALUE_OFFSET;}
	
}

///*----------------------------------------------------------------------------------------
//                                �������Ʊ���
//----------------------------------------------------------------------------------------*/
//Enum_CAP_STATE CAP_STATE ;                       //���ݹ����ж�
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
//float nextPowerBuffer_mes = 0;                   //��һʱ�̵ĵ��̹��ʻ����Ԥ��
//float my_Watch_Coe = 0.93;
//float cur_limit;

//double CAP_Cur_Coe;
//double CAP_POW_USE;
///*----------------------------------------------------------------------------------------
//�������ܣ�����ң����ģʽ
//----------------------------------------------------------------------------------------*/
//float fpVx_pow ;   //X�����������ٶ�
//float fpVy_pow ;   //Y�����������ٶ�
//float fpW_pow ;    //���ٶ�������
//void Chassis_RC_Mode(void)
//{
//	X_Offset = g_stDBus.stRC.Ch2 - RC_CH_VALUE_OFFSET;        //ͨ��ֵ��ȥ�м�ҡ��ֵ
//  Y_Offset = g_stDBus.stRC.Ch3 - RC_CH_VALUE_OFFSET;
//	
//	RC_Simulation_RampSignal();                               //ң����ģ��б���ź�
// 
//	RC_Velt_X_RampSignal();                                   //X�ٶ�б���ź�
//  RC_Velt_Y_RampSignal();                                   //Y�ٶ�б���ź�
// 		
//	//С����ģʽ
//	if( ( g_stDBus.stRC.SW_L == RC_SW_MID && abs(X_Offset) > RC_CH_VALUE_CHASSIS_DEAD ) )  //��߿������м���X�޶�ֵ����һ������
//  {
//		Chassis_RC_TOP();
//  }
//	else                                                                                   //���̸���ģʽ
//  {
//		Chassis_Follow();                                                                    
//  }		
//}
//ST_PID Pow_ControlPID = QuickPID(1,0.1,0,10,200,200,0,1.0);
///*----------------------------------------------------------------------------------------
//                           �����ٶȴ�����
//----------------------------------------------------------------------------------------*/
///*-------------------------------------------------------------------------------------------------
//�������ܣ������ٶȱջ�����
//-------------------------------------------------------------------------------------------------*/
//void Chassis_Velocity_Loop(void)
//{	
//		CalIWeakenPID(&g_stCM1_SpeedPID);
//		CalIWeakenPID(&g_stCM2_SpeedPID);
//		CalIWeakenPID(&g_stCM3_SpeedPID);
//		CalIWeakenPID(&g_stCM4_SpeedPID);
//}
///*-------------------------------------------------------------------------------------------------
//�������ܣ������ٶȸ�ֵ
//-------------------------------------------------------------------------------------------------*/
//void Velt_Distribute(float fpVx,float fpVy,float fpW)
//{
//	  g_stCM1_SpeedPID.m_fpDes = -(-fpVx * Mec_Weel_Coe + fpVy * Mec_Weel_Coe + fpW * Mec_Weel_Row_Front_Left);   //��λrpm
//    g_stCM2_SpeedPID.m_fpDes =  ( fpVx * Mec_Weel_Coe + fpVy * Mec_Weel_Coe - fpW * Mec_Weel_Row_Front_Right);  //��λrpm
//    g_stCM3_SpeedPID.m_fpDes =  (-fpVx * Mec_Weel_Coe + fpVy * Mec_Weel_Coe - fpW * Mec_Weel_Row_Back_Left);    //��λrpm
//    g_stCM4_SpeedPID.m_fpDes = -( fpVx * Mec_Weel_Coe + fpVy * Mec_Weel_Coe + fpW * Mec_Weel_Row_Back_Right);   //��λrpm
//}
///*-------------------------------------------------------------------------------------------------
//�������ܣ���������ʵ���ٶȼ���
//-------------------------------------------------------------------------------------------------*/
//void Chassis_Real_Velt_Calculate(void)
//{
//    W_Real_Pre = W_Real ;
//		Vx_Real =( g_stCM1_SpeedPID.m_fpFB + g_stCM2_SpeedPID.m_fpFB - g_stCM3_SpeedPID.m_fpFB - g_stCM4_SpeedPID.m_fpFB ) / ( 4 * Mec_Weel_Coe ) ;                                                                                         //Vx=��v1+v2-v3-v4)/(4*�����ܳ�)
//		Vy_Real =( g_stCM2_SpeedPID.m_fpFB + g_stCM3_SpeedPID.m_fpFB - g_stCM1_SpeedPID.m_fpFB - g_stCM4_SpeedPID.m_fpFB ) / ( 4 * Mec_Weel_Coe ) ;
//		W_Real = g_stCM1_SpeedPID.m_fpFB/(-4*Mec_Weel_Row_Front_Left)+g_stCM2_SpeedPID.m_fpFB/(-4*Mec_Weel_Row_Front_Right)+g_stCM3_SpeedPID.m_fpFB/(-4*Mec_Weel_Row_Back_Left)+g_stCM4_SpeedPID.m_fpFB/(-4* Mec_Weel_Row_Back_Right );			//��4�����ӵ���ʵ�ٶȽ��������Vx��Vy��Vz
//}
///*-------------------------------------------------------------------------------------------------
//�������ܣ����̰�ȫģʽ���ٶ�ȫΪ0
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
//�������ܣ����������㷨
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
//		 nextPowerBuffer_mes = PowerBuffer_mes - ( Power_mes - MaxPower ) * 0.1f ; //�����ʣ�����ֵ����(ϵ��ȡ0.1����ΪҪ�Լ����㵱ǰ������100ms�ڵ���������)
//	}
//	else
//	{
//		 nextPowerBuffer_mes = PowerBuffer_mes ;                                   //�������ʣ�����ֵ���䣨���ؼ��㣩
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
//	temp_des4  = 0 ;				                                                      //����Щ��������
//	
//	if( total_cur > total_cur_limit )
//	{
//			if(    fabs( g_stCM1_SpeedPID.m_fpDes ) > 100.0f && fabs( g_stCM2_SpeedPID.m_fpDes ) > 100.0f \
//					&& fabs( g_stCM3_SpeedPID.m_fpDes ) > 100.0f && fabs( g_stCM4_SpeedPID.m_fpDes ) > 100.0f )//����һ�����ӵ��ٶ�Ŀ��ֵ��100ʱ���·����ٶ�Ŀ��ֵ
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
//					Chassis_Velocity_Loop();	           //PID����
//			}
//			else
//			{
//					Chassis_Velocity_Loop();	           //PID����
//					g_stCM1_SpeedPID.m_fpU = g_stCM1_SpeedPID.m_fpU / total_cur * total_cur_limit;
//					g_stCM2_SpeedPID.m_fpU = g_stCM2_SpeedPID.m_fpU / total_cur * total_cur_limit;
//					g_stCM3_SpeedPID.m_fpU = g_stCM3_SpeedPID.m_fpU / total_cur * total_cur_limit;
//					g_stCM4_SpeedPID.m_fpU = g_stCM4_SpeedPID.m_fpU / total_cur * total_cur_limit;
//			}
//  }
//	
//	if( nextPowerBuffer_mes <= 15 )          //����ֵ����15J
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
//�������ܣ������ٶȹ���Vx,Vy,Vw�����㷨���Ż���
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
//	  g_stCM1_Limit_SpeedPID.m_fpDes = -(-Limit_fpVx * Mec_Weel_Coe + Limit_fpVy * Mec_Weel_Coe + Limit_fpW * Mec_Weel_Row_Front_Left);   //��λrpm
//    g_stCM2_Limit_SpeedPID.m_fpDes =  ( Limit_fpVx * Mec_Weel_Coe + Limit_fpVy * Mec_Weel_Coe - Limit_fpW * Mec_Weel_Row_Front_Right);  //��λrpm
//    g_stCM3_Limit_SpeedPID.m_fpDes =  (-Limit_fpVx * Mec_Weel_Coe + Limit_fpVy * Mec_Weel_Coe - Limit_fpW * Mec_Weel_Row_Back_Left);    //��λrpm
//    g_stCM4_Limit_SpeedPID.m_fpDes = -( Limit_fpVx * Mec_Weel_Coe + Limit_fpVy * Mec_Weel_Coe + Limit_fpW * Mec_Weel_Row_Back_Right);   //��λrpm
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


