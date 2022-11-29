#include "global_declare.h"

SYSTEM_MONITOR 		system_monitor = {0};             //系统监视器
u8 								res_data[ReceiveBufSize];
u8								send_data[SendBufSize];
USART2_DMA 				SendBuf={
													 .head[0]=FRAME_HEAD1,
													 .head[1]=FRAME_HEAD2,
													};
USART2_DMA 				ReceiveBuf;
SEND_DMA_UNION 		Send_Union;
const u8 					NUM_BUF=1;
u8  							cushioning[13];
motor_parameter   motor_wheel[13];
ST_ENCODER 				encoder[13]={{0,0,0,8192,0},{0,0,0,8192,0},{0,0,0,8192,0},{0,0,0,8192,0},
															 {0,0,0,8192,0},{0,0,0,8192,0},{0,0,0,8192,0},{0,0,0,8192,0},
															 {0,0,0,8192,0},{0,0,0,8192,0},{0,0,0,8192,0},{0,0,0,8192,0},
															 {0,0,0,8192,0}};
float							YAW_POS_DES;
float							PITCH_POS_DES;
float							gravity_fb=-6000;
float 						COE=1;
ST_TD 						YAW_POS_TD;
ST_TD							PITCH_POS_TD={0,0,0,150,0.001,0.001,0};
ST_PID 						YAW_POS_PID;
ST_PID 						YAW_SPEED_PID;
ST_PID 						PITCH_POS_PID={.fpKp=0,.fpKi=0.0,.fpDt=0.001,.fpUiMax=5000,.fpEMin=1,.fpUMax=2000};
ST_PID 						PITCH_SPEED_PID={.fpKp=0,.fpKd=0,.fpUdMax=25000,.fpDt=0.001,.fpUMax=28000};
ST_PID						TRIGGER_POS_PID={.fpKp=0.215,.fpUMax=8000};
ST_PID						TRIGGER_SPEED_PID={.fpKp=500,.fpUMax=8000};
ST_PID						SHOOTER_SPEED_PID_L;
ST_PID						SHOOTER_SPEED_PID_R;
ST_LPF 						PITCH_LPF={0,0,0,500,0.001};
ST_LPF 						YAW_LPF={0,0,0,500,0.001};
ST_ANGLE					YAW_ANGLE={0,0,0,360,0};
ST_ANGLE					PITCH_ANGLE={0,0,0,360,0};
u8								DOWN_PLATFOM_STATUS=1;
u8								DOWN_SHOOT_STATUS=1;
u16								bullet_num;
IMU_MODE 					imu_mode = INIT;

ST_LPF 						gyro_x = {0,0,0,150,0.00025f};
ST_LPF 						gyro_y = {0,0,0,150,0.00025f};
ST_LPF 						gyro_z = {0,0,0,150,0.00025f};

ST_LPF 						acc_x = {0,0,0,500,0.00025f};
ST_LPF 						acc_y = {0,0,0,500,0.00025f};
ST_LPF 						acc_z = {0,0,0,500,0.00025f};

u16 							Cali_Cnt = CALI_NUM;
s16 							Real_Temp;
float 						Acc_X_Ori;
float 						Acc_Y_Ori;
float 						Acc_Z_Ori;
float							Gyro_X_Ori;
float 						Gyro_Y_Ori;
float							Gyro_Z_Ori;

float 						Acc_X_Real;											//单位为mg
float 						Acc_Y_Real;
float 						Acc_Z_Real;

float 						Gyro_X_Real;											//单位为弧度每秒
float 						Gyro_Y_Real;
float 						Gyro_Z_Real;

float 						Gyro_X_Speed;
float 						Gyro_Y_Speed;
float 						Gyro_Z_Speed;

/*以下为零飘标定值*/
float 						Acc_X_Offset = 0;
float 						Acc_Y_Offset = 0;
float 						Acc_Z_Offset = 0;
float 						Gyro_X_Offset = 0;
float 						Gyro_Y_Offset = 0;
float 						Gyro_Z_Offset = 0;

float 						Status_offset[3][CALI_NUM] = {0};  //1 X;2 Y;3 Z.
float 						Num_offset[CALI_NUM] = {0};
int  							Const_offset = CALI_NUM;
int  							flag_offset = 0;
float 						testvalue[3] = {0};

float 						SEE = 0;    //Z_OFFSET实时标定的值


float 						Test_Flash_Ori = 1.1;
float 						Test_Flash_Write[2] = {1.1,2.2};
float  						Test_Flash_Get[2] = {0,0};


float 						BMIPitchSpeed;
float 						BMIYawSpeed;
float 						BMIYawAngle;
float 						BMIPitchAngle;
float 						BMIRollSpeed;
float 						BMIRollAngle;
float 						DOWN_SHOOT_FREQ;
_imu_st imu_data =  {1,0,0,0,0,0,
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    0,0,0
};

float Kp = 0.5f;/**/
float Ki = 0.001f;/**/
/**/
float exInt = 0.0f;
float eyInt = 0.0f;
float ezInt = 0.0f;




