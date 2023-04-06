#ifndef GIMBAL_TASK_H
#define GIMBAL_TASK_H
#include "rm_redefine_types.h"

typedef struct ENEMYPOS
{
	float FR;	/*右道远点*/	//yaw 40 pit -4
	float HR;	/*高地右侧*/	//yaw 98 pit -6    //高地下面：pit-22
	float HM;	/*高地正中*/
	float HL;	/*高地左侧*/	//-100 -7//-100 -25
	float FL;	/*左道远点*/	//yaw-60 pit -7
	float CL;	/*左道近点*/	//yaw-60 pit -20
	float CM;	/*中间近点*/	//0,0  0,-20
	float CR;	/*右道近点*/	//yaw40 pit -20
	float BL;	/*后方左点*/	//yaw233 pit-13
	float BR;	/*后方右点*/	//yaw133 pit -10
	float BM;  /*后方中点*/  //后中：yaw178 pit-14
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

