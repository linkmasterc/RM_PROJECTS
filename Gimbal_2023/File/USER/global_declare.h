#ifndef GLOBAL_DECLARE_H
#define GLOBAL_DECLARE_H

#include "rm_rc_types.h"
#include "rm_rs_types.h"
#include "rm_pid_types.h"
#include "rm_scanner_types.h"
#include "rm_communicate_types.h"
#include "stm32f4xx.h"
#include <string.h>
#define __packed

#define ChassisBufLen_Rx	USART3_RX_BUF_LEN
#define ChassisBufLen_Tx	USART3_TX_BUF_LEN

#define ST_GIMBAL_FLAG ST_IQRFLAG




//摩擦轮
#define fric_speed1 -10400.0
#define fric_speed2 -10400.0
#define Fric 139

//拨弹电机
#define Bottom_SupplyStep 29491

//pitch轴电机
#define PITCH_ANGLE_FEEDBACK_GRYOSCOPE
//#define PITCH_ANGLE_FEEDBACK_ENCODER
#define PITCH_SPEED_FEEDBACK_GRYOSCOPE
//#define PITCH_SPEED_FEEDBACK_ENCODER
#define	PITCH_USE_TD
#define GBDN_PITCH_MAX	30.0f
#define GBDN_PITCH_MIN	-30.0f
#define GM_PITCH_MIDPOS 4106

/** @brief 通讯所用变量 */
	/** @brief can通讯 */
		extern CanRxMsg CAN1_RX_Message;
		extern CanRxMsg CAN2_RX_Message;
	
	
	/** @brief 大疆遥控器*/
		extern ST_DBUS 	g_StDbus;	
	
	
	/** @brief 串口通讯变量*/
		extern __IO u8 USART2_Cushioning_Rx[1];		
		extern __IO u8 USART3_Cushioning_Rx[ChassisBufLen_Rx];
		extern __IO u8 UART4_Cushioning_Rx[1];
		extern __IO u8 UART5_Cushioning_Rx[1];
		extern __IO u8 USART6_Cushioning_Rx[1];
		extern ST_IMU ChassisData;


/** @brief 电机所用变量 */
	/** @brief	编码器数据结构体 */
		extern ST_ENCODER GimbalYawEncoder;
		extern ST_ENCODER ShootEncoder;
		extern ST_ENCODER ChassisEncoder;
		extern ST_ENCODER MotorEncoder;
		extern ST_ENCODER ChassisZigEncoder;
		extern ST_ENCODER GimbalSecondEncoder;
	
	/** @brief 电机控制pid */
		extern ST_PID GimbalYawPosPid;
		extern ST_PID GimbalYawSpeedPid; 
		extern ST_PID GimbalPitchPosPid;
		extern ST_PID GimbalPitchSpeedPid;
		extern ST_PID GimbalSecondPosPid;
		extern ST_PID GimbalSecondSpeedPid;
		extern ST_TD	YawTD;
	/** @brief 电调返回转化位常用单位 */
	  extern float YawPosDes;
		extern float YawEncoderAngle;
		extern float YawEncoderSpeed;
		extern float YawBMIAngle;
		extern float YawBMISpeed;
		
		extern float PitchPosDes;
		extern float PitchBMIAngle;
		extern float PitchBMISpeed;
	/** @brief 摩擦轮 */
		extern float Friction1_Temp;
		extern float Friction2_Temp;
		extern float Real_Friction1_Temp;
		extern float Real_Friction2_Temp;
		extern ST_ENCODER g_stFriction1Encoder;
		extern ST_ENCODER g_stFriction2Encoder;
		extern ST_SMC g_stFriction1SMC;
		extern ST_SMC g_stFriction2SMC;
	/** @brief 拨弹电机 */
		extern bool Shooter_BLOCK;
		extern float ShooterN;
		extern ST_ENCODER g_stShooterEncoder;
		extern ST_PID g_stShooterPosPID;
		extern ST_PID g_stShooterSpeedPID;
	/** @brief pitch轴电机 */
		extern float COMP;
		extern float PitchTest;
		extern s16 G_Compensate;

		extern float Pitch_Encoder_angle;
		extern float Pitch_Encoder_speed;
		extern ST_ANGLE BMIPitchAngle;
		extern float BMIPitchSpeed;
		extern ST_ANGLE BMIYawAngle;
		extern float BMIYawSpeed;

		extern float PitchDiff;
		extern float Gravity_Compensate;


		extern ST_ENCODER g_stPitchEncoder;
		extern float PitchSpeedCompensate;
		extern float	PitchCoe;
		extern ST_TD 	g_stPitchTD;
		extern ST_PID g_stPitchPosPID;
		extern ST_PID g_stPitchSpeedPID;
/** @brief 侦测 */
	/**	@brief	系统监视器 */
		extern SYSTEM_MONITOR systemMonitor;
		extern SUB_SYSTEM_MONITOR	SubSystemMonitor;
		extern RS_SYSTEM_MONITOR	RSSystemMonitor;
		extern ST_ERROR stError;	// 模块异常标志位#endif
		extern ST_FLAG 	stFlag;
		extern ST_GIMBAL_FLAG stGimbalFlag; //云台控制标志位
		
		
		extern s16	Barrel1_Heat_Cur;
		extern s16	Barrel2_Heat_Cur;
		extern float Bullet1SpeedMessage;
		extern float Bullet2SpeedMessage;	
		
	/**滤波器**/
		extern ST_KMF GimbalYawKMF;
#endif








