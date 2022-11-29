#include "uart_protocol.h"
#include "CRC_TABLE.h"

/*************************************************************************
�� �� ����Get_CRC8_Check_Sum
�������ܣ�����CRC8�㷨�����������ݵ�CRC8������
��    ע��
*************************************************************************/
u8 Get_CRC8_Check_Sum(u8 *pchMessage,u32 dwLength,u8 ucCRC8)
{
    u8 ucIndex;
    while (dwLength--)
    {
        ucIndex = ucCRC8^(*pchMessage++);//����������λ�������
        ucCRC8 = CRC8_TAB[ucIndex];
    }
    return(ucCRC8);
}



/*************************************************************************
�� �� ����Verify_CRC8_Check_Sum
�������ܣ�CRC8����
��    ע�����ؼ�����0�����󣩻�1����ȷ��
*************************************************************************/
u32 Verify_CRC8_Check_Sum(u8 *pchMessage, u32 dwLength)
{
    u8 ucExpected = 0;
    if ((pchMessage == 0) || (dwLength <= 2)) 
        return 0;
    ucExpected = Get_CRC8_Check_Sum (pchMessage, dwLength-1, CRC8_INIT);
    return ( ucExpected == pchMessage[dwLength-1] );
}



/*************************************************************************
�� �� ����Append_CRC8_Check_Sum
�������ܣ�����������β���CRC8������
��    ע��dwLength = Data + chechsum
*************************************************************************/
void Append_CRC8_Check_Sum(u8 *pchMessage, u32 dwLength)
{
    u8 ucCRC = 0;
    if ((pchMessage == 0) || (dwLength <= 2)) return;

    ucCRC = Get_CRC8_Check_Sum ( (u8 *)pchMessage, dwLength-1, CRC8_INIT);
    pchMessage[dwLength-1] = ucCRC;
}



/*************************************************************************
�� �� ����Get_CRC16_Check_Sum
�������ܣ�����CRC16�㷨�����������ݵ�CRC16������
��    ע��
*************************************************************************/
u16 Get_CRC16_Check_Sum(u8 *pchMessage,u32 dwLength,u16 wCRC)
{
	u8 chData;
	if (pchMessage == 0) return 0xFFFF;
	while(dwLength--)
	{
        chData = *pchMessage++;
        (wCRC) = ((u16)(wCRC) >> 8) ^ CRC16_Table[((u16)(wCRC) ^(u16)(chData)) & 0x00ff];
	}
	return wCRC;
}



/*************************************************************************
�� �� ����Verify_CRC16_Check_Sum
�������ܣ�CRC16����
��    ע�����ؼ�����0�����󣩻�1����ȷ��
*************************************************************************/
u32 Verify_CRC16_Check_Sum(u8 *pchMessage, u32 dwLength)
{
	u16 wExpected = 0;
	if ((pchMessage == 0) || (dwLength <= 2)) return 0;
	wExpected = Get_CRC16_Check_Sum ( pchMessage, dwLength - 2, CRC16_INIT);
	return ((wExpected & 0xFF) == pchMessage[dwLength - 2] && ((wExpected >> 8) & 0xFF)== pchMessage[dwLength- 1]);
}



/*************************************************************************
�� �� ����Append_CRC16_Check_Sum
�������ܣ�����������β���CRC16������
��    ע��dwLength = Data + chechsum
*************************************************************************/
void Append_CRC16_Check_Sum(u8 * pchMessage,u32 dwLength)
{
	u16 wCRC = 0;
	if ((pchMessage == 0) || (dwLength <= 2)) return;
	wCRC = Get_CRC16_Check_Sum ( (u8 *)pchMessage, dwLength-2, CRC16_INIT );
	pchMessage[dwLength-2] = (u8)(wCRC & 0x00ff);
	pchMessage[dwLength-1] = (u8)((wCRC >> 8)& 0x00ff);
}


