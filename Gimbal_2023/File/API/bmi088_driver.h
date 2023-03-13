#ifndef __BMI088_H__
#define __BMI088_H__

#include "spi.h"
#include "delay.h"
#include "math.h"
#include "general_math.h"

bool bmi088_init(void);
u8 BMI088_Read_Gyro_Data(u8 reg_id);
void BMI088_Write_Gyro_Data(u8 reg_id,u8 data);
u8 BMI088_Read_Acc_Data(u8 reg_id);
void BMI088_Write_Acc_Data(u8 reg_id,u8 data);
void Sensor_Data_Prepare(void);
void LpFilter(ST_LPF* lpf);
float invSqrt(float x);

extern IMU_MODE imu_mode;
extern SSHORT16 Real_Temp;
//u8 STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite);		//从指定地址开始写入指定长度的数据
//void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead);   		//从指定地址开始读出指定长度的数据
extern FP32 Gyro_X_Ori;
extern FP32 Gyro_Y_Ori;
extern FP32 Gyro_Z_Ori;

extern FP32 Gyro_X_Speed;
extern FP32 Gyro_Y_Speed;
extern FP32 Gyro_Z_Speed;

extern FP32 Acc_X_Real;
extern FP32 Acc_Y_Real;
extern FP32 Acc_Z_Real;

extern ST_KMF stZSpeedKMF;
//Accelerater_Reg

#define	ACC_ID				0X00
#define	ACC_ERR				0X02
#define ACC_STATUS		0X03
#define	ACC_X_LSB			0X12
#define	ACC_X_MSB			0X13
#define	ACC_Y_LSB 		0X14
#define	ACC_Y_MSB 		0X15
#define	ACC_Z_LSB 		0X16
#define	ACC_Z_MSB 		0X17
#define ACC_RANGE			0X41
#define	TEMP_MSB			0X22
#define	TEMP_LSB 			0X23
#define ACC_CONF			0X40
#define	ACC_PWR_CTRL	0X7D
#define ACC_PWR_CONF	0X7C
#define ACC_SOFT_R		0X7E


//Gyro_Reg

#define	GYRO_ID				0X00
#define	GYRO_X_LSB		0X02
#define	GYRO_X_MSB		0X03
#define	GYRO_Y_LSB		0X04
#define	GYRO_Y_MSB		0X05
#define	GYRO_Z_LSB		0X06
#define	GYRO_Z_MSB		0X07
#define	GYRO_RANGE		0X0F
#define	GYRO_BANDW		0X10
#define GYRO_POWER		0X11
#define	GYRO_SOFT_R		0X14

#define CALI_NUM			5000

#define FAIL          0
#define SUCCESS       1

#define NOP			__asm{NOP;}

#define OFF_FREQ			1500
#endif





