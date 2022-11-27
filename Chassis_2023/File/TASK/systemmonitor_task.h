#ifndef SYSTEMMONITOR_TASK_H
#define SYSTEMMONITOR_TASK_H

#include "stm32f4xx_rcc.h"

#define Calc_System_Monitor_fps(xx)	do{	\
	systemMonitor.xx##_fps = systemMonitor.xx##_cnt;	\
	systemMonitor.xx##_cnt = 0;	\
}while(0)
	
extern RCC_ClocksTypeDef get_rcc_clock; 


#define Calc_All_fps() do{	\
	Calc_System_Monitor_fps(System);\
}while(0)

void SystemMonitorDetect(void);

#endif
