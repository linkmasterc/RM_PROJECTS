#include "global_declare.h"

/** @brief ͨѶ���ñ��� */
	/** @brief canͨѶ */
		CanRxMsg CAN1_RX_Message;
		CanRxMsg CAN2_RX_Message;
	/** @brief ����ϵͳ */
		u8 UA2TxDMAbuf[USART2_TX_DMA_BUF_LEN] = {0};
		__IO u8 UA2RxDMAbuf[USART2_RX_DMA_BUF_LEN] = {0};
    u8 UA2RxMailbox[USART2_RX_MB_BUF_LEN] = {0};
		USART_RX_TypeDef USART2_Rcr = {USART2,USART2_RX_STREAM,UA2RxMailbox,UA2RxDMAbuf,USART2_RX_MB_BUF_LEN,USART2_RX_DMA_BUF_LEN,0,0,0};

	
		u8 		RSReceiveDataBuff[USART2_RX_MB_BUF_LEN] 	= {0};	// ����ϵͳ����������
		u8 		BalancesoldierNumber  				= 0;
		bool 	BanlanReconizeFinshFlag				= FALSE;						// ƽ�ⲽ��ʶ�����־λ
		
		
		s16     Barrel1_Heat_Cur;															// ����̨��ǰǹ������
		s16     Barrel2_Heat_Cur;															// ����̨��ǰǹ������
		bool	Invincible_Status					= FALSE;								// �޵�״̬
		u16     Bullet_Num_Remain;															// ʣ���ӵ���Ŀ
		u16     SentryHPMessage						= 600;								// �ڱ���ǰѪ��
		float   ChassisPowerMessage					= 0;								// ��ǰ���̹���
		u16     ChassisPowerBufferMessageLast		= 0;						// �����ϴ�ʣ�໺������
		u16     ChassisPowerBufferMessage			= 0;							// ����ʣ�໺������
		u8      RobotIDMessage						= 7;									// ��������ID
		u8 		Bullet1FreqMessage;																// ����ϵͳ���ص�����̨��Ƶ
		u8		Bullet2FreqMessage;																// ����ϵͳ���ص�����̨��Ƶ
		float   Bullet1SpeedMessage;														// ����ϵͳ���ص�����̨����
		float   Bullet2SpeedMessage;														// ����ϵͳ���ص�����̨����
		u16		StageRemainTimeMessage;														// ��ǰ�����׶�ʣ��ʱ��


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
		
		u16		SumHurtPerSecond;																								// ÿ���˺�
		bool 	SumHurtFlag 						= FALSE;																// ���˱�־λ
		u8		HitOurselfFlag							= 0;																// ���򼺷���־λ
		
		bool	RSPowerUpdataFlag			= FALSE;		    													//����ϵͳ���ʸ��±�־λ��ʵ��10Hz
		u8 		CalibrateState				= CAPACITOR_CALIBRATE_WAIT;								//�궨״̬��־λ
	/**	@brief 	����ϵͳ���صı���ʵʱ��Ϣ */
		ext_game_status_t												GameStatus;										// ����״̬����
		ext_game_result_t												GameResult;										// �����������
		ext_game_robot_HP_t											GameRobotHP;									// ������Ѫ������
		ext_ICRA_buff_debuff_zone_status_t			ICRABuffDebuffZoneStatus;			// �˹�������ս���ӳ���ͷ���״̬
		ext_event_data_t												EventData;										// �����¼�����
		ext_supply_projectile_action_t					SupplyProjectileAction;				// ����վ������ʶ
		ext_referee_warning_t										RefereeWarning;								// ���о�����Ϣ
		ext_dart_remaining_time_t								DartRemainingTime;						// ���ڷ���ڵ���ʱ
		ext_game_robot_status_t									GameRobotStatus;							// ����������״̬
		ext_power_heat_data_t										PowerHeatData;								// ʵʱ������������
		ext_game_robot_pos_t										GameRobotPos;									// ������λ��
		ext_buff_t															Buff;													// ����������
		aerial_robot_energy_t										AerialRobotEnergy;						// ���л���������״̬
		ext_robot_hurt_t												RobotHurt = {.hurt_type = 9, 	// �˺�״̬
																												 .armor_id = 9};	
		ext_shoot_data_t												ShootData;										// ʵʱ�����Ϣ
		ext_bullet_remaining_t									BulletRemaining;							// �ӵ�ʣ�෢����Ŀ
		ext_rfid_status_t												RFIDStatus;										// ������RFID״̬
		ext_dart_client_cmd_t										DartClientCmd;								// ���ڻ����˿ͻ���ָ������
		ext_student_interactive_header_data_t		StudentInteractiveHeaderData;	// �����˼佻����Ϣ
		robot_interactive_data_t								RobotInteractiveData;					// ��������
	/** @brief ��ң����*/
		ST_DBUS  g_StDbus;	
	/** @brief ����ͨѶ����*/
				 u8 USART1_Cushioning_Rx[USART1_RX_BUF_LEN]={0};
			 
		__IO u8 USART3_Cushioning_Rx[ChassisBufLen_Rx]={0};
		__IO u8 UART4_Cushioning_Rx[1]={0};
		__IO u8 UART5_Cushioning_Rx[1]={0};
		ST_IMU GimbalData={.Send.head[0]=0x55,.Send.head[1]=0x00};
	/**	@brief	�Ӿ�ͨѶ */
		u8 VisionDataBuffTemp[VISION_RECIEVE_DATA_LEN - 1] 			= {0};
		VISION_SEND_DATA VisionDataSendBuff 	= {.My_Data.ID = 0x0A,.My_Data.Head1 = 0x55, .My_Data.Head2 = 0x00, .My_Data.Tail1 = 0x00, .My_Data.Tail2 = 0xAA};

		__IO u8 UA5RxDMAbuf[UART5_RX_BUF_LEN]= {0};
		__IO u8 UA6RxDMAbuf[UA6RxDMAbuf_LEN] = {0};
				 u8 UA6RxMailbox[UA6RxMBbuf_LEN] = {0};
		USART_RX_TypeDef USART6_Rcr = {USART6,USART6_RX_STREAM,UA6RxMailbox,UA6RxDMAbuf,UA6RxMBbuf_LEN,UA6RxDMAbuf_LEN,0,0,0};
		UN_VISION_DATA VisionDataReceiveBuff 	= {0};						// ����̨�Ӿ�����������
		NAVIGATION_RECEIVE_DATA   NavigationReceiveBuff   = {0};						// ��������������
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


