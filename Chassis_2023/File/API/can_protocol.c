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
bool FirstIN=TRUE;
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
	switch(CAN1_RX_Message.StdId)
	{
		case 0x400:  //��������
			capacitor_msg.CAP_Vol     = (float)( ( CAN1_RX_Message.Data[0] << 8 ) | (CAN1_RX_Message.Data[1] ) ) / 100.0f;
			capacitor_msg.Pow_In      = (float)( ( CAN1_RX_Message.Data[2] << 8 ) | (CAN1_RX_Message.Data[3] ) ) / 100.0f;
			capacitor_msg.Pow_Out     = (float)( ( CAN1_RX_Message.Data[4] << 8 ) | (CAN1_RX_Message.Data[5] ) ) / 100.0f;
			capacitor_msg.Volt_Out		= (float)( ( CAN1_RX_Message.Data[6] << 8 ) | (CAN1_RX_Message.Data[7] ) ) / 100.0f;	
		break;
		
		case 0x201:
		{
			AbsEncoderProcess(&stServoEncoder[0],GetEncoderNumber(CAN1_RX_Message.Data));
			stServoWheel_PosPid[0].m_fpFB=stServoEncoder[0].fpSumValue/8192.0*40.0;
			stServoWheel_SpeedPid[0].m_fpFB=GetSpeed(CAN1_RX_Message.Data)/9;
			systemMonitor.CAN1_SERVO1_RX_cnt++;
		}
		break;
		
		case 0x202:
		{
			stWheel_SpeedPid[1].m_fpFB=GetSpeed(CAN1_RX_Message.Data);
			systemMonitor.CAN1_SPEED2_RX_cnt++;
		}
		break;
		
		case 0x203:
		{
			stWheel_SpeedPid[2].m_fpFB=GetSpeed(CAN1_RX_Message.Data);
			systemMonitor.CAN1_SPEED3_RX_cnt++;
		}
		break;
		
		case 0x204:
		{
			AbsEncoderProcess(&stServoEncoder[3],GetEncoderNumber(CAN1_RX_Message.Data));
			stServoWheel_PosPid[3].m_fpFB=stServoEncoder[3].fpSumValue/8192.0*40.0;			
			stServoWheel_SpeedPid[3].m_fpFB=GetSpeed(CAN1_RX_Message.Data)/9;
			systemMonitor.CAN1_SERVO4_RX_cnt++;
		}
		break;
		case 0x205:	//�������
		{
			AbsEncoderProcess(&g_stShooterEncoder,GetEncoderNumber(CAN1_RX_Message.Data));
			c_stShooterPosPID.m_fpFB = -g_stShooterEncoder.fpSumValue;
			c_stShooterSpeedPID.m_fpFB = -(GetSpeed(CAN1_RX_Message.Data)/g_stShooterEncoder.fpGearRatio);
			systemMonitor.CAN1_BODAN_RX_cnt++;
		}

		
		default:
		break;
			
	}
		
	//				capacitor_msg.CAP_ERROR = CAN_RxMsg.Data[6] ;
}


/**
 * @brief		canͨѶ��Э�飬�ɽ��յ��������е�id��Ӧ���������������ʽ
 * @param 	
 * @return 	��
 */
void CAN2_Protocol(void)
{
	static bool FirstInDNYaw = TRUE;
//	static bool firstInUPYaw		= TRUE;
//	static bool firstInUPShoot 		= TRUE;
	switch(CAN2_RX_Message.StdId)
	{	
		/** ����̨Yaw 6020��� */
		case 0x205: 
		{
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
			systemMonitor.CAN2_YAW_RX_cnt++;
		}
		break;

		
		case 0x201:
		{
			stWheel_SpeedPid[0].m_fpFB=GetSpeed(CAN2_RX_Message.Data);
			systemMonitor.CAN2_SPEED1_RX_cnt++;
		}
		break;
		
		case 0x202:
		{
			AbsEncoderProcess(&stServoEncoder[1],GetEncoderNumber(CAN2_RX_Message.Data));
			stServoWheel_PosPid[1].m_fpFB=stServoEncoder[1].fpSumValue/8192.0*40.0;
			stServoWheel_SpeedPid[1].m_fpFB=GetSpeed(CAN2_RX_Message.Data)/9;
			systemMonitor.CAN2_SERVO2_RX_cnt++;
		}
		break;
		
		case 0x203:
		{
			AbsEncoderProcess(&stServoEncoder[2],GetEncoderNumber(CAN2_RX_Message.Data));
			stServoWheel_PosPid[2].m_fpFB=stServoEncoder[2].fpSumValue/8192.0*40.0;
			stServoWheel_SpeedPid[2].m_fpFB=GetSpeed(CAN2_RX_Message.Data)/9;
			systemMonitor.CAN2_SERVO3_RX_cnt++;
		}
		break;
		
		case 0x204:
		{
			stWheel_SpeedPid[3].m_fpFB=GetSpeed(CAN2_RX_Message.Data);
			systemMonitor.CAN2_SPEED4_RX_cnt++;
		}
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
void AbsEncoderProcess(ST_ENCODER* pEncoder, float value)
{
	if(ControlMode==0x00)
	{
		pEncoder->uiPreRawValue=value;
		FirstIN=FALSE;
	}
	else
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





