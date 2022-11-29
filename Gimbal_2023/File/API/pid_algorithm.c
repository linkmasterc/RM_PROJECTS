#include "pid_algorithm.h"
#include "bmi088_driver.h"
ST_LPF Ud_LPF={0,0,0,10,0.001};
/*******************************************
函数名：value_ctrl
功能	：将cur的值限制在max_value内
备注	：
*******************************************/
float value_ctrl(float cur,float max_value,float min_value)
{
	if(cur>max_value)
		cur=max_value;
	else if(cur<min_value)
		cur=min_value;
	else
		cur=cur;
	return cur;
}
void Ramp_Signal(float* input,float desoutput,float step)
{
	if(*input-desoutput<0)
		(*input)+=step;
	else if(*input-desoutput==0)
		(*input)=desoutput;
	else if(*input-desoutput>0)
		(*input)-=step;
}
/*******************************************
函数名：Sgn
功能	：对x进行符号检验
备注	：(大于0输出1，等于0输出0，小于0输出-1)
*******************************************/
int Sgn(float x)
{
	if(x>0)
		return 1;
	else if(x==0)
		return 0;
	else 
		return -1;
}

/*********************************************** 滤波器 ***********************************************************/

/** --------------------------------------------------------------------------
	* @brief  	均值平滑滤波器
  
			
	* @note		
 -------------------------------------------------------------------------- **/
float DataSmoothFilter(float* Box, u8 BoxSize, float RawData)
{
	float DataAllAdd = 0;

	for(int i = 1;i<BoxSize;i++)					// 数据移位
	{
		Box[BoxSize-i] = Box[BoxSize-i-1];
	}
	
	Box[0] = RawData;								// 添加新数据
	
	for(int i = 0;i<BoxSize;i++)					// 数据均值
	{
		DataAllAdd += Box[i];
		if(i == BoxSize-1)
		{
			return (DataAllAdd / BoxSize);
		}
	}
}

/*******************************************
函数名：PID_Control
作用	：PID控制算法
备注	：
*******************************************/
void PID_Control(ST_PID* PID1)
{
	u8 uck = 1;
	
	PID1->fpE=PID1->fpDes-PID1->fpFB;				//获得与目标的差值

	if(fabs(PID1->fpE) < PID1->fpEMin)
	{
		PID1->fpSumE += PID1->fpE;	//计算偏差累积
	}			
	
	if(fabs(PID1->fpSumE) > PID1->fpUiMax)			//偏差过大时，不进行积分项计算
	{
		uck = 0;
		PID1->fpSumE=0;
	}
	else
		uck=1;
	PID1->fpUp=PID1->fpKp*PID1->fpE;
	
	PID1->fpUi=PID1->fpKi*PID1->fpSumE*uck;
	
	PID1->fpUd=PID1->fpKd*(PID1->fpE-PID1->fpPreE);
	PID1->fpUd=value_ctrl(PID1->fpUd,PID1->fpUdMax,-PID1->fpUdMax);
	PID1->fpU=PID1->fpUp+PID1->fpUi+PID1->fpUd;
	
	PID1->fpU=value_ctrl(PID1->fpU,PID1->fpUMax,-PID1->fpUMax);	
	PID1->fpPreE=PID1->fpE;
}

/*******************************************
函数名：PID_LPF_Control
作用	：PID低通滤波控制算法
备注	：
*******************************************/
void PID_LPF_Control(ST_PID* PID1,ST_LPF* UD_LPF)
{
	u8 uck = 1;
	
	PID1->fpE=PID1->fpDes-PID1->fpFB;				//获得与目标的差值

	if(fabs(PID1->fpE) < PID1->fpEMin)
	{
		PID1->fpSumE += PID1->fpE;	//计算偏差累积
	}			
	
	if(fabs(PID1->fpSumE) > PID1->fpUiMax)			//偏差过大时，不进行积分项计算
	{
		uck = 0;
		PID1->fpSumE=0;
	}
	else
		uck=1;
	PID1->fpUp=PID1->fpKp*PID1->fpE;
	
	PID1->fpUi=PID1->fpKi*PID1->fpSumE*uck;
	
	PID1->fpUd=PID1->fpKd*(PID1->fpE-PID1->fpPreE);
	UD_LPF->in=PID1->fpUd;
	LpFilter(UD_LPF);
	PID1->fpUd=UD_LPF->out;
	PID1->fpUd=value_ctrl(PID1->fpUd,PID1->fpUdMax,-PID1->fpUdMax);
	PID1->fpU=PID1->fpUp+PID1->fpUi+PID1->fpUd;
	
	PID1->fpU=value_ctrl(PID1->fpU,PID1->fpUMax,-PID1->fpUMax);	
	PID1->fpPreE=PID1->fpE;
}

/**
* @brief 	通过TD跟踪微分器计算速度x2和位置x1
* @param 	pStTD	TD数据结构体 
* @comment	m_aim为真实期望位置，x1为过渡期望位置，x2为过渡期望速度
 */
void CalTD(ST_TD *pStTD)
{
	float d,d0,y,a0,a=0,fhan;
	pStTD->m_x = pStTD->m_x1 - pStTD->m_aim;
	d = pStTD->m_r * pStTD->m_h;
	d0 = pStTD->m_h * d;
	y = pStTD->m_x + pStTD->m_h * pStTD->m_x2;
	a0 = sqrt(d * d + 8 * pStTD->m_r * fabs(y));

	if(fabs(y) > d0)
		a = pStTD->m_x2 + (a0 - d) * Sgn(y) / 2;
	else
		a = pStTD->m_x2 + y / pStTD->m_h;

	if(fabs(a) > d)
		fhan = -1 * pStTD->m_r * Sgn(a);
	else
		fhan = -1 * pStTD->m_r * a / d;

	pStTD->m_x1 += pStTD->m_T * pStTD->m_x2;
	pStTD->m_x2 += pStTD->m_T * fhan;
}




