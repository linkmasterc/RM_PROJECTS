#include "stm32f4xx_it.h"

/** --------------------------------------------------------------------------
  * @brief  ϵͳʱ���жϷ�����
			
  * @note	1ms����һ���жϣ����ж��жԼ���ֵ���м�1�ԶԳ�������ʱ����м�ʱ
			ÿ��1sͳ�Ƹ������ִ�д����Եõ�ÿ������ִ�е�Ƶ��
 -------------------------------------------------------------------------- **/
void SysTick_Handler(void)
{
	systemMonitor.SysTickTime++;
	if(systemMonitor.SysTickTime % 1000 == 0)		// ��1s�ڵĴ�������֡��
	{
		/** Main Task Monitor */
		DETECT_MONITOR(SystemMonitorTask);
		DETECT_MONITOR(ModeChooseTask);
		DETECT_MONITOR(GimbalTask);
		DETECT_MONITOR(ShootTask);
		DETECT_MONITOR(ChassisTask);
		DETECT_MONITOR(MotorDataSendTask);
		DETECT_MONITOR(GimbalDataSendTask);
		DETECT_MONITOR(VisionDataSendTask);
		DETECT_MONITOR(CapacitorTask);
		DETECT_MONITOR(StateDataSendTask);
		DETECT_MONITOR(LEDStateChangeTask);
		DETECT_MONITOR(TriggerTask);
		
		/** IT Monitor */
		DETECT_MONITOR(USART1_rx);					// �����ж�֡�ʼ��
		DETECT_MONITOR(USART2_rx);
		DETECT_MONITOR(USART3_rx);
		DETECT_MONITOR(UART4_rx);
		DETECT_MONITOR(UART5_rx);
		DETECT_MONITOR(USART6_rx);
		DETECT_MONITOR(USART1_rx_valid);			// ��Ч����֡�ʼ��
		DETECT_MONITOR(USART2_rx_valid);
		DETECT_MONITOR(USART3_rx_valid);
		DETECT_MONITOR(UART4_rx_valid);
		DETECT_MONITOR(UART5_rx_valid);
		DETECT_MONITOR(USART6_rx_valid);
		DETECT_MONITOR(CAN1_rx);
		DETECT_MONITOR(CAN2_rx);
		DETECT_MONITOR(CAN1_BODAN_RX);
		DETECT_MONITOR(CAN1_SERVO1_RX);
		DETECT_MONITOR(CAN2_SERVO2_RX);
		DETECT_MONITOR(CAN2_SERVO3_RX);
		DETECT_MONITOR(CAN1_SERVO4_RX);
		DETECT_MONITOR(CAN2_SPEED1_RX);
		DETECT_MONITOR(CAN1_SPEED2_RX);
		DETECT_MONITOR(CAN1_SPEED3_RX);
		DETECT_MONITOR(CAN2_SPEED4_RX);
		DETECT_MONITOR(CAN2_YAW_RX);
//		/** can1֡�ʼ�� */
//		DETECT_SUB_MONITOR(CAN1_CapacitorMode_rx);
//		DETECT_SUB_MONITOR(CAN1_PowerBoard_rx);
//		DETECT_SUB_MONITOR(CAN1_SlaveMotor_rx);
//		DETECT_SUB_MONITOR(CAN1_ChassisMotor_rx);
//		DETECT_SUB_MONITOR(CAN2_DNYawMotor_rx);
		
		/** can2֡�ʼ�� */
//		DETECT_SUB_MONITOR(CAN2_ChassisZigMotor_rx);
//		DETECT_SUB_MONITOR(CAN2_UPShootMotor_rx);
//		DETECT_SUB_MONITOR(CAN2_UPYawMotor_rx);
		
		/** ����ϵͳ֡�ʼ�� */
		DETECT_RS_MONITOR(GameStatus);
		DETECT_RS_MONITOR(GameRobotHP);
		DETECT_RS_MONITOR(DartStatus);
		DETECT_RS_MONITOR(EventData);
		DETECT_RS_MONITOR(RefereeWarning);
		DETECT_RS_MONITOR(DartRemainingTime);
		DETECT_RS_MONITOR(GameRobotStatus);
		DETECT_RS_MONITOR(PowerHeatData);
		DETECT_RS_MONITOR(GameRobotPos);
		DETECT_RS_MONITOR(Buff);
		DETECT_RS_MONITOR(RobotHurt);
		DETECT_RS_MONITOR(ShootData);
		DETECT_RS_MONITOR(BulletRemaining);
		DETECT_RS_MONITOR(RobotInteractiveData);
	}

}

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
  * @brief  ����1�жϷ�����
  
  * @retval None
  
  * @param	None
			
  * @note	����1���ں�ң��������ͨ��
			ʹ�ÿ����ж�+DMA�����ݽ��д���
			�ȶ�SR�ٶ�DR���IDLE�жϱ�־λ
 -------------------------------------------------------------------------- **/
