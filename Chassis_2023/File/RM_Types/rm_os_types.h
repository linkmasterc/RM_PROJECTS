#ifndef RM_OS_TYPES_H
#define RM_OS_TYPES_H

//ʱ�������꺯��������Ҫ����delay_ms(1000)���Ѷ೤ʱ�䣬д��"Time_EstimateFunction(delay_ms(1000));"
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
	OS_READY,		//�ȴ�ִ��
	OS_FINISH,		//ִ�����
	OS_SUSPENDED	//��ʱ����
} OS_TASK_STATE;

typedef struct
{
	char*			Name;				//���������ַ���
	void*			Func;				//������ָ��
	OS_TASK_STATE	State;				//����ִ��״̬
	unsigned int	TimeDelay;			//��������ӳ����ʱ���ٴν�������
	unsigned int	TimeExecuteLast;	//��һ��ִ�������ʱ��
	unsigned int	TimeDiff;			//�������������ִ��ʱ����
	unsigned int	TimeUsed;			//����ռ��ʱ��
	unsigned int	TimeUsedMost;		//����ռ��ʱ��
	unsigned int	TimeUsedLeast;		//����ռ��ʱ��
} ST_TAST;

#endif
