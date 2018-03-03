#include "BoardConfig.h"



#define SERIAL_TX_BUFSIZE		5
#define SERIAL_RX_BUFSIZE	  5


typedef enum
{
    USART_DMA_Enable 		=1 ,				
    USART_DMA_Disable		=0
}USART_DMA;

typedef enum
{
    USART_TX ,				//Write data to periph
		USART_RX,					//recieve from periph
}USART_DIR;

typedef enum
{
    USART_MODE_Continuos,		
    USART_MODE_Single,				
}USART_MODE;

typedef enum
{
    USART_LineState_Free,		 				
    USART_LineState_Busy	
}USART_LineState;


typedef struct 
							{
									USART_DMA							USART_DMA;								
									uint8_t	  						*Buffer;								
									uint32_t							BuffPos;					
									uint32_t							BuffSize;
									USART_MODE						Mode;
									
									uint32_t 							DMA_Channel;
									DMA_Stream_TypeDef   *DMA_Stream;
									uint8_t 							DMA_NVIC_IRQ;
									uint8_t 							preemtionPri;
									uint8_t 							SubPri;				
							} USART_Config;

typedef struct 
							{
									USART_Config			RX;
									USART_Config			TX;
									USART_TypeDef* 		USART;								
									uint8_t 					IT_NVIC_IRQ;
							} USART;



extern	USART USART_1, USART_3;

uint8_t USART_BUSY_Check (USART_Config* Line);
void Initialize_USART_2 (void);
void USART_OpenPort (USART * serial, uint32_t BaudRate);

void  USART_DMA_Set (USART * serial,USART_DIR DIR, uint8_t *Buffer, uint32_t BufferSize, USART_MODE MODE);
void  USART_Interrupt_Set (USART * serial,USART_DIR DIR, uint8_t *Buffer, uint32_t BufferSize, USART_MODE MODE);
