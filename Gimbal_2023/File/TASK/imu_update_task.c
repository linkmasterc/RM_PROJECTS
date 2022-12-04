#include "imu_update_task.h"


_imu_st imu_data =  {1,0,0,0,0,0,
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    0,0,0
};

extern _sensor_st sensor;


extern FP32 Gyro_X_Real;
extern FP32 Gyro_Y_Real;
extern FP32 Gyro_Z_Real;

extern FP32 Acc_X_Real;
extern FP32 Acc_Y_Real;
extern FP32 Acc_Z_Real;

float Kp = 0.5f;/**/
float Ki = 0.001f;/**/
/**/
float exInt = 0.0f;
float eyInt = 0.0f;
float ezInt = 0.0f;
/**/
float q0 = 1.0f;
float q1 = 0.0f;
float q2 = 0.0f;
float q3 = 0.0f;

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

//	BMIPitchSpeed 	= Gyro_Y_Speed;
//	BMIYawSpeed			=-Gyro_Z_Speed;
//	BMIRollSpeed		= Gyro_X_Speed;
//	BMIYawAngle			= imu_data.yaw;
//	BMIPitchAngle   = imu_data.pit;
//	BMIRollAngle		= imu_data.rol;
}

void IMU_Update_Mahony(_imu_st* imu,float dt)
{
    float normalise;
    float nor_acc[VEC_XYZ] = {0};
    float ex, ey, ez;//
    float q0s, q1s, q2s, q3s;/*  */
    static float R11,R21;/* (1,1),(2,1) */
    static float vecxZ, vecyZ, veczZ;/* z(0,0,1)' */
    float half_T = 0.5f * dt;
	
    float q0Last = q0;
    float q1Last = q1;
    float q2Last = q2;
    float q3Last = q3;
    float delta_theta[3];/* xyz */
    float delta_theta_s;/* xyz */

	
    /* 0 */
    if((Acc_X_Real != 0.0f) || (Acc_Y_Real != 0.0f) || (Acc_Z_Real != 0.0f))
    {
        nor_acc[X] = Acc_X_Real;
        nor_acc[Y] = Acc_Y_Real;
        nor_acc[Z] = Acc_Z_Real;

        /*  */
        normalise = invSqrt(nor_acc[X] * nor_acc[X] + nor_acc[Y] * nor_acc[Y] + nor_acc[Z] * nor_acc[Z]);
        nor_acc[X] *= normalise;
        nor_acc[Y] *= normalise;
        nor_acc[Z] *= normalise;

        /* , */
        /* |a x b| = |a|*|b|*sin(theta);|a|=|b|=1,thetasin(theta)theta, */
        ex = (nor_acc[Y] * veczZ - nor_acc[Z] * vecyZ);
        ey = (nor_acc[Z] * vecxZ - nor_acc[X] * veczZ);
        ez = (nor_acc[X] * vecyZ - nor_acc[Y] * vecxZ);

        /* , */
        exInt += Ki * ex * dt ;
        eyInt += Ki * ey * dt ;
        ezInt += Ki * ez * dt ;

        /* PI, */
        Gyro_X_Real += Kp * ex + exInt;
        Gyro_Y_Real += Kp * ey + eyInt;
        Gyro_Z_Real += Kp * ez + ezInt;
    }

    /* TkTk+1, */
    delta_theta[0] = Gyro_X_Real*half_T;
    delta_theta[1] = Gyro_Y_Real*half_T;
    delta_theta[2] = Gyro_Z_Real*half_T;
    delta_theta_s = delta_theta[0]*delta_theta[0] + delta_theta[1]*delta_theta[1] + delta_theta[2]*delta_theta[2];
    /* , */
    /* Q(Tk+1)=(I+0.5*delta_theta)Q(Tk) */
// 	q0 += -q1Last * delta_theta[0] - q2Last * delta_theta[1] - q3Last * delta_theta[2];
// 	q1 +=  q0Last * delta_theta[0] + q2Last * delta_theta[2] - q3Last * delta_theta[1];
// 	q2 +=  q0Last * delta_theta[1] - q1Last * delta_theta[2] + q3Last * delta_theta[0];
// 	q3 +=  q0Last * delta_theta[2] + q1Last * delta_theta[1] - q2Last * delta_theta[0];

    /*  */
    /* Q(Tk+1)=((1-0.125*delta_theta_s)I+0.5*delta_theta)Q(Tk) */
    q0 = q0Last*(1-delta_theta_s) - q1Last * delta_theta[0] - q2Last * delta_theta[1] - q3Last * delta_theta[2];
    q1 = q1Last*(1-delta_theta_s) + q0Last * delta_theta[0] + q2Last * delta_theta[2] - q3Last * delta_theta[1];
    q2 = q2Last*(1-delta_theta_s) + q0Last * delta_theta[1] - q1Last * delta_theta[2] + q3Last * delta_theta[0];
    q3 = q3Last*(1-delta_theta_s) + q0Last * delta_theta[2] + q1Last * delta_theta[1] - q2Last * delta_theta[0];

    /*  */
    normalise = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= normalise;
    q1 *= normalise;
    q2 *= normalise;
    q3 *= normalise;
    /*  */
    q0s = q0 * q0;
    q1s = q1 * q1;
    q2s = q2 * q2;
    q3s = q3 * q3;

    R11 = q0s + q1s - q2s - q3s;/* (1,1) */
    R21 = 2 * (q1 * q2 + q0 * q3);/* (2,1) */

    /* z(0,0,1) */
    vecxZ = 2 * (q1 * q3 - q0 * q2);/* (3,1) */
    vecyZ = 2 * (q0 * q1 + q2 * q3);/* (3,2) */
    veczZ = q0s - q1s - q2s + q3s;	/* (3,3) */

    if (vecxZ>1) vecxZ=1;
    if (vecxZ<-1) vecxZ=-1;
    /* roll pitch yaw  */
    imu->pit = -asinf(vecxZ) * 57.30f;
    imu->rol = atan2f(vecyZ, veczZ) * 57.30f;
    imu->yaw = atan2f(R21, R11) * 57.30f;


}

