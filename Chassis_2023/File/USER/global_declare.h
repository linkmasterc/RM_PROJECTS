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
	/** @brief 裁判系统 */
		extern u8 		RSReceiveDataBuff[USART2_RX_BUF_LEN];					// 裁判系统发来的数据
		extern u8 		BalancesoldierNumber;
		extern bool 	BanlanReconizeFinshFlag;				// 平衡步兵识别完标志位
		
		extern s16     Barrel_Heat_Cur_UP;											// 上云台当前枪管热量
		extern s16     Barrel_Heat_Cur_DN;											// 下云台当前枪管热量
		extern bool	Invincible_Status;												// 无敌状态
		extern u16     Bullet_Num_Remain;											// 剩余子弹数目
		extern u16     SentryHPMessage;												// 哨兵当前血量
		extern float   ChassisPowerMessage;										// 当前底盘功率
		extern u16     ChassisPowerBufferMessageLast;					// 底盘上次剩余缓冲能量
		extern u16     ChassisPowerBufferMessage;							// 底盘剩余缓冲能量
		extern u8      RobotIDMessage;													// 本机器人ID
		extern u8 		Bullet1FreqMessage;												// 裁判系统返回的上云台弹频
		extern u8		Bullet2FreqMessage;												// 裁判系统返回的下云台弹频
		extern float   Bullet1SpeedMessage;										// 裁判系统返回的上云台射速
		extern float   Bullet2SpeedMessage;										// 裁判系统返回的下云台射速
		extern u16		StageRemainTimeMessage;										// 当前比赛阶段剩余时间


		extern u16     OutpostHPMessage;
		extern u16		EnemySentryHPMessage;
		extern u8      DartFlagMessage;

		extern u16		ChassisPowerLimitMessage;
		extern u16		Shooter1HeatLimitMessage;
		extern u16		Shooter2HeatLimitMessage;
		extern u16		Shooter1HeatCoolingMessage;
		extern u16		Shooter2HeatCoolingMessage;
		extern u8      ShooterIDMessage;
		extern u16 	Bullet1Cnt;
		extern u16 	Bullet2Cnt;
	
	/** @brief 串口通讯变量*/
		extern __IO u8 USART2_Cushioning_Rx[1];		
		extern __IO u8 USART3_Cushioning_Rx[ChassisBufLen_Rx];
		extern __IO u8 UART4_Cushioning_Rx[1];
		extern __IO u8 UART5_Cushioning_Rx[1];
		extern ST_IMU GimbalReceiveData;
		extern ST_IMU GimbalSendData;
	/**	@brief	通讯数据存储结构体 */
		extern u8 VisionDataBuffTemp[VISION_RECIEVE_DATA_LEN_DN - 1];
		extern VISION_SEND_DATA_DN VisionDataSendBuff;
		extern __IO u8 UA6RxDMAbuf[UA6RxDMAbuf_LEN];
		extern u8 UA6RxMailbox[UA6RxMBbuf_LEN];
		extern USART_RX_TypeDef USART6_Rcr;
		extern UN_VISION_DATA_DN VisionDataReceiveBuff;						// 下云台视觉发来的数据
		extern float 		VisionData1 ;	//记录前帧数据
		extern float 		VisionData2 ;	//记录前帧数据
		extern float		VisionData3 ;
		extern float		VisionData4 ;
		extern float		VisionData5 ;
		extern float		VisionData6 ;
		extern float		VisionData7 ;
		extern float		VisionData8 ;
		extern float		VisionData9 ;
		extern float		VisionData10 ;
		extern float		VisionData11 ;
		
		
		extern float		VisionSendData1 ;
		extern float		VisionSendData2 ;
		extern float		VisionSendData3 ;
		extern float		VisionSendData4 ;
		extern float		VisionSendData5 ;
		extern float		VisionSendData6 ;
		extern float		VisionSendData7 ;
		extern float		VisionSendData8 ;
		extern float		VisionSendData9 ;
		extern float		VisionSendData10 ;
		extern float		VisionSendData11 ;


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
	/** @brief 射击相关电机 */
		extern u32 	Friction_State_UP;								// 摩擦轮状态(500停止700开启）
		extern u32 	Friction_State ;
		extern s16 	Bullet_Des; 																// 拨弹目标数目				
		extern s16 	Bullet_Des_Pre; 									// 前一次拨弹目标数目
		extern bool 	Follow_Flag_UP;						// 紧密跟随标志位
		extern bool 	Follow_Flag_DN;
		extern bool 	Shoot_Area_Flag;						// 射击区域检测标志位
		extern u8		Shoot_Frequency_UP;									// 上云台射速
		extern u8		Shoot_Frequency_DN;// 下云台射速
		extern int 	upShooterStaticTimes;								// 卡弹计数
		extern bool	FrictFirstIn;								// 用于摩擦轮延时启动
		extern bool BoDanFirstIn;
		extern ST_ShootTestControl DNSTC;									// 下云台射击
/** @brief 系统侦测 */
	/**	@brief	系统监视器 */
		extern SYSTEM_MONITOR systemMonitor;
		extern SUB_SYSTEM_MONITOR	SubSystemMonitor;
		extern RS_SYSTEM_MONITOR	RSSystemMonitor;
		extern ST_ERROR stError;	// 模块异常标志位#endif
		extern ST_FLAG 	stFlag;
 	/** @brief 模式选择 */
		extern u8 ControlMode;
#endif






