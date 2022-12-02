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
	/** @brief ����ϵͳ */
		extern u8 		RSReceiveDataBuff[USART2_RX_BUF_LEN];					// ����ϵͳ����������
		extern u8 		BalancesoldierNumber;
		extern bool 	BanlanReconizeFinshFlag;				// ƽ�ⲽ��ʶ�����־λ
		
		extern s16     Barrel_Heat_Cur_UP;											// ����̨��ǰǹ������
		extern s16     Barrel_Heat_Cur_DN;											// ����̨��ǰǹ������
		extern bool	Invincible_Status;												// �޵�״̬
		extern u16     Bullet_Num_Remain;											// ʣ���ӵ���Ŀ
		extern u16     SentryHPMessage;												// �ڱ���ǰѪ��
		extern float   ChassisPowerMessage;										// ��ǰ���̹���
		extern u16     ChassisPowerBufferMessageLast;					// �����ϴ�ʣ�໺������
		extern u16     ChassisPowerBufferMessage;							// ����ʣ�໺������
		extern u8      RobotIDMessage;													// ��������ID
		extern u8 		Bullet1FreqMessage;												// ����ϵͳ���ص�����̨��Ƶ
		extern u8		Bullet2FreqMessage;												// ����ϵͳ���ص�����̨��Ƶ
		extern float   Bullet1SpeedMessage;										// ����ϵͳ���ص�����̨����
		extern float   Bullet2SpeedMessage;										// ����ϵͳ���ص�����̨����
		extern u16		StageRemainTimeMessage;										// ��ǰ�����׶�ʣ��ʱ��


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
	
	/** @brief ����ͨѶ����*/
		extern __IO u8 USART2_Cushioning_Rx[1];		
		extern __IO u8 USART3_Cushioning_Rx[ChassisBufLen_Rx];
		extern __IO u8 UART4_Cushioning_Rx[1];
		extern __IO u8 UART5_Cushioning_Rx[1];
		extern ST_IMU GimbalReceiveData;
		extern ST_IMU GimbalSendData;
	/**	@brief	ͨѶ���ݴ洢�ṹ�� */
		extern u8 VisionDataBuffTemp[VISION_RECIEVE_DATA_LEN_DN - 1];
		extern VISION_SEND_DATA_DN VisionDataSendBuff;
		extern __IO u8 UA6RxDMAbuf[UA6RxDMAbuf_LEN];
		extern u8 UA6RxMailbox[UA6RxMBbuf_LEN];
		extern USART_RX_TypeDef USART6_Rcr;
		extern UN_VISION_DATA_DN VisionDataReceiveBuff;						// ����̨�Ӿ�����������
		extern float 		VisionData1 ;	//��¼ǰ֡����
		extern float 		VisionData2 ;	//��¼ǰ֡����
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
	/** @brief �����ص�� */
		extern u32 	Friction_State_UP;								// Ħ����״̬(500ֹͣ700������
		extern u32 	Friction_State ;
		extern s16 	Bullet_Des; 																// ����Ŀ����Ŀ				
		extern s16 	Bullet_Des_Pre; 									// ǰһ�β���Ŀ����Ŀ
		extern bool 	Follow_Flag_UP;						// ���ܸ����־λ
		extern bool 	Follow_Flag_DN;
		extern bool 	Shoot_Area_Flag;						// ����������־λ
		extern u8		Shoot_Frequency_UP;									// ����̨����
		extern u8		Shoot_Frequency_DN;// ����̨����
		extern int 	upShooterStaticTimes;								// ��������
		extern bool	FrictFirstIn;								// ����Ħ������ʱ����
		extern bool BoDanFirstIn;
		extern ST_ShootTestControl DNSTC;									// ����̨���
/** @brief ϵͳ��� */
	/**	@brief	ϵͳ������ */
		extern SYSTEM_MONITOR systemMonitor;
		extern SUB_SYSTEM_MONITOR	SubSystemMonitor;
		extern RS_SYSTEM_MONITOR	RSSystemMonitor;
		extern ST_ERROR stError;	// ģ���쳣��־λ#endif
		extern ST_FLAG 	stFlag;
 	/** @brief ģʽѡ�� */
		extern u8 ControlMode;
#endif






