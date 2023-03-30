#ifndef GIMBAL_TASK_H
#define GIMBAL_TASK_H
#include "rm_redefine_types.h"

#define FixGapTime 420

typedef struct ENEMYPOS
{
	float FR;	/*�ҵ�Զ��*/
	float HR;	/*�ߵ��Ҳ�*/
	float HM;	/*�ߵ�����*/
	float HL;	/*�ߵ����*/
	float FL;	/*���Զ��*/
	float CL;	/*�������*/
	float CM;	/*�м����*/
	float CR;	/*�ҵ�����*/
	float BL;	/*�����*/
	float BR;	/*���ҵ�*/
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

