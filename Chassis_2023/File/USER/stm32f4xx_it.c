#include "stm32f4xx_it.h"

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
		CAN1_Ptotocol();
		
	}
}



