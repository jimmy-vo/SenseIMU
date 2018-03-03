#include "Serial_Link.h"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h> 
#include "Dev_USART.h"
#include "RGB.h"
#include "Timing_Manage.h"
#include "Parameter.h"
#include "Algorithm.h"


#define  SERIALINK_CONSOLE_BUFFSIZE 									100
#define  SERIALINK_CONSOLE_STACKSIZE 									1000

#define  SERIALINK_PARAM_BUFFSIZE 										PARA_MODE_4 + 4 //protocol VND$ 
#define  SERIALINK_PARAM_STACKSIZE 										PARA_TYPE_MAX 

#define  SERIALINK_STREAM_BUFFSIZE 										100 

#define  SERIALINK_INPUT_BUFFSIZE 										20

DATA_STREAM DATA_OUT ;

static SERIALLINK_MODE 				SerialLink_Mode 				= SERIALLINK_MODE_Stream;
static SERIALLINK_INPUT_MODE 	SerialLink_Input_Mode 	= SERIALLINK_INPUT_MODE_None;

///////////////////////////////////////////////////////////////////////////	
////TOOLs	
//	
char *byte_to_binary(int x, uint8_t size)
{
	int z;
	static char b[32];
    for (z = 0; z < size; z++) 
        b[size-z-1] = ((x>>z) & 0x1) ? '1' : '0';
    return b;
}
///////////////////////////////////////////////////////////////////////////
////SerialLink INPUT Process
//
static SERIALLINK_INPUT_CMD SerialLink_Input_CMD;
static uint8_t SerialLink_Input [SERIALINK_INPUT_BUFFSIZE];

static uint8_t SerialLink_Input_cnt = 0;
static uint8_t SerialLink_Input_match = 0;

void SerialLink_INPUT_INTERRUPT (uint16_t INPUT)
{	
	switch (SerialLink_Input_Mode)
	{
		case SERIALLINK_INPUT_MODE_None:
							switch (INPUT)
							{
								case 'V': 
													SerialLink_Input_match = 1;
													break;
								case 'N': 
													if (SerialLink_Input_match == 1) 	
														SerialLink_Input_match++;
															else 				
																SerialLink_Input_match = 0;
													break;
								case 'D': 
													if (SerialLink_Input_match == 2) 	
														SerialLink_Input_match++;
														else 
															SerialLink_Input_match = 0;
													break;
								case '_': 									
													if (SerialLink_Input_match == 3) 
														{
															SerialLink_Input_Mode = SERIALLINK_INPUT_MODE_CMD;
															SerialLink_Input_match = 0;
														}
														else 					
															SerialLink_Input_match = 0;
													break;
								case '#': 
													if (SerialLink_Input_match == 3) 
														{
															SerialLink_Input_Mode = SERIALLINK_INPUT_MODE_Parameter;
															SerialLink_Input_match = 0;
														}
														else 					
															SerialLink_Input_match = 0;
													break;
								default:
													SerialLink_Input_match = 0;
													break;
												
							}
							break;
		case SERIALLINK_INPUT_MODE_CMD:
							SerialLink_Input_CMD 	= (SERIALLINK_INPUT_CMD) INPUT;
							USART_ITConfig(USART_3.USART, USART_IT_RXNE, DISABLE);
							SerialLink_Input_Mode = SERIALLINK_INPUT_MODE_None;
							USART_ITConfig(USART_3.USART, USART_IT_RXNE, ENABLE);
							break;
		case SERIALLINK_INPUT_MODE_Parameter:
							SerialLink_Input[SerialLink_Input_cnt++] = INPUT;
							if (SerialLink_Input_cnt >= PARA_MODE_4-1)
								{		
									SerialLink_Input_cnt = 0;
									PARAM_Read(SerialLink_Input);	
									SerialLink_Input_Mode = SERIALLINK_INPUT_MODE_None;
									RGB_Flash(RGB_GREEN);
								}
							break;			
	}
}
///////////////////////////////////////////////////////////////////////////	
////SerialLink_Console	
//	
static uint8_t  SerialLink_Console_StackSize = 0; 
static uint8_t  SerialLink_Console_StackIndex = 0; 
static uint8_t	SerialLink_Console_printfcnt=0;
static uint8_t  SerialLink_Console_BUFF[SERIALINK_CONSOLE_STACKSIZE][SERIALINK_CONSOLE_BUFFSIZE];
void SerialLink_Console(SERIALLINK_LEVEL type, const char *fmt, ...) 
{
		va_list args;		
		//protocol
		SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =  (uint8_t) 'V';
		SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =  (uint8_t) 'N';
		SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =  (uint8_t) 'D';
		SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =  (uint8_t) '*';					
		
		switch (type)
		{
			case Event:
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	'\n';
								break;
			case EventInfo: 	
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	'-';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								break;
			case EventInfoDetail: 	
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	'+';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								break;
			case DetailList: 	
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								break;
								
			case Highlevel: 	
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	'#';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								break;
			case Lowlevel: 	
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	'#';
								SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] =	' ';
								break;
			default:
							 break;
								
		}		
		//Copy Content
		va_start(args,fmt);
		vprintf(fmt,args);
		va_end(args);		
		
		//Errase empty buffer
		while (SerialLink_Console_printfcnt<SERIALINK_CONSOLE_BUFFSIZE)	
		{
			SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] = 0x00;
		}		
				
		//switch dma mode		
		if (SerialLink_Mode != SERIALLINK_MODE_Parameter)
		SerialLink_Mode =  SERIALLINK_MODE_Debug;		
		
		//increase Stack size
		SerialLink_Console_StackSize ++;
		
		SerialLink_Console_printfcnt = 0;
}

