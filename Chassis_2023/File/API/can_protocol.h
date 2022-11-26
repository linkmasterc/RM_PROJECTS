#ifndef CAN_PROTOCOL_H
#define CAN_PROTOCOL_H
#include "stm32f4xx.h"
#include "stm32f4xx_can.h"
#include "rm_communicate_types.h"

s32 GetEncoderNumber(u8* pData);
s32 GetSpeed(u8* pData);
void AbsEncoderProcess(ST_ENCODER* pEncoder, s32 value);
void CAN_SendData(CAN_TypeDef* CANx,u32 id,s16 Data1,s16 Data2,s16 Data3,s16 Data4);
void CAN1_Ptotocol(void);
void CAN2_Ptotocol(void);
#endif

