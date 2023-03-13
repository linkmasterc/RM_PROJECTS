//
// Created by Zhou Zhicheng on 2021/2/25.
//

#ifndef HAL_CHASSIS_RM_PID_TYPES_H
#define HAL_CHASSIS_RM_PID_TYPES_H


#include "rm_redefine_types.h"


#define QuickPID(Kp,Ki,Kd,UMax,UpMax,EiMax,UdMax,EMin) {.m_fpKp=Kp, .m_fpKi=Ki, .m_fpKd=Kd,	\
		.m_fpUMax=UMax, .m_fpUpMax=UpMax, .m_fpEiMax=EiMax, .m_fpUdMax=UdMax, .m_fpEMin=EMin}
#define		QuickTD(r,h,T)	{.m_r = r, .m_h = h, .m_T = T}
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
typedef struct
{
	float preout;
	float out;
	float in;
	float off_freq;
	float samp_tim;
}ST_LPF;
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
/*�����˵����˶��ٶȽṹ��*/
typedef struct
{
	float fpVx; //X�����ٶȣ���λ��mm/s��
	float fpVy; //Y�����ٶȣ���λ��mm/s��
	float fpW;  //���ٶȣ���ʱ��Ϊ������λrad/s��
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
