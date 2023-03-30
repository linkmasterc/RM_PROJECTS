#include "mode_choose_task.h"
#include "global_declare.h"
//#include "data_send_task.h"




#define GIMBAL_OFF				TRUE
#define GIMBAL_ON					FALSE

#define SPEED_MOTOR_OFF		TRUE
#define SPEED_MOTOR_ON		FALSE

#define SERVO_MOTOR_OFF   TRUE
#define SERVO_MOTOR_ON    FALSE

#define SHOOT_ON					TRUE
#define SHOOT_OFF					FALSE





static void 	MotorInit(bool GimbalOff, bool SpeedMotorOff, bool ServoMotorOff);								// 运动电机初始化

static void 	ShootInit(bool UPShoot);											// 发射机构初始化

//static void 	ShootSpeedStrategy(void);														// 射速决策

//static void 	UPShootSpeedDistanceStraegy(void);												// 上云台射速决策（敌方距离）

//static void 	UPShootSpeedHeatStraegy(void);													// 上云台射速决策（当前热量）

//static void 	DNShootSpeedDistanceStraegy(void);												// 下云台射速决策（敌方距离）

//static void 	DNShootSpeedHeatStraegy(void);													// 下云台射速决策（当前热量）

//static void 	ChassisMoveStrategy(void);														// 底盘运动方式决策

//static void 	DangerStateJudge(void);															// 危险状态判断

//static void		EnemyDistanceJudge(void);														// 敌方距离判断

//static void 	RecognizeBanlanceSoldier(void);													// 平衡步兵判断


void ModeChoose(void)
{
	/*********************安全模式********************/
	/**@breif
					 左下 右下
				或 左上 右上
	*/
	if((g_StDbus.stRC.SW_L==RC_SW_DOWN&&g_StDbus.stRC.SW_R==RC_SW_DOWN)||(g_StDbus.stRC.SW_L==RC_SW_UP&&g_StDbus.stRC.SW_R==RC_SW_UP))
	{
		ControlMode=0x00;
		MotorInit(GIMBAL_OFF,SPEED_MOTOR_OFF,SERVO_MOTOR_OFF);
		ShootInit(SHOOT_OFF);
	}
	
	
	/*********************机械测试模式********************/
	/**@breif
					 左下 右中
					 右摇杆可控制pit和yaw缓慢运动
					 左摇杆控制弹频，从垂直最下逆时针波动，弹频依次为，1，9，13，15发/秒
					 此模式同时用于底盘直线行驶舵向电机标定，全部电调朝内侧（导轮方向为前后方向）
	*/
	else if (g_StDbus.stRC.SW_L==RC_SW_DOWN&&g_StDbus.stRC.SW_R==RC_SW_MID)
	{
		ControlMode=0x01;
		MotorInit(GIMBAL_ON,SPEED_MOTOR_OFF,SERVO_MOTOR_ON);
		ShootInit(SHOOT_ON);
	}
	
	
	/***********************视觉跟随模式*************************/
	/**@breif
					 左下 右上
				   遥控器左上角拨轮下拨摩擦轮开启，上拨摩擦轮关闭
			     左摇杆控制打弹，弹频与机械测试模式相同
				   右摇杆控制底盘移动（右摇杆看视觉需求）

	*/
	else if(g_StDbus.stRC.SW_L==RC_SW_DOWN&&g_StDbus.stRC.SW_R==RC_SW_UP)
	{
		ControlMode=0x02;
		MotorInit(GIMBAL_ON,SPEED_MOTOR_ON,SERVO_MOTOR_ON);
		ShootInit(SHOOT_ON);
	}
	
	
	/***********************小陀螺模式**************************/
	/**@brief
					 左中，右上
	  		   此模式云台同样由视觉控制
			     左摇杆控制打弹	
	*/
	else if(g_StDbus.stRC.SW_L==RC_SW_MID&&g_StDbus.stRC.SW_R==RC_SW_UP)
	{
		ControlMode=0x03;
		MotorInit(GIMBAL_ON,SPEED_MOTOR_ON,SERVO_MOTOR_ON);
		ShootInit(SHOOT_ON);
	}
	
	/*********************XX模式********************/
	else if(g_StDbus.stRC.SW_L==RC_SW_MID&&g_StDbus.stRC.SW_R==RC_SW_DOWN)
	{
		ControlMode=0x04;

	}

	/*********************XX模式********************/
	else if(g_StDbus.stRC.SW_L==RC_SW_MID&&g_StDbus.stRC.SW_R==RC_SW_DOWN)
	{
		ControlMode=0x05;

	}
	
	
	/*********************XX螺模式********************/
	else if(g_StDbus.stRC.SW_L==RC_SW_UP&&g_StDbus.stRC.SW_R==RC_SW_DOWN)
	{
		ControlMode=0x06;

	}
	
	/*********************XX模式********************/
	else if(g_StDbus.stRC.SW_L==RC_SW_UP&&g_StDbus.stRC.SW_R==RC_SW_MID)
	{
		ControlMode=0x07;

	}
	
	/*********************比赛模式********************/
	/**@brief
					 左中，右中
	*/
	else if(GameStatus.game_progress==4&&systemMonitor.USART1_rx_fps==0)
	{
		ControlMode=0x09;
		MotorInit(GIMBAL_ON,SPEED_MOTOR_ON,SERVO_MOTOR_ON);
		ShootInit(SHOOT_ON);
	}
	
	/*********************比赛结束模式**************************/
	else if(GameStatus.game_progress==5&&systemMonitor.USART1_rx_fps==0)
	{
		ControlMode=0x00;
		MotorInit(GIMBAL_OFF,SPEED_MOTOR_OFF,SERVO_MOTOR_OFF);
		ShootInit(SHOOT_OFF);
	}
}




