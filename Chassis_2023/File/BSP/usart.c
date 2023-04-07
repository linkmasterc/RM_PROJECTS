#include "usart.h"
#include "global_declare.h"
#include "rm_communicate_types.h"

#include <stdio.h>

/* 告知连接器不从C库链接使用半主机的函数 */
#pragma import(__use_no_semihosting)

/* 定义 _sys_exit() 以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}

/* 标准库需要的支持类型 */
struct __FILE
{
    int handle;
};

FILE __stdout;
/*重定义fputc的另外一种方式*/
int fputc(int ch, FILE *stream)
{
    /* 堵塞判断串口是否发送完成 */
    while((UART4->SR & 0X40) == 0);

    /* 串口发送完成，将该字符发送 */
    UART4->DR = (uint8_t) ch;

    return ch;
}

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
	USART_InitStructure.USART_BaudRate = 100000;
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
	DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&(USART1->DR));
	DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(USART1_Cushioning_Rx);
	DMA_InitStructure.DMA_BufferSize=(u32)(USART1_RX_BUF_LEN);
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
//	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);												// 开启串口的空闲中断
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);												// 开启串口DMA接收功能
	USART_Cmd(USART1,ENABLE);																	// 使能串口
    DMA_Cmd(USART1_RX_STREAM,ENABLE);															// 使能DMA
}




/** --------------------------------------------------------------------------
  * @brief  串口2初始化
  
  * @retval None
  
  * @param	None
			
  * @note	与裁判系统进行通信
			裁判系统的波特率为115200bit/s
			发送口为PD5
			接收口为PD6
			中断优先级为1-0
 -------------------------------------------------------------------------- **/
void usart2_init(u32 bound)
{
	/*定义初始化相关结构体*/
	GPIO_InitTypeDef 	GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	DMA_InitTypeDef 	DMA_InitStructure;

	
	/*配置时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1,ENABLE);				// 使能PD端口时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);									// 使能USART2时钟
	
	
	/*配置端口复用*/	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); 

	
	/*配置GPIO*/
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;											// 复用模式
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;										// 推挽输出
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;									// IO口速度为100MHz
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;										// 浮空输入
	GPIO_Init(GPIOD,&GPIO_InitStructure);													// 根据设定参数初始化GPIOA
	
	
	/*配置串口*/
	USART_InitStructure.USART_BaudRate 				= bound;								// 波特率
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;					// 字长为8位数据格式
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;						// 一个停止位
	USART_InitStructure.USART_Parity 				= USART_Parity_No;						// 无奇偶校验位
	USART_InitStructure.USART_Mode 					= USART_Mode_Tx|USART_Mode_Rx;			// 收发模式
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;		// 无硬件数据流控制
	USART_Init(USART2,&USART_InitStructure);//初始化串口

	
