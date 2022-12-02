#include "vision_protocol.h"
#define ABS(x) ( (x)>0?(x):-(x) )
/***********************串口5&6****************************
串口协议为 55 00 ID NUM NUM*float 00 AA

比如 功能ID为 03
发送数据为6个float 则协议为
55 00 03 06 xx xx xx xx 00 AA
								 4x6
*********************************************************/

/*******************************数据帧协议格式*******************************/
/*0x55    0x00    0xXX    0xXX    |0xXX 0xXX 0xXX 0xXX|    0x00    0xAA*/
/*帧头             ID   数据长度  |   1个浮点数据     |            帧尾*/
/****************************************************************************/

static float HP_Detect(void);

bool UPReceiveFlag = FALSE;
bool DNReceiveFlag = FALSE;


/** --------------------------------------------------------------------------
  * @brief  解码下云台小电脑发送的视觉数据
  
  * @retval None
  
  * @param	None
			
  * @note	串口协议为 55 00 ID NUM NUM*float 00 AA

			比如 功能ID为 03
			发送数据为6个float 则协议为
			55 00 03 06 xx xx xx xx 00 AA

			**目前是按指定六个数据来接收的 **
			详细注释参考上云台小电脑解码函数
 -------------------------------------------------------------------------- **/
void DNVisionDataProtocol(void) {
	V_RX_Status DNStatus = V_RX_FREE_1;
	u8 DNBit = 0;
	u8 DNDataNum = 0;
	u8 DNData;

	DNReceiveFlag = TRUE;														// 接收到数据才开始发送
	
	for (u32 i = 0; i < VisionDataBuffLen; i++) {
		DNData = VisionRcr.pMailbox[i];

		switch (DNStatus) {
			case V_RX_FREE_1:
				if (DNData == 0x55) {
					DNStatus = V_RX_FREE_2;
				}
				break;
			case V_RX_FREE_2:
				if (DNData == 0x00) {
					DNStatus = V_RX_FLAG;
				} else 
				{
					i = VisionDataBuffLen;													// 接收错误则立即结束此次接收
				}
				break;
			case V_RX_FLAG:
				if (DNData == 0x00 || DNData == 0x01 || DNData == 0x02 || DNData == 0x03) {
					VisionDataReceiveBuff.pVisionData[0] = DNData;
					DNDataNum = VISION_RECIEVE_DATA_NUM_DN;
					DNStatus = V_RX_START;
				} else 
				{
					i = VisionDataBuffLen;													// 接收错误则立即结束此次接收

				}
				break;
			case V_RX_START:
				if (DNData == DNDataNum) {
					DNStatus = V_RX_DATA;
					DNBit = 0;
				} else 
				{
					i = VisionDataBuffLen;													// 接收错误则立即结束此次接收
				}
				break;
			case V_RX_DATA:
				if (DNBit < DNDataNum * 4) {
					*(VisionDataBuffTemp + DNBit) = DNData;
					DNBit++;
				} 
				else 
				{
					if (DNData == 0x00) {
						DNStatus = V_RX_END;
					} 
					else 
					{
						i = VisionDataBuffLen;													// 接收错误则立即结束此次接收
					}
				}
				break;
			case V_RX_END: {
				if (DNData == 0xAA)																	// 如果接到了0xAA，数据有效
				{
					for (u32 j = 1; j < VISION_RECIEVE_DATA_LEN_DN; j++) {
						VisionDataReceiveBuff.pVisionData[j] = VisionDataBuffTemp[j - 1];
					}
				}
				DNStatus = V_RX_FREE_1;
				
				
				if(fpclassify(VisionDataReceiveBuff.stVisionData.Recieve_Data1) != FP_NORMAL				// 为了防止接收到奇怪的数据
					|| fpclassify(VisionDataReceiveBuff.stVisionData.Recieve_Data2) != FP_NORMAL
					|| fpclassify(VisionDataReceiveBuff.stVisionData.Recieve_Data3) != FP_NORMAL
			//		|| fpclassify(VisionDataReceiveBuff.stVisionData.Recieve_Data4) != FP_NORMAL
			//		|| fpclassify(VisionDataReceiveBuff.stVisionData.Recieve_Data5) != FP_NORMAL
			//		|| fpclassify(VisionDataReceiveBuff.stVisionData.Recieve_Data6) != FP_NORMAL
				)
				{
					VisionDataReceiveBuff.stVisionData.Recieve_ID	= 0;
				}
				
				if(ABS(VisionDataReceiveBuff.stVisionData.Recieve_Data1 - GimbalPitchPosPid.m_fpFB) > 40.0f)
				{	VisionDataReceiveBuff.stVisionData.Recieve_Data1 = VisionData1;}
				if(ABS(VisionDataReceiveBuff.stVisionData.Recieve_Data2 - GimbalPitchPosPid.m_fpFB) > 90.0f)
				{	VisionDataReceiveBuff.stVisionData.Recieve_Data2 = VisionData2;}
				
				VisionData1 = VisionDataReceiveBuff.stVisionData.Recieve_Data1;								// pitch
				VisionData2 = VisionDataReceiveBuff.stVisionData.Recieve_Data2;								// yaw
				VisionData3 = VisionDataReceiveBuff.stVisionData.Recieve_Data3;
				VisionData4 = VisionDataReceiveBuff.stVisionData.Recieve_Data4;
				VisionData5 = VisionDataReceiveBuff.stVisionData.Recieve_Data5;
				VisionData6 = VisionDataReceiveBuff.stVisionData.Recieve_Data6;
				VisionData7 = VisionDataReceiveBuff.stVisionData.Recieve_Data7;
				VisionData8 = VisionDataReceiveBuff.stVisionData.Recieve_Data8;
				VisionData9 = VisionDataReceiveBuff.stVisionData.Recieve_Data9;
				VisionData10 = VisionDataReceiveBuff.stVisionData.Recieve_Data10;
				VisionData11 = VisionDataReceiveBuff.stVisionData.Recieve_Data11;
				
				systemMonitor.USART6_rx_valid_cnt++;															// 对有效接收次数进行计数（1s内）

			}
				break;

			default:
				break;
		}
	}
}






