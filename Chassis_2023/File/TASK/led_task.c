#include "led_task.h"
#include "global_declare.h"
#include "mode_choose_task.h"


 /** --------------------------------------------------------------------------
  * @brief  LED状态改变
			
  * @note	云控通信异常		——	红灯闪烁(1s)
			视觉通信异常		——	蓝灯闪烁(1.25s)
			CAN通信异常			——	绿灯闪烁(1.50s)
			裁判系统通信异常	——	橙灯闪烁(1.75s)
			所有模块工作正常	——	粉灯常亮
 -------------------------------------------------------------------------- **/
void LEDStateChange(void)
{
	static u32 LEDTick = 0;
	u8 ErrorFlag = 0;
	LEDTick++;
		
	/* 云控通信异常 */
	if(stError.GimbalState)									
	{	
		if(!(LEDTick % 150))	
		{
			LED_PINK_OFF();
			LED_BLUE_OFF();
			LED_GREEN_OFF();
			
			LED_RED_ON();													// 红灯闪烁
		}
		ErrorFlag = 1;
	}
	
	/* 视觉通信异常 */	
	if(stError.ComputerState)
	{	
		if(!(LEDTick % 200))	
		{
			LED_PINK_OFF();
			LED_GREEN_OFF();
			LED_RED_OFF();
			
			LED_BLUE_ON();													// 蓝灯闪烁
		}
		ErrorFlag = 1;
	}
	
	/* CAN通信异常 */
	if(stError.Can1State 		|| stError.Can2State)
	{	
		if(!(LEDTick % 250))
		{
			LED_PINK_OFF();
			LED_BLUE_OFF();
			LED_RED_OFF();
			
			LED_GREEN_ON();													// 绿灯闪烁
		}
		ErrorFlag = 1;
	}
	
	/*裁判系统通信异常*/
	if(stError.RSState)
	{	
		if(!(LEDTick % 100))
		{	
			LED_ORANGE_TOGGLE();											// 橙灯闪烁
		}
		ErrorFlag = 1;
	}
	else
	{
		LED_ORANGE_OFF();
	}
	
	/*所有模块工作正常*/
	if(!ErrorFlag)
	{	
		LED_BLUE_OFF();
		LED_GREEN_OFF();
		LED_RED_OFF();
		LED_ORANGE_OFF();
		LED_PINK_ON();
	}

//	LED_BLUE_ON();
//	LED_RED_ON();
//	LED_GREEN_ON();
//	LED_PINK_ON();
//	LED_ORANGE_ON();
}

