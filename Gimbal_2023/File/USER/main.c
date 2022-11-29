#include "stm32f4xx.h"
#include "main.h"

/*--------------------------------------
�����ܣ�������
--------------------------------------*/
int main(void)
{
    BSP_Init(); //�����ʼ��

    OS_RUN();   //ִ�и�������
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

