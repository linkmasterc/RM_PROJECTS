#ifndef RM_COMMUNICATE_TYPES_H
#define RM_COMMUNICATE_TYPES_H

#include "rm_redefine_types.h"
#include "rm_rs_types.h"
#include "stm32f4xx_usart.h"
#define USART1_TX_STREAM        DMA2_Stream7
#define USART1_RX_STREAM        DMA2_Stream2

#define USART2_TX_STREAM        DMA1_Stream6
#define USART2_RX_STREAM        DMA1_Stream5

#define USART3_TX_STREAM        DMA1_Stream3
#define USART3_RX_STREAM        DMA1_Stream1

#define UART4_TX_STREAM         DMA1_Stream4
#define UART4_RX_STREAM         DMA1_Stream2

#define UART5_TX_STREAM       	DMA1_Stream7
#define UART5_RX_STREAM        	DMA1_Stream0

#define USART6_TX_STREAM        DMA2_Stream6
#define USART6_RX_STREAM        DMA2_Stream1

#define UART7_TX_STREAM         DMA1_Stream1
#define UART7_RX_STREAM         DMA1_Stream3

#define UART8_TX_STREAM         DMA1_Stream0
#define UART8_RX_STREAM         DMA1_Stream6



#define GimbalCushioning_Rx     USART3_Cushioning_Rx 	
#define GimbalCushioning_Tx			GimbalSendData

#define USART1_RX_BUF_LEN        18

#define USART2_RX_DMA_BUF_LEN    500
#define USART2_RX_MB_BUF_LEN		 250
#define USART2_TX_DMA_BUF_LEN    200
#define USART2_TX_MB_BUF_LEN     20

#define USART3_RX_BUF_LEN        31
#define USART3_TX_BUF_LEN				 31

#define UART4_RX_BUF_LEN       	
#define UART4_TX_BUF_LEN				

#define UART5_RX_BUF_LEN    18
#define UART5_TX_BUF_LEN 		60
#define UA6RxDMAbuf_LEN    60
#define UA6RxMBbuf_LEN     (VISION_RECIEVE_DATA_LEN + 5)

#define STATE_SEND_DATA_LEN	(STATE_SEND_DATA_NUM * 4 + 4)	 //�����ڷ��ؼӴ����������Э�飩

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
	float uiRawValue;     	//���α�������ԭʼֵ
	float uiPreRawValue;  	//��һ�α�������ԭʼֵ
	float siDiff;         	//����������ԭʼֵ�Ĳ�ֵ
	float fpSpeed;      	//��������������ת�٣���λ��r/min
	float fpGearRatio; 		//������������ٱ�
	float siNumber;       	//����������
	float fpSumValue; 	  	//�������ۼ�ֵ
} ST_ENCODER;



/**@brief �����ƿ�ͨѶЭ��ṹ��*/
typedef __packed struct
{
	__packed struct
	{
		u8 head[2];								//֡ͷ1~2
		float BMIYawAngle;				//IMU������õ���yaw��Ƕ�
		float BMIYawSpeed;				//IMU������yaw����ٶ�
		float PitchDesAngle;
		float BMIPitchAngle;			//IMU������õ���pitch��Ƕ�
		float BMIPitchSpeed;			//IMU������pitch����ٶ�
		float SecondYawDesAngle;	//������̨�����Ƕ�
		float SecondYawAngle;			//������̨����Ƕ�
		u8 TriggerState;					//1
		u8 CRC8_Bit;							//CRCУ��λ
		
	}Receive;
	
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
		float FirstYawPosDes;
		float Barrel1_Speed;
		float Barrel2_Speed;
		float PitAngleDes;		//���ֲ�ʱ�����ƿص�pitch�Ƕ�����
		u8 CRC8_Bit;
		
	}Send;
	
}ST_IMU;

/** �Ӿ����ݽ��սṹ�� */
#define VISION_RECIEVE_DATA_NUM		8
#define VISION_RECIEVE_DATA_LEN		(VISION_RECIEVE_DATA_NUM * 4 + 1)
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
} ST_VISION_DATA;
typedef union
{
	ST_VISION_DATA stVisionData;
	u8 pVisionData[VISION_RECIEVE_DATA_LEN];
} UN_VISION_DATA;



