#include "Dev_SDIO.h"
#include "stm32f4xx_sdio.h"
#include "Serial_Link.h"


static uint32_t 		CardType =  SDIO_STD_CAPACITY_SDIO_CARD_V1_1;
static uint32_t			CSDIO_Tab[4], CID_Tab[4], RCA = 0;
static uint8_t 			SDSTATUS_Tab[16];
__IO uint32_t 			StopCondition = 0;
__IO SDIO_STATE 		TransferError = SDIO_OK;
__IO uint32_t 			TransferEnd = 0, DMAEndOfTransfer = 0;
SDIO_CardInfo 		SDCardInfo;
SDIO_CardStatus		SDCardStatus;

SDIO_InitTypeDef 			SDIO_InitStructure;
SDIO_CmdInitTypeDef 	SDIO_CmdInitStructure;
SDIO_DataInitTypeDef 	SDIO_DataInitStructure;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Low level Layer
//
void SDIO_DeinitHardware(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  SDIO_ClockCmd(DISABLE);

  //Set Power State to OFF
  SDIO_SetPowerState(SDIO_PowerState_OFF);

  SDIO_DeInit();

  //Disable the Clock 
  RCC_APB2PeriphClockCmd(SDIO_CLK, DISABLE);

  GPIO_PinAFConfig(SDIO_GPIO_D0_PORT, 	SDIO_GPIO_D0_SOURCE, 	GPIO_AF_MCO);
  GPIO_PinAFConfig(SDIO_GPIO_D1_PORT, 	SDIO_GPIO_D1_SOURCE,	GPIO_AF_MCO);
  GPIO_PinAFConfig(SDIO_GPIO_D2_PORT,	 	SDIO_GPIO_D2_SOURCE, 	GPIO_AF_MCO);
  GPIO_PinAFConfig(SDIO_GPIO_D3_PORT, 	SDIO_GPIO_D3_SOURCE, 	GPIO_AF_MCO);
  GPIO_PinAFConfig(SDIO_GPIO_CLK_PORT, 	SDIO_GPIO_CLK_SOURCE, GPIO_AF_MCO);
  GPIO_PinAFConfig(SDIO_GPIO_CMD_PORT, 	SDIO_GPIO_CMD_SOURCE, GPIO_AF_MCO);

  //Configure Data pins
  GPIO_InitStructure.GPIO_Pin = SDIO_GPIO_DATA_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(SDIO_GPIO_DATA_PORT, &GPIO_InitStructure);

  //Configure CMD line
  GPIO_InitStructure.GPIO_Pin = SDIO_GPIO_CMD_PIN;
  GPIO_Init(SDIO_GPIO_CMD_PORT, &GPIO_InitStructure);

  //Configure CLK pin
  GPIO_InitStructure.GPIO_Pin = SDIO_GPIO_CLK_PIN;
  GPIO_Init(SDIO_GPIO_CLK_PORT, &GPIO_InitStructure);
}

void SDIO_InitHardware(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

#ifdef SDIO_DMA
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */
	NVIC_PriorityGroupConfig(NVIC_GROUP_DEFAULT);

	NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_Prepriority_SDIO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_Subpriority_SDIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);		
	
	/* Enable the DMA2 Clock */
	RCC_AHB1PeriphClockCmd(SDIO_DMA_CLK, ENABLE);
#endif

  //Enable GPIO Periph clock 
  RCC_AHB1PeriphClockCmd(SDIO_GPIO_DATA_CLK | SDIO_GPIO_CLK_CLK | SDIO_GPIO_CMD_CLK , ENABLE);

  GPIO_PinAFConfig(SDIO_GPIO_D0_PORT, 	SDIO_GPIO_D0_SOURCE, 	GPIO_AF_SDIO);
  GPIO_PinAFConfig(SDIO_GPIO_D1_PORT, 	SDIO_GPIO_D1_SOURCE,	GPIO_AF_SDIO);
  GPIO_PinAFConfig(SDIO_GPIO_D2_PORT,	 	SDIO_GPIO_D2_SOURCE, 	GPIO_AF_SDIO);
  GPIO_PinAFConfig(SDIO_GPIO_D3_PORT, 	SDIO_GPIO_D3_SOURCE, 	GPIO_AF_SDIO);
  GPIO_PinAFConfig(SDIO_GPIO_CLK_PORT, 	SDIO_GPIO_CLK_SOURCE, GPIO_AF_SDIO);
  GPIO_PinAFConfig(SDIO_GPIO_CMD_PORT, 	SDIO_GPIO_CMD_SOURCE, GPIO_AF_SDIO);

  //Configure DATA pins 
  GPIO_InitStructure.GPIO_Pin 		= SDIO_GPIO_DATA_PIN;
  GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd 		= GPIO_PuPd_UP;
  GPIO_Init(SDIO_GPIO_DATA_PORT, &GPIO_InitStructure);

  //Configure CMD line 
  GPIO_InitStructure.GPIO_Pin = SDIO_GPIO_CMD_PIN;
  GPIO_Init(SDIO_GPIO_CMD_PORT, &GPIO_InitStructure);

  //Configure CLK pin 
  GPIO_InitStructure.GPIO_Pin = SDIO_GPIO_CLK_PIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(SDIO_GPIO_CLK_PORT, &GPIO_InitStructure);


	#ifdef SDIO_GPIO_DETECT_PIN
		RCC_AHB1PeriphClockCmd(SDIO_GPIO_DETECT_CLK , ENABLE);
		
		GPIO_InitStructure.GPIO_Pin = SDIO_GPIO_DETECT_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(SDIO_GPIO_DETECT_PORT, &GPIO_InitStructure);
	#endif
	
  /* Enable the SDIO APB2 Clock */
  RCC_APB2PeriphClockCmd(SDIO_CLK, ENABLE);

}
#ifdef SDIO_DMA
void SDIO_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize)
{
  DMA_InitTypeDef SDDMA_InitStructure;

  DMA_ClearFlag(SDIO_DMA_STREAM, SDIO_DMA_FLAG_FEIF | SDIO_DMA_FLAG_DMEIF | SDIO_DMA_FLAG_TEIF | SDIO_DMA_FLAG_HTIF | SDIO_DMA_FLAG_TCIF);

  DMA_Cmd(SDIO_DMA_STREAM, DISABLE);

  DMA_DeInit(SDIO_DMA_STREAM);

  SDDMA_InitStructure.DMA_Channel										= SDIO_DMA_CHANNEL;
  SDDMA_InitStructure.DMA_PeripheralBaseAddr 				= (uint32_t)SDIO_FIFO_ADDRESS;
  SDDMA_InitStructure.DMA_Memory0BaseAddr 					= (uint32_t)BufferSRC;
  SDDMA_InitStructure.DMA_DIR 											= DMA_DIR_MemoryToPeripheral;
  SDDMA_InitStructure.DMA_BufferSize 								= 0;
  SDDMA_InitStructure.DMA_PeripheralInc 						= DMA_PeripheralInc_Disable;
  SDDMA_InitStructure.DMA_MemoryInc 								= DMA_MemoryInc_Enable;
  SDDMA_InitStructure.DMA_PeripheralDataSize 				= DMA_PeripheralDataSize_Word;
  SDDMA_InitStructure.DMA_MemoryDataSize 						= DMA_MemoryDataSize_Word;
  SDDMA_InitStructure.DMA_Mode 											= DMA_Mode_Normal;
  SDDMA_InitStructure.DMA_Priority 									= DMA_Priority_VeryHigh;
  SDDMA_InitStructure.DMA_FIFOMode 									= DMA_FIFOMode_Enable;
  SDDMA_InitStructure.DMA_FIFOThreshold 						= DMA_FIFOThreshold_Full;
  SDDMA_InitStructure.DMA_MemoryBurst 							= DMA_MemoryBurst_INC4;
  SDDMA_InitStructure.DMA_PeripheralBurst 					= DMA_PeripheralBurst_INC4;
  DMA_Init(SDIO_DMA_STREAM, &SDDMA_InitStructure);
  DMA_ITConfig(SDIO_DMA_STREAM, DMA_IT_TC, ENABLE);
  DMA_FlowControllerConfig(SDIO_DMA_STREAM, DMA_FlowCtrl_Peripheral);

  DMA_Cmd(SDIO_DMA_STREAM, ENABLE);
}

void SDIO_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize)
{
  DMA_InitTypeDef SDDMA_InitStructure;

  DMA_ClearFlag(SDIO_DMA_STREAM, SDIO_DMA_FLAG_FEIF | SDIO_DMA_FLAG_DMEIF | SDIO_DMA_FLAG_TEIF | SDIO_DMA_FLAG_HTIF | SDIO_DMA_FLAG_TCIF);

  DMA_Cmd(SDIO_DMA_STREAM, DISABLE);
	
  DMA_DeInit(SDIO_DMA_STREAM);

  SDDMA_InitStructure.DMA_Channel = SDIO_DMA_CHANNEL;
  SDDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;
  SDDMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)BufferDST;
  SDDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  SDDMA_InitStructure.DMA_BufferSize = 0;
  SDDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  SDDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  SDDMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  SDDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  SDDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  SDDMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  SDDMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  SDDMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  SDDMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_INC4;
  SDDMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_INC4;
  DMA_Init(SDIO_DMA_STREAM, &SDDMA_InitStructure);
  DMA_ITConfig(SDIO_DMA_STREAM, DMA_IT_TC, ENABLE);
  DMA_FlowControllerConfig(SDIO_DMA_STREAM, DMA_FlowCtrl_Peripheral);

  /* DMA2 Stream3 or Stream6 enable */
  DMA_Cmd(SDIO_DMA_STREAM, ENABLE);
}

