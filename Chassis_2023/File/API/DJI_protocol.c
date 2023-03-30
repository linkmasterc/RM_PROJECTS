#include "DJI_protocol.h"
#include "verify.h"

static void RSDataDecode(u16 cmdID, u8 *pData, u16 Size);
u16 RsCmdID;

void RC_Init(void)
{
	g_StDbus.stRC.SW_L 	= RC_SW_DOWN;
	g_StDbus.stRC.SW_R 	= RC_SW_DOWN;
	g_StDbus.stRC.Ch0	= 1024;
	g_StDbus.stRC.Ch1	= 1024;
	g_StDbus.stRC.Ch2	= 1024;
	g_StDbus.stRC.Ch3	= 1024;
	g_StDbus.stRC.Ch4	= 1024;
}


/** --------------------------------------------------------------------------
  * @brief  ��Э�����ң����������ͨ������
  
  * @retval ���ڽ��յ���ͨ������
  
  * @param	None
			
  * @note	Э���ʽ�����RoboMaster ������ר��ң���������ջ����û��ֲ�
 -------------------------------------------------------------------------- **/
void RCProtocol(u8 *pData)
{
	/*��ҡ��*/
	g_StDbus.stRC.Ch0 = (pData[0] | (pData[1] << 8)) & 0x07FF;           					// Channe0����ˮƽͨ��
	g_StDbus.stRC.Ch1 = ((pData[1]) >> 3 | (pData[2] << 5)) & 0x07FF;   					// Channe1������ֱͨ��
	
	
	/*��ҡ��*/
	g_StDbus.stRC.Ch2 = ((pData[3] >> 6 | (pData[3] << 2) | (pData[4] << 10))) & 0x07FF;	// Channe2����ˮƽͨ��
	g_StDbus.stRC.Ch3 = ((pData[4] >> 1) | (pData[5] << 7)) & 0x07FF;                   	// Channe3������ֱͨ��
	
	
	/*����*/
	g_StDbus.stRC.Ch4 = (pData[16] | (pData[17] << 8)) & 0x07ff;                        	// Channe4������ֱͨ��

	
	/*��3λ����*/
	g_StDbus.stRC.SW_L = ((pData[5] >> 4) & 0x000C) >> 2;
	
	
	/*��3λ����*/
	g_StDbus.stRC.SW_R = ((pData[5] >> 4) & 0x0003);
	
	systemMonitor.USART1_rx_valid_cnt++;													// ����Ч���մ������м�������������Ϊ1s��
}

/** --------------------------------------------------------------------------
  * @brief  ����ϵͳ���ݽ���
  
  * @retval None
  
  * @param	uart_rx:	���ڽ���״̬���ṹ��
			pData:		RoboMaster����ϵͳ��Ϣ�洢����
			Size:		�������ݳ���

  * @note	�����������ο�����ϵͳѧ������Э�鸽¼��RoboMaster�ٷ��ĵ���
			��������Ե������ϵͳ����Э��Ϊ��Ҫ���ݣ�ÿ����ܻ�������仯
			2022�꣺frame_header (5-byte��SOF, data_length, seq, CRC8) + cmd_id (2-byte) + data (n-byte) + frame_tail (2-byte��CRC16������У��)
 -------------------------------------------------------------------------- **/
