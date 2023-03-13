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
	s16 Acc_Original[VEC_XYZ];	//���ٶ�ԭʼ����
	s16 Gyro_Original[VEC_XYZ];	//�Ƕ�ԭʼ����
	s16 Tempreature;
	float Tempreature_C;
	
	float Acc[VEC_XYZ];       //ģ����
	float Acc_cmss[VEC_XYZ];  //��λ��cm/s2
	float Gyro[VEC_XYZ];      //ģ����
	float Gyro_deg[VEC_XYZ];  //��λ�ǽǶ�/s
	float Gyro_rad[VEC_XYZ];  //��λ�ǻ���/s
	
	float acc_coe[VEC_XYZ];   //���ٶȼƿ̶�����
	float acc_offset[VEC_XYZ];//���ٶȼ���ƫ
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

		
// PID�����ṹ��
typedef struct
{
	float m_fpDes;		//���Ʊ���Ŀ��ֵ
	float m_fpFB;		//���Ʊ�������ֵ

	float m_fpKp;		//����ϵ��
	float m_fpKi;		//����ϵ��
	float m_fpKd;		//΢��ϵ��

	float m_fpUp;		//�������
	float m_fpUi;		//�������
	float m_fpUd;		//΢�����

	float m_fpE;		//����ƫ��
	float m_fpPreE;		//�ϴ�ƫ��
	float m_fpSumE;		//��ƫ��

	float m_fpU;		//����PID������

	float m_fpUMax;		//PID�����������ֵ����������ʱ������ֵ
	float m_fpUpMax;	//�������������
	float m_fpEiMax;	//��������
	float m_fpUdMax;	//΢�����������

	float m_fpEMin;
} ST_PID;

#define		QuickTD(r,h,T)	{.m_r = r, .m_h = h, .m_T = T}
// TD��΢�ָ������������ṹ��
typedef struct
{
	float m_x1;		//λ��
	float m_x2;		//�ٶ�
	float m_x;		//λ��
	float m_r;		//TD�������ӣ����������ٶȣ�rԽ�����Խ�죬���׷�������Ӧ��΢��Ԥ����˲�Ч�����
	float m_h;		//TD�˲����ӣ��㷨ʽ�е�h0��h0Խ��΢��Ԥ����˲�Ч��Խ�ã�
	float m_T;		//TD���ֲ�����hΪ����,hԽС�˲�Ч��Խ�ã����ֵӦ�����������һ�£�
	float m_aim;	//Ŀ��λ��
}ST_TD;

// SMC����ģ�����㷨�������ṹ��
typedef struct
{
	float m_fpDes;	//���Ʊ���Ŀ��ֵ
	float m_fpFB;	//���Ʊ�������ֵ
	float m_fpE;	//����ƫ��
	float m_fpU;	//����������
	float m_fpUMax;	//�������

	//SMC����
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
