#include "gimbal_task.h"
#include "rm_algorithm.h"
#include "ParamConfig.h"
#include "global_declare.h"
#define ABS(x) ( (x)>0?(x):-(x) )
u8 judgeTD=0;
u8 PatrolReductionRate=1;
u8 WaitCnt=0;
u8 EnemyPos=0;
u8 FixedPatrolTime=0;
u32 WaitTimes=0;
u32 LossTimes=0;
FixedPos GimbalFixedPos={.PitchSite1.FR = -6,.PitchSite2.FR = -16.74,.PitchSite3.FR = -16.74,
												 .PitchSite1.HR = -8,.PitchSite2.HR = -15.12,.PitchSite3.HR = -14.77,
												 .PitchSite1.HM = 0,.PitchSite2.HM = -18.24,.PitchSite3.HM = -18.02,
												 .PitchSite1.HL = -27,.PitchSite2.HL = -15.60,.PitchSite3.HL = -16.79,
												 .PitchSite1.FL = -9,.PitchSite2.FL = -15.90,.PitchSite3.FL = -17.09,
												 .PitchSite1.CL = -22,.PitchSite2.CL = -28.65,.PitchSite3.CL = -28.78,
												 .PitchSite1.CM = -22,.PitchSite2.CM = -30.72,.PitchSite3.CM = -30.85,
												 .PitchSite1.CR = -22,.PitchSite2.CR = -26.06,.PitchSite3.CR = -28.84,
												 .PitchSite1.BL = -15,
												 .PitchSite1.BR = -12,
												 .PitchSite1.BM = -16,
	
												 .YawSite1.FR = -40,.YawSite2.FR = 37.88,.YawSite3.FR = 44.30,
												 .YawSite1.HR = -98,.YawSite2.HR = 19.25,.YawSite3.HR = 28.65,
												 .YawSite1.HM = 0,.YawSite2.HM = 1.27,.YawSite3.HM = 15.51,
												 .YawSite1.HL = 100,.YawSite2.HL = -22.15,.YawSite3.HL = -9.54,
												 .YawSite1.FL = 60,.YawSite2.FL = -41.35,.YawSite3.FL = -33.75,
												 .YawSite1.CL = 60,.YawSite2.CL = -39.97,.YawSite3.CL = -23.37,
												 .YawSite1.CM = 0,.YawSite2.CM = -0.75,.YawSite3.CM = 29.75,
												 .YawSite1.CR = -40,.YawSite2.CR = 34.98,.YawSite3.CR = 47.63,
												 .YawSite1.BL = 127,
												 .YawSite1.BR = -133,
												 .YawSite1.BM = -178,
																																													};
/** --------------------------------------------------------------------------
  * @brief  遥控器控制云台运动
			
  * @note	遥控器遥控范围不对YAW轴角度进行限制
 -------------------------------------------------------------------------- **/
void GimbalRCMode(void)
{	
	/* 遥控器通道死区限制 */
	if(ABS(g_StDbus.stRC.Ch0 - RC_CH_VALUE_OFFSET) < RC_CH_VALUE_DEAD)
	{	g_StDbus.stRC.Ch0 = RC_CH_VALUE_OFFSET;}
	if(ABS(g_StDbus.stRC.Ch1 - RC_CH_VALUE_OFFSET) < RC_CH_VALUE_DEAD)
	{	g_StDbus.stRC.Ch1 = RC_CH_VALUE_OFFSET;}
	
	
	/* 遥控器通道1控制云台PITCH轴位置 */
	if(g_StDbus.stRC.Ch1 - RC_CH_VALUE_OFFSET != 0 )		
	{	
		if(stFlag.GimbalRunFlag == TRUE)
		{
			GimbalPitchPosPid.m_fpDes -= (((float)(g_StDbus.stRC.Ch1 - RC_CH_VALUE_OFFSET)/RC_CH_VALUE_RANGE)/GM_PITCH_SENSIBILITY_TEST);
			GimbalPitchPosPid.m_fpDes = Clip(GimbalPitchPosPid.m_fpDes, PITCH_RC_ANGLE_MIN, PITCH_RC_ANGLE_MAX);
		}
	}

	/* 遥控器通道0控制云台YAW轴位置 */
	if(g_StDbus.stRC.Ch0 - RC_CH_VALUE_OFFSET != 0 )		
	{	
		if(stFlag.GimbalRunFlag == TRUE)
		{
			YawPosDes-= (((float)(g_StDbus.stRC.Ch0 - RC_CH_VALUE_OFFSET)/RC_CH_VALUE_RANGE) * GM_YAW_SENSIBILITY_TEST);
		}
	}
}
/** --------------------------------------------------------------------------
	* @brief  云台定点扫描
			
	* @retval 
  
	* @param	

	* @note					
 -------------------------------------------------------------------------- **/
