#include "usart.h"
#include "rm_communicate_types.h"
/* ----------------------- Function Implements ---------------------------- */
/******************************************************************************
* @fn RC_Init
* 
* @brief configure stm32 usart2 port
* - USART Parameters
* - 100Kbps
* - 8-N-1
* - DMA Mode
* 
* @return None.
* 
* @note This code is fully tested on STM32F405RGT6 Platform, You can port 
it
* to the other platform. Using doube buffer to receive data prevent 
losing data.
*/
void usart1_init()
{
/*------------------声明初始化所用结构体-------------------*/
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	

/*----------------------时钟使能---------------------------*/	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	

/*----------------------引脚复用---------------------------*/	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10 ,GPIO_AF_USART1);

	
/*---------------------IO口初始化--------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

/*---------------------串口2初始化--------------------------*/
	USART_InitStructure.USART_BaudRate = 10000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
  USART_Init(USART1, &USART_InitStructure);

	
	
/*-------------------USART1_RX的DMA初始化------------------*/
	DMA_DeInit(USART1_RX_STREAM);
	while (DMA_GetCmdStatus(USART1_RX_STREAM) != DISABLE){};
	DMA_InitStructure.DMA_Channel=DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&(USART2->DR));
//	DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&cushioning);
//	DMA_InitStructure.DMA_BufferSize=(u32)(sizeof(cushioning));
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralToMemory;//传输方向：从外设到内存
	
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;	
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(USART1_RX_STREAM,&DMA_InitStructure);
	
	/*使能串口、DMA、中断*/
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);												// 开启串口的空闲中断
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);												// 开启串口DMA接收功能
	USART_Cmd(USART1,ENABLE);																	// 使能串口
    DMA_Cmd(USART1_RX_STREAM,ENABLE);															// 使能DMA
}




void usart2_init(u32 bound)
{
/*------------------声明初始化所用结构体-------------------*/
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	

/*----------------------时钟使能---------------------------*/	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	

/*----------------------引脚复用---------------------------*/	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);

	
/*---------------------IO口初始化--------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

/*---------------------串口2初始化--------------------------*/
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);

	
	
/*-------------------USART2_RX的DMA初始化------------------*/
	DMA_DeInit(USART2_RX_STREAM);
	while (DMA_GetCmdStatus(USART2_RX_STREAM) != DISABLE){};
		DMA_InitStructure.DMA_Channel=DMA_Channel_4;
		DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&(USART2->DR));
		
		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&cushioning);
		DMA_InitStructure.DMA_BufferSize=(u32)(sizeof(cushioning));
		DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralToMemory;//传输方向：从外设到内存
		
		DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority=DMA_Priority_High;
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;	
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(USART2_RX_STREAM,&DMA_InitStructure);
	
	
/*-------------------USART2_TX的DMA初始化------------------*/	
	DMA_DeInit(USART2_TX_STREAM);
	while (DMA_GetCmdStatus(USART2_TX_STREAM) != DISABLE){};

		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&SendBuf);
		DMA_InitStructure.DMA_BufferSize=(u32)(sizeof(USART2_DMA));
		DMA_InitStructure.DMA_DIR=DMA_DIR_MemoryToPeripheral;
	
	DMA_Init(USART2_TX_STREAM,&DMA_InitStructure);
		
	/*使能串口、DMA、中断*/
	USART_Cmd(USART2, ENABLE);  															// 使能串口2
		
		USART_ClearFlag(USART2, USART_FLAG_TC);
		USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);											// 使能空闲中断	
		
		USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);											// 使能串口2接收DMA
		USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);											// 使能串口2发送DMA
		
		DMA_Cmd(USART2_RX_STREAM, ENABLE);														// 使能接收DMA
		DMA_Cmd(USART2_TX_STREAM, ENABLE);														// 失能发送DMA(进行发送时再打开)
}

void usart3_init(u32 bound)
{
/*------------------声明初始化所用结构体-------------------*/
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	

/*----------------------时钟使能---------------------------*/	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	

/*----------------------引脚复用---------------------------*/	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3);

	
/*---------------------IO口初始化--------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

/*---------------------串口3初始化--------------------------*/
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure);

	
	
