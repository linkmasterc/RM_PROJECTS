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
		float BMIPitchAngle;	//IMU������õ���pitch��Ƕ�
		u8 TRIGGER_STATUS;
		u8 CRC8_Bit;					//CRCУ��λ
		
	}Receive;
	
	__packed struct
	{
		u8 head[2];
		u8 GIMBAL_STATUS;			//��̨״̬���� 2:�ر�	1:�ֶ�����	3:�Զ�����
		u8 SHOOT_STATUS;			//ǹ�ܷ���״̬ �ݶ�
//		s16 ShooterSendDes;
//		s16 FrictionSendDes;
		float PitAngleDes;		//���ֲ�ʱ�����ƿص�pitch�Ƕ�����
		float ShootFreq;			//ǹ�ܴ�Ƶ��
		u8 CRC8_Bit;
		
	}Send;
	
}ST_IMU;


#endif