//	/*配置中断*/
//	NVIC_InitStructure.NVIC_IRQChannel 						= USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;							// 抢占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;							// 子优先级
//	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;						// IRQ通道使能 
//	NVIC_Init(&NVIC_InitStructure);															// 根据指定的参数初始化VIC寄存器


	/*配置接收DMA*/
	DMA_DeInit(USART2_RX_STREAM);
    DMA_InitStructure.DMA_Channel                 =    DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr      =    (uint32_t)&(USART2->DR);				// 设置DMA传输外设基地址（把DMA应用于串口1的接收中）
    DMA_InitStructure.DMA_Memory0BaseAddr         =    (uint32_t)UA2RxDMAbuf;				// 设置DMA传输内存基地址（用户自定义，串口1接收到的数据将存在这里面）
    DMA_InitStructure.DMA_DIR                     =    DMA_DIR_PeripheralToMemory;			// 设置数据传输方向（外设接收到内存）
    DMA_InitStructure.DMA_BufferSize              =    USART2_RX_DMA_BUF_LEN;						// 设置DMA一次传输数据量的大小（500字节）
    DMA_InitStructure.DMA_PeripheralInc           =    DMA_PeripheralInc_Disable;			// 设置外设地址不变
    DMA_InitStructure.DMA_MemoryInc               =    DMA_MemoryInc_Enable;				// 设置内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize      =    DMA_PeripheralDataSize_Byte;			// 设置外设的数据长度为字节（8bits）
    DMA_InitStructure.DMA_MemoryDataSize          =    DMA_MemoryDataSize_Byte;				// 设置内存的数据长度为字节（8bits）
    DMA_InitStructure.DMA_Mode                    =    DMA_Mode_Circular;					// 设置DMA模式为循环模式
    DMA_InitStructure.DMA_Priority                =    DMA_Priority_VeryHigh;				// 设置DMA通道的优先级为最高优先级
    DMA_InitStructure.DMA_FIFOMode                =    DMA_FIFOMode_Disable;				// 不使用FIFO模式
    DMA_InitStructure.DMA_FIFOThreshold           =    DMA_FIFOThreshold_1QuarterFull;		// FIFO阈值
    DMA_InitStructure.DMA_MemoryBurst             =    DMA_Mode_Normal;						// 使用普通模式
    DMA_InitStructure.DMA_PeripheralBurst         =    DMA_PeripheralBurst_Single;			// 外设突发单次传输
	DMA_Init(USART2_RX_STREAM,&DMA_InitStructure);
	
	
	/*配置发送DMA*/
	DMA_DeInit(USART2_TX_STREAM);
	while( DMA_GetCmdStatus(USART2_TX_STREAM) == ENABLE);									// 等待DMA可配置
	
	DMA_InitStructure.DMA_Channel 				= DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&(USART2->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr 		= (u32)&StateDataSendBuff;
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_MemoryToPeripheral;				// 内存到外设
	DMA_InitStructure.DMA_BufferSize 			= STATE_SEND_DATA_LEN;
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;				// 固定外设基地址
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;						// 内存递增发送
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Normal;							// 普通模式发送
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode 				= DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold 		= DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst 			= DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
	DMA_Init(USART2_TX_STREAM, &DMA_InitStructure);


	/*使能串口、DMA、中断*/
	USART_Cmd(USART2, ENABLE);  															// 使能串口2
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
		DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&(USART3->DR));
		
		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&GimbalCushioning_Rx);
		DMA_InitStructure.DMA_BufferSize=(u32)(ChassisBufLen_Rx);
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

		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&GimbalData.Send);
		DMA_InitStructure.DMA_BufferSize=(u32)(ChassisBufLen_Tx);
		DMA_InitStructure.DMA_DIR=DMA_DIR_MemoryToPeripheral;
		DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	
	DMA_Init(USART3_TX_STREAM,&DMA_InitStructure);
		
	
	USART_Cmd(USART3, ENABLE);  															// 使能串口3
		USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);						// 使能空闲中断
		
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
		DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&(UART4->DR));
		
		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)0;
		DMA_InitStructure.DMA_BufferSize=(u32)(0);
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

		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&StateDataSendBuff);
		DMA_InitStructure.DMA_BufferSize=(u32)(STATE_SEND_DATA_LEN);
		DMA_InitStructure.DMA_DIR=DMA_DIR_MemoryToPeripheral;
		DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	
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
		DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&(UART5->DR));
		
		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&UA5RxDMAbuf);
		DMA_InitStructure.DMA_BufferSize=(u32)(NavigationBufLen_Rx);
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
		DMA_InitStructure.DMA_Channel=DMA_Channel_4;
		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&DecisionSendBuf);
		DMA_InitStructure.DMA_BufferSize=(u32)(DECISION_SEND_ALL_DATA_LEN);
		DMA_InitStructure.DMA_DIR=DMA_DIR_MemoryToPeripheral;
		DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
		DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;
		DMA_InitStructure.DMA_Mode 					= DMA_Mode_Normal;							// 普通模式发送
		DMA_InitStructure.DMA_Priority 				= DMA_Priority_VeryHigh;
		DMA_InitStructure.DMA_FIFOMode 				= DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold 		= DMA_FIFOThreshold_1QuarterFull;
		DMA_InitStructure.DMA_MemoryBurst 			= DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
	
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
	/*定义初始化相关结构体*/
	GPIO_InitTypeDef 	GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	DMA_InitTypeDef   	DMA_InitStructure;

	
	/*配置时钟*/			
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	
	
	/*配置端口复用*/	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6); //TX	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6); //RX
	
	
	/*配置GPIO*/	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;   //PP
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	/*配置串口*/	
	USART_DeInit(USART6);
	USART_InitStructure.USART_BaudRate 				= bound;			
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;
	USART_InitStructure.USART_Parity 				= USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;	 
	USART_Init(USART6, &USART_InitStructure);
	
	
