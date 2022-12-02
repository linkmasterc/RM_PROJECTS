#include "global_declare.h"

/** @brief 通讯所用变量 */
	/** @brief can通讯 */
		CanRxMsg CAN1_RX_Message;
		CanRxMsg CAN2_RX_Message;
	/** @brief 裁判系统 */
		u8 		RSReceiveDataBuff[USART2_RX_BUF_LEN] 	= {0};					// 裁判系统发来的数据
		u8 		BalancesoldierNumber  				= 0;
		bool 	BanlanReconizeFinshFlag				= FALSE;				// 平衡步兵识别完标志位
		
		
		s16     Barrel_Heat_Cur_UP;											// 上云台当前枪管热量
		s16     Barrel_Heat_Cur_DN;											// 下云台当前枪管热量
		bool	Invincible_Status					= FALSE;					// 无敌状态
		u16     Bullet_Num_Remain;											// 剩余子弹数目
		u16     SentryHPMessage						= 600;					// 哨兵当前血量
		float   ChassisPowerMessage					= 0;					// 当前底盘功率
		u16     ChassisPowerBufferMessageLast		= 0;					// 底盘上次剩余缓冲能量
		u16     ChassisPowerBufferMessage			= 0;					// 底盘剩余缓冲能量
		u8      RobotIDMessage						= 7;												// 本机器人ID
		u8 		Bullet1FreqMessage;											// 裁判系统返回的上云台弹频
		u8		Bullet2FreqMessage;											// 裁判系统返回的下云台弹频
		float   Bullet1SpeedMessage;										// 裁判系统返回的上云台射速
		float   Bullet2SpeedMessage;										// 裁判系统返回的下云台射速
		u16		StageRemainTimeMessage;										// 当前比赛阶段剩余时间


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
	/** @brief 大疆遥控器*/
		ST_DBUS  g_StDbus;	
	/** @brief 串口通讯变量*/
		__IO u8 USART2_Cushioning_Rx[1]={0};
		__IO u8 USART3_Cushioning_Rx[ChassisBufLen_Rx]={0};
		__IO u8 UART4_Cushioning_Rx[1]={0};
		__IO u8 UART5_Cushioning_Rx[1]={0};
		ST_IMU GimbalReceiveData;		
		ST_IMU GimbalSendData;
	/**	@brief	视觉通讯 */
		u8 VisionDataBuffTemp[VISION_RECIEVE_DATA_LEN_DN - 1] 			= {0};
		VISION_SEND_DATA_DN VisionDataSendBuff 	= {.My_Data.ID = 0x0A,.My_Data.Head1 = 0x55, .My_Data.Head2 = 0x00, .My_Data.Tail1 = 0x00, .My_Data.Tail2 = 0xAA};
		__IO u8 UA6RxDMAbuf[UA6RxDMAbuf_LEN] = {0};
				 u8 UA6RxMailbox[UA6RxMBbuf_LEN] = {0};
		USART_RX_TypeDef USART6_Rcr = {USART6,USART6_RX_STREAM,UA6RxMailbox,UA6RxDMAbuf,UA6RxMBbuf_LEN,UA6RxDMAbuf_LEN,0,0,0};
		UN_VISION_DATA_DN VisionDataReceiveBuff 	= {0};						// 下云台视觉发来的数据
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
	
		u8 			TirggerState = 0 ;

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
	/** @brief 射击相关电机 */
		u32 	Friction_State_UP			= 500;								// 摩擦轮状态(500停止700开启）
		u32 	Friction_State 				= 500;
		s16 	Bullet_Des 						= 0;											// 拨弹目标数目
		s16 	Bullet_Des_Pre 		= 0;								// 前一次拨弹目标数目
		bool 	Follow_Flag_UP				= FALSE;							// 紧密跟随标志位
		bool 	Follow_Flag_DN				= FALSE;
		bool 	Shoot_Area_Flag				= FALSE;							// 射击区域检测标志位
		u8		Shoot_Frequency_UP		= 9;								// 上云台射速
		u8		Shoot_Frequency_DN		= 9;								// 下云台射速
		int 	upShooterStaticTimes 	= 0;								// 卡弹计数
		bool	FrictFirstIn 				= TRUE;										// 用于摩擦轮延时启动
		bool 	BoDanFirstIn				= TRUE;	
		ST_ShootTestControl DNSTC = {										// 下云台射击
			.BoDan_Flag 	= FALSE,
			.PreBoDan_Flag 	= FALSE,

			.Fact_ShootSumNumber 	= 0,
			.Fact_ShootNumber 		= 0,
			.Fact_pShootNumber 		= 0,
			.Des_pShootNumber 		= 10000,
		};


/** @brief 系统侦测 */
	/**	@brief	系统监视器 */
		SYSTEM_MONITOR 		systemMonitor 		= {0};
		SUB_SYSTEM_MONITOR	SubSystemMonitor 	= {0};
		RS_SYSTEM_MONITOR	RSSystemMonitor		= {0};
		
		ST_ERROR stError = {FALSE};	// 模块异常标志位	
		ST_FLAG 	stFlag = {FALSE};	// 全局标志位		
	/** @brief 模式选择 */
		u8 ControlMode;
		
		
		