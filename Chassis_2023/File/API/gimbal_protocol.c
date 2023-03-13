#include "gimbal_protocol.h"
#include "verify.h"

void GimbalReceiveDataProtocol(void)
{
	if(	GimbalCushioning_Rx[0]==0x55 &&																	// ֡ͷУ��
		GimbalCushioning_Rx[1]==0x00 &&																	
		Verify_CRC8_Check_Sum((u8*)GimbalCushioning_Rx, ChassisBufLen_Rx))							// ֡βCRC16У��
	{
		memcpy(&GimbalData.Receive, (u8*)GimbalCushioning_Rx, sizeof(GimbalData.Receive));	// ֡βCRC16У��
		
		YawBMIAngle			= -GimbalData.Receive.BMIYawAngle;										// ����̨YAW��Ƕȣ������ǲ�ã���ʵ��ʹ�õ��ǵ����������õĽǶȣ�
		AbsEncoderProcess(&GimbalYawABS,YawBMIAngle);
		GimbalYawPosPid.m_fpFB=GimbalYawABS.fpSumValue;
		
		GimbalPitchPosPid.m_fpFB	= GimbalData.Receive.BMIPitchAngle;									// ����̨Pitch��Ƕ�(�����ǲ�ã�
//		GimbalPitchPosPid.m_fpDes = GimbalData.Receive.PitchDesAngle;
		PitchBMISpeed = GimbalData.Receive.BMIPitchSpeed;
		TirggerState  = GimbalData.Receive.TriggerState;
		if(PitchBMIAngle < 90.0f)																// �˴����⣿
		{	
			YawBMISpeed = GimbalData.Receive.BMIYawSpeed;
		}
		else
		{	
			YawBMISpeed = -GimbalData.Receive.BMIYawSpeed;
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
	GimbalData.Send.PitAngleDes		= -NoiseSimulator(GimbalPitchPosPid.m_fpDes,FALSE);
	GimbalData.Send.Shooter_Send_Des		= Bullet_Des;
	GimbalData.Send.Friction_Send_Des		= Friction_State;
	GimbalData.Send.Flag_Run				= stFlag.GimbalRunFlag;
	GimbalData.Send.Flag_Shoot			= stFlag.ShootFlag;
	GimbalData.Send.Flag_Sniper			= stFlag.SniperFlag;
//	GimbalData.Send.Flag_Reset			= stFlag.ResetFlag;
//	GimbalData.Send.Flag_Pitch_Protect	= stFlag.PT_DNFlag; 
	Append_CRC8_Check_Sum(&GimbalData.Send.head[0], ChassisBufLen_Tx);
	
	DMA_ClearITPendingBit(USART3_TX_STREAM, DMA_IT_TCIF3);
	
	DMA_Cmd(USART3_TX_STREAM, DISABLE);
	USART3_TX_STREAM->M0AR = (u32)&GimbalData.Send;
	USART3_TX_STREAM->NDTR = (u32)ChassisBufLen_Tx;
	DMA_Cmd(USART3_TX_STREAM, ENABLE);
}



