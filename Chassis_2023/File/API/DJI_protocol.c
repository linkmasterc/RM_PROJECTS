#include "DJI_protocol.h"


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


