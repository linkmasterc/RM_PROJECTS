#include "stm32f4xx_it.h"

/** --------------------------------------------------------------------------
  * @brief  系统时钟中断服务函数
			
  * @note	1ms进入一次中断，在中断中对计数值进行加1以对程序运行时间进行计时
			每过1s统计各任务的执行次数以得到每个任务执行的频率
 -------------------------------------------------------------------------- **/
void SysTick_Handler(void)
{
	systemMonitor.SysTickTime++;
	if(systemMonitor.SysTickTime % 1000 == 0)		// 用1s内的次数计算帧率
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
		
		/** IT Monitor */
		DETECT_MONITOR(USART1_rx);					// 进入中断帧率检测
		DETECT_MONITOR(USART2_rx);
		DETECT_MONITOR(USART3_rx);
		DETECT_MONITOR(UART4_rx);
		DETECT_MONITOR(UART5_rx);
		DETECT_MONITOR(USART6_rx);
		DETECT_MONITOR(USART1_rx_valid);			// 有效接收帧率检测
		DETECT_MONITOR(USART2_rx_valid);
		DETECT_MONITOR(USART3_rx_valid);
		DETECT_MONITOR(UART4_rx_valid);
		DETECT_MONITOR(UART5_rx_valid);
		DETECT_MONITOR(USART6_rx_valid);
		DETECT_MONITOR(CAN1_rx);
		DETECT_MONITOR(CAN2_rx);
		
//		/** can1帧率检测 */
//		DETECT_SUB_MONITOR(CAN1_CapacitorMode_rx);
//		DETECT_SUB_MONITOR(CAN1_PowerBoard_rx);
//		DETECT_SUB_MONITOR(CAN1_SlaveMotor_rx);
//		DETECT_SUB_MONITOR(CAN1_ChassisMotor_rx);
//		DETECT_SUB_MONITOR(CAN2_DNYawMotor_rx);
		
		/** can2帧率检测 */
//		DETECT_SUB_MONITOR(CAN2_ChassisZigMotor_rx);
//		DETECT_SUB_MONITOR(CAN2_UPShootMotor_rx);
//		DETECT_SUB_MONITOR(CAN2_UPYawMotor_rx);
		
		/** 裁判系统帧率检测 */
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
  * @brief  串口1中断服务函数
  
  * @retval None
  
  * @param	None
			
  * @note	串口1用于和遥控器进行通信
			使用空闲中断+DMA对数据进行处理
			先读SR再读DR清除IDLE中断标志位
 -------------------------------------------------------------------------- **/
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
	{
		USART1->SR;																
		USART1->DR;
		
		if(DMA_GetCurrDataCounter(USART1_RX_STREAM) == USART1_RX_BUF_LEN)				// 验证一次接收结束时DMA中接收到的数据是否完整
		{
			RCProtocol(USART1_Cushioning_Rx);												// 按协议对接收到的遥控器通讯数据进行处理
			systemMonitor.USART1_rx_cnt++;											// 对接收次数进行计数（计数周期为1s）
		}
		else
		{
			DMA_Cmd(USART1_RX_STREAM, DISABLE);
//			USART1_RX_STREAM->M0AR = (u32)(&USART1_Cushioning_Rx);
			USART1_RX_STREAM->NDTR = USART1_RX_BUF_LEN;								// DMA清空为下一次接收做准备
			DMA_Cmd(USART1_RX_STREAM, ENABLE);
		}
	}
}

/** --------------------------------------------------------------------------
  * @brief  串口2中断服务函数
  
  * @retval None
  
  * @param	None
			
  * @note	串口2用于接收裁判系统发送的数据帧（包含指令和数据）
			使用空闲中断+DMA对数据进行处理
			先读SR再读DR清除IDLE中断标志位
 -------------------------------------------------------------------------- **/
void USART2_IRQHandler(void)
{
  if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
  {
		USART_ClearITPendingBit(USART2, USART_IT_IDLE);													// 清除中断标志位
		USART2->SR;																					
		USART2->DR;   	  	
		RSProtocol(&USART2_Rcr, RSReceiveDataBuff, USART_Receive(&USART2_Rcr));

//		#ifdef USING_HMI
//		USART_HMI_Decode();
	  
		if(systemMonitor.USART2_rx_valid_fps == 0)
		{
			DMA_Cmd(USART2_RX_STREAM, DISABLE);																// DMA清空为下一次接收做准备
			USART2_RX_STREAM->NDTR = USART2_RX_DMA_BUF_LEN;
			DMA_Cmd(USART2_RX_STREAM, ENABLE);
		}
//		#endif

	  	systemMonitor.USART2_rx_cnt++;																	// 对接收次数进行计数（计数周期为1s）
	}	
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

/** --------------------------------------------------------------------------
  * @brief  TIM8输入捕获
			
  * @note		TIM8分频后计数频率为1us
						重装载周期为1ms
 -------------------------------------------------------------------------- **/
void TIM8_CC_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM8,TIM8_CC_IRQn)!=RESET)
	{
		TIM_ClearITPendingBit(TIM8,TIM8_CC_IRQn);
		if(!(TIM8->CCER&0x0002))															//此位为0是上升沿触发
		{
			TIM8_Falling_ARR=0;
			TIM8->CNT=0;																				//清空一下计数器，使计数器从0开始计数
			TIM8_OverflowTimes=0;																//捕获上升沿后便重新记录计数器溢出次数
			
			TIM_Cmd(TIM8,DISABLE);
			TIM_OC3PolarityConfig(TIM8,TIM_ICPolarity_Falling); //此处使用输出比较模式的函数没有问题，可以配置边沿触发模式(因为输入捕获没有单独的配置函数)
			TIM_Cmd(TIM8,ENABLE);
		}
		if(TIM8->CCER&0x0002)																	//此位为1是下降沿触发
		{
			TIM8_Falling_ARR=TIM_GetCapture3(TIM8);
			US_Distance=(TIM8_OverflowTimes*1+TIM8_Falling_ARR*0.001)*0.34/2.0;//计算与障碍物的距离
			
			TIM_Cmd(TIM8,DISABLE);
			TIM_OC3PolarityConfig(TIM8,TIM_ICPolarity_Rising);
			TIM_Cmd(TIM8,ENABLE);
		}
		
	}
}

void TIM8_UP_TIM13_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM8,TIM8_UP_TIM13_IRQn)!=RESET)
	{
		TIM_ClearITPendingBit(TIM8,TIM8_UP_TIM13_IRQn);
		
		TIM8_OverflowTimes++;
		
	}
}




