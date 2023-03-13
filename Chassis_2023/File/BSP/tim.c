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

void TIM9_Configuration(u32 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_TimeICStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);

	/****************��������******************/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM9);	
	
	/****************��ʱ����ʼ��******************/
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=arr;
	TIM_TimeBaseStructure.TIM_Prescaler=psc;
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure);
	
	/****************���벶���ܳ�ʼ��******************/
	TIM_ICStructInit(&TIM_TimeICStructure);
	TIM_ICInit(TIM9,&TIM_TimeICStructure);
	
	/****************�ж�����******************/
	TIM_ITConfig(TIM9,TIM_IT_CC1|TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM9,ENABLE);
	
}
