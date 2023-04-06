#ifndef GIMBAL_TASK_H
#define GIMBAL_TASK_H

#include "rm_pid_types.h"
#include "global_declare.h"
#include "can_protocol.h"
#define GetBMIPitchAngle()	Angle_180_To_Inf(imu_data.pit, &BMIPitchAngle)
#define GetBMIYawAngle()	Angle_180_To_Inf(imu_data.yaw, &BMIYawAngle)

extern u8	DNPITCHANGLEBOXSIZE;
extern float DNPitchAnglebox[100];



float Angle_180_To_Inf(float angle_input, ST_ANGLE* st_angle);
void ChamberTranfer(void);
void BarrelTranfer(void);
void GimbalControl(void);

#endif
