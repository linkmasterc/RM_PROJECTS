#include "usart.h"
#include "global_declare.h"
#include "rm_communicate_types.h"

#include <stdio.h>

/* ��֪����������C������ʹ�ð������ĺ��� */
#pragma import(__use_no_semihosting)

/* ���� _sys_exit() �Ա���ʹ�ð�����ģʽ */
void _sys_exit(int x)
{
    x = x;
}

/* ��׼����Ҫ��֧������ */
struct __FILE
{
    int handle;
};

FILE __stdout;
/*�ض���fputc������һ�ַ�ʽ*/
int fputc(int ch, FILE *stream)
{
    /* �����жϴ����Ƿ������ */
    while((UART4->SR & 0X40) == 0);

    /* ���ڷ�����ɣ������ַ����� */
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
/*------------------������ʼ�����ýṹ��-------------------*/
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	

/*----------------------ʱ��ʹ��---------------------------*/	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	

/*----------------------���Ÿ���---------------------------*/	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10 ,GPIO_AF_USART1);

	
/*---------------------IO�ڳ�ʼ��--------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

/*---------------------����2��ʼ��--------------------------*/
	USART_InitStructure.USART_BaudRate = 100000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
  USART_Init(USART1, &USART_InitStructure);

	
	
/*-------------------USART1_RX��DMA��ʼ��------------------*/
	DMA_DeInit(USART1_RX_STREAM);
	while (DMA_GetCmdStatus(USART1_RX_STREAM) != DISABLE){};
	DMA_InitStructure.DMA_Channel=DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&(USART1->DR));
	DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(USART1_Cushioning_Rx);
	DMA_InitStructure.DMA_BufferSize=(u32)(USART1_RX_BUF_LEN);
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralToMemory;//���䷽�򣺴����赽�ڴ�
	
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
	
	/*ʹ�ܴ��ڡ�DMA���ж�*/
//	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);												// �������ڵĿ����ж�
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);												// ��������DMA���չ���
	USART_Cmd(USART1,ENABLE);																	// ʹ�ܴ���
    DMA_Cmd(USART1_RX_STREAM,ENABLE);															// ʹ��DMA
}




/** --------------------------------------------------------------------------
  * @brief  ����2��ʼ��
  
  * @retval None
  
  * @param	None
			
  * @note	�����ϵͳ����ͨ��
			����ϵͳ�Ĳ�����Ϊ115200bit/s
			���Ϳ�ΪPD5
			���տ�ΪPD6
			�ж����ȼ�Ϊ1-0
 -------------------------------------------------------------------------- **/
void usart2_init(u32 bound)
{
	/*�����ʼ����ؽṹ��*/
	GPIO_InitTypeDef 	GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	DMA_InitTypeDef 	DMA_InitStructure;

	
	/*����ʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_DMA1,ENABLE);				// ʹ��PD�˿�ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);									// ʹ��USART2ʱ��
	
	
	/*���ö˿ڸ���*/	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2); 

	
	/*����GPIO*/
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;											// ����ģʽ
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;										// �������
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;									// IO���ٶ�Ϊ100MHz
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;										// ��������
	GPIO_Init(GPIOD,&GPIO_InitStructure);													// �����趨������ʼ��GPIOA
	
	
	/*���ô���*/
	USART_InitStructure.USART_BaudRate 				= bound;								// ������
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;					// �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;						// һ��ֹͣλ
	USART_InitStructure.USART_Parity 				= USART_Parity_No;						// ����żУ��λ
	USART_InitStructure.USART_Mode 					= USART_Mode_Tx|USART_Mode_Rx;			// �շ�ģʽ
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;		// ��Ӳ������������
	USART_Init(USART2,&USART_InitStructure);//��ʼ������

	
