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
	/*����̨�Ӿ����ݷ���*/
	#ifdef DN_CAMERA_HARDWARE_TRIGER_ENABLE
	if(TirggerState)
	#else
	if(TRUE)
	#endif
	{
		DNVisionDataSend();
		systemMonitor.VisionDataSendTask_cnt++;
	}	
}
void VofaPlusTask()
{
	VofaDataScan();
}

