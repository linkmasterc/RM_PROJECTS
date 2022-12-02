#include "main.h"
#include "shoot_task.h"
#include "global_declare.h"
#include "rm_algorithm.h"
#include "mode_choose_task.h"
#include "ParamConfig.h"



static void 	RCShootBullet(void);											// 遥控射击
static void 	AutoShootBullet(void);											// 自动射击
static void 	GBShoot(int Shoot_f, bool mode);								// 按频率拨弹
static void		RammerDetect(void);												// 子弹装填情况检测
static void 	ShootNumberRecord(bool mode);									// 实际发射子弹数记录
static void 	ShootAreaDetect(void);											// 射击允许区域检测
static void 	UPShooterPID(void);												// 射击用拨盘PID计算

/** --------------------------------------------------------------------------
  * @brief  控制子弹发射
			
  * @note	遥控器通道4下拨开启摩擦轮及拨弹开关上拨关闭
			遥控器通道2、3控制拨弹频率
			
			自动打弹状态下只有跟随误差小于一定值才会进行设计
			自动射击的弹频为9hz
 -------------------------------------------------------------------------- **/
void ShootBullet(void)
{

	/* 遥控打弹 */
	if(ControlMode == 0x01	|| ControlMode == 0x02 )
	{
		RCShootBullet();
	}
		
//	/* 自动打弹 */
//	else if(ControlMode == 0x03 || ControlMode == 0x06 || ControlMode == 0x08 || 
//			ControlMode == 0x09)
//	{	
//		AutoShootBullet();
//	}
	
	
/****************/
	

//	/*射击区域检测*/
//	ShootAreaDetect();

//	/*子弹装填情况检测*/
//	RammerDetect();
	
	/*记录状态*/
	ShootNumberRecord(DNShootMode);																							// 记录下云台实际发射子弹数	
	DNSTC.PreBoDan_Flag = DNSTC.BoDan_Flag;
	Bullet_Des_Pre 	= Bullet_Des;
}

void RCShootBullet()
{
	static u32	FrictionTick 	= 0;																						// 用于摩擦轮延时启动
	static bool BoDanFirstIn			= FALSE;																					

	/*遥控器通道4控制摩擦轮及拨弹开关*/
	if(g_StDbus.stRC.Ch4 - RC_CH_VALUE_OFFSET > 300)						
	{
		if(stFlag.ShootFlag)
		{
			Friction_State 	= 700;																						// 下拨通道4开启摩擦轮
		}

		FrictionTick			= systemMonitor.SysTickTime;
		BoDanFirstIn					= TRUE;
	}
	else if(g_StDbus.stRC.Ch4 - RC_CH_VALUE_OFFSET < -300)																	// 上拨通道4关闭摩擦轮和拨弹
	{
		if(stFlag.ShootFlag)
		{
			Friction_State 	= 500;
			DNSTC.BoDan_Flag 	= FALSE;
		}	
	}
	
	
	/*延时打开拨弹开关*/
	if(systemMonitor.SysTickTime - FrictionTick > 0 && (Friction_State == 700 || Friction_State_UP == 700) && BoDanFirstIn)// 开启摩擦轮后延时0s开启拨弹开关
	{
		if(Friction_State == 700)	{DNSTC.BoDan_Flag 		= TRUE;}
		BoDanFirstIn											= FALSE;
	}
	/*遥控器通道2、3控制拨弹频率*/

	if(DNSTC.BoDan_Flag)																									// 下云台拨弹
	{
		if((Friction_State == 700) && stFlag.ShootFlag)												
		{
			if(g_StDbus.stRC.Ch3 - RC_CH_VALUE_OFFSET < -300 )
			{	GBShoot(1, DNShootMode);}
			else if(g_StDbus.stRC.Ch2 - RC_CH_VALUE_OFFSET > 300)
			{	GBShoot(9, DNShootMode);}
			else if(g_StDbus.stRC.Ch3 - RC_CH_VALUE_OFFSET > 300)
			{	GBShoot(13, DNShootMode);}
			else if(g_StDbus.stRC.Ch2 - RC_CH_VALUE_OFFSET < -300)
			{	GBShoot(17, DNShootMode);}
		}
	}
}


/** --------------------------------------------------------------------------
	* @brief  	拨弹函数
  
	* @retval 	None
  
	* @param	Shoot_f		拨弹频率Hz
	 			mode		拨弹模式：	UPShootMode 上云台拨弹
										DNShootMode 下云台拨弹
			
	* @note		不同的拨弹频率对应不同的拨弹时间间隔
 -------------------------------------------------------------------------- **/
void GBShoot(int Shoot_f, bool mode)
{
	static u32 time_Shoot = 0;

	if(mode == DNShootMode)
	{
		if(systemMonitor.SysTickTime - time_Shoot > 1000 / Shoot_f)
		{
			Bullet_Des += 1;															// 下云台拨弹目标数目递增
			time_Shoot= systemMonitor.SysTickTime;
		}
	}
}

/** --------------------------------------------------------------------------
  * @brief  弹夹实际发射数记录
  
  * @retval None
  
  * @param	None

  * @note	计算的是云台在控制状态下实际射击子弹数
			区别于Shoot_Cnt_UP_FB
 -------------------------------------------------------------------------- **/
void ShootNumberRecord(bool mode)
{
	if(mode == DNShootMode)
	{
		if(DNSTC.BoDan_Flag && !DNSTC.PreBoDan_Flag)
		{
			DNSTC.Fact_PreShootPos 	= ShootEncoder.fpSumValue;
			DNSTC.Fact_ShootPos 	= DNSTC.Fact_PreShootPos;
		}
		else if(DNSTC.BoDan_Flag)
		{
			DNSTC.Fact_PreShootPos 	= DNSTC.Fact_ShootPos;
			DNSTC.Fact_ShootPos = ShootEncoder.fpSumValue;
		}
		DNSTC.Fact_pShootNumber 	= (s32)((DNSTC.Fact_ShootPos - DNSTC.Fact_PreShootPos) / (-(BOTTOM_SUPPLY_STEP)) + 0.5f);
		DNSTC.Fact_ShootSumNumber 	+= DNSTC.Fact_pShootNumber;
	}
}



