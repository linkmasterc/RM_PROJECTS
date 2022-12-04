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

#define GM_YAW_SENSIBILITY		3800							// 遥控器yaw灵敏度
#define GM_PITCH_SENSIBILITY	12								// 遥控器pitch灵敏度
#define GM_YAW_SENSIBILITY_TEST		0.1					// 遥控器yaw灵敏度(调弹道用）
#define GM_PITCH_SENSIBILITY_TEST	25						// 遥控器pitch灵敏度（调弹道用）

#define RecordBullet1Num	10
#define RecordBullet2Num	10
#define RecordEncoderLengthNum	10
#define __packed

/** @brief 通讯所用变量 */
	/** @brief can通讯 */
		extern CanRxMsg CAN1_RX_Message;
		extern CanRxMsg CAN2_RX_Message;
	
	
	/** @brief 大疆遥控器*/
		extern ST_DBUS 	g_StDbus;	
	/** @brief 裁判系统 */
	  extern u8 UA2TxDMAbuf[USART2_TX_DMA_BUF_LEN];
		extern __IO u8 UA2RxDMAbuf[USART2_RX_DMA_BUF_LEN];
    extern  u8 UA2RxMailbox[USART2_RX_MB_BUF_LEN];
		extern USART_RX_TypeDef USART2_Rcr;
	

		extern u8 		RSReceiveDataBuff[USART2_RX_MB_BUF_LEN];					// 裁判系统发来的数据
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
		
		extern u16		SumHurtPerSecond;											// 每秒伤害
		extern bool 	SumHurtFlag;													// 受伤标志位
		extern u8		HitOurselfFlag;													// 击打己方标志位
		
		extern bool	RSPowerUpdataFlag;														//裁判系统功率更新标志位，实际10Hz
		extern u8 	CalibrateState;																//标定状态标志位
		
		extern float PreChassisPowerMessage;
		
		extern ext_game_status_t						GameStatus;						// 比赛状态数据
		extern ext_game_result_t						GameResult;						// 比赛结果数据
		extern ext_game_robot_HP_t						GameRobotHP;					// 机器人血量数据
		extern ext_ICRA_buff_debuff_zone_status_t		ICRABuffDebuffZoneStatus;		// 人工智能挑战赛加成与惩罚区状态
		extern ext_event_data_t						EventData;						// 场地事件数据
		extern ext_supply_projectile_action_t			SupplyProjectileAction;			// 补给站动作标识
		extern ext_referee_warning_t					RefereeWarning;					// 裁判警告信息
		extern ext_dart_remaining_time_t				DartRemainingTime;				// 飞镖发射口倒计时
		extern ext_game_robot_status_t					GameRobotStatus;				// 比赛机器人状态
		extern ext_power_heat_data_t					PowerHeatData;					// 实时功率热量数据
		extern ext_game_robot_pos_t					GameRobotPos;					// 机器人位置
		extern ext_buff_t								Buff;							// 机器人增益
		extern aerial_robot_energy_t					AerialRobotEnergy;				// 空中机器人能量状态
		extern ext_robot_hurt_t						RobotHurt;						// 伤害状态
		extern ext_shoot_data_t						ShootData;						// 实时射击信息
		extern ext_bullet_remaining_t					BulletRemaining;				// 子弹剩余发射数目
		extern ext_rfid_status_t						RFIDStatus;						// 机器人RFID状态
		extern ext_dart_client_cmd_t					DartClientCmd;					// 飞镖机器人客户端指令数据
		extern ext_student_interactive_header_data_t	StudentInteractiveHeaderData;	// 机器人间交互信息
		extern robot_interactive_data_t				RobotInteractiveData;			// 交互数据extern 
	
	/** @brief 串口通讯变量*/
		extern u8 USART1_Cushioning_Rx[USART1_RX_BUF_LEN];
		extern __IO u8 USART2_Cushioning_Rx[1];		
		extern __IO u8 USART3_Cushioning_Rx[ChassisBufLen_Rx];
		extern __IO u8 UART4_Cushioning_Rx[1];
		extern __IO u8 UART5_Cushioning_Rx[1];
		extern ST_IMU GimbalData;
	/**	@brief	视觉通讯 */
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


		extern u8 			TirggerState;

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
		extern bool Auto_BoDanFirstIn;
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
		
		
/** @brief 控制过程重要参数 */
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

	/** @brief底盘功率 */
	

	/** @brief车间通讯 */
		extern bool	Range360Flag;
		extern u8		ResetState;
		extern bool	FindEnemyR2	;
		extern bool	FindEnemyRoad;

	/** @brief平均弹速计算 */
		extern float	Bullet1SpeedMean;
		extern float	Bullet1SpeedRecord[RecordBullet1Num];
		extern float	Bullet2SpeedMean;
		extern float	Bullet2SpeedRecord[RecordBullet2Num];

	/** @brief打弹量计算 */
		extern u16		Shooter1HeatRemainingNum;
		extern u16		Shooter2HeatRemainingNum;
		extern u16		Shooter1ShootNum;
		extern u16		Shooter2ShootNum;
		extern u16		Shooter1StableBulletFreq;
		extern u16		Shooter2StableBulletFreq;
		extern float	Shooter1NumberLast;
		extern float	Shooter2NumberLast;
	
	/**@brief敌方单位编号 */
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