void RSProtocol(USART_RX_TypeDef *uart_rx, u8 *pData, u16 Size)
{
	RS_RX_Status RXStatus = RS_RX_FREE;
	u8 BufNum = 0;
	u8 RsData;
	u16 RsDataLength;
	u8 CorrectFlag = 0;															// ��ȷ���ձ�־λ


	for (int i = 0; i < Size; i++)												// ����ÿ�����յ��ֽ�
	{
		RsData = uart_rx->pMailbox[i];											// ��i�����յ��ֽ�
		switch (RXStatus)														// �жϸ��ֽ������ݰ���������λ��
		{
			case RS_RX_FREE:
				if (RsData == 0xA5)												// ���ݰ���ʼ֡SOF�̶�ֵ0xA5
				{
					BufNum = 0;													// ��0λ����
					RXStatus = RS_RX_Length;									// ��ʼ֡У��ɹ�׼��������һ�����ݶΣ���Ч���ݳ���
					pData[BufNum++] = 0xA5;										// �Ѹ�λ���ݴ����û�����Ľ���������			
				}
				else															// ����ʧ����ֱ�ӷ��������������ݽ����ֹ������󣬵ȴ���һ�ν���
				{
					RXStatus = RS_RX_FREE;
				}
				break;
			
			case RS_RX_Length:
				pData[BufNum++] = RsData;										// ���ձ�ʾ���ݳ��ȵ������ֽ�
				if(BufNum == 3)
				{
					RsDataLength = pData[1] | (pData[2] << 8);					// �õ���Ч���ݳ��ȣ�data_lengthռ�����ֽڣ�
					RXStatus = RS_RX_Num;										// ׼�����հ����
				}
				break;
			
			case RS_RX_Num:
				pData[BufNum++] = RsData;										// ���հ����
				RXStatus = RS_RX_CRC8;											// ׼������CRC8У����
				break;
			
			case RS_RX_CRC8:
				pData[BufNum++] = RsData;										// ����CRC8У����
				if (Verify_CRC8_Check_Sum(pData, BufNum))
				{	
					RXStatus = RS_RX_CmdID;										// CRC8У��ɹ���׼����������ID
				}
				else															
				{
					RXStatus = RS_RX_FREE;										// CRC8У��ʧ�ܣ������˴ν��յ�����
				}
				break;
			
			case RS_RX_CmdID:
				pData[BufNum++] = RsData;										// ��������ID
				if(BufNum == 7)
				{
					RsCmdID = pData[5] | (pData[6] << 8);						// �õ�����ID��
					RXStatus = RS_RX_Data;										// ׼����������	
				}
				break;
				
			case RS_RX_Data:
				pData[BufNum++] = RsData;										// ��������
				if(BufNum == 7 + RsDataLength)
				{
					RXStatus = RS_RX_CRC16;										// ׼����������CRC16У���ֽ�	
				}
				break;

			case RS_RX_CRC16:
				pData[BufNum++] = RsData;										// ��������CRC16У���ֽ�
				if(BufNum == 9 + RsDataLength)
				{
					if (Verify_CRC16_Check_Sum(pData, BufNum))
					{	
						RSDataDecode(RsCmdID, pData, RsDataLength);				// ����ϵͳ���ݸ��Ƶ���Ӧ�ṹ��	��ע����:һ�ο����жϻ���յ��������ݣ�һ���жϻ��ν��룩��
						
						RXStatus = RS_RX_FREE;
						systemMonitor.USART2_rx_valid_cnt++;					// ����Ч���մ������м�������������Ϊ1s��
					}
				}
				break;

			default:
				break;
		}
	}
}






/**
  * @brief  �����ϵͳ�����Զ������ݣ���δʹ�ã�
  * @retval None
  * @param	desID:	����Ŀ�������ID
  * 		pData:	������������
  * 		Size:	�������鳤��
  */
