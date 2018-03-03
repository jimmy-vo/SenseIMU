#include "Dev_I2C.h"
#include <stdio.h>
#include "Timing_Manage.h"



#ifdef DEBUGi2c	 									 
int i2c_debug_Count;

void __I2C_Debug__(I2C *network, I2C_State num)
{
	if (i2c_debug_Count<DEBUGi2c)
		{
			network->I2C_State[i2c_debug_Count++] = num;
			if (num==null)
				{
					i2c_debug_Count=0;
					while (i2c_debug_Count<DEBUGi2c)
					{
								network->I2C_State[i2c_debug_Count++] = null;
					}
					i2c_debug_Count=0;
				}
		}		
}
#endif

I2C 							IMU, EEPROM;
DMA_InitTypeDef		DMA_I2C_InitStructure;

void Initialize_I2C(I2C* network, uint32_t BaudRate)
{
    I2C_InitTypeDef   								I2C_InitStructure;
		NVIC_InitTypeDef        					NVIC_InitStructure;
    
	/* I2C Periph clock enable */
		if 			(network->I2C == I2C1)    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		else if	(network->I2C == I2C2)    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
		else if	(network->I2C == I2C3)    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3, ENABLE);
	
    /* I2C Init */
		I2C_DeInit(network->I2C);
	
    I2C_InitStructure.I2C_Mode 								= I2C_Mode_I2C; 
    I2C_InitStructure.I2C_DutyCycle 					= I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 				= 0x00;
    I2C_InitStructure.I2C_Ack 								= I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed 					= BaudRate;
		
    I2C_Init(network->I2C, &I2C_InitStructure);
		
    /* I2C2 Init */
    I2C_Cmd(network->I2C, ENABLE);		
		
											
		/* Enable the Interrupt */	
		NVIC_PriorityGroupConfig(NVIC_GROUP_DEFAULT);
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = network->RX.preemtionPri;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = network->RX.SubPri;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		if			(network->I2C == I2C1)		NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
		else if (network->I2C == I2C2)		NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQn;
		else if (network->I2C == I2C3)		NVIC_InitStructure.NVIC_IRQChannel = I2C3_EV_IRQn;
		
		NVIC_Init(&NVIC_InitStructure);		
		
		I2C_ITConfig(network->I2C, I2C_IT_EVT | I2C_IT_BUF, ENABLE);															
																			
		/* Enable the DMA Interrupt */		
		if (network->TX.DMA == I2C_DMA_Enable)//TX
			{			
				
				NVIC_InitStructure.NVIC_IRQChannel = network->TX.DMA_NVIC_IRQ;
				
				NVIC_Init(&NVIC_InitStructure);
				
				DMA_ITConfig(network->TX.DMA_Stream, DMA_IT_TC, ENABLE);
				
				///DMA Config
				DMA_DeInit	(network->TX.DMA_Stream);
			
				DMA_I2C_InitStructure.DMA_Channel										= network->TX.DMA_Channel;			//tx channel
				DMA_I2C_InitStructure.DMA_PeripheralBaseAddr 				= (uint32_t)&network->I2C->DR;
				DMA_I2C_InitStructure.DMA_PeripheralInc							= DMA_PeripheralInc_Disable;
				DMA_I2C_InitStructure.DMA_MemoryInc 								= DMA_MemoryInc_Enable;
				DMA_I2C_InitStructure.DMA_PeripheralDataSize 				= DMA_PeripheralDataSize_Byte;
				DMA_I2C_InitStructure.DMA_MemoryDataSize 						= DMA_MemoryDataSize_Byte;
				DMA_I2C_InitStructure.DMA_Mode 											= DMA_Mode_Normal;
				DMA_I2C_InitStructure.DMA_Priority 									= DMA_Priority_VeryHigh;
				DMA_I2C_InitStructure.DMA_FIFOMode 									= DMA_FIFOMode_Enable;
				DMA_I2C_InitStructure.DMA_FIFOThreshold 						= DMA_FIFOThreshold_Full;
				DMA_I2C_InitStructure.DMA_MemoryBurst 							= DMA_MemoryBurst_Single;
				DMA_I2C_InitStructure.DMA_PeripheralBurst 					= DMA_PeripheralBurst_Single;
				
				DMA_Init		(network->TX.DMA_Stream, &DMA_I2C_InitStructure);
				
				RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);	
				
				DMA_ITConfig(network->TX.DMA_Stream, DMA_IT_TC, ENABLE);
				
				I2C_DMACmd(network->I2C,ENABLE);
			}						
		if (network->RX.DMA == I2C_DMA_Enable)//RX
			{	
				RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);	
				
				NVIC_InitStructure.NVIC_IRQChannel = network->RX.DMA_NVIC_IRQ;
				
				NVIC_Init(&NVIC_InitStructure);
				
				DMA_ITConfig(network->RX.DMA_Stream, DMA_IT_TC, ENABLE);
				
					
				///DMA Config		
				DMA_DeInit	(network->RX.DMA_Stream);			
				
				
				DMA_I2C_InitStructure.DMA_Channel										= network->RX.DMA_Channel;			//rx channel
				DMA_I2C_InitStructure.DMA_PeripheralBaseAddr 				= (uint32_t)&network->I2C->DR;
				DMA_I2C_InitStructure.DMA_PeripheralInc							= DMA_PeripheralInc_Disable;
				DMA_I2C_InitStructure.DMA_MemoryInc 								= DMA_MemoryInc_Enable;
				DMA_I2C_InitStructure.DMA_PeripheralDataSize 				= DMA_PeripheralDataSize_Byte;
				DMA_I2C_InitStructure.DMA_MemoryDataSize 						= DMA_MemoryDataSize_Byte;
				DMA_I2C_InitStructure.DMA_Mode 											= DMA_Mode_Normal;
				DMA_I2C_InitStructure.DMA_Priority 									= DMA_Priority_VeryHigh;
				DMA_I2C_InitStructure.DMA_FIFOMode 									= DMA_FIFOMode_Enable;
				DMA_I2C_InitStructure.DMA_FIFOThreshold 						= DMA_FIFOThreshold_Full;
				DMA_I2C_InitStructure.DMA_MemoryBurst 							= DMA_MemoryBurst_Single;
				DMA_I2C_InitStructure.DMA_PeripheralBurst 					= DMA_PeripheralBurst_Single;
				
				DMA_Init		(network->RX.DMA_Stream, &DMA_I2C_InitStructure);
				
				RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);	
				
				DMA_ITConfig(network->RX.DMA_Stream, DMA_IT_TC, ENABLE);
				
				I2C_DMACmd(network->I2C,ENABLE);
			}				 			
		
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 																																																												 ////
//// 																						For DMA MODE																																 ////
//// 																																																												 ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void I2C_DMA_Config(I2C* network, I2C_Phase Phase, uint8_t* Buffer, uint32_t BufferSize)
{
    switch (Phase)
			{
				case I2C_Phase_TX:									
								DMA_I2C_InitStructure.DMA_Channel										= network->TX.DMA_Channel;			//tx channel
								DMA_I2C_InitStructure.DMA_PeripheralBaseAddr 				= (uint32_t)&network->I2C->DR;
								DMA_I2C_InitStructure.DMA_Memory0BaseAddr 					= (uint32_t)Buffer;								
								DMA_I2C_InitStructure.DMA_DIR 											= DMA_DIR_MemoryToPeripheral;	//tx dma type
								DMA_I2C_InitStructure.DMA_BufferSize 								= BufferSize;											//tx dma length
								DMA_Init		(network->TX.DMA_Stream, &DMA_I2C_InitStructure);							
								break;
				case I2C_Phase_RX:					
								DMA_I2C_InitStructure.DMA_Channel										= network->RX.DMA_Channel;			//rx channel
								DMA_I2C_InitStructure.DMA_PeripheralBaseAddr 				= (uint32_t)&network->I2C->DR;
								DMA_I2C_InitStructure.DMA_Memory0BaseAddr 					= (uint32_t)Buffer;
								DMA_I2C_InitStructure.DMA_DIR 											= DMA_DIR_PeripheralToMemory;	//rx dma type
								DMA_I2C_InitStructure.DMA_BufferSize 								= BufferSize;											//rx dma length
								DMA_Init		(network->RX.DMA_Stream, &DMA_I2C_InitStructure);								
								break;
				default: break;
			}
}

