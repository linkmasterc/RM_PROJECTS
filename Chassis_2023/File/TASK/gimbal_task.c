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
FixedPos GimbalFixedPos;
/** --------------------------------------------------------------------------
  * @brief  遥控器控制云台运动
			
  * @note	遥控器遥控范围不对YAW轴角度进行限制
 -------------------------------------------------------------------------- **/
void GimbalRCMode(void)
{
	static bool TestFlag = FALSE;
	
	if(1)
	{
		TestFlag = TRUE;
	}
	
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
			if(TestFlag)
			{
				GimbalPitchPosPid.m_fpDes -= (((float)(g_StDbus.stRC.Ch1 - RC_CH_VALUE_OFFSET)/RC_CH_VALUE_RANGE)/GM_PITCH_SENSIBILITY_TEST);
			}
			else
			{
				GimbalPitchPosPid.m_fpDes -= ((float)(g_StDbus.stRC.Ch1 - RC_CH_VALUE_OFFSET)/RC_CH_VALUE_RANGE/GM_PITCH_SENSIBILITY);
			}
			GimbalPitchPosPid.m_fpDes = Clip(GimbalPitchPosPid.m_fpDes, DN_PITCH_RC_ANGLE_MIN, DN_PITCH_RC_ANGLE_MAX);
		}
	}

	
	/* 遥控器通道0控制云台YAW轴位置 */
	if(g_StDbus.stRC.Ch0 - RC_CH_VALUE_OFFSET != 0 )		
	{	
		if(stFlag.GimbalRunFlag == TRUE)
		{	
			if(TestFlag)
			{
				YawPosDes+= (((float)(g_StDbus.stRC.Ch0 - RC_CH_VALUE_OFFSET)/RC_CH_VALUE_RANGE) * GM_YAW_SENSIBILITY_TEST);
			}
			else
			{
				YawPosDes+= ((float)(g_StDbus.stRC.Ch0 - RC_CH_VALUE_OFFSET)/RC_CH_VALUE_RANGE * GM_YAW_SENSIBILITY) * 360/8192.0f/1400 * 1.5f;
			}
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
		case UP_FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.FR;
			GimbalPitchPosPid.m_fpDes 	= GimbalFixedPos.PitchSite1.FR;
			EnemyPos				= 1;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;

		/*高地右侧*/
		case 2*UP_FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.HR;
			GimbalPitchPosPid.m_fpDes 	= GimbalFixedPos.PitchSite1.HR;
			EnemyPos				= 2;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;

		/*高地正中*/
		case 3*UP_FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.HM;
			GimbalPitchPosPid.m_fpDes 	= GimbalFixedPos.PitchSite1.HM;
			EnemyPos				= 3;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;

		/*高地左侧*/
		case 4*UP_FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.HL;
			GimbalPitchPosPid.m_fpDes 	= GimbalFixedPos.PitchSite1.HL;
			EnemyPos				= 4;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;

		/*左道远点*/
		case 5*UP_FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.FL;
			GimbalPitchPosPid.m_fpDes 	= GimbalFixedPos.PitchSite1.FL;
			EnemyPos				= 5;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;

		/*左道近点*/
		case 6*UP_FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.CL;
			GimbalPitchPosPid.m_fpDes 	= GimbalFixedPos.PitchSite1.CL;
			EnemyPos				= 6;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;

		/*中间近点*/
		case 7*UP_FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.CM;
			GimbalPitchPosPid.m_fpDes 	= GimbalFixedPos.PitchSite1.CM;
			EnemyPos				= 7;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;

		/*右道近点*/
		case 8*UP_FIXED_PATROL_GAPTIME:
		{
			YawPosDes 			= GimbalFixedPos.YawSite1.CR;
			GimbalPitchPosPid.m_fpDes 	= GimbalFixedPos.PitchSite1.CR;
			EnemyPos				= 8;
			FixedPatrolTime		= systemMonitor.SysTickTime;
		}break;

		default:
			break;
	}
}

/** --------------------------------------------------------------------------
	* @brief	上云台跟随模式
			
	* @note		云台目标值取小电脑发回来的值
 -------------------------------------------------------------------------- **/
void GimbalFollowMove(void)
{
	if( systemMonitor.SysTickTime - FixedPatrolTime < UP_FIXED_PATROL_GAPTIME &&
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
		if(WaitTimes == 8*UP_FIXED_PATROL_GAPTIME)
		{
			WaitTimes = 0;
		}
		WaitCnt = 0;
		WaitTimes++;
	}
	

	if(stFlag.GimbalRunFlag && !stFlag.SniperFlag)									// 扫描巡逻状态
	{
		LossTimes++;																// 丢失次数累加
		FixedPosScan();
	}
	else if(stFlag.GimbalRunFlag && stFlag.SniperFlag)								// 跟随状态
	{
		LossTimes = 0;															// 丢失目标次数清零
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
	GimbalYawSpeedPid.m_fpU =-GimbalYawSpeedPid.m_fpU;

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
