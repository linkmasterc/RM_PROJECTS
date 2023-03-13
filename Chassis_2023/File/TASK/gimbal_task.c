#include "gimbal_task.h"
#include "rm_algorithm.h"
#include "ParamConfig.h"
#include "global_declare.h"
#define ABS(x) ( (x)>0?(x):-(x) )
u8 judgeTD=0;
u8 yaw_span_type=0;
u8 pit_span_type=0;
float yaw_ramp_step=0.02;
float pit_ramp_step=0.05;
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
  * @brief  云台自动巡逻（正弦巡逻）
			
  * @note	
 -------------------------------------------------------------------------- **/
void GimbalAutoScan(void)
{
	if(yaw_span_type==1)
	{
		FPRampSignal(&YawPosDes,60,yaw_ramp_step);
		if(YawPosDes>=60)
			yaw_span_type=0;
	}
	else if(yaw_span_type==0)
	{
		FPRampSignal(&YawPosDes,-60,yaw_ramp_step);
		if(YawPosDes<=-60)
			yaw_span_type=1;
	}
		
		
	if(pit_span_type==1)
	{
		FPRampSignal(&GimbalPitchPosPid.m_fpDes,-30,pit_ramp_step);
		if(GimbalPitchPosPid.m_fpDes<=0)
			pit_span_type=0;
	}
	else if(pit_span_type==0)
	{
		FPRampSignal(&GimbalPitchPosPid.m_fpDes,30,pit_ramp_step);
		if(GimbalPitchPosPid.m_fpDes>=40)
			pit_span_type=1;
	}
}

void GimbalFollowAim(void)
{
	if(VisionDataReceiveBuff.stVisionData.Recieve_ID==1)
	{
		YawPosDes=VisionDataReceiveBuff.stVisionData.Recieve_Data2;
		GimbalPitchPosPid.m_fpDes=-VisionDataReceiveBuff.stVisionData.Recieve_Data1;
	}
	else 
	{
		YawPosDes=GimbalYawPosPid.m_fpFB;
		GimbalPitchPosPid.m_fpDes=-GimbalPitchPosPid.m_fpFB;
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
	
	if(stFlag.GimbalRunFlag==TRUE&&stFlag.SniperFlag==FALSE)
	{
		GimbalAutoScan();
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
	GimbalYawSpeedPid.m_fpFB=YawBMISpeed;
	CalIWeakenPID(&GimbalYawPosPid);													// 得到位置环PID输出（只用PI控制，D使用TD跟踪微分器）
	GimbalYawSpeedPid.m_fpDes = GimbalYawPosPid.m_fpU ;		
	CalIWeakenPID(&GimbalYawSpeedPid);													// 得到速度环PID输出
	GimbalYawSpeedPid.m_fpU =-GimbalYawSpeedPid.m_fpU;

}

void GimbalModeChoose()
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
	else if(ControlMode==0x02||ControlMode==0x03)
	{
		FPRampSignal(&GimbalYawSpeedPid.m_fpUMax,28000,10);
		GimbalFollowAim();
	}
	DNYawPID();
	CAN_SendData(CAN2,0x1ff, GimbalYawSpeedPid.m_fpU,0,0,-(s16)(c_stShooterSpeedPID.m_fpU));
}

