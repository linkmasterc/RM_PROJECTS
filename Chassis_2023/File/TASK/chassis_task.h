#ifndef CHASSIS_TASK_H
#define CHASSIS_TASK_H

#include "global_declare.h"
#include "rm_algorithm.h"
//#include "rm_rs_types.h"

#define Volt_mes            ((float)PowerHeatData.chassis_volt/1000.0f)           //电压
#define Current_mes         ((float)PowerHeatData.chassis_current/1000.0f)        //电流
#define PowerBuffer_mes     ((float)PowerHeatData.chassis_power_buffer)           //功率缓冲
#define Power_mes           ((float)PowerHeatData.chassis_power)                  //功率
#define ShooterHeat_mes     ((s32)PowerHeatData.shooter_id1_17mm_cooling_heat)    //枪口热量
#define ShooterHeat_Rate    ((float)GameRobotStatus.shooter_id1_17mm_cooling_rate)//枪口热量冷却
#define ShooterHeat_Limit   ((s32)GameRobotStatus.shooter_id1_17mm_cooling_limit) //枪口热量上限
#define ShooterSpeed_Limit  ((u8)GameRobotStatus.shooter_id1_17mm_speed_limit)    //枪口射速上限
#define HP_Left_mes         ((u16)GameRobotStatus.remain_HP)                      //机器人剩余血量
#define RobotLevel_mes      ((u8)GameRobotStatus.robot_level)                     //机器人等级
#define HurtType_mes        ((u8)RobotHurt.hurt_type)							                 //血量变化类型
#define PelletFreq_mes      ((u8)ShootData.bullet_freq)                            //射频
#define PelletSpeed_mes     ((float)ShootData.bullet_speed)                        //射速
#define MY_ID               ((u8)GameRobotStatus.robot_id)                        //机器人ID
//#define MY_ID               ((u8)GameRobotStatus.robot_id)                      //机器人ID
//#define Bufftype_mes       //记录Buff类型
//#define BuffAddition_mes   //记录Buff加成百分比

/*----------------------------------------------------------------------------------------
                                功率限制变量
----------------------------------------------------------------------------------------*/
typedef enum
{
  Pow_UnLimited,				//不限功率
	Pow_Limited,					//限制功率
	Pow_Stop							//不输出能量
} Enum_CAP_STATE;

extern int Super_CAP_Change_Cnt;
extern u8 Super_CAP_Break_Flag ;
extern u8 Super_CAP_Change_Flag;

extern float PrePower_mes;
extern float nextPowerBuffer_mes;    
extern float my_Watch_Coe;
extern float cur_limit;

extern bool Is_Tank;
extern bool FLAG_CAP_Vol_Warning;
extern bool FLAG_CAP_Vol_Dangerous;
extern bool Cap_Is_Wraning;
extern bool Chassis_Is_Start_Wraning;
extern bool Chassis_Is_Starting;
extern bool Chassis_Is_Climbing_Wraning;
extern bool Chassis_Is_Rushing_Wraning;
extern bool FLAG_CAP_Pow;

extern Enum_CAP_STATE CAP_STATE ;
extern Enum_CAP_STATE CAP_STATE_Pre;

extern ST_PID Pow_ControlPID;
/*----------------------------------------------------------------------------------------
                               底盘速度功率限制优化算法变量
----------------------------------------------------------------------------------------*/
extern float Limit_fpVx;
extern float Limit_fpVy;
extern float Limit_fpW;

extern ST_PID g_stCM1_Limit_SpeedPID;
extern ST_PID g_stCM2_Limit_SpeedPID;
extern ST_PID g_stCM3_Limit_SpeedPID;
extern ST_PID g_stCM4_Limit_SpeedPID;

extern double CM1_Limit_Speed_Coe;
extern double CM2_Limit_Speed_Coe;
extern double CM3_Limit_Speed_Coe;
extern double CM4_Limit_Speed_Coe;

void Power_Limit_algorithm(float limit_cur,int limit_v);

#endif
