#include "Dev_USART.h"
#include <stdio.h>
#include <stdlib.h>
#include "Timing_Manage.h"
#include "Serial_Link.h"

USART USART_1;
USART USART_3;

DMA_InitTypeDef  DMA_USART_InitStructure;	

uint8_t DevUSART_BUFF [3];

void USART_OpenPort (USART * serial, uint32_t BaudRate)
	{
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
		if 			(serial->USART == USART1)	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		else if (serial->USART == USART2)	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		else if (serial->USART == USART3)	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		else if (serial->USART == UART4)	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
		else if (serial->USART == UART5)	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
		else if (serial->USART == USART6)	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
		
		/* Open Serial Port */
		USART_InitStructure.USART_BaudRate = BaudRate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(serial->USART, &USART_InitStructure);	
		
		
		/* If request, Configure DMA for TX */		
		NVIC_PriorityGroupConfig(NVIC_GROUP_DEFAULT);
		
		switch  (serial->TX.USART_DMA)	
			{
				case USART_DMA_Enable:
																if 	(serial->TX.DMA_Stream < DMA2_Stream0)   	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
																		else																			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
																	
																DMA_DeInit(serial->TX.DMA_Stream);			 
																	DMA_USART_InitStructure.DMA_Channel 											= serial->TX.DMA_Channel;
																	DMA_USART_InitStructure.DMA_DIR 													= DMA_DIR_MemoryToPeripheral; // Transmit
																	DMA_USART_InitStructure.DMA_PeripheralBaseAddr						= (uint32_t)&serial->USART->DR;
																	DMA_USART_InitStructure.DMA_PeripheralInc 								= DMA_PeripheralInc_Disable;
																	DMA_USART_InitStructure.DMA_MemoryInc 										= DMA_MemoryInc_Enable;
																	DMA_USART_InitStructure.DMA_PeripheralDataSize 						= DMA_PeripheralDataSize_Byte;
																	DMA_USART_InitStructure.DMA_MemoryDataSize 								= DMA_MemoryDataSize_Byte;
																	DMA_USART_InitStructure.DMA_Mode 													= DMA_Mode_Circular;
																	DMA_USART_InitStructure.DMA_Priority 											= DMA_Priority_High;
																	DMA_USART_InitStructure.DMA_FIFOMode 											= DMA_FIFOMode_Enable;
																	DMA_USART_InitStructure.DMA_FIFOThreshold 								= DMA_FIFOThreshold_Full;
																	DMA_USART_InitStructure.DMA_MemoryBurst										= DMA_MemoryBurst_Single;
																	DMA_USART_InitStructure.DMA_PeripheralBurst								= DMA_PeripheralBurst_Single;
																DMA_Init(serial->TX.DMA_Stream, &DMA_USART_InitStructure); 
																
																/* Enable the DMA Interrupt */
																	NVIC_InitStructure.NVIC_IRQChannel 												= serial->TX.DMA_NVIC_IRQ;
																	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 			= serial->TX.preemtionPri;
																	NVIC_InitStructure.NVIC_IRQChannelSubPriority 						= serial->TX.SubPri;
																	NVIC_InitStructure.NVIC_IRQChannelCmd 										= ENABLE;
																NVIC_Init(&NVIC_InitStructure);

																/* Enable interrupt */
																DMA_ITConfig(serial->TX.DMA_Stream, DMA_IT_TC, ENABLE);
																																
																/* Enable the USART Tx DMA request */
																USART_DMACmd(serial->USART, USART_DMAReq_Tx, ENABLE);  
																
																/* Enable the DMA TX Stream */
																DMA_Cmd(serial->TX.DMA_Stream, DISABLE);																
																break;
					case USART_DMA_Disable:					
																/* Enable the USART Interrupt */
																	NVIC_InitStructure.NVIC_IRQChannel 												= serial->IT_NVIC_IRQ;
																	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 			= serial->TX.preemtionPri;
																	NVIC_InitStructure.NVIC_IRQChannelSubPriority 						= serial->TX.SubPri;
																	NVIC_InitStructure.NVIC_IRQChannelCmd 										= ENABLE;
																NVIC_Init(&NVIC_InitStructure);					
					
																/* Enable USART Receive interrupts */
																USART_ITConfig(serial->USART, USART_IT_TXE, DISABLE);		
																break;
			}				
		
		/* If request, Configure DMA for RX */
		switch  (serial->RX.USART_DMA)	
			{
				case USART_DMA_Enable:
																if 	(serial->RX.DMA_Stream < DMA2_Stream0)  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
																		else																		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
																
																	DMA_DeInit(serial->RX.DMA_Stream);			 
																	DMA_USART_InitStructure.DMA_Channel 											= serial->RX.DMA_Channel;
																	DMA_USART_InitStructure.DMA_DIR 													= DMA_DIR_PeripheralToMemory; // Receive
																	DMA_USART_InitStructure.DMA_PeripheralBaseAddr						= (uint32_t)&serial->USART->DR;
																	DMA_USART_InitStructure.DMA_PeripheralInc 								= DMA_PeripheralInc_Disable;
																	DMA_USART_InitStructure.DMA_MemoryInc 										= DMA_MemoryInc_Enable;
																	DMA_USART_InitStructure.DMA_PeripheralDataSize 						= DMA_PeripheralDataSize_Byte;
																	DMA_USART_InitStructure.DMA_MemoryDataSize 								= DMA_MemoryDataSize_Byte;
																	DMA_USART_InitStructure.DMA_Mode 													= DMA_Mode_Circular;
																	DMA_USART_InitStructure.DMA_Priority 											= DMA_Priority_High;
																	DMA_USART_InitStructure.DMA_FIFOMode 											= DMA_FIFOMode_Enable;
																	DMA_USART_InitStructure.DMA_FIFOThreshold 								= DMA_FIFOThreshold_Full;
																	DMA_USART_InitStructure.DMA_MemoryBurst										= DMA_MemoryBurst_Single;
																	DMA_USART_InitStructure.DMA_PeripheralBurst								= DMA_PeripheralBurst_Single;																																						
																											DMA_USART_InitStructure.DMA_Channel							= serial->RX.DMA_Channel;			//rx channel
																											DMA_USART_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)DevUSART_BUFF;
																											DMA_USART_InitStructure.DMA_BufferSize 					= 12;									//rx dma length
																DMA_Init(serial->RX.DMA_Stream, &DMA_USART_InitStructure); 
																
																/* Enable the DMA Interrupt */
																	NVIC_InitStructure.NVIC_IRQChannel 												= serial->RX.DMA_NVIC_IRQ;
																	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 			= serial->RX.preemtionPri;
																	NVIC_InitStructure.NVIC_IRQChannelSubPriority 						= serial->RX.SubPri;
																	NVIC_InitStructure.NVIC_IRQChannelCmd 										= ENABLE;
																NVIC_Init(&NVIC_InitStructure);
																
																/* Enable interrupt */
																DMA_ITConfig(serial->RX.DMA_Stream, DMA_IT_TC, ENABLE);																																			
																
																/* Enable the USART Rx DMA request */
																USART_DMACmd(serial->USART, USART_DMAReq_Rx, ENABLE); 
																
																/* Enable the DMA RX Stream */
																DMA_Cmd(serial->RX.DMA_Stream, ENABLE);	
																break;
					case USART_DMA_Disable:					
																/* Enable the USART Interrupt */
																	NVIC_InitStructure.NVIC_IRQChannel 												= serial->IT_NVIC_IRQ;
																	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 			= serial->RX.preemtionPri;
																	NVIC_InitStructure.NVIC_IRQChannelSubPriority 						= serial->RX.SubPri;
																	NVIC_InitStructure.NVIC_IRQChannelCmd 										= ENABLE;
																NVIC_Init(&NVIC_InitStructure);					
					
																/* Enable USART Receive interrupts */
																USART_ITConfig(serial->USART, USART_IT_RXNE, DISABLE);		
																break;
				}
						
		USART_Cmd(serial->USART, ENABLE);
	}
