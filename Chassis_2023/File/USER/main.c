#include "main.h"



int main()
{
	BSP_Init();
	OS_RUN();
}



/** --------------------------------------------------------------------------
	* @brief  系统监视任务
			
	* @note	1Hz
 -------------------------------------------------------------------------- **/
void SystemMonitorTask(void)
{
	SystemMonitorDetect();																				// 系统任务执行时间帧率检测
	systemMonitor.SystemMonitorTask_cnt++;
}

void GimbalTask(void)
{
	GimbalModeChoose();
	systemMonitor.GimbalTask_cnt++;
}


void SendDataTask(void)
{
	GimbalSendDataProtocol();
	systemMonitor.GimbalDataSendTask_cnt++;																
}

/** --------------------------------------------------------------------------
	* @brief  控制模式选择任务
			
	* @note	1000Hz
 -------------------------------------------------------------------------- **/
void ModeChooseTask(void)
{
	ModeChoose();																						// 选择控制模式
	
	systemMonitor.ModeChooseTask_cnt++;																	// 任务计数
}	
/** --------------------------------------------------------------------------
	* @brief  	射击任务
			
	* @note		1000Hz
 -------------------------------------------------------------------------- **/
void ShootTask(void)
{
	ShootBullet();
//	CalIResistedPID(&c_stShooterPosPID);
//	c_stShooterSpeedPID.m_fpDes = c_stShooterPosPID.m_fpU / 8192 * 360.0f;
//	CalIWeakenPID(&c_stShooterSpeedPID);
//	CAN_SendData(CAN2,0x1ff, GimbalYawSpeedPid.m_fpU,0,0,-(s16)(c_stShooterSpeedPID.m_fpU));
	systemMonitor.ShootTask_cnt++;																		
}



/** --------------------------------------------------------------------------
	* @brief  	LED指示灯状态切换任务
			
	* @note		1000 * 20Hz
 -------------------------------------------------------------------------- **/
void LEDStateChangeTask(void)
{
	LEDStateChange();
	
	systemMonitor.LEDStateChangeTask_cnt++;
}

/** --------------------------------------------------------------------------
	* @brief  	视觉数据发送任务

	* @note		1000Hz
				
 -------------------------------------------------------------------------- **/
void VisionDataSendTask(void)
{
//	/*下云台视觉数据发送*/
//	#ifdef DN_CAMERA_HARDWARE_TRIGER_ENABLE
//	if(TirggerState)
//	#else
//	if(TRUE)
//	#endif
	{
		DNVisionDataSend();
		systemMonitor.VisionDataSendTask_cnt++;
	}	
}
void VofaPlusTask()
{
	VofaDataScan();
}

void ChassisTask(void)
{
	ChassisModeChosse();
	ServoWheelControl();
	
}

void TriggerTask(void)
{
	
	Trigger_HIGH();
	delay_us(10);
	Trigger_LOW();
//	delay_us(10);
//	Trigger_HIGH();
//	delay_us(10);
//	Trigger_LOW();
	systemMonitor.TriggerTask_cnt++;
}
