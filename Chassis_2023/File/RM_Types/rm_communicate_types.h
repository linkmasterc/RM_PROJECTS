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
#define GimbalCushioning_Tx			GimbalSendData

#define USART3_RX_BUF_LEN       	16
#define USART3_TX_BUF_LEN				13

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
		float BMIPitchAngle;	//IMU经解算得到的pitch轴角度
		u8 TRIGGER_STATUS;
		u8 CRC8_Bit;					//CRC校验位
		
	}Receive;
	
	__packed struct
	{
		u8 head[2];
		u8 GIMBAL_STATUS;			//云台状态变量 2:关闭	1:手动操作	3:自动操作
		u8 SHOOT_STATUS;			//枪管发射状态 暂定
//		s16 ShooterSendDes;
//		s16 FrictionSendDes;
		float PitAngleDes;		//在手操时发给云控的pitch角度期望
		float ShootFreq;			//枪管打蛋频率
		u8 CRC8_Bit;
		
	}Send;
	
}ST_IMU;


#endif




