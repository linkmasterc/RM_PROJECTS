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
  * @brief  按协议接收遥控器发来的通信数据
  
  * @retval 串口接收到的通信数据
  
  * @param	None
			
  * @note	协议格式详情见RoboMaster 机器人专用遥控器（接收机）用户手册
 -------------------------------------------------------------------------- **/
void RCProtocol(u8 *pData)
{
	/*右摇杆*/
	g_StDbus.stRC.Ch0 = (pData[0] | (pData[1] << 8)) & 0x07FF;           					// Channe0——水平通道
	g_StDbus.stRC.Ch1 = ((pData[1]) >> 3 | (pData[2] << 5)) & 0x07FF;   					// Channe1——垂直通道
	
	
	/*左摇杆*/
	g_StDbus.stRC.Ch2 = ((pData[3] >> 6 | (pData[3] << 2) | (pData[4] << 10))) & 0x07FF;	// Channe2——水平通道
	g_StDbus.stRC.Ch3 = ((pData[4] >> 1) | (pData[5] << 7)) & 0x07FF;                   	// Channe3——垂直通道
	
	
	/*滑轮*/
	g_StDbus.stRC.Ch4 = (pData[16] | (pData[17] << 8)) & 0x07ff;                        	// Channe4——垂直通道

	
	/*左3位开关*/
	g_StDbus.stRC.SW_L = ((pData[5] >> 4) & 0x000C) >> 2;
	
	
	/*右3位开关*/
	g_StDbus.stRC.SW_R = ((pData[5] >> 4) & 0x0003);
	
	systemMonitor.USART1_rx_valid_cnt++;													// 对有效接收次数进行计数（计数周期为1s）
}

/** --------------------------------------------------------------------------
  * @brief  裁判系统数据解码
  
  * @retval None
  
  * @param	uart_rx:	串口接收状态机结构体
			pData:		RoboMaster裁判系统信息存储数组
			Size:		接收数据长度

  * @note	解码规则详情参考裁判系统学生串口协议附录（RoboMaster官方文档）
			解码规则以当年裁判系统串口协议为首要依据，每年可能会有少许变化
			2022年：frame_header (5-byte，SOF, data_length, seq, CRC8) + cmd_id (2-byte) + data (n-byte) + frame_tail (2-byte，CRC16，整包校验)
 -------------------------------------------------------------------------- **/
void RSProtocol(USART_RX_TypeDef *uart_rx, u8 *pData, u16 Size)
{
	RS_RX_Status RXStatus = RS_RX_FREE;
	u8 BufNum = 0;
	u8 RsData;
	u16 RsDataLength;
	u8 CorrectFlag = 0;															// 正确接收标志位


	for (int i = 0; i < Size; i++)												// 遍历每个接收的字节
	{
		RsData = uart_rx->pMailbox[i];											// 第i个接收的字节
		switch (RXStatus)														// 判断该字节在数据包中所处的位置
		{
			case RS_RX_FREE:
				if (RsData == 0xA5)												// 数据包起始帧SOF固定值0xA5
				{
					BufNum = 0;													// 第0位数据
					RXStatus = RS_RX_Length;									// 起始帧校验成功准备解码下一个数据段：有效数据长度
					pData[BufNum++] = 0xA5;										// 把该位数据存入用户定义的接收数组中			
				}
				else															// 数据失真则直接放弃接下来的数据解码防止解码错误，等待下一次接收
				{
					RXStatus = RS_RX_FREE;
				}
				break;
			
			case RS_RX_Length:
				pData[BufNum++] = RsData;										// 接收表示数据长度的两个字节
				if(BufNum == 3)
				{
					RsDataLength = pData[1] | (pData[2] << 8);					// 得到有效数据长度（data_length占两个字节）
					RXStatus = RS_RX_Num;										// 准备接收包序号
				}
				break;
			
			case RS_RX_Num:
				pData[BufNum++] = RsData;										// 接收包序号
				RXStatus = RS_RX_CRC8;											// 准备接收CRC8校验字
				break;
			
			case RS_RX_CRC8:
				pData[BufNum++] = RsData;										// 接收CRC8校验字
				if (Verify_CRC8_Check_Sum(pData, BufNum))
				{	
					RXStatus = RS_RX_CmdID;										// CRC8校验成功，准备接收数据ID
				}
				else															
				{
					RXStatus = RS_RX_FREE;										// CRC8校验失败，放弃此次接收的数据
				}
				break;
			
			case RS_RX_CmdID:
				pData[BufNum++] = RsData;										// 接收数据ID
				if(BufNum == 7)
				{
					RsCmdID = pData[5] | (pData[6] << 8);						// 得到数据ID号
					RXStatus = RS_RX_Data;										// 准备接收数据	
				}
				break;
				
			case RS_RX_Data:
				pData[BufNum++] = RsData;										// 接收数据
				if(BufNum == 7 + RsDataLength)
				{
					RXStatus = RS_RX_CRC16;										// 准备接收两个CRC16校验字节	
				}
				break;

			case RS_RX_CRC16:
				pData[BufNum++] = RsData;										// 接收两个CRC16校验字节
				if(BufNum == 9 + RsDataLength)
				{
					if (Verify_CRC16_Check_Sum(pData, BufNum))
					{	
						RSDataDecode(RsCmdID, pData, RsDataLength);				// 裁判系统数据复制到对应结构体	（注！！:一次空闲中断会接收到多组数据（一次中断会多次解码））
						
						RXStatus = RS_RX_FREE;
						systemMonitor.USART2_rx_valid_cnt++;					// 对有效接收次数进行计数（计数周期为1s）
					}
				}
				break;

			default:
				break;
		}
	}
}






