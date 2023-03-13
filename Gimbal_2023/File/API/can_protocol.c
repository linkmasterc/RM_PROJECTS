#include "ParamConfig.h"
#include "can_protocol.h"
#include "global_declare.h"
#include "rm_algorithm.h"
/**
* @brief		���豸��CAN���߷�������
 * @param 	Data1~4��Ӧ�����Ϲ��ص�ĳ�ĸ��豸��
						�ɷ��͵�id����
 * @return 	��
 */
void CAN_SendData(CAN_TypeDef* CANx,u32 id,s16 Data1,s16 Data2,s16 Data3,s16 Data4)
{
	CanTxMsg TX_Message;
	
	TX_Message.IDE=CAN_Id_Standard;							//ʹ�ñ�׼id������չid
	
	TX_Message.StdId=id;												//��ñ�֡���ݵ�id��
	
	TX_Message.DLC=0x08;												//��֡���ݵĳ�����8���ֽ�(�ó��ȷ�Χ��0��8)
	
	TX_Message.RTR=CAN_RTR_Data;								//CAN��������֡����Զ��֡
																							//(Զ��֡���������������豸������Ϣ��Զ��֡�����߱�����λ)
	
	
	TX_Message.Data[0] = (u8)(Data1 >> 8);			// �����ͽṹ����װ�����͵�ʮ��λ����ֵ
	TX_Message.Data[1] = (u8)Data1;
	TX_Message.Data[2] = (u8)(Data2 >> 8);
	TX_Message.Data[3] = (u8)Data2;
	TX_Message.Data[4] = (u8)(Data3 >> 8);
	TX_Message.Data[5] = (u8)Data3;
	TX_Message.Data[6] = (u8)(Data4 >> 8);
	TX_Message.Data[7] = (u8)Data4;
	
	CAN_Transmit(CANx,&TX_Message);
}


//Ħ������3508��C620���
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


//pitch����6020�����õ��
//���������2006��C610���
void CAN2_Protocol(void)
{
	switch(CAN2_RX_Message.StdId)
	{
	//ǰ��λ��int��curλ��
	//����λ��int��torque		
		case 0x206:
			AbsEncoderProcess(&g_stPitchEncoder,Get_Encoder_Number(&CAN2_RX_Message));
			Pitch_Encoder_angle = (float)((s16)g_stPitchEncoder.uiRawValue - GM_PITCH_MIDPOS) * 360/8192.0f;
			Pitch_Encoder_speed = - Get_Speed(&CAN2_RX_Message);
			break;
				
		case 0x205:	//�������
			AbsEncoderProcess(&g_stShooterEncoder,Get_Encoder_Number(&CAN2_RX_Message));
			g_stShooterPosPID.m_fpFB = g_stShooterEncoder.fpSumValue;
			g_stShooterSpeedPID.m_fpFB = Get_Speed(&CAN2_RX_Message)/g_stShooterEncoder.fpGearRatio;
			break;
				
		default:
			
		break;
	}
}


/*************************************************************************
�� �� ����GetEncoderNumber
�������ܣ�����6025��6623��RM3510����巵�صĻ�е�Ƕ�ֵ������ʽ������ֵ��
��    ע����е�Ƕ�ֵ��Χ��0~8191��0x1FFF��
*************************************************************************/
s32 Get_Encoder_Number(CanRxMsg* rx_message)
{
    s32 encoder_temp;
	encoder_temp = rx_message->Data[0]<<8 | rx_message->Data[1];
	return encoder_temp;
}


/************************************************************************
�� �� ����Get_Speed
�������ܣ�����RM3510��RM3508����巵�ص�ת�٣���λ��r/min
��    ע��RM3508������ٱ�Ϊ1��19��RM3510���������1��19��1��27
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
  * @brief  ����ʽ���������ݴ���
  
  * @retval None
  
  * @param	pEncoder	�������ṹ��
			value		��ǰ����ֵ
			
  * @note	0~8192ת��Ϊ������~������
			��Ա���ֵ��8191��ǰһ�Σ�-2����һ�Σ����������������
			��ʵ�ʱ仯3��������ֵǰ�����Ϊ-8189��
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
�� �� ����Get_Temperature
�������ܣ�����RM3508����巵�صĵ���¶ȣ���λ�����϶�
��    ע��
*************************************************************************/
float Get_Temperature(CanRxMsg* rx_message)
{
    float temperature_temp;
	temperature_temp = rx_message->Data[6];
	return temperature_temp;
}



