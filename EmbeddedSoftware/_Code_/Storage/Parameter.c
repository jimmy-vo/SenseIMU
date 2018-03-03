#include "Parameter.h"
#include "Timing_Manage.h"
#include "EEPROM.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Serial_Link.h"
#include "SENSOR_DATA.h"


EEPROM_Status EEP_State;
FLOAT PARA_STRUCT[PARA_TYPE_MAX];

#define EPPROM_PAGE 0
uint8_t PARAM_EPPROM[200];
uint8_t PARAM_EPPROM_size;

uint8_t PARAM_check  [Page_Size];

uint8_t  PARAM_Individual_Init (PARAM_TYPE type, float *Pnt, uint8_t *BufAd, BYTE_MODE	 	mode)
{
	PARA_STRUCT[type].mode 		= mode;
	PARA_STRUCT[type].BufAdd 	= BufAd;	
	PARA_STRUCT[type].Pointer = Pnt;
	return mode;
}
float empty = PARA_RANGE_MODE_4;

void Initialize_PARAMETER (uint8_t writeInit)
{
	uint8_t * BUFADR = PARAM_EPPROM;	
	
	SerialLink_Console (Event, "Initialize Parameter");
	
	Initialize_EEPROM();
	
	BUFADR += PARAM_Individual_Init(GYRO_BIAS_X, 		&GyroBias.x, BUFADR,PARA_MODE_2);
	BUFADR += PARAM_Individual_Init(GYRO_BIAS_Y, 		&GyroBias.y, BUFADR,PARA_MODE_2);
	BUFADR += PARAM_Individual_Init(GYRO_BIAS_Z, 		&GyroBias.z, BUFADR,PARA_MODE_2);
	
	BUFADR += PARAM_Individual_Init(MAGN_OFFSET_X, 		&MagOffset.x, BUFADR,PARA_MODE_2);
	BUFADR += PARAM_Individual_Init(MAGN_OFFSET_Y, 		&MagOffset.y, BUFADR,PARA_MODE_2);
	BUFADR += PARAM_Individual_Init(MAGN_OFFSET_Z, 		&MagOffset.z, BUFADR,PARA_MODE_2);
		
	BUFADR += PARAM_Individual_Init(MAGN_SCALEGAIN_X, &MagScalerGain.x, BUFADR,PARA_MODE_2);
	BUFADR += PARAM_Individual_Init(MAGN_SCALEGAIN_Y, &MagScalerGain.y, BUFADR,PARA_MODE_2);	 
	BUFADR += PARAM_Individual_Init(MAGN_SCALEGAIN_Z, &MagScalerGain.z, BUFADR,PARA_MODE_2);	 
	
	BUFADR += PARAM_Individual_Init(MAGN_AXIS_X, &MagAxis.x, BUFADR,PARA_MODE_2);
	BUFADR += PARAM_Individual_Init(MAGN_AXIS_Y, &MagAxis.y, BUFADR,PARA_MODE_2);	 
	BUFADR += PARAM_Individual_Init(MAGN_AXIS_Z, &MagAxis.z, BUFADR,PARA_MODE_2);	 
	
	BUFADR += PARAM_Individual_Init(COMP_X, 	&Comp.x, BUFADR,PARA_MODE_1);	 
	BUFADR += PARAM_Individual_Init(COMP_Y, &Comp.y, BUFADR,PARA_MODE_1);	 
	BUFADR += PARAM_Individual_Init(COMP_Z, 	&Comp.z, BUFADR,PARA_MODE_1);		
	
	BUFADR += PARAM_Individual_Init(AHRS_KP, &AHRS_Kp, BUFADR,PARA_MODE_2);		
	BUFADR += PARAM_Individual_Init(AHRS_KI, &AHRS_Ki, BUFADR,PARA_MODE_2);		
	
	BUFADR += PARAM_Individual_Init(KFDCM_Q1, &KFDCM_q1, BUFADR,PARA_MODE_3);	 	
	BUFADR += PARAM_Individual_Init(KFDCM_R1, &KFDCM_r1, BUFADR,PARA_MODE_3);		
	BUFADR += PARAM_Individual_Init(KFDCM_Q2, &KFDCM_q2, BUFADR,PARA_MODE_3);	 	
	BUFADR += PARAM_Individual_Init(KFDCM_R2, &KFDCM_r2, BUFADR,PARA_MODE_3);		
	
	BUFADR += PARAM_Individual_Init(THRS_ACCE, &Thres_Acce, BUFADR,PARA_MODE_2);	 	
	BUFADR += PARAM_Individual_Init(THRS_GYRO, &Thres_Gyro, BUFADR,PARA_MODE_2);		
	
	BUFADR += PARAM_Individual_Init(aKFDCM_Q1, &aKFDCM_q1, BUFADR,PARA_MODE_3);	 	
	BUFADR += PARAM_Individual_Init(aKFDCM_R1, &aKFDCM_r1, BUFADR,PARA_MODE_3);		
	BUFADR += PARAM_Individual_Init(aKFDCM_Q2, &aKFDCM_q2, BUFADR,PARA_MODE_3);	 	
	BUFADR += PARAM_Individual_Init(aKFDCM_R2, &aKFDCM_r2, BUFADR,PARA_MODE_3);				
	
	BUFADR += PARAM_Individual_Init(FREQUENCY, &SenseFreq, BUFADR,PARA_MODE_2);			
		
	
	PARAM_EPPROM_size = (uint8_t) (BUFADR - PARAM_EPPROM);
	
	SerialLink_Console (EventInfo, "%d static bytes created", PARAM_EPPROM_size);
	
	if(writeInit == 1)
	{
		GyroBias.x = 20.119f;
		GyroBias.y = -1.9492;
		GyroBias.z = 17.256;
		MagOffset.x = -37.5f;
		MagOffset.y = 188.5f;
		MagOffset.z = 283.5f;
		MagScalerGain.x = 1.1045f;
		MagScalerGain.y = 1.1207f;
		MagScalerGain.z = 1.1175f;	
		
		Comp.x = Comp.y = Comp.z = 0.98f	;	
		AHRS_Kp = 2; AHRS_Ki = 0.0000f;
		
		
		KFDCM_r1 = 	KFDCM_r2 =  0.001f;
		KFDCM_q1 =	KFDCM_q2 = 0.00000001f;
		
		SenseFreq = 500;		
		
		Thres_Acce = 1.11f;
		Thres_Gyro = 0.1f;
		
		PARAM_EEPROM_WRITE(PARA_TYPE_MAX);	
	}	
	else if(writeInit == 2)
	{
		GyroBias.x = 20.119f;
		GyroBias.y = -1.9492;
		GyroBias.z = 17.256;
		MagOffset.x = -24.5;
		MagOffset.y = 153.0f;
		MagOffset.z = 190.0f;
		MagScalerGain.x = 1.0841f;
		MagScalerGain.y = 1.1005f;
		MagScalerGain.z = 1.4f;	
		
		Comp.x = Comp.y = Comp.z = 0.98f	;	
		AHRS_Kp = 2; AHRS_Ki = 0.0000f;
		
		
		KFDCM_r1 = 	KFDCM_r2 =  0.001f;
		KFDCM_q1 =	KFDCM_q2 = 0.00000001f;
		
		SenseFreq = 500;		
		
		Thres_Acce = 1.11f;
		Thres_Gyro = 0.1f;
		
		PARAM_EEPROM_WRITE(PARA_TYPE_MAX);	
	}
	
	SerialLink_Console(EventInfo,	"Read EPPROM");		
	if (PARAM_EEPROM_READ()!= PARAM_OK)							
	{
		SerialLink_Console(EventInfo, "Fail to read parameters");
	}
}
PARAMETER PARAM_VariableToSerial(PARAM_TYPE type)
{//prepare string to send to serial
	int16_t Write = 0;
	if (PARAM_Write(type, PARA_STRUCT[type].BufAdd) < 0)		
	{
		SerialLink_Console (Highlevel, "PARAM: [%s] Overfloat", String_PARAMETER_Type[abs(Write)]);
		return PARAM_OverFloat;
	}
	else 	SerialLink_SetParameter(type, PARA_STRUCT[type].BufAdd);		
		return PARAM_OK;
}
int16_t PARAM_Write (PARAM_TYPE type, uint8_t *Addr)
{//convert float value in pointer address to buffer // 
	float 		PARA_Limit = 0;
	int8_t 		PARA_exp = 0;
	int32_t 	PARA_sig = 0;
	float 		value;
		
	//Get Float value from pointer
	value = *(PARA_STRUCT[type].Pointer);
	
	//Convert float to struct
	if			(PARA_STRUCT[type].mode == PARA_MODE_1) PARA_Limit = PARA_RANGE_MODE_1;
	else if (PARA_STRUCT[type].mode == PARA_MODE_2) PARA_Limit = PARA_RANGE_MODE_2;
	else if (PARA_STRUCT[type].mode == PARA_MODE_3) PARA_Limit = PARA_RANGE_MODE_3;
	else if (PARA_STRUCT[type].mode == PARA_MODE_4) PARA_Limit = PARA_RANGE_MODE_4;
	
	if (abs(value)> PARA_Limit) 
	{
		return -(int16_t)type;;
	}

	if (value == 0)
	{
		PARA_exp	 				= 0;
		PARA_sig				 	= 0;
	}
	else if ((value - (int32_t) value) != 0)
	{
		while (abs(PARA_sig) <= PARA_Limit)
		{
			if (PARA_exp < PARA_RANGE_EXP)
			{
				PARA_exp --;
				value *= 10;
				PARA_sig = (int32_t)(value / 1);
			}
			else // nearly 0;
			{				
				PARA_exp	= 0;
				PARA_sig 	= 0;
			}
		}
		while (abs(PARA_sig) > PARA_Limit) 
		{
			if (PARA_exp<0)
			{
				PARA_exp++;
				PARA_sig /= 10;
			}
		}
	}
	else 
	{
		PARA_exp	 				= 0;
		PARA_sig 	= (int32_t)value;
	}	
	
	*(Addr++) = 		PARA_STRUCT[type].mode;	
	*(Addr++) = 		type;	
	
	//Convert struct to buffer 	
	switch (PARA_STRUCT[type].mode)
	{
		case PARA_MODE_0 : 
											break; 
		case PARA_MODE_1 : 
											*(Addr++)	= (uint8_t) (PARA_sig  &  0x000000FF); 
											break; 
		case PARA_MODE_2 : 
											*(Addr++)	= (uint8_t) ((PARA_sig &  0x0000FF00) >> 8);
											*(Addr++)	= (uint8_t) (PARA_sig  &  0x000000FF); 
											break; 
		case PARA_MODE_3 : 
											*(Addr++)	= (uint8_t) ((PARA_sig &  0x00FF0000) >> 16);
											*(Addr++)	= (uint8_t) ((PARA_sig &  0x0000FF00) >> 8);
											*(Addr++)	= (uint8_t) ( PARA_sig &  0x000000FF); 
											break; 
		case PARA_MODE_4 : 
											*(Addr++)	= (uint8_t) ((PARA_sig &  0xFF000000) >> 31 >>1);
											*(Addr++)	= (uint8_t) ((PARA_sig &  0x00FF0000) >> 16);
											*(Addr++)	= (uint8_t) ((PARA_sig &  0x0000FF00) >> 8);
											*(Addr++)	= (uint8_t) ( PARA_sig &  0x000000FF); 
											break; 
	}
	*(Addr++) = (uint8_t) ( abs(PARA_exp) & 0x00FF); 
	
	SerialLink_SetParameter(type, Addr);	
//	SerialLink_Console (Highlevel, "PARAM: WRITE [%s]", String_PARAMETER_Type[type]);
	
	return (int16_t)type;
}


