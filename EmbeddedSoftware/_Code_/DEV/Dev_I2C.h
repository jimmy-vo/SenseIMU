#include "BoardConfig.h"



typedef enum
{
    I2C_Phase_TX ,				//Write data to periph
		I2C_Phase_COMMAND	, 	//Command
    I2C_Phase_TXREG	,			//Write to request from periph
		I2C_Phase_RX,					//recieve from periph
		I2C_Phase_DMA					//DMA
}I2C_Phase;


typedef enum
{
    I2C_DMA_Enable 		=1 ,				
    I2C_DMA_Disable		=0
}I2C_DMA;

typedef enum 
{	
		I2C_STATUS_OK									=	0,
		I2C_STATUS_BUSY								=	1,	
		I2C_STATUS_TIMEOUT						=	2	
}I2C_Status;

typedef struct 
{
		uint8_t 							*Address;
		uint8_t								RegSize;
} Register;

typedef struct 
	{
		uint8_t 							SlaveAddr; 

		uint8_t 							*Buffer;
		uint32_t							BufPos;
		uint32_t 							BufSize;	
		I2C_DMA								DMA;

		uint8_t 							DMA_NVIC_IRQ;		
		uint8_t 							preemtionPri;
		uint8_t 							SubPri;				
		uint32_t 							DMA_Channel;
		DMA_Stream_TypeDef   *DMA_Stream;
		uint32_t 							DMA_Flag;
	} I2C_Config;

	
	
	
	
#define DEBUGi2c			80
	
#ifdef DEBUGi2c		
	typedef enum
{
	null = 0,
	TX_DMA_selected,
	TX_IT_selected,
	COMMAND_selected,
	RX_DMA_selected,
	RX_IT_selected,	
	Request_done,	
	START_bit_Sent,
	
	TX_Address_sent,
	RX_Address_sent1,
	RX_Address_sent2,
	First_REG_sent,
	Next_REG_sent,
	TX_enable_DMA,
	TX_Sending_DATA,
	TX_STOP_bit_Sent,
	COMMAND_STOP_bit_Sent,
	
	START_bit_Sent2,
	RX_enable_DMA,
	RX_Recieving_DATA,
	RX_STOP_bit_Sent,
	
	RX_DMA_TC,
	TX_DMA_TC,	
	
	ER,
	ER_ACK,
}I2C_State;
#endif	
	
typedef struct 
{			
			I2C_Config										RX;
			I2C_Config										TX;
			I2C_TypeDef* 									I2C;
			Register 											Register; 
			I2C_Phase											Phase;
			uint16_t 											TimeOut;
	
#ifdef DEBUGi2c			
			I2C_State											I2C_State[DEBUGi2c];	
#endif
} I2C;




extern I2C IMU, EEPROM;

void Initialize_I2C(I2C* network, uint32_t BaudRate);
I2C_Status I2C_Request(I2C* network,I2C_Phase Phase, uint8_t TXAddr, uint8_t RXAddr,  uint8_t *Register, uint16_t RegisterSize, uint8_t *Buffer, uint32_t BufferSize);

I2C_Status I2C_BUSY_Check (I2C* network);
I2C_Status I2C_LAST_TRANSMIT_Check (I2C* network);
I2C_Status I2C_EVENT_Check (I2C* network, uint32_t I2C_EVENT);
