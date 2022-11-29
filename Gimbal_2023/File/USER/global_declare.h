#ifndef _GLOBAL_DECLARE_H_
#define _GLOBAL_DECLARE_H_

#include "stm32f4xx.h"
#include "rm_robot_types.h"
#include <string.h>
#define __packed






extern SYSTEM_MONITOR system_monitor;           //系统监视器



extern u8 res_data[ReceiveBufSize];								//所需数据填充buf

extern u8 send_data[SendBufSize];							//发送的数据

extern const u8 NUM_BUF;													//数据包的数量

extern USART2_DMA SendBuf;												//DMA发送结构体
	
extern USART2_DMA ReceiveBuf;											//DMA接收结构体

extern SEND_DMA_UNION Send_Union;									//DMA发送联合体

extern u8 cushioning[13];					//DMA接收缓冲区

extern motor_parameter motor_wheel[13];

extern ST_ENCODER encoder[13];

extern float YAW_POS_DES;

extern float PITCH_POS_DES;

extern float gravity_fb;

extern float COE;

extern ST_TD YAW_POS_TD;

extern ST_TD PITCH_POS_TD;

extern ST_PID YAW_POS_PID;

extern ST_PID YAW_SPEED_PID;

extern ST_PID PITCH_POS_PID;

extern ST_PID PITCH_SPEED_PID;

extern ST_PID	TRIGGER_POS_PID;

extern ST_PID	TRIGGER_SPEED_PID;

extern ST_PID	SHOOTER_SPEED_PID_L;

extern ST_PID	SHOOTER_SPEED_PID_R;

extern ST_LPF PITCH_LPF;

extern ST_LPF YAW_LPF;

extern ST_LPF	YAW_LPF;

extern ST_ANGLE YAW_ANGLE;

extern u8 DOWN_PLATFOM_STATUS;

extern u8	DOWN_SHOOT_STATUS;

extern u16 bullet_num;

extern const u8 CRC8_INIT;												//crc8初始值
	
extern const u8 CRC8_TAB[256];										//crc8查询表

extern u16 CRC16_INIT;

extern const u16 CRC16_Table[256];

typedef enum {FALSE = 0, TRUE = !FALSE} bool;

extern IMU_MODE imu_mode;

extern ST_LPF gyro_x;
extern ST_LPF gyro_y;
extern ST_LPF gyro_z;

extern ST_LPF acc_x;
extern ST_LPF acc_y;
extern ST_LPF acc_z;

extern u16 Cali_Cnt;
extern s16 Real_Temp;
extern float Acc_X_Ori;
extern float Acc_Y_Ori;
extern float Acc_Z_Ori;
extern float Gyro_X_Ori;
extern float Gyro_Y_Ori;
extern float Gyro_Z_Ori;

extern float Acc_X_Real;											//单位为mg
extern float Acc_Y_Real;
extern float Acc_Z_Real;

extern float Gyro_X_Real;											//单位为弧度每秒
extern float Gyro_Y_Real;
extern float Gyro_Z_Real;

extern float Gyro_X_Speed;
extern float Gyro_Y_Speed;
extern float Gyro_Z_Speed;

/*以下为零飘标定值*/
extern float Acc_X_Offset;
extern float Acc_Y_Offset;
extern float Acc_Z_Offset;
extern float Gyro_X_Offset;
extern float Gyro_Y_Offset;
extern float Gyro_Z_Offset;

extern float Status_offset[3][CALI_NUM];  //1 X;2 Y;3 Z.
extern float Num_offset[CALI_NUM];
extern int  Const_offset;
extern int  flag_offset;
extern float testvalue[3];

extern float SEE;

extern float Test_Flash_Ori;
extern float Test_Flash_Write[2];
extern float  Test_Flash_Get[2];

extern float BMIPitchSpeed;
extern float BMIYawSpeed;
extern float BMIYawAngle;
extern float BMIPitchAngle;
extern float BMIRollSpeed;
extern float BMIRollAngle;
extern float DOWN_SHOOT_FREQ;

extern _imu_st imu_data;
extern float Kp;/**/
extern float Ki;/**/
/**/
extern float exInt;
extern float eyInt;
extern float ezInt;
/**/
static float q0 = 1.0f;
static float q1 = 0.0f;
static float q2 = 0.0f;
static float q3 = 0.0f;



#endif








