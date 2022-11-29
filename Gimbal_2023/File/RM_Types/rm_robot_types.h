#ifndef __RM_ROBOT_H__
#define __RM_ROBOT_H__

/*---------数据包id号-------*/
#define ID_1 0x01
#define ID_2 0x02
#define ID_3 0x03
#define ID_4 0x04
#define ID_5 0x05
#define ID_6 0x06
/*--------------------------*/

/*----------统一帧头帧尾----------*/
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

	
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
#ifndef PI
#define PI               3.14159265358979f
#endif
#include "stm32f4xx.h"
#include "parameter_config.h"


//系统帧率检测器
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

//pid算法
typedef struct{
	float fpDes; //控制变量目标值
	float fpFB;	//控制变量反馈值
	
	float fpKp;	//比例系数
	float fpKi;
	float fpKd;
	
	float fpUp;	//比例输出
	float fpUi;	//积分输出
	float fpUd;
	
	float fpE;		//本次偏差
	float fpPreE;	//上次偏差
	float fpSumE;	//总偏差
	float fpU;		//本次pid运算结果
	
	float fpUMax;	//PID运算后输出上限值
	float fpUpMax;//比例项输出最大值
	float fpUiMax;
	float fpUdMax;
	
	float fpEMin;	//积分上限
	
	float fpDt;	//控制周期
}ST_PID;

//码盘处理
typedef struct{
	s32 RawValue;  //本次编码值
	s32 PreValue;		//上次编码值
	s32 diff;			//差值
	s32 Number;		//线数
	s32 Sumvalue;//累加值


}ST_ENCODER;


//电机参数
typedef struct{
	s32 sum_value;
	s16 speed;
	s16 encoder;

}motor_parameter;


//接收数据状态机总状态集合S
typedef enum{
	D_S0=0,							//初始状态S0
	D_Head1=1,					//帧头第一位
	D_Head2=2,					//帧头第二位
	D_DATA=3,						//数据位
	D_CRC_NUM=4,				//CRC校验码位
	D_Tail1=5,					//帧尾第一位
	D_Tail2=6						//帧尾第二位
		
}ST_DATA;


//发送数据包的结构体
typedef __packed struct{
	u8 head[2];						//帧头
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
	float Acc_cmss[VEC_XYZ];  //??λ??cm/s2
	float Gyro[VEC_XYZ];      //?????
	float Gyro_deg[VEC_XYZ];  //??λ????/s
	float Gyro_rad[VEC_XYZ];  //??λ?????/s
	
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

typedef union
{
	u8 ch[9];
	u8 head[2];						//帧头
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
#endif











