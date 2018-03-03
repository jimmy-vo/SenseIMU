#include "Button.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "RGB.h"
#include "Timing_Manage.h"
#include "Calibration.h"
#include "SENSOR_DATA.h"
#include "Serial_Link.h"
#include "Parameter.h"
#include "Dev_I2C.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ff.h"
#include "Voltage_FB.h"

static int16_3D Magn_PosBias, Magn_NegBias, Magn_max, Magn_min;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////Calibrate function
//

static CALIB_Status Estimate_GyroBias(uint32_t gyro_second)
{	
	static int16_t gx, gy, gz;
	static int16_t ax, ay, az;
	float_3D bias, temp;
	Quaternion q;
	DCM dcm;
		
	uint64_t SENSOR_Lastime = Get_msTick();
	uint64_t SENSOR_LastSample = Get_msTick();
	
	SerialLink_Console(EventInfo, "Data during the first 5 seconds will be ignored.");	
	SerialLink_Console(EventInfo, "When the LED turns Blue, the process begin.");
	SerialLink_Console(EventInfo, "Press [button 0] when you are done");		
	SerialLink_Console(EventInfo, "Press [button 1] to cancel.");	
	SerialLink_Console(EventInfo, "Lay the sensor on flat surface and don't touch it.");	
	// Disregarding the first few data  
	SENSOR_Lastime = Get_msTick();
	while((Get_msTick() - SENSOR_Lastime <3000)&&(Button_Check(0)== IsReleased)) 	
	{
		if (Button_Check(1)== IsPressed) 																												return CALIB_GyroBias_Cancel;
		if (Get_msTick()- SENSOR_LastSample >= 10)
		{			
			if (MPU6050_Get_rawMotion (&gx, &gy, &gz, &ax, &ay, &az) != MPU6050_STATUS_OK)				return CALIB_READ_MPU6050_Fail;
			
			SENSOR_DataAlign(&temp, &bias, &temp, &temp, &q, &dcm, &temp.x,  &temp.x, &temp, &temp);
		}
	}
	
	SerialLink_Console(EventInfo, "The process start.", gyro_second);	
	RGB_Set(RGB_BLUE_T);
	SerialLink_Console(EventInfo, "The process will take %d minutes.", gyro_second/60);	
	// colect data  
	SENSOR_Lastime = Get_msTick();
	while((Get_msTick() - SENSOR_Lastime <gyro_second)&&(Button_Check(0)== IsReleased)) 	
	{		
		if (Button_Check(1)== IsPressed) 																												return CALIB_GyroBias_Cancel;
				
		if (Get_msTick()- SENSOR_LastSample >= 10)
		{			
			SENSOR_LastSample = Get_msTick();
			if (MPU6050_Get_rawMotion (&gx, &gy, &gz, &ax, &ay, &az) != MPU6050_STATUS_OK)				return CALIB_READ_MPU6050_Fail;
			bias.x += gx;
			bias.y += gy;
			bias.z += gz;
			
			SENSOR_DataAlign(&temp, &bias, &temp, &temp, &q, &dcm, &temp.x, &temp.x, &temp, &temp);
		}
	}
		
	// compute scale offset
	GyroBias.x = (bias.x/gyro_second);
	GyroBias.y = (bias.y/gyro_second);
	GyroBias.z = (bias.z/gyro_second);	
				
	SerialLink_Console(None,  "=> Gyro Bias: %3.1f  %3.1f  %3.1f", 	GyroBias.x, GyroBias.y, GyroBias.z );			
		
	SerialLink_Console(EventInfo, "Done\n");	
	
	if (PARAM_VariableToSerial(GYRO_BIAS_X)!= PARAM_OK) 	return CALIB_WriteEPPROM_Fail;	
	if (PARAM_VariableToSerial(GYRO_BIAS_Y)!= PARAM_OK) 	return CALIB_WriteEPPROM_Fail;	
	if (PARAM_VariableToSerial(GYRO_BIAS_Z)!= PARAM_OK) 	return CALIB_WriteEPPROM_Fail;		
	SerialLink_SendParameter();
	return CALIB_OK;
}

