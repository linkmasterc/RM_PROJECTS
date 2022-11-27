#ifndef RM_RS_TYPES_H
#define RM_RS_TYPES_H
#include "rm_redefine_types.h"
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

	bool UP_RunFlag_Pre;		//ǰ��pitch����˶����
	bool UP_RunFlag;			//pitch����˶����
	bool UP_ShootFlag;			//��������˶����
	bool UP_FrictionFlag;		//Ħ��������
	bool UP_SniperFlag_Pre;		//ǰ��ʶ���־
	bool UP_SniperFlag;			//�Ӿ�ʶ��������

	bool DN_RunFlag_Pre;		//ǰ��pitch����˶����
	bool DN_RunFlag;			//pitch����˶����
	bool DN_ShootFlag;			//��������˶����
	bool DN_FrictionFlag;		//Ħ����������־
	bool DN_SniperFlag_Pre;		//ǰ��ʶ���־
	bool DN_SniperFlag;			//�Ӿ�ʶ��������

	bool CS_RunFlag;			//����������־
	bool CS_RunFlag_Pre;		//ǰ�ε���������־
	
	bool PT_UPFlag;				//����̨�յ���ͣ�籣����־
	bool PT_DNFlag;				//����̨�յ���ͣ�籣����־
	
	bool ResetFlag;				//��λ��־λ
	bool InitFlag;				//��ʼ���ɹ���־λ
	
} ST_FLAG;
#endif
