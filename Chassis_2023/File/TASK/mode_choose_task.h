#ifndef MODE_CHOOSE_TASK_H
#define MODE_CHOOSE_TASK_H
#include <stdio.h>



//#define  DEBUG_ENABLE  							// 调试模式

#ifdef DEBUG_ENABLE
  #define DEG(fmt, args...)  \
  {\
    printf("<<File:%s	Line:%d  Function:%s>>\n ", __FILE__, __LINE__, __FUNCTION__);\
    printf(fmt, ##args);\
  }
#else
  #define DEG(fmt, args...)   
#endif
  
  
  

/* 比赛模式与测试模式切换 */
//#define TEST_MODE							// 测试模式
#define MATCH_MODE							// 比赛模式

/* 是否挂载在轨道右端 */
#define OUTPOST_DETECT					

/* 是否启用原地变向 */
//#define AUTO_TURN

/* 是否自启动 */
#define AUTO_START							// 自启动

/* 是否击打己方 */
//#define	HIT_OURSELF							// 击打己方

/* 是否使用显示屏 */
#define	USING_HMI							// 击打己方

/* 是否撞柱云台保护 */
//#define GIMBAM_PRO

/* 是否巡逻后方 */
#define PATROL_BACK

/* 相机硬触发使能 */
//#define UP_CAMERA_HARDWARE_TRIGER_ENABLE			
#define DN_CAMERA_HARDWARE_TRIGER_ENABLE		

/* 无线发送数据协议选择 */
//#define MINYOU_PROTOCOL
#define VOFA_PROTOCOL

/* 无线发送数据包选择 */
//#define WIRELESS_SEND_CHASSIS
#define WIRELESS_SEND_VISION
//#define WIRELESS_SEND_SHOOT
//#define WIRELESS_SEND_GIMBAL
//#define WIRELESS_SEND_TESTBOX
//#define WIRELESS_SEND_REFREE


void ModeChoose(void);
#endif
