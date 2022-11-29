#include "stm32f4xx_it.h"
//#include "rm_data_types.h"
//#include "main.h"
//#include "string.h"
//USHORT16 Clear_IT = 0;
///*************************************************************************
//�жϴ��������ƣ�SysTick_Handler
//�жϲ������ƣ�1ms����һ���жϡ�
//�������ܣ���SysTick��ϵͳ�δ�ʱ������Ϊ����ϵͳ��ʱ��
//*************************************************************************/
//extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{	
	system_monitor.SysTickTime++;
}

///*************************************************************************
//�жϴ��������ƣ�USART1_IRQHandler
//�жϲ������ƣ�DR16ÿ��14msͨ��DBus����һ֡���ݣ�18�ֽڣ�,USART1ÿ����һ֡����
//             ����һ���ж�
//�������ܣ�ң�����������ݽ���
//*************************************************************************/
//extern USART_RX_TypeDef USART1_Rcr;
//extern EventGroupHandle_t IRQHandler;
//void USART1_IRQHandler(void)
//{
//	BaseType_t Result,xHigherPriorityTaskWoken;
//  if(USART_GetITStatus(USART1, USART_IT_IDLE)!= RESET)
//	{
//		Clear_IT = USART1->SR;
//		Clear_IT = USART1->DR;//�ȶ�SR���DR����жϱ�־λ
//		
//    if(USART_Receive(&USART1_Rcr)==USART1_RXMB_LEN)
//	  {
//			 if(IRQHandler!=NULL)		
//		 {
//			Result = xEventGroupSetBitsFromISR(IRQHandler,0x01,&xHigherPriorityTaskWoken);
//			if(Result!=pdFAIL)
//		  {
//				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//		  }
//		}
//			system_monitor.USART1_cnt++;
//			system_monitor.USART1_cnt_100ms++;
//		} 
//	}		
//}

///*************************************************************************
//�жϴ��������ƣ�USART2_IRQHandler
//�жϲ������ƣ�USART2���յ�һ�����ֽں󴥷��ж�
//�������ܣ�����ϵͳͨѶ
//*************************************************************************/
//void USART2_IRQHandler(void)
//{ 	
//	BaseType_t Result,xHigherPriorityTaskWoken;
//  if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
//  {   		 	
//		Clear_IT = USART2->SR;
//		Clear_IT = USART2->DR;                   //�ȶ�SR���DR����жϱ�־λ
//    
//     USART_Receive(&USART2_Rcr);	
//     if(IRQHandler!=NULL)		
//		 {
//	     Result = xEventGroupSetBitsFromISR(IRQHandler,0x02,&xHigherPriorityTaskWoken);
//		   if(Result!=pdFAIL)
//		   {
//			   portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//		   }
//		 }
//		   system_monitor.USART2rx_cnt++;
//	} 
//}

///***********************************************************************************
//�жϴ��������ƣ�DMA1_Stream6_IRQHandler
//�жϲ������ƣ�����2��������ж�
//�������ܣ�
//************************************************************************************/
//void DMA1_Stream6_IRQHandler(void)
//{
//    if(DMA_GetITStatus(DMA1_Stream6, DMA_IT_TCIF6))
//    {
//        DMA_ClearFlag(DMA1_Stream6, DMA_FLAG_TCIF6);//�����־λ
//    	  DMA_Cmd(DMA1_Stream6, DISABLE);             //�ر�DMA���� 
//    }
//}
///***********************************************************************************
//�жϴ��������ƣ�UART4_IRQHandler
//�жϲ������ƣ��Ӿ�ͨѶ
//�������ܣ�
//************************************************************************************/
//void UART4_IRQHandler(void)
//{
//	BaseType_t Result,xHigherPriorityTaskWoken;
//	if(USART_GetITStatus(UART4, USART_IT_IDLE)!= RESET)
//	{
//		Clear_IT = UART4->SR;
//		Clear_IT = UART4->DR;//�ȶ�SR���DR����жϱ�־λ
//		
//		if(USART_Receive(&UART4_Rcr)==UART4_RXMB_LEN)
//		{
//			 if(IRQHandler!=NULL)		
//		 {
//			Result = xEventGroupSetBitsFromISR(IRQHandler,0x03,&xHigherPriorityTaskWoken);
//		  if(Result!=pdFAIL)
//		  {
//				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//		  }	
//		 }
//     system_monitor.USART4rx_cnt++;		 
//		}		
//	}
//}

