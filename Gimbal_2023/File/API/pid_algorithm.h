#ifndef __PID_ALGORITHM_H
#define __PID_ALGORITHM_H

#include "main.h"
#include "math.h"
float value_ctrl(float cur,float max_value,float min_value);
void Ramp_Signal(float* input,float desoutput,float step);
int Sgn(float x);
void PID_Control(ST_PID* PID1);
void PID_LPF_Control(ST_PID* PID1,ST_LPF* UD_LPF);
void CalTD(ST_TD *pStTD);
#endif

