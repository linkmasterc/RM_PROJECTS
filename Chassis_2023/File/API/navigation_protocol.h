#ifndef _NAVIGATION_PROTOCOL_H_
#define _NAVIGATION_PROTOCOL_H_

#include "global_declare.h"

/**
 *@breif	舵轮行驶方式定义
 *
 *
 */
#define CHASSIS_STEER_DRIVING_MODE
// #define CHASSIS_DIFFERENTIAL_DRIVING_MODE            
// #define CHASSIS_COMMOM_DRIVING_MODE 

//舵轮正统行驶方式
#ifdef CHASSIS_STEER_DRIVING_MODE
#define CHASSIS_STEER_ANGLE_MAX                             ( (float)(90) )
#define CHASSIS_LINE_SPEED_MAX                              ( (float)(5) )
#define CHASSIS_STEER_ANGLE_SENSITIVITY                     ( (float)(0.1) )
#endif

//差速行驶方式
#ifdef CHASSIS_DIFFERENTIAL_DRIVING_MODE
#define CHASSIS_ANGLE_SPEED_MAX                              ( (float)(300) )
#define CHASSIS_LINE_SPEED_MAX                               ( (float)(20) )
#endif

#ifdef CHASSIS_COMMOM_DRIVING_MODE
#define CHASSIS_LINE_SPEED_MAX                              ( (float)(10) )
#define CHASSIS_ANGLE_SPEED_MAX                              ( (float)(300) )

#endif 

//轮子半径
#define CHASSIS_WHEEL_RADIUS                                (float)(0.06f)

#define RADIAN2DEGREE_VALUE                                (float)(57.29577f)

#define RIDAR_FIXED_ANGLE_ERROR                            (float)(90.0f)

#define SPIN_ANGLE                                      (float)(45.0f)

#define SEC_GIMBAL_DES_WORLD_ANGLE                               (float)(-90.0f)
	
void NavigationDataReceiveProtocol(void);
void WCS_to_LCS(void);
#endif