/**
  * @brief  向裁判系统发送自定义数据（暂未使用）
  * @retval None
  * @param	desID:	发送目标机器人ID
  * 		pData:	发送数据数组
  * 		Size:	发送数组长度
  */
u8 CustomDataBuf[127] = {0};	//实测上限数据长度为127，128会被裁判系统屏蔽（10Hz发送）
void SendDataToRS(u16 desID, u8 *pData, u16 Size)
{
	static u8 SendTimeCnt = 0;
	if(SendTimeCnt == 100)			//帧率1000Hz / 100 = 10Hz
	{
		/** 裁判系统通讯头协议 */
		CustomDataBuf[0] = 0xA5;
		CustomDataBuf[1] = (u8) (Size + 6);
		CustomDataBuf[2] = (u8) ((Size + 6) >>8);
		CustomDataBuf[3] = 0x00;
		Append_CRC8_Check_Sum(CustomDataBuf, 5);
		
		/** 命令码ID */
		CustomDataBuf[5] = 0x01;
		CustomDataBuf[6] = 0x03;
		
		/** 机器人间通讯头结构 */
		StudentInteractiveHeaderData.data_cmd_id 	= 0x200;
		StudentInteractiveHeaderData.sender_ID 		= RobotIDMessage;
		StudentInteractiveHeaderData.receiver_ID 	= desID;
		memcpy(&CustomDataBuf[7], &StudentInteractiveHeaderData, 6);
		
		/** 数据段 */
		memcpy(&CustomDataBuf[13], pData, Size);

		/** 帧尾校验 */
		Append_CRC16_Check_Sum(CustomDataBuf, (Size + 6 + 9));
		
		/** 串口2发送 */
		DMA_ClearITPendingBit(USART2_TX_STREAM, DMA_IT_TCIF6);  //开启DMA_Mode_Normal,即便没有使用完成中断也要软件清除，否则只发一次

		DMA_Cmd(USART2_TX_STREAM, DISABLE);				         //设置当前计数值前先禁用DMA
		USART2_TX_STREAM->M0AR = (uint32_t)CustomDataBuf; //设置当前待发数据基地址:Memory0 tARget
		USART2_TX_STREAM->NDTR = (uint32_t)(Size + 6 + 9);//设置当前待发的数据的数量:Number of Data units to be TRansferred
		DMA_Cmd(USART2_TX_STREAM, ENABLE);				         //启用串口DMA发送
		
		SendTimeCnt = 0;
	}
	SendTimeCnt++;
}





/**
  * @brief  裁判系统数据复制到对应结构体，协议为20210203 v1.0版本
  * @retval None
  * @param	cmdID:	裁判系统数据指令ID
  * 		pData:	RoboMaster裁判系统信息存储数组
  * 		Size:	数据长度
  */
void RSDataDecode(u16 cmdID, u8 *pData, u16 Size)
{
	static u16 	SumHurtTick;
	static u16 	SumHurtLastHP		= 600;									// 上一次接收的血量
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
				Invincible_Status = TRUE;												// 前哨站未丢，无敌状态
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
		
			RobotIDMessage 				= GameRobotStatus.robot_id;							// 本机器人ID
			SentryHPMessage 			= GameRobotStatus.remain_HP;						// 本机器人当前血量
			ChassisPowerLimitMessage 	= GameRobotStatus.chassis_power_limit;				// 本机器人底盘功率上限
			Shooter1HeatLimitMessage	= GameRobotStatus.shooter_id1_17mm_cooling_limit;	// 上枪口热量缓冲
			Shooter2HeatLimitMessage	= GameRobotStatus.shooter_id2_17mm_cooling_limit;	// 下枪口热量缓冲
			Shooter1HeatCoolingMessage	= GameRobotStatus.shooter_id1_17mm_cooling_rate;	// 上枪口热量冷却率
			Shooter2HeatCoolingMessage	= GameRobotStatus.shooter_id2_17mm_cooling_rate;	// 下枪口热量冷却率
		
		
			if(!SumHurtFlag && (SentryHPMessage < SumHurtLastHP))							// 受到伤害
			{
				SumHurtTick = systemMonitor.SysTickTime;
				SumHurtFlag = TRUE;
			}
			else if(systemMonitor.SysTickTime - SumHurtTick >= 1000)						// 计算秒伤
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
			ChassisPowerMessage 			= PowerHeatData.chassis_power;						// 当前底盘功率
			ChassisPowerBufferMessageLast 	= ChassisPowerBufferMessage;						// 上次底盘剩余缓冲能量
			ChassisPowerBufferMessage 		= PowerHeatData.chassis_power_buffer;				// 当前底盘剩余缓冲能量
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
				Bullet1FreqMessage 	= ShootData.bullet_freq;						// 上云台弹频		
				Bullet1SpeedMessage = ShootData.bullet_speed;						// 下云台射速
				Bullet1Cnt++;														// 真实射出子弹计数

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
		
			Bullet_Num_Remain = BulletRemaining.bullet_remaining_num_17mm;						// 获取剩余子弹数目
		
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
