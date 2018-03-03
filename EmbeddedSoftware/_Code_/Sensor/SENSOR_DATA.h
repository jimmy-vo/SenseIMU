#include <math.h>
#include "arm_math.h"  
#include "BoardConfig.h"

#ifndef SENSOR_DATA
#define SENSOR_DATA
typedef struct 
{
    int16_t x;
    int16_t y;
    int16_t z;
}int16_3D;

typedef struct 
{
    int64_t x;
    int64_t y;
    int64_t z;
}int64_3D;

typedef struct 
{
    float x;
    float y;
    float z;
}float_3D;

typedef struct 
{
    int16_t q1;
    int16_t q2;
    int16_t q3;
    int16_t q4;
}int16_4D;

typedef struct 
{
    float32_t q1;
    float32_t q2;
    float32_t q3;
    float32_t q4;
}Quaternion;


typedef struct 
{
    float32_t dcm0;
    float32_t dcm1;
    float32_t dcm2;
    float32_t dcm3;
    float32_t dcm4;
    float32_t dcm5;
    float32_t dcm6;
    float32_t dcm7;
    float32_t dcm8;
}DCM;

typedef enum 
{	
	SENSOR_Normalize_Enable,
	SENSOR_Normalize_Disable,
}SENSOR_Norm;

typedef enum 
{//SENSOR_Status
		SENSOR_OK															,
    SENSOR_FAIL 			 										,
		///////////////////////////////////////
    SENSOR_SETUP_HMC5883_Fail							,
    SENSOR_SETUP_MPU6050_Fail							,
		///////////////////////////////////////		
    SENSOR_READ_HMC5883_Fail		 					,
    SENSOR_READ_MPU6050_Fail							,		
		///////////////////////////////////////				
}SENSOR_Status;


/////////////////////////////////////////////////
//Constant
//
#define  DEG_TO_RAD  	0.0174532925f
#define  RAD_TO_DEG  	57.2957795f
/////////////////////////////////////////////////
// Calib Sample
//
#define NM_sample					 	3500
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//	Magn
//
#define EKFQUA_HMC5883_AVERAGE						HMC5883_SAMPLEAVERAGE_0
#define EKFQUA_HMC5883_RATE							HMC5883_RATE_75
#define EKFQUA_HMC5883_BIAS							HMC5883_BIAS_NORMAL
#define EKFQUA_HMC5883_HIGHSPEED					HMC5883_HIGHSPEED_DISABLE
#define EKFQUA_HMC5883_MODE							HMC5883_MODE_CONTINUOUS
#define EKFQUA_HMC5883_GAIN							HMC5883_GAIN_1370
 	
#define EKFQUA_MAGN_LIMIT												1160
/////////////////////////////////////////////////
//	MPU6050
//
#define EKFQUA_MPU6050_SleepMode							MPU6050_SLEEP_DISABLE
#define EKFQUA_MPU6050_Temperature						MPU6050_TEMPERATURE_ENABLE
#define EKFQUA_MPU6050_Clock									MPU6050_CLOCK_PLL_XGYRO

#define EKFQUA_MPU6050_DLPF_CFG							DLPF_CFG_0
#define EKFQUA_MPU6050_SMPLRT_DIV						0x07

#define EKFQUA_GYRO_FullScaleRange						MPU6050_GYRO_FS_250
#define EKFQUA_GYRO_Sensitivity								131.0f

#define EKFQUA_ACCEL_FullScaleRange						MPU6050_ACCEL_FS_2
#define EKFQUA_ACCEL_Sensitivity							1638.4f
/////////////////////////////////////////////////
//	Macro
//
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))


SENSOR_Status SENSOR_Update_Data (float_3D *Acce, float_3D *Gyro, float_3D *Magn, float *Temp, float32_t *SampleTime);

SENSOR_Status Initialize_SENSOR_Bus (void);
void SENSOR_DataAlign(float_3D *Acce, float_3D *Gyro, float_3D *Magn, float_3D *DEG, Quaternion *Qua, DCM *dcm, float *dt, float* SenseTemp, float_3D *Velocity, float_3D *Position);

extern float_3D 		MagScalerGain, MagOffset, MagAxis, GyroBias;
extern float 				EKFQUA_r,EKFQUA_q;
extern float 				KFDCM_r1, KFDCM_q1, KFDCM_r2,KFDCM_q2;
extern float 				aKFDCM_r1, aKFDCM_q1, aKFDCM_r2,aKFDCM_q2;
extern float 			Thres_Acce; 
extern float 			Thres_Gyro;
extern float 				AHRS_Kp, AHRS_Ki;
extern float_3D 		Comp; 
extern float 				SenseFreq, SenseBat;

#endif