int fputc(int ch, FILE *f)
{
	SerialLink_Console_BUFF[SerialLink_Console_StackSize][SerialLink_Console_printfcnt++] = ch;
  return ch;
}
///////////////////////////////////////////////////////////////////////////	
////SerialLink_Parameter	
//	
uint8_t SerialLink_Param_BUFF[SERIALINK_PARAM_STACKSIZE][SERIALINK_PARAM_BUFFSIZE];
static uint8_t SerialLink_Param_StackIndex;
void SerialLink_SetParameter (uint16_t stack, uint8_t* addr )
{
	uint8_t cnt_serial = 0 ;
	SerialLink_Param_BUFF[stack][cnt_serial++] =   'V';
	SerialLink_Param_BUFF[stack][cnt_serial++] =   'N';
	SerialLink_Param_BUFF[stack][cnt_serial++] =   'D';
	SerialLink_Param_BUFF[stack][cnt_serial++] =   '$';		
	for  (cnt_serial = 4; cnt_serial < SERIALINK_PARAM_BUFFSIZE; cnt_serial++)		
		SerialLink_Param_BUFF[stack][cnt_serial] = *(addr++);	
}
void SerialLink_SendParameter (void)
{
//	SerialLink_Console(Event,"PARAM: Send parameter to station");
	SerialLink_Param_StackIndex = 0;
	SerialLink_Mode = SERIALLINK_MODE_Parameter;
}