static CALIB_Status Estimate_MagnOffset()
{	
	int16_3D Tmp_Mag;
	float_3D	temp, mag;
	Quaternion q;
	DCM dcm;
	uint64_t SENSOR_Lastime;
	
	SerialLink_Console(EventInfo, "When the LED turns GREEN, the process begin.");	
	SerialLink_Console(EventInfo, "Press [button 0] when you are done.");		
	SerialLink_Console(EventInfo, "Press [button 1] to cancel.");		
	
	if(HMC5883_Configuration_A(EKFQUA_HMC5883_AVERAGE, EKFQUA_HMC5883_RATE, EKFQUA_HMC5883_BIAS)!= I2C_STATUS_OK)			return	CALIB_MagCalibOffset_ConfigFail;					
	if(HMC5883_Configuration_B(EKFQUA_HMC5883_GAIN)!= I2C_STATUS_OK)																									return	CALIB_MagCalibOffset_ConfigFail;	
	if(HMC5883_Configuration_MODE(EKFQUA_HMC5883_HIGHSPEED, EKFQUA_HMC5883_MODE)!= I2C_STATUS_OK)											return	CALIB_MagCalibOffset_ConfigFail;		
	
	SENSOR_Lastime = Get_msTick();
	// Disregarding the first few data  	
	while(Get_msTick() - SENSOR_Lastime <1000)
	{
		if(HMC5883_Get_RawMagn(&Tmp_Mag.x, &Tmp_Mag.y, &Tmp_Mag.z)!= I2C_STATUS_OK)																			return 	CALIB_READ_HMC5883_Fail; 
		SENSOR_DataAlign(&temp, &temp, &mag, &temp, &q, &dcm, &temp.x,  &temp.x, &temp, &temp);
	}
	
	SerialLink_Console(EventInfo, "Move the sensor in any rotation in few minutes...");	
	// colect data  
	RGB_Set(RGB_GREEN_T);	
	while(Button_Check(0) == IsReleased) 
	{
		if(Button_Check(1) == IsPressed)																																								return 	CALIB_MagCalibOffset_Cancel; 			
		if(HMC5883_Get_RawMagn(&Tmp_Mag.x, &Tmp_Mag.y, &Tmp_Mag.z)!= I2C_STATUS_OK)																			return 	CALIB_READ_HMC5883_Fail; 					
			else
				{									
					Magn_max.x = max(Magn_max.x	,	Tmp_Mag.x);
					Magn_max.y = max(Magn_max.y	,	Tmp_Mag.y);
					Magn_max.z = max(Magn_max.z	,	Tmp_Mag.z);
						
					Magn_min.x = min(Magn_min.x	,	Tmp_Mag.x);
					Magn_min.y = min(Magn_min.y	,	Tmp_Mag.y);
					Magn_min.z = min(Magn_min.z	,	Tmp_Mag.z);
					
					mag.x = (float)Tmp_Mag.x;
					mag.y = (float)Tmp_Mag.y;
					mag.z = (float)Tmp_Mag.z;
					SENSOR_DataAlign(&temp, &temp, &mag, &temp, &q, &dcm, &temp.x, &temp.x, &temp, &temp);
				}
	}		
		
	// compute scale offset
	MagOffset.x = (((float)(Magn_max.x - Magn_min.x)/2)- Magn_max.x);
	MagOffset.y = (((float)(Magn_max.y - Magn_min.y)/2)- Magn_max.y);
	MagOffset.z = (((float)(Magn_max.z - Magn_min.z)/2)- Magn_max.z);
	
	SerialLink_Console(None,  "   Max Magnetic value:  %5d  %5d  %5d", 		  Magn_max.x, Magn_max.y, Magn_max.z );	
	SerialLink_Console(None,  "   Min Magnetic value:  %5d  %5d  %5d", 		  Magn_min.x, Magn_min.y, Magn_min.z );	
	SerialLink_Console(None,  "=> Magnetic Offset   :  %5d  %5d  %5d",      (int)MagOffset.x, (int)MagOffset.y, (int)MagOffset.z );		
		
	SerialLink_Console(EventInfo, "Done\n");	
	
	if (PARAM_VariableToSerial(MAGN_OFFSET_X)!= PARAM_OK) 	return CALIB_WriteEPPROM_Fail;	
	if (PARAM_VariableToSerial(MAGN_OFFSET_Y)!= PARAM_OK) 	return CALIB_WriteEPPROM_Fail;	
	if (PARAM_VariableToSerial(MAGN_OFFSET_Z)!= PARAM_OK) 	return CALIB_WriteEPPROM_Fail;		
	SerialLink_SendParameter();
		
	return CALIB_OK;
}

