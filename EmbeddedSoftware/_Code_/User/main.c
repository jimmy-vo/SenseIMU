#include "BoardConfig.h"
#include "INCLUDE.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "misc.h"

int main(void)
{
	Initialize_SerialLink();	
	
	Initialize_Timer();	
	
	Initialize_PARAMETER(1);	
	
	Button_Initialize();
	
	RGB_Initialize();	
	
	Voltage_FB_Initiallize();	
	
	while (1)
		{
			switch (SerialLink_GetCMD())
			{
				case INPUT_CMD_ParamBoardtoPC: 
																				PARAM_EEPROM_READ();	
																				SerialLink_SendParameter();				
																				SerialLink_ResetCMD();																					
																				break;
				case INPUT_CMD_ParamSaveEPPROM: 
																				PARAM_EEPROM_WRITE(PARA_TYPE_MAX);		
																				SerialLink_ResetCMD();
																				break;	
				case INPUT_CMD_Calib: 
																				CALIB_Calibrate(5*1000*60,0,1);	
																				SerialLink_ResetCMD();
																				Algorithm_Reset();
																				break;	
				case INPUT_CMD_None: 
																				Algorithm_Orientiation();
																				break;	
				default: 											
																				Algorithm_Switch((ALGORITHM)SerialLink_GetCMD());
																				SerialLink_ResetCMD();
																				break;	
			}
		}
}

void HardFault_Handler(void)
{	
  while (1);
}
