#include "BoardConfig.h"

typedef enum
{
	PARAM_OK    						 ,
	PARAM_OverFloat					 ,
	PARAM_ModeConflict			 ,
	PARAM_EEPROM_WRITE_FAIL  ,
	PARAM_EEPROM_READ_FAIL 	 ,
}PARAMETER;

typedef enum
{	
	GYRO_BIAS_X    				= 1, 
	GYRO_BIAS_Y    				= 2, 
	GYRO_BIAS_Z    				= 3,  
	
	MAGN_OFFSET_X    			= 4, 
	MAGN_OFFSET_Y    			= 5, 
	MAGN_OFFSET_Z    			= 6,  
	
	MAGN_SCALEGAIN_X    	= 7, 
	MAGN_SCALEGAIN_Y    	= 8, 
	MAGN_SCALEGAIN_Z    	= 9, 
	
	MAGN_AXIS_X    				= 10, 
	MAGN_AXIS_Y    				= 11, 
	MAGN_AXIS_Z    				= 12, 
		
	COMP_X  							= 13, 
	COMP_Y     						= 14, 
	COMP_Z     						= 15, 

	AHRS_KP    						= 16, 
	AHRS_KI    						= 17, 

	KFDCM_R1	    				= 18, 		
	KFDCM_Q1							= 19, 
	KFDCM_R2	    				= 20, 		
	KFDCM_Q2							= 21, 
	
	THRS_ACCE 						= 22,
	THRS_GYRO 						= 23,
		
	aKFDCM_R1	    				= 24, 		
	aKFDCM_Q1							= 25, 
	aKFDCM_R2	    				= 26, 		
	aKFDCM_Q2							= 27, 

	FREQUENCY 						= 28, 
	
	
	PARA_TYPE_MIN   			= 1, 
	PARA_TYPE_MAX    			= 29,  //FREQUENCY+1
	PARA_TYPE_NONE  			= 30, 	
}PARAM_TYPE;

	#define AddrOffset 3
typedef enum
{
	PARA_MODE_0 =	0,		//  significand bytes 
	PARA_MODE_1 =	1 + AddrOffset,		//1 byte							//+-127
	PARA_MODE_2 =	2 + AddrOffset,		//2 bytes							//+=32767
	PARA_MODE_3 =	3 + AddrOffset,		//3 bytes							//+-8388607
	PARA_MODE_4 =	4 + AddrOffset		//4 bytes							//+-214748364
}BYTE_MODE;

#define PARA_RANGE_MODE_1				127
#define PARA_RANGE_MODE_2				32767
#define PARA_RANGE_MODE_3				8388607
#define PARA_RANGE_MODE_4				2147483647

#define PARA_RANGE_EXP					127
typedef struct 
{
	float	 			*Pointer;
	BYTE_MODE	 	mode;
	uint8_t     *BufAdd;
}FLOAT;

uint8_t PARAM_Buff_READ (PARAM_TYPE type, uint8_t pos);
extern int16_t PARAM_Read (uint8_t 	*Addr);
extern int16_t PARAM_Write (PARAM_TYPE type, uint8_t *Addr);
PARAMETER PARAM_EEPROM_WRITE (PARAM_TYPE Paratype);
PARAMETER PARAM_EEPROM_READ (void);
void Initialize_PARAMETER (uint8_t writeInit);
PARAMETER PARAM_VariableToSerial(PARAM_TYPE type);
