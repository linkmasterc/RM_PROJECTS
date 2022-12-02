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
	GimbalRCMode();
	systemMonitor.GimbalTask_cnt++;
}

void MotorDataSendTask(void)
{
	DNYawPID();
	CAN_SendData(CAN2,0x1ff,GimbalYawSpeedPid.m_fpU,0,0,0);
	systemMonitor.MotorDataSendTask_cnt++;
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

