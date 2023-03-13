#include "rm_algorithm.h"

static s32 Sgn(float fpNumber);
static float SMC_SatFunc(float in, float d);

/*********************************************** 通用数学函数 ***********************************************************/

/**
 * @brief 线性插值函数，要求样本X为正序，从小到大
 * @param 	fpSampleX		插值样本X
 * 			fpSampleY		插值样本Y	
 * 			fpX				插值点
 *			size			插值样本大小
 * @return 削波后的数据
 */
float LinearInterpolation(const float *fpSampleX, const float *fpSampleY, float fpX, uint8_t size)
{
	if(size == 0)
	{	return 0;}
	
	for(uint8_t i = 0;i < size;i++)
	{
		if(fpX < fpSampleX[i])
		{
			if(i > 0)
			{	
				if(fpSampleY[i] == fpSampleY[i-1])
				{	return fpSampleY[i];}
				else
				{	return (fpSampleY[i] - fpSampleY[i-1]) / (fpSampleX[i] - fpSampleX[i-1]) * fpX + fpSampleY[i-1];}
			}
			else
			{	return fpSampleY[i];}
		}
	}
	return fpSampleY[size - 1];
}
/**
 * @brief 削波函数，去除超出最大值与最小值之间的值，代之以最大或最小值
 * @param 	fpValue		输入数据
 * 			fpMin		削波最小值
 * 			fpMax		削波最大值
 * @return 削波后的数据
 */
float Clip(float fpValue, float fpMin, float fpMax)
{
	float ret;
	if(fpValue < fpMin)
	{
		ret = fpMin;
	}
	else if(fpValue > fpMax)
	{
		ret = fpMax;
	}
	else
	{
		ret = fpValue;
	}
	return ret;
}

/**
 * @brief	符号函数
 * @param 	fpNumber	原数值
 * @return 	s32			0 or 1 or -1
 */
