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


/**
 * @brief		can通讯的协议，由接收到的数据中的id对应到电机来决定处理方式
 * @param 	
 * @return 	无
 */
void CAN1_Protocol(void)
{
	
}


/**
 * @brief		can通讯的协议，由接收到的数据中的id对应到电机来决定处理方式
 * @param 	
 * @return 	无
 */
void CAN2_Protocol(void)
{
	static bool FirstInDNYaw = TRUE;
	static bool firstInUPYaw		= TRUE;
	static bool firstInUPShoot 		= TRUE;
	switch(CAN2_RX_Message.StdId)
	{	
		/** 下云台Yaw 6020电机 */
		case 0x205: 
		SubSystemMonitor.CAN2_YawMotor_rx_cnt++;
		if(FirstInDNYaw)
		{
			GimbalYawEncoder.uiRawValue = GetEncoderNumber(CAN2_RX_Message.Data);													// 初次获取电机编码值fpSumValue取当前编码值
			GimbalYawEncoder.fpSumValue = GimbalYawEncoder.uiRawValue;
			if((GimbalYawEncoder.fpSumValue-DN_YAW_MID) > GimbalYawEncoder.siNumber / 2)								
			{	
				GimbalYawEncoder.fpSumValue -= GimbalYawEncoder.siNumber;
			}
			else if((GimbalYawEncoder.fpSumValue-DN_YAW_MID) < -GimbalYawEncoder.siNumber / 2)
			{	
				GimbalYawEncoder.fpSumValue += GimbalYawEncoder.siNumber;
			}
			FirstInDNYaw = FALSE;
		}
		else
		{	
			AbsEncoderProcess(&GimbalYawEncoder, GetEncoderNumber(CAN2_RX_Message.Data));											// 对YAW电机编码器返回的原始值进行预处理
		}
			YawEncoderAngle = (GimbalYawEncoder.fpSumValue - DN_YAW_MID) * 360 / 8192.0f;										// 以YAW电机设定中值为0°,得到YAW轴当前角度
			YawEncoderSpeed = GetSpeed(CAN2_RX_Message.Data);																	// 得到编码器测得的速度
		break;

		default:
			break;
	}
}


/**
 * @brief	读取编码器返回的机械角度，可用范围是C610、C620、6020电机
 * @param 	pData 	原始数据数组
 * @return 	机械角度(0~8191)
 */
s32 GetEncoderNumber(u8* pData)
{
    s32 encoder_temp;
	encoder_temp = pData[0]<<8 | pData[1];
	return encoder_temp;
}


/**
 * @brief	读取编码器返回的电机转速，可用范围是C610、C620、6020电机
 * @param 	pData	原始数据数组
 * @return	电机转速
 */
s32 GetSpeed(u8* pData)
{
	s32 speed_temp;
	s32 base_value = 0xFFFF;
	if(pData[2] & 0x01<<7 )
	{	speed_temp = (base_value<<16 | pData[2]<<8 | pData[3]);}
	else
	{	speed_temp = pData[2]<<8 | pData[3];}
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





