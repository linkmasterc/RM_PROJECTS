#include "chassis_protocol.h"
#include "global_declare.h"
#include "imu_update_task.h"
#include "bmi088_driver.h"
#include "verify.h"
#include "gpio.h"
#include "rm_communicate_types.h"
#include "rm_algorithm.h"
void ChassisSendDataProtocol(void)
{
	ChassisData.Send.BMIPitchAngle=imu_data.pit;
	ChassisData.Send.BMIPitchSpeed=Gyro_X_Speed;
	ChassisData.Send.BMIYawAngle=imu_data.yaw;
	ChassisData.Send.BMIYawSpeed=-Gyro_Z_Speed;
	
	ChassisData.Send.TriggerState		= TriggerState;
	if(TriggerState)
	{
		TriggerState = 0;
		Trigger_LOW();
	}
	Append_CRC8_Check_Sum(&ChassisData.Send.head[0], ChassisBufLen_Tx);
	
	DMA_ClearITPendingBit(USART3_TX_STREAM, DMA_IT_TCIF3);
	
	DMA_Cmd(USART3_TX_STREAM, DISABLE);
	USART3_TX_STREAM->M0AR = (u32)&ChassisData.Send;
	USART3_TX_STREAM->NDTR = (u32)ChassisBufLen_Tx;
	DMA_Cmd(USART3_TX_STREAM, ENABLE);
}

void GimbalReceiveDataProtocol(void)
{
	static s16 Last_Shooter_Send_Des = 0;

	if(	GimbalCushioning_Rx[0]==0x55 &&																	// 帧头校验
		GimbalCushioning_Rx[1]==0x00 &&																	
		Verify_CRC8_Check_Sum((u8*)GimbalCushioning_Rx, ChassisBufLen_Rx))							// 帧尾CRC8校验
	{
		systemMonitor.USART3_rx_valid_cnt++;																// 对有效接收次数进行计数（1s内）

		memcpy(&ChassisData.Receive, (u8*)GimbalCushioning_Rx, sizeof(ChassisData.Receive));	
		
		PitchPosDes=ChassisData.Receive.PitAngleDes;
		stGimbalFlag.ShootFlag      = (bool)ChassisData.Receive.Flag_Shoot;
		stGimbalFlag.RunFlag 		   	= (bool)ChassisData.Receive.Flag_Run;
		stGimbalFlag.ShootFlag 			= (bool)ChassisData.Receive.Flag_Shoot;
		stGimbalFlag.SniperFlag			= (bool)ChassisData.Receive.Flag_Sniper;
		
	if(stGimbalFlag.RunFlag == TRUE)
		{
			FPRampSignal(&g_stPitchSpeedPID.m_fpUMax, 28000, 10);
			#ifdef USING_G_COM
			G_Compensate = Gravity_Compensate;
			#endif
			if(stGimbalFlag.ShootFlag == TRUE)
			{
				g_stShooterPosPID.m_fpUMax = 6000;
				g_stShooterSpeedPID.m_fpUMax = 6000;
				if(ABS(Last_Shooter_Send_Des - ChassisData.Receive.Shooter_Send_Des) > 2)
				{
					g_stShooterPosPID.m_fpDes	= g_stShooterPosPID.m_fpFB;
					Last_Shooter_Send_Des = ChassisData.Receive.Shooter_Send_Des;
				}
				if(Last_Shooter_Send_Des != ChassisData.Receive.Shooter_Send_Des)
				{
					g_stShooterPosPID.m_fpDes 	= g_stShooterPosPID.m_fpDes + Bottom_SupplyStep * (ChassisData.Receive.Shooter_Send_Des - Last_Shooter_Send_Des);
					Last_Shooter_Send_Des = ChassisData.Receive.Shooter_Send_Des;
				}
			}
			else
			{
				g_stShooterPosPID.m_fpDes = g_stShooterPosPID.m_fpFB;
				Last_Shooter_Send_Des = ChassisData.Receive.Shooter_Send_Des;
				g_stShooterSpeedPID.m_fpUMax = 0;
			}
		}
		else if(stGimbalFlag.RunFlag == FALSE)
		{
			FPRampSignal(&g_stPitchSpeedPID.m_fpUMax,0,20);
			if(stGimbalFlag.ShootFlag == TRUE)
			{
				g_stShooterPosPID.m_fpUMax = 8000;
				g_stShooterSpeedPID.m_fpUMax = 8000;
				if(ABS(Last_Shooter_Send_Des - ChassisData.Receive.Shooter_Send_Des) > 2)
				{
					g_stShooterPosPID.m_fpDes 	= g_stShooterPosPID.m_fpFB;
					Last_Shooter_Send_Des = ChassisData.Receive.Shooter_Send_Des;
				}
				if(Last_Shooter_Send_Des != ChassisData.Receive.Shooter_Send_Des)
				{
					g_stShooterPosPID.m_fpDes 	= g_stShooterPosPID.m_fpDes + Bottom_SupplyStep * (ChassisData.Receive.Shooter_Send_Des - Last_Shooter_Send_Des);
					Last_Shooter_Send_Des = ChassisData.Receive.Shooter_Send_Des;
				}
			}
			else
			{
				g_stShooterPosPID.m_fpUMax 		= 0;
				g_stShooterPosPID.m_fpDes 		= g_stShooterPosPID.m_fpFB;
				Last_Shooter_Send_Des 			= ChassisData.Receive.Shooter_Send_Des;
				g_stShooterSpeedPID.m_fpUMax	= 0;
			}
		}
		//摩擦轮模式选择
		if(ChassisData.Receive.Friction_Send_Des != 500)
		{	stGimbalFlag.FrictionFlag = TRUE;}
		else if(ChassisData.Receive.Friction_Send_Des == 500)
		{	stGimbalFlag.FrictionFlag = FALSE;}
	}
}


