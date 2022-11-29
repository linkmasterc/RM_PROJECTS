#ifndef __IMU_UPDATE_TASK
#define __IMU_UPDATE_TASK

#include "global_declare.h"
#include "bmi088_driver.h"
#include "math.h"

void IMU_Update_Task(void);
void IMU_Update_Mahony(_imu_st *imu,float dt);
extern	_imu_st imu_data;
#endif

