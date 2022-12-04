#ifndef __MAIN_H__
#define __MAIN_H__

#include "global_declare.h"
/*--------------Types--------------*/

/*--------------BSP--------------*/
#include "bsp_init.h"
#include "delay.h"
#include "tim.h"
#include "can.h"
#include "usart.h"
#include "nvic.h"
#include "stm32f4xx_it.h"

/*--------------API--------------*/

/*--------------TASK--------------*/
#include "systemmonitor_task.h"
#include "gimbal_task.h"
#include "led_task.h"
#include "mode_choose_task.h"
#include "shoot_task.h"
#include "data_send_task.h"

/*------------OS_init-----------*/
#include "os.h"

void SystemMonitorTask(void);
void GimbalTask(void);
void MotorDataSendTask(void);
void SendDataTask(void);
void ModeChooseTask(void);
void ShootTask(void);
void LEDStateChangeTask(void);
void VisionDataSendTask(void);
void VofaPlusTask(void);
#endif