s32 Sgn(float fpNumber)
{
	if(fpNumber > 0)
	{
		return 1;
	}
	else if(fpNumber < 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

/*********************************************** PID基础算法以及优化算法 *************************************************/
/**
 * @brief	普通位置式PID算法
 * @param 	pStPID 	PID数据结构体
 */
void CalPID(ST_PID* pStPID)
{
	pStPID->m_fpE = pStPID->m_fpDes - pStPID->m_fpFB;	//计算当前误差
	if(fabs(pStPID->m_fpE) <= pStPID->m_fpEMin)	//偏差死区限制
	{
		pStPID->m_fpE = 0;
	}

	pStPID->m_fpSumE += pStPID->m_fpE;	//计算偏差累积

	//位置PID计算公式
	pStPID->m_fpU = pStPID->m_fpKp * pStPID->m_fpE
				  + pStPID->m_fpKi * pStPID->m_fpSumE
				  + pStPID->m_fpKd * (pStPID->m_fpE - pStPID->m_fpPreE);

	pStPID->m_fpPreE = pStPID->m_fpE;	//保存本次偏差

	//PID运算输出限幅
	pStPID->m_fpU = Clip(pStPID->m_fpU, -pStPID->m_fpUMax, pStPID->m_fpUMax);

}

/**
 * @brief	积分分离式PID算法
 * @details	可减小启动、停止或大幅度增减时较大偏差对积分项的积累，从而避免出现较大的超调及振荡现象。
 * @param 	pStPID	PID数据结构体
 */
void CalISeparatedPID(ST_PID *pStPID)
{
	u8 uck = 1;

	pStPID->m_fpE = pStPID->m_fpDes - pStPID->m_fpFB;	//计算当前偏差
	if(fabs(pStPID->m_fpE) <= pStPID->m_fpEMin)			//偏差死区限制
	{
		pStPID->m_fpE = 0;
	}

	pStPID->m_fpSumE += pStPID->m_fpE;
	if(fabs(pStPID->m_fpSumE) > pStPID->m_fpEiMax)			//偏差过大时，不进行积分项计算
	{
		uck = 0;
	}

	//位置式PID计算公式
	pStPID->m_fpU = pStPID->m_fpKp * pStPID->m_fpE
				  + pStPID->m_fpKi * pStPID->m_fpSumE * uck
				  + pStPID->m_fpKd * (pStPID->m_fpE - pStPID->m_fpPreE);

	pStPID->m_fpPreE = pStPID->m_fpE;	//保存本次偏差

	//PID运算输出限幅
	pStPID->m_fpU = Clip(pStPID->m_fpU, -pStPID->m_fpUMax, pStPID->m_fpUMax);
}

/**
 * @brief	抗积分饱和PID算法
 * @details	能消除系统往一个方向运动会产生较大积分误差，会在几个周期内产生振荡或超调
 * @param 	pStPID	PID数据结构体
 */
void CalIResistedPID(ST_PID* pStPID)
{
	pStPID->m_fpE = pStPID->m_fpDes - pStPID->m_fpFB;	//计算当前误差
	pStPID->m_fpSumE += pStPID->m_fpE;	//计算偏差累积

	pStPID->m_fpSumE = Clip(pStPID->m_fpSumE, -pStPID->m_fpEiMax, pStPID->m_fpEiMax);
	pStPID->m_fpUi = pStPID->m_fpKi * pStPID->m_fpSumE;
	pStPID->m_fpUp = Clip(pStPID->m_fpKp * pStPID->m_fpE, -pStPID->m_fpUpMax, pStPID->m_fpUpMax);
	pStPID->m_fpUd = Clip(pStPID->m_fpKd * (pStPID->m_fpE - pStPID->m_fpPreE), -pStPID->m_fpUdMax, pStPID->m_fpUdMax);

	//若偏差在死区之内，则清零积分累积项
	if(fabs(pStPID->m_fpE) < pStPID->m_fpEMin)	//判断是否满足积分饱和条件
	{
		pStPID->m_fpSumE = 0;	//消除偏差累积
	}

	//位置PID计算公式
	pStPID->m_fpU = pStPID->m_fpUp + pStPID->m_fpUi + pStPID->m_fpUd;

	pStPID->m_fpPreE = pStPID->m_fpE;	//保存本次偏差

	//PID运算输出限幅
	pStPID->m_fpU = Clip(pStPID->m_fpU, -pStPID->m_fpUMax, pStPID->m_fpUMax);
}





/**
 * @brief	遇限削弱积分PID改进算法

 * @param 	pStPID	PID数据结构体
			位置式PID
 */
void CalIWeakenPID(ST_PID* pStPID)
{
	pStPID->m_fpE = pStPID->m_fpDes - pStPID->m_fpFB;																		// 计算当前偏差

	if(fabs(pStPID->m_fpE) < pStPID->m_fpEMin)																				// 微小偏差不计入积分
	{
		pStPID->m_fpSumE += pStPID->m_fpE;																					// 计算偏差累积
	}

	pStPID->m_fpSumE = Clip(pStPID->m_fpSumE, -pStPID->m_fpEiMax, pStPID->m_fpEiMax);										// 累积偏差（积分）限幅
	pStPID->m_fpUi = pStPID->m_fpKi * pStPID->m_fpSumE;																		// 得到积分器输出
	pStPID->m_fpUp = Clip(pStPID->m_fpKp * pStPID->m_fpE, -pStPID->m_fpUpMax, pStPID->m_fpUpMax);							// 得到比例器输出
	pStPID->m_fpUd = Clip(pStPID->m_fpKd * (pStPID->m_fpE - pStPID->m_fpPreE), -pStPID->m_fpUdMax, pStPID->m_fpUdMax);		// 得到微分器输出

	
	pStPID->m_fpU = pStPID->m_fpUp + pStPID->m_fpUi + pStPID->m_fpUd;														// 比例、积分、微分累加得到输出

	pStPID->m_fpPreE = pStPID->m_fpE;																						// 保存本次偏差
	
	pStPID->m_fpU = Clip(pStPID->m_fpU, -pStPID->m_fpUMax, pStPID->m_fpUMax);												// PID运算输出限幅
}






/*********************************************** SMC(滑模控制)算法 ******************************************************/
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

/**
 * @brief	滑模控制所用函数
 * @param 	in 	？
 * @param 	d 	？
 * @return ？
 */
float SMC_SatFunc(float in, float d)
{
	if(fabs(in) >= d)
		return Sgn(in);
	else
		return in / d;
}

/**
 * @brief	通过TD跟踪微分器计算速度x2和位置x1
 * @param 	pStSMC	滑模控制数据结构体
 */
void CalSMC(ST_SMC *pStSMC)
{
	pStSMC->m_TD.m_aim = pStSMC->m_fpDes;
	CalTD(&pStSMC->m_TD);
	pStSMC->m_fpE = pStSMC->m_TD.m_x1 - pStSMC->m_fpFB;
	pStSMC->m_fpU = 1 / pStSMC->m_b * (pStSMC->m_TD.m_x2
								   + pStSMC->m_eps * SMC_SatFunc(pStSMC->m_fpE, pStSMC->m_dead)
								   + pStSMC->m_gain * pStSMC->m_fpE);
	pStSMC->m_fpU = Clip(pStSMC->m_fpU, -pStSMC->m_fpUMax, pStSMC->m_fpUMax);
}

/*********************************************** 斜坡信号输出 ***********************************************************/
/**
 * @brief	将s16类型的数据按直线，以固定步长，向目标值靠近
 * @param 	Output		输入数据
 * @param 	DesValue	目标值
 * @param 	Step		步长
 */
void S16RampSignal(s16 *Output, s16 DesValue, s16 Step)
{
	u8 type;
	if(*Output < DesValue) type = 0;
	else if(*Output > DesValue) type = 1;
	else type = 2;

	if(type == 0)
	{
		if(*Output >= DesValue) *Output = DesValue;
		else
		{
			*Output += Step;
			if(*Output >= DesValue) *Output = DesValue;
		}
	}
	else if(type == 1)
	{
		if(*Output <= DesValue) *Output = DesValue;
		else
		{
			*Output -= Step;
			if(*Output <= DesValue) *Output = DesValue;
		}
	}
	else
		return;
}

/**
 * @brief	将u32类型的数据按直线，以固定步长，向目标值靠近
 * @param 	Output		输入数据
 * @param 	DesValue	目标值
 * @param 	Step		步长
 */
void U32RampSignal(u32 *Output, u32 DesValue, u32 Step)
{
	u8 type;
	if(*Output < DesValue) type = 0;
	else if(*Output > DesValue) type = 1;
	else type =2;

	if(type == 0)
	{
		if(*Output >= DesValue) *Output = DesValue;
		else
		{
			*Output += Step;
			if(*Output >= DesValue) *Output = DesValue;
		}
	}
	else if(type == 1)
	{
		if(*Output <= DesValue) *Output = DesValue;
		else
		{
			*Output -= Step;
			if(*Output <= DesValue) *Output = DesValue;
		}
	}
	else
		return;
}

/**
 * @brief	将float类型的数据按直线，以固定步长，向目标值靠近
 * @param 	Output		输入数据
 * @param 	DesValue	目标值
 * @param 	Step		步长
 */
void FPRampSignal(float* Output, float DesValue, float Step)
{
	u8 type;
	if(*Output < DesValue) type = 0;
	else if(*Output > DesValue) type = 1;
	else type =2;

	if(type == 0)
	{
		if(*Output >= DesValue) *Output = DesValue;
		else
		{
			*Output += Step;
			if(*Output >= DesValue) *Output = DesValue;
		}
	}
	else if(type == 1)
	{
		if(*Output <= DesValue) *Output = DesValue;
		else
		{
			*Output -= Step;
			if(*Output <= DesValue) *Output = DesValue;
		}
	}
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
	}
	return (DataAllAdd / BoxSize);
}

 /** --------------------------------------------------------------------------
  * @brief  离散信号模拟器
  
  * @retval None
  
  * @param	None
			
  * @note	暂未使用
 -------------------------------------------------------------------------- **/
float NoiseSimulator(float in, bool state)
{
	static uint8_t 	delayTick = 0;
	static float	delayAngle;
	if(state)
	{
		if(delayTick--)
		{	return delayAngle;}
		else
		{
			delayTick	= TIM5->CNT % 15;
			delayAngle	= in;
		}
		return delayAngle;
	}
	else
		return in;
}

 /** --------------------------------------------------------------------------
  * @brief  一维卡尔曼滤波器
  
  * @retval None
  
  * @param	None
			
  * @note	
 -------------------------------------------------------------------------- **/
void KalmanFilter(ST_KMF* pStKMF)
{
	pStKMF->m_rawP=pStKMF->m_preP+pStKMF->m_Q;
	pStKMF->m_K=pStKMF->m_rawP/(pStKMF->m_rawP+pStKMF->m_R);
	pStKMF->m_output=pStKMF->m_output+pStKMF->m_K*(pStKMF->m_input-pStKMF->m_output);
	pStKMF->m_preP=(1-pStKMF->m_K)*pStKMF->m_rawP;
}