int16_t PARAM_Read (uint8_t 	*Addr)
{// convert from buffer to float, put result in pointer address	
	int32_t Tmp_Si = 0x00;		
	PARAM_TYPE PARA_Type ;
	BYTE_MODE	PARA_Mode ;
	int32_t PARA_Sig = 0;
	int8_t PARA_exp = 0;
	float 	PARA_result = 0;
	uint8_t *PARAM_Serial_ADDR = Addr;
		
	PARA_Mode = (BYTE_MODE)(*Addr++);
	PARA_Type = (PARAM_TYPE)(*Addr++);
		
	if (PARA_Mode != PARA_STRUCT[PARA_Type].mode)
		return -(int16_t)PARA_Type;
		
	//Convert buffer value to struct	
	switch (PARA_STRUCT[PARA_Type].mode)
	{
		case PARA_MODE_0 : 
											break; 
		case PARA_MODE_1 : 
											Tmp_Si |= 		*(Addr++)						& 0x000000FF; 
											if ((Tmp_Si & 0x00000080) == 0x00000080)
												Tmp_Si  |= 0xFFFFFF00;
											break; 
		case PARA_MODE_2 : 
											Tmp_Si |= 		(*(Addr++) << 8)		& 0x0000FF00; 
											Tmp_Si |= 		*(Addr++)						& 0x000000FF;  
											if ((Tmp_Si & 0x00008000) == 0x00008000)
												Tmp_Si  |= 0xFFFF0000;
											break; 
		case PARA_MODE_3 : 
											Tmp_Si |= 		(*(Addr++) << 16)		& 0x00FF0000; 
											Tmp_Si |= 		(*(Addr++) << 8)		& 0x0000FF00; 
											Tmp_Si |= 		*(Addr++)						& 0x000000FF; 
											if ((Tmp_Si & 0x00800000) == 0x00800000)
												Tmp_Si  |= 0xFF000000;
											break; 
		case PARA_MODE_4 : 
											Tmp_Si |= 		(*(Addr++) << 31 <1)& 0xFF000000; 
											Tmp_Si |= 		(*(Addr++) << 16)		& 0x00FF0000; 
											Tmp_Si |= 		(*(Addr++) << 8) 		& 0x0000FF00; 
											Tmp_Si |= 		*(Addr++)						& 0x000000FF; 
											break; 
	}
	
	PARA_Sig 	= Tmp_Si;
	PARA_exp	= *(Addr);
	
	//Convert struct to float
	PARA_result = PARA_Sig;
	
	while (PARA_exp--!=0)
		PARA_result /= 10;
		
	//Save float value to pointer address
	*(PARA_STRUCT[PARA_Type].Pointer) = PARA_result;
			
	SerialLink_SetParameter(PARA_Type, PARAM_Serial_ADDR);	 
	
//	SerialLink_Console (Highlevel, "PARAM: READ [%s]", String_PARAMETER_Type[PARA_Type]);
	return (int16_t)PARA_Type;
}