//	/*配置中断*/
//	NVIC_InitStructure.NVIC_IRQChannel 						= USART6_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

	
	/*配置接收DMA*/
	DMA_DeInit(USART6_RX_STREAM);
    DMA_InitStructure.DMA_Channel                 =    DMA_Channel_5;
    DMA_InitStructure.DMA_PeripheralBaseAddr      =    (uint32_t)&(USART6->DR);				// 设置DMA传输外设基地址（把DMA应用于串口1的接收中）
    DMA_InitStructure.DMA_Memory0BaseAddr         =    (uint32_t)UA6RxDMAbuf;				// 设置DMA传输内存基地址（用户自定义，串口1接收到的数据将存在这里面）
    DMA_InitStructure.DMA_DIR                     =    DMA_DIR_PeripheralToMemory;			// 设置数据传输方向（外设接收到内存）
    DMA_InitStructure.DMA_BufferSize              =    38;						// 设置DMA一次传输数据量的大小（500字节）
    DMA_InitStructure.DMA_PeripheralInc           =    DMA_PeripheralInc_Disable;			// 设置外设地址不变
    DMA_InitStructure.DMA_MemoryInc               =    DMA_MemoryInc_Enable;				// 设置内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize      =    DMA_PeripheralDataSize_Byte;			// 设置外设的数据长度为字节（8bits）
    DMA_InitStructure.DMA_MemoryDataSize          =    DMA_MemoryDataSize_Byte;				// 设置内存的数据长度为字节（8bits）
    DMA_InitStructure.DMA_Mode                    =    DMA_Mode_Circular;					// 设置DMA模式为循环模式
    DMA_InitStructure.DMA_Priority                =    DMA_Priority_VeryHigh;				// 设置DMA通道的优先级为最高优先级
    DMA_InitStructure.DMA_FIFOMode                =    DMA_FIFOMode_Disable;				// 不使用FIFO模式
    DMA_InitStructure.DMA_FIFOThreshold           =    DMA_FIFOThreshold_Full;				// FIFO阈值
    DMA_InitStructure.DMA_MemoryBurst             =    DMA_Mode_Normal;						// 使用普通模式
    DMA_InitStructure.DMA_PeripheralBurst         =    DMA_PeripheralBurst_Single;			// 外设突发单次传输
    DMA_Init(USART6_RX_STREAM,&DMA_InitStructure);		

	
	
	/*配置发送DMA*/
	DMA_DeInit(USART6_TX_STREAM);
	while( DMA_GetCmdStatus(USART6_TX_STREAM) == ENABLE);									// 等待DMA可配置
	DMA_InitStructure.DMA_Channel 				= DMA_Channel_5;
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (u32)&(USART6->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr 		= NULL;
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_MemoryToPeripheral;				// 内存到外设
	DMA_InitStructure.DMA_BufferSize 			= NULL;
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Normal;							// 普通模式发送
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode 				= DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold 		= DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst 			= DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
	DMA_Init(USART6_TX_STREAM, &DMA_InitStructure);
	
	
	/*使能串口、DMA、中断*/
    USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);   										// 开启空闲中断
	USART_Cmd(USART6, ENABLE);                       										// 使能串口
	USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);   										// 使能串口的接收DMA
	USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);   										// 使能串口的发送DMA
    DMA_Cmd(USART6_RX_STREAM,ENABLE);														// 使能DMA
	DMA_Cmd(USART6_TX_STREAM,DISABLE);														// 失能DMA
}

