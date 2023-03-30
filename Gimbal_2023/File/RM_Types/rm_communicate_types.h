#ifndef RM_COMMUNICATE_TYPES_H
#define RM_COMMUNICATE_TYPES_H

#include "rm_redefine_types.h"

#define UART5_RX_STREAM        	DMA1_Stream0
#define USART3_RX_STREAM        DMA1_Stream1
#define UART4_RX_STREAM         DMA1_Stream2
#define USART3_TX_STREAM        DMA1_Stream3
#define UART4_TX_STREAM         DMA1_Stream4
#define USART2_RX_STREAM        DMA1_Stream5
#define USART2_TX_STREAM        DMA1_Stream6
#define UART5_TX_STREAM       	DMA1_Stream7
#define UART8_TX_STREAM         DMA1_Stream0
#define UART7_TX_STREAM         DMA1_Stream1
#define UART7_RX_STREAM         DMA1_Stream3
#define UART8_RX_STREAM         DMA1_Stream6
#define USART6_RX_STREAM        DMA2_Stream1
#define USART1_RX_STREAM        DMA2_Stream2
#define USART6_TX_STREAM        DMA2_Stream6
#define USART1_TX_STREAM        DMA2_Stream7


#define GimbalCushioning_Rx     USART3_Cushioning_Rx 	
#define ChassisCushioning_Tx			ChassisData

#define USART3_RX_BUF_LEN       	27
#define USART3_TX_BUF_LEN				24

#define QuickEncoder(Ratio, Number) {.fpGearRatio=Ratio, .siNumber=Number}

/**@brief 电机编码器解码结构体 */
typedef struct
{
	u32 uiRawValue;     	//本次编码器的原始值
	u32 uiPreRawValue;  	//上一次编码器的原始值
	s32 siDiff;         	//编码器两次原始值的差值
	float fpSpeed;      	//电机减速器输出轴转速，单位：r/min
	float fpGearRatio; 		//电机减速器减速比
	s32 siNumber;       	//编码器线数
	float fpSumValue; 	  	//编码器累加值
} ST_ENCODER;



/**@brief 主控云控通讯协议结构体*/
typedef __packed struct
{
	__packed struct
	{
		u8 head[2];						//帧头1~2
		float BMIYawAngle;		//IMU经解算得到的yaw轴角度
		float BMIYawSpeed;		//IMU发出的yaw轴角速度
		float PitchDesAngle;  //测试时使用的期望角度
		float BMIPitchAngle;	//IMU经解算得到的pitch轴角度
		float BMIPitchSpeed;	//IMU发出的pitch轴角速度
		u8 TriggerState;			//1
		u8 CRC8_Bit;					//CRC校验位
		
	}Send;
	
	__packed struct
	{
		u8 head[2];
		u8 Flag_Run;					//云台使能标志位
		u8 Flag_Shoot;				
		u8 Flag_Sniper;		
		u8 Flag_Reset;						
		s16 Friction_Send_Des;//摩擦轮转速期望值
		s16 Shooter_Send_Des;		//弹数期望
		s16 Barrel1_Heat;
		s16 Barrel2_Heat;
		float Barrel1_Speed;
		float Barrel2_Speed;
		float PitAngleDes;		//在手操时发给云控的pitch角度期望
		u8 CRC8_Bit;
		
	}Receive;
	
}ST_IMU;


#endif




