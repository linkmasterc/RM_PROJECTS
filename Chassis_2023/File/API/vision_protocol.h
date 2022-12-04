#ifndef VISION_PROTOCOL_H
#define VISION_PROTOCOL_H

#include "global_declare.h"

#define VisionDataBuffLen UA6RxMBbuf_LEN
#define VisionRcr USART6_Rcr



extern bool UPReceiveFlag;
extern bool DNReceiveFlag;


typedef enum {
	V_RX_FREE_1 = 0,
	V_RX_FREE_2 = 1,
	V_RX_FLAG = 2,
	V_RX_START = 3,
	V_RX_DATA = 4,
	V_RX_END = 5,
}V_RX_Status;
float HP_Detect(void);
void DNVisionDataProtocol(void);
void DNVisionDataSend(void);

#endif