//extern USART_RX_TypeDef USART6_Rcr;
//void USART6_IRQHandler(void)
//{
//	BaseType_t Result,xHigherPriorityTaskWoken;
//	if(USART_GetITStatus(USART6, USART_IT_IDLE)!= RESET)
//	{
//		Clear_IT = USART6->SR;
//		Clear_IT = USART6->DR;//�ȶ�SR���DR����жϱ�־λ
//		
//		if(USART_Receive(&USART6_Rcr)==USART6_RXMB_LEN)
//		{
//		 if(IRQHandler!=NULL)		
//		 {
//			Result = xEventGroupSetBitsFromISR(IRQHandler,0x04,&xHigherPriorityTaskWoken);
//		  if(Result!=pdFAIL)
//		  {
//				portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//		  }	
//		 }	 
//	   system_monitor.USART6rx_cnt++; 	
//	 }		 
//	}
//}
///*************************************************************************
//�жϴ��������ƣ�void TIM5_IRQHandler(void)
//�жϲ������ƣ�
//�������ܣ��ṩ4000֡֡��
//*************************************************************************/
//extern TaskHandle_t FWheelSampleTask_Handler; 
//void TIM1_UP_TIM10_IRQHandler(void)
//{
//	BaseType_t Result,xHigherPriorityTaskWoken;
//  if(TIM_GetITStatus(TIM10,TIM_IT_Update) != RESET) 
//	{
//		TIM_ClearITPendingBit(TIM10,TIM_IT_Update);
//		if(FWheelSampleTask_Handler!=NULL)
//		{
//		  vTaskNotifyGiveFromISR(FWheelSampleTask_Handler,&xHigherPriorityTaskWoken);
//	   	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//		}
//	}
//}
///*************************************************************************
//�жϴ��������ƣ�void TIM1_BRK_TIM9_IRQHandler(void)
//�жϲ������ƣ�
//�������ܣ��ṩ4000֡֡��
//*************************************************************************/
//extern TaskHandle_t IMUSampleTask_Handler; 
//void TIM1_BRK_TIM9_IRQHandler(void)
//{
//	BaseType_t Result,xHigherPriorityTaskWoken;
//  if(TIM_GetITStatus(TIM9,TIM_IT_Update) != RESET) 
//	{
//		TIM_ClearITPendingBit(TIM9,TIM_IT_Update);
//		if(IMUSampleTask_Handler!=NULL)
//		{
//		  vTaskNotifyGiveFromISR(IMUSampleTask_Handler,&xHigherPriorityTaskWoken);
//	   	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
//		}
//	}
//}