//	/*�����ж�*/
//	NVIC_InitStructure.NVIC_IRQChannel 						= USART2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;							// ��ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;							// �����ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;						// IRQͨ��ʹ�� 
//	NVIC_Init(&NVIC_InitStructure);															// ����ָ���Ĳ�����ʼ��VIC�Ĵ���


	/*���ý���DMA*/
	DMA_DeInit(USART2_RX_STREAM);
    DMA_InitStructure.DMA_Channel                 =    DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr      =    (uint32_t)&(USART2->DR);				// ����DMA�����������ַ����DMAӦ���ڴ���1�Ľ����У�
    DMA_InitStructure.DMA_Memory0BaseAddr         =    (uint32_t)UA2RxDMAbuf;				// ����DMA�����ڴ����ַ���û��Զ��壬����1���յ������ݽ����������棩
    DMA_InitStructure.DMA_DIR                     =    DMA_DIR_PeripheralToMemory;			// �������ݴ��䷽��������յ��ڴ棩
    DMA_InitStructure.DMA_BufferSize              =    USART2_RX_DMA_BUF_LEN;						// ����DMAһ�δ����������Ĵ�С��500�ֽڣ�
    DMA_InitStructure.DMA_PeripheralInc           =    DMA_PeripheralInc_Disable;			// ���������ַ����
    DMA_InitStructure.DMA_MemoryInc               =    DMA_MemoryInc_Enable;				// �����ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize      =    DMA_PeripheralDataSize_Byte;			// ������������ݳ���Ϊ�ֽڣ�8bits��
    DMA_InitStructure.DMA_MemoryDataSize          =    DMA_MemoryDataSize_Byte;				// �����ڴ�����ݳ���Ϊ�ֽڣ�8bits��
    DMA_InitStructure.DMA_Mode                    =    DMA_Mode_Circular;					// ����DMAģʽΪѭ��ģʽ
    DMA_InitStructure.DMA_Priority                =    DMA_Priority_VeryHigh;				// ����DMAͨ�������ȼ�Ϊ������ȼ�
    DMA_InitStructure.DMA_FIFOMode                =    DMA_FIFOMode_Disable;				// ��ʹ��FIFOģʽ
    DMA_InitStructure.DMA_FIFOThreshold           =    DMA_FIFOThreshold_1QuarterFull;		// FIFO��ֵ
    DMA_InitStructure.DMA_MemoryBurst             =    DMA_Mode_Normal;						// ʹ����ͨģʽ
    DMA_InitStructure.DMA_PeripheralBurst         =    DMA_PeripheralBurst_Single;			// ����ͻ�����δ���
	DMA_Init(USART2_RX_STREAM,&DMA_InitStructure);
	
	
	/*���÷���DMA*/
	DMA_DeInit(USART2_TX_STREAM);
	while( DMA_GetCmdStatus(USART2_TX_STREAM) == ENABLE);									// �ȴ�DMA������
	
	DMA_InitStructure.DMA_Channel 				= DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&(USART2->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr 		= (u32)&StateDataSendBuff;
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_MemoryToPeripheral;				// �ڴ浽����
	DMA_InitStructure.DMA_BufferSize 			= STATE_SEND_DATA_LEN;
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;				// �̶��������ַ
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;						// �ڴ��������
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Normal;							// ��ͨģʽ����
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode 				= DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold 		= DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst 			= DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
	DMA_Init(USART2_TX_STREAM, &DMA_InitStructure);


	/*ʹ�ܴ��ڡ�DMA���ж�*/
	USART_Cmd(USART2, ENABLE);  															// ʹ�ܴ���2
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);											// ʹ�ܿ����ж�	
	USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);											// ʹ�ܴ���2����DMA
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);											// ʹ�ܴ���2����DMA
	DMA_Cmd(USART2_RX_STREAM, ENABLE);														// ʹ�ܽ���DMA
	DMA_Cmd(USART2_TX_STREAM, ENABLE);														// ʧ�ܷ���DMA(���з���ʱ�ٴ�)

}
void usart3_init(u32 bound)
{
/*------------------������ʼ�����ýṹ��-------------------*/
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	

/*----------------------ʱ��ʹ��---------------------------*/	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	

/*----------------------���Ÿ���---------------------------*/	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3);

	
/*---------------------IO�ڳ�ʼ��--------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

/*---------------------����3��ʼ��--------------------------*/
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure);

	
	
