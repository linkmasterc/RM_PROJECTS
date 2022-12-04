#include "system_monitor_task.h"

void System_Monitor(void)
{
	DETECT_MONITOR(IMUSampleTask);
	DETECT_MONITOR(IMUUpdateTask);
	DETECT_MONITOR(GimbalTask);
	DETECT_MONITOR(DataSendTask);
}