/** --------------------------------------------------------------------------
  * @brief  云台&底盘电机模式初始化
			
  * @note	1000Hz
			使用遥控器对云台控制模式进行选择
 -------------------------------------------------------------------------- **/
void MotorInit(bool GimbalOff, bool SpeedMotorOff, bool ServoMotorOff)
{	
	if(GimbalOff)
	{
				stFlag.GimbalRunFlag = FALSE;			// 下云台电机失能
	}
	else{	stFlag.GimbalRunFlag = TRUE;}
	
	
	/*底盘速度轮电机*/
	if(SpeedMotorOff)
	{
				stFlag.CS_SPEED_RunFlag = FALSE;	// 底盘电机失能
//			rush.StartFlag 			= FALSE;			// 底盘启动标志位清零
	}
	else{	stFlag.CS_SPEED_RunFlag = TRUE;}
	
	if(ServoMotorOff)
	{
				stFlag.CS_SERVO_RunFlag = FALSE;   
	}
	else{ stFlag.CS_SERVO_RunFlag = TRUE;}
}





/** --------------------------------------------------------------------------
  * @brief  发射机构初始化
			
  * @note	
 -------------------------------------------------------------------------- **/
void ShootInit(bool ShootOn)
{
	if(ShootOn)
	{
		stFlag.ShootFlag  = TRUE;		// 发射机构启动标志位
	}
	else
	{
		//测弹模式重置		
		stFlag.ShootFlag  = FALSE;	// 发射机构启动标志位
		Friction_State_UP = 500;		// 摩擦轮开启标志位
	}
}




///** --------------------------------------------------------------------------
//  * @brief  弹频决策
//			
//  * @note	
// -------------------------------------------------------------------------- **/
//void ShootSpeedStrategy(void)
//{
//	if(stFlag.SniperFlag)										// 识别到目标
//	{
//		DNShootSpeedDistanceStraegy();																						// 根据敌方距离选择弹频
//		DNShootSpeedHeatStraegy();																							// 根据枪管热量选择弹频
//	}
//	if(stFlag.SniperFlag)										// 识别到目标
//	{
//		UPShootSpeedDistanceStraegy();																						// 根据敌方距离选择弹频
//		UPShootSpeedHeatStraegy();																							// 根据枪管热量选择弹频	
//	}
//}





