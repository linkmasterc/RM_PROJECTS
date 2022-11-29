#ifndef __PARAMETER_CONFIG_H__
#define __PARAMETER_CONFIG_H__

#define SendBufSize 	 17
#define ReceiveBufSize 17

//Accelerater_Reg
#define	ACC_ID				0X00
#define	ACC_ERR				0X02
#define ACC_STATUS		0X03
#define	ACC_X_LSB			0X12
#define	ACC_X_MSB			0X13
#define	ACC_Y_LSB 		0X14
#define	ACC_Y_MSB 		0X15
#define	ACC_Z_LSB 		0X16
#define	ACC_Z_MSB 		0X17
#define ACC_RANGE			0X41
#define	TEMP_MSB			0X22
#define	TEMP_LSB 			0X23
#define ACC_CONF			0X40
#define	ACC_PWR_CTRL	0X7D
#define ACC_PWR_CONF	0X7C
#define ACC_SOFT_R		0X7E

//Gyro_Reg
#define	GYRO_ID				0X00
#define	GYRO_X_LSB		0X02
#define	GYRO_X_MSB		0X03
#define	GYRO_Y_LSB		0X04
#define	GYRO_Y_MSB		0X05
#define	GYRO_Z_LSB		0X06
#define	GYRO_Z_MSB		0X07
#define	GYRO_RANGE		0X0F
#define	GYRO_BANDW		0X10
#define GYRO_POWER		0X11
#define	GYRO_SOFT_R		0X14

#define CALI_NUM			5000
#define Cali_Rol_Coe 	1
#define Cali_Pit_Coe 	1
#define Cali_Yaw_Coe 	1.0021f

#define Bottom_SupplyStep 29491
#endif



