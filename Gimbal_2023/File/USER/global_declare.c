#include "global_declare.h"

SYSTEM_MONITOR 		system_monitor = {0};             //系统监视器

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
		ST_IMU ChassisData={.Send.head[0]=0x55,.Send.head[1]=0x00};


/** @brief 电机所用变量 */
	/** @brief	编码器数据结构体 */
		ST_ENCODER GimbalYawEncoder		= QuickEncoder(1,8192);
		ST_ENCODER ShootEncoder 		= QuickEncoder(36,8192);
		ST_ENCODER ChassisEncoder 		= QuickEncoder(14,8192);
		ST_ENCODER MotorEncoder				= QuickEncoder(1,8192);
		ST_ENCODER ChassisZigEncoder	= QuickEncoder(19,8192);	
		ST_ENCODER GimbalSecondEncoder=QuickEncoder(1,8192);
	/** @brief 电机控制pid */
		ST_PID GimbalYawPosPid;
		ST_PID GimbalYawSpeedPid;	
		ST_PID GimbalPitchPosPid;
		ST_PID GimbalPitchSpeedPid;
		ST_PID GimbalSecondPosPid={.m_fpKp=30,.m_fpUMax=1000,.m_fpUpMax=1000};
		ST_PID GimbalSecondSpeedPid={.m_fpKp=200,.m_fpUMax=8000,.m_fpUpMax=8000};
		ST_TD	 YawTD;
	/** @brief 控制电机的角度与速度 */
		float YawPosDes=0;
		float YawEncoderAngle=0;
		float YawEncoderSpeed=0;
		float YawBMIAngle=0;
		float YawBMISpeed=0;
		
		float PitchPosDes=0;
		float PitchBMIAngle=0;
		float PitchBMISpeed=0;
	/** @brief 摩擦轮 */
		float Friction1_Temp = 0;
		float Friction2_Temp = 0;
		float Real_Friction1_Temp = 0;
		float Real_Friction2_Temp = 0;
		ST_ENCODER g_stFriction1Encoder	= {0,0,0,0,36,8192,0};
		ST_ENCODER g_stFriction2Encoder = {0,0,0,0,36,8192,0};
		ST_SMC g_stFriction1SMC	= {
			.m_b = 4.4936132,
			.m_eps = 8000,
			.m_dead = 5,
			.m_gain = 20,
			.m_fpDes = 0,
			.m_fpFB = 0,
			.m_fpU = 0,
			.m_fpUMax = 16200,
			
			.m_TD.m_h = 0.001,
			.m_TD.m_r = 5000,
			.m_TD.m_T = 0.001,
			.m_TD.m_aim = 0,
			.m_TD.m_x = 0,
			.m_TD.m_x1 = 0,
			.m_TD.m_x2 = 0
		};
		ST_SMC g_stFriction2SMC = {
			.m_b = 4.4936132,
			.m_eps = 8000,
			.m_dead = 5,
			.m_gain = 20,
			.m_fpDes = 0,
			.m_fpFB = 0,
			.m_fpU = 0,
			.m_fpUMax = 16200,
			
			.m_TD.m_h = 0.001,
			.m_TD.m_r = 5000,
			.m_TD.m_T = 0.001,
			.m_TD.m_aim = 0,
			.m_TD.m_x = 0,
			.m_TD.m_x1 = 0,
			.m_TD.m_x2 = 0
		};
	/** @brief 拨弹电机 */
		bool Shooter_BLOCK = FALSE;
		float ShooterN = 0;
		ST_ENCODER g_stShooterEncoder 	= {0,0,0,0,36,8192,0};
		ST_PID g_stShooterPosPID 		= QuickPID(0.215,0,0,8000,6000,100,0,100);
		ST_PID g_stShooterSpeedPID 		= QuickPID(500,0,0,8000,10000,0,10000,20);
	/** @brief pitch轴电机 */
		float COMP = 0;
		float PitchTest = 0;
		s16 G_Compensate = 2500;
		float Pitch_Encoder_angle = 0;
		float Pitch_Encoder_speed = 0;
		ST_ANGLE BMIPitchAngle	= {0};
		float BMIPitchSpeed = 0;
		ST_ANGLE BMIYawAngle	= {0};
		float BMIYawSpeed	= 0;

		float PitchDiff	= 0.0f;
		float Gravity_Compensate = -11300;

		ST_ENCODER g_stPitchEncoder 	= {0,0,0,0,1,8192,0};
		float PitchSpeedCompensate=0;

		#if defined(PITCH_ANGLE_FEEDBACK_GRYOSCOPE) && defined(PITCH_SPEED_FEEDBACK_GRYOSCOPE)
		/**	@brief angle & speed used gryoscope
			*	@date ?
			*/
		float	PitchCoe			= 1.0f;
		ST_TD 	g_stPitchTD			= QuickTD(5000,0.001f,0.001f);
		ST_PID 	g_stPitchPosPID 	= QuickPID(40,0.5,0,3000,3000,3000,1500,1);
		ST_PID 	g_stPitchSpeedPID 	= QuickPID(300,0,0,28000,28000,1,15000,12);

		#elif defined(PITCH_ANGLE_FEEDBACK_ENCODER) && defined(PITCH_SPEED_FEEDBACK_GRYOSCOPE)
		/** @brief angle used encoder, speed used gryoscope	
			*	@date ?
			*/
		float	PitchCoe			= 1.0f;
		ST_TD 	g_stPitchTD			= QuickTD(6000,0.008f,0.001f);
		ST_PID 	g_stPitchPosPID 	= QuickPID(-45,-0.01,0,3000,3000,6000,1500,100);	// 有前馈
		ST_PID 	g_stPitchSpeedPID 	= QuickPID(-300,0,0,28000,28000,15000,15000,12);
		#endif	

/** @brief 系统侦测 */
	/**	@brief	系统监视器 */
		SYSTEM_MONITOR 		systemMonitor 		= {0};
	  SUB_SYSTEM_MONITOR	SubSystemMonitor 	= {0};
		RS_SYSTEM_MONITOR	RSSystemMonitor		= {0};
		
		ST_ERROR stError = {FALSE};	// 模块异常标志位	
		ST_FLAG 	stFlag = {FALSE};	// 全局标志位		
		ST_GIMBAL_FLAG stGimbalFlag; //云台控制标志位
		
		s16	Barrel1_Heat_Cur=0;
		s16	Barrel2_Heat_Cur=0;
		float Bullet1SpeedMessage=0;
		float Bullet2SpeedMessage=0;		
		
		
	/**滤波器**/
	ST_KMF GimbalYawKMF={.m_preP=1,.m_Q=0.1,.m_R=0.8};
