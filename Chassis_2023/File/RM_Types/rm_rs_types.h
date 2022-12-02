#ifndef RM_RS_TYPES_H
#define RM_RS_TYPES_H
#include "rm_redefine_types.h"

#define	Red_1_Hero		1
#define	Red_2_Engineer	2
#define Red_3_Standard 	3
#define Red_4_Standard  4
#define Red_5_Standard  5
#define Red_6_Aerial    6
#define Red_7_Sentry    7
#define	Red_9_Radar     9
    
#define	Blue_1_Hero     101
#define Blue_2_Engineer 102
#define Blue_3_Standard 103
#define Blue_4_Standard 104
#define Blue_5_Standard 105
#define Blue_6_Aerial   106
#define Blue_7_Sentry   107
#define	Blue_9_Radar    109


#define GameStatusID    			0x0001
#define GameResultID    			0x0002
#define GameRobotHPID   			0x0003
#define DartStatusID    			0x0004
#define ICRABuffDebuffZoneStatusID  0x0005
#define EventDataID     			0x0101
#define SupplyProjectileActionID    0x0102
#define ApplySupplyProjectoleID		0x0103
#define RefereeWaringID     		0x0104
#define DartRemainingTimeID     	0x0105
#define GameRobotStatusID       	0x0201
#define PowerHeatDataID     		0x0202
#define GameRobotPosID      		0x0203
#define BuffID      				0x0204
#define AerialRobotEnergyID     	0x0205
#define RobotHurtID     			0x0206
#define ShootDataID     			0x0207
#define BulletRemainingID   		0x0208
#define RFIDStatusID        		0x0209
#define DartClientCmdID     		0x020A
#define RobotInteractiveDataID  	0x0301

#define ClientCustomGraphicDeleteID		0x0100
#define ClientCustomGraphicSingleID		0x0101
#define ClientCustomGraphicDoubleID		0x0102
#define ClientCustomGraphicFiveID		0x0103
#define ClientCustomGraphicSevenID		0x0104
#define ClientCustomCharacterID			0x0110

#define Custom_Data_Length_MAX			113

/** 模块离线标志结构体 */
typedef struct
{
	bool RSState;			//裁判系统离线
	bool UPGimbalState;		//上云台云控离线
	bool UPComputerState;	//上云台小电脑离线
	bool DNGimbalState;		//下云台云控离线
	bool DNComputerState;	//下云台小电脑离线
	bool Can1State;			//CAN1模块离线
	bool Can2State;			//CAN2模块离线
	bool CapacitorState; 	//电容模块异常
} ST_ERROR;
/** 全局标志结构体 */
typedef struct
{
	bool EXTI1_IT_ENABLE_FLAG;
	bool EXTI2_IT_ENABLE_FLAG;


	bool RunFlag_Pre;		//前次pitch电机运动标记
	bool RunFlag;			//pitch电机运动标记
	bool ShootFlag;			//拨弹电机运动标记
	bool FrictionFlag;		//摩擦轮启动标志
	bool SniperFlag_Pre;		//前次识别标志
	bool SniperFlag;			//视觉识别物体标记

	bool CS_RunFlag;			//底盘启动标志
	bool CS_RunFlag_Pre;		//前次底盘启动标志
	
	bool PT_UPFlag;				//上云台空弹仓停电保护标志
	bool PT_DNFlag;				//下云台空弹仓停电保护标志
	
	bool ResetFlag;				//复位标志位
	bool InitFlag;				//初始化成功标志位
	
} ST_FLAG;

/** 弹夹测试模式结构体 */
typedef struct
{
	bool BoDan_Flag;			//打弹开关
	bool PreBoDan_Flag;			//前次开关状态

	float Fact_PreShootPos;		//前次拨弹位置
	float Fact_ShootPos;		//当前拨弹位置

	s32 Fact_ShootSumNumber;	//测试累积打弹数
	s32 Fact_ShootNumber;		//测试累积打完弹夹数
	s32 Fact_pShootNumber;		//单次统计打弹数
	s32 Des_pShootNumber;		//允许单次上限打弹数

}ST_ShootTestControl;
#endif
