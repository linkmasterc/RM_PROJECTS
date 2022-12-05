#ifndef CHASSIS_TASK_H
#define CHASSIS_TASK_H

#include "global_declare.h"
#include "rm_algorithm.h"
//#include "rm_rs_types.h"

#define Volt_mes            ((float)PowerHeatData.chassis_volt/1000.0f)           //��ѹ
#define Current_mes         ((float)PowerHeatData.chassis_current/1000.0f)        //����
#define PowerBuffer_mes     ((float)PowerHeatData.chassis_power_buffer)           //���ʻ���
#define Power_mes           ((float)PowerHeatData.chassis_power)                  //����
#define ShooterHeat_mes     ((s32)PowerHeatData.shooter_id1_17mm_cooling_heat)    //ǹ������
#define ShooterHeat_Rate    ((float)GameRobotStatus.shooter_id1_17mm_cooling_rate)//ǹ��������ȴ
#define ShooterHeat_Limit   ((s32)GameRobotStatus.shooter_id1_17mm_cooling_limit) //ǹ����������
#define ShooterSpeed_Limit  ((u8)GameRobotStatus.shooter_id1_17mm_speed_limit)    //ǹ����������
#define HP_Left_mes         ((u16)GameRobotStatus.remain_HP)                      //������ʣ��Ѫ��
#define RobotLevel_mes      ((u8)GameRobotStatus.robot_level)                     //�����˵ȼ�
#define HurtType_mes        ((u8)RobotHurt.hurt_type)							                 //Ѫ���仯����
#define PelletFreq_mes      ((u8)ShootData.bullet_freq)                            //��Ƶ
#define PelletSpeed_mes     ((float)ShootData.bullet_speed)                        //����
#define MY_ID               ((u8)GameRobotStatus.robot_id)                        //������ID
//#define MY_ID               ((u8)GameRobotStatus.robot_id)                      //������ID
//#define Bufftype_mes       //��¼Buff����
//#define BuffAddition_mes   //��¼Buff�ӳɰٷֱ�

/*----------------------------------------------------------------------------------------
                                �������Ʊ���
----------------------------------------------------------------------------------------*/
typedef enum
{
  Pow_UnLimited,				//���޹���
	Pow_Limited,					//���ƹ���
	Pow_Stop							//���������
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
                               �����ٶȹ��������Ż��㷨����
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
