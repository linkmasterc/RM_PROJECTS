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

/**@brief �������������ṹ�� */
typedef struct
{
	u32 uiRawValue;     	//���α�������ԭʼֵ
	u32 uiPreRawValue;  	//��һ�α�������ԭʼֵ
	s32 siDiff;         	//����������ԭʼֵ�Ĳ�ֵ
	float fpSpeed;      	//��������������ת�٣���λ��r/min
	float fpGearRatio; 		//������������ٱ�
	s32 siNumber;       	//����������
	float fpSumValue; 	  	//�������ۼ�ֵ
} ST_ENCODER;



/**@brief �����ƿ�ͨѶЭ��ṹ��*/
typedef __packed struct
{
	__packed struct
	{
		u8 head[2];						//֡ͷ1~2
		float BMIYawAngle;		//IMU������õ���yaw��Ƕ�
		float BMIYawSpeed;		//IMU������yaw����ٶ�
		float PitchDesAngle;  //����ʱʹ�õ������Ƕ�
		float BMIPitchAngle;	//IMU������õ���pitch��Ƕ�
		float BMIPitchSpeed;	//IMU������pitch����ٶ�
		u8 TriggerState;			//1
		u8 CRC8_Bit;					//CRCУ��λ
		
	}Send;
	
	__packed struct
	{
		u8 head[2];
		u8 Flag_Run;					//��̨ʹ�ܱ�־λ
		u8 Flag_Shoot;				
		u8 Flag_Sniper;		
		u8 Flag_Reset;						
		s16 Friction_Send_Des;//Ħ����ת������ֵ
		s16 Shooter_Send_Des;		//��������
		s16 Barrel1_Heat;
		s16 Barrel2_Heat;
		float Barrel1_Speed;
		float Barrel2_Speed;
		float PitAngleDes;		//���ֲ�ʱ�����ƿص�pitch�Ƕ�����
		u8 CRC8_Bit;
		
	}Receive;
	
}ST_IMU;


#endif




