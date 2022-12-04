#ifndef RM_OS_TYPES_H
#define RM_OS_TYPES_H


#define LED_YELLOW_OFF()  		LED_D1_OFF()
#define LED_YELLOW_ON()		    LED_D1_ON()
#define LED_YELLOW_POLLING()  	LED_D1_POLLING()

#define LED_GREEN_OFF()  		LED_D2_OFF()
#define LED_GREEN_ON()		    LED_D2_ON()
#define LED_GREEN_POLLING()  	LED_D2_POLLING()

#define LED_BLUE_OFF()  		LED_D3_OFF()
#define LED_BLUE_ON()		    LED_D3_ON()
#define LED_BLUE_POLLING()  	LED_D3_POLLING()

#define LED_D1_OFF()  		GPIOC->BSRRL = GPIO_Pin_13
#define LED_D1_ON()		    GPIOC->BSRRH = GPIO_Pin_13
#define LED_D1_POLLING()   	GPIOC->ODR  ^= GPIO_Pin_13

#define LED_D2_OFF()  		GPIOC->BSRRL = GPIO_Pin_14
#define LED_D2_ON()		    GPIOC->BSRRH = GPIO_Pin_14
#define LED_D2_POLLING()   	GPIOC->ODR  ^= GPIO_Pin_14

#define LED_D3_OFF()  		GPIOC->BSRRL = GPIO_Pin_15
#define LED_D3_ON()		    GPIOC->BSRRH = GPIO_Pin_15
#define LED_D3_POLLING()   	GPIOC->ODR  ^= GPIO_Pin_15


//时间评估宏函数，例如要计算delay_ms(1000)花费多长时间，写作"Time_EstimateFunction(delay_ms(1000));"
#define Time_EstimateFunction(function)	do{	\
	u32 cnt = TIM5->CNT;	\
	function;	\
	printf("%s Runtime = %.3f ms\r\n", #function, (TIM5->CNT - cnt)/1000.0);	\
}while(0)


#define Time_EstimateCreateVar(VarName)	\
u32 TimeEstimate##VarName

#define Time_EsatimateStart(VarName)	do{	\
	extern u32 TimeEstimate##VarName;	\
	TimeEstimate##VarName = TIM5->CNT;	\
}while(0)

#define Time_EstimateEnd(VarName)	do{	\
	extern u32 TimeEstimate##VarName;	\
	TimeEstimate##VarName = TIM5->CNT - TimeEstimate##VarName;	\
	printf("%s Runtime = %.3f ms\r\n", #VarName, TimeEstimate##VarName/1000.0);	\
}while(0)


typedef enum
{
	OS_READY,		//等待执行
	OS_FINISH,		//执行完毕
	OS_SUSPENDED	//暂时挂起
} OS_TASK_STATE;

typedef struct
{
	char*			Name;				//任务名称字符串
	void*			Func;				//任务函数指针
	OS_TASK_STATE	State;				//任务执行状态
	unsigned int	TimeDelay;			//任务结束延迟最多时间再次进入任务
	unsigned int	TimeExecuteLast;	//上一次执行任务的时刻
	unsigned int	TimeDiff;			//相邻两次任务的执行时间间隔
	unsigned int	TimeUsed;			//任务占用时间
	unsigned int	TimeUsedMost;		//任务占用时间
	unsigned int	TimeUsedLeast;		//任务占用时间
} ST_TAST;

#endif
