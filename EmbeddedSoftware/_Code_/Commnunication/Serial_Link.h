#include "BoardConfig.h"
#include "Serial_Link_string.h"

typedef enum
{
	None,
	Event,
	EventInfo,
	EventInfoDetail,
	DetailList,	
	
	Highlevel,	
	Lowlevel,
}SERIALLINK_LEVEL;

typedef enum
{
	SERIALLINK_INPUT_MODE_None,
	SERIALLINK_INPUT_MODE_CMD,
	SERIALLINK_INPUT_MODE_Parameter,	
}SERIALLINK_INPUT_MODE;

typedef enum
{
	INPUT_CMD_None 						= 0,
	INPUT_CMD_ParamBoardtoPC 	= 1,
	INPUT_CMD_ParamSaveEPPROM	= 2,
	INPUT_CMD_Calib						= 3,
	INPUT_CMD_Algo_TiltComp 	= 4,
	INPUT_CMD_Algo_GyroInteg 	= 5,
	INPUT_CMD_Algo_Comple 		= 6,
	INPUT_CMD_Algo_AHRS 			= 7,
	INPUT_CMD_Algo_DCMkf 			= 8,
	INPUT_CMD_Algo_DCMakf     = 9,
}SERIALLINK_INPUT_CMD;



typedef struct 
{
	uint8_t		FrameCnt;							/// 2byte
	
	int16_t		Gyrox;							/// 2byte 
	int16_t		Gyroy;							/// 2byte 
	int16_t		Gyroz;							/// 2byte 
	
	int16_t		Magnx;							/// 2byte 
	int16_t		Magny;							/// 2byte 
	int16_t		Magnz;							/// 2byte 	
	
	int16_t		Accelx;							/// 2byte 
	int16_t		Accely;							/// 2byte 
	int16_t		Accelz;							/// 2byte 
	
	int16_t		Eulerx;								/// 2byte 
	int16_t		Eulery;							/// 2byte 
	int16_t		Eulerz;								/// 2byte
	
	int32_t		Qua1;								/// 3byte 
	int32_t		Qua2;								/// 3byte 
	int32_t		Qua3;								/// 3byte
	int32_t		Qua4;								/// 3byte 
		
	int32_t		dcm0;								/// 3byte 
	int32_t		dcm1;								/// 3byte 
	int32_t		dcm2;								/// 3byte	
	int32_t		dcm3;								/// 3byte 
	int32_t		dcm4;								/// 3byte 
	int32_t		dcm5;								/// 3byte
	int32_t		dcm6;								/// 3byte 
	int32_t		dcm7;								/// 3byte 
	int32_t		dcm8;								/// 3byte
	
	uint16_t	SenseDT;								/// 4byte
	uint16_t	SenseBat;								/// 2byte
	uint16_t	SenseTemp;								/// 2byte
	uint8_t		Button;							/// 1byte 
	
	int32_t		Velx;								/// 3byte 
	int32_t		Vely;								/// 3byte 
	int32_t		Velz;								/// 3byte
	
	int32_t		Posx;								/// 3byte 
	int32_t		Posy;								/// 3byte 
	int32_t		Posz;								/// 3byte
}DATA_STREAM;

typedef enum 
{
	SERIALLINK_MODE_Debug,
	SERIALLINK_MODE_Parameter,
	SERIALLINK_MODE_Stream
}SERIALLINK_MODE;

extern DATA_STREAM DATA_OUT;

char *byte_to_binary(int x, uint8_t size);
void SerialLink_StartStream(void);
void Initialize_SerialLink (void);
void SerialLink_INPUT_INTERRUPT (uint16_t INPUT);

void SerialLink_DMATX_Process (void);
void SerialLink_INPUT_Process (void);

void SerialLink_SetParameter (uint16_t stack, uint8_t* addr );
void SerialLink_SendParameter (void);

void SerialLink_Console(SERIALLINK_LEVEL type, const char *fmt, ...) ;
SERIALLINK_INPUT_CMD SerialLink_GetCMD(void);
void SerialLink_ResetCMD(void);
