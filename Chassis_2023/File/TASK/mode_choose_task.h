#ifndef MODE_CHOOSE_TASK_H
#define MODE_CHOOSE_TASK_H
#include <stdio.h>



//#define  DEBUG_ENABLE  							// ����ģʽ

#ifdef DEBUG_ENABLE
  #define DEG(fmt, args...)  \
  {\
    printf("<<File:%s	Line:%d  Function:%s>>\n ", __FILE__, __LINE__, __FUNCTION__);\
    printf(fmt, ##args);\
  }
#else
  #define DEG(fmt, args...)   
#endif
  
  
  

/* ����ģʽ�����ģʽ�л� */
//#define TEST_MODE							// ����ģʽ
#define MATCH_MODE							// ����ģʽ

/* �Ƿ�����ڹ���Ҷ� */
#define OUTPOST_DETECT					

/* �Ƿ�����ԭ�ر��� */
//#define AUTO_TURN

/* �Ƿ������� */
#define AUTO_START							// ������

/* �Ƿ���򼺷� */
//#define	HIT_OURSELF							// ���򼺷�

/* �Ƿ�ʹ����ʾ�� */
#define	USING_HMI							// ���򼺷�

/* �Ƿ�ײ����̨���� */
//#define GIMBAM_PRO

/* �Ƿ�Ѳ�ߺ� */
#define PATROL_BACK

/* ���Ӳ����ʹ�� */
//#define UP_CAMERA_HARDWARE_TRIGER_ENABLE			
#define DN_CAMERA_HARDWARE_TRIGER_ENABLE		

/* ���߷�������Э��ѡ�� */
//#define MINYOU_PROTOCOL
#define VOFA_PROTOCOL

/* ���߷������ݰ�ѡ�� */
//#define WIRELESS_SEND_CHASSIS
#define WIRELESS_SEND_VISION
//#define WIRELESS_SEND_SHOOT
//#define WIRELESS_SEND_GIMBAL
//#define WIRELESS_SEND_TESTBOX
//#define WIRELESS_SEND_REFREE


void ModeChoose(void);
#endif