PARAMETER PARAM_EEPROM_WRITE (PARAM_TYPE Paratype)
{// parameter variable -> array -> EPPROM
	PARAM_TYPE type = PARA_TYPE_NONE;	
	uint8_t * addr = PARAM_EPPROM;
	uint16_t page = 0;
	int16_t Write = 0;
		
	//Write new parameter on buffer	
	switch (Paratype)
	{ 
		case PARA_TYPE_MAX:////////////////////////////////////////////////////////////////////////	
												for (type = PARA_TYPE_MIN; type<PARA_TYPE_MAX; type++)
													if ((Write = PARAM_Write(type, PARA_STRUCT[type].BufAdd)) < 0)   type = PARA_TYPE_MAX;
												break;			
		default :////////////////////////////////////////////////////////////////////////					
												Write = PARAM_Write(type, PARA_STRUCT[type].BufAdd);
												break;
}
	
	if (Write < 0)		
	{
		SerialLink_Console (Highlevel, "PARAM: [%s] Overfloat", String_PARAMETER_Type[abs(Write)]);
		return PARAM_OverFloat;
	}
	else
		{	
			while (addr - PARAM_EPPROM < PARAM_EPPROM_size)
			{	
				if (EEPROM_WritePage(addr, EPPROM_PAGE + page++) != EEPROM_Status_OK)
					return PARAM_EEPROM_WRITE_FAIL;
					addr	+=  Page_Size;
			}
			SerialLink_Console (EventInfo, "Write %d pages EPPROM", page);				
			return PARAM_OK;		
		}
}

PARAMETER PARAM_EEPROM_READ (void)
{// EPPROM -> array -> parameter variable  
	uint8_t * addr = PARAM_EPPROM;
	uint16_t page = 0;	
	uint16_t type;
	int16_t Read = 0;
	
	while (addr - PARAM_EPPROM < PARAM_EPPROM_size)
	{
		if (EEPROM_ReadPage(addr, EPPROM_PAGE + page++) != EEPROM_Status_OK)
			return PARAM_EEPROM_READ_FAIL;	
		addr	+=  Page_Size;
	}
		
	for  (type = (uint16_t)PARA_TYPE_MIN; type < (uint16_t)PARA_TYPE_MAX; type++)
	{
		if ((Read = PARAM_Read(PARA_STRUCT[type].BufAdd)) < 0)		
		{			
			SerialLink_Console (Highlevel, "[%s] Not matched (%d)", String_PARAMETER_Type[abs(Read)], Read);
			return PARAM_OverFloat;
		}
	}
	
	SerialLink_Console (EventInfo, "Parameter loaded  ", page);		
	return PARAM_OK;		
		
}


