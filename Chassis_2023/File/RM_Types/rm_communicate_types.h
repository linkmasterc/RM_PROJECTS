#ifndef RM_COMMUNICATE_TYPES_H
#define RM_COMMUNICATE_TYPES_H

#include "rm_redefine_types.h"

#define UART5_RX_STREAM        	DMA1_Stream0
#define USART3_RX_STREAM        DMA1_Stream1
#define UART4_RX_STREAM         DMA1_Stream2
#define USART3_TX_STREAM        DMA1_Stream3
#define UART4_TX_STREAM         DMA1_Stream4
#define USART2_RX_STREAM        DMA1_Stream5
#define USART2_TX_STREAM        DMA1_Stream6
#define UART5_TX_STREAM       	DMA1_Stream7
#define UART8_TX_STREAM         DMA1_Stream0
#define UART7_TX_STREAM         DMA1_Stream1
#define UART7_RX_STREAM         DMA1_Stream3
#define UART8_RX_STREAM         DMA1_Stream6
#define USART6_RX_STREAM        DMA2_Stream1
#define USART1_RX_STREAM        DMA2_Stream2
#define USART6_TX_STREAM        DMA2_Stream6
#define USART1_TX_STREAM        DMA2_Stream7

/** �������������ṹ�� */
typedef struct
{
	u32 uiRawValue;     	//���α�������ԭʼֵ
	u32 uiPreRawValue;  	//��һ�α�������ԭʼֵ
	s32 siDiff;         	//����������ԭʼֵ�Ĳ�ֵ
	float fpSpeed;      	//��������������ת�٣���λ��r/min
	float fpGearRatio; 		//������������ٱ�
	s32 siNumber;       	//����������
	float fpSumValue; 	  	//�������ۼ�ֵ
} ST_ENCODER;
#endif




