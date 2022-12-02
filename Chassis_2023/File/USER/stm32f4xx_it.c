#include "stm32f4xx_it.h"


/** --------------------------------------------------------------------------
  * @brief  ��ȡ���ڵ�DMA�н������ݵĳ���
  
  * @retval �������ݵĳ���
  
  * @param	���ڽṹ�����
			
  * @note	
 -------------------------------------------------------------------------- **/
u16 USART_Receive(USART_RX_TypeDef* USARTx)
{
	USARTx->rxConter = USARTx->DMALen - DMA_GetCurrDataCounter(USARTx->DMAy_Streamx);				// ����DMA��������䵽��λ��
	USARTx->rxBufferPtr += USARTx->rxSize;															// �ϴ�DMA��������䵽��λ��
	
	if(USARTx->rxBufferPtr >= USARTx->DMALen)														// ˵��DMA�������Ѿ�����һ��
	{
		USARTx->rxBufferPtr %= USARTx->DMALen;
	}
	
	if(USARTx->rxBufferPtr < USARTx->rxConter)
	{
		USARTx->rxSize = USARTx->rxConter - USARTx->rxBufferPtr; 									// ���㱾�ν������ݳ���
		
		if(USARTx->rxSize <= USARTx->MBLen)
		{
			for(u16 i=0;i<USARTx->rxSize;i++)
			{
				*(USARTx->pMailbox + i) = *(USARTx->pDMAbuf + USARTx->rxBufferPtr + i);
			}
		}
	}
	else
	{
		USARTx->rxSize = USARTx->rxConter + USARTx->DMALen - USARTx->rxBufferPtr;					// ���㱾�ν������ݳ���
		
		if(USARTx->rxSize <= USARTx->MBLen)															// ���յ����ݳ������������ݳ�����ͬ����ѱ�����д������
		{
			for(u16 i=0; i<USARTx->rxSize-USARTx->rxConter; i++)
			{
				*(USARTx->pMailbox + i) = *(USARTx->pDMAbuf + USARTx->rxBufferPtr + i);
			}
			for(u16 i=0; i<USARTx->rxConter; i++)
			{
				*(USARTx->pMailbox + USARTx->rxSize - USARTx->rxConter + i) = *(USARTx->pDMAbuf + i);
			}
		}
	}
	return USARTx->rxSize;
}


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
  * @brief  ����6�жϷ�����
  
  * @retval None
  
  * @param	None
			
  * @note	����6���ڽ�������̨С���Է��͵�����
			ʹ�ÿ����ж�+DMA�����ݽ��д���
			�ȶ�SR�ٶ�DR���IDLE�жϱ�־λ
 -------------------------------------------------------------------------- **/
void USART6_IRQHandler(void)
{
	if(USART_GetITStatus(USART6, USART_IT_IDLE) == SET)
	{
		USART6->SR;
		USART6->DR;
		
		USART_Receive(&USART6_Rcr);																// ��ȡ���ڵ�DMA�н������ݵĳ���(��δ���ã� 
		DNVisionDataProtocol();
		systemMonitor.USART6_rx_cnt++;
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