I2C_Status I2C_Request(I2C* network,I2C_Phase Phase, uint8_t TXAddr, uint8_t RXAddr,  uint8_t *Register, uint16_t RegisterSize, uint8_t *Buffer, uint32_t BufferSize)
{
	uint32_t buffcount=0; 
	if (I2C_BUSY_Check(network)==I2C_STATUS_OK)
		{			
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, null);				
																																																														#endif			
			switch (Phase)
					{		
						case I2C_Phase_TX:		// case write	on register		
													network->Phase									=	I2C_Phase_TX;
													network->TX.SlaveAddr						= TXAddr;
													network->RX.SlaveAddr						= RXAddr;		
													network->Register.Address				= Register;	
													network->Register.RegSize				= RegisterSize;																													
													switch (network->TX.DMA)
														{
															case I2C_DMA_Enable:																																												
																						network->TX.BufSize						= BufferSize;		//this use in interrupt
																						I2C_DMA_Config(network, I2C_Phase_TX, Buffer, BufferSize);								
																						I2C_AcknowledgeConfig(network->I2C, ENABLE);																
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, TX_DMA_selected);				
																																																														#endif																				
																						break;	
															case I2C_DMA_Disable:																														
																						network->TX.BufSize							= BufferSize;						
																						network->TX.BufPos							=	0;								//reset counter									
																						network->TX.Buffer							=	Buffer;						//copy buffer address
																						for (buffcount=0; buffcount<BufferSize ; buffcount++)	//copy contain
																						*(network->TX.Buffer + buffcount) = *(Buffer + buffcount);	

																						I2C_AcknowledgeConfig(network->I2C, ENABLE);
																						I2C_ITConfig(network->I2C, I2C_IT_EVT | I2C_IT_BUF, ENABLE);	
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, TX_IT_selected);				
																																																														#endif														
																						break;
														}
													break;	
						case I2C_Phase_COMMAND:			
													network->Phase									=	I2C_Phase_TX;
													network->TX.SlaveAddr						= TXAddr;
													network->RX.SlaveAddr						= RXAddr;		
													network->Register.Address				= Register;	
													network->Register.RegSize				= RegisterSize;													
													network->TX.BufSize						= 0;									
													I2C_AcknowledgeConfig(network->I2C, ENABLE);	
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, COMMAND_selected);				
																																																														#endif
													break;		
						case I2C_Phase_RX:																																																							
													network->Phase									=	I2C_Phase_TXREG;				//send command first
													network->TX.SlaveAddr						= TXAddr;
													network->RX.SlaveAddr						= RXAddr;		
													network->Register.Address				= Register;	
													network->Register.RegSize				= RegisterSize;			
													switch (network->RX.DMA)
														{
															case I2C_DMA_Enable:						
																						if (BufferSize==1) BufferSize=2;									// 1 does not work for dma
																						I2C_DMA_Config(network, I2C_Phase_RX, Buffer, BufferSize);													
																						I2C_DMALastTransferCmd(network->I2C, ENABLE);			// Enable DMA NACK automatic
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, RX_DMA_selected);				
																																																														#endif
																						break;	
															case I2C_DMA_Disable:
																						//prepare RX buffer
																						network->RX.BufSize							= BufferSize;							
																						network->RX.BufPos							=	0;								//reset counter			
																						network->RX.Buffer							=	Buffer;

																						I2C_AcknowledgeConfig(network->I2C, ENABLE);
																						I2C_ITConfig(network->I2C, I2C_IT_EVT | I2C_IT_BUF, ENABLE);	
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, RX_IT_selected);				
																																																														#endif
																						break;
														}
													break;
						default:			break;
					}		
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, Request_done);				
																																																														#endif
			/* Start condition */
			I2C_GenerateSTART(network->I2C, ENABLE);
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, START_bit_Sent);				
																																																														#endif			
			//check if jump into interrupt
			return I2C_STATUS_OK;
		}
		else 
			return I2C_STATUS_BUSY;	
}

