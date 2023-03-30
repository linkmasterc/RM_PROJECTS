#include "tim.h"

/****************************************************************************************************
��������: TIM5_Configuration()
��������: ����ϵͳ��ʱ��ʱ��ο�
�������: ��
���ز���: ��
��   ע:  
****************************************************************************************************/
//TIM5 �ṩʱ��ο�
#define TIM5_Prescaler  84-1
#define TIM5_Period     4294967296-1        //4294967296us=71min��ʱ�乻���ˣ��������ദ��

#define TIM3_Prescaler  840-1
#define TIM3_Period			1000-1

#define TIM4_Prescaler  42000-1
#define TIM4_Period     2000-1
//��ʱ����ʼ������ʱ��5��32λͨ�ö�ʱ����APB1=84MHz��1us��һ���ж�
void TIM5_Configuration(void)
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    
	TIM_TimeBaseStructure.TIM_Prescaler         = TIM5_Prescaler; //����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_Period            = TIM5_Period;  //�趨�Զ�����ֵ
	TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    
  TIM_Cmd(TIM5, ENABLE);
}


void TIM3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	TIM_TimeBaseStructure.TIM_Prescaler=TIM3_Prescaler;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //����ģʽ
	TIM_TimeBaseStructure.TIM_Period=TIM3_Period;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(  TIM3 ,&TIM_TimeBaseStructure);//��ʼ����ʱ��
	
	TIM_Cmd(TIM3 ,ENABLE  );  //ʹ��TIM
}

void TIM4_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructrue;
	TIM_OCInitTypeDef 			TIM_OCStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_TimeBaseStructrue.TIM_Prescaler=TIM4_Prescaler;
	TIM_TimeBaseStructrue.TIM_Period=TIM4_Period;
	TIM_TimeBaseStructrue.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructrue.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructrue);
	TIM_ARRPreloadConfig(TIM4,ENABLE);
	
	TIM_OCStructure.TIM_OCMode=TIM_OCMode_PWM2;
	TIM_OCStructure.TIM_OCPolarity=TIM_OCPolarity_Low;
	TIM_OCStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC4Init(TIM4,&TIM_OCStructure);
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM4,ENABLE);
}


