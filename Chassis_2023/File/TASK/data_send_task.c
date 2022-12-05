#include "data_send_task.h"
#include "global_declare.h"
#include "can_protocol.h"
//#include "chassis_task.h"
#include "gimbal_task.h"
#include "stdlib.h"
#include "stdio.h"
#include "mode_choose_task.h"

void VofaDataScan()
{
	StateDataSendBuff.Send_Data1=GimbalPitchPosPid.m_fpDes;
	StateDataSendBuff.Send_Data2=GimbalPitchPosPid.m_fpFB;
	StateDataSendBuff.Send_Data3=GimbalYawPosPid.m_fpDes;
	StateDataSendBuff.Send_Data4=GimbalYawPosPid.m_fpFB;
	StateDataSendBuff.Send_Data5=Bullet1SpeedMessage;
	StateDataSendBuff.Send_Data6=Bullet2FreqMessage;
	StateDataSendBuff.Send_Data7=SentryHPMessage;
	StateDataSendBuff.Send_Data8=Bullet_Num_Remain;
	StateDataSendBuff.Send_Data9=Shooter2HeatLimitMessage;
	StateDataSendBuff.Send_Data10=Shooter2HeatCoolingMessage;
	StateDataSendBuff.Send_Data11=ChassisPowerBufferMessage;
	StateDataSendBuff.Send_Data12=ChassisPowerLimitMessage;

	DMA_ClearITPendingBit(USART2_TX_STREAM, DMA_IT_TCIF6);								//没开中断的情况下标志位要手动清除，否则只发一次
	
	DMA_Cmd(USART2_TX_STREAM, DISABLE);				       								//设置当前计数值前先禁用DMA
	while (DMA_GetCmdStatus(USART2_TX_STREAM) != DISABLE){} 							//确保 DMA 可以被设置
	USART2_TX_STREAM->M0AR = (uint32_t)&StateDataSendBuff;							//设置当前待发数据基地址:Memory0 tARget
	USART2_TX_STREAM->NDTR = (uint32_t)STATE_SEND_DATA_LEN;			        		//设置当前待发的数据的数量:Number of Data units to be TRansferred
	DMA_Cmd(USART2_TX_STREAM, ENABLE);	
}

