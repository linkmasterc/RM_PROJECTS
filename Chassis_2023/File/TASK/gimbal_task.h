#ifndef GIMBAL_TASK_H
#define GIMBAL_TASK_H
#include "rm_redefine_types.h"

typedef struct ENEMYPOS
{
	float FR;	/*�ҵ�Զ��*/	//yaw 40 pit -4
	float HR;	/*�ߵ��Ҳ�*/	//yaw 98 pit -6    //�ߵ����棺pit-22
	float HM;	/*�ߵ�����*/
	float HL;	/*�ߵ����*/	//-100 -7//-100 -25
	float FL;	/*���Զ��*/	//yaw-60 pit -7
	float CL;	/*�������*/	//yaw-60 pit -20
	float CM;	/*�м����*/	//0,0  0,-20
	float CR;	/*�ҵ�����*/	//yaw40 pit -20
	float BL;	/*�����*/	//yaw233 pit-13
	float BR;	/*���ҵ�*/	//yaw133 pit -10
	float BM;  /*���е�*/  //���У�yaw178 pit-14
}enemypos;

typedef struct FIXED_PATROL_POS
{
	enemypos PitchSite1;
	enemypos PitchSite2;
	enemypos PitchSite3;

	enemypos YawSite1;
	enemypos YawSite2;
	enemypos YawSite3;

}FixedPos;

extern u8 EnemyPos;

void GimbalRCMode(void);
void FixedPosScan(void);
void GimbalFollowMove(void);
void GimbalAutoMode(void);
void YawPID(void);
void GimbalModeChoose(void);
#endif