/*-------------------USART3_RX的DMA初始化------------------*/
	DMA_DeInit(USART3_RX_STREAM);
	while (DMA_GetCmdStatus(USART3_RX_STREAM) != DISABLE){};
		DMA_InitStructure.DMA_Channel=DMA_Channel_4;
		DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&(USART2->DR));
		
		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&cushioning);
		DMA_InitStructure.DMA_BufferSize=(u32)(sizeof(cushioning));
		DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralToMemory;//传输方向：从外设到内存
		
		DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority=DMA_Priority_High;
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;	
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(USART3_RX_STREAM,&DMA_InitStructure);
	
	
/*-------------------USART3_TX的DMA初始化------------------*/	
	DMA_DeInit(USART3_TX_STREAM);
	while (DMA_GetCmdStatus(USART3_TX_STREAM) != DISABLE){};

		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&SendBuf);
		DMA_InitStructure.DMA_BufferSize=(u32)(sizeof(USART2_DMA));
		DMA_InitStructure.DMA_DIR=DMA_DIR_MemoryToPeripheral;
	
	DMA_Init(USART3_TX_STREAM,&DMA_InitStructure);
		
	/*使能串口、DMA、中断*/
	USART_Cmd(USART3, ENABLE);  															// 使能串口3
		
		USART_ClearFlag(USART3, USART_FLAG_TC);
		USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);											// 使能空闲中断	
		
		USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);											// 使能串口3接收DMA
		USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);											// 使能串口3发送DMA
		
		DMA_Cmd(USART3_RX_STREAM, ENABLE);														// 使能接收DMA
		DMA_Cmd(USART3_TX_STREAM, ENABLE);														
}

void uart4_init(u32 bound)
{
/*------------------声明初始化所用结构体-------------------*/
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	

/*----------------------时钟使能---------------------------*/	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	

/*----------------------引脚复用---------------------------*/	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); 								// GPIOC10复用为UART4 TX
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); 								// GPIOC11复用为UART4 RX

	
/*---------------------IO口初始化--------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

/*---------------------串口4初始化--------------------------*/
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UART4, &USART_InitStructure);

	
	
/*-------------------UART4_RX的DMA初始化------------------*/
	DMA_DeInit(UART4_RX_STREAM);
	while (DMA_GetCmdStatus(UART4_RX_STREAM) != DISABLE){};
		DMA_InitStructure.DMA_Channel=DMA_Channel_4;
		DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&(USART2->DR));
		
		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&cushioning);
		DMA_InitStructure.DMA_BufferSize=(u32)(sizeof(cushioning));
		DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralToMemory;//传输方向：从外设到内存
		
		DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority=DMA_Priority_High;
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;	
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(UART4_RX_STREAM,&DMA_InitStructure);
	
	
/*-------------------UART4_TX的DMA初始化------------------*/	
	DMA_DeInit(UART4_TX_STREAM);
	while (DMA_GetCmdStatus(UART4_TX_STREAM) != DISABLE){};

		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&SendBuf);
		DMA_InitStructure.DMA_BufferSize=(u32)(sizeof(USART2_DMA));
		DMA_InitStructure.DMA_DIR=DMA_DIR_MemoryToPeripheral;
	
	DMA_Init(UART4_TX_STREAM,&DMA_InitStructure);
		
	/*使能串口、DMA、中断*/
	USART_Cmd(UART4, ENABLE);  															// 使能串口4
		
		USART_ClearFlag(UART4, USART_FLAG_TC);
		USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);											// 使能空闲中断	
		
		USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);											// 使能串口4接收DMA
		USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);											// 使能串口4发送DMA
		
		DMA_Cmd(UART4_RX_STREAM, ENABLE);														// 使能接收DMA
		DMA_Cmd(UART4_TX_STREAM, ENABLE);														// 失能发送DMA(进行发送时再打开)
}

