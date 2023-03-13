//
// Created by Zhou Zhicheng on 2021/2/25.
//

#ifndef HAL_CHASSIS_RM_PID_TYPES_H
#define HAL_CHASSIS_RM_PID_TYPES_H


#include "rm_redefine_types.h"

#define QuickPID(Kp,Ki,Kd,UMax,UpMax,EiMax,UdMax,EMin) {.m_fpKp=Kp, .m_fpKi=Ki, .m_fpKd=Kd,	\
		.m_fpUMax=UMax, .m_fpUpMax=UpMax, .m_fpEiMax=EiMax, .m_fpUdMax=UdMax, .m_fpEMin=EMin}

typedef enum {OFF = 0, ON = 1, TWINKLE = 2}LED_MODE;
typedef enum {INIT = 0, NORMAL = 1, CALIBRATION = 2}IMU_MODE;
typedef enum {LOOP = 0, IDENTIFY = 1}CTRL_MODE;

//-----------------------------------------------------------------------------------------//
//-----------------------------------------imu20602----------------------------------------//
//-----------------------------------------------------------------------------------------//

#define Cali_Rol_Coe 1
#define Cali_Pit_Coe 1
#define Cali_Yaw_Coe 1.0021f//1.0021f

enum {X = 0, Y = 1, Z = 2, VEC_XYZ};
enum {A_X = 0, A_Y, A_Z, G_X, G_Y, G_Z, TEM, MPU_ITEMS};

typedef struct
{
	float center_pos_cm[VEC_XYZ];
	float gyro_rad[VEC_XYZ];
	float gyro_rad_old[VEC_XYZ];
	float gyro_rad_acc[VEC_XYZ];
	float linear_acc[VEC_XYZ];
}_center_pos_st;

typedef struct 
{
	u8 acc_CALIBRATE;
	u8 gyr_CALIBRATE;
	u8 acc_z_auto_CALIBRATE;
	u8 temp_CALIBRATE;
	u8 sensor_ok;
	u8 motionless;
	s16 Acc_Original[VEC_XYZ];	//加速度原始数据
	s16 Gyro_Original[VEC_XYZ];	//角度原始数据
	s16 Tempreature;
	float Tempreature_C;
	
	float Acc[VEC_XYZ];       //模拟量
	float Acc_cmss[VEC_XYZ];  //单位是cm/s2
	float Gyro[VEC_XYZ];      //模拟量
	float Gyro_deg[VEC_XYZ];  //单位是角度/s
	float Gyro_rad[VEC_XYZ];  //单位是弧度/s
	
	float acc_coe[VEC_XYZ];   //加速度计刻度因数
	float acc_offset[VEC_XYZ];//加速度计零偏
	float gyro_offset[VEC_XYZ];	
}_sensor_st;

typedef struct
{
	float preout;
	float out;
	float in;
	float off_freq;
	float samp_tim;
}ST_LPF;

typedef struct
{
	float q0;
	float q1;
	float q2;
	float q3;
	
	float gkp;
	float gki;

	float x_vec[VEC_XYZ];
	float y_vec[VEC_XYZ];
	float z_vec[VEC_XYZ];

	float a_acc[VEC_XYZ];
	float gacc_deadzone[VEC_XYZ];
	float gra_acc[VEC_XYZ];
	
	float rol;
	float pit;
	float yaw;
} _imu_st ;

typedef struct
{
    float angle_180;
    float angle_180_pre;
    float angle_inf;
} ST_ANGLE;		

		
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
