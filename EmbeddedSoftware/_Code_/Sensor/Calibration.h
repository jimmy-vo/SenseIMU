#include <math.h>
#include "BoardConfig.h"

typedef enum 
{//CALIB_Status
		CALIB_OK														,
    CALIB_FAIL 			 										,
		///////////////////////////////////////
    CALIB_SETUP_I2C_Fail								,
    CALIB_READ_HMC5883_Fail		 					,
    CALIB_READ_MPU6050_Fail							,		
    CALIB_WriteEPPROM_Fail							,		
		///////////////////////////////////////	
    CALIB_GyroBias_Cancel     			 		,
		///////////////////////////////////////	
    CALIB_MagCalibOffset_ConfigFail     ,
    CALIB_MagCalibOffset_Cancel     		,
		///////////////////////////////////////			
    CALIB_MagCalibGain_PosBias_ConfigFail ,
    CALIB_MagCalibGain_PosBias_ReadFail ,
    CALIB_MagCalibGain_PosBias_Timeout	,		
    CALIB_MagCalibGain_NegBias_ConfigFail ,		
    CALIB_MagCalibGain_NegBias_Timeout	,
    CALIB_MagCalibGain_NegBias_ReadFail ,
		///////////////////////////////////////					
}CALIB_Status;


CALIB_Status CALIB_Calibrate (uint32_t gyro_second, uint8_t gyroButton, uint8_t magButton);