///** --------------------------------------------------------------------------
//  * @brief  上云台弹频决策（敌方距离）
//			
//  * @note	优先级：当前热量>敌方距离
// -------------------------------------------------------------------------- **/
//void UPShootSpeedDistanceStraegy(void)
//{
//	if(Enemy_Distance_UP < DEFEND_THIRD_LINE_DISTANCE_UP)
//	{
//		Shoot_Frequency_UP = DEFEND_THIRD_LINE_SHOOT_FRC_UP;
//	}
//	else if(Enemy_Distance_UP < DEFEND_SECOND_LINE_DISTANCE_UP)
//	{
//		Shoot_Frequency_UP = DEFEND_SECOND_LINE_SHOOT_FRC_UP;
//	}
//	else if(Enemy_Distance_UP < DEFEND_FIRST_LINE_DISTANCE_UP)
//	{
//		Shoot_Frequency_UP = DEFEND_FIRST_LINE_SHOOT_FRC_UP;
//	}
//}





///** --------------------------------------------------------------------------
//  * @brief  上云台弹频决策（当前枪管热量）
//			
//  * @note	优先级：当前热量>敌方距离
//			设置射速上限
// -------------------------------------------------------------------------- **/
//void UPShootSpeedHeatStraegy(void)
//{
//	static bool RecoverFlag = FALSE;
//	if((Barrel_Heat_Cur_UP >= BARREL_HEAT_THIRD_LINE_UP) || (RecoverFlag && (Barrel_Heat_Cur_UP >= BARREL_HEAT_FIRST_LINE_UP)))
//	{
//		if(Shoot_Frequency_UP >= BARREL_HEAT_THIRD_LINE_SHOOT_FRC_UP)
//		{
//			Shoot_Frequency_UP = BARREL_HEAT_THIRD_LINE_SHOOT_FRC_UP;
//			RecoverFlag = TRUE;
//		}
//	}
//	else if(Barrel_Heat_Cur_UP >= BARREL_HEAT_SECOND_LINE_UP)
//	{
//		if(Shoot_Frequency_UP >= BARREL_HEAT_SECOND_LINE_SHOOT_FRC_UP)
//		{
//			Shoot_Frequency_UP = BARREL_HEAT_SECOND_LINE_SHOOT_FRC_UP;
//		}
//		RecoverFlag = FALSE;
//	}
//	else if(Barrel_Heat_Cur_UP < BARREL_HEAT_SECOND_LINE_UP)
//	{
//		if(Shoot_Frequency_UP > BARREL_HEAT_FIRST_LINE_SHOOT_FRC_UP)
//		{
//			Shoot_Frequency_UP = BARREL_HEAT_FIRST_LINE_SHOOT_FRC_UP;
//		}
//		RecoverFlag = FALSE;
//	}
//	else
//	{
//		RecoverFlag = FALSE;
//	}
//}





///** --------------------------------------------------------------------------
//  * @brief  下云台弹频决策（敌方距离）
//			
//  * @note	优先级：当前热量>敌方距离
// -------------------------------------------------------------------------- **/
//void DNShootSpeedDistanceStraegy(void)
//{
//	if(Enemy_Distance_DN < DEFEND_THIRD_LINE_DISTANCE_DN)
//	{
//		Shoot_Frequency_DN = DEFEND_THIRD_LINE_SHOOT_FRC_DN;
//	}
//	else if(Enemy_Distance_DN < DEFEND_SECOND_LINE_DISTANCE_DN)
//	{
//		Shoot_Frequency_DN = DEFEND_SECOND_LINE_SHOOT_FRC_DN;
//	}
//	else if(Enemy_Distance_DN < DEFEND_FIRST_LINE_DISTANCE_DN)
//	{
//		Shoot_Frequency_DN = DEFEND_FIRST_LINE_SHOOT_FRC_DN;
//	}
//}





