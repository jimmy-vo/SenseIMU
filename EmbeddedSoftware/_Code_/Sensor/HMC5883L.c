#include "HMC5883L.h"
#include "Dev_I2C.h"
#include "Serial_Link.h"


uint8_t HMC5883_Reg [HMC5883_RegSize];

float HMC5883_Gain = 1;;

//#define HMC5883_RegDebug			1 		
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////I2C	Driver
//
static HMC5883_Status I2C_RegWrite (HMC58_Register Register, uint8_t *buffer)
{	
	uint8_t reg = Register;	
	HMC5883_Status State = HMC5883_STATUS_OK;
	I2C_Status I2C;
		
	if ((I2C = I2C_Request(&IMU,	I2C_Phase_TX,  HMC5883_DEFAULT_ADDRESS, HMC5883_DEFAULT_ADDRESS, &reg, 1, buffer, 1)) != I2C_STATUS_OK)
	{
		State = HMC5883_STATUS_REQ_FAIL;
		SerialLink_Console(Lowlevel,"%s -> %s", String_HMC5883_Status[State], String_I2C_Status[I2C]);
		return State;
	}
	
	if ((I2C = I2C_BUSY_Check (&IMU)) != I2C_STATUS_OK)	
	{
		State = HMC5883_STATUS_BUSY;
		SerialLink_Console(Lowlevel,"%s -> %s", String_HMC5883_Status[State], String_I2C_Status[I2C]);
		return State;
	}
	
	return State;
}

