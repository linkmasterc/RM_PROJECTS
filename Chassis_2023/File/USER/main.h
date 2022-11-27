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

/*------------OS_init-----------*/
#include "os.h"

void SystemMonitorTask(void);
void GimbalTask(void);
void MotorDataSendTask(void);
void SendDataTask(void);
#endif
