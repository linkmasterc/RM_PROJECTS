#include "ParamConfig.h"
#include "can_protocol.h"
#include "global_declare.h"
#include "rm_algorithm.h"
/**
* @brief		由设备向CAN总线发送数据
 * @param 	Data1~4对应总线上挂载的某四个设备，
						由发送的id决定
 * @return 	无
 */
void CAN_SendData(CAN_TypeDef* CANx,u32 id,s16 Data1,s16 Data2,s16 Data3,s16 Data4)
{
	CanTxMsg TX_Message;
	
	TX_Message.IDE=CAN_Id_Standard;							//使用标准id而非扩展id
	
	TX_Message.StdId=id;												//获得本帧数据的id号
	
	TX_Message.DLC=0x08;												//本帧数据的长度是8个字节(该长度范围是0到8)
	
	TX_Message.RTR=CAN_RTR_Data;								//CAN发送数据帧而非远程帧
																							//(远程帧多用于请求其他设备发送信息，远程帧本身不具备数据位)
	
	
	TX_Message.Data[0] = (u8)(Data1 >> 8);			// 往发送结构体中装待发送的十六位变量值
	TX_Message.Data[1] = (u8)Data1;
	TX_Message.Data[2] = (u8)(Data2 >> 8);
	TX_Message.Data[3] = (u8)Data2;
	TX_Message.Data[4] = (u8)(Data3 >> 8);
	TX_Message.Data[5] = (u8)Data3;
	TX_Message.Data[6] = (u8)(Data4 >> 8);
	TX_Message.Data[7] = (u8)Data4;
	
	CAN_Transmit(CANx,&TX_Message);
}


//摩擦轮是3508，C620电调
void CAN1_Protocol(void)
{
		switch(CAN1_RX_Message.StdId)
	{
		case 0x208:
			AbsEncoderProcess(&g_stFriction1Encoder, Get_Encoder_Number(&CAN1_RX_Message));
			g_stFriction1SMC.m_fpFB = Get_Speed(&CAN1_RX_Message);
			Friction1_Temp = Get_Temperature(&CAN1_RX_Message);
		break;
		
		case 0x205:
			AbsEncoderProcess(&g_stFriction2Encoder, Get_Encoder_Number(&CAN1_RX_Message));
			g_stFriction2SMC.m_fpFB = Get_Speed(&CAN1_RX_Message);
			Friction2_Temp = Get_Temperature(&CAN1_RX_Message);
		break;
				
		case 0x206:
			AbsEncoderProcess(&GimbalSecondEncoder,Get_Encoder_Number(&CAN1_RX_Message));
			GimbalSecondPosPid.m_fpFB=GimbalSecondEncoder.fpSumValue/8192.0*360.0;
			GimbalSecondSpeedPid.m_fpFB=Get_Speed(&CAN1_RX_Message);
		
		break;
		default:
			
		break;
	}
}


//pitch都是6020，内置电调
//拨弹电机是2006，C610电调
void CAN2_Protocol(void)
{
	switch(CAN2_RX_Message.StdId)
	{
	//前四位是int型cur位置
	//后四位是int型torque		
		case 0x206:
			AbsEncoderProcess(&g_stPitchEncoder,Get_Encoder_Number(&CAN2_RX_Message));
			Pitch_Encoder_angle = (float)((s16)g_stPitchEncoder.uiRawValue - GM_PITCH_MIDPOS) * 360/8192.0f;
			Pitch_Encoder_speed = - Get_Speed(&CAN2_RX_Message);
			break;
				
		case 0x205:	//拨弹电机
			AbsEncoderProcess(&g_stShooterEncoder,Get_Encoder_Number(&CAN2_RX_Message));
			g_stShooterPosPID.m_fpFB = g_stShooterEncoder.fpSumValue;
			g_stShooterSpeedPID.m_fpFB = Get_Speed(&CAN2_RX_Message)/g_stShooterEncoder.fpGearRatio;
			break;
				
		default:
			
		break;
	}
}


/*************************************************************************
函 数 名：GetEncoderNumber
函数功能：接收6025、6623或RM3510电调板返回的机械角度值（绝对式编码器值）
备    注：机械角度值范围：0~8191（0x1FFF）
*************************************************************************/
s32 Get_Encoder_Number(CanRxMsg* rx_message)
{
    s32 encoder_temp;
	encoder_temp = rx_message->Data[0]<<8 | rx_message->Data[1];
	return encoder_temp;
}


/************************************************************************
函 数 名：Get_Speed
函数功能：接收RM3510或RM3508电调板返回的转速，单位：r/min
备    注：RM3508电机减速比为1：19；RM3510电机减速有1：19和1：27
*************************************************************************/
s32 Get_Speed(CanRxMsg* rx_message)
{
    s32 speed_temp;
	s32 base_value=0xFFFF;
	if(rx_message->Data[2] & 0x01<<7)
	{	
		speed_temp = (base_value<<16 | rx_message->Data[2]<<8 | rx_message->Data[3]);
	}
	else
	{
	    speed_temp = (rx_message->Data[2]<<8 | rx_message->Data[3]);//rpm
	}
	return speed_temp;
}

/** --------------------------------------------------------------------------
  * @brief  绝对式编码器数据处理
  
  * @retval None
  
  * @param	pEncoder	编码器结构体
			value		当前编码值
			
  * @note	0~8192转化为负无穷~正无穷
			针对编码值“8191（前一次）-2（后一次）”这种情况的跳变
			（实际变化3，但返回值前后差异为-8189）
 -------------------------------------------------------------------------- **/
void AbsEncoderProcess(ST_ENCODER* pEncoder, s32 value)
{
	pEncoder->uiPreRawValue = pEncoder->uiRawValue;
	pEncoder->uiRawValue = value;
	pEncoder->siDiff = value - pEncoder->uiPreRawValue;
	if(pEncoder->siDiff < -pEncoder->siNumber/2)
	{
		pEncoder->siDiff += pEncoder->siNumber;
	}
	else if(pEncoder->siDiff > pEncoder->siNumber/2)
	{
		pEncoder->siDiff -= pEncoder->siNumber;
	}
	pEncoder->fpSumValue += pEncoder->siDiff;
}

/*************************************************************************
函 数 名：Get_Temperature
函数功能：接收RM3508电调板返回的电机温度，单位：摄氏度
备    注：
*************************************************************************/
float Get_Temperature(CanRxMsg* rx_message)
{
    float temperature_temp;
	temperature_temp = rx_message->Data[6];
	return temperature_temp;
}