////ģ���Ӿ�֡�ʷ�������������
////void TIM7_IRQHandler(void)
////{
////   if(TIM_GetITStatus(TIM7,TIM_IT_Update) != RESET)
////	 {
////     TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
////       static s8 a;
////		   static u8 Flag=0;
////		 if(!Flag){
////       a = rand() % 100;
////			 Flag=1;}
////		 else {a = -rand() % 100;
////			 Flag=0;}
////       static bool UpPitchDir 			= TRUE;
////	     static bool UpYawDir			= FALSE;
////       if(UpPitchDir)
////				 Visionrx_freq_data_p=Visionrx_freq_data_p+0.01f+a*0.005;
////			 else 
////				 Visionrx_freq_data_p=Visionrx_freq_data_p-0.01f-a*0.005;
////			 if(Visionrx_freq_data_p>15.0f) UpPitchDir=FALSE;
////			 if(Visionrx_freq_data_p<-30.0f) UpPitchDir=TRUE;
////       
////	 }
////}
///*************************************************************************
//�жϴ��������ƣ�CAN1_TX_IRQHandler
//�жϲ������ƣ�
//�������ܣ�
//*************************************************************************/
//void CAN1_TX_IRQHandler(void)
//{
//    if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
//	{
//		CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
//  }
//}
///***********************************************************
//�жϴ��������ƣ�CAN1_RX0_IRQHandler
//�жϲ������ƣ�
//�������ܣ�
//*************************************************************************/
//CanRxMsg CAN1_RxMsg;
//void CAN1_RX0_IRQHandler(void)
//{   
//  if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
//	{
//    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);		
//		CAN_Receive(CAN1, CAN_FIFO0, &CAN1_RxMsg);
//		switch(CAN1_RxMsg.StdId)
//		{
//			case 0x201:
////				Abs_Encoder_Process(&g_stShooterEncoder,Get_Encoder_Number(&CAN1_RxMsg));
////	      g_stShooterPosPID.fpFB = g_stShooterEncoder.siSumValue;
////			  g_stShooterSpeedPID.fpFB = Get_Speed(&CAN1_RxMsg)/36.0f;
//			break;
//			
//			case 0x207:
//							
//			break;
//			
//			default:				
//			break;
//		}	
//		system_monitor.CAN1_cnt++;
//    }
//}
///*************************************************************************
//�жϴ��������ƣ�CAN2_TX_IRQHandler
//�жϲ������ƣ�
//�������ܣ�
//*************************************************************************/
//void CAN2_TX_IRQHandler(void)
//{
//  if (CAN_GetITStatus(CAN2,CAN_IT_TME)!= RESET)
//  {
//	  CAN_ClearITPendingBit(CAN2,CAN_IT_TME);   
//  }
//}
///*************************************************************************
//�жϴ��������ƣ�CAN2_RX0_IRQHandler
//�жϲ������ƣ�
//�������ܣ�
//*************************************************************************/
//#define Pitch_ZERO 120.717773
//#define Pitch_ZERO1  -5.38330078
//#define Yaw_ZERO     55.2392578
//void CAN2_RX0_IRQHandler(void)
//{
//	CanRxMsg CAN2_RxMsg;
//	
//  if (CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
//	{
//    CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);		

//		CAN_Receive(CAN2, CAN_FIFO0, &CAN2_RxMsg);
//		switch(CAN2_RxMsg.StdId)
//		{
//			case 0x203:   //���������IDΪ3
//					
//			  Abs_Encoder_Process(&g_stShooterEncoder,Get_Encoder_Number(&CAN2_RxMsg));
//	      g_stShooterPosPID.fpFB = g_stShooterEncoder.siSumValue;
//			  g_stShooterSpeedPID.fpFB = Get_Speed(&CAN2_RxMsg)/36.0f;
//			// ����תһȦ ���ƽ��ת295527.8ת  ���˸�ÿ�񲽽�����=36941
//			break;
//			
//			case 0x205://yaw
//        Yaw_encode  = -Angle_Inf_To_180(-Get_Encoder_Number(&CAN2_RxMsg)*360.0/8192.0+Yaw_ZERO);	
//			  Yaw_Limit_Up_imu = -Receive_Angle_Data.Angle_Data.Yaw_Angle - Yaw_encode + Yaw_Limit_Up;
//			  Yaw_Limit_Down_imu = -Receive_Angle_Data.Angle_Data.Yaw_Angle - Yaw_encode + Yaw_Limit_Down;
//			break;
//			
//			case 0x206://pitch
//				Pitch_encode =Angle_Inf_To_180(-Get_Encoder_Number(&CAN2_RxMsg)*360.0/8192.0-Pitch_ZERO);
//			  Pitch_Limit_Up_imu = Receive_Angle_Data.Angle_Data.Pitch_Angle - Pitch_encode + Pitch_Limit_Up;
//			  Pitch_Limit_Down_imu = Receive_Angle_Data.Angle_Data.Pitch_Angle - Pitch_encode + Pitch_Limit_Down;
//			break;
//			
//			default:				
//			break;
//		}	
//		system_monitor.CAN2_cnt++;
//  }
//	
//}

