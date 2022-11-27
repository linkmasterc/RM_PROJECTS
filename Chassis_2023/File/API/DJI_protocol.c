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


