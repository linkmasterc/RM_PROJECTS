#ifndef DJI_PROTOCOL_H
#define DJI_PROTOCOL_H

#include "global_declare.h"
#include "rm_redefine_types.h"

extern u16 RsCmdID;



/*接收到的来自裁判系统的字节在数据包中位置*/
typedef enum {
	RS_RX_FREE = 0,
	RS_RX_Length = 1,
	RS_RX_Num = 2,
	RS_RX_CRC8 = 3,
	RS_RX_CmdID = 4,
	RS_RX_Data = 5,
	RS_RX_CRC16 = 6,
}RS_RX_Status;

void RC_Init(void);
void RCProtocol(u8 *pData);
void RSProtocol(USART_RX_TypeDef *uart_rx, u8 *pData, u16 Size);
void RSDataDecode(u16 cmdID, u8 *pData, u16 Size);
void SendDataToRS(u16 desID, u8 *pData, u16 Size);
#endif

