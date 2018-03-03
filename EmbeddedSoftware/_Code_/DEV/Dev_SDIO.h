
#include "BoardConfig.h"


typedef enum
{//SDIO_STATE
  SDIO_CMD_CRC_FAIL                    = (1), /*!< Command response received (but CRC check failed) */
  SDIO_DATA_CRC_FAIL                   = (2), /*!< Data bock sent/received (CRC check Failed) */
  SDIO_CMD_RSP_TIMEOUT                 = (3), /*!< Command response timeout */
  SDIO_DATA_TIMEOUT                    = (4), /*!< Data time out */
  SDIO_TX_UNDERRUN                     = (5), /*!< Transmit FIFO under-run */
  SDIO_RX_OVERRUN                      = (6), /*!< Receive FIFO over-run */
  SDIO_START_BIT_ERR                   = (7), /*!< Start bit not detected on all data signals in widE bus mode */
  SDIO_CMD_OUT_OF_RANGE                = (8), /*!< CMD's argument was out of range.*/
  SDIO_ADDR_MISALIGNED                 = (9), /*!< Misaligned address */
  SDIO_BLOCK_LEN_ERR                   = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
  SDIO_ERASE_SEQ_ERR                   = (11), /*!< An error in the sequence of erase command occurs.*/
  SDIO_BAD_ERASE_PARAM                 = (12), /*!< An Invalid selection for erase groups */
  SDIO_WRITE_PROT_VIOLATION            = (13), /*!< Attempt to program a write protect block */
  SDIO_LOCK_UNLOCK_FAILED              = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
  SDIO_COM_CRC_FAILED                  = (15), /*!< CRC check of the previous command failed */
  SDIO_ILLEGAL_CMD                     = (16), /*!< Command is not legal for the card state */
  SDIO_CARD_ECC_FAILED                 = (17), /*!< Card internal ECC was applied but failed to correct the data */
  SDIO_CC_ERROR                        = (18), /*!< Internal card controller error */
  SDIO_GENERAL_UNKNOWN_ERROR           = (19), /*!< General or Unknown error */
  SDIO_STREAM_READ_UNDERRUN            = (20), /*!< The card could not sustain data transfer in stream read operation. */
  SDIO_STREAM_WRITE_OVERRUN            = (21), /*!< The card could not sustain data programming in stream mode */
  SDIO_CID_CSDIO_OVERWRITE           = (22), /*!< CID/CSD overwrite error */
  SDIO_WP_ERASE_SKIP                   = (23), /*!< only partial address space was erased */
  SDIO_CARD_ECC_DISABLED               = (24), /*!< Command has been executed without using internal ECC */
  SDIO_ERASE_RESET                     = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
  SDIO_AKE_SEQ_ERROR                   = (26), /*!< Error in sequence of authentication. */
  SDIO_INVALID_VOLTRANGE               = (27),
  SDIO_ADDR_OUT_OF_RANGE               = (28),
  SDIO_SWITCH_ERROR                    = (29),
  SDIO_SDIO_DISABLED                   = (30),
  SDIO_SDIO_FUNCTION_BUSY              = (31),
  SDIO_SDIO_FUNCTION_FAILED            = (32),
  SDIO_SDIO_UNKNOWN_FUNCTION           = (33),
	// Standard error defines
  SDIO_INTERNAL_ERROR,
  SDIO_NOT_CONFIGURED,
  SDIO_REQUEST_PENDING,
  SDIO_REQUEST_NOT_APPLICABLE,
  SDIO_INVALID_PARAMETER,
  SDIO_UNSUPPORTED_FEATURE,
  SDIO_UNSUPPORTED_HW,
  SDIO_ERROR,
  SDIO_OK = 0
} SDIO_STATE;


typedef enum
{//SDTransferState
  SDIO_TRANSFER_OK  = 0,
  SDIO_TRANSFER_BUSY = 1,
  SDIO_TRANSFER_ERROR
} SDTransferState;


