#include "SDCARD.h"
#include "Serial_Link.h"
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "ff.h"
#include "Timing_Manage.h"
#include "SENSOR_DATA.h"


FATFS 		fatfs;
FIL 			SDLOGFILE_Sensor, SDLOGFILE_Param;
FRESULT 	SDLOG_State ;
int 			StreamState = 1;


int SDLOG_OpenFile(void)
{	
	SerialLink_Console(Event, "Initialize SD CARD");	
	if ((SDLOG_State = f_mount(0, &fatfs)) != FR_OK)
	{
		SerialLink_Console(EventInfo, "Fail to mount SD Card - %s", String_FATFS_Status[SDLOG_State]);
		return SDLOG_State;
	}
	else
		{		
			SerialLink_Console(EventInfo, "SD Card mounted sucessfully");
		}
		
	if ((SDLOG_State = f_open(&SDLOGFILE_Sensor, "Sensor.txt", FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK)
	{
		SerialLink_Console(EventInfo, "Fail to Create Sensor.txt - %s", String_FATFS_Status[SDLOG_State]);
		return SDLOG_State;
	}
	else
		{		
			SerialLink_Console(EventInfo, "Sensor.txt is created");
		}
	
	return SDLOG_State;
}

int SDLOG_CloseFile(void)
{	
	SerialLink_Console(Event, "Deinitialize SD CARD");	
	if ((SDLOG_State = f_close(&SDLOGFILE_Sensor)) != FR_OK)
	{
		SerialLink_Console(EventInfo, "Fail to close Sensor.txt - %s", String_FATFS_Status[SDLOG_State]);
		return SDLOG_State;
	}
	else
		{		
			SerialLink_Console(EventInfo, "Sensor.txt is closed");
		}
				
	if ((SDLOG_State = f_mount(0, 0)) != FR_OK)
	{
		SerialLink_Console(EventInfo, "Fail to unmount SD Card - %s", String_FATFS_Status[SDLOG_State]);
		return SDLOG_State;
	}
	else
		{		
			SerialLink_Console(EventInfo, "SD Card unmounted sucessfully");
		}		
	return SDLOG_State;
}

int SDLOG_UpdateParam(void)
{		
	if ((SDLOG_State = f_open(&SDLOGFILE_Param, "Param.txt", FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK)
	{
		SerialLink_Console(EventInfo, "Fail to Create Param.txt - %s", String_FATFS_Status[SDLOG_State]);
		return SDLOG_State;
	}
	else
		{		
			SerialLink_Console(EventInfo, "Param.txt is created");
			
			SerialLink_Console(EventInfo, "Write to Param.txt");
									
			f_printf(&SDLOGFILE_Param, "Magnetic offset value:\nOx = %d.%d   Oy = %d.%d   Oz = %d.%d\n\n",    
								(int)MagOffset.x, (int)abs(MagOffset.x*10)%10,
								(int)MagOffset.y, (int)abs(MagOffset.y*10)%10,
								(int)MagOffset.z, (int)abs(MagOffset.z*10)%10);
								
			#define afpt 		1000
			f_printf(&SDLOGFILE_Param, "Magnetic Error Gain:\nGx = %d.%03d   Gy = %d.%03d   Gz = %d.%03d\n\n",  
								(int)MagScalerGain.x, (int)abs(MagScalerGain.x*afpt)%afpt,
								(int)MagScalerGain.y, (int)abs(MagScalerGain.y*afpt)%afpt,
								(int)MagScalerGain.z, (int)abs(MagScalerGain.z*afpt)%afpt);
		}
								if ((SDLOG_State = f_close(&SDLOGFILE_Param)) != FR_OK)
	{
		SerialLink_Console(EventInfo, "Fail to close Param.txt - %s", String_FATFS_Status[SDLOG_State]);
		return SDLOG_State;
	}
	else
		{		
			SerialLink_Console(EventInfo, "Param.txt is closed");
		}
	return SDLOG_State;
}


void SDLOG_UpdateSensor(int t, int gx, int gy, int gz, int ax, int ay, int az, int mx, int my, int mz )
{
	if (StreamState != -1)
	{
		StreamState = f_printf(&SDLOGFILE_Sensor,"%6d\t%5d\t%5d\t%5d\t%5d\t%5d\t%5d\t%5d\t%5d\t%5d\n", t, gx, gy, gz, ax, ay, az, mx, my, mz );
		f_sync(&SDLOGFILE_Sensor);
	}
}
