#ifndef GLOBAL_DECLARE_H
#define GLOBAL_DECLARE_H


#include "rm_rc_types.h"
#include "rm_rs_types.h"
#include "rm_pid_types.h"
#include "rm_scanner_types.h"
#include "rm_communicate_types.h"

#include "gimbal_protocol.h"

#include "stm32f4xx.h"
#include <string.h>

#define ChassisBufLen_Rx	USART3_RX_BUF_LEN
#define ChassisBufLen_Tx	USART3_TX_BUF_LEN

#define GM_YAW_SENSIBILITY		3800							// 遥控器yaw灵敏度
#define GM_PITCH_SENSIBILITY	12								// 遥控器pitch灵敏度
#define GM_YAW_SENSIBILITY_TEST		100							// 遥控器yaw灵敏度(调弹道用）
#define GM_PITCH_SENSIBILITY_TEST	100							// 遥控器pitch灵敏度（调弹道用）
#define __packed

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
		extern ST_IMU GimbalReceiveData;
		extern ST_IMU GimbalSendData;

/** @brief 电机所用变量 */
	/** @brief	编码器数据结构体 */
		extern ST_ENCODER GimbalYawEncoder;
		extern ST_ENCODER ShootEncoder;
		extern ST_ENCODER ChassisEncoder;
		extern ST_ENCODER MotorEncoder;
		extern ST_ENCODER ChassisZigEncoder;
	
	/** @brief 电机控制pid */
		extern ST_PID GimbalYawPosPid;
		extern ST_PID GimbalYawSpeedPid; 
		extern ST_PID GimbalPitchPosPid;
		extern ST_PID GimbalPitchSpeedPid;
		extern ST_TD	YawTD;
	/** @brief 电调返回转化位常用单位 */
	  extern float YawPosDes;
		extern float YawEncoderAngle;
		extern float YawEncoderSpeed;
		extern float YawBMIAngle;
		extern float YawBMISpeed;
		extern float PitchBMIAngle;
		extern float PitchBMISpeed;
/** @brief 系统侦测 */
	/**	@brief	系统监视器 */
		extern SYSTEM_MONITOR systemMonitor;
		extern SUB_SYSTEM_MONITOR	SubSystemMonitor;
		extern RS_SYSTEM_MONITOR	RSSystemMonitor;
		extern ST_ERROR stError;	// 模块异常标志位#endif
		extern ST_FLAG 	stFlag;
 
#endif






