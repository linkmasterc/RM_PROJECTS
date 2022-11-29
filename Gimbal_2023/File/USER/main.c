#include "stm32f4xx.h"
#include "main.h"

/*--------------------------------------
任务功能：主函数
--------------------------------------*/
int main(void)
{
    BSP_Init(); //外设初始化

    OS_RUN();   //执行各项任务
}



void SystemMonitorTask(void)
{	 
		System_Monitor();		
}

void ImuUpdateTask()
{
	IMU_Update_Task();
	system_monitor.ImuUpdateTask_cnt++;
}

void SensorDataTask()
{
	SENSOR_DATA();
	system_monitor.SensorDataTask_cnt++;
}

void CommunicateTask()
{
	COMMUNICATE();
	system_monitor.CommunicateTask_cnt++;
}

void MotorTask()
{
	MOTOR_TASK();
	system_monitor.PlatformTask_cnt++;
}

void ShootTask()
{
	SHOOT_TASK();
	system_monitor.ShootTask_cnt++;
}