void I2C_Interupt(I2C *network)
{
	switch (I2C_GetLastEvent(network->I2C))
	{
			case I2C_EVENT_MASTER_MODE_SELECT:																																													
							switch (network->Phase)				//send device address	
								{
									case I2C_Phase_TX:      		
																I2C_Send7bitAddress(network->I2C, network->TX.SlaveAddr, I2C_Direction_Transmitter);
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, TX_Address_sent);				
																																																														#endif	
																break;
									case I2C_Phase_TXREG:		    
																I2C_Send7bitAddress(network->I2C, network->TX.SlaveAddr, I2C_Direction_Transmitter);
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, RX_Address_sent1);				
																																																														#endif	
																break;
									case I2C_Phase_RX: 		 
																I2C_Send7bitAddress(network->I2C, network->RX.SlaveAddr, I2C_Direction_Receiver);
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, RX_Address_sent2);				
																																																														#endif
																break;
									default: 			break;
								}
							break;
			
			case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:																															
 							I2C_SendData(network->I2C, *(network->Register.Address++));	// send register address
 							network->Register.RegSize--;				
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, First_REG_sent);				
																																																														#endif			
							if(network->Phase == I2C_Phase_COMMAND)
								{			
										I2C_AcknowledgeConfig(network->I2C, DISABLE);
										I2C_GenerateSTOP(network->I2C, ENABLE);		
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, COMMAND_STOP_bit_Sent);				
																																																														#endif				
								}
							break;
								
			case I2C_EVENT_MASTER_BYTE_TRANSMITTED:																																													
							if (network->Register.RegSize)
							 {
									I2C_SendData(network->I2C, *(network->Register.Address++));	// send register address				
									network->Register.RegSize--;
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, Next_REG_sent);				
																																																														#endif				
							 }
							else if(network->Phase == I2C_Phase_TX)
								{
									switch (network->TX.DMA)
										{
											case I2C_DMA_Enable:
																				DMA_Cmd(network->TX.DMA_Stream, ENABLE);	// Enable DMA Channel for RX 							
																				network->Phase = I2C_Phase_DMA;		
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, TX_enable_DMA);				
																																																														#endif			
																				break;
											case I2C_DMA_Disable:	
																			if( network->TX.BufPos < network->TX.BufSize)	// send until the end of buffer	
																				{
																					I2C_SendData(network->I2C, *(network->TX.Buffer++));
																					network->TX.BufPos++;
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, TX_Sending_DATA);				
																																																														#endif			
																				}
																				else		//send stop if last byte is transfered		
																					{
																						I2C_GenerateSTOP(network->I2C, ENABLE);
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, TX_STOP_bit_Sent);				
																																																														#endif			
																					}
										}											
								}
								else if(network->Phase == I2C_Phase_TXREG)														//Generate second start and switch to receiver mode											
									{										
										I2C_GenerateSTART(network->I2C,ENABLE);																																			
										network->Phase = I2C_Phase_RX;	
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, START_bit_Sent2);				
																																																														#endif				
									}
							break;
				
			case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:																																					
							if (network->RX.DMA == I2C_DMA_Enable)
								{																																																		
									DMA_Cmd(network->RX.DMA_Stream, ENABLE);	    								// Enable DMA Channel for RX 
									network->Phase = I2C_Phase_DMA;					
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, RX_enable_DMA);				
																																																														#endif									
								}
							break;	
								
			case I2C_EVENT_MASTER_BYTE_RECEIVED:																																								
							if (network->RX.DMA==I2C_DMA_Disable)
								{		
									if (network->RX.BufPos == network->RX.BufSize-1)										//if last byte => stop
										{							
											I2C_AcknowledgeConfig(network->I2C, DISABLE);
											I2C_GenerateSTOP(network->I2C, ENABLE);	
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, RX_STOP_bit_Sent);				
																																																														#endif	
										}
									*(network->RX.Buffer++) = 		I2C_ReceiveData(network->I2C);			//recieve data
									network->RX.BufPos++;										
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(network, RX_Recieving_DATA);				
																																																														#endif
								}
							break;							
			default:						break;		
	}			
}