/*-------------------USART3_RX��DMA��ʼ��------------------*/
	DMA_DeInit(USART3_RX_STREAM);
	while (DMA_GetCmdStatus(USART3_RX_STREAM) != DISABLE){};
		DMA_InitStructure.DMA_Channel=DMA_Channel_4;
		DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&(USART3->DR));
		
		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&GimbalCushioning_Rx);
		DMA_InitStructure.DMA_BufferSize=(u32)(ChassisBufLen_Rx);
		DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralToMemory;//���䷽�򣺴����赽�ڴ�
		
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
	
	
/*-------------------USART3_TX��DMA��ʼ��------------------*/	
	DMA_DeInit(USART3_TX_STREAM);
	while (DMA_GetCmdStatus(USART3_TX_STREAM) != DISABLE){};

		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&GimbalData.Send);
		DMA_InitStructure.DMA_BufferSize=(u32)(ChassisBufLen_Tx);
		DMA_InitStructure.DMA_DIR=DMA_DIR_MemoryToPeripheral;
		DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	
	DMA_Init(USART3_TX_STREAM,&DMA_InitStructure);
		
	
	USART_Cmd(USART3, ENABLE);  															// ʹ�ܴ���3
		USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);						// ʹ�ܿ����ж�
		
		USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);											// ʹ�ܴ���3����DMA
		USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);											// ʹ�ܴ���3����DMA
		
		DMA_Cmd(USART3_RX_STREAM, ENABLE);														// ʹ�ܽ���DMA
		DMA_Cmd(USART3_TX_STREAM, ENABLE);														
}

void uart4_init(u32 bound)
{
/*------------------������ʼ�����ýṹ��-------------------*/
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	

/*----------------------ʱ��ʹ��---------------------------*/	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	

/*----------------------���Ÿ���---------------------------*/	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); 								// GPIOC10����ΪUART4 TX
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); 								// GPIOC11����ΪUART4 RX

	
/*---------------------IO�ڳ�ʼ��--------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

/*---------------------����4��ʼ��--------------------------*/
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UART4, &USART_InitStructure);

	
	
/*-------------------UART4_RX��DMA��ʼ��------------------*/
	DMA_DeInit(UART4_RX_STREAM);
	while (DMA_GetCmdStatus(UART4_RX_STREAM) != DISABLE){};
		DMA_InitStructure.DMA_Channel=DMA_Channel_4;
		DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&(UART4->DR));
		
		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)0;
		DMA_InitStructure.DMA_BufferSize=(u32)(0);
		DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralToMemory;//���䷽�򣺴����赽�ڴ�
		
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
	
	
/*-------------------UART4_TX��DMA��ʼ��------------------*/	
	DMA_DeInit(UART4_TX_STREAM);
	while (DMA_GetCmdStatus(UART4_TX_STREAM) != DISABLE){};

		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&StateDataSendBuff);
		DMA_InitStructure.DMA_BufferSize=(u32)(STATE_SEND_DATA_LEN);
		DMA_InitStructure.DMA_DIR=DMA_DIR_MemoryToPeripheral;
		DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;
	
	DMA_Init(UART4_TX_STREAM,&DMA_InitStructure);
		
	/*ʹ�ܴ��ڡ�DMA���ж�*/
	USART_Cmd(UART4, ENABLE);  															// ʹ�ܴ���4
		
		USART_ClearFlag(UART4, USART_FLAG_TC);
		USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);											// ʹ�ܿ����ж�	
		
		USART_DMACmd(UART4, USART_DMAReq_Rx, ENABLE);											// ʹ�ܴ���4����DMA
		USART_DMACmd(UART4, USART_DMAReq_Tx, ENABLE);											// ʹ�ܴ���4����DMA
		
		DMA_Cmd(UART4_RX_STREAM, ENABLE);														// ʹ�ܽ���DMA
		DMA_Cmd(UART4_TX_STREAM, ENABLE);														// ʧ�ܷ���DMA(���з���ʱ�ٴ�)
}

