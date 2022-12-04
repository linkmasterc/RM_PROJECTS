#ifndef RM_RS_TYPES_H
#define RM_RS_TYPES_H
#include "rm_redefine_types.h"
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

	bool UP_RunFlag_Pre;		//前次pitch电机运动标记
	bool UP_RunFlag;			//pitch电机运动标记
	bool UP_ShootFlag;			//拨弹电机运动标记
	bool UP_FrictionFlag;		//摩擦轮启动
	bool UP_SniperFlag_Pre;		//前次识别标志
	bool UP_SniperFlag;			//视觉识别物体标记

	bool DN_RunFlag_Pre;		//前次pitch电机运动标记
	bool DN_RunFlag;			//pitch电机运动标记
	bool DN_ShootFlag;			//拨弹电机运动标记
	bool DN_FrictionFlag;		//摩擦轮启动标志
	bool DN_SniperFlag_Pre;		//前次识别标志
	bool DN_SniperFlag;			//视觉识别物体标记

	bool CS_RunFlag;			//底盘启动标志
	bool CS_RunFlag_Pre;		//前次底盘启动标志
	
	bool PT_UPFlag;				//上云台空弹仓停电保护标志
	bool PT_DNFlag;				//下云台空弹仓停电保护标志
	
	bool ResetFlag;				//复位标志位
	bool InitFlag;				//初始化成功标志位
	
} ST_FLAG;
#endif
