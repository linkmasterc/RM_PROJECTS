#include "global_declare.h"

/** @brief 通讯所用变量 */
	/** @brief can通讯 */
		CanRxMsg CAN1_RX_Message;
		CanRxMsg CAN2_RX_Message;
	/** @brief 裁判系统 */
		u8 UA2TxDMAbuf[USART2_TX_DMA_BUF_LEN] = {0};
		__IO u8 UA2RxDMAbuf[USART2_RX_DMA_BUF_LEN] = {0};
    u8 UA2RxMailbox[USART2_RX_MB_BUF_LEN] = {0};
		USART_RX_TypeDef USART2_Rcr = {USART2,USART2_RX_STREAM,UA2RxMailbox,UA2RxDMAbuf,USART2_RX_MB_BUF_LEN,USART2_RX_DMA_BUF_LEN,0,0,0};

	
		u8 		RSReceiveDataBuff[USART2_RX_MB_BUF_LEN] 	= {0};	// 裁判系统发来的数据
		u8 		BalancesoldierNumber  				= 0;
		bool 	BanlanReconizeFinshFlag				= FALSE;						// 平衡步兵识别完标志位
		
		
		s16     Barrel1_Heat_Cur;															// 上云台当前枪管热量
		s16     Barrel2_Heat_Cur;															// 下云台当前枪管热量
		bool	Invincible_Status					= FALSE;								// 无敌状态
		u16     Bullet_Num_Remain;															// 剩余子弹数目
		u16     SentryHPMessage						= 600;								// 哨兵当前血量
		float   ChassisPowerMessage					= 0;								// 当前底盘功率
		u16     ChassisPowerBufferMessageLast		= 0;						// 底盘上次剩余缓冲能量
		u16     ChassisPowerBufferMessage			= 0;							// 底盘剩余缓冲能量
		u8      RobotIDMessage						= 7;									// 本机器人ID
		u8 		Bullet1FreqMessage;																// 裁判系统返回的上云台弹频
		u8		Bullet2FreqMessage;																// 裁判系统返回的下云台弹频
		float   Bullet1SpeedMessage;														// 裁判系统返回的上云台射速
		float   Bullet2SpeedMessage;														// 裁判系统返回的下云台射速
		u16		StageRemainTimeMessage;														// 当前比赛阶段剩余时间


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
		
		u16		SumHurtPerSecond;																								// 每秒伤害
		bool 	SumHurtFlag 						= FALSE;																// 受伤标志位
		u8		HitOurselfFlag							= 0;																// 击打己方标志位
		
		bool	RSPowerUpdataFlag			= FALSE;		    													//裁判系统功率更新标志位，实际10Hz
		u8 		CalibrateState				= CAPACITOR_CALIBRATE_WAIT;								//标定状态标志位
	/**	@brief 	裁判系统返回的比赛实时信息 */
		ext_game_status_t												GameStatus;										// 比赛状态数据
		ext_game_result_t												GameResult;										// 比赛结果数据
		ext_game_robot_HP_t											GameRobotHP;									// 机器人血量数据
		ext_ICRA_buff_debuff_zone_status_t			ICRABuffDebuffZoneStatus;			// 人工智能挑战赛加成与惩罚区状态
		ext_event_data_t												EventData;										// 场地事件数据
		ext_supply_projectile_action_t					SupplyProjectileAction;				// 补给站动作标识
		ext_referee_warning_t										RefereeWarning;								// 裁判警告信息
		ext_dart_remaining_time_t								DartRemainingTime;						// 飞镖发射口倒计时
		ext_game_robot_status_t									GameRobotStatus;							// 比赛机器人状态
		ext_power_heat_data_t										PowerHeatData;								// 实时功率热量数据
		ext_game_robot_pos_t										GameRobotPos;									// 机器人位置
		ext_buff_t															Buff;													// 机器人增益
		aerial_robot_energy_t										AerialRobotEnergy;						// 空中机器人能量状态
		ext_robot_hurt_t												RobotHurt = {.hurt_type = 9, 	// 伤害状态
																												 .armor_id = 9};	
		ext_shoot_data_t												ShootData;										// 实时射击信息
		ext_bullet_remaining_t									BulletRemaining;							// 子弹剩余发射数目
		ext_rfid_status_t												RFIDStatus;										// 机器人RFID状态
		ext_dart_client_cmd_t										DartClientCmd;								// 飞镖机器人客户端指令数据
		ext_student_interactive_header_data_t		StudentInteractiveHeaderData;	// 机器人间交互信息
		robot_interactive_data_t								RobotInteractiveData;					// 交互数据
	/** @brief 大疆遥控器*/
		ST_DBUS  g_StDbus;	
	/** @brief 串口通讯变量*/
				 u8 USART1_Cushioning_Rx[USART1_RX_BUF_LEN]={0};
			 
		__IO u8 USART3_Cushioning_Rx[ChassisBufLen_Rx]={0};
		__IO u8 UART4_Cushioning_Rx[1]={0};
		__IO u8 UART5_Cushioning_Rx[1]={0};
		ST_IMU GimbalData={.Send.head[0]=0x55,.Send.head[1]=0x00};
	/**	@brief	视觉通讯 */
		u8 VisionDataBuffTemp[VISION_RECIEVE_DATA_LEN - 1] 			= {0};
		VISION_SEND_DATA VisionDataSendBuff 	= {.My_Data.ID = 0x0A,.My_Data.Head1 = 0x55, .My_Data.Head2 = 0x00, .My_Data.Tail1 = 0x00, .My_Data.Tail2 = 0xAA};

		__IO u8 UA5RxDMAbuf[UART5_RX_BUF_LEN]= {0};
		__IO u8 UA6RxDMAbuf[UA6RxDMAbuf_LEN] = {0};
				 u8 UA6RxMailbox[UA6RxMBbuf_LEN] = {0};
		USART_RX_TypeDef USART6_Rcr = {USART6,USART6_RX_STREAM,UA6RxMailbox,UA6RxDMAbuf,UA6RxMBbuf_LEN,UA6RxDMAbuf_LEN,0,0,0};
		UN_VISION_DATA VisionDataReceiveBuff 	= {0};						// 下云台视觉发来的数据
		NAVIGATION_RECEIVE_DATA   NavigationReceiveBuff   = {0};						// 导航发来的数据
		float 	VisionData1 = 0;	//记录前帧数据
		float 	VisionData2 = 0;	//记录前帧数据
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


