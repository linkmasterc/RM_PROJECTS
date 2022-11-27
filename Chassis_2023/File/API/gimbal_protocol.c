#include "gimbal_protocol.h"
#include "verify.h"

void GimbalReceiveDataProtocol(void)
{
	if(	GimbalCushioning_Rx[0]==0x55 &&																	// 帧头校验
		GimbalCushioning_Rx[1]==0x00 &&																	
		Verify_CRC8_Check_Sum((u8*)GimbalCushioning_Rx, ChassisBufLen_Rx))							// 帧尾CRC16校验
	{
		memcpy(&GimbalReceiveData.Receive, (u8*)GimbalCushioning_Rx, sizeof(GimbalReceiveData.Receive));	// 帧尾CRC16校验
		YawBMIAngle			= GimbalReceiveData.Receive.BMIYawAngle;										// 上云台YAW轴角度（陀螺仪测得，但实际使用的是电机编码器测得的角度）
		PitchBMIAngle	= GimbalReceiveData.Receive.BMIPitchAngle;									// 上云台Pitch轴角度(陀螺仪测得）
		if(PitchBMIAngle < 90.0f)																// 此处用意？
		{	
			YawBMISpeed = GimbalReceiveData.Receive.BMIYawSpeed;
		}
		else
		{	
			YawBMISpeed = -GimbalReceiveData.Receive.BMIYawSpeed;
		}
		
		systemMonitor.UART4_rx_valid_cnt++;																// 对有效接收次数进行计数（1s内）
	}
}

/** --------------------------------------------------------------------------
	* @brief  向下云台核心板发送的控制数据
			
	* @note	注意字节长度，添加CRC16校验字的时候长度与发送数据的长度相关
			如果发送变量长度改变注意修改
 -------------------------------------------------------------------------- **/
void GimbalSendDataProtocol(void)
{
	GimbalSendData.Send.PitAngleDes		= NoiseSimulator(GimbalPitchPosPid.m_fpDes,FALSE);
	GimbalSendData.Send.GIMBAL_STATUS	= g_StDbus.stRC.SW_L;
	GimbalSendData.Send.SHOOT_STATUS  = g_StDbus.stRC.SW_R;
//	GimbalSendData.Send.ShooterSendDes		= Bullet_Des_DN;
//	GimbalSendData.Send.FrictionSendDes		= Friction_State_DN;
//	GimbalSendData.Send.Flag_Run				= stFlag.DN_RunFlag;
//	GimbalSendData.Send.Flag_Shoot			= stFlag.DN_ShootFlag;
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