/** �Ӿ����ݷ��ͽṹ�� */
#define VISION_SEND_DATA_NUM	4
#define VISION_SEND_DATA_LEN	(VISION_SEND_DATA_NUM * 4 + 6)

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
//	float Send_Data5;   //Chassis Position
//	float Send_Data6;   //Chassis Speed
//	float Send_Data7;   //Banlance Soldior Number
//	float Send_Data8;	//ȫ����ҰPitch
//	float Send_Data9;	//ȫ����ҰYaw
//	float Send_Data10;	//ȫ����ҰȨ��
//	float Send_Data11;	//ȫ����Ұ����


	u8 Tail1;
	u8 Tail2;
} ST_DATA_V;          //���Ӿ��������ݽṹ��



typedef union
{
	ST_DATA_V My_Data;
	UCHAR8 Vision_Send_Data_Buf[VISION_SEND_DATA_LEN];
} VISION_SEND_DATA;		//���Ӿ����ʹ��������

#define NAVIGATION_RECEIVE_DATA_NUM 3
#define NAVIGATION_RECEIVE_DATA_LEN (NAVIGATION_RECEIVE_DATA_NUM*4+1)
typedef __packed struct
{
	u8 Recieve_ID;
	float Receive_Data1;
	float Receive_Data2;
	float Receive_Data3;
}ST_NAVIGATION_RECEIVE_DATA;


typedef union
{
	ST_NAVIGATION_RECEIVE_DATA stNavigationData;
	u8 pNavigationData[NAVIGATION_RECEIVE_DATA_LEN];
}NAVIGATION_RECEIVE_DATA;

//#define DECISION_SEND_DATA_LEN (51)

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_game_status_t RobotGameStatus;
	u8 Tail[2];
}ST_DECISION_SEND_GAME_STATUS_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_game_result_t RobotResult;
	u8 Tail[2];
}ST_DECISION_SEND_RESULT_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_game_robot_HP_t RobotHP;
	u8 Tail[2];
}ST_DECISION_SEND_HP_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_event_data_t RobotEvent;
	u8 Tail[2];
}ST_DECISION_SEND_EVENT_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_supply_projectile_action_t RobotSupply;
	u8 Tail[2];
}ST_DECISION_SEND_SUPPLY_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_referee_warning_t RobotWarning;
	u8 Tail[2];
}ST_DECISION_SEND_Warning_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_dart_remaining_time_t RobotRemainingTime;
	u8 Tail[2];
}ST_DECISION_SEND_REMAINING_TIME_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_game_robot_status_t RobotStatus;
	u8 Tail[2];
}ST_DECISION_SEND_ROBOT_STATUS_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_power_heat_data_t RobotPowerHeat;
	u8 Tail[2];
}ST_DECISION_SEND_POWER_HEAT_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_game_robot_pos_t RobotPos;
	u8 Tail[2];
}ST_DECISION_SEND_POS_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_buff_t RobotBuff;
	u8 Tail[2];
}ST_DECISION_SEND_BUFF_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	aerial_robot_energy_t AerialRobotEnergy;
	u8 Tail[2];
}ST_DECISION_SEND_AERIAL_ROBOT_ENERGY_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_robot_hurt_t RobotHurt;
	u8 Tail[2];
}ST_DECISION_SEND_HURT_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_shoot_data_t RobotShoot;
	u8 Tail[2];
}ST_DECISION_SEND_SHOOT_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_bullet_remaining_t RobotBulletRemaining;
	u8 Tail[2];
}ST_DECISION_SEND_BULLET_REMAINING_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_rfid_status_t RobotRfidStatus;
	u8 Tail[2];
}ST_DECISION_SEND_RFID_SATAUS_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_dart_client_cmd_t RobotDartClient;
	u8 Tail[2];
}ST_DECISION_SEND_DART_CLIENT_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	ext_student_interactive_header_data_t RobotInteractiveHeader;
	u8 Tail[2];
}ST_DECISION_SEND_INTERACTIVE_HEADER_DATA;

typedef __packed struct
{
	u8 Head[2];
	u16 Send_ID;
	u8 Data_Length;
	robot_interactive_data_t RobotInteractiveData;
	u8 Tail[2];
}ST_DECISION_SEND_INTERACTIVE_DATA;



typedef struct
{
    /*������Ϣ��ID0x400*/
    float CAP_Vol;
    float Pow_In;
    float Pow_Out;
		float Volt_Out;
	  u8 CAP_ERROR;

    /*������Ϣ��ID0x220*/
    float TxPower;
} CAPACITOR_MSG;
/* ���ڴ�������ͨ�ſ�״̬�����Ĳ��� */
#define STATE_SEND_DATA_NUM	8
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




