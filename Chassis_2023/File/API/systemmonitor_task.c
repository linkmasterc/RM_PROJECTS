#include "systemmonitor_task.h"
#include "global_declare.h"
#include "mode_choose_task.h"

RCC_ClocksTypeDef get_rcc_clock;


 /** --------------------------------------------------------------------------
  * @brief  ϵͳ�����Լ��ж�֡�ʼ��
			
  * @note	
 -------------------------------------------------------------------------- **/
void SystemMonitorDetect(void)
{	
	RCC_GetClocksFreq(&get_rcc_clock);

	/*����ִ��ʱ��ͳ��*/
	systemMonitor.TaskTotalTime = 0;
	
	for(u32 i=0; i<TotalTaskNum; i++) systemMonitor.TaskTotalTime += G_ST_Task[i].TimeUsed;						// ����ϵͳ����ִ����ʱ��
	
	systemMonitor.TaskTotalTimeMost = systemMonitor.TaskTotalTimeMost > systemMonitor.TaskTotalTime ?			// ϵͳ����ִ����ʱ������ֵ
										systemMonitor.TaskTotalTimeMost : systemMonitor.TaskTotalTime;

	/*֡���쳣���*/
	if(systemMonitor.USART3_rx_valid_fps < 500)	{	stError.DNGimbalState = TRUE;}								// ���ƿ�ͨ��֡���쳣���
	else										{	stError.DNGimbalState = FALSE;}
	
	if(systemMonitor.UART4_rx_valid_fps < 500)	{	stError.UPGimbalState = TRUE;}								// ���ƿ�ͨ��֡���쳣���
	else										{	stError.UPGimbalState = FALSE;}
	
	if(systemMonitor.USART2_rx_valid_fps < 10)	{	stError.RSState = TRUE;}									// ����ϵͳͨ��֡���쳣���
	else										{	stError.RSState = FALSE;}
	
	if(systemMonitor.CAN1_rx_fps < 1500)		{	stError.Can1State = TRUE;}									// CAN1ͨ��֡���쳣���
	else										{	stError.Can1State = FALSE;}

	if(systemMonitor.CAN2_rx_fps < 2500)		{	stError.Can2State = TRUE;}									// CAN2ͨ��֡���쳣���
	else										{	stError.Can2State = FALSE;}

	if(SubSystemMonitor.CAN1_CapacitorMode_rx_fps < 800)		{	stError.CapacitorState = TRUE;}				// ����ͨ���쳣���
	else										{	stError.CapacitorState = FALSE;}
	
	if(systemMonitor.UART5_rx_valid_fps < 10)																	// ����̨С����ͨ��֡���쳣���
	{	
		stError.UPComputerState = TRUE;
		UPVisionDataReceiveBuff.stVisionData.Recieve_ID = 0;													// ֡���쳣ʱ��Ϊδʶ��Ŀ��
	}
	else	{	stError.UPComputerState = FALSE;}
	
	if(systemMonitor.USART6_rx_valid_fps < 10)																	// ����̨С����ͨ��֡���쳣���
	{
		stError.DNComputerState = TRUE;
		DNVisionDataReceiveBuff.stVisionData.Recieve_ID = 0;													// ֡���쳣ʱ��Ϊδʶ��Ŀ��
	}
	else	{	stError.DNComputerState = FALSE;}
}
