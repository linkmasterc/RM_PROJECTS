#include "stm32f4xx_it.h"
#include "verify.h"
#include "can_protocol.h"
void SysTick_Handler(void)
{	
	systemMonitor.SysTickTime++;
	if(systemMonitor.SysTickTime%1000==0)
	{
		DETECT_MONITOR(IMUSampleTask);
		DETECT_MONITOR(IMUUpdateTask);
		DETECT_MONITOR(GimbalTask);
		DETECT_MONITOR(DataSendTask);
		DETECT_MONITOR(USART3_rx);
		DETECT_MONITOR(CAN1_rx);
		DETECT_MONITOR(CAN2_rx);
	}
}

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_IDLE) == SET)
	{
		USART3->SR;
		USART3->DR;
		if(DMA_GetCurrDataCounter(USART3_RX_STREAM) == ChassisBufLen_Rx)							// 验证一次接收结束时DMA中接收到的数据是否完整
		{
			GimbalReceiveDataProtocol();
			systemMonitor.USART3_rx_cnt++;
		}
		else
		{
			DMA_Cmd(USART3_RX_STREAM, DISABLE);
			USART3_RX_STREAM->NDTR = USART3_RX_BUF_LEN;
			DMA_Cmd(USART3_RX_STREAM, ENABLE);
		}
	}
}

void CAN1_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);		
		CAN_Receive(CAN1, CAN_FIFO0, &CAN1_RX_Message);
		CAN1_Protocol();
		systemMonitor.CAN1_rx_cnt++;
	}
}
void CAN2_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
		CAN_Receive(CAN2, CAN_FIFO0, &CAN2_RX_Message);
		CAN2_Protocol();
		systemMonitor.CAN2_rx_cnt++;
	}
}

