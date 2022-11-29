#include "imu_update_task.h"

/*--------------------------------------
任务功能：IMU数据更新
--------------------------------------*/

void IMU_Update_Task()
{
    /*设置重力互补融合修正kp系数*/
	imu_data.gkp = 0.4f;

	/*设置重力互补融合修正ki系数*/
	imu_data.gki =  0.002f;
	IMU_Update_Mahony(&imu_data,1e-3f);	//1ms执行一次

	BMIPitchSpeed 	= Gyro_Y_Speed;
	BMIYawSpeed			=-Gyro_Z_Speed;
	BMIRollSpeed		= Gyro_X_Speed;
	BMIYawAngle			= imu_data.yaw;
	BMIPitchAngle   = imu_data.pit;
	BMIRollAngle		= imu_data.rol;
}



