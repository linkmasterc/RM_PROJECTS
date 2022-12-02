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

#define USART2_RX_BUF_LEN       	500
#define USART2_TX_BUF_LEN					250

#define USART3_RX_BUF_LEN       	16
#define USART3_TX_BUF_LEN				17

#define UART4_RX_BUF_LEN       	
#define UART4_TX_BUF_LEN				

#define UA6RxDMAbuf_LEN    60
#define UA6RxMBbuf_LEN     (VISION_RECIEVE_DATA_LEN_DN + 5)

#define QuickEncoder(Ratio, Number) {.fpGearRatio=Ratio, .siNumber=Number}

/** ״̬���ṹ�� */
typedef struct {
	USART_TypeDef* USARTx;					//���ں�
	DMA_Stream_TypeDef* DMAy_Streamx;		//DMA������
	u8 *pMailbox;                           //���������ַ
	__IO u8 *pDMAbuf;                       //DMA�ڴ����ַ
	u16 MBLen;                              //�����С
	u16 DMALen;                             //DMA��������С
	u16 rxConter;                           //��ǰ����֡������ַ+1
	u16 rxBufferPtr;                        //��ǰ֡��ʼ��ַ
	u16 rxSize;                             //��ǰ֡��С
} USART_RX_TypeDef;

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
		u8 TriggerState;			//1
		u8 CRC8_Bit;					//CRCУ��λ
		
	}Receive;
	
	__packed struct
	{
		u8 head[2];
		u8 Flag_Run;					//��̨ʹ�ܱ�־λ
		u8 Flag_Shoot;				
		s16 Friction_Send_Des;//Ħ����ת������ֵ
		s16 Shooter_Send_Des;		//��������
		float PitAngleDes;		//���ֲ�ʱ�����ƿص�pitch�Ƕ�����
		float ShootFreq;			//ǹ�ܴ�Ƶ��
		u8 CRC8_Bit;
		
	}Send;
	
}ST_IMU;

/** �Ӿ����ݽ��սṹ�� */
#define VISION_RECIEVE_DATA_NUM_UP		11
#define VISION_RECIEVE_DATA_LEN_UP		(VISION_RECIEVE_DATA_NUM_UP * 4 + 1)
#define VISION_RECIEVE_DATA_NUM_DN		11
#define VISION_RECIEVE_DATA_LEN_DN		(VISION_RECIEVE_DATA_NUM_DN * 4 + 1)
typedef __packed struct
{
	u8 Recieve_ID;
	float Recieve_Data1;	//Pitch
	float Recieve_Data2;	//Yaw
	float Recieve_Data3;	//Distance
	float Recieve_Data4;	//�������PITCH
	float Recieve_Data5;	//�������YAW
	float Recieve_Data6;	//�˲�PITCH
	float Recieve_Data7;	//�˲�YAW	
	float Recieve_Data8;	//ȫ����ҰPITCH
	float Recieve_Data9;	//ȫ����ҰYAW
	float Recieve_Data10;	//ȫ����ҰȨ��
	float Recieve_Data11;	//ȫ����ҰȨ��
} ST_VISION_DATA_UP;
typedef __packed struct
{
	u8 Recieve_ID;
	float Recieve_Data1;	//Pitch
	float Recieve_Data2;	//Yaw
	float Recieve_Data3;	//Distance
	float Recieve_Data4;	//�������PITCH
	float Recieve_Data5;	//�������YAW
	float Recieve_Data6;	//�˲�PITCH
	float Recieve_Data7;	//�˲�YAW	
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



/** �Ӿ����ݷ��ͽṹ�� */
#define VISION_SEND_DATA_NUM_UP	8
#define VISION_SEND_DATA_LEN_UP	(VISION_SEND_DATA_NUM_UP * 4 + 6)
#define VISION_SEND_DATA_NUM_DN	11
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
} ST_DATA_UP_V;          //���Ӿ��������ݽṹ��

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
	float Send_Data8;	//ȫ����ҰPitch
	float Send_Data9;	//ȫ����ҰYaw
	float Send_Data10;	//ȫ����ҰȨ��
	float Send_Data11;	//ȫ����Ұ����


	u8 Tail1;
	u8 Tail2;
} ST_DATA_DN_V;          //���Ӿ��������ݽṹ��

typedef union
{
	ST_DATA_UP_V My_Data;
	UCHAR8 Vision_Send_Data_Buf[VISION_SEND_DATA_LEN_UP];
} VISION_SEND_DATA_UP;		//���Ӿ����ʹ��������

typedef union
{
	ST_DATA_DN_V My_Data;
	UCHAR8 Vision_Send_Data_Buf[VISION_SEND_DATA_LEN_DN];
} VISION_SEND_DATA_DN;		//���Ӿ����ʹ��������
#endif




