#include "Algorithm.h"

#include "Serial_Link.h"

#include "RGB.h"
#include "Button.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "Timing_Manage.h"
#include "SDCARD.h"
#include "Parameter.h"
#include "Dev_I2C.h"

#include "Voltage_FB.h"
#include "EKF_DCM.h"
#include "Adaptive.h"
#include "AHRS.h"
#include "Computation.h"


static ALGORITHM Algorithm = ALGORITHM_DCMkf ;
static float_3D Raw_Acce, Raw_Gyro, Raw_Magn;
static float_3D Velocity, Position;
static float Algorithm_dt;
 float Algorithm_Tempe;
static uint64_t Algorithm_Lastime;
ALGORITHM_Status Status = ALGORITHM_SensorInit;


 float_3D EulerRAD, EulerDEG;
 Quaternion Qua;
 DCM 				dcm;

void Algorithm_Switch (ALGORITHM algo)
{
	Algorithm = algo;
	SerialLink_Console(EventInfo, "%s",String_Algorithm[Algorithm]);
		
	dcm.dcm0	= 1;
	dcm.dcm1	= 0;
	dcm.dcm2	= 0;
	dcm.dcm3	= 1;
	dcm.dcm4	= 0;
	dcm.dcm5	= 0;
	dcm.dcm6	= 0;
	dcm.dcm7	= 0;
	dcm.dcm8	= 1;
	
	Status = ALGORITHM_AlgoInit;
}


static void Algorithm_Init (void)
{				
		Tilt_Compensate(&EulerRAD, &Raw_Acce, &Raw_Magn);											
		Convert_Rad_Quaternion(&EulerRAD, &Qua);	
		Convert_Rad_Deg(&EulerRAD, &EulerDEG);	
		
		switch (Algorithm) 
		{
				case ALGORITHM_AHRS				:////////////////////////////////////////////// 			
																	Initialize_AHRS(&Qua); 	
																	break;
				case ALGORITHM_DCMkf			:////////////////////////////////////////////// 			
																	Initialize_EKFDCM(&Raw_Acce,&Raw_Magn);
																	break;			
				default								 		:////////////////////////////////////////////// 
																	break;																	
		}		
}

static void Algorithm_Process (void)
{	
	switch (Algorithm) 
	{
			case ALGORITHM_TiltComp		://////////////////////////////////////////////
																Tilt_Compensate(&EulerRAD, &Raw_Acce, &Raw_Magn);											
																Convert_Rad_Quaternion(&EulerRAD, &Qua);
																Convert_Rad_Deg(&EulerRAD, &EulerDEG);
																Convert_Quaternion_Dcm(&Qua, &dcm);
																break;
			case ALGORITHM_GyroInteg	://////////////////////////////////////////////				
																Quaternion_UpdateRate (&Qua, &Raw_Gyro, &Qua, Algorithm_dt);			
																Normalize_Quaternion(&Qua);														
																Convert_Quaternion_Deg(&Qua, &EulerDEG);
																Convert_Quaternion_Dcm(&Qua, &dcm);
																break;
			case ALGORITHM_Comple			://////////////////////////////////////////////
																Complementary_Estimate(&EulerRAD, &Raw_Gyro, &Raw_Acce, &Raw_Magn, Algorithm_dt);		
																Convert_Rad_Quaternion(&EulerRAD, &Qua); 	
																Convert_Rad_Deg(&EulerRAD, &EulerDEG);
																Convert_Quaternion_Dcm(&Qua, &dcm);
																break;
			case ALGORITHM_AHRS				:////////////////////////////////////////////// 			
																AHRS_EstimateAttitude(&Qua, &Raw_Gyro, &Raw_Acce, &Raw_Magn, Algorithm_dt);	 	
																Convert_Quaternion_Deg(&Qua, &EulerDEG);
																Convert_Quaternion_Dcm(&Qua, &dcm);
																break;
			case ALGORITHM_DCMkf			://////////////////////////////////////////////		
																DCMKF_Estimate(&dcm, &Raw_Gyro, &Raw_Acce, &Raw_Magn, Algorithm_dt);	
																Convert_Dcm_Deg(&dcm, &EulerDEG);																
																Convert_Deg_Quaternion(&EulerDEG, &Qua);
																break;		
			case ALGORITHM_DCMakf			://////////////////////////////////////////////		
																DCMKF_Estimate(&dcm, &Raw_Gyro, &Raw_Acce, &Raw_Magn, Algorithm_dt);	
																Convert_Dcm_Deg(&dcm, &EulerDEG);																
																Convert_Deg_Quaternion(&EulerDEG, &Qua);
																if ((Button_Check(0)== IsPressed) || (Button_Check(1)== IsPressed))
																	Adaptive_ResetVelPos(&Velocity, &Position);
																else
																Adaptive_EstimateVelPos(&Velocity, &Position, dcm, Raw_Acce, Algorithm_dt);														
																break;					
	}	
	
	
	// Serial Link Out		
	SENSOR_DataAlign( &Raw_Acce, &Raw_Gyro, &Raw_Magn, &EulerDEG, &Qua, &dcm, &Algorithm_dt, &Algorithm_Tempe, &Velocity, &Position);	
//			SDLOG_UpdateSensor(
//													Algorithm_dt, 
//													Raw_Gyro.x, Raw_Gyro.y, Raw_Gyro.z,
//													Raw_Acce.x, Raw_Acce.y, Raw_Acce.z,
//													Raw_Magn.x , Raw_Magn.y , Raw_Magn.z  
//												);
}

