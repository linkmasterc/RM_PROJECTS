#include "stm32f4xx_it.h"


/** --------------------------------------------------------------------------
  * @brief  获取串口的DMA中接收数据的长度
  
  * @retval 接收数据的长度
  
  * @param	串口结构体变量
			
  * @note	
 -------------------------------------------------------------------------- **/
u16 USART_Receive(USART_RX_TypeDef* USARTx)
{
	USARTx->rxConter = USARTx->DMALen - DMA_GetCurrDataCounter(USARTx->DMAy_Streamx);				// 本次DMA缓冲区填充到的位置
	USARTx->rxBufferPtr += USARTx->rxSize;															// 上次DMA缓冲区填充到的位置
	
	if(USARTx->rxBufferPtr >= USARTx->DMALen)														// 说明DMA缓冲区已经满了一次
	{
		USARTx->rxBufferPtr %= USARTx->DMALen;
	}
	
	if(USARTx->rxBufferPtr < USARTx->rxConter)
	{
		USARTx->rxSize = USARTx->rxConter - USARTx->rxBufferPtr; 									// 计算本次接收数据长度
		
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
		USARTx->rxSize = USARTx->rxConter + USARTx->DMALen - USARTx->rxBufferPtr;					// 计算本次接收数据长度
		
		if(USARTx->rxSize <= USARTx->MBLen)															// 接收的数据长度与期望数据长度相同，则把本数据写进邮箱
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
  * @brief  串口3中断服务函数
  
  * @retval None
  
  * @param	None
			
  * @note	串口3用于接收下云控发送的数据
			使用空闲中断+DMA对数据进行处理
			先读SR再读DR清除IDLE中断标志位
 -------------------------------------------------------------------------- **/
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_IDLE) == SET)
	{
		USART3->SR;
		USART3->DR;
		
		if(DMA_GetCurrDataCounter(USART3_RX_STREAM) == ChassisBufLen_Rx)							// 验证一次接收结束时DMA中接收到的数据是否完整
		{
			GimbalReceiveDataProtocol();																// 按协议对接收到的下云控通讯数据进行处理
			systemMonitor.USART3_rx_cnt++;														// 对接收次数进行计数（计数周期为1s）
		}
		else
		{
			DMA_Cmd(USART3_RX_STREAM, DISABLE);													// DMA清空为下一次接收做准备
			USART3_RX_STREAM->NDTR = ChassisBufLen_Rx;
			DMA_Cmd(USART3_RX_STREAM, ENABLE);
		}
	}
}

/** --------------------------------------------------------------------------
  * @brief  串口6中断服务函数
  
  * @retval None
  
  * @param	None
			
  * @note	串口6用于接收下云台小电脑发送的数据
			使用空闲中断+DMA对数据进行处理
			先读SR再读DR清除IDLE中断标志位
 -------------------------------------------------------------------------- **/
void USART6_IRQHandler(void)
{
	if(USART_GetITStatus(USART6, USART_IT_IDLE) == SET)
	{
		USART6->SR;
		USART6->DR;
		
		USART_Receive(&USART6_Rcr);																// 获取串口的DMA中接收数据的长度(暂未利用） 
		DNVisionDataProtocol();
		systemMonitor.USART6_rx_cnt++;
	}	
}

/** --------------------------------------------------------------------------
  * @brief  CAN1接收中断
  
  * @retval 
  
  * @param	
			
  * @note	
 -------------------------------------------------------------------------- **/
void CAN1_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)										// 接收挂起中断（相当于空闲中断）
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);		
		CAN_Receive(CAN1, CAN_FIFO0, &CAN1_RX_Message);										// 从FIFO中提取CAN接收的数据
		CAN1_Protocol();
		systemMonitor.CAN1_rx_cnt++;

	}
}

/** --------------------------------------------------------------------------
  * @brief  CAN2接收中断
			
  * @note	
 -------------------------------------------------------------------------- **/
void CAN2_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)										// 接收挂起中断（相当于空闲中断）
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
		CAN_Receive(CAN2, CAN_FIFO0, &CAN2_RX_Message);
		CAN2_Protocol();
		systemMonitor.CAN2_rx_cnt++;
	}
}

