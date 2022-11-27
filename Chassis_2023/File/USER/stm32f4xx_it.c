#include "stm32f4xx_it.h"

/** --------------------------------------------------------------------------
  * @brief  ����3�жϷ�����
  
  * @retval None
  
  * @param	None
			
  * @note	����3���ڽ������ƿط��͵�����
			ʹ�ÿ����ж�+DMA�����ݽ��д���
			�ȶ�SR�ٶ�DR���IDLE�жϱ�־λ
 -------------------------------------------------------------------------- **/
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_IDLE) == SET)
	{
		USART3->SR;
		USART3->DR;
		
		if(DMA_GetCurrDataCounter(USART3_RX_STREAM) == ChassisBufLen_Rx)							// ��֤һ�ν��ս���ʱDMA�н��յ��������Ƿ�����
		{
			GimbalReceiveDataProtocol();																// ��Э��Խ��յ������ƿ�ͨѶ���ݽ��д���
			systemMonitor.USART3_rx_cnt++;														// �Խ��մ������м�������������Ϊ1s��
		}
		else
		{
			DMA_Cmd(USART3_RX_STREAM, DISABLE);													// DMA���Ϊ��һ�ν�����׼��
			USART3_RX_STREAM->NDTR = ChassisBufLen_Rx;
			DMA_Cmd(USART3_RX_STREAM, ENABLE);
		}
	}
}


/** --------------------------------------------------------------------------
  * @brief  CAN1�����ж�
  
  * @retval 
  
  * @param	
			
  * @note	
 -------------------------------------------------------------------------- **/
void CAN1_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)										// ���չ����жϣ��൱�ڿ����жϣ�
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);		
		CAN_Receive(CAN1, CAN_FIFO0, &CAN1_RX_Message);										// ��FIFO����ȡCAN���յ�����
		CAN1_Protocol();
		systemMonitor.CAN1_rx_cnt++;

	}
}

/** --------------------------------------------------------------------------
  * @brief  CAN2�����ж�
			
  * @note	
 -------------------------------------------------------------------------- **/
void CAN2_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)										// ���չ����жϣ��൱�ڿ����жϣ�
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
		CAN_Receive(CAN2, CAN_FIFO0, &CAN2_RX_Message);
		CAN2_Protocol();
		systemMonitor.CAN2_rx_cnt++;
	}
}

