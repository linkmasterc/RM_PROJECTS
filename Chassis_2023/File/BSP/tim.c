#include "tim.h"

/****************************************************************************************************
函数名称: TIM5_Configuration()
函数功能: 操作系统定时器时间参考
输入参数: 无
返回参数: 无
备   注:  
****************************************************************************************************/
//TIM5 提供时间参考
#define TIM5_Prescaler  84-1
#define TIM5_Period     4294967296-1        //4294967296us=71min，时间够用了，不做多余处理

#define TIM3_Prescaler  840-1
#define TIM3_Period			1000-1
//定时器初始化，定时器5是32位通用定时器，APB1=84MHz，1us进一次中断
void TIM5_Configuration(void)
{
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    
	TIM_TimeBaseStructure.TIM_Prescaler         = TIM5_Prescaler; //设置预分频值
	TIM_TimeBaseStructure.TIM_Period            = TIM5_Period;  //设定自动重载值
	TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision     = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    
  TIM_Cmd(TIM5, ENABLE);
}


void TIM3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	TIM_TimeBaseStructure.TIM_Prescaler=TIM3_Prescaler;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //计数模式
	TIM_TimeBaseStructure.TIM_Period=TIM3_Period;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(  TIM3 ,&TIM_TimeBaseStructure);//初始化定时器
	
	TIM_Cmd(TIM3 ,ENABLE  );  //使能TIM
}

void TIM9_Configuration(u32 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_TimeICStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);

	/****************引脚配置******************/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM9);	
	
	/****************定时器初始化******************/
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=arr;
	TIM_TimeBaseStructure.TIM_Prescaler=psc;
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure);
	
	/****************输入捕获功能初始化******************/
	TIM_ICStructInit(&TIM_TimeICStructure);
	TIM_ICInit(TIM9,&TIM_TimeICStructure);
	
	/****************中断请求******************/
	TIM_ITConfig(TIM9,TIM_IT_CC1|TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM9,ENABLE);
	
}
