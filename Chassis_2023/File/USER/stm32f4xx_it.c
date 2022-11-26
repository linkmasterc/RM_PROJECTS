#include "stm32f4xx_it.h"

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
		CAN1_Ptotocol();
		
	}
}



