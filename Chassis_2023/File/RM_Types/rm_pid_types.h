//
// Created by Zhou Zhicheng on 2021/2/25.
//

#ifndef HAL_CHASSIS_RM_PID_TYPES_H
#define HAL_CHASSIS_RM_PID_TYPES_H


#include "rm_redefine_types.h"


#define QuickPID(Kp,Ki,Kd,UMax,UpMax,EiMax,UdMax,EMin) {.m_fpKp=Kp, .m_fpKi=Ki, .m_fpKd=Kd,	\
		.m_fpUMax=UMax, .m_fpUpMax=UpMax, .m_fpEiMax=EiMax, .m_fpUdMax=UdMax, .m_fpEMin=EMin}
#define		QuickTD(r,h,T)	{.m_r = r, .m_h = h, .m_T = T}
// PID参数结构体
typedef struct
{
	float m_fpDes;		//控制变量目标值
	float m_fpFB;		//控制变量反馈值

	float m_fpKp;		//比例系数
	float m_fpKi;		//积分系数
	float m_fpKd;		//微分系数

	float m_fpUp;		//比例输出
	float m_fpUi;		//积分输出
	float m_fpUd;		//微分输出

	float m_fpE;		//本次偏差
	float m_fpPreE;		//上次偏差
	float m_fpSumE;		//总偏差

	float m_fpU;		//本次PID运算结果

	float m_fpUMax;		//PID运算后输出最大值及遇限削弱时的上限值
	float m_fpUpMax;	//比例项输出上限
	float m_fpEiMax;	//积分上限
	float m_fpUdMax;	//微分项输出上限

	float m_fpEMin;
} ST_PID;

#define		QuickTD(r,h,T)	{.m_r = r, .m_h = h, .m_T = T}
// TD（微分跟踪器）参数结构体
typedef struct
{
	float m_x1;		//位置
	float m_x2;		//速度
	float m_x;		//位移
	float m_r;		//TD阻尼因子（决定跟踪速度，r越大跟得越快，如果追求快速响应，微分预测的滤波效果会变差）
	float m_h;		//TD滤波因子（算法式中的h0，h0越大微分预测的滤波效果越好）
	float m_T;		//TD积分步长（h为步长,h越小滤波效果越好，这个值应该与采样周期一致）
	float m_aim;	//目标位置
}ST_TD;
typedef struct
{
	float preout;
	float out;
	float in;
	float off_freq;
	float samp_tim;
}ST_LPF;
// SMC（滑模控制算法）参数结构体
typedef struct
{
	float m_fpDes;	//控制变量目标值
	float m_fpFB;	//控制变量反馈值
	float m_fpE;	//本次偏差
	float m_fpU;	//本次运算结果
	float m_fpUMax;	//输出上限

	//SMC参数
	float m_b;
	float m_eps;
	float m_gain;
	float m_dead;
	ST_TD m_TD;
}ST_SMC;
/*机器人底盘运动速度结构体*/
typedef struct
{
	float fpVx; //X方向速度（单位：mm/s）
	float fpVy; //Y方向速度（单位：mm/s）
	float fpW;  //角速度（逆时针为正，单位rad/s）
} ST_VELT;

typedef struct
{
	float m_preP;
	float m_rawP;
	float m_Q;
	float m_R;
	float m_K;
	float m_input;
	float m_output;
	
}ST_KMF;

#endif //HAL_CHASSIS_RM_PID_TYPES_H