void FixedPosScan(void)
{	
	switch(WaitTimes)
	{
		/*右道远点*/
		case FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.FR;
			GimbalPitchPosPid.m_fpDes 	= -GimbalFixedPos.PitchSite1.FR;
			EnemyPos				= 1;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;

		/*高地右侧*/
		case 2*FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.HR;
			GimbalPitchPosPid.m_fpDes 	= -GimbalFixedPos.PitchSite1.HR;
			EnemyPos				= 2;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;

		/*高地正中*/
		case 3*FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.HM;
			GimbalPitchPosPid.m_fpDes 	= -GimbalFixedPos.PitchSite1.HM;
			EnemyPos				= 3;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;

		/*高地左侧*/
		case 4*FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.HL;
			GimbalPitchPosPid.m_fpDes 	= -GimbalFixedPos.PitchSite1.HL;
			EnemyPos				= 4;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;

		/*左道远点*/
		case 5*FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.FL;
			GimbalPitchPosPid.m_fpDes 	= -GimbalFixedPos.PitchSite1.FL;
			EnemyPos				= 5;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;

		/*左道近点*/
		case 6*FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.CL;
			GimbalPitchPosPid.m_fpDes 	= -GimbalFixedPos.PitchSite1.CL;
			EnemyPos				= 6;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;

		/*中间近点*/
		case 7*FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.CM;
			GimbalPitchPosPid.m_fpDes 	= -GimbalFixedPos.PitchSite1.CM;
			EnemyPos				= 7;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;

		/*右道近点*/
		case 8*FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.CR;
			GimbalPitchPosPid.m_fpDes 	= -GimbalFixedPos.PitchSite1.CR;
			EnemyPos				= 8;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;
		
		case 9*FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.BL;
			GimbalPitchPosPid.m_fpDes 	= -GimbalFixedPos.PitchSite1.BL;
			EnemyPos				= 9;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;
		
		case 10*FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.BR;
			GimbalPitchPosPid.m_fpDes 	= -GimbalFixedPos.PitchSite1.BR;
			EnemyPos				= 10;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;
		
		case 11*FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.BM;
			GimbalPitchPosPid.m_fpDes 	= -GimbalFixedPos.PitchSite1.BM;
			EnemyPos				= 11;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;
			
		default:
			break;
	}
}

/** --------------------------------------------------------------------------
	* @brief  云台连续正弦扫描
			
	* @retval 
  
	* @param	

	* @note					
 -------------------------------------------------------------------------- **/
void GimbalSinScan()
{
	static bool YawDirJudge=TRUE;
	static bool PitDirJudge=TRUE;
	if(YawDirJudge)
	{
		YawPosDes+=YAW_LINE_MOVE_SPEED;
		if(YawPosDes>YAW_SCAN_ANGLE_MAX-0.5f)
			YawDirJudge=FALSE;	
	}
	else
	{
		YawPosDes-=YAW_LINE_MOVE_SPEED;
		if(YawPosDes<YAW_SCAN_ANGLE_MIN+0.5f)
			YawDirJudge=TRUE;
	}

	
	if(PitDirJudge)
	{
		GimbalPitchPosPid.m_fpDes+=PITCH_LINE_MOVE_SPEED;
		if(GimbalPitchPosPid.m_fpDes>PITCH_SCAN_ANGLE_MAX-1.0f)
			PitDirJudge=FALSE;
	}
	else
	{
		GimbalPitchPosPid.m_fpDes-=PITCH_LINE_MOVE_SPEED;
		if(GimbalPitchPosPid.m_fpDes<PITCH_SCAN_ANGLE_MIN+1.0f)
			PitDirJudge=TRUE;
	}
}
/** --------------------------------------------------------------------------
	* @brief	上云台跟随模式
			
	* @note		云台目标值取小电脑发回来的值
 -------------------------------------------------------------------------- **/
