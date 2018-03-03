#include "EEPROM.h"

#include "Timing_Manage.h"
#include "Dev_I2C.h"
#include "Parameter.h"

#include "Serial_Link.h"

uint16_t EEPROM_Register;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////I2C	Driver
//
static uint64_t EEPROM_zero;
static EEPROM_Status EEPROM_Write (uint16_t Register, uint8_t *buffer, uint8_t length)
	{	
		I2C_Status I2C = I2C_STATUS_OK;
		EEPROM_Status State = EEPROM_Status_OK;
		uint8_t reg[2];		
		EEPROM_Register = Register;
		
		reg[0] 		= (uint8_t) ((Register&0x7F00)>>8);
		reg[1] 		= (uint8_t)	(Register&0xFF);		
			
		//Make sure the last transfer was timeout ago;
		while(!Check_msTick(EEPROM_zero, EEPROM.TimeOut));		
		
		if ((I2C = I2C_Request(&EEPROM, I2C_Phase_TX, LC24256_WRITE_ADDRESS, LC24256_WRITE_ADDRESS, reg, 2, buffer, length))!=I2C_STATUS_OK) 
		{
			State = EEPROM_STATUS_REQ_FAIL;
			SerialLink_Console(Lowlevel,"%s -> %s", String_EPPROM_Status[State], String_I2C_Status[I2C]);
			return State;
		}
		
		if ((I2C = I2C_BUSY_Check (&EEPROM)) != I2C_STATUS_OK)	
		{
			State = EEPROM_Status_BUSY;
			SerialLink_Console(Lowlevel,"%s -> %s", String_EPPROM_Status[State], String_I2C_Status[I2C]);
			return State;
		}
		
		EEPROM_zero = Get_msTick();
		
		return EEPROM_Status_OK;
	}

static EEPROM_Status EEPROM_Read (uint16_t Register, uint8_t *buffer, uint32_t length)
	{	
		I2C_Status I2C = I2C_STATUS_OK;
		EEPROM_Status State = EEPROM_Status_OK;
		uint8_t reg[2];		
		EEPROM_Register = Register;		
		
		reg[0] 		= (uint8_t) ((Register&0x7F00)>>8);
		reg[1] 		= (uint8_t)	(Register&0xFF);	
		
		//Make sure the last transfer was timeout ago;
		while(!Check_msTick(EEPROM_zero, EEPROM.TimeOut));				
			
		if ((I2C = I2C_Request(&EEPROM, I2C_Phase_RX, LC24256_WRITE_ADDRESS, LC24256_READ_ADDRESS, reg, 2, buffer, length))!=I2C_STATUS_OK) 
		{
			State = EEPROM_STATUS_REQ_FAIL;
			SerialLink_Console(Lowlevel,"%s -> %s", String_EPPROM_Status[State], String_I2C_Status[I2C]);
			return State;
		}
			
		if ((I2C = I2C_BUSY_Check (&EEPROM))!=I2C_STATUS_OK) 
		{
			State = EEPROM_Status_BUSY;
			SerialLink_Console(Lowlevel,"%s -> %s", String_EPPROM_Status[State], String_I2C_Status[I2C]);
			return State;
		}
		
		EEPROM_zero = Get_msTick();
		
		return EEPROM_Status_OK;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////USERCODE
//
// 24lC256 has 32000 x byte =32kbit * 8 = 256 kbit
// 64 byte/page
// ==>500 pages
EEPROM_Status Initialize_EEPROM (void)
{		
	  GPIO_InitTypeDef  GPIO_InitStructure;
	
    /* GPIO Periph clock enable */
    RCC_AHB1PeriphClockCmd(I2C2_GPIO_CLK, ENABLE);
	
		SerialLink_Console(EventInfo, "Initialize EEPROM");
    /* Configure I2C2 pins: SCL and SDA */
    GPIO_InitStructure.GPIO_Pin 		=  	I2C2_SCL_PIN | I2C2_SDA_PIN;
		GPIO_InitStructure.GPIO_Mode 		= 	GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd  	= 	GPIO_PuPd_NOPULL;
	
    GPIO_Init(I2C2_GPIO_PORT, &GPIO_InitStructure);
	
	  /* Connect GPIO pins to peripheral */
		GPIO_PinAFConfig(I2C2_GPIO_PORT, I2C2_SCL_SOURCE, I2C2_AF); 
		GPIO_PinAFConfig(I2C2_GPIO_PORT, I2C2_SDA_SOURCE, I2C2_AF);
	
		EEPROM.TimeOut 									= 10;		//ms
		EEPROM.I2C											= I2C2;		
	
 		EEPROM.TX.DMA										= I2C_DMA_Enable;
		EEPROM.TX.DMA_Channel						=	I2C2_TX_DMA_CH;
		EEPROM.TX.DMA_Stream						=	I2C2_TX_DMA_ST;
		EEPROM.TX.DMA_Flag							=	I2C2_TX_DMA_FLAG;
		EEPROM.TX.DMA_NVIC_IRQ					=	I2C2_TX_DMA_IRQn;
		EEPROM.RX.preemtionPri					=	NVIC_Prepriority_EPPROM;
		EEPROM.RX.SubPri								=	NVIC_Subpriority_EPPROM;
			
 		EEPROM.RX.DMA										= I2C_DMA_Enable;
		EEPROM.RX.DMA_Channel						=	I2C2_RX_DMA_CH;
		EEPROM.RX.DMA_Stream						=	I2C2_RX_DMA_ST;
		EEPROM.RX.DMA_Flag							=	I2C2_RX_DMA_FLAG;
		EEPROM.RX.DMA_NVIC_IRQ					=	I2C2_RX_DMA_IRQn;
		EEPROM.RX.preemtionPri					=	NVIC_Prepriority_EPPROM;
		EEPROM.RX.SubPri								=	NVIC_Subpriority_EPPROM+1;
		
		Initialize_I2C(&EEPROM,400000);			//300kHz		
		
		return EEPROM_Status_OK;
}


EEPROM_Status EEPROM_ReadPage (uint8_t *buffer, uint16_t page)
{	
	return EEPROM_Read (page * Page_Size, buffer, Page_Size);
}

EEPROM_Status EEPROM_WritePage (uint8_t *buffer, uint16_t page)
{			
	return EEPROM_Write (page * Page_Size, buffer, Page_Size);	
}