typedef enum
{//SDCardState
  SDIO_CARD_READY                  = ((uint32_t)0x00000001),
  SDIO_CARD_IDENTIFICATION         = ((uint32_t)0x00000002),
  SDIO_CARD_STANDBY                = ((uint32_t)0x00000003),
  SDIO_CARD_TRANSFER               = ((uint32_t)0x00000004),
  SDIO_CARD_SENDING                = ((uint32_t)0x00000005),
  SDIO_CARD_RECEIVING              = ((uint32_t)0x00000006),
  SDIO_CARD_PROGRAMMING            = ((uint32_t)0x00000007),
  SDIO_CARD_DISCONNECTED           = ((uint32_t)0x00000008),
  SDIO_CARD_ERROR                  = ((uint32_t)0x000000FF)
}SDCardState;

typedef struct
{//SDIO_CSD
  __IO uint8_t  CSDStruct;            /*!< CSD structure */
  __IO uint8_t  SysSpecVersion;       /*!< System specification version */
  __IO uint8_t  Reserved1;            /*!< Reserved */
  __IO uint8_t  TAAC;                 /*!< Data read access-time 1 */
  __IO uint8_t  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
  __IO uint8_t  MaxBusClkFrec;        /*!< Max. bus clock frequency */
  __IO uint16_t CardComdClasses;      /*!< Card command classes */
  __IO uint8_t  RdBlockLen;           /*!< Max. read data block length */
  __IO uint8_t  PartBlockRead;        /*!< Partial blocks for read allowed */
  __IO uint8_t  WrBlockMisalign;      /*!< Write block misalignment */
  __IO uint8_t  RdBlockMisalign;      /*!< Read block misalignment */
  __IO uint8_t  DSRImpl;              /*!< DSR implemented */
  __IO uint8_t  Reserved2;            /*!< Reserved */
  __IO uint32_t DeviceSize;           /*!< Device Size */
  __IO uint8_t  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
  __IO uint8_t  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
  __IO uint8_t  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
  __IO uint8_t  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
  __IO uint8_t  DeviceSizeMul;        /*!< Device size multiplier */
  __IO uint8_t  EraseGrSize;          /*!< Erase group size */
  __IO uint8_t  EraseGrMul;           /*!< Erase group size multiplier */
  __IO uint8_t  WrProtectGrSize;      /*!< Write protect group size */
  __IO uint8_t  WrProtectGrEnable;    /*!< Write protect group enable */
  __IO uint8_t  ManDeflECC;           /*!< Manufacturer default ECC */
  __IO uint8_t  WrSpeedFact;          /*!< Write speed factor */
  __IO uint8_t  MaxWrBlockLen;        /*!< Max. write data block length */
  __IO uint8_t  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
  __IO uint8_t  Reserved3;            /*!< Reserded */
  __IO uint8_t  ContentProtectAppli;  /*!< Content protection application */
  __IO uint8_t  FileFormatGrouop;     /*!< File format group */
  __IO uint8_t  CopyFlag;             /*!< Copy flag (OTP) */
  __IO uint8_t  PermWrProtect;        /*!< Permanent write protection */
  __IO uint8_t  TempWrProtect;        /*!< Temporary write protection */
  __IO uint8_t  FileFormat;           /*!< File Format */
  __IO uint8_t  ECC;                  /*!< ECC code */
  __IO uint8_t  CSDIO_CRC;              /*!< CSD CRC */
  __IO uint8_t  Reserved4;            /*!< always 1*/
} SDIO_CSD;


typedef struct
{//SDIO_CID
  __IO uint8_t  ManufacturerID;       /*!< ManufacturerID */
  __IO uint16_t OEM_AppliID;          /*!< OEM/Application ID */
  __IO uint32_t ProdName1;            /*!< Product Name part1 */
  __IO uint8_t  ProdName2;            /*!< Product Name part2*/
  __IO uint8_t  ProdRev;              /*!< Product Revision */
  __IO uint32_t ProdSN;               /*!< Product Serial Number */
  __IO uint8_t  Reserved1;            /*!< Reserved1 */
  __IO uint16_t ManufactDate;         /*!< Manufacturing Date */
  __IO uint8_t  CID_CRC;              /*!< CID CRC */
  __IO uint8_t  Reserved2;            /*!< always 1 */
} SDIO_CID;