///** --------------------------------------------------------------------------
//  * @brief  下云台弹频决策（当前枪管热量）
//			
//  * @note	优先级：当前热量>敌方距离
//			设置射速上限
// -------------------------------------------------------------------------- **/
//void DNShootSpeedHeatStraegy(void)
//{
//	static bool RecoverFlag = FALSE;
//	
//	if((Barrel_Heat_Cur_DN >= BARREL_HEAT_THIRD_LINE_DN) || (RecoverFlag && (Barrel_Heat_Cur_DN >= BARREL_HEAT_FIRST_LINE_DN)))
//	{
//		if(Shoot_Frequency_DN >= BARREL_HEAT_THIRD_LINE_SHOOT_FRC_DN)
//		{
//			Shoot_Frequency_DN = BARREL_HEAT_THIRD_LINE_SHOOT_FRC_DN;
//			RecoverFlag = TRUE;
//		}
//	}
//	else if(Barrel_Heat_Cur_DN >= BARREL_HEAT_SECOND_LINE_DN)
//	{
//		if(Shoot_Frequency_DN >= BARREL_HEAT_SECOND_LINE_SHOOT_FRC_DN)
//		{
//			Shoot_Frequency_DN = BARREL_HEAT_SECOND_LINE_SHOOT_FRC_DN;
//		}
//		RecoverFlag = FALSE;
//	}
//	else if(Barrel_Heat_Cur_DN < BARREL_HEAT_SECOND_LINE_DN)
//	{
//		if(Shoot_Frequency_DN >= BARREL_HEAT_FIRST_LINE_SHOOT_FRC_DN)
//		{
//			Shoot_Frequency_DN = BARREL_HEAT_FIRST_LINE_SHOOT_FRC_DN;
//		}
//		RecoverFlag = FALSE;
//	}
//	else
//	{
//		RecoverFlag = FALSE;
//	}
//}





///** --------------------------------------------------------------------------
//  * @brief  危险状态判断
//			
//  * @note	装甲板受到伤害即置为危险状态，20s内没受到伤害置为安全状态
// -------------------------------------------------------------------------- **/
//void DangerStateJudge(void)													
//{
//	static u32 DangerTime = 0;
//	
//	if(RobotHurt.hurt_type == 0)
//	{
//		rush.DangerFlag = TRUE;
//		DangerTime = systemMonitor.SysTickTime;
//		RobotHurt.hurt_type = 9;
//	}
//	if(systemMonitor.SysTickTime - DangerTime > 20000)
//	{
//		rush.DangerFlag = FALSE;
//	}
//}




///** --------------------------------------------------------------------------
//  * @brief  敌方距离判断
//			
//  * @note	有1s的时间看到敌方处于某防线则认为敌方处于该防线
// -------------------------------------------------------------------------- **/
//void EnemyDistanceJudge(void)														
//{
//	static u32 UPSniperTime  = 0;
//	static u32 DNSniperTime  = 0;
//	static u32 UPEnemyDis1 = 0; 
//	static u32 UPEnemyDis2 = 0; 
//	static u32 UPEnemyDis3 = 0; 
//	static u32 DNEnemyDis1 = 0; 
//	static u32 DNEnemyDis2 = 0; 
//	static u32 DNEnemyDis3 = 0; 
//	
//	
//	/**下云台**/
//	/*敌方在防线中现身时间累积*/
//	if(stFlag.DN_SniperFlag)
//	{
//		if(Enemy_Distance_DN < DEFEND_THIRD_LINE_DISTANCE_DN)
//		{
//			DNEnemyDis3++;
//		}
//		else if(Enemy_Distance_DN < DEFEND_SECOND_LINE_DISTANCE_DN)
//		{
//			DNEnemyDis2++;
//		}
//		else if(Enemy_Distance_DN < DEFEND_FIRST_LINE_DISTANCE_DN)
//		{
//			DNEnemyDis1++;
//		}
//		DNSniperTime	= systemMonitor.SysTickTime;	
//	}

//	/*敌方所处防线判断*/
//	if(DNEnemyDis3 > 1000)
//	{
//		DNEnemyPosition = 3;											// 敌方处于第三道防线中
//		DNEnemyDis3 = 0;
//		DNEnemyDis2 = 0;
//		DNEnemyDis1 = 0;
//	}
//	else if(DNEnemyDis2 > 1000)
//	{
//		DNEnemyPosition = 2;
//		DNEnemyDis3 = 0;
//		DNEnemyDis2 = 0;
//		DNEnemyDis1 = 0;		
//	}
//	else if(DNEnemyDis1 > 1000)
//	{
//		DNEnemyPosition = 1;
//		DNEnemyDis3 = 0;
//		DNEnemyDis2 = 0;
//		DNEnemyDis1 = 0;		
//	}
//	
//	/*5s中未识别到敌方认为敌方未进入防线*/
//	if(systemMonitor.SysTickTime - DNSniperTime > 5000)
//	{
//		DNEnemyPosition = 0;
//		DNEnemyDis3 = 0;
//		DNEnemyDis2 = 0;
//		DNEnemyDis1 = 0;	
//	}
//	
//	
//	
//	/**上云台**/
//	/*敌方在防线中现身时间累积*/
//	if(stFlag.UP_SniperFlag)
//	{
//		if(Enemy_Distance_UP < DEFEND_THIRD_LINE_DISTANCE_UP)
//		{
//			UPEnemyDis3++;
//		}
//		else if(Enemy_Distance_UP < DEFEND_SECOND_LINE_DISTANCE_UP)
//		{
//			UPEnemyDis2++;
//		}
//		else if(Enemy_Distance_UP < DEFEND_FIRST_LINE_DISTANCE_UP)
//		{
//			UPEnemyDis1++;
//		}
//		UPSniperTime	= systemMonitor.SysTickTime;	
//	}