/** @brief ������ñ��� */
	/** @brief	���������ݽṹ�� */
		ST_ENCODER GimbalYawEncoder		= QuickEncoder(1,8192);
		ST_ENCODER ShootEncoder 		= QuickEncoder(36,8192);
		ST_ENCODER ChassisEncoder 		= QuickEncoder(14,8192);
		ST_ENCODER MotorEncoder				= QuickEncoder(1,8192);
		ST_ENCODER ChassisZigEncoder	= QuickEncoder(19,8192);	
	/** @brief �������pid */
		ST_PID GimbalYawPosPid=QuickPID(30,0.5,0,20000,20000,10000,1500,1);
		ST_PID GimbalYawSpeedPid=QuickPID(280,0,0,0,28000,20000,20000,15);	
		ST_ENCODER GimbalYawABS=QuickEncoder(1,360);
		ST_PID GimbalPitchPosPid;
		ST_PID GimbalPitchSpeedPid;
		ST_TD	 YawTD=QuickTD(7500,0.001f,0.001f);
	/** @brief ���Ƶ���ĽǶ����ٶ� */
		float YawPosDes							=	0;
		float YawEncoderAngle       =	0;
		float YawEncoderSpeed				=	0;
		float YawBMIAngle						=	0;
		float YawBMISpeed						=	0;
		float PitchBMIAngle					= 0;
		float PitchBMISpeed					= 0;
		float SecondYawAngle        = 0;
		
		float Robot_Yaw_Des					= 0;
		float Robot_Yaw_Cur					= 0;
		float Chassis_Speed					= 0;
		float Wheel_Angle_Des				= 0;
		
		float WCSYawAngle						=	0;
		float LCSYawAngle						=	0;
		float WheelSpeed						= 0;
		float Receive_WheelSpeed=0;
	/** @brief �����ص�� */
		u32 	Friction_State_UP			= 500;							// Ħ����״̬(500ֹͣ700������
		u32 	Friction_State 				= 500;
		s16 	Bullet_Des 						= 0;								// ����Ŀ����Ŀ
		s16 	Bullet_Des_Pre 				= 0;								// ǰһ�β���Ŀ����Ŀ
		bool 	Follow_Flag_UP				= FALSE;						// ���ܸ����־λ
		bool 	Follow_Flag_DN				= FALSE;
		bool 	Shoot_Area_Flag				= FALSE;						// ����������־λ
		u8		Shoot_Frequency_UP		= 9;								// ����̨����
		u8		Shoot_Frequency_DN		= 9;								// ����̨����
		int 	upShooterStaticTimes 	= 0;								// ��������
		bool	FrictFirstIn 					= TRUE;							// ����Ħ������ʱ����
		bool 	Auto_BoDanFirstIn				= TRUE;	
		ST_ShootTestControl DNSTC 	= {									// ����̨���
			.BoDan_Flag 	= FALSE,
			.PreBoDan_Flag 	= FALSE,
			.Fact_ShootSumNumber 	= 0,
			.Fact_ShootNumber 		= 0,
			.Fact_pShootNumber 		= 0,
			.Des_pShootNumber 		= 10000,
		};
	/** @brief ���̵�� */
		ST_PID ChassisPowerPid;
		ST_PID stWheel_SpeedPid[4];
		ST_PID stServoWheel_PosPid[4];
		ST_PID stServoWheel_SpeedPid[4];
		ST_ENCODER stServoEncoder[4];
		ST_ENCODER g_stShooterEncoder 	= {0,0,0,0,36,8192,0};
		ST_PID c_stShooterPosPID 		  = QuickPID(0.18,0,0,10000,8000,0,0,100);
		ST_PID c_stShooterSpeedPID 		= QuickPID(100,0,0,10000,10000,0,0,20);
		CAPACITOR_MSG capacitor_msg = {0};

		

