#include "global_declare.h"

/** @brief ͨѶ���ñ��� */
	/** @brief canͨѶ */
		CanRxMsg CAN1_RX_Message;
		CanRxMsg CAN2_RX_Message;
	/** @brief ����ϵͳ */
		u8 		RSReceiveDataBuff[USART2_RX_BUF_LEN] 	= {0};					// ����ϵͳ����������
		u8 		BalancesoldierNumber  				= 0;
		bool 	BanlanReconizeFinshFlag				= FALSE;				// ƽ�ⲽ��ʶ�����־λ
		
		
		s16     Barrel_Heat_Cur_UP;											// ����̨��ǰǹ������
		s16     Barrel_Heat_Cur_DN;											// ����̨��ǰǹ������
		bool	Invincible_Status					= FALSE;					// �޵�״̬
		u16     Bullet_Num_Remain;											// ʣ���ӵ���Ŀ
		u16     SentryHPMessage						= 600;					// �ڱ���ǰѪ��
		float   ChassisPowerMessage					= 0;					// ��ǰ���̹���
		u16     ChassisPowerBufferMessageLast		= 0;					// �����ϴ�ʣ�໺������
		u16     ChassisPowerBufferMessage			= 0;					// ����ʣ�໺������
		u8      RobotIDMessage						= 7;												// ��������ID
		u8 		Bullet1FreqMessage;											// ����ϵͳ���ص�����̨��Ƶ
		u8		Bullet2FreqMessage;											// ����ϵͳ���ص�����̨��Ƶ
		float   Bullet1SpeedMessage;										// ����ϵͳ���ص�����̨����
		float   Bullet2SpeedMessage;										// ����ϵͳ���ص�����̨����
		u16		StageRemainTimeMessage;										// ��ǰ�����׶�ʣ��ʱ��


		u16     OutpostHPMessage;
		u16		EnemySentryHPMessage;
		u8      DartFlagMessage;

		u16		ChassisPowerLimitMessage			= 30;
		u16		Shooter1HeatLimitMessage			= 320;
		u16		Shooter2HeatLimitMessage			= 320;
		u16		Shooter1HeatCoolingMessage			= 100;
		u16		Shooter2HeatCoolingMessage			= 100;
		u8      ShooterIDMessage;
		u16 	Bullet1Cnt 							= 0;
		u16 	Bullet2Cnt 							= 0;
	/** @brief ��ң����*/
		ST_DBUS  g_StDbus;	
	/** @brief ����ͨѶ����*/
		__IO u8 USART2_Cushioning_Rx[1]={0};
		__IO u8 USART3_Cushioning_Rx[ChassisBufLen_Rx]={0};
		__IO u8 UART4_Cushioning_Rx[1]={0};
		__IO u8 UART5_Cushioning_Rx[1]={0};
		ST_IMU GimbalReceiveData;		
		ST_IMU GimbalSendData;
	/**	@brief	�Ӿ�ͨѶ */
		u8 VisionDataBuffTemp[VISION_RECIEVE_DATA_LEN_DN - 1] 			= {0};
		VISION_SEND_DATA_DN VisionDataSendBuff 	= {.My_Data.ID = 0x0A,.My_Data.Head1 = 0x55, .My_Data.Head2 = 0x00, .My_Data.Tail1 = 0x00, .My_Data.Tail2 = 0xAA};
		__IO u8 UA6RxDMAbuf[UA6RxDMAbuf_LEN] = {0};
				 u8 UA6RxMailbox[UA6RxMBbuf_LEN] = {0};
		USART_RX_TypeDef USART6_Rcr = {USART6,USART6_RX_STREAM,UA6RxMailbox,UA6RxDMAbuf,UA6RxMBbuf_LEN,UA6RxDMAbuf_LEN,0,0,0};
		UN_VISION_DATA_DN VisionDataReceiveBuff 	= {0};						// ����̨�Ӿ�����������
		float 	VisionData1 = 0;	//��¼ǰ֡����
		float 	VisionData2 = 0;	//��¼ǰ֡����
		float		VisionData3 = 0;
		float		VisionData4 = 0;
		float		VisionData5 = 0;
		float		VisionData6 = 0;
		float		VisionData7 = 0;
		float		VisionData8 = 0;
		float		VisionData9 = 0;
		float		VisionData10 = 0;
		float		VisionData11 = 0;
		
		
		float		VisionSendData1 = 0;
		float		VisionSendData2 = 0;
		float		VisionSendData3 = 0;
		float		VisionSendData4 = 0;
		float		VisionSendData5 = 0;
		float		VisionSendData6 = 0;
		float		VisionSendData7 = 0;
		float		VisionSendData8 = 0;
		float		VisionSendData9 = 0;
		float		VisionSendData10 = 0;
		float		VisionSendData11 = 0;
	
		u8 			TirggerState = 0 ;

