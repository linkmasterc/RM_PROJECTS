#include "system_monitor_task.h"

void System_Monitor(void)
{
  /*计算系统各个任务及通信帧率*/
	system_monitor.LedTask_fps= system_monitor.LedTask_cnt;
	system_monitor.LedTask_cnt = 0;
	
	system_monitor.ImuUpdateTask_fps=system_monitor.ImuUpdateTask_cnt;
	system_monitor.ImuUpdateTask_cnt=0;
	
	system_monitor.SensorDataTask_fps=system_monitor.SensorDataTask_cnt;
	system_monitor.SensorDataTask_cnt=0;
	
	system_monitor.CommunicateTask_fps=system_monitor.CommunicateTask_cnt;
	system_monitor.CommunicateTask_cnt=0;
	
	system_monitor.PlatformTask_fps=system_monitor.PlatformTask_cnt;
	system_monitor.PlatformTask_cnt=0;
	
	system_monitor.ShootTask_fps=system_monitor.ShootTask_cnt;
	system_monitor.ShootTask_cnt=0;
	
}
