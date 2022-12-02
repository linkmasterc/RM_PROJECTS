#ifndef RM_RS_TYPES_H
#define RM_RS_TYPES_H
#include "rm_redefine_types.h"

#define	Red_1_Hero		1
#define	Red_2_Engineer	2
#define Red_3_Standard 	3
#define Red_4_Standard  4
#define Red_5_Standard  5
#define Red_6_Aerial    6
#define Red_7_Sentry    7
#define	Red_9_Radar     9
    
#define	Blue_1_Hero     101
#define Blue_2_Engineer 102
#define Blue_3_Standard 103
#define Blue_4_Standard 104
#define Blue_5_Standard 105
#define Blue_6_Aerial   106
#define Blue_7_Sentry   107
#define	Blue_9_Radar    109


#define GameStatusID    			0x0001
#define GameResultID    			0x0002
#define GameRobotHPID   			0x0003
#define DartStatusID    			0x0004
#define ICRABuffDebuffZoneStatusID  0x0005
#define EventDataID     			0x0101
#define SupplyProjectileActionID    0x0102
#define ApplySupplyProjectoleID		0x0103
#define RefereeWaringID     		0x0104
#define DartRemainingTimeID     	0x0105
#define GameRobotStatusID       	0x0201
#define PowerHeatDataID     		0x0202
#define GameRobotPosID      		0x0203
#define BuffID      				0x0204
#define AerialRobotEnergyID     	0x0205
#define RobotHurtID     			0x0206
#define ShootDataID     			0x0207
#define BulletRemainingID   		0x0208
#define RFIDStatusID        		0x0209
#define DartClientCmdID     		0x020A
#define RobotInteractiveDataID  	0x0301

#define ClientCustomGraphicDeleteID		0x0100
#define ClientCustomGraphicSingleID		0x0101
#define ClientCustomGraphicDoubleID		0x0102
#define ClientCustomGraphicFiveID		0x0103
#define ClientCustomGraphicSevenID		0x0104
#define ClientCustomCharacterID			0x0110

#define Custom_Data_Length_MAX			113

/** ģ�����߱�־�ṹ�� */
typedef struct
{
	bool RSState;			//����ϵͳ����
	bool UPGimbalState;		//����̨�ƿ�����
	bool UPComputerState;	//����̨С��������
	bool DNGimbalState;		//����̨�ƿ�����
	bool DNComputerState;	//����̨С��������
	bool Can1State;			//CAN1ģ������
	bool Can2State;			//CAN2ģ������
	bool CapacitorState; 	//����ģ���쳣
} ST_ERROR;
/** ȫ�ֱ�־�ṹ�� */
typedef struct
{
	bool EXTI1_IT_ENABLE_FLAG;
	bool EXTI2_IT_ENABLE_FLAG;


	bool RunFlag_Pre;		//ǰ��pitch����˶����
	bool RunFlag;			//pitch����˶����
	bool ShootFlag;			//��������˶����
	bool FrictionFlag;		//Ħ����������־
	bool SniperFlag_Pre;		//ǰ��ʶ���־
	bool SniperFlag;			//�Ӿ�ʶ��������

	bool CS_RunFlag;			//����������־
	bool CS_RunFlag_Pre;		//ǰ�ε���������־
	
	bool PT_UPFlag;				//����̨�յ���ͣ�籣����־
	bool PT_DNFlag;				//����̨�յ���ͣ�籣����־
	
	bool ResetFlag;				//��λ��־λ
	bool InitFlag;				//��ʼ���ɹ���־λ
	
} ST_FLAG;

/** ���в���ģʽ�ṹ�� */
typedef struct
{
	bool BoDan_Flag;			//�򵯿���
	bool PreBoDan_Flag;			//ǰ�ο���״̬

	float Fact_PreShootPos;		//ǰ�β���λ��
	float Fact_ShootPos;		//��ǰ����λ��

	s32 Fact_ShootSumNumber;	//�����ۻ�����
	s32 Fact_ShootNumber;		//�����ۻ����굯����
	s32 Fact_pShootNumber;		//����ͳ�ƴ���
	s32 Des_pShootNumber;		//���������޴���

}ST_ShootTestControl;
#endif