void I2C1_RX_DMA_IT(void) 
{	
  if (DMA_GetITStatus	(I2C1_RX_DMA_ST, I2C1_RX_DMA_FLAG))
  {			
    DMA_ClearITPendingBit(I2C1_RX_DMA_ST,  I2C1_RX_DMA_FLAG);
		
		DMA_Cmd(I2C1_RX_DMA_ST, DISABLE);	
		I2C_GenerateSTOP(I2C1, ENABLE);		
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(&IMU, RX_DMA_TC);				
																																																														#endif
	}
}
void I2C1_TX_DMA_IT(void) 
{		
  if (DMA_GetITStatus	(I2C1_TX_DMA_ST, I2C1_TX_DMA_FLAG))
  {		
    DMA_ClearITPendingBit(I2C1_TX_DMA_ST,  I2C1_TX_DMA_FLAG);
																				
		DMA_Cmd(I2C1_TX_DMA_ST, DISABLE);																																												
		I2C_LAST_TRANSMIT_Check(&IMU);
		I2C_GenerateSTOP(I2C1, ENABLE);	
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(&IMU, TX_DMA_TC);				
																																																														#endif
	}
}
void I2C2_RX_DMA_IT(void) 
{	
  if (DMA_GetITStatus	(I2C2_RX_DMA_ST, I2C2_RX_DMA_FLAG))
  {		
    DMA_ClearITPendingBit(I2C2_RX_DMA_ST,  I2C2_RX_DMA_FLAG);	
		
		I2C_GenerateSTOP(I2C2, ENABLE);	
		DMA_Cmd(I2C2_RX_DMA_ST, DISABLE);																																																															
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(&EEPROM, TX_DMA_TC);				
																																																														#endif
	}
}