static HMC5883_Status I2C_Read (HMC58_Register Register, uint8_t *buffer, uint32_t length)
{
	uint8_t reg =  Register;	
	HMC5883_Status State = HMC5883_STATUS_OK;
	I2C_Status I2C;
		
	if ((I2C = I2C_Request(&IMU,	I2C_Phase_RX,  HMC5883_DEFAULT_ADDRESS, HMC5883_DEFAULT_ADDRESS, &reg, 1, buffer, length)) != I2C_STATUS_OK)
	{
		State = HMC5883_STATUS_REQ_FAIL;
		SerialLink_Console(Lowlevel,"%s -> %s", String_HMC5883_Status[State], String_I2C_Status[I2C]);
		return State;
	}
	
	if ((I2C = I2C_BUSY_Check (&IMU)) != I2C_STATUS_OK)	
	{
		State = HMC5883_STATUS_BUSY;
		SerialLink_Console(Lowlevel,"%s -> %s", String_HMC5883_Status[State], String_I2C_Status[I2C]);
		return State;
	}
	
	return State;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////CONFIGURATION REGISTER																																																											 ////
//
HMC5883_Status HMC5883_Configuration_A(HMC5883_SAMPLEAVERAGE  SAMPLEAVERAGE, HMC5883_RATE RATE, HMC5883_BIAS BIAS )
{	
	uint8_t check, newconfig = 0x00, mask = 0x00;	
	
	if(I2C_Read(HMC58_R_CONFA,&newconfig,1) != I2C_STATUS_OK)				return HMC5883_STATUS_FAIL;	
	
	// SAMPLEAVERAGE
	if (SAMPLEAVERAGE != HMC5883_SAMPLEAVERAGE_DontCare) 		
	{
		newconfig &= HMC5883_SAMPLEAVERAGE_clear;
		newconfig |= (SAMPLEAVERAGE<<HMC5883_SAMPLEAVERAGE_Position&0xFF);
		mask |= ~ HMC5883_SAMPLEAVERAGE_clear;
	}
	
	//RATE
	if (RATE != HMC5883_RATE_DontCare) 											
	{
		newconfig &= HMC5883_RATE_clear;
		newconfig |= (RATE<<HMC5883_RATE_Position&0xFF);
		mask |= ~ HMC5883_RATE_clear;
	}
	
	//BIAS
	if (BIAS != HMC5883_BIAS_DontCare) 					
	{
		newconfig &= HMC5883_BIAS_clear;
		newconfig |= (BIAS<<HMC5883_BIAS_Position&0xFF);
		mask |= ~ HMC5883_BIAS_clear;
	}
		
	if(I2C_RegWrite	(HMC58_R_CONFA,&newconfig) != I2C_STATUS_OK)			return HMC5883_STATUS_FAIL;     
	
	if(I2C_Read			(HMC58_R_CONFA,&check,1) != I2C_STATUS_OK)				return HMC5883_STATUS_FAIL; 
	
	HMC5883_Reg [HMC58_R_CONFA]  = check;	    
	
	if (check!=newconfig)																												
		{
			SerialLink_Console(Highlevel,"HMC5883 - CONFA Fail");
			return HMC5883_STATUS_FAIL;
		}			
	else 																																			
		{
			#ifdef HMC5883_RegDebug	
			SerialLink_Console(Highlevel,"HMC5883 - CONFA: %s",byte_to_binary(check,8));
			#endif
			return HMC5883_STATUS_OK;
		}
}

HMC5883_Status HMC5883_Configuration_B(HMC5883_GAIN  GAIN )
{	
	uint8_t check, newconfig = 0x00, mask = 0x00;	
	
	if(I2C_Read(HMC58_R_CONFB,&newconfig,1) != I2C_STATUS_OK)					return HMC5883_STATUS_FAIL;	
	
	// GAIN
	if (GAIN != HMC5883_GAIN_DontCare) 									
	{
		newconfig &= HMC5883_GAIN_clear;
		newconfig |= (GAIN<<HMC5883_GAIN_Position&0xFF);
		mask |= ~ HMC5883_GAIN_clear;
	}
		
	if(I2C_RegWrite	(HMC58_R_CONFB,&newconfig) != I2C_STATUS_OK)			return HMC5883_STATUS_FAIL;     
	if(I2C_Read			(HMC58_R_CONFB,&check,1) != I2C_STATUS_OK)				return HMC5883_STATUS_FAIL; 
	
	HMC5883_Reg[HMC58_R_CONFB] = check;	   
	
	if (check!=newconfig)																												
		{
			SerialLink_Console(Highlevel,"HMC5883 - CONFB Fail");
			return HMC5883_STATUS_FAIL;
		}			
	else 																																			
		{
			if 			(GAIN == HMC5883_GAIN_1370)		HMC5883_Gain 	= 0.73f;
			else if (GAIN == HMC5883_GAIN_1090)		HMC5883_Gain	= 0.92f;
			else if (GAIN == HMC5883_GAIN_820)		HMC5883_Gain	= 1.22f;
			else if (GAIN == HMC5883_GAIN_660)		HMC5883_Gain	= 1.52f;
			else if (GAIN == HMC5883_GAIN_440)		HMC5883_Gain	= 2.27f;
			else if (GAIN == HMC5883_GAIN_390)		HMC5883_Gain	= 2.56f;
			else if (GAIN == HMC5883_GAIN_330)		HMC5883_Gain	= 3.03f;
			else if (GAIN == HMC5883_GAIN_230)		HMC5883_Gain	= 4.35f;
			
			#ifdef HMC5883_RegDebug	
			SerialLink_Console(Highlevel,"HMC5883 - CONFB: %s",byte_to_binary(check,8));
			#endif
			return HMC5883_STATUS_OK;
		}
}

HMC5883_Status HMC5883_Configuration_MODE(HMC5883_HIGHSPEED  HIGHSPEED, HMC5883_MODE MODE )
{	
	uint8_t check, newconfig = 0x00, mask = 0x00;	
	
	if(I2C_Read(HMC58_R_MODE,&newconfig,1) != I2C_STATUS_OK)					return HMC5883_STATUS_FAIL;	
	
	// prepare for config
	if (HIGHSPEED != HMC5883_HIGHSPEED_DontCare) 					
	{
		newconfig &= HMC5883_HIGHSPEED_clear;
		newconfig |= (HIGHSPEED<<HMC5883_HIGHSPEED_Position&0xFF);
		mask |= ~ HMC5883_HIGHSPEED_clear;
	}
	if (MODE != HMC5883_MODE_DontCare) 						
	{
		newconfig &= HMC5883_MODE_clear;
		newconfig |= (MODE<<HMC5883_MODE_Position&0xFF);
		mask |= ~ HMC5883_MODE_clear;
	}
		
	if(I2C_RegWrite	(HMC58_R_MODE,&newconfig) != I2C_STATUS_OK)				return HMC5883_STATUS_FAIL;     
	if(I2C_Read			(HMC58_R_MODE,&check,1) != I2C_STATUS_OK)					return HMC5883_STATUS_FAIL; 
	
	HMC5883_Reg[HMC58_R_MODE] = check;
		
	if (check!=newconfig)																												
		{
			SerialLink_Console(Highlevel,"HMC5883 - MODE  Fail");
			return HMC5883_STATUS_FAIL;
		}			
	else 																																			
		{
			#ifdef HMC5883_RegDebug	
			SerialLink_Console(Highlevel,"HMC5883 - MODE:  %s",byte_to_binary(check,8));
			#endif
			return HMC5883_STATUS_OK;
		}		
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 																																																												 ////
//// 																							INDIVIDUAL CONFIGURATION 																									 ////
//// 																																																												 ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HMC5883_Status HMC5883_Set_OutputRate(HMC5883_RATE RATE) 
{
	return HMC5883_Configuration_A(HMC5883_SAMPLEAVERAGE_DontCare, RATE, HMC5883_BIAS_DontCare);	    			
}

HMC5883_Status HMC5883_Set_SAMPLEAVERAGE(HMC5883_SAMPLEAVERAGE SAMPLEAVERAGE) 
{
	return HMC5883_Configuration_A(SAMPLEAVERAGE, HMC5883_RATE_DontCare, HMC5883_BIAS_DontCare);	    			
}

HMC5883_Status HMC5883_Set_BIAS(HMC5883_BIAS BIAS) 
{
	return HMC5883_Configuration_A(HMC5883_SAMPLEAVERAGE_DontCare, HMC5883_RATE_DontCare, BIAS);	    			
}

HMC5883_Status HMC5883_Set_Gain(HMC5883_GAIN gain) //if (gain > 7)
{
	return 	HMC5883_Configuration_B(gain);
}

HMC5883_Status HMC5883_Set_Mode(HMC5883_MODE MODE)
{
    return HMC5883_Configuration_MODE(HMC5883_HIGHSPEED_DontCare, MODE);
}
HMC5883_Status HMC5883_Set_HighSpeed(HMC5883_HIGHSPEED HIGHSPEED)
{
    return HMC5883_Configuration_MODE(HIGHSPEED , HMC5883_MODE_DontCare);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	USER READ
//
HMC5883_Status HMC5883_GetDeviceID(uint8_t *id) 
{
    if(I2C_Read(	HMC58_R_IDA,id,3)!= I2C_STATUS_OK) 
				return HMC5883_STATUS_FAIL;
			else
				return HMC5883_STATUS_OK;
}

HMC5883_Status HMC5883_ConnectionCheck(void) 
{  
      uint8_t data[3];
     if(HMC5883_GetDeviceID (data) == HMC5883_STATUS_OK)
			{
				if((data[0] == 'H' && data[1] == '4' && data[2] == '3'))
						return HMC5883_STATUS_OK;
					else
							return HMC5883_STATUS_FAIL;
			}	
			else 	return HMC5883_STATUS_FAIL;
}

HMC5883_Status  HMC5883_Get_RawMagn(int16_t *Mx,int16_t *My,int16_t *Mz)
{	
    uint8_t data[6];
        
    if(I2C_Read(HMC58_R_X_MSB,data,6) == I2C_STATUS_OK) 
			{			
        *Mx = (((int16_t)data[0]) 	<< 8) 	| data[1]	; 
        *Mz = (((int16_t)data[2]) 	<< 8) 	| data[3]	;
        *My = (((int16_t)data[4]) 	<< 8) 	| data[5]	;
				
				*Mx = (int16_t) (HMC5883_Gain * (*Mx));
				*Mz = (int16_t) (HMC5883_Gain * (*Mz));
				*My = (int16_t) (HMC5883_Gain * (*My));
				
				return HMC5883_STATUS_OK;
			}
			else 
				return HMC5883_STATUS_FAIL; 
}

float HMC5883_Get_Gain (void)
{
	return HMC5883_Gain;
}