void SDIO_DMA_IRQHANDLER(void)
{
	  if(SDIO_DMA->LISR & SDIO_DMA_FLAG_TCIF)
  {
    DMAEndOfTransfer = 0x01;
    DMA_ClearFlag(SDIO_DMA_STREAM, SDIO_DMA_FLAG_TCIF|SDIO_DMA_FLAG_FEIF);
  }
}
#endif


void SDIO_IRQHandler(void)
{
  if (SDIO_GetITStatus(SDIO_IT_DATAEND) != RESET)
  {
    TransferError = SDIO_OK;
    SDIO_ClearITPendingBit(SDIO_IT_DATAEND);
    TransferEnd = 1;
  }
  else if (SDIO_GetITStatus(SDIO_IT_DCRCFAIL) != RESET)
  {
    SDIO_ClearITPendingBit(SDIO_IT_DCRCFAIL);
    TransferError = SDIO_DATA_CRC_FAIL;
  }
  else if (SDIO_GetITStatus(SDIO_IT_DTIMEOUT) != RESET)
  {
    SDIO_ClearITPendingBit(SDIO_IT_DTIMEOUT);
    TransferError = SDIO_DATA_TIMEOUT;
  }
  else if (SDIO_GetITStatus(SDIO_IT_RXOVERR) != RESET)
  {
    SDIO_ClearITPendingBit(SDIO_IT_RXOVERR);
    TransferError = SDIO_RX_OVERRUN;
  }
  else if (SDIO_GetITStatus(SDIO_IT_TXUNDERR) != RESET)
  {
    SDIO_ClearITPendingBit(SDIO_IT_TXUNDERR);
    TransferError = SDIO_TX_UNDERRUN;
  }
  else if (SDIO_GetITStatus(SDIO_IT_STBITERR) != RESET)
  {
    SDIO_ClearITPendingBit(SDIO_IT_STBITERR);
    TransferError = SDIO_START_BIT_ERR;
  }

  SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND |
                SDIO_IT_TXFIFOHE | SDIO_IT_RXFIFOHF | SDIO_IT_TXUNDERR |
                SDIO_IT_RXOVERR | SDIO_IT_STBITERR, DISABLE);
}

void SDIO_SendCMD(SDIO_CMD SDIO_CmdIndex, uint32_t SDIO_Argument, uint32_t SDIO_Response, uint32_t SDIO_Wait, uint32_t SDIO_CPSM)
{	
  SDIO_CmdInitStructure.SDIO_Argument = SDIO_Argument;
  SDIO_CmdInitStructure.SDIO_CmdIndex = SDIO_CmdIndex;
  SDIO_CmdInitStructure.SDIO_Response = SDIO_Response;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM;
  SDIO_SendCommand(&SDIO_CmdInitStructure);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// SDCard Comunicate
//
SDIO_STATE SDIO_Setup(void)
{
  __IO SDIO_STATE errorstatus = SDIO_OK;

	
  SDIO_DeinitHardware();
  SDIO_InitHardware();

	//Power ON
  if ((errorstatus = SDIO_PowerON()) != SDIO_OK)
		return(errorstatus);

	//SDIO_Initialize
  if ((errorstatus = SDIO_Initialize()) != SDIO_OK)
		return(errorstatus);
		
  //Configure the SDIO peripheral
  SDIO_InitStructure.SDIO_ClockDiv 						= SDIO_TRANSFER_CLK_DIV;
  SDIO_InitStructure.SDIO_ClockEdge 					= SDIO_ClockEdge_Rising;
  SDIO_InitStructure.SDIO_ClockBypass 				= SDIO_ClockBypass_Disable;
  SDIO_InitStructure.SDIO_ClockPowerSave 			= SDIO_ClockPowerSave_Disable;
  SDIO_InitStructure.SDIO_BusWide 						= SDIO_BusWide_1b;
  SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
  SDIO_Init(&SDIO_InitStructure);
  
	//SDIO_GetCardInfo
  if ((errorstatus = SDIO_GetCardInfo(&SDCardInfo)) != SDIO_OK)	
		return(errorstatus);
	
	//Select/Deselect
  if ((errorstatus = SDIO_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16))) != SDIO_OK)	
		return(errorstatus);

	//Switch to 4bit mode
  if ((errorstatus = SDIO_EnableWideBusOperation(SDIO_BusWide_4b)) != SDIO_OK)	
		return(errorstatus);
		
	//Switch to 4bit mode
  if ((errorstatus = SDIO_GetCardStatus(&SDCardStatus)) != SDIO_OK)
		return(errorstatus);
		
  return(errorstatus);
}

SDTransferState SDIO_GetStatus(void)
{
  SDCardState cardstate =  SDIO_CARD_TRANSFER;

  cardstate = SDIO_GetState();

  if (cardstate == SDIO_CARD_TRANSFER)	return(SDIO_TRANSFER_OK);
  else if(cardstate == SDIO_CARD_ERROR)	return (SDIO_TRANSFER_ERROR);  
  else																		return(SDIO_TRANSFER_BUSY);
}


SDCardState SDIO_GetState(void)
{
  uint32_t resp1 = 0;

  if(SDIO_Detect()== SDIO_PRESENT)
  {
    if (SDIO_SendStatus(&resp1) != SDIO_OK) 	return SDIO_CARD_ERROR;
    else																					return (SDCardState)((resp1 >> 9) & 0x0F);
  }
  else
  {
    return SDIO_CARD_ERROR;
  }
}

uint8_t SDIO_Detect(void)
{
  __IO uint8_t status = SDIO_PRESENT;
	
	#ifdef SDIO_GPIO_DETECT_PIN
		if (GPIO_ReadInputDataBit(SDIO_GPIO_DETECT_PORT, SDIO_GPIO_DETECT_PIN) == Bit_RESET)
		{
			status = SDIO_NOT_PRESENT;
		}
	#endif
  return status;
}