void  USART_DMA_Set (USART * serial,USART_DIR DIR, uint8_t *Buffer, uint32_t BufferSize, USART_MODE MODE)
{																																																																	
	switch (DIR)
		{		
			case USART_TX:																													
										DMA_Cmd(serial->TX.DMA_Stream,DISABLE);		
										serial->TX.USART_DMA 														= USART_DMA_Enable;	
										serial->TX.Mode			 														= MODE;												
										DMA_USART_InitStructure.DMA_Channel							= serial->TX.DMA_Channel;			//tx channel
										DMA_USART_InitStructure.DMA_PeripheralBaseAddr	= (uint32_t)&serial->USART->DR;
										DMA_USART_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)Buffer;								
										DMA_USART_InitStructure.DMA_DIR 								= DMA_DIR_MemoryToPeripheral;	//tx dma type
										DMA_USART_InitStructure.DMA_BufferSize 					= BufferSize;									//tx dma length															
										DMA_Init		(serial->TX.DMA_Stream, &DMA_USART_InitStructure);																											
										DMA_ITConfig(serial->TX.DMA_Stream, DMA_IT_TC, ENABLE);						
										DMA_Cmd(serial->TX.DMA_Stream, ENABLE);			
										break;	
			case USART_RX:																																																			
										DMA_Cmd(serial->RX.DMA_Stream, DISABLE);							
										serial->RX.USART_DMA 														=	USART_DMA_Enable;																												
										DMA_USART_InitStructure.DMA_Channel							= serial->RX.DMA_Channel;			//rx channel
										DMA_USART_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&serial->USART->DR;
										DMA_USART_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)Buffer;
										DMA_USART_InitStructure.DMA_BufferSize 					= BufferSize;									//rx dma length
										DMA_USART_InitStructure.DMA_DIR 								= DMA_DIR_PeripheralToMemory;	//rx dma type
										DMA_Init		(serial->RX.DMA_Stream, &DMA_USART_InitStructure);		
										serial->RX.Mode 																= MODE;		
										DMA_Cmd(serial->RX.DMA_Stream, ENABLE);						
										break;
		}		
		
}