u8 CustomDataBuf[127] = {0};	//ʵ���������ݳ���Ϊ127��128�ᱻ����ϵͳ���Σ�10Hz���ͣ�
void SendDataToRS(u16 desID, u8 *pData, u16 Size)
{
	static u8 SendTimeCnt = 0;
	if(SendTimeCnt == 100)			//֡��1000Hz / 100 = 10Hz
	{
		/** ����ϵͳͨѶͷЭ�� */
		CustomDataBuf[0] = 0xA5;
		CustomDataBuf[1] = (u8) (Size + 6);
		CustomDataBuf[2] = (u8) ((Size + 6) >>8);
		CustomDataBuf[3] = 0x00;
		Append_CRC8_Check_Sum(CustomDataBuf, 5);
		
		/** ������ID */
		CustomDataBuf[5] = 0x01;
		CustomDataBuf[6] = 0x03;
		
		/** �����˼�ͨѶͷ�ṹ */
		StudentInteractiveHeaderData.data_cmd_id 	= 0x200;
		StudentInteractiveHeaderData.sender_ID 		= RobotIDMessage;
		StudentInteractiveHeaderData.receiver_ID 	= desID;
		memcpy(&CustomDataBuf[7], &StudentInteractiveHeaderData, 6);
		
		/** ���ݶ� */
		memcpy(&CustomDataBuf[13], pData, Size);

		/** ֡βУ�� */
		Append_CRC16_Check_Sum(CustomDataBuf, (Size + 6 + 9));
		
		/** ����2���� */
		DMA_ClearITPendingBit(USART2_TX_STREAM, DMA_IT_TCIF6);  //����DMA_Mode_Normal,����û��ʹ������ж�ҲҪ������������ֻ��һ��

		DMA_Cmd(USART2_TX_STREAM, DISABLE);				         //���õ�ǰ����ֵǰ�Ƚ���DMA
		USART2_TX_STREAM->M0AR = (uint32_t)CustomDataBuf; //���õ�ǰ�������ݻ���ַ:Memory0 tARget
		USART2_TX_STREAM->NDTR = (uint32_t)(Size + 6 + 9);//���õ�ǰ���������ݵ�����:Number of Data units to be TRansferred
		DMA_Cmd(USART2_TX_STREAM, ENABLE);				         //���ô���DMA����
		
		SendTimeCnt = 0;
	}
	SendTimeCnt++;
}





/**
  * @brief  ����ϵͳ���ݸ��Ƶ���Ӧ�ṹ�壬Э��Ϊ20210203 v1.0�汾
  * @retval None
  * @param	cmdID:	����ϵͳ����ָ��ID
  * 		pData:	RoboMaster����ϵͳ��Ϣ�洢����
  * 		Size:	���ݳ���
  */