typedef struct
{//SDIO_CardStatus
  __IO uint8_t DAT_BUS_WIDTH;
  __IO uint8_t SECURED_MODE;
  __IO uint16_t CARD_TYPE;
  __IO uint32_t SIZE_OF_PROTECTED_AREA;
  __IO uint8_t SPEED_CLASS;
  __IO uint8_t PERFORMANCE_MOVE;
  __IO uint8_t AU_SIZE;
  __IO uint16_t ERASE_SIZE;
  __IO uint8_t ERASE_TIMEOUT;
  __IO uint8_t ERASE_OFFSET;
} SDIO_CardStatus;




typedef struct
{//SDIO_CardInfo
  SDIO_CSD SDIO_csd;
  SDIO_CID SDIO_cid;
  uint64_t CardCapacity;  /*!< Card Capacity */
  uint32_t CardBlockSize; /*!< Card Block Size */
  uint16_t RCA;
  uint8_t CardType;
} SDIO_CardInfo;

typedef enum
{ //CardType
 SDIO_STD_CAPACITY_SDIO_CARD_V1_1             = ((uint32_t)0x00000000),
 SDIO_STD_CAPACITY_SDIO_CARD_V2_0             = ((uint32_t)0x00000001),
 SDIO_HIGH_CAPACITY_SDIO_CARD                 = ((uint32_t)0x00000002),
 SDIO_MULTIMEDIA_CARD                       		= ((uint32_t)0x00000003),
 SDIO_SECURE_DIGITAL_IO_CARD                		= ((uint32_t)0x00000004),
 SDIO_HIGH_SPEED_MULTIMEDIA_CARD            		= ((uint32_t)0x00000005),
 SDIO_SECURE_DIGITAL_IO_COMBO_CARD          		= ((uint32_t)0x00000006),
 SDIO_HIGH_CAPACITY_MMC_CARD                		= ((uint32_t)0x00000007),
}SDIO_CardType;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																																																			 ////
////SDIO Commands  Index
//
typedef enum
{
 SDIO_CMD_GO_IDLE_STATE                       = ((uint8_t)0),
 SDIO_CMD_SEND_OP_COND                        = ((uint8_t)1),
 SDIO_CMD_ALL_SEND_CID                        = ((uint8_t)2),
 SDIO_CMD_SET_REL_ADDR                        = ((uint8_t)3), //!< SDIO_SEND_REL_ADDR for SD Card t)/
 SDIO_CMD_SET_DSR                             = ((uint8_t)4),
 SDIO_CMD_SDIO_SEN_OP_COND                    = ((uint8_t)5),
 SDIO_CMD_HS_SWITCH                           = ((uint8_t)6),
 SDIO_CMD_SEL_DESEL_CARD                      = ((uint8_t)7),
 SDIO_CMD_HS_SEND_EXT_CSD                     = ((uint8_t)8),
 SDIO_CMD_SEND_CSD                            = ((uint8_t)9),
 SDIO_CMD_SEND_CID                            = ((uint8_t)10),
 SDIO_CMD_READ_DAT_UNTIL_STOP                 = ((uint8_t)11), //t)!< SD Card doesn't support it t)/
 SDIO_CMD_STOP_TRANSMISSION                   = ((uint8_t)12),
 SDIO_CMD_SEND_STATUS                         = ((uint8_t)13),
 SDIO_CMD_HS_BUSTEST_READ                     = ((uint8_t)14),
 SDIO_CMD_GO_INACTIVE_STATE                   = ((uint8_t)15),
 SDIO_CMD_SET_BLOCKLEN                        = ((uint8_t)16),
 SDIO_CMD_READ_SINGLE_BLOCK                   = ((uint8_t)17),
 SDIO_CMD_READ_MULT_BLOCK                     = ((uint8_t)18),
 SDIO_CMD_HS_BUSTEST_WRITE                    = ((uint8_t)19),
 SDIO_CMD_WRITE_DAT_UNTIL_STOP                = ((uint8_t)20), //t)!< SD Card doesn't support it t)/
 SDIO_CMD_SET_BLOCK_COUNT                     = ((uint8_t)23), //t)!< SD Card doesn't support it t)/
 SDIO_CMD_WRITE_SINGLE_BLOCK                  = ((uint8_t)24),
 SDIO_CMD_WRITE_MULT_BLOCK                    = ((uint8_t)25),
 SDIO_CMD_PROG_CID                            = ((uint8_t)26), //t)!< reserved for manufacturers t)/
 SDIO_CMD_PROG_CSD                            = ((uint8_t)27),
 SDIO_CMD_SET_WRITE_PROT                      = ((uint8_t)28),
 SDIO_CMD_CLR_WRITE_PROT                      = ((uint8_t)29),
 SDIO_CMD_SEND_WRITE_PROT                     = ((uint8_t)30),
 SDIO_CMD_SDIO_ERASE_GRP_START                  = ((uint8_t)32), //t)!< To set the address of the first write block to be erased. (For SD card only)
 SDIO_CMD_SDIO_ERASE_GRP_END                    = ((uint8_t)33), //t)!< To set the address of the last write block of the continuous range to be erased. (For SD card only)
 SDIO_CMD_ERASE_GRP_START                     = ((uint8_t)35), //t)!< To set the address of the first write block to be erased. (For MMC card only spec 3.31)
 SDIO_CMD_ERASE_GRP_END                       = ((uint8_t)36), //t)!< To set the address of the last write block of the continuous range to be erased. (For MMC card only spec 3.31)
 SDIO_CMD_ERASE                               = ((uint8_t)38),
 SDIO_CMD_FAST_IO                             = ((uint8_t)39), //t)!< SD Card doesn't support it t)/
 SDIO_CMD_GO_IRQ_STATE                        = ((uint8_t)40), //t)!< SD Card doesn't support it t)/
 SDIO_CMD_LOCK_UNLOCK                         = ((uint8_t)42),
 SDIO_CMD_APP_CMD                             = ((uint8_t)55),
 SDIO_CMD_GEN_CMD                           	= ((uint8_t)56),
 SDIO_CMD_NO_CMD                          	  = ((uint8_t)64),
 
 SDIO_SEND_IF_COND               						= ((uint32_t)0x00000008),
 SDIO_CMD_APP_SDIO_SET_BUSWIDTH                 = ((uint8_t)6),  //!< For SD Card only t)/
 SDIO_CMD_SDIO_APP_STAUS                        = ((uint8_t)13), //!< For SD Card only t)/
 SDIO_CMD_SDIO_APP_SEND_NUM_WRITE_BLOCKS        = ((uint8_t)22), //!< For SD Card only t)/
 SDIO_CMD_SDIO_APP_OP_COND                      = ((uint8_t)41), //!< For SD Card only t)/
 SDIO_CMD_SDIO_APP_SET_CLR_CARD_DETECT          = ((uint8_t)42), //!< For SD Card only t)/
 SDIO_CMD_SDIO_APP_SEND_SCR                     = ((uint8_t)51), //!< For SD Card only t)/
 SDIO_CMD_SDIO_RW_DIRECT                      = ((uint8_t)52), //!< For SD I/O Card only t)/
 SDIO_CMD_SDIO_RW_EXTENDED                    = ((uint8_t)53), //!< For SD I/O Card only t)/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																																																			 ////
////SD Card Specific security commands, SDIO_APP_CMD should be sent before sending these commands.
//
 SDIO_CMD_SDIO_APP_GET_MKB                      = ((uint8_t)43), //!< For SD Card only t)/
 SDIO_CMD_SDIO_APP_GET_MID                      = ((uint8_t)44), //!< For SD Card only t)/
 SDIO_CMD_SDIO_APP_SET_CER_RN1                  = ((uint8_t)45), //!< For SD Card only t)/
 SDIO_CMD_SDIO_APP_GET_CER_RN2                  = ((uint8_t)46), //!< For SD Card only t)/
 SDIO_CMD_SDIO_APP_SET_CER_RES2                 = ((uint8_t)47), //!< For SD Card only t)/
 SDIO_CMD_SDIO_APP_GET_CER_RES1                 = ((uint8_t)48), //!< For SD Card only t)/
 SDIO_CMD_SDIO_APP_SECURE_READ_MULTIPLE_BLOCK		= ((uint8_t)18), //!< For SD Card only t)/
 SDIO_CMD_SDIO_APP_SECURE_WRITE_MULTIPLE_BLOCK	= ((uint8_t)25), //!< For SD Card only t)/
 SDIO_CMD_SDIO_APP_SECURE_ERASE									= ((uint8_t)38), //!< For SD Card only t)/
 SDIO_CMD_SDIO_APP_CHANGE_SECURE_AREA           = ((uint8_t)49), //!< For SD Card only t)/
 SDIO_CMD_SDIO_APP_SECURE_WRITE_MKB             = ((uint8_t)48), /*!< For SD Card only */
}SDIO_CMD;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																																																			 ////
////SD detection on its memory slot
// 
typedef enum
{//SDIO_DETECT
 SDIO_PRESENT                   =              ((uint8_t)0x01),
 SDIO_NOT_PRESENT               =              ((uint8_t)0x00),
}SDIO_DETECT;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																																																			 ////
//// Define
// 
#define NULL 0
#define SDIO_STATIC_FLAGS               ((uint32_t)0x000005FF)
#define SDIO_CMD0TIMEOUT                ((uint32_t)0x00010000)
#define SDIO_FIFO_ADDRESS                ((uint32_t)0x40012C80)
#define SDIO_INIT_CLK_DIV                ((uint8_t)0x76)
#define SDIO_TRANSFER_CLK_DIV            ((uint8_t)0x2)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																																																			 ////
////Mask for errors Card Status R1 (OCR Register)
// 
#define SDIO_OCR_ADDR_OUT_OF_RANGE        ((uint32_t)0x80000000)
#define SDIO_OCR_ADDR_MISALIGNED          ((uint32_t)0x40000000)
#define SDIO_OCR_BLOCK_LEN_ERR            ((uint32_t)0x20000000)
#define SDIO_OCR_ERASE_SEQ_ERR            ((uint32_t)0x10000000)
#define SDIO_OCR_BAD_ERASE_PARAM          ((uint32_t)0x08000000)
#define SDIO_OCR_WRITE_PROT_VIOLATION     ((uint32_t)0x04000000)
#define SDIO_OCR_LOCK_UNLOCK_FAILED       ((uint32_t)0x01000000)
#define SDIO_OCR_COM_CRC_FAILED           ((uint32_t)0x00800000)
#define SDIO_OCR_ILLEGAL_CMD              ((uint32_t)0x00400000)
#define SDIO_OCR_CARD_ECC_FAILED          ((uint32_t)0x00200000)
#define SDIO_OCR_CC_ERROR                 ((uint32_t)0x00100000)
#define SDIO_OCR_GENERAL_UNKNOWN_ERROR    ((uint32_t)0x00080000)
#define SDIO_OCR_STREAM_READ_UNDERRUN     ((uint32_t)0x00040000)
#define SDIO_OCR_STREAM_WRITE_OVERRUN     ((uint32_t)0x00020000)
#define SDIO_OCR_CID_CSDIO_OVERWRIETE       ((uint32_t)0x00010000)
#define SDIO_OCR_WP_ERASE_SKIP            ((uint32_t)0x00008000)
#define SDIO_OCR_CARD_ECC_DISABLED        ((uint32_t)0x00004000)
#define SDIO_OCR_ERASE_RESET              ((uint32_t)0x00002000)
#define SDIO_OCR_AKE_SEQ_ERROR            ((uint32_t)0x00000008)
#define SDIO_OCR_ERRORBITS                ((uint32_t)0xFDFFE008)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																																																			 ////
////Masks for R6 Response
// 
#define SDIO_R6_GENERAL_UNKNOWN_ERROR     ((uint32_t)0x00002000)
#define SDIO_R6_ILLEGAL_CMD               ((uint32_t)0x00004000)
#define SDIO_R6_COM_CRC_FAILED            ((uint32_t)0x00008000)

