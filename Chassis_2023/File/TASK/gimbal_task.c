#include "gimbal_task.h"
#include "rm_algorithm.h"
#include "ParamConfig.h"
#include "global_declare.h"
#define ABS(x) ( (x)>0?(x):-(x) )
u8 judgeTD=0;
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
		if(stFlag.DN_RunFlag == TRUE)
		{
			if(TestFlag)
			{
				GimbalPitchPosPid.m_fpDes -= ((float)(g_StDbus.stRC.Ch1 - RC_CH_VALUE_OFFSET)/RC_CH_VALUE_RANGE/GM_PITCH_SENSIBILITY_TEST);
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
		if(stFlag.DN_RunFlag == TRUE)
		{	
			if(TestFlag)
			{
				YawPosDes+= ((float)(g_StDbus.stRC.Ch0 - RC_CH_VALUE_OFFSET)/RC_CH_VALUE_RANGE * GM_YAW_SENSIBILITY_TEST) * 360/8192.0f/1400 * 1.5f;
			}
			else
			{
				YawPosDes+= ((float)(g_StDbus.stRC.Ch0 - RC_CH_VALUE_OFFSET)/RC_CH_VALUE_RANGE * GM_YAW_SENSIBILITY) * 360/8192.0f/1400 * 1.5f;
			}
		}
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
void DNYawPID(void)
{	
	if(judgeTD==1)
	{
		YawTD.m_aim=YawPosDes;
		CalTD(&YawTD);
		GimbalYawPosPid.m_fpDes=YawTD.m_x1;
	}
	else
	{
		GimbalYawPosPid.m_fpDes=YawPosDes;
	}
	GimbalYawPosPid.m_fpFB=YawBMIAngle;
	GimbalYawSpeedPid.m_fpFB=YawBMISpeed;
	CalIWeakenPID(&GimbalYawPosPid);													// 得到位置环PID输出（只用PI控制，D使用TD跟踪微分器）
	GimbalYawSpeedPid.m_fpDes = GimbalYawPosPid.m_fpU ;		
	CalIWeakenPID(&GimbalYawSpeedPid);													// 得到速度环PID输出
	GimbalYawSpeedPid.m_fpU = GimbalYawSpeedPid.m_fpU;
}


