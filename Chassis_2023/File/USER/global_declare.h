#ifndef _GLOBAL_DECLARE_H_
#define _GLOBAL_DECLARE_H_


#include "rm_rc_types.h"
#include "stm32f4xx.h"
#include <string.h>
#define __packed

/** @brief 通讯所用变量 */
	/** @brief can通讯 */
	extern CanRxMsg CAN1_RX_Message;
	extern CanRxMsg CAN2_RX_Message;
	
	/** @brief 大疆遥控器*/
	extern ST_DBUS 	g_StDbus;	

#endif