void uart5_init(u32 bound)
{
/*------------------声明初始化所用结构体-------------------*/
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	

/*----------------------时钟使能---------------------------*/	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	

/*----------------------引脚复用---------------------------*/	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5); 	//TX
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2,  GPIO_AF_UART5); 	//RX

	
/*---------------------IO口初始化--------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

/*---------------------串口5初始化--------------------------*/
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UART5, &USART_InitStructure);

	
	
/*-------------------USART5_RX的DMA初始化------------------*/
	DMA_DeInit(UART5_RX_STREAM);
	while (DMA_GetCmdStatus(UART5_RX_STREAM) != DISABLE){};
		DMA_InitStructure.DMA_Channel=DMA_Channel_4;
		DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&(USART2->DR));
		
		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&cushioning);
		DMA_InitStructure.DMA_BufferSize=(u32)(sizeof(cushioning));
		DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralToMemory;//传输方向：从外设到内存
		
		DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority=DMA_Priority_High;
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;	
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(UART5_RX_STREAM,&DMA_InitStructure);
	
	
/*-------------------USART5_TX的DMA初始化------------------*/	
	DMA_DeInit(UART5_TX_STREAM);
	while (DMA_GetCmdStatus(UART5_TX_STREAM) != DISABLE){};

		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&SendBuf);
		DMA_InitStructure.DMA_BufferSize=(u32)(sizeof(USART2_DMA));
		DMA_InitStructure.DMA_DIR=DMA_DIR_MemoryToPeripheral;
	
	DMA_Init(UART5_TX_STREAM,&DMA_InitStructure);
		
	/*使能串口、DMA、中断*/
	USART_Cmd(UART5, ENABLE);  															// 使能串口5
		
		USART_ClearFlag(UART5, USART_FLAG_TC);
		USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);											// 使能空闲中断	
		
		USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE);											// 使能串口5接收DMA
		USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE);											// 使能串口5发送DMA
		
		DMA_Cmd(UART5_RX_STREAM, ENABLE);														// 使能接收DMA
		DMA_Cmd(UART5_TX_STREAM, ENABLE);														// 失能发送DMA(进行发送时再打开)
}

void usart6_init(u32 bound)
{
/*------------------声明初始化所用结构体-------------------*/
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	

/*----------------------时钟使能---------------------------*/	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	

/*----------------------引脚复用---------------------------*/	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6); //TX	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6); //RX

	
/*---------------------IO口初始化--------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

/*---------------------串口6初始化--------------------------*/
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART6, &USART_InitStructure);

	
	
/*-------------------USART6_RX的DMA初始化------------------*/
	DMA_DeInit(USART6_RX_STREAM);
	while (DMA_GetCmdStatus(USART6_RX_STREAM) != DISABLE){};
		DMA_InitStructure.DMA_Channel=DMA_Channel_4;
		DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&(USART2->DR));
		
		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&cushioning);
		DMA_InitStructure.DMA_BufferSize=(u32)(sizeof(cushioning));
		DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralToMemory;//传输方向：从外设到内存
		
		DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority=DMA_Priority_High;
		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;	
		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(USART6_RX_STREAM,&DMA_InitStructure);
	
	
/*-------------------USART6_TX的DMA初始化------------------*/	
	DMA_DeInit(USART6_TX_STREAM);
	while (DMA_GetCmdStatus(USART6_TX_STREAM) != DISABLE){};

		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&SendBuf);
		DMA_InitStructure.DMA_BufferSize=(u32)(sizeof(USART2_DMA));
		DMA_InitStructure.DMA_DIR=DMA_DIR_MemoryToPeripheral;
	
	DMA_Init(USART6_TX_STREAM,&DMA_InitStructure);
		
	/*使能串口、DMA、中断*/
	USART_Cmd(USART6, ENABLE);  															// 使能串口6
		
		USART_ClearFlag(USART6, USART_FLAG_TC);
		USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);											// 使能空闲中断	
		
		USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);											// 使能串口6接收DMA
		USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);											// 使能串口6发送DMA
		
		DMA_Cmd(USART6_RX_STREAM, ENABLE);														// 使能接收DMA
		DMA_Cmd(USART6_TX_STREAM, ENABLE);														// 失能发送DMA(进行发送时再打开)
}
