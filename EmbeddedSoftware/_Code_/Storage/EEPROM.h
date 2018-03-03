#include "BoardConfig.h"

#define LC24256_READ_ADDRESS					0xA1
#define LC24256_WRITE_ADDRESS					0xA0

#define Page_Size 										64  		//byte
#define LC24256_SIZE 									32000		//byte
#define Total_Page										LC24256_SIZE/Page_Size


#define MAXStandby_trial 							10
typedef enum 
{
		EEPROM_Status_OK										= 0,
    EEPROM_Status_BUSY 			 						= 1,
    EEPROM_STATUS_REQ_FAIL							= 2,
    EEPROM_Status_FAIL 			 						= 3,
}EEPROM_Status;

EEPROM_Status Initialize_EEPROM (void);

EEPROM_Status EEPROM_ReadPage (uint8_t *buffer, uint16_t page);
EEPROM_Status EEPROM_WritePage (uint8_t *buffer, uint16_t page);
EEPROM_Status EEPROM_UpdatePage (uint8_t *buffer, uint16_t page);
