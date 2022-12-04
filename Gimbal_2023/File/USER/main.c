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

void SensorDataPrepareTask(void)
{
	Sensor_Data_Prepare();
	systemMonitor.IMUSampleTask_cnt++;
}

void IMUUpdateTask(void)
{
	IMU_Update_Task();
	systemMonitor.IMUUpdateTask_cnt++;
}

void GimbalControlTask(void)
{
	GimbalControl();
	systemMonitor.GimbalTask_cnt++;
}

void SendDataTask(void)
{
	ChassisSendDataProtocol();
	systemMonitor.DataSendTask_cnt++;
}


void LedTask(void)
{
	/*����ͨ������������*/
	if(systemMonitor.USART3_rx_fps > 500)
	{
				LED_BLUE_ON();

	}
	else
	{
				LED_BLUE_OFF();

	}
	
	/*CANͨ���������̵�*/
	if(systemMonitor.CAN1_rx_fps  > 1500 && systemMonitor.CAN2_rx_fps > 1500)
	{
				LED_GREEN_ON();				

	}
	else
	{
				LED_GREEN_OFF();

	}
	
	systemMonitor.LEDTask_cnt++;
}

void TriggerTask(void)
{
	delay_us(5);
	Trigger_HIGH();
	TriggerState = 1;
	delay_us(5);
	systemMonitor.TriggerTask_cnt++;
}