SDIO_STATE SDIO_PowerON(void)
{
  __IO SDIO_STATE errorstatus = SDIO_OK;
  uint32_t response = 0, count = 0, validvoltage = 0;
  uint32_t SDType = SDIO_STD_CAPACITY;

  //SDIO Config
  SDIO_InitStructure.SDIO_ClockDiv = SDIO_INIT_CLK_DIV;
  SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
  SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
  SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
  SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;
  SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
  SDIO_Init(&SDIO_InitStructure);

  //Set Power State to ON
  SDIO_SetPowerState(SDIO_PowerState_ON);

  //Enable SDIO Clock
  SDIO_ClockCmd(ENABLE);

  //GO_IDLE_STATE
	SDIO_SendCMD(SDIO_CMD_GO_IDLE_STATE, 0x00, SDIO_Response_No, SDIO_Wait_No,SDIO_CPSM_Enable );
  errorstatus = CmdError();
  if (errorstatus != SDIO_OK)    return(errorstatus);


  //SEND_IF_COND check R7
	SDIO_SendCMD(SDIO_SEND_IF_COND, SDIO_CHECK_PATTERN, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp7Error();

  //Define Card Type
  if (errorstatus == SDIO_OK)
  {
    CardType = SDIO_STD_CAPACITY_SDIO_CARD_V2_0; /*!< SD Card 2.0 */
    SDType = SDIO_HIGH_CAPACITY;
  }
  else
  {		
		SDIO_SendCMD(SDIO_CMD_APP_CMD, 0x00, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
    errorstatus = CmdResp1Error(SDIO_CMD_APP_CMD);
  }
	SDIO_SendCMD(SDIO_CMD_APP_CMD, 0x00, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_APP_CMD);

     
  if (errorstatus == SDIO_OK)
  {// If errorstatus is SDIO_OK it is a SD card: SD card 2.0 (voltage range mismatch) or SD card 1.x
    //Send ACMD41 SDIO_APP_OP_COND with Argument 0x80100000
    while ((!validvoltage) && (count < SDIO_MAX_VOLT_TRIAL))
    {
      /*!< SEND CMD55 APP_CMD with RCA as 0 */
			
			SDIO_SendCMD(SDIO_CMD_APP_CMD, 0x00, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
      errorstatus = CmdResp1Error(SDIO_CMD_APP_CMD);
      if (errorstatus != SDIO_OK)	return(errorstatus);
			
			
			SDIO_SendCMD(SDIO_CMD_SDIO_APP_OP_COND, SDIO_VOLTAGE_WINDOW_SD | SDType, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
      errorstatus = CmdResp3Error();
      if (errorstatus != SDIO_OK)	return(errorstatus);
			

      response = SDIO_GetResponse(SDIO_RESP1);
      validvoltage = (((response >> 31) == 1) ? 1 : 0);
      count++;
    }
    if (count >= SDIO_MAX_VOLT_TRIAL)
    {
      errorstatus = SDIO_INVALID_VOLTRANGE;
      return(errorstatus);
    }

    if (response &= SDIO_HIGH_CAPACITY)
    {
      CardType = SDIO_HIGH_CAPACITY_SDIO_CARD;
    }

  }/*!< else MMC Card */

  return(errorstatus);
}


SDIO_STATE SDIO_PowerOFF(void)
{
  SDIO_STATE errorstatus = SDIO_OK;
  SDIO_SetPowerState(SDIO_PowerState_OFF);
  return(errorstatus);
}


SDIO_STATE SDIO_Initialize(void)
{
  SDIO_STATE errorstatus = SDIO_OK;
  uint16_t rca = 0x01;

  if (SDIO_GetPowerState() == SDIO_PowerState_OFF)
  {
    errorstatus = SDIO_REQUEST_NOT_APPLICABLE;
    return(errorstatus);
  }

  if (SDIO_SECURE_DIGITAL_IO_CARD != CardType)
  {
    //Send CMD2 ALL_SEND_CID
		SDIO_SendCMD(SDIO_CMD_ALL_SEND_CID, 0x00, SDIO_Response_Long, SDIO_Wait_No, SDIO_CPSM_Enable );
    errorstatus = CmdResp2Error();
    if (SDIO_OK != errorstatus)	return(errorstatus);

    CID_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
    CID_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
    CID_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
    CID_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
  }
  if ((SDIO_STD_CAPACITY_SDIO_CARD_V1_1 == CardType) 			|| 
			(SDIO_STD_CAPACITY_SDIO_CARD_V2_0 == CardType) 			|| 
			(SDIO_SECURE_DIGITAL_IO_COMBO_CARD == CardType)  	|| 
			(SDIO_HIGH_CAPACITY_SDIO_CARD == CardType))
  {
    //Send CMD3 SET_REL_ADDR with argument 0, SD Card publishes its RCA.		
		SDIO_SendCMD(SDIO_CMD_SET_REL_ADDR, 0x00, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
    errorstatus = CmdResp6Error(SDIO_CMD_SET_REL_ADDR, &rca);
    if (SDIO_OK != errorstatus)	return(errorstatus);
  }

  if (SDIO_SECURE_DIGITAL_IO_CARD != CardType)
  {
    RCA = rca;

    //Send CMD9 SEND_CSD with argument as card's RCA 
		SDIO_SendCMD(SDIO_CMD_SEND_CSD, (uint32_t)(rca << 16), SDIO_Response_Long, SDIO_Wait_No, SDIO_CPSM_Enable );
    errorstatus = CmdResp2Error();
    if (SDIO_OK != errorstatus)	return(errorstatus);

    CSDIO_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
    CSDIO_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
    CSDIO_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
    CSDIO_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
  }

  errorstatus = SDIO_OK; /*!< All cards get intialized */

  return(errorstatus);
}


SDIO_STATE SDIO_GetCardInfo(SDIO_CardInfo *cardinfo)
{
  SDIO_STATE errorstatus = SDIO_OK;
  uint8_t tmp = 0;

  cardinfo->CardType = (uint8_t)CardType;
  cardinfo->RCA = (uint16_t)RCA;

  /*!< Byte 0 */
  tmp = (uint8_t)((CSDIO_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SDIO_csd.CSDStruct = (tmp & 0xC0) >> 6;
  cardinfo->SDIO_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
  cardinfo->SDIO_csd.Reserved1 = tmp & 0x03;

  /*!< Byte 1 */
  tmp = (uint8_t)((CSDIO_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SDIO_csd.TAAC = tmp;

  /*!< Byte 2 */
  tmp = (uint8_t)((CSDIO_Tab[0] & 0x0000FF00) >> 8);
  cardinfo->SDIO_csd.NSAC = tmp;

  /*!< Byte 3 */
  tmp = (uint8_t)(CSDIO_Tab[0] & 0x000000FF);
  cardinfo->SDIO_csd.MaxBusClkFrec = tmp;

  /*!< Byte 4 */
  tmp = (uint8_t)((CSDIO_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SDIO_csd.CardComdClasses = tmp << 4;

  /*!< Byte 5 */
  tmp = (uint8_t)((CSDIO_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SDIO_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
  cardinfo->SDIO_csd.RdBlockLen = tmp & 0x0F;

  /*!< Byte 6 */
  tmp = (uint8_t)((CSDIO_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SDIO_csd.PartBlockRead = (tmp & 0x80) >> 7;
  cardinfo->SDIO_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
  cardinfo->SDIO_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
  cardinfo->SDIO_csd.DSRImpl = (tmp & 0x10) >> 4;
  cardinfo->SDIO_csd.Reserved2 = 0; /*!< Reserved */

  if ((CardType == SDIO_STD_CAPACITY_SDIO_CARD_V1_1) || (CardType == SDIO_STD_CAPACITY_SDIO_CARD_V2_0))
  {
    cardinfo->SDIO_csd.DeviceSize = (tmp & 0x03) << 10;

    /*!< Byte 7 */
    tmp = (uint8_t)(CSDIO_Tab[1] & 0x000000FF);
    cardinfo->SDIO_csd.DeviceSize |= (tmp) << 2;

    /*!< Byte 8 */
    tmp = (uint8_t)((CSDIO_Tab[2] & 0xFF000000) >> 24);
    cardinfo->SDIO_csd.DeviceSize |= (tmp & 0xC0) >> 6;

    cardinfo->SDIO_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
    cardinfo->SDIO_csd.MaxRdCurrentVDDMax = (tmp & 0x07);

    /*!< Byte 9 */
    tmp = (uint8_t)((CSDIO_Tab[2] & 0x00FF0000) >> 16);
    cardinfo->SDIO_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
    cardinfo->SDIO_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
    cardinfo->SDIO_csd.DeviceSizeMul = (tmp & 0x03) << 1;
    /*!< Byte 10 */
    tmp = (uint8_t)((CSDIO_Tab[2] & 0x0000FF00) >> 8);
    cardinfo->SDIO_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;

    cardinfo->CardCapacity = (cardinfo->SDIO_csd.DeviceSize + 1) ;
    cardinfo->CardCapacity *= (1 << (cardinfo->SDIO_csd.DeviceSizeMul + 2));
    cardinfo->CardBlockSize = 1 << (cardinfo->SDIO_csd.RdBlockLen);
    cardinfo->CardCapacity *= cardinfo->CardBlockSize;
  }
  else if (CardType == SDIO_HIGH_CAPACITY_SDIO_CARD)
  {
    /*!< Byte 7 */
    tmp = (uint8_t)(CSDIO_Tab[1] & 0x000000FF);
    cardinfo->SDIO_csd.DeviceSize = (tmp & 0x3F) << 16;

    /*!< Byte 8 */
    tmp = (uint8_t)((CSDIO_Tab[2] & 0xFF000000) >> 24);

    cardinfo->SDIO_csd.DeviceSize |= (tmp << 8);

    /*!< Byte 9 */
    tmp = (uint8_t)((CSDIO_Tab[2] & 0x00FF0000) >> 16);

    cardinfo->SDIO_csd.DeviceSize |= (tmp);

    /*!< Byte 10 */
    tmp = (uint8_t)((CSDIO_Tab[2] & 0x0000FF00) >> 8);

    cardinfo->CardCapacity = (uint64_t)(cardinfo->SDIO_csd.DeviceSize + 1) * (uint64_t)(512 * 1024);
    cardinfo->CardBlockSize = 512;
  }


  cardinfo->SDIO_csd.EraseGrSize = (tmp & 0x40) >> 6;
  cardinfo->SDIO_csd.EraseGrMul = (tmp & 0x3F) << 1;

  /*!< Byte 11 */
  tmp = (uint8_t)(CSDIO_Tab[2] & 0x000000FF);
  cardinfo->SDIO_csd.EraseGrMul |= (tmp & 0x80) >> 7;
  cardinfo->SDIO_csd.WrProtectGrSize = (tmp & 0x7F);

  /*!< Byte 12 */
  tmp = (uint8_t)((CSDIO_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SDIO_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
  cardinfo->SDIO_csd.ManDeflECC = (tmp & 0x60) >> 5;
  cardinfo->SDIO_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
  cardinfo->SDIO_csd.MaxWrBlockLen = (tmp & 0x03) << 2;

  /*!< Byte 13 */
  tmp = (uint8_t)((CSDIO_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SDIO_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
  cardinfo->SDIO_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
  cardinfo->SDIO_csd.Reserved3 = 0;
  cardinfo->SDIO_csd.ContentProtectAppli = (tmp & 0x01);

  /*!< Byte 14 */
  tmp = (uint8_t)((CSDIO_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SDIO_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
  cardinfo->SDIO_csd.CopyFlag = (tmp & 0x40) >> 6;
  cardinfo->SDIO_csd.PermWrProtect = (tmp & 0x20) >> 5;
  cardinfo->SDIO_csd.TempWrProtect = (tmp & 0x10) >> 4;
  cardinfo->SDIO_csd.FileFormat = (tmp & 0x0C) >> 2;
  cardinfo->SDIO_csd.ECC = (tmp & 0x03);

  /*!< Byte 15 */
  tmp = (uint8_t)(CSDIO_Tab[3] & 0x000000FF);
  cardinfo->SDIO_csd.CSDIO_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SDIO_csd.Reserved4 = 1;


  /*!< Byte 0 */
  tmp = (uint8_t)((CID_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SDIO_cid.ManufacturerID = tmp;

  /*!< Byte 1 */
  tmp = (uint8_t)((CID_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SDIO_cid.OEM_AppliID = tmp << 8;

  /*!< Byte 2 */
  tmp = (uint8_t)((CID_Tab[0] & 0x000000FF00) >> 8);
  cardinfo->SDIO_cid.OEM_AppliID |= tmp;

  /*!< Byte 3 */
  tmp = (uint8_t)(CID_Tab[0] & 0x000000FF);
  cardinfo->SDIO_cid.ProdName1 = tmp << 24;

  /*!< Byte 4 */
  tmp = (uint8_t)((CID_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SDIO_cid.ProdName1 |= tmp << 16;

  /*!< Byte 5 */
  tmp = (uint8_t)((CID_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SDIO_cid.ProdName1 |= tmp << 8;

  /*!< Byte 6 */
  tmp = (uint8_t)((CID_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SDIO_cid.ProdName1 |= tmp;

  /*!< Byte 7 */
  tmp = (uint8_t)(CID_Tab[1] & 0x000000FF);
  cardinfo->SDIO_cid.ProdName2 = tmp;

  /*!< Byte 8 */
  tmp = (uint8_t)((CID_Tab[2] & 0xFF000000) >> 24);
  cardinfo->SDIO_cid.ProdRev = tmp;

  /*!< Byte 9 */
  tmp = (uint8_t)((CID_Tab[2] & 0x00FF0000) >> 16);
  cardinfo->SDIO_cid.ProdSN = tmp << 24;

  /*!< Byte 10 */
  tmp = (uint8_t)((CID_Tab[2] & 0x0000FF00) >> 8);
  cardinfo->SDIO_cid.ProdSN |= tmp << 16;

  /*!< Byte 11 */
  tmp = (uint8_t)(CID_Tab[2] & 0x000000FF);
  cardinfo->SDIO_cid.ProdSN |= tmp << 8;

  /*!< Byte 12 */
  tmp = (uint8_t)((CID_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SDIO_cid.ProdSN |= tmp;

  /*!< Byte 13 */
  tmp = (uint8_t)((CID_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SDIO_cid.Reserved1 |= (tmp & 0xF0) >> 4;
  cardinfo->SDIO_cid.ManufactDate = (tmp & 0x0F) << 8;

  /*!< Byte 14 */
  tmp = (uint8_t)((CID_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SDIO_cid.ManufactDate |= tmp;

  /*!< Byte 15 */
  tmp = (uint8_t)(CID_Tab[3] & 0x000000FF);
  cardinfo->SDIO_cid.CID_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SDIO_cid.Reserved2 = 1;

  return(errorstatus);
}


SDIO_STATE SDIO_GetCardStatus(SDIO_CardStatus *cardstatus)
{
  SDIO_STATE errorstatus = SDIO_OK;
  uint8_t tmp = 0;

  errorstatus = SDIO_SendSDStatus((uint32_t *)SDSTATUS_Tab);
  if (errorstatus  != SDIO_OK)	return(errorstatus);

  /*!< Byte 0 */
  tmp = (uint8_t)((SDSTATUS_Tab[0] & 0xC0) >> 6);
  cardstatus->DAT_BUS_WIDTH = tmp;

  /*!< Byte 0 */
  tmp = (uint8_t)((SDSTATUS_Tab[0] & 0x20) >> 5);
  cardstatus->SECURED_MODE = tmp;

  /*!< Byte 2 */
  tmp = (uint8_t)((SDSTATUS_Tab[2] & 0xFF));
  cardstatus->CARD_TYPE = tmp << 8;

  /*!< Byte 3 */
  tmp = (uint8_t)((SDSTATUS_Tab[3] & 0xFF));
  cardstatus->CARD_TYPE |= tmp;

  /*!< Byte 4 */
  tmp = (uint8_t)(SDSTATUS_Tab[4] & 0xFF);
  cardstatus->SIZE_OF_PROTECTED_AREA = tmp << 24;

  /*!< Byte 5 */
  tmp = (uint8_t)(SDSTATUS_Tab[5] & 0xFF);
  cardstatus->SIZE_OF_PROTECTED_AREA |= tmp << 16;

  /*!< Byte 6 */
  tmp = (uint8_t)(SDSTATUS_Tab[6] & 0xFF);
  cardstatus->SIZE_OF_PROTECTED_AREA |= tmp << 8;

  /*!< Byte 7 */
  tmp = (uint8_t)(SDSTATUS_Tab[7] & 0xFF);
  cardstatus->SIZE_OF_PROTECTED_AREA |= tmp;

  /*!< Byte 8 */
  tmp = (uint8_t)((SDSTATUS_Tab[8] & 0xFF));
  cardstatus->SPEED_CLASS = tmp;

  /*!< Byte 9 */
  tmp = (uint8_t)((SDSTATUS_Tab[9] & 0xFF));
  cardstatus->PERFORMANCE_MOVE = tmp;

  /*!< Byte 10 */
  tmp = (uint8_t)((SDSTATUS_Tab[10] & 0xF0) >> 4);
  cardstatus->AU_SIZE = tmp;

  /*!< Byte 11 */
  tmp = (uint8_t)(SDSTATUS_Tab[11] & 0xFF);
  cardstatus->ERASE_SIZE = tmp << 8;

  /*!< Byte 12 */
  tmp = (uint8_t)(SDSTATUS_Tab[12] & 0xFF);
  cardstatus->ERASE_SIZE |= tmp;

  /*!< Byte 13 */
  tmp = (uint8_t)((SDSTATUS_Tab[13] & 0xFC) >> 2);
  cardstatus->ERASE_TIMEOUT = tmp;

  /*!< Byte 13 */
  tmp = (uint8_t)((SDSTATUS_Tab[13] & 0x3));
  cardstatus->ERASE_OFFSET = tmp;

  return SDIO_OK;
}


SDIO_STATE SDIO_EnableWideBusOperation(uint32_t WideMode)
{
  SDIO_STATE errorstatus = SDIO_OK;

  /*!< MMC Card doesn't support this feature */
  if (SDIO_MULTIMEDIA_CARD == CardType)
  {
    errorstatus = SDIO_UNSUPPORTED_FEATURE;
    return(errorstatus);
  }
  else if ((SDIO_STD_CAPACITY_SDIO_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SDIO_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SDIO_CARD == CardType))
  {
    if (SDIO_BusWide_8b == WideMode)
    {
      errorstatus = SDIO_UNSUPPORTED_FEATURE;
      return(errorstatus);
    }
    else if (SDIO_BusWide_4b == WideMode)
    {
      errorstatus = SDEnWideBus(ENABLE);

      if (SDIO_OK == errorstatus)
      {
        /*!< Configure the SDIO peripheral */
        SDIO_InitStructure.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV;
        SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
        SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
        SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
        SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_4b;
        SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
        SDIO_Init(&SDIO_InitStructure);
      }
    }
    else
    {
      errorstatus = SDEnWideBus(DISABLE);

      if (SDIO_OK == errorstatus)
      {
        /*!< Configure the SDIO peripheral */
        SDIO_InitStructure.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV;
        SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
        SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
        SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
        SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;
        SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
        SDIO_Init(&SDIO_InitStructure);
      }
    }
  }

  return(errorstatus);
}


SDIO_STATE SDIO_SelectDeselect(uint32_t addr)
{
  SDIO_STATE errorstatus = SDIO_OK;

  /*!< Send CMD7 SDIO_SEL_DESEL_CARD */	
	SDIO_SendCMD(SDIO_CMD_SEL_DESEL_CARD, addr, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_SEL_DESEL_CARD);

  return(errorstatus);
}


SDIO_STATE SDIO_Erase(uint32_t startaddr, uint32_t endaddr)
{//Allows to erase memory area specified for the given card.
  SDIO_STATE errorstatus = SDIO_OK;
  uint32_t delay = 0;
  __IO uint32_t maxdelay = 0;
  uint8_t cardstate = 0;

  /*!< Check if the card coomnd class supports erase command */
  if (((CSDIO_Tab[1] >> 20) & SDIO_CCCC_ERASE) == 0)
  {
    errorstatus = SDIO_REQUEST_NOT_APPLICABLE;
    return(errorstatus);
  }

  maxdelay = 120000 / ((SDIO->CLKCR & 0xFF) + 2);

  if (SDIO_GetResponse(SDIO_RESP1) & SDIO_CARD_LOCKED)
  {
    errorstatus = SDIO_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }

  if (CardType == SDIO_HIGH_CAPACITY_SDIO_CARD)
  {
    startaddr /= 512;
    endaddr /= 512;
  }

  //According to sd-card spec 1.0 ERASE_GROUP_START and erase_group_end
  if ((SDIO_STD_CAPACITY_SDIO_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SDIO_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SDIO_CARD == CardType))
  {
    //Send CMD32 SDIO_ERASE_GRP_START with argument as addr 		
		SDIO_SendCMD(SDIO_CMD_SDIO_ERASE_GRP_START, (uint32_t) startaddr, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
    errorstatus = CmdResp1Error(SDIO_CMD_SDIO_ERASE_GRP_START);
    if (errorstatus != SDIO_OK)			return(errorstatus);

    // Send CMD33 SDIO_ERASE_GRP_END with argument as addr 
		SDIO_SendCMD(SDIO_CMD_SDIO_ERASE_GRP_END, (uint32_t) endaddr, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
    errorstatus = CmdResp1Error(SDIO_CMD_SDIO_ERASE_GRP_END);
    if (errorstatus != SDIO_OK)			return(errorstatus);
  }

  //Send CMD38 ERASE
	SDIO_SendCMD(SDIO_CMD_ERASE, (uint32_t) 0x00, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_ERASE);
  if (errorstatus != SDIO_OK)return(errorstatus);

  for (delay = 0; delay < maxdelay; delay++);

  //Wait till the card is in programming state
  errorstatus = IsCardProgramming(&cardstate);
  delay = SDIO_DATATIMEOUT;
  while ((delay > 0) && (errorstatus == SDIO_OK) && ((SDIO_CARD_PROGRAMMING == cardstate) || (SDIO_CARD_RECEIVING == cardstate)))
  {
    errorstatus = IsCardProgramming(&cardstate);
    delay--;
  }

  return(errorstatus);
}

SDIO_STATE SDIO_SendStatus(uint32_t *pcardstatus)
{//Returns the current card's status.
  SDIO_STATE errorstatus = SDIO_OK;

  if (pcardstatus == NULL)
  {
    errorstatus = SDIO_INVALID_PARAMETER;
    return(errorstatus);
  }

	SDIO_SendCMD(SDIO_CMD_SEND_STATUS, (uint32_t) RCA << 16, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_SEND_STATUS);
  if (errorstatus != SDIO_OK)    return(errorstatus);

  *pcardstatus = SDIO_GetResponse(SDIO_RESP1);
	
  return(errorstatus);
}

SDIO_STATE SDIO_SendSDStatus(uint32_t *psdstatus)
{//Returns the current SD card's status.
  SDIO_STATE errorstatus = SDIO_OK;
  uint32_t count = 0;

  if (SDIO_GetResponse(SDIO_RESP1) & SDIO_CARD_LOCKED)
  {
    errorstatus = SDIO_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }

  // Set block size for card if it is not equal to current block size for card.
	SDIO_SendCMD(SDIO_CMD_SET_BLOCKLEN, (uint32_t) 64, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
	errorstatus = CmdResp1Error(SDIO_CMD_SET_BLOCKLEN);
  if (errorstatus != SDIO_OK)    return(errorstatus);


  // SDIO_CMD_APP_CMD
	SDIO_SendCMD(SDIO_CMD_APP_CMD, (uint32_t) RCA << 16, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_APP_CMD);
  if (errorstatus != SDIO_OK)    return(errorstatus);

  SDIO_DataInitStructure.SDIO_DataTimeOut = SDIO_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = 64;
  SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_64b;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);

  //Send ACMD13 SDIO_APP_STAUS  with argument as card's RCA
	SDIO_SendCMD(SDIO_CMD_SDIO_APP_STAUS, (uint32_t) 0, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_SDIO_APP_STAUS);
  if (errorstatus != SDIO_OK)    return(errorstatus);

  while (!(SDIO->STA &(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR)))
  {
    if (SDIO_GetFlagStatus(SDIO_FLAG_RXFIFOHF) != RESET)
    {
      for (count = 0; count < 8; count++)
      {
        *(psdstatus + count) = SDIO_ReadData();
      }
      psdstatus += 8;
    }
  }

  if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
    errorstatus = SDIO_DATA_TIMEOUT;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
    errorstatus = SDIO_DATA_CRC_FAIL;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
    errorstatus = SDIO_RX_OVERRUN;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_STBITERR);
    errorstatus = SDIO_START_BIT_ERR;
    return(errorstatus);
  }

  count = SDIO_DATATIMEOUT;
  while ((SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET) && (count > 0))
  {
    *psdstatus = SDIO_ReadData();
    psdstatus++;
    count--;
  }
  /*!< Clear all the static status flags*/
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  return(errorstatus);
}


SDIO_STATE SDEnWideBus(FunctionalState NewState)
{// Enables or disables the SDIO wide bus mode.
  SDIO_STATE errorstatus = SDIO_OK;

  uint32_t scr[2] = {0, 0};

  if (SDIO_GetResponse(SDIO_RESP1) & SDIO_CARD_LOCKED)
  {
    errorstatus = SDIO_LOCK_UNLOCK_FAILED;
    return(errorstatus);
  }

  /*!< Get SCR Register */
  errorstatus = FindSCR(RCA, scr);

  if (errorstatus != SDIO_OK)
  {
    return(errorstatus);
  }

  /*!< If wide bus operation to be enabled */
  if (NewState == ENABLE)
  {
    /*!< If requested card supports wide bus operation */
    if ((scr[1] & SDIO_WIDE_BUS_SUPPORT) != SDIO_ALLZERO)
    {
      /*!< Send CMD55 APP_CMD with argument as card's RCA.*/
			SDIO_SendCMD(SDIO_CMD_APP_CMD, (uint32_t) RCA << 16, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
      errorstatus = CmdResp1Error(SDIO_CMD_APP_CMD);
      if (errorstatus != SDIO_OK)return(errorstatus);

      /*!< Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
			SDIO_SendCMD(SDIO_CMD_APP_SDIO_SET_BUSWIDTH, (uint32_t) 0x2, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
      errorstatus = CmdResp1Error(SDIO_CMD_APP_SDIO_SET_BUSWIDTH);

      return(errorstatus);
    }
    else
    {
      errorstatus = SDIO_REQUEST_NOT_APPLICABLE;
      return(errorstatus);
    }
  }   /*!< If wide bus operation to be disabled */
  else
  {
    /*!< If requested card supports 1 bit mode operation */
    if ((scr[1] & SDIO_SINGLE_BUS_SUPPORT) != SDIO_ALLZERO)
    {
      /*!< Send CMD55 APP_CMD with argument as card's RCA.*/
			SDIO_SendCMD(SDIO_CMD_APP_CMD, (uint32_t) RCA << 16, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
      errorstatus = CmdResp1Error(SDIO_CMD_APP_CMD);
      if (errorstatus != SDIO_OK)		return(errorstatus);

      /*!< Send ACMD6 APP_CMD with argument as 2 for wide bus mode */
			SDIO_SendCMD(SDIO_CMD_APP_SDIO_SET_BUSWIDTH, (uint32_t) 0x00, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
      errorstatus = CmdResp1Error(SDIO_CMD_APP_SDIO_SET_BUSWIDTH);
      return(errorstatus);
    }
    else
    {
      errorstatus = SDIO_REQUEST_NOT_APPLICABLE;
      return(errorstatus);
    }
  }
}

SDIO_STATE IsCardProgramming(uint8_t *pstatus)
{//Checks if the SD card is in programming state.
  SDIO_STATE errorstatus = SDIO_OK;
  __IO uint32_t respR1 = 0, status = 0;
	
	SDIO_SendCMD(SDIO_CMD_SEND_STATUS, (uint32_t) RCA << 16, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );

  status = SDIO->STA;
  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SDIO_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if (status & SDIO_FLAG_CCRCFAIL)
  {
    errorstatus = SDIO_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  status = (uint32_t)SDIO_GetCommandResponse();

  /*!< Check response received is of desired command */
  if (status != SDIO_CMD_SEND_STATUS)
  {
    errorstatus = SDIO_ILLEGAL_CMD;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);


  /*!< We have received response, retrieve it for analysis  */
  respR1 = SDIO_GetResponse(SDIO_RESP1);

  /*!< Find out card status */
  *pstatus = (uint8_t) ((respR1 >> 9) & 0x0000000F);

  if ((respR1 & SDIO_OCR_ERRORBITS) == SDIO_ALLZERO)
  {
    return(errorstatus);
  }

  if (respR1 & SDIO_OCR_ADDR_OUT_OF_RANGE)
  {
    return(SDIO_ADDR_OUT_OF_RANGE);
  }

  if (respR1 & SDIO_OCR_ADDR_MISALIGNED)
  {
    return(SDIO_ADDR_MISALIGNED);
  }

  if (respR1 & SDIO_OCR_BLOCK_LEN_ERR)
  {
    return(SDIO_BLOCK_LEN_ERR);
  }

  if (respR1 & SDIO_OCR_ERASE_SEQ_ERR)
  {
    return(SDIO_ERASE_SEQ_ERR);
  }

  if (respR1 & SDIO_OCR_BAD_ERASE_PARAM)
  {
    return(SDIO_BAD_ERASE_PARAM);
  }

  if (respR1 & SDIO_OCR_WRITE_PROT_VIOLATION)
  {
    return(SDIO_WRITE_PROT_VIOLATION);
  }

  if (respR1 & SDIO_OCR_LOCK_UNLOCK_FAILED)
  {
    return(SDIO_LOCK_UNLOCK_FAILED);
  }

  if (respR1 & SDIO_OCR_COM_CRC_FAILED)
  {
    return(SDIO_COM_CRC_FAILED);
  }

  if (respR1 & SDIO_OCR_ILLEGAL_CMD)
  {
    return(SDIO_ILLEGAL_CMD);
  }

  if (respR1 & SDIO_OCR_CARD_ECC_FAILED)
  {
    return(SDIO_CARD_ECC_FAILED);
  }

  if (respR1 & SDIO_OCR_CC_ERROR)
  {
    return(SDIO_CC_ERROR);
  }

  if (respR1 & SDIO_OCR_GENERAL_UNKNOWN_ERROR)
  {
    return(SDIO_GENERAL_UNKNOWN_ERROR);
  }

  if (respR1 & SDIO_OCR_STREAM_READ_UNDERRUN)
  {
    return(SDIO_STREAM_READ_UNDERRUN);
  }

  if (respR1 & SDIO_OCR_STREAM_WRITE_OVERRUN)
  {
    return(SDIO_STREAM_WRITE_OVERRUN);
  }

  if (respR1 & SDIO_OCR_CID_CSDIO_OVERWRIETE)
  {
    return(SDIO_CID_CSDIO_OVERWRITE);
  }

  if (respR1 & SDIO_OCR_WP_ERASE_SKIP)
  {
    return(SDIO_WP_ERASE_SKIP);
  }

  if (respR1 & SDIO_OCR_CARD_ECC_DISABLED)
  {
    return(SDIO_CARD_ECC_DISABLED);
  }

  if (respR1 & SDIO_OCR_ERASE_RESET)
  {
    return(SDIO_ERASE_RESET);
  }

  if (respR1 & SDIO_OCR_AKE_SEQ_ERROR)
  {
    return(SDIO_AKE_SEQ_ERROR);
  }

  return(errorstatus);
}


SDIO_STATE FindSCR(uint16_t rca, uint32_t *pscr)
{//Find the SD card SCR register value.
  uint32_t index = 0;
  SDIO_STATE errorstatus = SDIO_OK;
  uint32_t tempscr[2] = {0, 0};

  /*!< Set Block Size To 8 Bytes */
  /*!< Send CMD55 APP_CMD with argument as card's RCA */	
	SDIO_SendCMD(SDIO_CMD_SET_BLOCKLEN, (uint32_t) 8, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_SET_BLOCKLEN);
  if (errorstatus != SDIO_OK)	return(errorstatus);

  /*!< Send CMD55 APP_CMD with argument as card's RCA */
	SDIO_SendCMD(SDIO_CMD_APP_CMD, (uint32_t) RCA << 16, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_APP_CMD);
  if (errorstatus != SDIO_OK) return(errorstatus);
	
  SDIO_DataInitStructure.SDIO_DataTimeOut = SDIO_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = 8;
  SDIO_DataInitStructure.SDIO_DataBlockSize = SDIO_DataBlockSize_8b;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);


  /*!< Send ACMD51 SDIO_APP_SEND_SCR with argument as 0 */
	SDIO_SendCMD(SDIO_CMD_SDIO_APP_SEND_SCR, (uint32_t) 0x0, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_SDIO_APP_SEND_SCR);
  if (errorstatus != SDIO_OK)   return(errorstatus);

  while (!(SDIO->STA & (SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR)))
  {
    if (SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET)
    {
      *(tempscr + index) = SDIO_ReadData();
      index++;
    }
  }

  if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
    errorstatus = SDIO_DATA_TIMEOUT;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
    errorstatus = SDIO_DATA_CRC_FAIL;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
    errorstatus = SDIO_RX_OVERRUN;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_STBITERR);
    errorstatus = SDIO_START_BIT_ERR;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  *(pscr + 1) = ((tempscr[0] & SDIO_0TO7BITS) << 24) | ((tempscr[0] & SDIO_8TO15BITS) << 8) | ((tempscr[0] & SDIO_16TO23BITS) >> 8) | ((tempscr[0] & SDIO_24TO31BITS) >> 24);

  *(pscr) = ((tempscr[1] & SDIO_0TO7BITS) << 24) | ((tempscr[1] & SDIO_8TO15BITS) << 8) | ((tempscr[1] & SDIO_16TO23BITS) >> 8) | ((tempscr[1] & SDIO_24TO31BITS) >> 24);

  return(errorstatus);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Read - Write Operation
//
SDIO_STATE SDIO_ReadBlock(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize)
{
  SDIO_STATE errorstatus = SDIO_OK;
#ifndef SDIO_DMA
  uint32_t count = 0, *tempbuff = (uint32_t *)readbuff;
#endif

  TransferError = SDIO_OK;
  TransferEnd = 0;
  StopCondition = 0;

  SDIO->DCTRL = 0x0;


  if (CardType == SDIO_HIGH_CAPACITY_SDIO_CARD)
  {
    BlockSize = 512;
    ReadAddr /= 512;
  }

  /* Set Block Size for Card */	
	SDIO_SendCMD(SDIO_CMD_SET_BLOCKLEN, (uint32_t) BlockSize, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_SET_BLOCKLEN);
  if (SDIO_OK != errorstatus)	return(errorstatus);

  SDIO_DataInitStructure.SDIO_DataTimeOut = SDIO_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = BlockSize;
  SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) 9 << 4;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);

  /*!< Send CMD17 READ_SINGLE_BLOCK */
	SDIO_SendCMD(SDIO_CMD_READ_SINGLE_BLOCK, (uint32_t) ReadAddr, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_READ_SINGLE_BLOCK);
  if (errorstatus != SDIO_OK)	return(errorstatus);



#ifdef SDIO_DMA
    SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR, ENABLE);
    SDIO_DMACmd(ENABLE);
    SDIO_DMA_RxConfig((uint32_t *)readbuff, BlockSize);
#else
  /*!< In case of single block transfer, no need of stop transfer at all.*/
  /*!< Polling mode */
  while (!(SDIO->STA &(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR)))
  {
    if (SDIO_GetFlagStatus(SDIO_FLAG_RXFIFOHF) != RESET)
    {
      for (count = 0; count < 8; count++)
      {
        *(tempbuff + count) = SDIO_ReadData();
      }
      tempbuff += 8;
    }
  }

  if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
    errorstatus = SDIO_DATA_TIMEOUT;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
    errorstatus = SDIO_DATA_CRC_FAIL;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
    errorstatus = SDIO_RX_OVERRUN;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_STBITERR);
    errorstatus = SDIO_START_BIT_ERR;
    return(errorstatus);
  }
  count = SDIO_DATATIMEOUT;
  while ((SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET) && (count > 0))
  {
    *tempbuff = SDIO_ReadData();
    tempbuff++;
    count--;
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);
#endif

  return(errorstatus);
}

SDIO_STATE SDIO_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint32_t BlockSize, uint32_t NumberOfBlocks)
{
  SDIO_STATE errorstatus = SDIO_OK;
  TransferError = SDIO_OK;
  TransferEnd = 0;
  StopCondition = 1;

  SDIO->DCTRL = 0x0;

  if (CardType == SDIO_HIGH_CAPACITY_SDIO_CARD)
    BlockSize = 512;
  else
    ReadAddr *= BlockSize; // Convert to Bytes for NON SDHC

  /*!< Set Block Size for Card */
	SDIO_SendCMD(SDIO_CMD_SET_BLOCKLEN, BlockSize, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_SET_BLOCKLEN);
  if (SDIO_OK != errorstatus)return(errorstatus);

  SDIO_DataInitStructure.SDIO_DataTimeOut = SDIO_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = NumberOfBlocks * BlockSize;
  SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) 9 << 4;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);

  /*!< Send CMD18 READ_MULT_BLOCK with argument data address */
	SDIO_SendCMD(SDIO_CMD_READ_MULT_BLOCK, ReadAddr, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_READ_MULT_BLOCK);
  if (errorstatus != SDIO_OK)return(errorstatus);

  SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR, ENABLE);
  SDIO_DMACmd(ENABLE);
  SDIO_DMA_RxConfig((uint32_t *)readbuff, (NumberOfBlocks * BlockSize));

  return(errorstatus);
}


SDIO_STATE SDIO_WaitReadOperation(void)
{
  SDIO_STATE errorstatus = SDIO_OK;
  uint32_t timeout;

  timeout = SDIO_DATATIMEOUT;

  while ((DMAEndOfTransfer == 0x00) && (TransferEnd == 0) && (TransferError == SDIO_OK) && (timeout > 0))
  {
    timeout--;
  }

  DMAEndOfTransfer = 0x00;

  timeout = SDIO_DATATIMEOUT;

  while(((SDIO->STA & SDIO_FLAG_RXACT)) && (timeout > 0))
  {
    timeout--;
  }

  if (StopCondition == 1)
  {
    errorstatus = SDIO_StopTransfer();
  }

  if ((timeout == 0) && (errorstatus == SDIO_OK))
  {
    errorstatus = SDIO_DATA_TIMEOUT;
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  if (TransferError != SDIO_OK)
  {
    return(TransferError);
  }
  else
  {
    return(errorstatus);
  }
}


SDIO_STATE SDIO_WriteBlock(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize)
{
  SDIO_STATE errorstatus = SDIO_OK;

#ifndef SDIO_DMA
  uint32_t bytestransferred = 0, count = 0, restwords = 0;
  uint32_t *tempbuff = (uint32_t *)writebuff;
#endif

  TransferError = SDIO_OK;
  TransferEnd = 0;
  StopCondition = 0;

  SDIO->DCTRL = 0x0;


  if (CardType == SDIO_HIGH_CAPACITY_SDIO_CARD)
  {
    BlockSize = 512;
    WriteAddr /= 512;
  }

  /* Set Block Size for Card */
	SDIO_SendCMD(SDIO_CMD_SET_BLOCKLEN, (uint32_t)BlockSize, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_SET_BLOCKLEN);
  if (errorstatus != SDIO_OK)return(errorstatus);

  /*!< Send CMD24 WRITE_SINGLE_BLOCK */
	SDIO_SendCMD(SDIO_CMD_WRITE_SINGLE_BLOCK, WriteAddr, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_WRITE_SINGLE_BLOCK);
  if (errorstatus != SDIO_OK)return(errorstatus);

  SDIO_DataInitStructure.SDIO_DataTimeOut = SDIO_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = BlockSize;
  SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) 9 << 4;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);

#ifdef SDIO_DMA
  SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR, ENABLE);
  SDIO_DMA_TxConfig((uint32_t *)writebuff, BlockSize);
  SDIO_DMACmd(ENABLE);
#else
  /*!< In case of single data block transfer no need of stop command at all */
  while (!(SDIO->STA & (SDIO_FLAG_DBCKEND | SDIO_FLAG_TXUNDERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_STBITERR)))
  {
    if (SDIO_GetFlagStatus(SDIO_FLAG_TXFIFOHE) != RESET)
    {
      if ((512 - bytestransferred) < 32)
      {
        restwords = ((512 - bytestransferred) % 4 == 0) ? ((512 - bytestransferred) / 4) : (( 512 -  bytestransferred) / 4 + 1);
        for (count = 0; count < restwords; count++, tempbuff++, bytestransferred += 4)
        {
          SDIO_WriteData(*tempbuff);
        }
      }
      else
      {
        for (count = 0; count < 8; count++)
        {
          SDIO_WriteData(*(tempbuff + count));
        }
        tempbuff += 8;
        bytestransferred += 32;
      }
    }
  }
  if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
    errorstatus = SDIO_DATA_TIMEOUT;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
    errorstatus = SDIO_DATA_CRC_FAIL;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_TXUNDERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_TXUNDERR);
    errorstatus = SDIO_TX_UNDERRUN;
    return(errorstatus);
  }
  else if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET)
  {
    SDIO_ClearFlag(SDIO_FLAG_STBITERR);
    errorstatus = SDIO_START_BIT_ERR;
    return(errorstatus);
  }
#endif

  return(errorstatus);
}


SDIO_STATE SDIO_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint32_t BlockSize, uint32_t NumberOfBlocks)
{
  SDIO_STATE errorstatus = SDIO_OK;

  TransferError = SDIO_OK;
  TransferEnd = 0;
  StopCondition = 1;

  SDIO->DCTRL = 0x0;

  if (CardType == SDIO_HIGH_CAPACITY_SDIO_CARD)
    BlockSize = 512;
  else
    WriteAddr *= BlockSize; // Convert to Bytes for NON SDHC
  
  //Set Block Size for Card
	SDIO_SendCMD(SDIO_CMD_SET_BLOCKLEN, (uint32_t)BlockSize, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_SET_BLOCKLEN);
  if (errorstatus != SDIO_OK)return(errorstatus);

  //To improve performance
	SDIO_SendCMD(SDIO_CMD_APP_CMD, (uint32_t) (RCA << 16), SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_APP_CMD);
  if (errorstatus != SDIO_OK) return(errorstatus);
		
  //To improve performance
	SDIO_SendCMD(SDIO_CMD_SET_BLOCK_COUNT, (uint32_t) NumberOfBlocks, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_SET_BLOCK_COUNT);
  if (errorstatus != SDIO_OK)return(errorstatus);

  //Send CMD25 WRITE_MULT_BLOCK with argument data address
	SDIO_SendCMD(SDIO_CMD_WRITE_MULT_BLOCK, (uint32_t) WriteAddr, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_WRITE_MULT_BLOCK);
  if (errorstatus != SDIO_OK)return(errorstatus);

  SDIO_DataInitStructure.SDIO_DataTimeOut = SDIO_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLength = NumberOfBlocks * BlockSize;
  SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) 9 << 4;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(&SDIO_DataInitStructure);

  SDIO_ITConfig(SDIO_IT_DCRCFAIL | SDIO_IT_DTIMEOUT | SDIO_IT_DATAEND | SDIO_IT_RXOVERR | SDIO_IT_STBITERR, ENABLE);
  SDIO_DMACmd(ENABLE);
  SDIO_DMA_TxConfig((uint32_t *)writebuff, (NumberOfBlocks * BlockSize));

  return(errorstatus);
}



SDIO_STATE SDIO_WaitWriteOperation(void)
{
  SDIO_STATE errorstatus = SDIO_OK;
  uint32_t timeout;

  timeout = SDIO_DATATIMEOUT;

  while ((DMAEndOfTransfer == 0x00) && (TransferEnd == 0) && (TransferError == SDIO_OK) && (timeout > 0))
  {
    timeout--;
  }

  DMAEndOfTransfer = 0x00;

  timeout = SDIO_DATATIMEOUT;

  while(((SDIO->STA & SDIO_FLAG_TXACT)) && (timeout > 0))
  {
    timeout--;
  }

  if (StopCondition == 1)
  {
    errorstatus = SDIO_StopTransfer();
  }

  if ((timeout == 0) && (errorstatus == SDIO_OK))
  {
    errorstatus = SDIO_DATA_TIMEOUT;
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  if (TransferError != SDIO_OK)
  {
    return(TransferError);
  }
  else
  {
    return(errorstatus);
  }
}

SDTransferState SDIO_GetTransferState(void)
{
  if (SDIO->STA & (SDIO_FLAG_TXACT | SDIO_FLAG_RXACT))    return(SDIO_TRANSFER_BUSY);
  else																										return(SDIO_TRANSFER_OK);
}

SDIO_STATE SDIO_StopTransfer(void)
{//Aborts an ongoing data transfer.
  SDIO_STATE errorstatus = SDIO_OK;
	
	SDIO_SendCMD(SDIO_CMD_STOP_TRANSMISSION, (uint32_t) 0x00, SDIO_Response_Short, SDIO_Wait_No, SDIO_CPSM_Enable );
  errorstatus = CmdResp1Error(SDIO_CMD_STOP_TRANSMISSION);
  return(errorstatus);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Check error
//
SDIO_STATE CmdError(void)
{//Checks for error conditions for CMD0.
  SDIO_STATE errorstatus = SDIO_OK;
  uint32_t timeout;

  timeout = SDIO_CMD0TIMEOUT; /*!< 10000 */

  while ((timeout > 0) && (SDIO_GetFlagStatus(SDIO_FLAG_CMDSENT) == RESET))
  {
    timeout--;
  }

  if (timeout == 0)
  {
    errorstatus = SDIO_CMD_RSP_TIMEOUT;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  return(errorstatus);
}


SDIO_STATE CmdResp1Error(uint8_t cmd)
{//Checks for error conditions for R1 response.
  SDIO_STATE errorstatus = SDIO_OK;
  uint32_t status;
  uint32_t response_r1;

  status = SDIO->STA;

  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SDIO_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if (status & SDIO_FLAG_CCRCFAIL)
  {
    errorstatus = SDIO_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  /*!< Check response received is of desired command */
  if (SDIO_GetCommandResponse() != cmd)
  {
    errorstatus = SDIO_ILLEGAL_CMD;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  /*!< We have received response, retrieve it for analysis  */
  response_r1 = SDIO_GetResponse(SDIO_RESP1);

  if ((response_r1 & SDIO_OCR_ERRORBITS) == SDIO_ALLZERO)
  {
    return(errorstatus);
  }

  if (response_r1 & SDIO_OCR_ADDR_OUT_OF_RANGE)
  {
    return(SDIO_ADDR_OUT_OF_RANGE);
  }

  if (response_r1 & SDIO_OCR_ADDR_MISALIGNED)
  {
    return(SDIO_ADDR_MISALIGNED);
  }

  if (response_r1 & SDIO_OCR_BLOCK_LEN_ERR)
  {
    return(SDIO_BLOCK_LEN_ERR);
  }

  if (response_r1 & SDIO_OCR_ERASE_SEQ_ERR)
  {
    return(SDIO_ERASE_SEQ_ERR);
  }

  if (response_r1 & SDIO_OCR_BAD_ERASE_PARAM)
  {
    return(SDIO_BAD_ERASE_PARAM);
  }

  if (response_r1 & SDIO_OCR_WRITE_PROT_VIOLATION)
  {
    return(SDIO_WRITE_PROT_VIOLATION);
  }

  if (response_r1 & SDIO_OCR_LOCK_UNLOCK_FAILED)
  {
    return(SDIO_LOCK_UNLOCK_FAILED);
  }

  if (response_r1 & SDIO_OCR_COM_CRC_FAILED)
  {
    return(SDIO_COM_CRC_FAILED);
  }

  if (response_r1 & SDIO_OCR_ILLEGAL_CMD)
  {
    return(SDIO_ILLEGAL_CMD);
  }

  if (response_r1 & SDIO_OCR_CARD_ECC_FAILED)
  {
    return(SDIO_CARD_ECC_FAILED);
  }

  if (response_r1 & SDIO_OCR_CC_ERROR)
  {
    return(SDIO_CC_ERROR);
  }

  if (response_r1 & SDIO_OCR_GENERAL_UNKNOWN_ERROR)
  {
    return(SDIO_GENERAL_UNKNOWN_ERROR);
  }

  if (response_r1 & SDIO_OCR_STREAM_READ_UNDERRUN)
  {
    return(SDIO_STREAM_READ_UNDERRUN);
  }

  if (response_r1 & SDIO_OCR_STREAM_WRITE_OVERRUN)
  {
    return(SDIO_STREAM_WRITE_OVERRUN);
  }

  if (response_r1 & SDIO_OCR_CID_CSDIO_OVERWRIETE)
  {
    return(SDIO_CID_CSDIO_OVERWRITE);
  }

  if (response_r1 & SDIO_OCR_WP_ERASE_SKIP)
  {
    return(SDIO_WP_ERASE_SKIP);
  }

  if (response_r1 & SDIO_OCR_CARD_ECC_DISABLED)
  {
    return(SDIO_CARD_ECC_DISABLED);
  }

  if (response_r1 & SDIO_OCR_ERASE_RESET)
  {
    return(SDIO_ERASE_RESET);
  }

  if (response_r1 & SDIO_OCR_AKE_SEQ_ERROR)
  {
    return(SDIO_AKE_SEQ_ERROR);
  }
  return(errorstatus);
}


SDIO_STATE CmdResp2Error(void)
{//Checks for error conditions for R2 (CID or CSD) response.
  SDIO_STATE errorstatus = SDIO_OK;
  uint32_t status;

  status = SDIO->STA;

  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SDIO_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if (status & SDIO_FLAG_CCRCFAIL)
  {
    errorstatus = SDIO_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  return(errorstatus);
}

SDIO_STATE CmdResp3Error(void)
{//Checks for error conditions for R3 (OCR) response.
  SDIO_STATE errorstatus = SDIO_OK;
  uint32_t status;

  status = SDIO->STA;

  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SDIO_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);
  return(errorstatus);
}

SDIO_STATE CmdResp6Error(uint8_t cmd, uint16_t *prca)
{//Checks for error conditions for R6 (RCA) response.
  SDIO_STATE errorstatus = SDIO_OK;
  uint32_t status;
  uint32_t response_r1;

  status = SDIO->STA;

  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CTIMEOUT | SDIO_FLAG_CMDREND)))
  {
    status = SDIO->STA;
  }

  if (status & SDIO_FLAG_CTIMEOUT)
  {
    errorstatus = SDIO_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }
  else if (status & SDIO_FLAG_CCRCFAIL)
  {
    errorstatus = SDIO_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
    return(errorstatus);
  }

  /*!< Check response received is of desired command */
  if (SDIO_GetCommandResponse() != cmd)
  {
    errorstatus = SDIO_ILLEGAL_CMD;
    return(errorstatus);
  }

  /*!< Clear all the static flags */
  SDIO_ClearFlag(SDIO_STATIC_FLAGS);

  /*!< We have received response, retrieve it.  */
  response_r1 = SDIO_GetResponse(SDIO_RESP1);

  if (SDIO_ALLZERO == (response_r1 & (SDIO_R6_GENERAL_UNKNOWN_ERROR | SDIO_R6_ILLEGAL_CMD | SDIO_R6_COM_CRC_FAILED)))
  {
    *prca = (uint16_t) (response_r1 >> 16);
    return(errorstatus);
  }

  if (response_r1 & SDIO_R6_GENERAL_UNKNOWN_ERROR)
  {
    return(SDIO_GENERAL_UNKNOWN_ERROR);
  }

  if (response_r1 & SDIO_R6_ILLEGAL_CMD)
  {
    return(SDIO_ILLEGAL_CMD);
  }

  if (response_r1 & SDIO_R6_COM_CRC_FAILED)
  {
    return(SDIO_COM_CRC_FAILED);
  }

  return(errorstatus);
}

SDIO_STATE CmdResp7Error(void)
{//Checks for error conditions for R7 response.
  SDIO_STATE errorstatus = SDIO_OK;
  uint32_t status;
  uint32_t timeout = SDIO_CMD0TIMEOUT;

  status = SDIO->STA;

  while (!(status & (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_CMDREND | SDIO_FLAG_CTIMEOUT)) && (timeout > 0))
  {
    timeout--;
    status = SDIO->STA;
  }

  if ((timeout == 0) || (status & SDIO_FLAG_CTIMEOUT))
  {
    /*!< Card is not V2.0 complient or card does not support the set voltage range */
    errorstatus = SDIO_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_CTIMEOUT);
    return(errorstatus);
  }

  if (status & SDIO_FLAG_CMDREND)
  {
    /*!< Card is SD V2.0 compliant */
    errorstatus = SDIO_OK;
    SDIO_ClearFlag(SDIO_FLAG_CMDREND);
    return(errorstatus);
  }
  return(errorstatus);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Debug
//
char *SDIO_STRING_State[] = 
{
	"SD_OK",
	"SD_CMD_CRC_FAIL",                    /*= (1), !< Command response received (but CRC check failed) */
	"SD_DATA_CRC_FAIL",                   /*= (2), !< Data bock sent/received (CRC check Failed) */
	"SD_CMD_RSP_TIMEOUT",                 /*= (3), !< Command response timeout */
	"SD_DATA_TIMEOUT",                    /*= (4), !< Data time out */
	"SD_TX_UNDERRUN",                     /*= (5), !< Transmit FIFO under-run */
	"SD_RX_OVERRUN",                      /*= (6), !< Receive FIFO over-run */
	"SD_START_BIT_ERR",                   /*= (7), !< Start bit not detected on all data signals in widE bus mode */
	"SD_CMD_OUT_OF_RANGE",                /*= (8), !< CMD's argument was out of range.*/
	"SD_ADDR_MISALIGNED",                 /*= (9), !< Misaligned address */
	"SD_BLOCK_LEN_ERR",                   /*= (10), !< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
	"SD_ERASE_SEQ_ERR",                   /*= (11), !< An error in the sequence of erase command occurs.*/
	"SD_BAD_ERASE_PARAM",                 /*= (12), !< An Invalid selection for erase groups */
	"SD_WRITE_PROT_VIOLATION",            /*= (13), !< Attempt to program a write protect block */
	"SD_LOCK_UNLOCK_FAILED",              /*= (14), !< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
	"SD_COM_CRC_FAILED",                  /*= (15), !< CRC check of the previous command failed */
	"SD_ILLEGAL_CMD",                     /*= (16), !< Command is not legal for the card state */
	"SD_CARD_ECC_FAILED",                 /*= (17), !< Card internal ECC was applied but failed to correct the data */
	"SD_CC_ERROR",                        /*= (18), !< Internal card controller error */
	"SD_GENERAL_UNKNOWN_ERROR",           /*= (19), !< General or Unknown error */
	"SD_STREAM_READ_UNDERRUN",            /*= (20), !< The card could not sustain data transfer in stream read operation. */
	"SD_STREAM_WRITE_OVERRUN",            /*= (21), !< The card could not sustain data programming in stream mode */
	"SD_CID_CSD_OVERWRITE",               /*= (22), !< CID/CSD overwrite error */
	"SD_WP_ERASE_SKIP",                   /*= (23), !< only partial address space was erased */
	"SD_CARD_ECC_DISABLED",               /*= (24), !< Command has been executed without using internal ECC */
	"SD_ERASE_RESET",                     /*= (25), !< Erase sequence was cleared before executing because an out of erase sequence command was received */
	"SD_AKE_SEQ_ERROR",                   /*= (26), !< Error in sequence of authentication. */
	"SD_INVALID_VOLTRANGE",               /*= (27),*/
	"SD_ADDR_OUT_OF_RANGE",               /*= (28),*/
	"SD_SWITCH_ERROR",                    /*= (29),*/
	"SD_SDIO_DISABLED",                   /*= (30),*/
	"SD_SDIO_FUNCTION_BUSY",              /*= (31),*/
	"SD_SDIO_FUNCTION_FAILED",            /*= (32),*/
	"SD_SDIO_UNKNOWN_FUNCTION",           /*= (33),*/
	"SD_INTERNAL_ERROR",
	"SD_NOT_CONFIGURED",
	"SD_REQUEST_PENDING",
	"SD_REQUEST_NOT_APPLICABLE",
	"SD_INVALID_PARAMETER",
	"SD_UNSUPPORTED_FEATURE",
	"SD_UNSUPPORTED_HW",
	"SD_ERROR"
};


char *SDIO_STRING_CardType[] = 
{
	"V1.1",
	"V2.0",
	"HIGH CAPACITY SDIO_STATE",
	"MULTIMEDIA CARD",
	"SECURE DIGITAL I/O CARD",
	"HIGH SPEED MULTIMEDIA CARD",
	"SECURE DIGITAL IO COMBO CARD",
	"HIGH CAPACITY MMC CARD",
};