void GimbalFollowMove(void)
{
	if( systemMonitor.SysTickTime - FixedPatrolTime < FIXED_PATROL_GAPTIME &&
		(ABS(YawPosDes-GimbalYawPosPid.m_fpFB) > 0.5f || 
		ABS(GimbalPitchPosPid.m_fpDes-GimbalPitchPosPid.m_fpFB) > 0.5f))						// 禁止跳跃过程中识别目标
	{
	}
	else
	{
		GimbalPitchPosPid.m_fpDes 		= VisionDataReceiveBuff.stVisionData.Recieve_Data1;	
		YawPosDes 				= VisionDataReceiveBuff.stVisionData.Recieve_Data2;
	}

}


/** --------------------------------------------------------------------------
  * @brief  云台自动巡逻模式选择
			
  * @note	
 -------------------------------------------------------------------------- **/
void GimbalAutoMode(void)
{
	
	if(VisionDataReceiveBuff.stVisionData.Recieve_ID == 0)
	{	stFlag.SniperFlag = FALSE;}
	else if(VisionDataReceiveBuff.stVisionData.Recieve_ID == 1)
	{	stFlag.SniperFlag = TRUE;}
	
	
	if(++WaitCnt == PatrolReductionRate)
	{
		if(WaitTimes == 12*FIXED_PATROL_GAPTIME)
		{
			WaitTimes = 0;
		}
		WaitCnt = 0;
		WaitTimes++;
	}
	

	if(stFlag.GimbalRunFlag && !stFlag.SniperFlag)	// 扫描巡逻状态
	{
		LossTimes++;																	// 丢失次数累加
		if(ControlMode==0x03)
			GimbalSinScan();
		else if(ControlMode==0x02)
			GimbalRCMode();
	}
	if(stFlag.GimbalRunFlag && stFlag.SniperFlag)		// 跟随状态
	{
		LossTimes = 0;																// 丢失目标次数清零
		GimbalFollowMove();														// 云台跟随目标运动
	}	
	
}

/** --------------------------------------------------------------------------
  * @brief  下云台yaw控制PID计算
  
  * @retval None
  
  * @param	None
			
  * @note	控制结构：TD微分器+PID位置环（外环）+PID速度环（内环）

			工作状态下DownTest为Flase,可通过置位DownTest进入调试状态
			调试状态仅使用双环PID,此状态用于对PID参数的整定
			工作状态下用于实战以及对TD微分跟踪器参数的整定
			离散信号模拟用于模拟视觉发送过来的目标值中夹杂的噪声
 -------------------------------------------------------------------------- **/
void YawPID(void)
{	
	if(judgeTD==1)
	{
		YawTD.m_aim=YawPosDes;
		CalTD(&YawTD);
		YawPosDes=YawTD.m_x1;
	}
	GimbalYawPosPid.m_fpDes=YawPosDes;
	GimbalYawSpeedPid.m_fpFB=YawBMISpeed;
	CalIWeakenPID(&GimbalYawPosPid);													// 得到位置环PID输出（只用PI控制，D使用TD跟踪微分器）
	GimbalYawSpeedPid.m_fpDes = GimbalYawPosPid.m_fpU ;		
	CalIWeakenPID(&GimbalYawSpeedPid);													// 得到速度环PID输出
//	GimbalYawSpeedPid.m_fpU =-GimbalYawSpeedPid.m_fpU;

}

void GimbalModeChoose()
{
	if(systemMonitor.USART3_rx_fps>=900&&systemMonitor.USART3_rx_fps<=1000)
	{
		if(ControlMode==0x00)
		{
			GimbalYawSpeedPid.m_fpUMax=0;
			YawPosDes=GimbalYawPosPid.m_fpFB;
		}
		else if(ControlMode==0x01)
		{
			FPRampSignal(&GimbalYawSpeedPid.m_fpUMax,28000,10);
			GimbalRCMode();
		}
		else if(ControlMode==0x02||ControlMode==0x03||ControlMode==0x09)
		{
			FPRampSignal(&GimbalYawSpeedPid.m_fpUMax,28000,10);
			GimbalAutoMode();
		}
	}
	else if(systemMonitor.USART3_rx_fps<900)
	{
		GimbalYawSpeedPid.m_fpUMax=0;
		YawPosDes=GimbalYawPosPid.m_fpFB;
	}
	YawPID();
	CAN_SendData(CAN2,0x1ff, GimbalYawSpeedPid.m_fpU,0,0,0);
}