void RSDataDecode(u16 cmdID, u8 *pData, u16 Size)
{
	static u16 	SumHurtTick;
	static u16 	SumHurtLastHP		= 600;									// ��һ�ν��յ�Ѫ��
	static u16 	SumHurt;	

	
	switch (cmdID)
	{
		case GameStatusID:
			RSSystemMonitor.GameStatus_cnt++;
			memcpy(&GameStatus, &pData[7], Size);
		
			StageRemainTimeMessage = GameStatus.stage_remain_time;
			break;
		case GameRobotHPID:
			RSSystemMonitor.GameRobotHP_cnt++;
			memcpy(&GameRobotHP, &pData[7], Size);
		
			if(RobotIDMessage == Red_7_Sentry)
			{
				OutpostHPMessage 		= GameRobotHP.red_outpost_HP;
				EnemySentryHPMessage	= GameRobotHP.blue_7_robot_HP;
			}
			else if(RobotIDMessage == Blue_7_Sentry)
			{
				OutpostHPMessage 		= GameRobotHP.blue_outpost_HP;
				EnemySentryHPMessage	= GameRobotHP.red_7_robot_HP;
			}
			break;
		case EventDataID:
			RSSystemMonitor.EventData_cnt++;
			memcpy(&EventData, &pData[7], Size);
		
			if((EventData.event_type >> 10) & 1)
			{
				Invincible_Status = TRUE;												// ǰ��վδ�����޵�״̬
			}
			else
			{
				Invincible_Status = FALSE;
			}
			
			break;
		case RefereeWaringID:
			RSSystemMonitor.RefereeWarning_cnt++;
			memcpy(&RefereeWarning, &pData[7], Size);
			break;
		case DartRemainingTimeID:
			RSSystemMonitor.DartRemainingTime_cnt++;
			memcpy(&DartRemainingTime, &pData[7], Size);
			break;
		case GameRobotStatusID:
			RSSystemMonitor.GameRobotStatus_cnt++;
			memcpy(&GameRobotStatus, &pData[7], Size);
		
			RobotIDMessage 				= GameRobotStatus.robot_id;							// ��������ID
			SentryHPMessage 			= GameRobotStatus.remain_HP;						// �������˵�ǰѪ��
			ChassisPowerLimitMessage 	= GameRobotStatus.chassis_power_limit;				// �������˵��̹�������
			Shooter1HeatLimitMessage	= GameRobotStatus.shooter_id1_17mm_cooling_limit;	// ��ǹ����������
			Shooter2HeatLimitMessage	= GameRobotStatus.shooter_id2_17mm_cooling_limit;	// ��ǹ����������
			Shooter1HeatCoolingMessage	= GameRobotStatus.shooter_id1_17mm_cooling_rate;	// ��ǹ��������ȴ��
			Shooter2HeatCoolingMessage	= GameRobotStatus.shooter_id2_17mm_cooling_rate;	// ��ǹ��������ȴ��
		
		
			if(!SumHurtFlag && (SentryHPMessage < SumHurtLastHP))							// �ܵ��˺�
			{
				SumHurtTick = systemMonitor.SysTickTime;
				SumHurtFlag = TRUE;
			}
			else if(systemMonitor.SysTickTime - SumHurtTick >= 1000)						// ��������
			{
				SumHurtFlag 		= FALSE;
				SumHurtPerSecond	= SumHurt;
				SumHurt			 	= 0;
			}
			SumHurt			+= SumHurtLastHP - SentryHPMessage;
			SumHurtLastHP 	= SentryHPMessage; 
			
			break;
			
		case PowerHeatDataID:
			RSSystemMonitor.PowerHeatData_cnt++;
			memcpy(&PowerHeatData, &pData[7], Size);
		
			PreChassisPowerMessage 			= ChassisPowerMessage;
			ChassisPowerMessage 			= PowerHeatData.chassis_power;						// ��ǰ���̹���
			ChassisPowerBufferMessageLast 	= ChassisPowerBufferMessage;						// �ϴε���ʣ�໺������
			ChassisPowerBufferMessage 		= PowerHeatData.chassis_power_buffer;				// ��ǰ����ʣ�໺������
			Barrel1_Heat_Cur 				= PowerHeatData.shooter_id1_17mm_cooling_heat;
			Barrel2_Heat_Cur 				= PowerHeatData.shooter_id2_17mm_cooling_heat;
		
//			if(ABS(PreChassisPowerMessage - ChassisPowerMessage) > 0.001f)
//			{	RSPowerUpdataFlag	= TRUE;}
//			else
//			{	RSPowerUpdataFlag	= FALSE;}
			RSPowerUpdataFlag	= TRUE;

		
//			Shooter1HeatRemainingNum = (Shooter1HeatLimitMessage - Barrel_Heat_Cur_UP) / PER_BULLET_HEAT;
//			Shooter2HeatRemainingNum = (Shooter2HeatLimitMessage - Barrel_Heat_Cur_DN) / PER_BULLET_HEAT;
			ShootHeatTick = systemMonitor.SysTickTime;
			break;
		case GameRobotPosID:
			RSSystemMonitor.GameRobotPos_cnt++;
			memcpy(&GameRobotPos, &pData[7], Size);
			break;
		case BuffID:
			RSSystemMonitor.Buff_cnt++;
			memcpy(&Buff, &pData[7], Size);
			break;
		case RobotHurtID:
			RSSystemMonitor.RobotHurt_cnt++;
			memcpy(&RobotHurt, &pData[7], Size);
			break;
		case ShootDataID:
			RSSystemMonitor.ShootData_cnt++;
			memcpy(&ShootData, &pData[7], Size);
		
			ShooterIDMessage = ShootData.shooter_id;
			if(ShooterIDMessage == 1)
			{
				Shooter1ShootNum++;
				Bullet1FreqMessage 	= ShootData.bullet_freq;						// ����̨��Ƶ		
				Bullet1SpeedMessage = ShootData.bullet_speed;						// ����̨����
				Bullet1Cnt++;														// ��ʵ����ӵ�����

//				#ifdef UPid1_DNid2
//				Shooter1NumberLast	= UPSTC.Fact_ShootSumNumber;			
//				#else
//				Shooter1NumberLast	= DNSTC.Fact_ShootSumNumber;
//				#endif
//				
//				if(!StartDNShootFlag)
//				{	Bullet1Cnt = 0;}
//				Bullet1Cnt++;
//				memmove(&Bullet1SpeedRecord[1], &Bullet1SpeedRecord[0], (RecordBullet1Num - 1) * sizeof(Bullet1SpeedRecord[0]));
//				Bullet1SpeedRecord[0] = Bullet1SpeedMessage;
//				
//				Bullet1SpeedMean = 0;
//				for(u16 i = 0; i < (5 < Bullet1Cnt ? 5 : Bullet1Cnt); i++)
//				{ Bullet1SpeedMean += Bullet1SpeedRecord[i];}
//				Bullet1SpeedMean /= (5 < Bullet1Cnt ? 5 : Bullet1Cnt);
				
			}
			else if(ShooterIDMessage == 2)
			{
				Shooter2ShootNum++;
				Bullet2FreqMessage 	= ShootData.bullet_freq;
				Bullet2SpeedMessage = ShootData.bullet_speed;
				Bullet2Cnt++;

//				#ifdef UPid1_DNid2
//				Shooter2NumberLast	= DNSTC.Fact_ShootSumNumber;
//				#else
//				Shooter2NumberLast	= UPSTC.Fact_ShootSumNumber;
//				#endif
//			
//				if(!StartUPShootFlag)
//				{	Bullet2Cnt = 0;}
//				Bullet2Cnt++;
//				memmove(&Bullet2SpeedRecord[1], &Bullet2SpeedRecord[0], (RecordBullet2Num - 1) * sizeof(Bullet2SpeedRecord[0]));
//				Bullet2SpeedRecord[0] = Bullet2SpeedMessage;
//				
//				Bullet2SpeedMean = 0;
//				for(u16 i = 0; i < (5 < Bullet2Cnt ? 5 : Bullet2Cnt); i++)
//				{ Bullet2SpeedMean += Bullet2SpeedRecord[i];}
//				Bullet2SpeedMean /= (5 < Bullet2Cnt ? 5 : Bullet2Cnt);

			}
			break;
		case BulletRemainingID:
			RSSystemMonitor.BulletRemaining_cnt++;
			memcpy(&BulletRemaining, &pData[7], Size);
		
			Bullet_Num_Remain = BulletRemaining.bullet_remaining_num_17mm;						// ��ȡʣ���ӵ���Ŀ
		
			break;
		case RobotInteractiveDataID:
			RSSystemMonitor.RobotInteractiveData_cnt++;
			memcpy(&StudentInteractiveHeaderData, &pData[7], 6);
			memcpy(&RobotInteractiveData, &pData[13], Size - 6);
			
			if(StudentInteractiveHeaderData.data_cmd_id == 0x200
				&& StudentInteractiveHeaderData.receiver_ID == RobotIDMessage
				&& StudentInteractiveHeaderData.sender_ID == RobotIDMessage - 1)
			{
				ResetState			= RobotInteractiveData.data[0];
				Range360Flag		= (bool)RobotInteractiveData.data[1];
				HitOurselfFlag		= (bool)RobotInteractiveData.data[2];
			}
			if(StudentInteractiveHeaderData.data_cmd_id == 0x201
				&& StudentInteractiveHeaderData.receiver_ID == RobotIDMessage
				&& StudentInteractiveHeaderData.sender_ID == RobotIDMessage + 2)
			{
				FindEnemyRoad	= (bool)RobotInteractiveData.data[0];
				FindEnemyR2		= (bool)RobotInteractiveData.data[1];
			}
			break;
		default:
			break;
	}
}
