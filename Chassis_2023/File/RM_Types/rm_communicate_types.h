#ifndef RM_COMMUNICATE_TYPES_H
#define RM_COMMUNICATE_TYPES_H

#include "rm_redefine_types.h"
#include "stm32f4xx_usart.h"
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

#define USART1_RX_BUF_LEN         18

#define USART2_RX_DMA_BUF_LEN       	500
#define USART2_RX_MB_BUF_LEN					250
#define USART2_TX_DMA_BUF_LEN    200
#define USART2_TX_MB_BUF_LEN     20

#define USART3_RX_BUF_LEN       	24
#define USART3_TX_BUF_LEN				15

#define UART4_RX_BUF_LEN       	
#define UART4_TX_BUF_LEN				

#define UA6RxDMAbuf_LEN    60
#define UA6RxMBbuf_LEN     (VISION_RECIEVE_DATA_LEN_DN + 5)

#define STATE_SEND_DATA_LEN	(STATE_SEND_DATA_NUM * 4 + 4)	 //（基于伏特加串口助手软件协议）

#define QuickEncoder(Ratio, Number) {.fpGearRatio=Ratio, .siNumber=Number}

/** 状态机结构体 */
typedef struct {
	USART_TypeDef* USARTx;					//串口号
	DMA_Stream_TypeDef* DMAy_Streamx;		//DMA数据流
	u8 *pMailbox;                           //接收邮箱地址
	__IO u8 *pDMAbuf;                       //DMA内存基地址
	u16 MBLen;                              //邮箱大小
	u16 DMALen;                             //DMA缓存区大小
	u16 rxConter;                           //当前接收帧结束地址+1
	u16 rxBufferPtr;                        //当前帧起始地址
	u16 rxSize;                             //当前帧大小
} USART_RX_TypeDef;

/**@brief 电机编码器解码结构体 */
typedef struct
{
	float uiRawValue;     	//本次编码器的原始值
	float uiPreRawValue;  	//上一次编码器的原始值
	float siDiff;         	//编码器两次原始值的差值
	float fpSpeed;      	//电机减速器输出轴转速，单位：r/min
	float fpGearRatio; 		//电机减速器减速比
	float siNumber;       	//编码器线数
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
		float PitchDesAngle;
		float BMIPitchAngle;	//IMU经解算得到的pitch轴角度
		float BMIPitchSpeed;	//IMU发出的pitch轴角速度
		u8 TriggerState;			//1
		u8 CRC8_Bit;					//CRC校验位
		
	}Receive;
	
	__packed struct
	{
		u8 head[2];
		u8 Flag_Run;					//云台使能标志位
		u8 Flag_Shoot;				
		u8 Flag_Sniper;		
		u8 Flag_Reset;						
		s16 Friction_Send_Des;//摩擦轮转速期望值
		s16 Shooter_Send_Des;		//弹数期望
		float PitAngleDes;		//在手操时发给云控的pitch角度期望
		u8 CRC8_Bit;
		
	}Send;
	
}ST_IMU;

/** 视觉数据接收结构体 */
#define VISION_RECIEVE_DATA_NUM_UP		11
#define VISION_RECIEVE_DATA_LEN_UP		(VISION_RECIEVE_DATA_NUM_UP * 4 + 1)
#define VISION_RECIEVE_DATA_NUM_DN		8
#define VISION_RECIEVE_DATA_LEN_DN		(VISION_RECIEVE_DATA_NUM_DN * 4 + 1)
typedef __packed struct
{
	u8 Recieve_ID;
	float Recieve_Data1;	//Pitch
	float Recieve_Data2;	//Yaw
	float Recieve_Data3;	//Distance
	float Recieve_Data4;	//相机解算PITCH
	float Recieve_Data5;	//相机解算YAW
	float Recieve_Data6;	//滤波PITCH
	float Recieve_Data7;	//滤波YAW	
	float Recieve_Data8;	//全局视野PITCH
	float Recieve_Data9;	//全局视野YAW
	float Recieve_Data10;	//全局视野权重
	float Recieve_Data11;	//全局视野权重
} ST_VISION_DATA_UP;
typedef __packed struct
{
	u8 Recieve_ID;
	float Recieve_Data1;	//Pitch
	float Recieve_Data2;	//Yaw
	float Recieve_Data3;	//Distance
	float Recieve_Data4;	//相机解算PITCH
	float Recieve_Data5;	//相机解算YAW
	float Recieve_Data6;	//滤波PITCH
	float Recieve_Data7;	//滤波YAW	
	float Recieve_Data8;	//Test1
	float Recieve_Data9;	//Test2
	float Recieve_Data10;	//Test3
	float Recieve_Data11;	//Test4	
} ST_VISION_DATA_DN;
typedef union
{
	ST_VISION_DATA_UP stVisionData;
	u8 pVisionData[VISION_RECIEVE_DATA_LEN_UP];
} UN_VISION_DATA_UP;
typedef union
{
	ST_VISION_DATA_DN stVisionData;
	u8 pVisionData[VISION_RECIEVE_DATA_LEN_DN];
} UN_VISION_DATA_DN;



