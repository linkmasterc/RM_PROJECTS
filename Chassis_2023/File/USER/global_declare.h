#ifndef _GLOBAL_DECLARE_H_
#define _GLOBAL_DECLARE_H_


#include "rm_rc_types.h"
#include "stm32f4xx.h"
#include <string.h>
#define __packed

/** @brief ͨѶ���ñ��� */
	/** @brief canͨѶ */
	extern CanRxMsg CAN1_RX_Message;
	extern CanRxMsg CAN2_RX_Message;
	
	/** @brief ��ң����*/
	extern ST_DBUS 	g_StDbus;	

#endif








