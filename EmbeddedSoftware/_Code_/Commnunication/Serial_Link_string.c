#include "Serial_Link_string.h"


char *String_I2C_Status[] = 
{	
		"I2C OK",
		"I2C Busy",	
		"I2C Timeout"	
};

char *String_HMC5883_Status[] = 
{	
		"HMC5883: OK",
		"HMC5883: Busy",	
		"HMC5883: Request Fail",	
		"HMC5883: Timeout"	
};

char *String_MPU6050_Status[] = 
{	
		"MPU6050: OK",
		"MPU6050: Busy",	
		"MPU6050: Request Fail",	
		"MPU6050: Timeout"	
};

char *String_EPPROM_Status[] = 
{
		"EPPROM: OK",
		"EPPROM: Busy",	
		"EPPROM: Request Fail",	
		"EPPROM: Timeout"	
};

char *String_PARAMETER_Status[] = 
{//PARAMETER				
	"PARAM: OK"    						,
	"PARAM: OverFloat"					,
	"PARAM: ModeConflict"			,
	"PARAM: EEPROM |-> Write Fail" ,
	"PARAM: EEPROM |-> Read Fail" ,
};


char *String_Algorithm[] = 
{//EKFQUA_Status			
	" "			,
	" "			,
	" "			,
	" "			,
	"Algorithm: Tilt Compensation"			,
	"Algorithm: Gyro-intergration"			,
	"Algorithm: Complementary"    			,
	"Algorithm: AHRS"										,
	"Algorithm: DCM-based Kalman filter",
	"Algorithm: Adaptive DCM-based Kalman filter",
};


char *String_ALGORITHM_Status[] = 
{//ALGORITHM_Status			
	"Algorithm OK"    														,
	"Fail to initialize Sensor"				,
	"Fail to initialize algorithm"			,
	"Fail to read from sensor"					,
	"Fail dt detected"					,
	"Fail"										
};


char *String_CALIB_Status[] = 
{//CALIB_Status
	"Calibration Ok",
	"Calibration Fail"  ,
	///////////////////////////////////////		
	"I2C bus Init Fail",
	"Fail to read HMC5883",
	"Fail to read MUP6050",
	"Fail to write to EPPROM",
	///////////////////////////////////////		
	"Canceled Gyrometer bias estimation",
	///////////////////////////////////////		
	"Fail to configure Magnetometer offset",
	"Canceled Magnetometer offset estimation",
	///////////////////////////////////////		
	"Fail to configure Positive bias"  ,
	"Fail to read Positive bias"  ,
	"Positive bias read timeout"  ,
	"Fail to configure Negative bias"  ,
	"Fail to read Negative bias"  ,
	"Negative bias read timeout"  ,
	///////////////////////////////////////		
};

char *String_SENSOR_Status[] = 
{//SENSOR_Status	
	"SENSOR: OK"													,
	///////////////////////////////////////
	"SENSOR: Setup |-> HMC5883 Fail"					,
	"SENSOR: Setup |-> MPU6050 Fail"					,
	///////////////////////////////////////			 					,
	"SENSOR: READ |-> HMC5883 Fail"		 					,
	"SENSOR: READ |-> MPU6050 Fail"							,		
};

char *String_PARAMETER_Type[] = 
{//PARAM_TYPE
	"TYPE 0",	
	
	"Gyro |-> BiasX", 
	"Gyro |-> BiasY", 
	"Gyro |-> BiasZ", 
	
	"Magn |-> OffsetX", 
	"Magn |-> OffsetY", 
	"Magn |-> OffsetZ", 
	
	"Magn |-> GainX", 
	"Magn |-> GainY", 
	"Magn |-> GainZ", 
	
	"COMP |-> Roll",
	"COMP |-> Pitch",
	"COMP |-> Yaw",
	
	"AHRS |-> Kp",
	"AHRS |-> Ki",	
	
	"DCM |-> R1", 
	"DCM |-> Q1", 
	"DCM |-> R2", 	
	"DCM |-> Q2",	
	
	"Frequency",
	
};

char * String_FATFS_Status[]=
{//FRESULT
	"FATFS: Succeeded",								
	"FATFS: A hard error occured in the low level disk I/O layer",			
	"FATFS: Assertion failed",					
	"FATFS: The physical drive cannot work",					
	"FATFS: Could not find the file",					
	"FATFS: Could not find the path ",						
	"FATFS: The path name format is invalid",		
	"FATFS: Acces denied due to prohibited access or directory full",
	"FATFS: Acces denied due to prohibited access",								
	"FATFS: The file/directory object is invalid",	
	"FATFS: The physical drive is write protected",	
	"FATFS: The logical drive number is invalid",				
	"FATFS: The volume has no work area ",				
	"FATFS: There is no valid FAT volume ",
	"FATFS: The f_mkfs() aborted due to any parameter error ",
	"FATFS: Could not get a grant to access the volume within defined period",
	"FATFS: The operation is rejected according to the file shareing policy",
	"FATFS: LFN working buffer could not be allocated",
	"FATFS: Number of open files > _FS_SHARE",
	"FATFS: Given parameter is invalid"
};