///////////////////////////////////////////////////////////////////////////
////SerialLink_Stream	
//	
static uint8_t SerialLink_Stream_BUFF[SERIALINK_STREAM_BUFFSIZE];
void SerialLink_StreamProcess (void)
{
	uint16_t cnt = 0;
	//////////////////
	////Indicator	
	//	
	SerialLink_Stream_BUFF [cnt++] = 'V';
	SerialLink_Stream_BUFF [cnt++] = 'N';
	SerialLink_Stream_BUFF [cnt++] = 'D';
	SerialLink_Stream_BUFF [cnt++] = '_';
	
	//////////////
	////Euler Angle
	//			
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Eulerx & 0xFF00) >>8);//4
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Eulerx & 0x00FF); 	 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Eulery & 0xFF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Eulery & 0x00FF); 	 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Eulerz & 0xFF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Eulerz & 0x00FF); 

	//////////////
	////Quaternion
	//			
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Qua1 & 0xFFFF0000) >>16);//10
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Qua1 & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Qua1 & 0x000000FF); 	 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Qua2 & 0xFFFF0000) >>16);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Qua2 & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Qua2 & 0x000000FF); 	 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Qua3 & 0xFFFF0000) >>16);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Qua3 & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Qua3 & 0x000000FF); 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Qua4 & 0xFFFF0000) >>16);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Qua4 & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Qua4 & 0x000000FF); 

	
	//////////////
	////dcm
	//			
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm0 & 0xFFFF0000) >>16);//22
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm0 & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.dcm0 & 0x000000FF); 	 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm1 & 0xFFFF0000) >>16);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm1 & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.dcm1 & 0x000000FF); 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm2 & 0xFFFF0000) >>16);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm2 & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.dcm2 & 0x000000FF); 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm3 & 0xFFFF0000) >>16);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm3 & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.dcm3 & 0x000000FF); 	 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm4 & 0xFFFF0000) >>16);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm4 & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.dcm4 & 0x000000FF); 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm5 & 0xFFFF0000) >>16);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm5 & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.dcm5 & 0x000000FF); 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm6 & 0xFFFF0000) >>16);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm6 & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.dcm6 & 0x000000FF); 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm7 & 0xFFFF0000) >>16);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm7 & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.dcm7 & 0x000000FF); 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm8 & 0xFFFF0000) >>16);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.dcm8 & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.dcm8 & 0x000000FF); 

	//////////////////
	////DT	
	//	
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.SenseDT & 0x00FF0000) >>16);//49
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.SenseDT & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.SenseDT & 0x000000FF); 
	
	//////////////////
	////Frame Count	
	//		
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.FrameCnt & 0xFF00) >>8);//52
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.FrameCnt & 0x00FF); 
	
	//////////////////
	////Baterry	
	//	
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.SenseBat & 0xFF00) >>8);//54
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.SenseBat & 0x00FF); 	
	
	//////////////////
	////Baterry	
	//	
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.SenseTemp & 0xFF00) >>8);//56
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.SenseTemp & 0x00FF); 	

	//////////////////
	////Button	
	//	
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Button); //55
	
	//////////////////
	////Gyro	
	//		
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Gyrox & 0xFF00) >>8);//58
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Gyrox & 0x00FF); 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Gyroy & 0xFF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Gyroy & 0x00FF); 	 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Gyroz & 0xFF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Gyroz & 0x00FF); 	 
	
	//////////////////
	////Magn	
	//		
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Magnx & 0xFF00) >>8);//62
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Magnx & 0x00FF); 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Magny & 0xFF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Magny & 0x00FF); 	 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Magnz & 0xFF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Magnz & 0x00FF); 	 
	
	//////////////////
	////Accel	
	//		
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Accelx & 0xFF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Accelx & 0x00FF); 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Accely & 0xFF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Accely & 0x00FF); 	 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Accelz & 0xFF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Accelz & 0x00FF); 	 		
	
	//////////////////
	////Velocity	
	//		
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Velx & 0xFFFF0000) >>16);//22
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Velx & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Velx & 0x000000FF); 	 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Vely & 0xFFFF0000) >>16);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Vely & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Vely & 0x000000FF); 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Velz & 0xFFFF0000) >>16);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Velz & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Velz & 0x000000FF); 
	
	//////////////////
	////Position	
	//	
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Posx & 0xFFFF0000) >>16);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Posx & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Posx & 0x000000FF); 	 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Posy & 0xFFFF0000) >>16);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Posy & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Posy & 0x000000FF); 
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Posz & 0xFFFF0000) >>16);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ((DATA_OUT.Posz & 0x0000FF00) >>8);
	SerialLink_Stream_BUFF [cnt++]	= (uint8_t) ( DATA_OUT.Posz & 0x000000FF); 

}
///////////////////////////////////////////////////////////////////////////
////SerialLink DMA	
//	
void SerialLink_DMATX_Process (void)
{	
	switch (SerialLink_Mode)
	{
		case SERIALLINK_MODE_Debug:			
			if (SerialLink_Console_StackIndex >= SerialLink_Console_StackSize)//switch back to stream if no debug string need to be sent
			{
				DMA_Cmd(UART3_TX_DMA_ST, DISABLE);				
				SerialLink_Console_StackIndex = SerialLink_Console_StackSize = 0;
				USART_DMA_Set (&USART_3,USART_TX, SerialLink_Stream_BUFF, SERIALINK_STREAM_BUFFSIZE,USART_MODE_Continuos);		
				SerialLink_Mode =  SERIALLINK_MODE_Stream;	
			}	
			else
			{	
				DMA_Cmd(UART3_TX_DMA_ST, DISABLE);			
				USART_DMA_Set (&USART_3,USART_TX, SerialLink_Console_BUFF[SerialLink_Console_StackIndex++], SERIALINK_CONSOLE_BUFFSIZE, USART_MODE_Continuos);
			}
			break;
			
		case SERIALLINK_MODE_Parameter:					
			if (SerialLink_Param_StackIndex >= SERIALINK_PARAM_STACKSIZE)//switch back to debug if no string need to be sent
			{
				DMA_Cmd(UART3_TX_DMA_ST, DISABLE);				
				SerialLink_Param_StackIndex = 0;
				if(SerialLink_Console_StackIndex < SerialLink_Console_StackSize)
					{
						USART_DMA_Set (&USART_3,USART_TX, SerialLink_Console_BUFF[SerialLink_Console_StackIndex++], SERIALINK_CONSOLE_BUFFSIZE,USART_MODE_Continuos);		
						SerialLink_Mode =  SERIALLINK_MODE_Debug;	
					}
					else
						{					
							USART_DMA_Set (&USART_3,USART_TX, SerialLink_Stream_BUFF, SERIALINK_STREAM_BUFFSIZE,USART_MODE_Continuos);		
							SerialLink_Mode =  SERIALLINK_MODE_Stream;	
						}
			}	
			else
			{	
				DMA_Cmd(UART3_TX_DMA_ST, DISABLE);			
				USART_DMA_Set (&USART_3,USART_TX, SerialLink_Param_BUFF[SerialLink_Param_StackIndex++], SERIALINK_PARAM_BUFFSIZE, USART_MODE_Continuos);
			}
			break;
			
		case SERIALLINK_MODE_Stream:
				SerialLink_StreamProcess();				
				DMA_Cmd(UART3_TX_DMA_ST, DISABLE);	
				break;
	}
}
///////////////////////////////////////////////////////////////////////////
////SerialLink
//	
SERIALLINK_INPUT_CMD SerialLink_GetCMD(void)
{
	return SerialLink_Input_CMD;
}
void SerialLink_ResetCMD(void)
{
	SerialLink_Input_CMD = INPUT_CMD_None;
}
void SerialLink_StartStream(void)
{
				DMA_Cmd(UART3_TX_DMA_ST, ENABLE);	
}
void Initialize_SerialLink (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* KEY Pin */
	RCC_AHB1PeriphClockCmd(HC05_KEY_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 		= HC05_KEY_PIN;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 		= GPIO_PuPd_UP;
	GPIO_Init(HC05_KEY_PORT, &GPIO_InitStructure); 	
	
	GPIO_ResetBits(HC05_KEY_PORT, HC05_KEY_PIN);
		
	/* Uart Pin */
	RCC_AHB1PeriphClockCmd(UART3_GPIO_CLK, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = UART3_RX_PIN | UART3_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(UART3_GPIO_PORT, &GPIO_InitStructure);
 
  GPIO_PinAFConfig(UART3_GPIO_PORT, UART3_TX_SOURCE, GPIO_AF_USART3);
  GPIO_PinAFConfig(UART3_GPIO_PORT, UART3_RX_SOURCE, GPIO_AF_USART3);
	
	
	USART_3.USART 							= USART3;
	USART_3.IT_NVIC_IRQ					= USART3_IRQn;
	
	USART_3.TX.USART_DMA				= USART_DMA_Enable;
	USART_3.TX.DMA_Channel 			= UART3_TX_DMA_CH;
	USART_3.TX.DMA_Stream	 			= UART3_TX_DMA_ST;
	USART_3.TX.DMA_NVIC_IRQ 		= UART3_TX_DMA_IRQn;
	USART_3.TX.preemtionPri 		= NVIC_Prepriority_SerialLink;
	USART_3.TX.SubPri 					= NVIC_Subpriority_SerialLink;

	USART_3.RX.USART_DMA				= USART_DMA_Disable;
	USART_3.RX.DMA_Channel 			= UART3_RX_DMA_CH;
	USART_3.RX.DMA_Stream	 			= UART3_RX_DMA_ST;
	USART_3.RX.DMA_NVIC_IRQ 		= UART3_RX_DMA_IRQn;	
	USART_3.RX.preemtionPri 		= NVIC_Prepriority_SerialLink;
	USART_3.RX.SubPri 					= NVIC_Subpriority_SerialLink+1;

	USART_OpenPort(&USART_3,115200);	
	
	USART_Interrupt_Set (&USART_3,USART_RX, SerialLink_Input, SERIALINK_INPUT_BUFFSIZE,USART_MODE_Continuos);	
	
	SerialLink_DMATX_Process();
	USART_DMA_Set (&USART_3,USART_TX, SerialLink_Stream_BUFF, SERIALINK_STREAM_BUFFSIZE,USART_MODE_Continuos);
	
	
	
	SerialLink_Console(None,"\n\n");
	SerialLink_Console(None,"          ================================");
	SerialLink_Console(None,"          ||         SENSE V2.0         ||");
	SerialLink_Console(None,"          ================================");
	SerialLink_Console(None,"                     vnd231092@gmail.com");	
}