void uart5_init(u32 bound)
{
/*------------------������ʼ�����ýṹ��-------------------*/
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	

/*----------------------ʱ��ʹ��---------------------------*/	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	

/*----------------------���Ÿ���---------------------------*/	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5); 	//TX
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2,  GPIO_AF_UART5); 	//RX

	
/*---------------------IO�ڳ�ʼ��--------------------------*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

/*---------------------����5��ʼ��--------------------------*/
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(UART5, &USART_InitStructure);

	
	
/*-------------------USART5_RX��DMA��ʼ��------------------*/
	DMA_DeInit(UART5_RX_STREAM);
	while (DMA_GetCmdStatus(UART5_RX_STREAM) != DISABLE){};
		DMA_InitStructure.DMA_Channel=DMA_Channel_4;
		DMA_InitStructure.DMA_PeripheralBaseAddr=(u32)(&(UART5->DR));
		
		DMA_InitStructure.DMA_Memory0BaseAddr=(u32)(&UA5RxDMAbuf);
		DMA_InitStructure.DMA_BufferSize=(u32)(NavigationBufLen_Rx);
		DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralToMemory;//���䷽�򣺴����赽�ڴ�
		
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
	
	
/*-------------------USART5_TX��DMA��ʼ��------------------*/	
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
		DMA_InitStructure.DMA_Mode 					= DMA_Mode_Normal;							// ��ͨģʽ����
		DMA_InitStructure.DMA_Priority 				= DMA_Priority_VeryHigh;
		DMA_InitStructure.DMA_FIFOMode 				= DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold 		= DMA_FIFOThreshold_1QuarterFull;
		DMA_InitStructure.DMA_MemoryBurst 			= DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
	
	DMA_Init(UART5_TX_STREAM,&DMA_InitStructure);
		
	/*ʹ�ܴ��ڡ�DMA���ж�*/
	USART_Cmd(UART5, ENABLE);  															// ʹ�ܴ���5
		
		USART_ClearFlag(UART5, USART_FLAG_TC);
		USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);											// ʹ�ܿ����ж�	
		
		USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE);											// ʹ�ܴ���5����DMA
		USART_DMACmd(UART5, USART_DMAReq_Tx, ENABLE);											// ʹ�ܴ���5����DMA
		
		DMA_Cmd(UART5_RX_STREAM, ENABLE);														// ʹ�ܽ���DMA
		DMA_Cmd(UART5_TX_STREAM, ENABLE);														// ʧ�ܷ���DMA(���з���ʱ�ٴ�)
}

void usart6_init(u32 bound)
{	
	/*�����ʼ����ؽṹ��*/
	GPIO_InitTypeDef 	GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	DMA_InitTypeDef   	DMA_InitStructure;

	
	/*����ʱ��*/			
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	
	
	/*���ö˿ڸ���*/	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6); //TX	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6); //RX
	
	
	/*����GPIO*/	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;   //PP
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	/*���ô���*/	
	USART_DeInit(USART6);
	USART_InitStructure.USART_BaudRate 				= bound;			
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;
	USART_InitStructure.USART_Parity 				= USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;	 
	USART_Init(USART6, &USART_InitStructure);
	
	