/** @brief 电机所用变量 */
	/** @brief	编码器数据结构体 */
		ST_ENCODER GimbalYawEncoder		= QuickEncoder(1,8192);
		ST_ENCODER ShootEncoder 		= QuickEncoder(36,8192);
		ST_ENCODER ChassisEncoder 		= QuickEncoder(14,8192);
		ST_ENCODER MotorEncoder				= QuickEncoder(1,8192);
		ST_ENCODER ChassisZigEncoder	= QuickEncoder(19,8192);	
	/** @brief 电机控制pid */
		ST_PID GimbalYawPosPid=QuickPID(30,0.5,0,20000,20000,10000,1500,1);
		ST_PID GimbalYawSpeedPid=QuickPID(280,0,0,0,28000,20000,20000,15);	
		ST_ENCODER GimbalYawABS=QuickEncoder(1,360);
		ST_PID GimbalPitchPosPid;
		ST_PID GimbalPitchSpeedPid;
		ST_TD	 YawTD=QuickTD(7500,0.001f,0.001f);
	/** @brief 控制电机的角度与速度 */
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
	/** @brief 射击相关电机 */
		u32 	Friction_State_UP			= 500;							// 摩擦轮状态(500停止700开启）
		u32 	Friction_State 				= 500;
		s16 	Bullet_Des 						= 0;								// 拨弹目标数目
		s16 	Bullet_Des_Pre 				= 0;								// 前一次拨弹目标数目
		bool 	Follow_Flag_UP				= FALSE;						// 紧密跟随标志位
		bool 	Follow_Flag_DN				= FALSE;
		bool 	Shoot_Area_Flag				= FALSE;						// 射击区域检测标志位
		u8		Shoot_Frequency_UP		= 9;								// 上云台射速
		u8		Shoot_Frequency_DN		= 9;								// 下云台射速
		int 	upShooterStaticTimes 	= 0;								// 卡弹计数
		bool	FrictFirstIn 					= TRUE;							// 用于摩擦轮延时启动
		bool 	Auto_BoDanFirstIn				= TRUE;	
		ST_ShootTestControl DNSTC 	= {									// 下云台射击
			.BoDan_Flag 	= FALSE,
			.PreBoDan_Flag 	= FALSE,
			.Fact_ShootSumNumber 	= 0,
			.Fact_ShootNumber 		= 0,
			.Fact_pShootNumber 		= 0,
			.Des_pShootNumber 		= 10000,
		};
	/** @brief 底盘电机 */
		ST_PID ChassisPowerPid;
		ST_PID stWheel_SpeedPid[4];
		ST_PID stServoWheel_PosPid[4];
		ST_PID stServoWheel_SpeedPid[4];
		ST_ENCODER stServoEncoder[4];
		ST_ENCODER g_stShooterEncoder 	= {0,0,0,0,36,8192,0};
		ST_PID c_stShooterPosPID 		  = QuickPID(0.18,0,0,10000,8000,0,0,100);
		ST_PID c_stShooterSpeedPID 		= QuickPID(100,0,0,10000,10000,0,0,20);
		CAPACITOR_MSG capacitor_msg = {0};

		

