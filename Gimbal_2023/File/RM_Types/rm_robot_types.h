#ifndef __RM_ROBOT_H__
#define __RM_ROBOT_H__

/*---------���ݰ�id��-------*/
#define ID_1 0x01
#define ID_2 0x02
#define ID_3 0x03
#define ID_4 0x04
#define ID_5 0x05
#define ID_6 0x06
/*--------------------------*/

/*----------ͳһ֡ͷ֡β----------*/
#define FRAME_HEAD1 0x55
#define FRAME_HEAD2 0x22
#define FRAME_TAIL1 0x01
#define FRAME_TAIL2 0xAA


#define spi_read_write_byte 	spi1_read_write_byte

#define  GYRO_CS_H    GPIO_SetBits(Open_SPIx_CSB2_PORT, Open_SPIx_CSB2_PIN)//GPIOB   GPIO_Pin_12
#define  GYRO_CS_L    GPIO_ResetBits(Open_SPIx_CSB2_PORT, Open_SPIx_CSB2_PIN)//GPIOB   GPIO_Pin_12

#define  ACC_CS_H    GPIO_SetBits(Open_SPIx_CSB1_PORT, Open_SPIx_CSB1_PIN)//GPIOC  GPIO_Pin_8
#define  ACC_CS_L    GPIO_ResetBits(Open_SPIx_CSB1_PORT, Open_SPIx_CSB1_PIN)//GPIOC  GPIO_Pin_8


#define FAIL          0
#define SUCCESS       1

#define NOP			__asm{NOP;}

#define OFF_FREQ			1500

	
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
#ifndef PI
#define PI               3.14159265358979f
#endif
#include "stm32f4xx.h"
#include "parameter_config.h"


//ϵͳ֡�ʼ����
typedef struct
{
	u32 SysTickTime;
	
  u16 LedTask_cnt;
	u16 McMotor_cnt;
	u16 CylinderTask_cnt;	
	u16 PlatformTask_cnt;
	u16 ServoTask_cnt;
	u16 CommunicateTask_cnt;
	u16 ImuUpdateTask_cnt;
	u16 SensorDataTask_cnt;
	u16 ShootTask_cnt;
	
	u16 LedTask_fps;
	u16 McMotor_fps;
	u16 CylinderTask_fps;
	u16 PlatformTask_fps;
	u16 ServoTask_fps;
	u16 CommunicateTask_fps;
	u16 ImuUpdateTask_fps;
	u16 SensorDataTask_fps;
	u16 ShootTask_fps;
}SYSTEM_MONITOR;

//pid�㷨
typedef struct{
	float fpDes; //���Ʊ���Ŀ��ֵ
	float fpFB;	//���Ʊ�������ֵ
	
	float fpKp;	//����ϵ��
	float fpKi;
	float fpKd;
	
	float fpUp;	//�������
	float fpUi;	//�������
	float fpUd;
	
	float fpE;		//����ƫ��
	float fpPreE;	//�ϴ�ƫ��
	float fpSumE;	//��ƫ��
	float fpU;		//����pid������
	
	float fpUMax;	//PID������������ֵ
	float fpUpMax;//������������ֵ
	float fpUiMax;
	float fpUdMax;
	
	float fpEMin;	//��������
	
	float fpDt;	//��������
}ST_PID;

//���̴���
typedef struct{
	s32 RawValue;  //���α���ֵ
	s32 PreValue;		//�ϴα���ֵ
	s32 diff;			//��ֵ
	s32 Number;		//����
	s32 Sumvalue;//�ۼ�ֵ


}ST_ENCODER;


//�������
typedef struct{
	s32 sum_value;
	s16 speed;
	s16 encoder;

}motor_parameter;


//��������״̬����״̬����S
typedef enum{
	D_S0=0,							//��ʼ״̬S0
	D_Head1=1,					//֡ͷ��һλ
	D_Head2=2,					//֡ͷ�ڶ�λ
	D_DATA=3,						//����λ
	D_CRC_NUM=4,				//CRCУ����λ
	D_Tail1=5,					//֡β��һλ
	D_Tail2=6						//֡β�ڶ�λ
		
}ST_DATA;


//�������ݰ��Ľṹ��
typedef __packed struct{
	u8 head[2];						//֡ͷ
	float PitchAngle;
	float PitchSpeed;
	float YawAngle;
	float YawSpeed;
	u8 CRC_Bit;

}USART2_DMA;

enum {X = 0, Y = 1, Z = 2, VEC_XYZ};
enum {A_X = 0, A_Y, A_Z, G_X, G_Y, G_Z, TEM, MPU_ITEMS};
typedef enum {INIT = 0, NORMAL = 1, CALIBRATION = 2}IMU_MODE;

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
	s16 Acc_Original[VEC_XYZ];	//???????????
	s16 Gyro_Original[VEC_XYZ];	//?????????
	s16 Tempreature;
	float Tempreature_C;
	
	float Acc[VEC_XYZ];       //?????
	float Acc_cmss[VEC_XYZ];  //??��??cm/s2
	float Gyro[VEC_XYZ];      //?????
	float Gyro_deg[VEC_XYZ];  //??��????/s
	float Gyro_rad[VEC_XYZ];  //??��?????/s
	
	float acc_coe[VEC_XYZ];   //????????????
	float acc_offset[VEC_XYZ];//?????????
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

typedef union
{
	u8 ch[9];
	u8 head[2];						//֡ͷ
	u8 UP_STATUS;
	u8 DOWN_STATUS;
	float Pitch_Pos_Des;
	u8 CRC_Bit;
	
}SEND_DMA_UNION;

typedef struct{
	float RawValue;
	float PreValue;
	float diff;
	float deg;
	float SumValue;

}ST_ANGLE;

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
#endif











