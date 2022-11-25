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

/*------------OS_init-----------*/
#include "os.h"

void SystemMonitorTask(void);
void CommunicateTask(void);
void ChassisTask(void);
void PlatformTask(void);
#endif
