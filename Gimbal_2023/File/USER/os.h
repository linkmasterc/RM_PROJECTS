#ifndef __OS_H__
#define __OS_H__

#include "stm32f4xx.h"
#include "rm_os_types.h"
#include "main.h"

extern ST_TAST G_ST_Task[];
extern u32 TotalTaskNum;
extern u32 TaskTotalTime;
extern u32 TaskTotalTimeMost;
extern u32 TaskTotalTimeLeast;

void OS_RUN(void);
void IRQFlagPost(bool* IRQFlagParameter);
bool IRQFlagGet(bool* IRQFlagParameter);


#endif