//void HardFault_Handler(void)
//{
//	while(1);
//}
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg can1_rx_msg;
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!=RESET)
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
		CAN_Receive(CAN1,CAN_FIFO0,&can1_rx_msg);
	}
	switch(can1_rx_msg.StdId)
	{
		case 0x201:
			motor_wheel[1].encoder=Get_Encoder_Number(&can1_rx_msg);
			motor_wheel[1].speed=Get_Speed(&can1_rx_msg);
			motor_wheel[1].sum_value=Abs_Encoder_Process(&encoder[1],motor_wheel[1].encoder);
			break;
		case 0x202:
			motor_wheel[2].encoder=Get_Encoder_Number(&can1_rx_msg);
			motor_wheel[2].speed=Get_Speed(&can1_rx_msg);
			motor_wheel[2].sum_value=Abs_Encoder_Process(&encoder[2],motor_wheel[2].encoder);
			break;
		case 0x203:
			motor_wheel[3].encoder=Get_Encoder_Number(&can1_rx_msg);
			motor_wheel[3].speed=Get_Speed(&can1_rx_msg);
			motor_wheel[3].sum_value=Abs_Encoder_Process(&encoder[3],motor_wheel[3].encoder);
			break;	
		case 0x204:
			motor_wheel[4].encoder=Get_Encoder_Number(&can1_rx_msg);
			motor_wheel[4].speed=Get_Speed(&can1_rx_msg);
			motor_wheel[4].sum_value=Abs_Encoder_Process(&encoder[4],motor_wheel[4].encoder);
			break;
		case 0x205:
			motor_wheel[5].encoder=Get_Encoder_Number(&can1_rx_msg);
			motor_wheel[5].speed=Get_Speed(&can1_rx_msg);
			motor_wheel[5].sum_value=Abs_Encoder_Process(&encoder[5],motor_wheel[5].encoder);
			break;
		case 0x206:
			motor_wheel[6].encoder=Get_Encoder_Number(&can1_rx_msg);
			motor_wheel[6].speed=Get_Speed(&can1_rx_msg);
			motor_wheel[6].sum_value=Abs_Encoder_Process(&encoder[6],motor_wheel[6].encoder);
			break;
		default:
			break;
	}
}
void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg can2_rx_msg;
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!=RESET)
	{
		CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);
		CAN_Receive(CAN2,CAN_FIFO0,&can2_rx_msg);
	}
	switch(can2_rx_msg.StdId)
	{
		case 0x201:
			motor_wheel[7].encoder=Get_Encoder_Number(&can2_rx_msg);
			motor_wheel[7].speed=Get_Speed(&can2_rx_msg);
			motor_wheel[7].sum_value=Abs_Encoder_Process(&encoder[7],motor_wheel[7].encoder);
			break;
		case 0x202:
			motor_wheel[8].encoder=Get_Encoder_Number(&can2_rx_msg);
			motor_wheel[8].speed=Get_Speed(&can2_rx_msg);
			motor_wheel[8].sum_value=Abs_Encoder_Process(&encoder[8],motor_wheel[8].encoder);
			break;
		case 0x203:
			motor_wheel[9].encoder=Get_Encoder_Number(&can2_rx_msg);
			motor_wheel[9].speed=Get_Speed(&can2_rx_msg);
			motor_wheel[9].sum_value=Abs_Encoder_Process(&encoder[9],motor_wheel[9].encoder);
			break;
		case 0x204:
			motor_wheel[10].encoder=Get_Encoder_Number(&can2_rx_msg);
			motor_wheel[10].speed=Get_Speed(&can2_rx_msg);
			motor_wheel[10].sum_value=Abs_Encoder_Process(&encoder[10],motor_wheel[10].encoder);
			break;
		case 0x205:
			motor_wheel[11].encoder=Get_Encoder_Number(&can2_rx_msg);
			motor_wheel[11].speed=Get_Speed(&can2_rx_msg);
			motor_wheel[11].sum_value=Abs_Encoder_Process(&encoder[11],motor_wheel[11].encoder);
			break;
		case 0x207:
			motor_wheel[12].encoder=Get_Encoder_Number(&can2_rx_msg);
			motor_wheel[12].speed=Get_Speed(&can2_rx_msg);
			motor_wheel[12].sum_value=Abs_Encoder_Process(&encoder[12],motor_wheel[12].encoder);
			break;
		default:
			break;
	}
}
void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{
		USART2->SR;
		USART2->DR;
		usart2_dma_rx();
		usart2_protocol_rx();
  }
} 

void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
	{
		USART3->SR;
		USART3->DR;
		usart3_dma_rx();
		usart3_protocol_rx();
  }
} 

void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4, USART_IT_IDLE) != RESET)
	{
		UART4->SR;
		UART4->DR;
		uart4_dma_rx();
		uart4_protocol_rx();
  }
} 