void I2C2_TX_DMA_IT(void) 
{		
  if (DMA_GetITStatus	(I2C2_TX_DMA_ST, I2C2_TX_DMA_FLAG))
  {			
    DMA_ClearITPendingBit(I2C2_TX_DMA_ST,  I2C2_TX_DMA_FLAG);
		
		DMA_Cmd(I2C2_TX_DMA_ST, DISABLE);																																																															
		I2C_LAST_TRANSMIT_Check(&EEPROM);
		I2C_GenerateSTOP(I2C2, ENABLE);																																											
																																																														#ifdef DEBUGi2c	
																																																															__I2C_Debug__(&EEPROM, TX_DMA_TC);				
																																																														#endif
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 																																																												 ////
//// 																				I2C	Interupt Handler																														 ////
//// 																																																												 ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void I2C1_EV_IRQHandler(void)
{
		I2C_Interupt(&IMU);
}

void I2C2_EV_IRQHandler(void)
{
		I2C_Interupt(&EEPROM);
}

void I2C1_ER_IRQHandler(void)
{																																																															
  if (I2C_GetITStatus(I2C1, I2C_IT_AF))
  {
    I2C_ClearITPendingBit(I2C1, I2C_IT_AF);																									
  }
}

void I2C2_ER_IRQHandler(void)
{																																																														
  if (I2C_GetITStatus(I2C2, I2C_IT_AF))
  {
    I2C_ClearITPendingBit(I2C2, I2C_IT_AF);																																								
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 																																																												 ////
//// 																					User Code																																			 ////
//// 																																																												 ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

I2C_Status I2C_BUSY_Check (I2C* network)
{
	uint32_t zero_time = Get_msTick();
	while (1)
	{
		if (I2C_GetFlagStatus (network->I2C, I2C_FLAG_BUSY) != SET) 				return I2C_STATUS_OK;		
		if(Check_msTick(zero_time,network->TimeOut)) 												return I2C_STATUS_TIMEOUT;
	}
}

I2C_Status I2C_EVENT_Check (I2C* network, uint32_t I2C_EVENT)
{
	uint32_t zero_time = Get_msTick();
	while (1)
	{		
		if(I2C_CheckEvent (network->I2C, I2C_EVENT) == SET) 			return I2C_STATUS_OK;				
		if(Check_msTick(zero_time,network->TimeOut)) 						return I2C_STATUS_TIMEOUT;
	}
}

I2C_Status I2C_LAST_TRANSMIT_Check (I2C* network)
{
	uint32_t zero_time = Get_msTick();
	while (1)
	{
		if (I2C_GetFlagStatus (network->I2C, I2C_FLAG_BTF) != RESET) 		return I2C_STATUS_OK;					
		if(Check_msTick(zero_time,network->TimeOut)) return I2C_STATUS_TIMEOUT;
	}
}