void Algorithm_Reset (void)
{
	Status = ALGORITHM_SensorInit;
	SENSOR_DataAlign( &Raw_Acce, &Raw_Gyro, &Raw_Magn, &EulerDEG, &Qua, &dcm, &Algorithm_dt, &Algorithm_Tempe, &Velocity, &Position);	
	Adaptive_ResetVelPos(&Velocity, &Position);
}

ALGORITHM_Status Algorithm_Orientiation (void)
{
	SENSOR_Status SENSOR = SENSOR_FAIL;	
	uint64_t dt;
	switch (Status)
	{
		case ALGORITHM_SensorInit:///////////////////////////////////////////		
					SerialLink_Console(Event,	"ALGORITHM Start");			
					
					SerialLink_Console(EventInfo,	"Initialize Sensor Bus");				
					if ((SENSOR = Initialize_SENSOR_Bus()) != SENSOR_OK)	
					{
						SerialLink_Console(EventInfoDetail, "%s",String_SENSOR_Status[SENSOR]);
						return (Status = ALGORITHM_FAIL);
					}			
					
					if (SenseFreq == 0) SenseFreq = 500;
					SerialLink_SendParameter();
										
					SerialLink_Console(EventInfo, "%s",String_Algorithm[Algorithm]);
					return (Status = ALGORITHM_AlgoInit);
		case ALGORITHM_AlgoInit:///////////////////////////////////////////
					RGB_Flash(RGB_WHITE);
					if ((SENSOR = SENSOR_Update_Data (&Raw_Acce, &Raw_Gyro,&Raw_Magn, &Algorithm_Tempe, &Algorithm_dt)) != SENSOR_OK)						Status = ALGORITHM_ReadError;
					else
					{
						Algorithm_Init();
						RGB_Set(RGB_Circle);	
						Algorithm_Lastime = Get_usTick();
						Status = ALGORITHM_OK;
					}
					break;				
		case ALGORITHM_OK://///////////////////////////////////////////
					dt = Get_usTick() - Algorithm_Lastime;
					if (dt < 1000000/SenseFreq)							return (Status = ALGORITHM_OK);
					else
					{
						Algorithm_Lastime = Get_usTick();
						if ((SENSOR = SENSOR_Update_Data (&Raw_Acce, &Raw_Gyro,&Raw_Magn, &Algorithm_Tempe, &Algorithm_dt)) != SENSOR_OK)							Status = ALGORITHM_ReadError;
						else
						{
							if (dt> 1000000/SenseFreq * Algorithm_errordt)		
								RGB_Flash(RGB_BLUE);				
							Algorithm_Process();		
						}
					}	
					break;				
		case ALGORITHM_ReadError://///////////////////////////////////////////		
					SerialLink_Console(EventInfoDetail, "%s",String_ALGORITHM_Status[Status]);	
					SerialLink_Console(EventInfoDetail, "%s",String_SENSOR_Status[SENSOR]);			
					RGB_Set(RGB_BLUE_T);
					Status = ALGORITHM_FAIL;
					break;	
		case ALGORITHM_DTError://///////////////////////////////////////////		
					SerialLink_Console(EventInfoDetail, "%s",String_ALGORITHM_Status[Status]);				
					RGB_Set(RGB_GREEN_T);
					Status = ALGORITHM_FAIL;
					break;	
		default://///////////////////////////////////////////						
					return Status;
	}
	return Status;	
}

