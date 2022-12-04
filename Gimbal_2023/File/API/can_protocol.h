#ifndef CAN_PROTOCOL_H
#define CAN_PROTOCOL_H
#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
#include "rm_communicate_types.h"

s32 Get_Encoder_Number(CanRxMsg* rx_message);
s32 Get_Speed(CanRxMsg* rx_message);
void AbsEncoderProcess(ST_ENCODER* pEncoder, s32 value);
float Get_Temperature(CanRxMsg* rx_message);
void CAN_SendData(CAN_TypeDef* CANx,u32 id,s16 Data1,s16 Data2,s16 Data3,s16 Data4);
void CAN1_Protocol(void);
void CAN2_Protocol(void);
#endif

