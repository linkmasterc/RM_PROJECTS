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

#define GM_YAW_SENSIBILITY		3800							// ң����yaw������
#define GM_PITCH_SENSIBILITY	12								// ң����pitch������
#define GM_YAW_SENSIBILITY_TEST		100							// ң����yaw������(�������ã�
#define GM_PITCH_SENSIBILITY_TEST	100							// ң����pitch�����ȣ��������ã�
#define __packed

/** @brief ͨѶ���ñ��� */
	/** @brief canͨѶ */
		extern CanRxMsg CAN1_RX_Message;
		extern CanRxMsg CAN2_RX_Message;
	
	
	/** @brief ��ң����*/
		extern ST_DBUS 	g_StDbus;	
	
	
	/** @brief ����ͨѶ����*/
		extern __IO u8 USART2_Cushioning_Rx[1];		
		extern __IO u8 USART3_Cushioning_Rx[ChassisBufLen_Rx];
		extern __IO u8 UART4_Cushioning_Rx[1];
		extern __IO u8 UART5_Cushioning_Rx[1];
		extern __IO u8 USART6_Cushioning_Rx[1];
		extern ST_IMU GimbalReceiveData;
		extern ST_IMU GimbalSendData;

/** @brief ������ñ��� */
	/** @brief	���������ݽṹ�� */
		extern ST_ENCODER GimbalYawEncoder;
		extern ST_ENCODER ShootEncoder;
		extern ST_ENCODER ChassisEncoder;
		extern ST_ENCODER MotorEncoder;
		extern ST_ENCODER ChassisZigEncoder;
	
	/** @brief �������pid */
		extern ST_PID GimbalYawPosPid;
		extern ST_PID GimbalYawSpeedPid; 
		extern ST_PID GimbalPitchPosPid;
		extern ST_PID GimbalPitchSpeedPid;
		extern ST_TD	YawTD;
	/** @brief �������ת��λ���õ�λ */
	  extern float YawPosDes;
		extern float YawEncoderAngle;
		extern float YawEncoderSpeed;
		extern float YawBMIAngle;
		extern float YawBMISpeed;
		extern float PitchBMIAngle;
		extern float PitchBMISpeed;
/** @brief ϵͳ��� */
	/**	@brief	ϵͳ������ */
		extern SYSTEM_MONITOR systemMonitor;
		extern SUB_SYSTEM_MONITOR	SubSystemMonitor;
		extern RS_SYSTEM_MONITOR	RSSystemMonitor;
		extern ST_ERROR stError;	// ģ���쳣��־λ#endif
		extern ST_FLAG 	stFlag;
 
#endif






