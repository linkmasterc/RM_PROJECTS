#include "systemmonitor_task.h"
#include "global_declare.h"
#include "mode_choose_task.h"

RCC_ClocksTypeDef get_rcc_clock;


 /** --------------------------------------------------------------------------
  * @brief  系统任务以及中断帧率检测
			
  * @note	
 -------------------------------------------------------------------------- **/
void SystemMonitorDetect(void)
{	
	RCC_GetClocksFreq(&get_rcc_clock);

	/*任务执行时间统计*/
	systemMonitor.TaskTotalTime = 0;
	
	for(u32 i=0; i<TotalTaskNum; i++) systemMonitor.TaskTotalTime += G_ST_Task[i].TimeUsed;						// 计算系统任务执行总时间
	
	systemMonitor.TaskTotalTimeMost = systemMonitor.TaskTotalTimeMost > systemMonitor.TaskTotalTime ?			// 系统任务执行总时间的最大值
										systemMonitor.TaskTotalTimeMost : systemMonitor.TaskTotalTime;

	/*帧率异常检测*/
	if(systemMonitor.USART3_rx_valid_fps < 500)	{	stError.DNGimbalState = TRUE;}								// 下云控通信帧率异常检测
	else										{	stError.DNGimbalState = FALSE;}
	
	if(systemMonitor.UART4_rx_valid_fps < 500)	{	stError.UPGimbalState = TRUE;}								// 上云控通信帧率异常检测
	else										{	stError.UPGimbalState = FALSE;}
	
	if(systemMonitor.USART2_rx_valid_fps < 10)	{	stError.RSState = TRUE;}									// 裁判系统通信帧率异常检测
	else										{	stError.RSState = FALSE;}
	
	if(systemMonitor.CAN1_rx_fps < 1500)		{	stError.Can1State = TRUE;}									// CAN1通信帧率异常检测
	else										{	stError.Can1State = FALSE;}

	if(systemMonitor.CAN2_rx_fps < 2500)		{	stError.Can2State = TRUE;}									// CAN2通信帧率异常检测
	else										{	stError.Can2State = FALSE;}

	if(SubSystemMonitor.CAN1_CapacitorMode_rx_fps < 800)		{	stError.CapacitorState = TRUE;}				// 电容通信异常检测
	else										{	stError.CapacitorState = FALSE;}
	
	if(systemMonitor.UART5_rx_valid_fps < 10)																	// 上云台小电脑通信帧率异常检测
	{	
		stError.UPComputerState = TRUE;
		UPVisionDataReceiveBuff.stVisionData.Recieve_ID = 0;													// 帧率异常时认为未识别到目标
	}
	else	{	stError.UPComputerState = FALSE;}
	
	if(systemMonitor.USART6_rx_valid_fps < 10)																	// 下云台小电脑通信帧率异常检测
	{
		stError.DNComputerState = TRUE;
		DNVisionDataReceiveBuff.stVisionData.Recieve_ID = 0;													// 帧率异常时认为未识别到目标
	}
	else	{	stError.DNComputerState = FALSE;}
}
