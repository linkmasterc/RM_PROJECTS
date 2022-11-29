#include "imu_update_task.h"

/*--------------------------------------
�����ܣ�IMU���ݸ���
--------------------------------------*/

void IMU_Update_Task()
{
    /*�������������ں�����kpϵ��*/
	imu_data.gkp = 0.4f;

	/*�������������ں�����kiϵ��*/
	imu_data.gki =  0.002f;
	IMU_Update_Mahony(&imu_data,1e-3f);	//1msִ��һ��

	BMIPitchSpeed 	= Gyro_Y_Speed;
	BMIYawSpeed			=-Gyro_Z_Speed;
	BMIRollSpeed		= Gyro_X_Speed;
	BMIYawAngle			= imu_data.yaw;
	BMIPitchAngle   = imu_data.pit;
	BMIRollAngle		= imu_data.rol;
}