/** --------------------------------------------------------------------------
  * @brief  向下云台小电脑发送的比赛数据
  
  * @retval None
  
  * @param	None
			
  * @note	

 -------------------------------------------------------------------------- **/
void DNVisionDataSend(void)
{
//	DNTirggerTimes++;

	if (RobotIDMessage == Red_7_Sentry)
	{	VisionDataSendBuff.My_Data.ID = 0x09;}
	else if (RobotIDMessage == Blue_7_Sentry) 
	{	VisionDataSendBuff.My_Data.ID = 0x0A;}
	
	VisionDataSendBuff.My_Data.Length = VISION_SEND_DATA_NUM_DN;

		/*击打己方判断*/
	#ifdef HIT_OURSELF
	if(RobotIDMessage == Red_7_Sentry && HitOurselfFlag)
	{	DNVisionDataSendBuff.My_Data.ID = 0x0A;}		//红方ID 7	（识别红色）
	else if(RobotIDMessage == Blue_7_Sentry && HitOurselfFlag)
	{	DNVisionDataSendBuff.My_Data.ID = 0x09;}		//蓝方ID 17	（识别蓝色）
	#endif
	
	/* PROTOCOL DATA CODE BEGIN */
	VisionDataSendBuff.My_Data.Send_Data1 = VisionSendData1 = GimbalPitchPosPid.m_fpFB;
	VisionDataSendBuff.My_Data.Send_Data2 = VisionSendData2 = GimbalYawPosPid.m_fpFB;
	VisionDataSendBuff.My_Data.Send_Data3 = VisionSendData3 = 1.0;//Bullet2SpeedMessage;
	VisionDataSendBuff.My_Data.Send_Data4 = VisionSendData4 = 1.0;//HP_Detect();
	VisionDataSendBuff.My_Data.Send_Data5 = VisionSendData5 = 1.0;//rush.EncoderSite - rush.LeftOffset;
	VisionDataSendBuff.My_Data.Send_Data6 = VisionSendData6 = 1.0;//EncoderSpeed;
	if(BanlanReconizeFinshFlag)
	{
		VisionDataSendBuff.My_Data.Send_Data7 = VisionSendData7 = BalancesoldierNumber;
	}
	VisionDataSendBuff.My_Data.Send_Data8 = VisionSendData8 = 1.0;//UPVisionDataReceiveBuff.stVisionData.Recieve_Data8;
	
	VisionDataSendBuff.My_Data.Send_Data9	= VisionSendData9 = 1.0;//UPVisionDataReceiveBuff.stVisionData.Recieve_Data9;
	
	VisionDataSendBuff.My_Data.Send_Data10 = VisionSendData10 = 1.0;//UPVisionDataReceiveBuff.stVisionData.Recieve_Data10;

	VisionDataSendBuff.My_Data.Send_Data11 = VisionSendData11 = 1.0;//UPVisionDataReceiveBuff.stVisionData.Recieve_Data11;
	
	/* PROTOCOL DATA CODE END */

	DMA_ClearITPendingBit(USART6_TX_STREAM, DMA_IT_TCIF6);								//开启DMA_Mode_Normal,即便没有使用完成中断也要软件清除，否则只发一次
	
	DMA_Cmd(USART6_TX_STREAM, DISABLE);				       								//设置当前计数值前先禁用DMA
	USART6_TX_STREAM->M0AR = (uint32_t)&VisionDataSendBuff;							//设置当前待发数据基地址:Memory0 tARget
	USART6_TX_STREAM->NDTR = (uint32_t)VISION_SEND_DATA_LEN_DN;			        		//设置当前待发的数据的数量:Number of Data units to be TRansferred
	DMA_Cmd(USART6_TX_STREAM, ENABLE);	
}



