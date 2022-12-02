#include "main.h"



int main()
{
	BSP_Init();
	OS_RUN();
}



/** --------------------------------------------------------------------------
	* @brief  ϵͳ��������
			
	* @note	1Hz
 -------------------------------------------------------------------------- **/
void SystemMonitorTask(void)
{
	SystemMonitorDetect();																				// ϵͳ����ִ��ʱ��֡�ʼ��
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
	* @brief  ����ģʽѡ������
			
	* @note	1000Hz
 -------------------------------------------------------------------------- **/
void ModeChooseTask(void)
{
	ModeChoose();																						// ѡ�����ģʽ
	
	systemMonitor.ModeChooseTask_cnt++;																	// �������
}	
/** --------------------------------------------------------------------------
	* @brief  	�������
			
	* @note		1000Hz
 -------------------------------------------------------------------------- **/
void ShootTask(void)
{
	ShootBullet();
	
	systemMonitor.ShootTask_cnt++;																		
}



/** --------------------------------------------------------------------------
	* @brief  	LEDָʾ��״̬�л�����
			
	* @note		1000 * 20Hz
 -------------------------------------------------------------------------- **/
void LEDStateChangeTask(void)
{
	LEDStateChange();
	
	systemMonitor.LEDStateChangeTask_cnt++;
}

