#ifndef __OS_H__
#define __OS_H__

#include "stm32f4xx.h"
#include "rm_os_types.h"
#include "rm_redefine_types.h"

#define OS_TIME() TIM5->CNT			// ��ʱ��5��Ϊ����ϵͳ��ʱ��Դ��CNTÿ1us����1

extern ST_TAST G_ST_Task[];
extern u32 TotalTaskNum;

void OS_RUN(void);
void IRQFlagPost(bool* IRQFlagParameter);
bool IRQFlagGet(bool* IRQFlagParameter);

#endif