#define SDIO_VOLTAGE_WINDOW_SD            ((uint32_t)0x80100000)
#define SDIO_HIGH_CAPACITY                ((uint32_t)0x40000000)
#define SDIO_STD_CAPACITY                 ((uint32_t)0x00000000)
#define SDIO_CHECK_PATTERN                ((uint32_t)0x000001AA)

#define SDIO_MAX_VOLT_TRIAL               ((uint32_t)0x0000FFFF)
#define SDIO_ALLZERO                      ((uint32_t)0x00000000)

#define SDIO_WIDE_BUS_SUPPORT             ((uint32_t)0x00040000)
#define SDIO_SINGLE_BUS_SUPPORT           ((uint32_t)0x00010000)
#define SDIO_CARD_LOCKED                  ((uint32_t)0x02000000)

#define SDIO_DATATIMEOUT                  ((uint32_t)0xFFFFFFFF)
#define SDIO_0TO7BITS                     ((uint32_t)0x000000FF)
#define SDIO_8TO15BITS                    ((uint32_t)0x0000FF00)
#define SDIO_16TO23BITS                   ((uint32_t)0x00FF0000)
#define SDIO_24TO31BITS                   ((uint32_t)0xFF000000)
#define SDIO_MAX_DATA_LENGTH              ((uint32_t)0x01FFFFFF)

