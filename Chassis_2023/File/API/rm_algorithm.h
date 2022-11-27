#ifndef RM_ALGORITHM_H
#define RM_ALGORITHM_H

#include "math.h"
#include "os.h"
#include "rm_pid_types.h"

#define PI 3.14159265358979f

/*********************************************** 通用数学函数 ***********************************************************/
float LinearInterpolation(const float *fpSampleX, const float *fpSampleY, float fpX, uint8_t size);
float Clip(float fpValue, float fpMin, float fpMax);

/*********************************************** PID基础算法以及优化算法 *************************************************/
void CalPID(ST_PID* pStPID);
void CalISeparatedPID(ST_PID *pStPID);
void CalIResistedPID(ST_PID* pStPID);
void CalIWeakenPID(ST_PID* pStPID);

/*********************************************** SMC(滑模控制)算法 ******************************************************/
void CalTD(ST_TD *pStTD);
void CalSMC(ST_SMC *pStSMC);

/*********************************************** 斜坡信号输出 ***********************************************************/
void S16RampSignal(s16 *Output, s16 DesValue, s16 Step);
void U32RampSignal(u32 *Output, u32 DesValue, u32 Step);
float FPRampSignal(float Output, float DesValue, float Step);
/*********************************************** 滤波器 ***********************************************************/
float DataSmoothFilter(float* Box, u8 BoxSize, float RawData);
/***********************************************离散信号***********************************************************/
float NoiseSimulator(float in, bool state);
#endif
