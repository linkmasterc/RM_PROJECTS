#ifndef __MAIN_H__
#define __MAIN_H__

#include "global_declare.h"
/*--------------Types--------------*/
#include "rm_robot_types.h"
/*--------------BSP--------------*/
#include "bsp_init.h"
#include "delay.h"
#include "spi.h"
#include "usart.h"
#include "can.h"
#include "nvic.h"
#include "spi.h"
#include "led.h"
#include "systick.h"
//#include "stm32f4xx_it.h"

/*--------------API--------------*/
#include "bmi088_driver.h"
#include "uart_protocol.h"
#include "mahony_algorithm.h"
#include "motor.h"
#include "pid_algorithm.h"
/*--------------TASK--------------*/
#include "test_task.h"
#include "led_task.h"
#include "imu_update_task.h"
#include "sensor_data_task.h"
#include "communicate_task.h"
#include "motor_task.h"
#include "system_monitor_task.h"
#include "shoot_task.h"
/*------------OS_init-----------*/
#include "os.h"

void LedTask(void);
void TestTask(void);
void SystemMonitorTask(void);
void ImuUpdateTask(void);
void SensorDataTask(void);
void CommunicateTask(void);
void MotorTask(void);
void ShootTask(void);
#endif