/** 视觉数据发送结构体 */
#define VISION_SEND_DATA_NUM_UP	8
#define VISION_SEND_DATA_LEN_UP	(VISION_SEND_DATA_NUM_UP * 4 + 6)
#define VISION_SEND_DATA_NUM_DN	8
#define VISION_SEND_DATA_LEN_DN	(VISION_SEND_DATA_NUM_DN * 4 + 6)
typedef struct
{
	u8 Head1;
	u8 Head2;
	u8 ID;
	u8 Length;

	float Send_Data1;   //Pitch
	float Send_Data2;   //Yaw
	float Send_Data3;   //Bullet Speed
	float Send_Data4;   //Enemy HP Detect
	float Send_Data5;   //Chassis Position
	float Send_Data6;   //Chassis Speed
	float Send_Data7;   //Banlance Soldior Number
	float Send_Data8;	//Trigger Times
//	float Send_Data9;	//Vision Anti Fault Sniper

	u8 Tail1;
	u8 Tail2;
} ST_DATA_UP_V;          //向视觉发送数据结构体

typedef struct
{
	u8 Head1;
	u8 Head2;
	u8 ID;
	u8 Length;

	float Send_Data1;   //Pitch
	float Send_Data2;   //Yaw
	float Send_Data3;   //Bullet Speed
	float Send_Data4;   //Enemy HP Detect
	float Send_Data5;   //Chassis Position
	float Send_Data6;   //Chassis Speed
	float Send_Data7;   //Banlance Soldior Number
	float Send_Data8;	//全局视野Pitch
//	float Send_Data9;	//全局视野Yaw
//	float Send_Data10;	//全局视野权重
//	float Send_Data11;	//全局视野号码


	u8 Tail1;
	u8 Tail2;
} ST_DATA_DN_V;          //向视觉发送数据结构体

typedef union
{
	ST_DATA_UP_V My_Data;
	UCHAR8 Vision_Send_Data_Buf[VISION_SEND_DATA_LEN_UP];
} VISION_SEND_DATA_UP;		//向视觉发送打包联合体

typedef union
{
	ST_DATA_DN_V My_Data;
	UCHAR8 Vision_Send_Data_Buf[VISION_SEND_DATA_LEN_DN];
} VISION_SEND_DATA_DN;		//向视觉发送打包联合体

typedef struct
{
    /*接收消息，ID0x400*/
    float CAP_Vol;
    float Pow_In;
    float Pow_Out;
		float Volt_Out;
	  u8 CAP_ERROR;

    /*发送消息，ID0x220*/
    float TxPower;
} CAPACITOR_MSG;
/* 用于串口助手通信看状态变量的波形 */
#define STATE_SEND_DATA_NUM	8

#ifdef MINYOU_PROTOCOL
	#define STATE_SEND_DATA_LEN	(STATE_SEND_DATA_NUM * 4 + 9)	 //（基于名优科创串口助手软件协议）
#elif defined VOFA_PROTOCOL
	#define STATE_SEND_DATA_LEN	(STATE_SEND_DATA_NUM * 4 + 4)	 //（基于伏特加串口助手软件协议）
#endif

typedef __packed struct
{
	u8 Head1;
	u8 Head2;
	u8 Head3;
	u8 Head4;
	u8 Head5;
	u8 Head6;
	u8 Head7;
	u8 Num;


	float Send_Data1; 
	float Send_Data2;
	float Send_Data3;  
	float Send_Data4;   
	float Send_Data5;   
	float Send_Data6;   
	float Send_Data7; 
	float Send_Data8;
	float Send_Data9;  
	float Send_Data10;   
	float Send_Data11;   
	float Send_Data12; 
	float Send_Data13; 
	float Send_Data14;
	float Send_Data15;  
	float Send_Data16;   
 
	
	u8 Tail1;
} ST_UART_DATA_MY;


typedef __packed struct
{
	float Send_Data1; 
	float Send_Data2;
	float Send_Data3;  
	float Send_Data4;   
	float Send_Data5;   
	float Send_Data6;   
	float Send_Data7; 
	float Send_Data8;
//	float Send_Data9;  
//	float Send_Data10;   
//	float Send_Data11;   
//	float Send_Data12; 
//	float Send_Data13; 
//	float Send_Data14;
//	float Send_Data15;  
//	float Send_Data16;   
// 	float Send_Data17;  
//	float Send_Data18;  
// 	float Send_Data19;  
//	float Send_Data20;
	
	u8 Tail1;
	u8 Tail2;
	u8 Tail3;
	u8 Tail4;
} ST_UART_DATA_VOFA;

#endif