/** @brief ������ñ��� */
	/** @brief	���������ݽṹ�� */
		ST_ENCODER GimbalYawEncoder		= QuickEncoder(1,8192);
		ST_ENCODER ShootEncoder 		= QuickEncoder(36,8192);
		ST_ENCODER ChassisEncoder 		= QuickEncoder(14,8192);
		ST_ENCODER MotorEncoder				= QuickEncoder(1,8192);
		ST_ENCODER ChassisZigEncoder	= QuickEncoder(19,8192);	
	/** @brief �������pid */
		ST_PID GimbalYawPosPid;
		ST_PID GimbalYawSpeedPid;	
		ST_PID GimbalPitchPosPid;
		ST_PID GimbalPitchSpeedPid;
		ST_TD	 YawTD;
	/** @brief ���Ƶ���ĽǶ����ٶ� */
		float YawPosDes=0;
		float YawEncoderAngle=0;
		float YawEncoderSpeed=0;
		float YawBMIAngle=0;
		float YawBMISpeed=0;
		float PitchBMIAngle=0;
		float PitchBMISpeed=0;
	/** @brief �����ص�� */
		u32 	Friction_State_UP			= 500;								// Ħ����״̬(500ֹͣ700������
		u32 	Friction_State 				= 500;
		s16 	Bullet_Des 						= 0;											// ����Ŀ����Ŀ
		s16 	Bullet_Des_Pre 		= 0;								// ǰһ�β���Ŀ����Ŀ
		bool 	Follow_Flag_UP				= FALSE;							// ���ܸ����־λ
		bool 	Follow_Flag_DN				= FALSE;
		bool 	Shoot_Area_Flag				= FALSE;							// ����������־λ
		u8		Shoot_Frequency_UP		= 9;								// ����̨����
		u8		Shoot_Frequency_DN		= 9;								// ����̨����
		int 	upShooterStaticTimes 	= 0;								// ��������
		bool	FrictFirstIn 				= TRUE;										// ����Ħ������ʱ����
		bool 	BoDanFirstIn				= TRUE;	
		ST_ShootTestControl DNSTC = {										// ����̨���
			.BoDan_Flag 	= FALSE,
			.PreBoDan_Flag 	= FALSE,

			.Fact_ShootSumNumber 	= 0,
			.Fact_ShootNumber 		= 0,
			.Fact_pShootNumber 		= 0,
			.Des_pShootNumber 		= 10000,
		};


/** @brief ϵͳ��� */
	/**	@brief	ϵͳ������ */
		SYSTEM_MONITOR 		systemMonitor 		= {0};
		SUB_SYSTEM_MONITOR	SubSystemMonitor 	= {0};
		RS_SYSTEM_MONITOR	RSSystemMonitor		= {0};
		
		ST_ERROR stError = {FALSE};	// ģ���쳣��־λ	
		ST_FLAG 	stFlag = {FALSE};	// ȫ�ֱ�־λ		
	/** @brief ģʽѡ�� */
		u8 ControlMode;
		
		
		