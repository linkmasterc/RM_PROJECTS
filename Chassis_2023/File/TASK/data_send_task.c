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
	StateDataSendBuff.Send_Data1=Bullet2SpeedMessage;
	StateDataSendBuff.Send_Data2=Bullet2FreqMessage;
	StateDataSendBuff.Send_Data3=HP_Detect();
	StateDataSendBuff.Send_Data4=GimbalPitchPosPid.m_fpFB;
	StateDataSendBuff.Send_Data5=GimbalPitchPosPid.m_fpDes;
	StateDataSendBuff.Send_Data6=GimbalYawPosPid.m_fpFB;
	StateDataSendBuff.Send_Data7=GimbalYawPosPid.m_fpDes;
	
	DMA_ClearITPendingBit(USART2_TX_STREAM, DMA_IT_TCIF6);								//û���жϵ�����±�־λҪ�ֶ����������ֻ��һ��
	DMA_Cmd(USART2_TX_STREAM, DISABLE);				       								//���õ�ǰ����ֵǰ�Ƚ���DMA
	while (DMA_GetCmdStatus(USART2_TX_STREAM) != DISABLE){} 							//ȷ�� DMA ���Ա�����
	USART2_TX_STREAM->M0AR = (uint32_t)&StateDataSendBuff;							//���õ�ǰ�������ݻ���ַ:Memory0 tARget
	USART2_TX_STREAM->NDTR = (uint32_t)STATE_SEND_DATA_LEN;			        		//���õ�ǰ���������ݵ�����:Number of Data units to be TRansferred
	DMA_Cmd(USART2_TX_STREAM, ENABLE);	
	
}