/** @brief 系统侦测 */
	/**	@brief	系统监视器 */
		SYSTEM_MONITOR 		systemMonitor 		= {0};
		SUB_SYSTEM_MONITOR	SubSystemMonitor 	= {0};
		RS_SYSTEM_MONITOR	RSSystemMonitor		= {0};
		
		ST_ERROR stError = {FALSE};	// 模块异常标志位	
		ST_FLAG 	stFlag = {FALSE};	// 全局标志位		
	/** @brief 模式选择 */
		u8 ControlMode;
		
/** @brief 控制过程重要参数 */
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
	/** @brief 超声波模块 */
	u32 		TIM9_OverflowTimes=0;
	u32    TIM9_Rising_ARR=0;
	u32    TIM9_Falling_ARR=0;
	float    US_Distance=0;

	u8 			TirggerState = 0 ;
		
	/** @brief底盘功率 */
		float PreChassisPowerMessage;

	/** @brief车间通讯 */
		bool	Range360Flag	= TRUE;
		u8		ResetState		= 0x00;
		bool	FindEnemyR2		= FALSE;
		bool	FindEnemyRoad	= FALSE;

	/** @brief平均弹速计算 */
		float	Bullet1SpeedMean;
		float	Bullet1SpeedRecord[RecordBullet1Num];
		float	Bullet2SpeedMean;
		float	Bullet2SpeedRecord[RecordBullet2Num];

	/** @brief打弹量计算 */
		u16		Shooter1HeatRemainingNum;
		u16		Shooter2HeatRemainingNum;
		u16		Shooter1ShootNum			=	0;
		u16		Shooter2ShootNum			=	0;
		u16		Shooter1StableBulletFreq	=	5;
		u16		Shooter2StableBulletFreq	=	5;
		float	Shooter1NumberLast;
		float	Shooter2NumberLast;
	
	/**@brief敌方单位编号 */
		bool		Robot1ShootFlag		= TRUE;
		bool		Robot2ShootFlag		= TRUE;
		bool		Robot3ShootFlag		= TRUE;
		bool		Robot4ShootFlag		= TRUE;
		bool		Robot5ShootFlag		= TRUE;

/**串口助手相关**/
// Vofa++
ST_UART_DATA_VOFA StateDataSendBuff = {.Tail1 = 0x00, .Tail2 = 0x00, .Tail3 = 0x80, .Tail4 = 0x7F};	

/***********************************************************************/
	
/**滤波器**/
ST_KMF US_KMF={.m_preP=1,.m_Q=0.1,.m_R=0.8};






