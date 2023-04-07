#include "navigation_protocol.h"

void NavigationDataReceiveProtocol()
{
	if(UA5RxDMAbuf[0]==0x55&&UA5RxDMAbuf[1]==0x00)
	{
		memcpy(&NavigationReceiveBuf,(u8*)(&UA5RxDMAbuf)+2,1);
		memcpy((u8*)(&NavigationReceiveBuf)+1,(u8*)(&UA5RxDMAbuf)+4,12);
		memcpy((u8*)(&VisionDataSendBuff.Vision_Send_Data_Buf)+20,(u8*)(&UA5RxDMAbuf)+16,17);
	}

}

void GlobalVisionDataReceiveProtocol()
{
	
}

void DecisionDataSendProtocol()
{
//	static u32 RS_Struct_Number=0;
//	if(RS_Struct_Number>RS_STRUCT_NUM)
//		RS_Struct_Number=0;
//	DecisionSendBuf.id=RSStructIDBuf[RS_Struct_Number];
//	DecisionSendBuf.length=sizeof(((u32*)RSStructAddressBuf[RS_Struct_Number]));
//	memcpy(DecisionSendBuf.data,(u8*)RSStructAddressBuf[RS_Struct_Number],DecisionSendBuf.length);
	
	
	DMA_ClearITPendingBit(UART5_TX_STREAM, DMA_IT_TCIF7);								//开启DMA_Mode_Normal,即便没有使用完成中断也要软件清除，否则只发一次
	
	DMA_Cmd(UART5_TX_STREAM, DISABLE);				       								//设置当前计数值前先禁用DMA
	UART5_TX_STREAM->M0AR = (uint32_t)&DecisionSendBuf;							//设置当前待发数据基地址:Memory0 tARget
	UART5_TX_STREAM->NDTR = (uint32_t)DECISION_SEND_ALL_DATA_LEN;			        		//设置当前待发的数据的数量:Number of Data units to be TRansferred
	DMA_Cmd(UART5_TX_STREAM, ENABLE);	
//	RS_Struct_Number++;
}

void WCS_to_LCS()
{
	static float robot_yaw_des_pre  = 0;
	static float robot_yaw_des_sum  = 0;
	static float chassis_steer_des  = 0;
	float yaw_angle_step  = 0;
	
	Robot_Yaw_Des = NavigationReceiveBuf.stNavigationData.Receive_Data1;
	Robot_Yaw_Cur = NavigationReceiveBuf.stNavigationData.Receive_Data2;
	Chassis_Speed = NavigationReceiveBuf.stNavigationData.Receive_Data3;
	
	
	// speed max limit
	if (Chassis_Speed > CHASSIS_LINE_SPEED_MAX) {
			Chassis_Speed = CHASSIS_LINE_SPEED_MAX;
	} 
	Chassis_Speed = Chassis_Speed /CHASSIS_WHEEL_RADIUS;

	// angle max limit
	yaw_angle_step = Robot_Yaw_Des - robot_yaw_des_pre;
	robot_yaw_des_pre = Robot_Yaw_Des;
	if (yaw_angle_step > 2 * CHASSIS_STEER_ANGLE_MAX) {
			yaw_angle_step = yaw_angle_step - 4 * CHASSIS_STEER_ANGLE_MAX;
	} else if (yaw_angle_step < -2 * CHASSIS_STEER_ANGLE_MAX) {
			yaw_angle_step = yaw_angle_step + 4 * CHASSIS_STEER_ANGLE_MAX;
	}

	// update robot current yaw angle des sum
	robot_yaw_des_sum += yaw_angle_step;

	// calculate the angle error between the wheel and the lidar
	float angle_error = YawEncoderAngle+SecondYawAngle+RIDAR_FIXED_ANGLE_ERROR-Robot_Yaw_Cur;

	Wheel_Angle_Des = robot_yaw_des_sum + angle_error;

}