//	/*�����ж�*/
//	NVIC_InitStructure.NVIC_IRQChannel 						= USART6_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 	= 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority 			= 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd 					= ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

	
	/*���ý���DMA*/
	DMA_DeInit(USART6_RX_STREAM);
    DMA_InitStructure.DMA_Channel                 =    DMA_Channel_5;
    DMA_InitStructure.DMA_PeripheralBaseAddr      =    (uint32_t)&(USART6->DR);				// ����DMA�����������ַ����DMAӦ���ڴ���1�Ľ����У�
    DMA_InitStructure.DMA_Memory0BaseAddr         =    (uint32_t)UA6RxDMAbuf;				// ����DMA�����ڴ����ַ���û��Զ��壬����1���յ������ݽ����������棩
    DMA_InitStructure.DMA_DIR                     =    DMA_DIR_PeripheralToMemory;			// �������ݴ��䷽��������յ��ڴ棩
    DMA_InitStructure.DMA_BufferSize              =    38;						// ����DMAһ�δ����������Ĵ�С��500�ֽڣ�
    DMA_InitStructure.DMA_PeripheralInc           =    DMA_PeripheralInc_Disable;			// ���������ַ����
    DMA_InitStructure.DMA_MemoryInc               =    DMA_MemoryInc_Enable;				// �����ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize      =    DMA_PeripheralDataSize_Byte;			// ������������ݳ���Ϊ�ֽڣ�8bits��
    DMA_InitStructure.DMA_MemoryDataSize          =    DMA_MemoryDataSize_Byte;				// �����ڴ�����ݳ���Ϊ�ֽڣ�8bits��
    DMA_InitStructure.DMA_Mode                    =    DMA_Mode_Circular;					// ����DMAģʽΪѭ��ģʽ
    DMA_InitStructure.DMA_Priority                =    DMA_Priority_VeryHigh;				// ����DMAͨ�������ȼ�Ϊ������ȼ�
    DMA_InitStructure.DMA_FIFOMode                =    DMA_FIFOMode_Disable;				// ��ʹ��FIFOģʽ
    DMA_InitStructure.DMA_FIFOThreshold           =    DMA_FIFOThreshold_Full;				// FIFO��ֵ
    DMA_InitStructure.DMA_MemoryBurst             =    DMA_Mode_Normal;						// ʹ����ͨģʽ
    DMA_InitStructure.DMA_PeripheralBurst         =    DMA_PeripheralBurst_Single;			// ����ͻ�����δ���
    DMA_Init(USART6_RX_STREAM,&DMA_InitStructure);		

	
	
	/*���÷���DMA*/
	DMA_DeInit(USART6_TX_STREAM);
	while( DMA_GetCmdStatus(USART6_TX_STREAM) == ENABLE);									// �ȴ�DMA������
	DMA_InitStructure.DMA_Channel 				= DMA_Channel_5;
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (u32)&(USART6->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr 		= NULL;
	DMA_InitStructure.DMA_DIR 					= DMA_DIR_MemoryToPeripheral;				// �ڴ浽����
	DMA_InitStructure.DMA_BufferSize 			= NULL;
	DMA_InitStructure.DMA_PeripheralInc 		= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize 		= DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode 					= DMA_Mode_Normal;							// ��ͨģʽ����
	DMA_InitStructure.DMA_Priority 				= DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode 				= DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold 		= DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst 			= DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
	DMA_Init(USART6_TX_STREAM, &DMA_InitStructure);
	
	
	/*ʹ�ܴ��ڡ�DMA���ж�*/
    USART_ITConfig(USART6, USART_IT_IDLE, ENABLE);   										// ���������ж�
	USART_Cmd(USART6, ENABLE);                       										// ʹ�ܴ���
	USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);   										// ʹ�ܴ��ڵĽ���DMA
	USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);   										// ʹ�ܴ��ڵķ���DMA
    DMA_Cmd(USART6_RX_STREAM,ENABLE);														// ʹ��DMA
	DMA_Cmd(USART6_TX_STREAM,DISABLE);														// ʧ��DMA
}

