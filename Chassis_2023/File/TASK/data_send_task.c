#include "data_send_task.h"
#include "global_declare.h"
#include "gimbal_task.h"
#include "mode_choose_task.h"

void VofaDataScan()
{
	#ifdef Vision_Use_Vofa
	StateDataSendBuff.Send_Data1=VisionDataReceiveBuff.stVisionData.Recieve_Data1;
	StateDataSendBuff.Send_Data2=VisionDataReceiveBuff.stVisionData.Recieve_Data2;
	StateDataSendBuff.Send_Data3=VisionDataReceiveBuff.stVisionData.Recieve_Data3;
	StateDataSendBuff.Send_Data4=VisionDataReceiveBuff.stVisionData.Recieve_Data4;
	StateDataSendBuff.Send_Data5=VisionDataReceiveBuff.stVisionData.Recieve_Data5;
	StateDataSendBuff.Send_Data6=VisionDataReceiveBuff.stVisionData.Recieve_Data6;
	StateDataSendBuff.Send_Data7=VisionDataReceiveBuff.stVisionData.Recieve_Data7;
	StateDataSendBuff.Send_Data8=VisionDataReceiveBuff.stVisionData.Recieve_Data8;
//	StateDataSendBuff.Send_Data9=Shooter2HeatLimitMessage;
//	StateDataSendBuff.Send_Data10=Shooter2HeatCoolingMessage;
//	StateDataSendBuff.Send_Data11=ChassisPowerBufferMessage;
//	StateDataSendBuff.Send_Data12=ChassisPowerLimitMessage;
//	StateDataSendBuff.Send_Data13=US_Distance;
//	StateDataSendBuff.Send_Data14=TIM9->CNT;
	
	#elif defined Gimbal_Check_Vofa
	StateDataSendBuff.Send_Data1=GimbalPitchPosPid.m_fpDes;
	StateDataSendBuff.Send_Data2=GimbalPitchPosPid.m_fpFB;
	StateDataSendBuff.Send_Data3=PitchBMISpeed;
	StateDataSendBuff.Send_Data4=GimbalYawPosPid.m_fpDes;
	StateDataSendBuff.Send_Data5=GimbalYawPosPid.m_fpFB;
	StateDataSendBuff.Send_Data6=GimbalYawSpeedPid.m_fpFB;

	
	#elif defined Normal_Use_Vofa
	#endif
	
	DMA_ClearITPendingBit(UART4_TX_STREAM, DMA_IT_TCIF4);								//没开中断的情况下标志位要手动清除，否则只发一次
	
	DMA_Cmd(UART4_TX_STREAM, DISABLE);				       								//设置当前计数值前先禁用DMA
	while (DMA_GetCmdStatus(UART4_TX_STREAM) != DISABLE){} 							//确保 DMA 可以被设置
	UART4_TX_STREAM->M0AR = (uint32_t)&StateDataSendBuff;							//设置当前待发数据基地址:Memory0 tARget
	UART4_TX_STREAM->NDTR = (uint32_t)STATE_SEND_DATA_LEN;			        		//设置当前待发的数据的数量:Number of Data units to be TRansferred
	DMA_Cmd(UART4_TX_STREAM, ENABLE);	
}

