#ifndef __MC_MOTOR_H
#define __MC_MOTOR_H

#include "main.h"
void shoot_frequency(u8 fre_Hz);
s16 Get_Encoder_Number(CanRxMsg* pCanRxMsg);
s16 Get_Speed(CanRxMsg* pCanRxMsg);
s32 Abs_Encoder_Process(ST_ENCODER* pEncoder, s32 value);
void CAN_SendCurrent(CAN_TypeDef* CANx,u32 id,s16 current1,s16 current2,s16 current3,s16 current4);
void send_current(void);
void chassis_ctrl(void);
void YAW_TD_PID(void);
void PITCH_TD_PID(void);
void TRIGGER_PID(void);
void SHOOTER_PID(void);
#endif


