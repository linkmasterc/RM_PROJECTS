#ifndef RM_SCANNER_TYPES_H
#define RM_SCANNER_TYPES_H

#include "rm_redefine_types.h"

/** ϵͳ�������ṹ�� */
#define DECLARE_MONITOR(name) 		u16 name##_cnt; u16 name##_fps
#define DETECT_MONITOR(name)    	systemMonitor.name##_fps = systemMonitor.name##_cnt; systemMonitor.name##_cnt = 0
#define DETECT_SUB_MONITOR(name)	SubSystemMonitor.name##_fps = SubSystemMonitor.name##_cnt; SubSystemMonitor.name##_cnt = 0;
#define DETECT_RS_MONITOR(name)		RSSystemMonitor.name##_fps = RSSystemMonitor.name##_cnt; RSSystemMonitor.name##_cnt = 0;


/** ����֡��ͳ�ƽṹ�� */
typedef struct
{	
	DECLARE_MONITOR(System);
	
	/** Main Task Monitor */
	DECLARE_MONITOR(SystemMonitorTask);
	DECLARE_MONITOR(ModeChooseTask);
	DECLARE_MONITOR(GimbalTask);
	DECLARE_MONITOR(ShootTask);
	DECLARE_MONITOR(ChassisTask);
	DECLARE_MONITOR(MotorDataSendTask);
	DECLARE_MONITOR(GimbalDataSendTask);
	DECLARE_MONITOR(UPVisionDataSendTask);
	DECLARE_MONITOR(DNVisionDataSendTask);
	DECLARE_MONITOR(CapacitorTask);
	DECLARE_MONITOR(DataSendTask);
	DECLARE_MONITOR(LEDTask);
	DECLARE_MONITOR(LEDStateChangeTask);
	DECLARE_MONITOR(IMUSampleTask);
	DECLARE_MONITOR(IMUUpdateTask);
	DECLARE_MONITOR(TriggerTask);
	
	/** IT Monitor */
	DECLARE_MONITOR(USART1_rx);
	DECLARE_MONITOR(USART2_rx);
	DECLARE_MONITOR(USART3_rx);
	DECLARE_MONITOR(UART4_rx);
	DECLARE_MONITOR(UART5_rx);
	DECLARE_MONITOR(USART6_rx);
	DECLARE_MONITOR(USART1_rx_valid);
	DECLARE_MONITOR(USART2_rx_valid);
	DECLARE_MONITOR(USART3_rx_valid);
	DECLARE_MONITOR(UART4_rx_valid);
	DECLARE_MONITOR(UART5_rx_valid);
	DECLARE_MONITOR(USART6_rx_valid);
	DECLARE_MONITOR(CAN1_rx);
	DECLARE_MONITOR(CAN2_rx);
	
	u32 SysTickTime;
	u16 TaskTotalTime;
	u16 TaskTotalTimeMost;
} SYSTEM_MONITOR;


/** CANͨѶ֡��ͳ�ƽṹ�� */
typedef struct
{
	/** can1֡�ʼ�� */
	DECLARE_MONITOR(CAN1_CapacitorMode_rx);
	DECLARE_MONITOR(CAN1_PowerBoard_rx);
	DECLARE_MONITOR(CAN1_SlaveMotor_rx);
	DECLARE_MONITOR(CAN1_ChassisMotor_rx);
	
	/** can2֡�ʼ�� */
	DECLARE_MONITOR(CAN2_YawMotor_rx);
	DECLARE_MONITOR(CAN2_ChassisZigMotor_rx);
	DECLARE_MONITOR(CAN2_ShootMotor_rx);
} SUB_SYSTEM_MONITOR;


/** ����ϵͳͨѶ֡��ͳ�ƽṹ�� */
typedef struct
{
	DECLARE_MONITOR(GameStatus);
	DECLARE_MONITOR(GameRobotHP);
	DECLARE_MONITOR(DartStatus);
	DECLARE_MONITOR(EventData);
	DECLARE_MONITOR(RefereeWarning);
	DECLARE_MONITOR(DartRemainingTime);
	DECLARE_MONITOR(GameRobotStatus);
	DECLARE_MONITOR(PowerHeatData);
	DECLARE_MONITOR(GameRobotPos);
	DECLARE_MONITOR(Buff);
	DECLARE_MONITOR(RobotHurt);
	DECLARE_MONITOR(ShootData);
	DECLARE_MONITOR(BulletRemaining);
	DECLARE_MONITOR(RobotInteractiveData);
	
} RS_SYSTEM_MONITOR;

typedef struct
{
	bool SysTickFlag;
	
	bool CAN1Flag;
	bool CAN2Flag;
	
	bool USART3Flag;
	
	bool RunFlag;			//����ģʽ
	bool ShootFlag;			//�����������
	bool FrictionFlag;		//Ħ��������
	bool SniperFlag;		//�����Ӿ�����
	bool GyroResetFlag;		//�����������ɹ�
	bool PitchProtectFlag;	//Pitch�������ģʽ
} ST_IQRFLAG;


#endif
