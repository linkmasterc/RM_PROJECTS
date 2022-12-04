#ifndef GLOBAL_DECLARE_H
#define GLOBAL_DECLARE_H


#include "rm_rc_types.h"
#include "rm_rs_types.h"
#include "rm_pid_types.h"
#include "rm_scanner_types.h"
#include "rm_communicate_types.h"
#include "ParamConfig.h"
#include "gimbal_protocol.h"

#include "stm32f4xx.h"
#include <string.h>

#define ChassisBufLen_Rx	USART3_RX_BUF_LEN
#define ChassisBufLen_Tx	USART3_TX_BUF_LEN

#define GM_YAW_SENSIBILITY		3800							// ң����yaw������
#define GM_PITCH_SENSIBILITY	12								// ң����pitch������
#define GM_YAW_SENSIBILITY_TEST		0.1					// ң����yaw������(�������ã�
#define GM_PITCH_SENSIBILITY_TEST	25						// ң����pitch�����ȣ��������ã�

#define RecordBullet1Num	10
#define RecordBullet2Num	10
#define RecordEncoderLengthNum	10
#define __packed

/** @brief ͨѶ���ñ��� */
	/** @brief canͨѶ */
		extern CanRxMsg CAN1_RX_Message;
		extern CanRxMsg CAN2_RX_Message;
	
	
	/** @brief ��ң����*/
		extern ST_DBUS 	g_StDbus;	
	/** @brief ����ϵͳ */
	  extern u8 UA2TxDMAbuf[USART2_TX_DMA_BUF_LEN];
		extern __IO u8 UA2RxDMAbuf[USART2_RX_DMA_BUF_LEN];
    extern  u8 UA2RxMailbox[USART2_RX_MB_BUF_LEN];
		extern USART_RX_TypeDef USART2_Rcr;
	

		extern u8 		RSReceiveDataBuff[USART2_RX_MB_BUF_LEN];					// ����ϵͳ����������
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
		
		extern u16		SumHurtPerSecond;											// ÿ���˺�
		extern bool 	SumHurtFlag;													// ���˱�־λ
		extern u8		HitOurselfFlag;													// ���򼺷���־λ
		
		extern bool	RSPowerUpdataFlag;														//����ϵͳ���ʸ��±�־λ��ʵ��10Hz
		extern u8 	CalibrateState;																//�궨״̬��־λ
		
		extern float PreChassisPowerMessage;
		
		extern ext_game_status_t						GameStatus;						// ����״̬����
		extern ext_game_result_t						GameResult;						// �����������
		extern ext_game_robot_HP_t						GameRobotHP;					// ������Ѫ������
		extern ext_ICRA_buff_debuff_zone_status_t		ICRABuffDebuffZoneStatus;		// �˹�������ս���ӳ���ͷ���״̬
		extern ext_event_data_t						EventData;						// �����¼�����
		extern ext_supply_projectile_action_t			SupplyProjectileAction;			// ����վ������ʶ
		extern ext_referee_warning_t					RefereeWarning;					// ���о�����Ϣ
		extern ext_dart_remaining_time_t				DartRemainingTime;				// ���ڷ���ڵ���ʱ
		extern ext_game_robot_status_t					GameRobotStatus;				// ����������״̬
		extern ext_power_heat_data_t					PowerHeatData;					// ʵʱ������������
		extern ext_game_robot_pos_t					GameRobotPos;					// ������λ��
		extern ext_buff_t								Buff;							// ����������
		extern aerial_robot_energy_t					AerialRobotEnergy;				// ���л���������״̬
		extern ext_robot_hurt_t						RobotHurt;						// �˺�״̬
		extern ext_shoot_data_t						ShootData;						// ʵʱ�����Ϣ
		extern ext_bullet_remaining_t					BulletRemaining;				// �ӵ�ʣ�෢����Ŀ
		extern ext_rfid_status_t						RFIDStatus;						// ������RFID״̬
		extern ext_dart_client_cmd_t					DartClientCmd;					// ���ڻ����˿ͻ���ָ������
		extern ext_student_interactive_header_data_t	StudentInteractiveHeaderData;	// �����˼佻����Ϣ
		extern robot_interactive_data_t				RobotInteractiveData;			// ��������extern 
	
	/** @brief ����ͨѶ����*/
		extern u8 USART1_Cushioning_Rx[USART1_RX_BUF_LEN];
		extern __IO u8 USART2_Cushioning_Rx[1];		
		extern __IO u8 USART3_Cushioning_Rx[ChassisBufLen_Rx];
		extern __IO u8 UART4_Cushioning_Rx[1];
		extern __IO u8 UART5_Cushioning_Rx[1];
		extern ST_IMU GimbalData;
	/**	@brief	�Ӿ�ͨѶ */
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


		extern u8 			TirggerState;

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
		extern bool Auto_BoDanFirstIn;
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
		
		
/** @brief ���ƹ�����Ҫ���� */
	extern u8			PreGimabalTestFlag;
	extern float 		AccelStep;
	extern float 		LineDecelerateStep;
	extern u16			UPShootSpeedTest;
	extern bool		StartUPShootFlag;
	extern bool		StartDNShootFlag;
	extern bool		LowShootFlag;
	extern u32 		ShootHeatTick;//bool 		UP_Range_Mode_First_in_Flag = TRUE;
	//extern //bool 		DN_Range_Mode_First_in_Flag = TRUE;
	extern bool		ChassisPositionLeftFlag;
	extern u8 			Chassis_Test_Flag;

	/** @brief���̹��� */
	

	/** @brief����ͨѶ */
		extern bool	Range360Flag;
		extern u8		ResetState;
		extern bool	FindEnemyR2	;
		extern bool	FindEnemyRoad;

	/** @briefƽ�����ټ��� */
		extern float	Bullet1SpeedMean;
		extern float	Bullet1SpeedRecord[RecordBullet1Num];
		extern float	Bullet2SpeedMean;
		extern float	Bullet2SpeedRecord[RecordBullet2Num];

	/** @brief�������� */
		extern u16		Shooter1HeatRemainingNum;
		extern u16		Shooter2HeatRemainingNum;
		extern u16		Shooter1ShootNum;
		extern u16		Shooter2ShootNum;
		extern u16		Shooter1StableBulletFreq;
		extern u16		Shooter2StableBulletFreq;
		extern float	Shooter1NumberLast;
		extern float	Shooter2NumberLast;
	
	/**@brief�з���λ��� */
		extern bool	  Robot1ShootFlag;
		extern bool 	Robot2ShootFlag;
		extern bool 	Robot3ShootFlag;
		extern bool 	Robot4ShootFlag;
		extern bool		Robot5ShootFlag;
#ifdef MINYOU_PROTOCOL
	extern ST_UART_DATA_MY StateDataSendBuff;
#elif defined VOFA_PROTOCOL
	extern ST_UART_DATA_VOFA StateDataSendBuff;
#endif
#endif






