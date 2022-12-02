#include "gimbal_protocol.h"
#include "verify.h"

void GimbalReceiveDataProtocol(void)
{
	if(	GimbalCushioning_Rx[0]==0x55 &&																	// ֡ͷУ��
		GimbalCushioning_Rx[1]==0x00 &&																	
		Verify_CRC8_Check_Sum((u8*)GimbalCushioning_Rx, ChassisBufLen_Rx))							// ֡βCRC16У��
	{
		memcpy(&GimbalReceiveData.Receive, (u8*)GimbalCushioning_Rx, sizeof(GimbalReceiveData.Receive));	// ֡βCRC16У��
		YawBMIAngle			= GimbalReceiveData.Receive.BMIYawAngle;										// ����̨YAW��Ƕȣ������ǲ�ã���ʵ��ʹ�õ��ǵ����������õĽǶȣ�
		PitchBMIAngle	= GimbalReceiveData.Receive.BMIPitchAngle;									// ����̨Pitch��Ƕ�(�����ǲ�ã�
		TirggerState  = GimbalReceiveData.Receive.TriggerState;
		if(PitchBMIAngle < 90.0f)																// �˴����⣿
		{	
			YawBMISpeed = GimbalReceiveData.Receive.BMIYawSpeed;
		}
		else
		{	
			YawBMISpeed = -GimbalReceiveData.Receive.BMIYawSpeed;
		}
		
		systemMonitor.UART4_rx_valid_cnt++;																// ����Ч���մ������м�����1s�ڣ�
	}
}

/** --------------------------------------------------------------------------
	* @brief  ������̨���İ巢�͵Ŀ�������
			
	* @note	ע���ֽڳ��ȣ����CRC16У���ֵ�ʱ�򳤶��뷢�����ݵĳ������
			������ͱ������ȸı�ע���޸�
 -------------------------------------------------------------------------- **/
void GimbalSendDataProtocol(void)
{
	GimbalSendData.Send.PitAngleDes		= NoiseSimulator(GimbalPitchPosPid.m_fpDes,FALSE);
	GimbalSendData.Send.Shooter_Send_Des		= Bullet_Des;
	GimbalSendData.Send.Friction_Send_Des		= Friction_State;
	GimbalSendData.Send.Flag_Run				= stFlag.RunFlag;
	GimbalSendData.Send.Flag_Shoot			= stFlag.ShootFlag;
//	GimbalSendData.Send.Flag_Sniper			= stFlag.DN_SniperFlag;
//	GimbalSendData.Send.Flag_Reset			= stFlag.ResetFlag;
//	GimbalSendData.Send.Flag_Pitch_Protect	= stFlag.PT_DNFlag; 
	Append_CRC8_Check_Sum(&GimbalSendData.Send.head[0], ChassisBufLen_Tx);
	
	DMA_ClearITPendingBit(USART3_TX_STREAM, DMA_IT_TCIF3);
	
	DMA_Cmd(USART3_TX_STREAM, DISABLE);
	USART3_TX_STREAM->M0AR = (u32)&GimbalSendData.Send;
	USART3_TX_STREAM->NDTR = (u32)ChassisBufLen_Tx;
	DMA_Cmd(USART3_TX_STREAM, ENABLE);
}