//	/*敌方所处防线判断*/
//	if(UPEnemyDis3 > 1000)
//	{
//		UPEnemyPosition = 3;											// 敌方处于第三道防线中
//		UPEnemyDis3 = 0;
//		UPEnemyDis2 = 0;
//		UPEnemyDis1 = 0;
//	}
//	else if(UPEnemyDis2 > 1000)
//	{
//		UPEnemyPosition = 2;
//		UPEnemyDis3 = 0;
//		UPEnemyDis2 = 0;
//		UPEnemyDis1 = 0;		
//	}
//	else if(UPEnemyDis1 > 1000)
//	{
//		UPEnemyPosition = 1;
//		UPEnemyDis3 = 0;
//		UPEnemyDis2 = 0;
//		UPEnemyDis1 = 0;		
//	}

//	
//	/*5s中未识别到敌方认为敌方未进入防线*/
//	if(systemMonitor.SysTickTime - UPSniperTime > 5000)
//	{
//		UPEnemyPosition = 0;
//		UPEnemyDis3 = 0;
//		UPEnemyDis2 = 0;
//		UPEnemyDis1 = 0;
//	}
//}




///** --------------------------------------------------------------------------
//  * @brief  平衡步兵号码牌判断
//			
//  * @note	根据开局的血量进行判断
// -------------------------------------------------------------------------- **/
//void RecognizeBanlanceSoldier(void)
//{
//	static bool FirstIn			= TRUE;
//	static u32	RecognizeTimes 	= 5000;
//	static bool	Num3Reconize 	= FALSE;
//	static bool	Num4Reconize 	= FALSE;
//	static bool	Num5Reconize 	= FALSE;

//	if(FirstIn && GameStatus.game_progress == 4)
//	{
//		FirstIn = FALSE;
//	}
//	
//	if(!FirstIn && RecognizeTimes > 0)
//	{
//		if(RobotIDMessage == Blue_7_Sentry)
//		{
//			if(GameRobotHP.red_3_robot_HP == 300 && !Num3Reconize)
//			{
//				BalancesoldierNumber += 3;
//				Num3Reconize = TRUE;
//			}
//			if(GameRobotHP.red_4_robot_HP == 300 && !Num4Reconize)
//			{
//				BalancesoldierNumber += 4; 
//				Num4Reconize = TRUE;
//			}
//			if(GameRobotHP.red_5_robot_HP == 300 && !Num5Reconize)
//			{
//				BalancesoldierNumber += 5; 
//				Num5Reconize = TRUE;
//			}
//		}
//		else
//		{
//			if(GameRobotHP.blue_3_robot_HP == 300 && !Num3Reconize)
//			{
//				BalancesoldierNumber += 3;
//				Num3Reconize = TRUE;
//			}
//			if(GameRobotHP.blue_3_robot_HP == 300 && !Num4Reconize)
//			{
//				BalancesoldierNumber += 4; 
//				Num4Reconize = TRUE;
//			}
//			if(GameRobotHP.blue_3_robot_HP == 300 && !Num5Reconize)
//			{
//				BalancesoldierNumber += 5; 
//				Num5Reconize = TRUE;
//			}
//		}
//		
//		RecognizeTimes--;
//	}
//	
//	if(RecognizeTimes == 0)
//	{
//		BanlanReconizeFinshFlag = TRUE;
//	}
//}

