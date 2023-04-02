#include "navigation_protocol.h"

void NavigationDataReceiveProtocol()
{
	if(UA5RxDMAbuf[0]==0x55&&UA5RxDMAbuf[1]==0x00)
	{
		memcpy(&NavigationReceiveBuff,(u8*)(&UA5RxDMAbuf)+2,1);
		memcpy((u8*)(&NavigationReceiveBuff)+1,(u8*)(&UA5RxDMAbuf)+4,12);
	}
	
}


void WCS_to_LCS()
{
	static float robot_yaw_des_pre  = 0;
	static float robot_yaw_des_sum  = 0;
	static float chassis_steer_des  = 0;
	float yaw_angle_step  = 0;
	
	Robot_Yaw_Des = NavigationReceiveBuff.stNavigationData.Receive_Data1*57.3;
	Robot_Yaw_Cur = NavigationReceiveBuff.stNavigationData.Receive_Data2*57.3;
	Chassis_Speed = NavigationReceiveBuff.stNavigationData.Receive_Data3;
	
	
	// speed max limit
	if (Chassis_Speed > CHASSIS_LINE_SPEED_MAX) {
			Chassis_Speed = CHASSIS_LINE_SPEED_MAX;
	} 
	Chassis_Speed = (Chassis_Speed / (CHASSIS_WHEEL_RADIUS*2*PI))*60*19;

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
