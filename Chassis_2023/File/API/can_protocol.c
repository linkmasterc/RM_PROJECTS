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


/**
 * @brief		canͨѶ��Э�飬�ɽ��յ��������е�id��Ӧ���������������ʽ
 * @param 	
 * @return 	��
 */
void CAN1_Protocol(void)
{
	
}


/**
 * @brief		canͨѶ��Э�飬�ɽ��յ��������е�id��Ӧ���������������ʽ
 * @param 	
 * @return 	��
 */
void CAN2_Protocol(void)
{
	static bool FirstInDNYaw = TRUE;
	static bool firstInUPYaw		= TRUE;
	static bool firstInUPShoot 		= TRUE;
	switch(CAN2_RX_Message.StdId)
	{	
		/** ����̨Yaw 6020��� */
		case 0x205: 
		SubSystemMonitor.CAN2_YawMotor_rx_cnt++;
		if(FirstInDNYaw)
		{
			GimbalYawEncoder.uiRawValue = GetEncoderNumber(CAN2_RX_Message.Data);													// ���λ�ȡ�������ֵfpSumValueȡ��ǰ����ֵ
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
			AbsEncoderProcess(&GimbalYawEncoder, GetEncoderNumber(CAN2_RX_Message.Data));											// ��YAW������������ص�ԭʼֵ����Ԥ����
		}
			YawEncoderAngle = (GimbalYawEncoder.fpSumValue - DN_YAW_MID) * 360 / 8192.0f;										// ��YAW����趨��ֵΪ0��,�õ�YAW�ᵱǰ�Ƕ�
			YawEncoderSpeed = GetSpeed(CAN2_RX_Message.Data);																	// �õ���������õ��ٶ�
		break;

		default:
			break;
	}
}


/**
 * @brief	��ȡ���������صĻ�е�Ƕȣ����÷�Χ��C610��C620��6020���
 * @param 	pData 	ԭʼ��������
 * @return 	��е�Ƕ�(0~8191)
 */
s32 GetEncoderNumber(u8* pData)
{
    s32 encoder_temp;
	encoder_temp = pData[0]<<8 | pData[1];
	return encoder_temp;
}


/**
 * @brief	��ȡ���������صĵ��ת�٣����÷�Χ��C610��C620��6020���
 * @param 	pData	ԭʼ��������
 * @return	���ת��
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





