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
	GimbalModeChoose();
	systemMonitor.GimbalTask_cnt++;
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

/** --------------------------------------------------------------------------
	* @brief  	�Ӿ����ݷ�������

	* @note		1000Hz
				
 -------------------------------------------------------------------------- **/
void VisionDataSendTask(void)
{
		DNVisionDataSend();
		systemMonitor.VisionDataSendTask_cnt++;
}

void NavigationDataSendTask(void)
{
	DecisionDataSendProtocol();
}
void VofaPlusTask()
{
	VofaDataScan();
}

void ChassisTask(void)
{
	ChassisModeChosse();	
}

void TriggerTask(void)
{
	
	Trigger_HIGH();
	delay_us(10);
	Trigger_LOW();
	systemMonitor.TriggerTask_cnt++;
}
