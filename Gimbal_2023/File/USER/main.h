#ifndef __MAIN_H__
#define __MAIN_H__

#include "global_declare.h"
/*--------------Types--------------*/
/*--------------BSP--------------*/
#include "bsp_init.h"

//#include "stm32f4xx_it.h"

/*--------------API--------------*/
#include "chassis_protocol.h"
/*--------------TASK--------------*/
#include "system_monitor_task.h"
#include "gimbal_task.h"
#include "bmi088_driver.h"
#include "imu_update_task.h"

/*------------OS_init-----------*/
#include "os.h"

void SystemMonitorTask(void);
void IMUUpdateTask(void);
void SensorDataPrepareTask(void);
void GimbalControlTask(void);
void SendDataTask(void);
void TriggerTask(void);
void LedTask(void);
#endif


