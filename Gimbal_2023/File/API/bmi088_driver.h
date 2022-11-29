#ifndef __BMI088_H__
#define __BMI088_H__

#include "spi.h"
#include "delay.h"
#include "math.h"

bool bmi088_init(void);
u8 BMI088_Read_Gyro_Data(u8 reg_id);
void BMI088_Write_Gyro_Data(u8 reg_id,u8 data);
u8 BMI088_Read_Acc_Data(u8 reg_id);
void BMI088_Write_Acc_Data(u8 reg_id,u8 data);
void Sensor_Data_Prepare(void);
void LpFilter(ST_LPF* lpf);
float invSqrt(float x);

#endif