///**
//  *	@brief	敌方血量评估
//  */
//float HP_Detect(void)
//{
//	UCHAR8 Hero_bit;
//	UCHAR8 Engi_bit;
//	UCHAR8 Stan1_bit;
//	UCHAR8 Stan2_bit;
//	UCHAR8 Stan3_bit;
//	
//	static u16 PreRobot1HP = 150;
//	static u32 Robot1ProtectTick = 0;
//	static u16 PreRobot2HP = 500;
//	static u32 Robot2ProtectTick = 0;
//	static u16 PreRobot3HP = 100;
//	static u32 Robot3ProtectTick = 0;
//	static u16 PreRobot4HP = 100;
//	static u32 Robot4ProtectTick = 0;
//	static u16 PreRobot5HP = 100;
//	static u32 Robot5ProtectTick = 0;
//	
//	if(RobotIDMessage == Red_7_Sentry)//红方ID 7	（识别蓝色）
//	{
//		/** 英雄无敌以及残血检测 */
//		if(PreRobot1HP == 0 && GameRobotHP.blue_1_robot_HP > 0)
//		{
//			Robot1ProtectTick 	= systemMonitor.SysTickTime;
//			Robot1ShootFlag 	= FALSE;
//			Hero_bit = 0;
//		}
//		else if(systemMonitor.SysTickTime - Robot1ProtectTick > 10000)
//		{
//			Robot1ShootFlag 	= TRUE;
//			if(GameRobotHP.blue_1_robot_HP <= Hero_HP_Threshold && GameRobotHP.blue_1_robot_HP != 0)
//			{	Hero_bit = 1;}
//			else
//			{	Hero_bit = 0;}
//		}
//		else
//		{	Hero_bit = 0;}
//		
//		/** 工程无敌以及残血检测 */
//		if(PreRobot2HP == 0 && GameRobotHP.blue_2_robot_HP > 0)
//		{
//			Robot2ProtectTick 	= systemMonitor.SysTickTime;
//			Robot2ShootFlag 	= FALSE;
//			Engi_bit = 0;
//		}
//		else if(systemMonitor.SysTickTime - Robot2ProtectTick > 10000)
//		{
//			Robot2ShootFlag 	= FALSE;
//			if(GameRobotHP.blue_2_robot_HP <= Engi_HP_Threshold && GameRobotHP.blue_2_robot_HP != 0)
//			{	Engi_bit = 2;}
//			else
//			{	Engi_bit = 0;}
//		}
//		else
//		{	Engi_bit = 0;}
//		
//		/** 步兵3无敌以及残血检测 */
//		if(PreRobot3HP == 0 && GameRobotHP.blue_3_robot_HP > 0)
//		{
//			Robot3ProtectTick 	= systemMonitor.SysTickTime;
//			Robot3ShootFlag 	= FALSE;
//			Stan1_bit = 0;
//		}
//		else if(systemMonitor.SysTickTime - Robot3ProtectTick > 10000)
//		{
//			Robot3ShootFlag 	= TRUE;
//			if(GameRobotHP.blue_3_robot_HP <= Stan_HP_Threshold && GameRobotHP.blue_3_robot_HP != 0)
//			{	Stan1_bit = 3;}
//			else
//			{	Stan1_bit = 0;}
//		}
//		else
//		{	Stan1_bit = 0;}
//		
//		
//		/** 步兵4无敌以及残血检测 */
//		if(PreRobot4HP == 0 && GameRobotHP.blue_4_robot_HP > 0)
//		{
//			Robot4ProtectTick 	= systemMonitor.SysTickTime;
//			Robot4ShootFlag 	= FALSE;
//			Stan2_bit = 0;
//		}
//		else if(systemMonitor.SysTickTime - Robot4ProtectTick > 10000)
//		{
//			Robot4ShootFlag 	= TRUE;
//			if(GameRobotHP.blue_4_robot_HP <= Stan_HP_Threshold && GameRobotHP.blue_4_robot_HP != 0)
//			{	Stan2_bit = 4;}
//			else
//			{	Stan2_bit = 0;}
//		}
//		else
//		{	Stan2_bit = 0;}
//		
//		/** 步兵5无敌以及残血检测 */
//		if(PreRobot5HP == 0 && GameRobotHP.blue_5_robot_HP > 0)
//		{
//			Robot5ProtectTick 	= systemMonitor.SysTickTime;
//			Robot5ShootFlag 	= FALSE;
//			Stan3_bit = 0;
//		}
//		else if(systemMonitor.SysTickTime - Robot5ProtectTick > 10000)
//		{
//			Robot5ShootFlag 	= TRUE;
//			if(GameRobotHP.blue_5_robot_HP <= Stan_HP_Threshold && GameRobotHP.blue_5_robot_HP != 0)
//			{	Stan3_bit = 5;}
//			else
//			{	Stan3_bit = 0;}
//		}
//		else
//		{	Stan3_bit = 0;}
//		
//		PreRobot1HP = GameRobotHP.blue_1_robot_HP;
//		PreRobot2HP = GameRobotHP.blue_2_robot_HP;
//		PreRobot3HP = GameRobotHP.blue_3_robot_HP;
//		PreRobot4HP = GameRobotHP.blue_4_robot_HP;
//		PreRobot5HP = GameRobotHP.blue_5_robot_HP;
//	}	
//	else if(RobotIDMessage == Blue_7_Sentry)//蓝方ID 17	（识别红色）
//	{
//		/** 英雄无敌以及残血检测 */
//		if(PreRobot1HP == 0 && GameRobotHP.red_1_robot_HP > 0)
//		{
//			Robot1ProtectTick 	= systemMonitor.SysTickTime;
//			Robot1ShootFlag 	= FALSE;
//			Hero_bit = 0;
//		}
//		else if(systemMonitor.SysTickTime - Robot1ProtectTick > 10000)
//		{
//			Robot1ShootFlag 	= TRUE;
//			if(GameRobotHP.red_1_robot_HP <= Hero_HP_Threshold && GameRobotHP.red_1_robot_HP != 0)
//			{	Hero_bit = 1;}
//			else
//			{	Hero_bit = 0;}
//		}
//		else
//		{	Hero_bit = 0;}
//		
//		/** 工程无敌以及残血检测 */
//		if(PreRobot2HP == 0 && GameRobotHP.red_2_robot_HP > 0)
//		{	
//			Robot2ProtectTick 	= systemMonitor.SysTickTime;
//			Robot2ShootFlag 	= FALSE;
//			Engi_bit = 0;
//		}
//		else if(systemMonitor.SysTickTime - Robot2ProtectTick > 10000)
//		{
//			Robot2ShootFlag 	= TRUE;
//			if(GameRobotHP.red_2_robot_HP <= Engi_HP_Threshold && GameRobotHP.red_2_robot_HP != 0)
//			{	Engi_bit = 2;}
//			else
//			{	Engi_bit = 0;}
//		}
//		else
//		{	Engi_bit = 0;}
//		/** 步兵3无敌以及残血检测 */
//		if(PreRobot3HP == 0 && GameRobotHP.red_3_robot_HP > 0)
//		{
//			Robot3ProtectTick 	= systemMonitor.SysTickTime;
//			Robot3ShootFlag 	= FALSE;
//			Stan1_bit = 0;
//		}
//		else if(systemMonitor.SysTickTime - Robot3ProtectTick > 10000)
//		{
//			Robot3ShootFlag 	= TRUE;
//			if(GameRobotHP.red_3_robot_HP <= Stan_HP_Threshold && GameRobotHP.red_3_robot_HP != 0)
//			{	Stan1_bit = 5;}
//			else
//			{	Stan1_bit = 0;}	
//		}
//		else
//		{	Stan1_bit = 0;}	
//		
//		/** 步兵4无敌以及残血检测 */
//		if(PreRobot4HP == 0 && GameRobotHP.red_4_robot_HP > 0)
//		{
//			Robot4ProtectTick 	= systemMonitor.SysTickTime;
//			Robot4ShootFlag 	= FALSE;
//			Stan2_bit = 0;
//		}
//		else if(systemMonitor.SysTickTime - Robot4ProtectTick > 10000)
//		{
//			Robot4ShootFlag 	= TRUE;
//			if(GameRobotHP.red_4_robot_HP <= Stan_HP_Threshold && GameRobotHP.red_4_robot_HP != 0)
//			{	Stan2_bit = 4;}
//			else
//			{	Stan2_bit = 0;}	
//		}
//		else
//		{	Stan2_bit = 0;}	
//		
//		/** 步兵5无敌以及残血检测 */
//		if(PreRobot5HP == 0 && GameRobotHP.red_5_robot_HP > 0)
//		{
//			Robot5ProtectTick 	= systemMonitor.SysTickTime;
//			Robot5ShootFlag 	= FALSE;
//			Stan3_bit = 0;
//		}
//		else if(systemMonitor.SysTickTime - Robot5ProtectTick > 10000)
//		{
//			Robot5ShootFlag 	= TRUE;
//			if(GameRobotHP.red_5_robot_HP <= Stan_HP_Threshold && GameRobotHP.red_5_robot_HP != 0)
//			{	Stan3_bit = 5;}
//			else
//			{	Stan3_bit = 0;}	
//		}
//		else
//		{	Stan3_bit = 0;}		
//		
//		PreRobot1HP = GameRobotHP.red_1_robot_HP;
//		PreRobot2HP = GameRobotHP.red_2_robot_HP;
//		PreRobot3HP = GameRobotHP.red_3_robot_HP;
//		PreRobot4HP = GameRobotHP.red_4_robot_HP;
//		PreRobot5HP = GameRobotHP.red_5_robot_HP;
//	}
//	
//	return (float)(Hero_bit*10000 + Engi_bit*1000 + Stan1_bit*100 + Stan2_bit*10 + Stan3_bit*1);
//}