void  USART_Interrupt_Set (USART * serial,USART_DIR DIR, uint8_t *Buffer, uint32_t BufferSize, USART_MODE MODE)
{																																																																	
	switch (DIR)
		{		
			case USART_TX:																							
										serial->TX.USART_DMA 														= USART_DMA_Disable;				
										serial->TX.Buffer 															= Buffer;																		
										serial->TX.BuffPos 															= 0;																
										serial->TX.BuffSize 														= BufferSize;
										serial->TX.Mode																	= MODE;	
										USART_ITConfig(serial->USART, USART_IT_TXE, ENABLE);		
										break;
			case USART_RX:		
										serial->RX.USART_DMA 														= USART_DMA_Disable;	
										serial->RX.Buffer 															= Buffer;																		
										serial->RX.BuffPos 															= 0;																
										serial->RX.BuffSize 														= BufferSize;
										serial->RX.Mode 																= MODE;		
										USART_ITConfig(serial->USART, USART_IT_RXNE, ENABLE);																		
										break;
		}				
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																																						 ////
////DMA MODE
//

void UART1_TX_DMA_IT(void) 
{	
	if (DMA_GetITStatus	(UART1_TX_DMA_ST, UART1_TX_DMA_FLAG))
	{
		DMA_ClearITPendingBit(UART1_TX_DMA_ST,  UART1_TX_DMA_FLAG);				
		DMA_Cmd(UART1_TX_DMA_ST, DISABLE);																																												
	}
}

void UART1_RX_DMA_IT(void)
{
	if (DMA_GetITStatus	(UART1_RX_DMA_ST, UART1_RX_DMA_FLAG))
	{
		DMA_ClearITPendingBit(UART1_RX_DMA_ST, UART1_RX_DMA_FLAG);																																	
	} 
}

void UART3_TX_DMA_IT(void) 
{	
  if (DMA_GetITStatus	(UART3_TX_DMA_ST, UART3_TX_DMA_FLAG))
	{					
		switch (USART_3.TX.Mode)
		{
			case USART_MODE_Continuos:
																		SerialLink_DMATX_Process();
																		DMA_ClearITPendingBit(UART3_TX_DMA_ST,  UART3_TX_DMA_FLAG);	
																		break;
			case USART_MODE_Single:												
																		DMA_Cmd(UART3_TX_DMA_ST, DISABLE);			
																		DMA_ClearITPendingBit(UART3_TX_DMA_ST,  UART3_TX_DMA_FLAG);	
																		break;	
		}
	}
}

void UART3_RX_DMA_IT(void)
{
  if (DMA_GetITStatus	(UART3_RX_DMA_ST, UART3_RX_DMA_FLAG))
  {
    DMA_ClearITPendingBit(UART3_RX_DMA_ST, UART3_RX_DMA_FLAG);
  }
 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////INTERRUPT MODE
//
void USART1_IRQHandler(void)
{
	uint8_t intput;
	if( USART_GetITStatus(USART2, USART_IT_RXNE) )
		{ 																																			  		
			intput = 	USART_ReceiveData(USART2);																																											
// 			if (intput=='*') USART_3.RX.BuffPos =0;	else																					//first uint8_t protocol
			*(USART_1.RX.Buffer + USART_1.RX.BuffPos++) 			= 				intput;	
			if (USART_1.RX.BuffPos >= USART_1.RX.BuffSize) 							USART_1.RX.BuffPos = 0;  	//for ring buffer
		}
	
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
		{																																																															
			USART_SendData(USART2, *(USART_1.TX.Buffer + USART_1.TX.BuffPos++));		
			if (USART_1.TX.BuffPos >= USART_1.TX.BuffSize) 						//Disable the UART4 Transmit interrupt
				USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		}
}

void USART3_IRQHandler(void)
{
	if( USART_GetITStatus(USART3, USART_IT_RXNE) )
		{  		
			SerialLink_INPUT_INTERRUPT(USART_ReceiveData(USART3));		
		}		
	if(USART_GetITStatus(USART3, USART_IT_TXE))
		{  			
			USART_SendData(USART3, *(USART_3.TX.Buffer + USART_3.TX.BuffPos++));		
			if (USART_3.TX.BuffPos >= USART_3.TX.BuffSize) 						
			{
				//Disable the Transmit interrupt
				USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
				
				//Turnon Stream
				//SerialLink_DMAProcess();
			}
		}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																																																	 ////
////USER CODE
//

