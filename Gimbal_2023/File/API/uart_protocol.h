#ifndef __UART_PROTOCOL_H
#define __UART_PROTOCOL_H
#include "main.h"

/*------------------------------crc8---------------------------------*/
u8 		Get_CRC8_Check_Sum(u8 *pchMessage,u32 dwLength,u8 ucCRC8);
u32 	Verify_CRC8_Check_Sum(u8 *pchMessage, u32 dwLength);
void 	Append_CRC8_Check_Sum(u8 *pchMessage, u32 dwLength);

/*------------------------------crc16---------------------------------*/
u16 	Get_CRC16_Check_Sum(u8 *pchMessage,u32 dwLength,u16 wCRC);
u32 	Verify_CRC16_Check_Sum(u8 *pchMessage, u32 dwLength);
void 	Append_CRC16_Check_Sum(u8 * pchMessage,u32 dwLength);
void imu_sendata(void);
void 	usart2_dma_rx(void);

void 	usart3_dma_tx(void);
void 	usart3_dma_rx(void);

void 	uart4_dma_tx(void);
void 	uart4_dma_rx(void);

void 	usart2_protocol_rx(void);
void 	usart3_protocol_rx(void);
void 	uart4_protocol_rx(void);
void uart4_dma_tx(void);
#endif



