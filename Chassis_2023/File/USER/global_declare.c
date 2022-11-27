#include "global_declare.h"

/** @brief 通讯所用变量 */
	/** @brief can通讯 */
		CanRxMsg CAN1_RX_Message;
		CanRxMsg CAN2_RX_Message;
	/** @brief 大疆遥控器*/
		ST_DBUS  g_StDbus;	
	/** @brief 串口通讯变量*/
		__IO u8 USART2_Cushioning_Rx[1]={0};
		__IO u8 USART3_Cushioning_Rx[ChassisBufLen_Rx]={0};
		__IO u8 UART4_Cushioning_Rx[1]={0};
		__IO u8 UART5_Cushioning_Rx[1]={0};
		__IO u8 USART6_Cushioning_Rx[1]={0};
		ST_IMU GimbalReceiveData;		
		ST_IMU GimbalSendData;


/** @brief 电机所用变量 */
	/** @brief	编码器数据结构体 */
		ST_ENCODER GimbalYawEncoder		= QuickEncoder(1,8192);
		ST_ENCODER ShootEncoder 		= QuickEncoder(36,8192);
		ST_ENCODER ChassisEncoder 		= QuickEncoder(14,8192);
		ST_ENCODER MotorEncoder				= QuickEncoder(1,8192);
		ST_ENCODER ChassisZigEncoder	= QuickEncoder(19,8192);	
	/** @brief 电机控制pid */
		ST_PID GimbalYawPosPid;
		ST_PID GimbalYawSpeedPid;	
		ST_PID GimbalPitchPosPid;
		ST_PID GimbalPitchSpeedPid;
		ST_TD	 YawTD;
	/** @brief 控制电机的角度与速度 */
		float YawPosDes=0;
		float YawEncoderAngle=0;
		float YawEncoderSpeed=0;
		float YawBMIAngle=0;
		float YawBMISpeed=0;
		float PitchBMIAngle=0;
		float PitchBMISpeed=0;


/** @brief 系统侦测 */
	/**	@brief	系统监视器 */
		SYSTEM_MONITOR 		systemMonitor 		= {0};
	  SUB_SYSTEM_MONITOR	SubSystemMonitor 	= {0};
		RS_SYSTEM_MONITOR	RSSystemMonitor		= {0};
		
		ST_ERROR stError = {FALSE};	// 模块异常标志位	
		ST_FLAG 	stFlag = {FALSE};	// 全局标志位		
		
		