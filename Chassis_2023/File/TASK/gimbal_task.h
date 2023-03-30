#ifndef GIMBAL_TASK_H
#define GIMBAL_TASK_H
#include "rm_redefine_types.h"

#define FixGapTime 420

typedef struct ENEMYPOS
{
	float FR;	/*右道远点*/
	float HR;	/*高地右侧*/
	float HM;	/*高地正中*/
	float HL;	/*高地左侧*/
	float FL;	/*左道远点*/
	float CL;	/*左道近点*/
	float CM;	/*中间近点*/
	float CR;	/*右道近点*/
	float BL;	/*后方左点*/
	float BR;	/*后方右点*/
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