u32 DMA_RAW_LEN=0;
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
	{
		USART1->SR;																
		USART1->DR;
		DMA_RAW_LEN=DMA_GetCurrDataCounter(USART1_RX_STREAM);
		if(DMA_GetCurrDataCounter(USART1_RX_STREAM) == USART1_RX_BUF_LEN)				// ��֤һ�ν��ս���ʱDMA�н��յ��������Ƿ�����
		{
			RCProtocol(USART1_Cushioning_Rx);												// ��Э��Խ��յ���ң����ͨѶ���ݽ��д���
			systemMonitor.USART1_rx_cnt++;											// �Խ��մ������м�������������Ϊ1s��
		}
		else
		{
			DMA_Cmd(USART1_RX_STREAM, DISABLE);
//			USART1_RX_STREAM->M0AR = (u32)(&USART1_Cushioning_Rx);
			USART1_RX_STREAM->NDTR = USART1_RX_BUF_LEN;								// DMA���Ϊ��һ�ν�����׼��
			DMA_Cmd(USART1_RX_STREAM, ENABLE);
		}
	}
}

/** --------------------------------------------------------------------------
  * @brief  ����2�жϷ�����
  
  * @retval None
  
  * @param	None
			
  * @note	����2���ڽ��ղ���ϵͳ���͵�����֡������ָ������ݣ�
			ʹ�ÿ����ж�+DMA�����ݽ��д���
			�ȶ�SR�ٶ�DR���IDLE�жϱ�־λ
 -------------------------------------------------------------------------- **/
void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
  {
		USART_ClearITPendingBit(USART2, USART_IT_IDLE);													// ����жϱ�־λ
		USART2->SR;																					
		USART2->DR;   	  	
		RSProtocol(&USART2_Rcr, RSReceiveDataBuff, USART_Receive(&USART2_Rcr));

//		#ifdef USING_HMI
//		USART_HMI_Decode();
	  
		if(systemMonitor.USART2_rx_valid_fps == 0)
		{
			DMA_Cmd(USART2_RX_STREAM, DISABLE);																// DMA���Ϊ��һ�ν�����׼��
			USART2_RX_STREAM->NDTR = USART2_RX_DMA_BUF_LEN;
			DMA_Cmd(USART2_RX_STREAM, ENABLE);
		}
//		#endif
	  systemMonitor.USART2_rx_cnt++;																	// �Խ��մ������м�������������Ϊ1s��
	}	
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
//	if(USART_GetITStatus(USART6, USART_IT_IDLE) == SET)
//	{
//		USART6->SR;
//		USART6->DR;
//		