CALIB_Status Estimate_MagnGain(void)
{	
	int16_3D Tmp_Mag;		
	uint64_t zero_time;
	float Highlimit, Lowlimt; 	
				
	SerialLink_Console(EventInfo, "This is fast, don't mind.");		
	
	if(HMC5883_Configuration_A(HMC5883_SAMPLEAVERAGE_8, HMC5883_RATE_15, HMC5883_BIAS_POSITIVE)!= I2C_STATUS_OK)		return CALIB_MagCalibGain_PosBias_ConfigFail;
		
	// Disregarding the first data  
	if(HMC5883_Get_RawMagn(&Tmp_Mag.x, &Tmp_Mag.y, &Tmp_Mag.z)!= I2C_STATUS_OK)																			return 	CALIB_MagCalibGain_PosBias_ReadFail; 
	
	// Read positive bias values
	zero_time = Get_msTick();
	Highlimit = 200/HMC5883_Get_Gain();
	while (Magn_PosBias.x < Highlimit | Magn_PosBias.y < Highlimit | Magn_PosBias.z < Highlimit)
	{
		if(Check_msTick(zero_time,10000)) 				return CALIB_MagCalibGain_PosBias_Timeout;
		if(HMC5883_Get_RawMagn(&Magn_PosBias.x, &Magn_PosBias.y, &Magn_PosBias.z)!= I2C_STATUS_OK)										return 	CALIB_MagCalibGain_PosBias_ReadFail;		
	}		
	
	if(HMC5883_Configuration_A(HMC5883_SAMPLEAVERAGE_8, HMC5883_RATE_15, HMC5883_BIAS_NEGATIVE)!= I2C_STATUS_OK)		return CALIB_MagCalibGain_NegBias_ConfigFail;
			
	// Disregarding the first data  
	if(HMC5883_Get_RawMagn(&Tmp_Mag.x, &Tmp_Mag.y, &Tmp_Mag.z)!= I2C_STATUS_OK)																			return 	CALIB_MagCalibGain_NegBias_ReadFail;		; 
	
	// Read negative bias values
	zero_time = Get_msTick();
	Lowlimt = -200/HMC5883_Get_Gain();
	while (Magn_NegBias.x > Lowlimt | Magn_NegBias.y > Lowlimt| Magn_NegBias.z > Lowlimt)
	{
		if(Check_msTick(zero_time,10000)) 				return CALIB_MagCalibGain_NegBias_Timeout;
		if(HMC5883_Get_RawMagn(&Magn_NegBias.x, &Magn_NegBias.y, &Magn_NegBias.z)!= I2C_STATUS_OK)										return 	CALIB_MagCalibGain_NegBias_ReadFail;		
	}			
	
	SerialLink_Console(None,  "   Negative bias value:  %5d  %5d  %5d",  			Magn_NegBias.x, Magn_NegBias.y, Magn_NegBias.z );	
	SerialLink_Console(None,  "   Positive bias value:  %5d  %5d  %5d", 			Magn_PosBias.x, Magn_PosBias.y, Magn_PosBias.z );		
	SerialLink_Console(None,  "=> Magnetic Gain      :  %0.3f  %0.3f  %0.3f", 	MagScalerGain.x, MagScalerGain.y, MagScalerGain.z );	
	
	MagScalerGain.x = (float) EKFQUA_MAGN_LIMIT/((Magn_PosBias.x	-	Magn_NegBias.x)/2);
	MagScalerGain.y = (float) EKFQUA_MAGN_LIMIT/((Magn_PosBias.y	-	Magn_NegBias.y)/2);
	MagScalerGain.z = (float) EKFQUA_MAGN_LIMIT/((Magn_PosBias.z	-	Magn_NegBias.z)/2);		
			
	SerialLink_Console(EventInfo, "Done\n");	

	if (PARAM_VariableToSerial(MAGN_SCALEGAIN_X)!= PARAM_OK) 	return CALIB_WriteEPPROM_Fail;	
	if (PARAM_VariableToSerial(MAGN_SCALEGAIN_Y)!= PARAM_OK) 	return CALIB_WriteEPPROM_Fail;	
	if (PARAM_VariableToSerial(MAGN_SCALEGAIN_Z)!= PARAM_OK) 	return CALIB_WriteEPPROM_Fail;
	SerialLink_SendParameter();
	
	return CALIB_OK;
}		





CALIB_Status CALIB_Calibrate (uint32_t gyro_second, uint8_t gyroButton, uint8_t magButton)
{
	CALIB_Status State;
	
	RGB_Flash(RGB_WHITE);
	
	SerialLink_Console(None,"\n\n");
	SerialLink_Console(None,"          ================================");
	SerialLink_Console(None,"          ||        Calibration         ||");
	SerialLink_Console(None,"          ================================");
	SerialLink_Console(None,"\n");
	
	if (Initialize_SENSOR_Bus() != SENSOR_OK) return CALIB_SETUP_I2C_Fail;
	
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SerialLink_Console(Event, "Estimate Gyrometer Bias:");	
	
	if ((State = Estimate_GyroBias(gyro_second)) != SENSOR_OK)	
		SerialLink_Console(EventInfo,"%s", String_CALIB_Status[State]);
	while (Button_Check(0)== IsPressed || Button_Check(1)== IsPressed);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	SerialLink_Console(Event, "Estimate Magnetometer Scale Gain:");		
	if ((State = Estimate_MagnGain()) != SENSOR_OK)
		SerialLink_Console(EventInfo,"%s", String_CALIB_Status[State]);
	while (Button_Check(0)== IsPressed || Button_Check(1)== IsPressed);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
	SerialLink_Console(Event, "Estimate Magnetometer Offset:");	
		
	if ((State = Estimate_MagnOffset()) != SENSOR_OK)	
		SerialLink_Console(EventInfo,"%s", String_CALIB_Status[State]);
	while (Button_Check(0)== IsPressed || Button_Check(1)== IsPressed);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		
	SerialLink_Console(Event, "This is the end of Calibration process:");	
	SerialLink_Console(EventInfo, "Click [Save] to write new parameters to EPPROM.");
  SerialLink_Console(EventInfo, "Click [Refresh] to use old parameters.");	
	
	return State;		
}