/** @brief ϵͳ��� */
	/**	@brief	ϵͳ������ */
		SYSTEM_MONITOR 		systemMonitor 		= {0};
		SUB_SYSTEM_MONITOR	SubSystemMonitor 	= {0};
		RS_SYSTEM_MONITOR	RSSystemMonitor		= {0};
		
		ST_ERROR stError = {FALSE};	// ģ���쳣��־λ	
		ST_FLAG 	stFlag = {FALSE};	// ȫ�ֱ�־λ		
	/** @brief ģʽѡ�� */
		u8 ControlMode;
		
/** @brief ���ƹ�����Ҫ���� */
	u8			PreGimabalTestFlag 	= 0x00;
	float 		AccelStep 				= 0.7;
	float 		LineDecelerateStep 		= 18;
	u16			UPShootSpeedTest 		= 0;
	bool		StartUPShootFlag 	= FALSE;
	bool		StartDNShootFlag 	= FALSE;
	bool		LowShootFlag	 	= FALSE;
	u32 		ShootHeatTick 		= 0;//bool 		UP_Range_Mode_First_in_Flag = TRUE;
	//bool 		DN_Range_Mode_First_in_Flag = TRUE;
	bool		ChassisPositionLeftFlag		= TRUE;
	u8 			Chassis_Test_Flag	= 0x00;
	/** @brief ������ģ�� */
	u32 		TIM9_OverflowTimes=0;
	u32    TIM9_Rising_ARR=0;
	u32    TIM9_Falling_ARR=0;
	float    US_Distance=0;

	u8 			TirggerState = 0 ;
		
	/** @brief���̹��� */
		float PreChassisPowerMessage;

	/** @brief����ͨѶ */
		bool	Range360Flag	= TRUE;
		u8		ResetState		= 0x00;
		bool	FindEnemyR2		= FALSE;
		bool	FindEnemyRoad	= FALSE;

	/** @briefƽ�����ټ��� */
		float	Bullet1SpeedMean;
		float	Bullet1SpeedRecord[RecordBullet1Num];
		float	Bullet2SpeedMean;
		float	Bullet2SpeedRecord[RecordBullet2Num];

	/** @brief�������� */
		u16		Shooter1HeatRemainingNum;
		u16		Shooter2HeatRemainingNum;
		u16		Shooter1ShootNum			=	0;
		u16		Shooter2ShootNum			=	0;
		u16		Shooter1StableBulletFreq	=	5;
		u16		Shooter2StableBulletFreq	=	5;
		float	Shooter1NumberLast;
		float	Shooter2NumberLast;
	
	/**@brief�з���λ��� */
		bool		Robot1ShootFlag		= TRUE;
		bool		Robot2ShootFlag		= TRUE;
		bool		Robot3ShootFlag		= TRUE;
		bool		Robot4ShootFlag		= TRUE;
		bool		Robot5ShootFlag		= TRUE;

/**�����������**/
// Vofa++
ST_UART_DATA_VOFA StateDataSendBuff = {.Tail1 = 0x00, .Tail2 = 0x00, .Tail3 = 0x80, .Tail4 = 0x7F};	

/***********************************************************************/
	
/**�˲���**/
ST_KMF US_KMF={.m_preP=1,.m_Q=0.1,.m_R=0.8};