#define SDIO_HALFFIFO                     ((uint32_t)0x00000008)
#define SDIO_HALFFIFOBYTES                ((uint32_t)0x00000020)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																																																			 ////
////Command Class Supported
// 
#define SDIO_CCCC_LOCK_UNLOCK             ((uint32_t)0x00000080)
#define SDIO_CCCC_WRITE_PROT              ((uint32_t)0x00000040)
#define SDIO_CCCC_ERASE                   ((uint32_t)0x00000020)




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Debug
//

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Low level Layer
//
void SDIO_DeinitHardware(void);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// SDCard Comunicate
//

SDIO_STATE SDIO_Setup(void);
SDIO_STATE SDIO_Initialize(void);
SDIO_STATE SDEnWideBus(FunctionalState NewState);
SDIO_STATE IsCardProgramming(uint8_t *pstatus);
SDIO_STATE FindSCR(uint16_t rca, uint32_t *pscr);
SDTransferState SDIO_GetStatus(void);
SDCardState SDIO_GetState(void);
uint8_t SDIO_Detect(void);
SDIO_STATE SDIO_PowerON(void);
SDIO_STATE SDIO_PowerOFF(void);
SDIO_STATE SDIO_GetCardInfo(SDIO_CardInfo *cardinfo);
SDIO_STATE SDIO_GetCardStatus(SDIO_CardStatus *cardstatus);
SDIO_STATE SDIO_EnableWideBusOperation(uint32_t WideMode);
SDIO_STATE SDIO_SelectDeselect(uint32_t addr);
SDIO_STATE SDIO_Erase(uint32_t startaddr, uint32_t endaddr);
SDIO_STATE SDIO_SendStatus(uint32_t *pcardstatus);
SDIO_STATE SDIO_SendSDStatus(uint32_t *psdstatus);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Read - Write Operation
//
SDIO_STATE SDIO_ReadBlock(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize);
SDIO_STATE SDIO_ReadMultiBlocks(uint8_t *readbuff, uint32_t ReadAddr, uint32_t BlockSize, uint32_t NumberOfBlocks);
SDIO_STATE SDIO_WriteBlock(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize);
SDIO_STATE SDIO_WriteMultiBlocks(uint8_t *writebuff, uint32_t WriteAddr, uint32_t BlockSize, uint32_t NumberOfBlocks);
SDTransferState SDIO_GetTransferState(void);
SDIO_STATE SDIO_StopTransfer(void);
SDIO_STATE SDIO_WaitReadOperation(void);
SDIO_STATE SDIO_WaitWriteOperation(void);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Check error
//
SDIO_STATE CmdError(void);
SDIO_STATE CmdResp1Error(uint8_t cmd);
SDIO_STATE CmdResp2Error(void);
SDIO_STATE CmdResp3Error(void);
SDIO_STATE CmdResp6Error(uint8_t cmd, uint16_t *prca);
SDIO_STATE CmdResp7Error(void);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Debug
//
extern char *SDIO_STRING_State[];
extern char *SDIO_STRING_CardType[];
extern SDIO_CardInfo 	SDCardInfo;