//		
//		DMA_GetCurrDataCounter(USART6_RX_STREAM);							// ��֤һ�ν��ս���ʱDMA�н��յ��������Ƿ�����
//				DMA_Cmd(USART6_RX_STREAM, DISABLE);													// DMA���Ϊ��һ�ν�����׼��
//			USART6_RX_STREAM->NDTR = UA6RxDMAbuf_LEN;
//			DMA_Cmd(USART6_RX_STREAM, ENABLE);
//		systemMonitor.USART6_rx_cnt++;
//	}	
	
	if(USART_GetITStatus(USART6, USART_IT_IDLE) == SET)
	{
		USART6->SR;
		USART6->DR;


		if(DMA_GetCurrDataCounter(USART6_RX_STREAM) == 38)							// ��֤һ�ν��ս���ʱDMA�н��յ��������Ƿ�����
		{
			//USART_Receive(&USART6_Rcr);																// ��ȡ���ڵ�DMA�н������ݵĳ���(��δ���ã� 
//			DNVisionDataProtocol();															// ��Э��Խ��յ������ƿ�ͨѶ���ݽ��д���
			if(UA6RxDMAbuf[0]==0x55&&UA6RxDMAbuf[1]==00)
			{
				memcpy(&VisionDataReceiveBuff,(u8*)(&UA6RxDMAbuf)+2,1);
				memcpy((u8*)(&VisionDataReceiveBuff)+1,(u8*)(&UA6RxDMAbuf)+4,32);
				systemMonitor.USART6_rx_valid_cnt++;
			}
			systemMonitor.USART6_rx_cnt++;														// �Խ��մ������м�������������Ϊ1s��
		}
		else
		{
			DMA_Cmd(USART6_RX_STREAM, DISABLE);													// DMA���Ϊ��һ�ν�����׼��
			USART6_RX_STREAM->NDTR = 38;
			DMA_Cmd(USART6_RX_STREAM, ENABLE);
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

/** --------------------------------------------------------------------------
  * @brief  TIM9���벶��
			
  * @note		TIM9��Ƶ�����Ƶ��Ϊ1us
						��װ������Ϊ1ms
 -------------------------------------------------------------------------- **/
float BOX[10];
float preUS_Distance;
void TIM1_BRK_TIM9_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM9,TIM_IT_CC1)!=RESET)
	{
		TIM_ClearITPendingBit(TIM9,TIM_IT_CC1);
		if(!(TIM9->CCER&0x0002))															//��λΪ0�������ش���
		{
			TIM9_Falling_ARR=0;
			TIM9->CNT=0;																				//���һ�¼�������ʹ��������0��ʼ����
			TIM9_OverflowTimes=0;																//���������غ�����¼�¼�������������
			
			TIM_Cmd(TIM9,DISABLE);
			TIM_OC1PolarityConfig(TIM9,TIM_ICPolarity_Falling); //�˴�ʹ������Ƚ�ģʽ�ĺ���û�����⣬�������ñ��ش���ģʽ(��Ϊ���벶��û�е��������ú���)
			TIM_Cmd(TIM9,ENABLE);
		}
		else if(TIM9->CCER&0x0002)																	//��λΪ1���½��ش���
		{
			TIM9_Falling_ARR=TIM_GetCapture1(TIM9);
			US_Distance=(TIM9_OverflowTimes*1+TIM9_Falling_ARR*0.001)*0.34/2.0;//�������ϰ���ľ���
			preUS_Distance=US_Distance;
			preUS_Distance=Clip(preUS_Distance,0,4.5);
			if(US_Distance>4.5)
				US_Distance=preUS_Distance;
			if(fabs(preUS_Distance-US_Distance)>0.3)
				US_Distance=preUS_Distance;
			US_Distance=DataSmoothFilter(BOX,10,US_Distance);
			US_Distance*=1000;
			TIM_Cmd(TIM9,DISABLE);
			TIM_OC1PolarityConfig(TIM9,TIM_ICPolarity_Rising);
			TIM_Cmd(TIM9,ENABLE);
		}
		
	}
	if(TIM_GetITStatus(TIM9,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
		
		TIM9_OverflowTimes++;
	}
}

//void TIM8_UP_TIM13_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM8,TIM8_UP_TIM13_IRQn)!=RESET)
//	{
//		TIM_ClearITPendingBit(TIM8,TIM8_UP_TIM13_IRQn);
//		
//		TIM8_OverflowTimes++;
//		
//	}
//}