void imu_sendata()
{
	SendBuf.PitchAngle=BMIPitchAngle;
	SendBuf.PitchSpeed=BMIPitchSpeed;
	SendBuf.YawAngle=BMIYawAngle;
	SendBuf.YawSpeed=BMIYawSpeed;
	Append_CRC8_Check_Sum(&SendBuf.head[0],sizeof(SendBuf));
}
/*---------------------����2dma����ʹ��----------------------*/
void usart2_dma_rx()
{
	DMA_ClearITPendingBit(DMA1_Stream5,DMA_IT_TCIF5);
	DMA_Cmd(DMA1_Stream5,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Stream5,sizeof(cushioning)); 
	DMA_ITConfig(DMA1_Stream5,DMA_IT_TE|DMA_IT_TC, ENABLE);
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	DMA_Cmd(DMA1_Stream5, ENABLE);
}
/*---------------------����3dma����ʹ��----------------------*/
void usart3_dma_tx()
{
		DMA_ClearITPendingBit(DMA1_Stream3,DMA_IT_TCIF3);
		imu_sendata();
		DMA_Cmd(DMA1_Stream3,DISABLE);
		DMA1_Stream4->M0AR=(u32)(&SendBuf);
		DMA1_Stream4->NDTR=(u32)(sizeof(SendBuf));
		DMA_ITConfig(DMA1_Stream3,DMA_IT_TCIF3, ENABLE);
		USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
		DMA_Cmd(DMA1_Stream3, ENABLE);
}
/*---------------------����3dma����ʹ��----------------------*/
void usart3_dma_rx()
{
	DMA_ClearITPendingBit(DMA1_Stream1,DMA_IT_TCIF1);
	DMA_Cmd(DMA1_Stream1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Stream1,sizeof(cushioning)); 
	DMA_ITConfig(DMA1_Stream1,DMA_IT_TE|DMA_IT_TC, ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	DMA_Cmd(DMA1_Stream1, ENABLE);
}
/*---------------------����4dma����ʹ��----------------------*/
void uart4_dma_tx()
{
		DMA_ClearITPendingBit(DMA1_Stream4,DMA_IT_TCIF4);
		imu_sendata();
		DMA_Cmd(DMA1_Stream4,DISABLE);
		DMA1_Stream4->M0AR=(u32)(&SendBuf);
		DMA1_Stream4->NDTR=(u32)(sizeof(SendBuf));
		DMA_ITConfig(DMA1_Stream4,DMA_IT_TCIF4, ENABLE);
		USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);
		DMA_Cmd(DMA1_Stream4, ENABLE);
}

/*---------------------����4dma����ʹ��----------------------*/
void uart4_dma_rx()
{
		DMA_ClearITPendingBit(DMA1_Stream2,DMA_IT_TCIF2);
		DMA_Cmd(DMA1_Stream2,DISABLE);
		DMA1_Stream2->M0AR=(u32)(&cushioning);
		DMA1_Stream2->NDTR=(u32)(sizeof(cushioning));
		DMA_ITConfig(DMA1_Stream2,DMA_IT_TCIF2, ENABLE);
		USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);
		DMA_Cmd(DMA1_Stream2, ENABLE);
}



/*----------------------crc8��ͨѶЭ��-----------------------*/
void usart2_protocol_rx()
{
	//u8 state_num=D_S0;								//����״̬				//���ʹ����DMA�����Ѿ���������ݰ����壬��û�б�Ҫʹ��״̬��
	for(int i=0;i<(sizeof(cushioning)/sizeof(u8));i++)
	{
		if(cushioning[i]==FRAME_HEAD1&&cushioning[++i]==FRAME_HEAD2)//�����iλ��֡ͷ
		{			
			switch (cushioning[++i])
			{
				case ID_1:
				{
					i++;
					if(Verify_CRC8_Check_Sum((cushioning+i+1),cushioning[i]))
						memcpy(res_data,cushioning+i+1,cushioning[i]);
					else
						i+=SendBufSize+2;//Ŀǰ��iλ�����ݳ���;��1������crc;��2������֡β
					break;
				}
				default:
					break;
			}
		}
	}
}

/*----------------------crc8��ͨѶЭ��-----------------------*/
void usart3_protocol_rx()
{
		if(cushioning[0]==0x55&&cushioning[1]==0x22&&Verify_CRC8_Check_Sum(cushioning,13))
	{
		DOWN_PLATFOM_STATUS=cushioning[2];
		DOWN_SHOOT_STATUS=cushioning[3];
//		memcpy(&PITCH_POS_DES,&cushioning[4],4);
		memcpy(&DOWN_SHOOT_FREQ,&cushioning[8],4);
		if(DOWN_SHOOT_FREQ<0)
			DOWN_SHOOT_FREQ=0;
	}
}

/*----------------------crc8��ͨѶЭ��-----------------------*/
void uart4_protocol_rx()
{
	//u8 state_num=D_S0;								//����״̬				//���ʹ����DMA�����Ѿ���������ݰ����壬��û�б�Ҫʹ��״̬��
	for(int i=0;i<(sizeof(cushioning)/sizeof(u8));i++)
	{
		if(cushioning[i]==FRAME_HEAD1&&cushioning[++i]==FRAME_HEAD2)//�����iλ��֡ͷ
		{			
			switch (cushioning[++i])
			{
				case ID_1:
				{
					i++;
					if(Verify_CRC8_Check_Sum((cushioning+i+1),cushioning[i]))
						memcpy(res_data,cushioning+i+1,cushioning[i]);
					else
						i+=SendBufSize+2;//Ŀǰ��iλ�����ݳ���;��1������crc;��2������֡β
					break;
				}
				default:
					break;
			}
		}
	}
}



