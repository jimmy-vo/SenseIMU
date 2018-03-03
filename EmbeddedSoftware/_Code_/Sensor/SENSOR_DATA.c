#include "Button.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "RGB.h"
#include "Timing_Manage.h"
#include "Computation.h"
#include "SENSOR_DATA.h"
#include "Voltage_FB.h"
#include "Serial_Link.h"
#include "Parameter.h"
#include "Dev_I2C.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ff.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////Variable
//
static uint64_t SENSOR_Lastime;
static  uint16_t 	Framecount = 0;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////extern global term	
//
float_3D 		MagScalerGain, MagOffset, MagAxis, GyroBias;

float 			EKFQUA_r; 
float 			EKFQUA_q;
float 			KFDCM_r1; 
float 			KFDCM_q1;
float 			KFDCM_r2; 
float 			KFDCM_q2;
float 			Thres_Acce; 
float 			Thres_Gyro;
float 			aKFDCM_r1; 
float 			aKFDCM_q1;
float 			aKFDCM_r2; 
float 			aKFDCM_q2;

float_3D 	Comp; 
float 			SenseFreq = 500, SenseBat;
float 			AHRS_Kp, AHRS_Ki;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////Driver
//
static void SENSOR_delay(u32 nCount)
{
	Delay_ms(nCount);
}

SENSOR_Status Initialize_SENSOR_Bus (void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	/* GPIO Periph clock enable */
	RCC_AHB1PeriphClockCmd(I2C1_GPIO_CLK, ENABLE);

	/* Configure I2C2 pins: SCL and SDA */
	GPIO_InitStructure.GPIO_Pin 		=  	I2C1_SCL_PIN | I2C1_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode 		= 	GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  	= 	GPIO_PuPd_NOPULL;
	GPIO_Init(I2C1_GPIO_PORT, &GPIO_InitStructure);

	/* Connect GPIO pins to peripheral */
	GPIO_PinAFConfig(I2C1_GPIO_PORT, I2C1_SCL_SOURCE, I2C1_AF); 
	GPIO_PinAFConfig(I2C1_GPIO_PORT, I2C1_SDA_SOURCE, I2C1_AF);

	IMU.TX.DMA									= I2C_DMA_Enable;
	IMU.TX.DMA_Channel					=	I2C1_TX_DMA_CH;
	IMU.TX.DMA_Stream						=	I2C1_TX_DMA_ST;
	IMU.TX.DMA_Flag							=	I2C1_TX_DMA_FLAG;
	IMU.TX.DMA_NVIC_IRQ					=	I2C1_TX_DMA_IRQn;
	IMU.TX.SubPri								=	NVIC_Subpriority_SENSOR;
	IMU.TX.preemtionPri					=	NVIC_Prepriority_SENSOR;

	IMU.RX.DMA									= I2C_DMA_Enable;
	IMU.RX.DMA_Channel					=	I2C1_RX_DMA_CH;
	IMU.RX.DMA_Stream						=	I2C1_RX_DMA_ST;
	IMU.RX.DMA_Flag							=	I2C1_RX_DMA_FLAG;
	IMU.RX.DMA_NVIC_IRQ					=	I2C1_RX_DMA_IRQn;
	IMU.TX.SubPri								=	NVIC_Subpriority_SENSOR;
	IMU.TX.preemtionPri					=	NVIC_Prepriority_SENSOR+1;
	
	IMU.TimeOut 									= 100;		//10ms
	IMU.I2C											= I2C1;		
	
	Initialize_I2C(&IMU,400000);			//400kHz
			
	SENSOR_delay(100);		

	if(MPU6050_ConnectionCheck()	==	MPU6050_STATUS_OK)	
		{	 				
			if	(MPU6050_Set_PWR_MGMT_1(EKFQUA_MPU6050_SleepMode, EKFQUA_MPU6050_Temperature, EKFQUA_MPU6050_Clock)!= MPU6050_STATUS_OK)
				return SENSOR_SETUP_MPU6050_Fail;		
				
			if	(	MPU6050_Set_GYRO_Config(EKFQUA_GYRO_FullScaleRange)!= MPU6050_STATUS_OK) 
				return SENSOR_SETUP_MPU6050_Fail;		
				
			if	(MPU6050_Set_ACCEL_Config(EKFQUA_ACCEL_FullScaleRange)!= MPU6050_STATUS_OK) 
				return SENSOR_SETUP_MPU6050_Fail;		
				
			if	(MPU6050_Set_Configuration(EKFQUA_MPU6050_DLPF_CFG)!= MPU6050_STATUS_OK)	
				return SENSOR_SETUP_MPU6050_Fail;		
				
			if	(	MPU6050_Set_SMPLRT_DIV(EKFQUA_MPU6050_SMPLRT_DIV)!= MPU6050_STATUS_OK)	
				return SENSOR_SETUP_MPU6050_Fail;		
		}		
//		
	if(HMC5883_ConnectionCheck() == HMC5883_STATUS_OK)
		{
			if(HMC5883_Configuration_A(EKFQUA_HMC5883_AVERAGE, EKFQUA_HMC5883_RATE, EKFQUA_HMC5883_BIAS)!= I2C_STATUS_OK)		
				return	SENSOR_SETUP_HMC5883_Fail;					
			if(HMC5883_Configuration_B(EKFQUA_HMC5883_GAIN)!= I2C_STATUS_OK)																									
				return	SENSOR_SETUP_HMC5883_Fail;	
			if(HMC5883_Configuration_MODE(EKFQUA_HMC5883_HIGHSPEED, EKFQUA_HMC5883_MODE)!= I2C_STATUS_OK)				
				return	SENSOR_SETUP_HMC5883_Fail;	
		}		
		
	return SENSOR_OK;
}

SENSOR_Status SENSOR_Update_Data (float_3D *Acce, float_3D *Gyro, float_3D *Magn, float *Temp, float32_t *SampleTime)
{
	SENSOR_Status READ = SENSOR_OK;
	static int16_t mx, my, mz;
	static int16_t ax, ay, az;
	static int16_t gx, gy, gz;
	static float_3D mag;
	static DCM R;
	/////////////////////////////////////////////////
	//	Calculate dt
	//	
	uint64_t Time = Get_usTick();
	(*SampleTime) = ((float32_t)(Time - SENSOR_Lastime))/1000000.0f;
	SENSOR_Lastime = Time;	

	/////////////////////////////////////////////////
	//	Sampling
	//
	if (MPU6050_Get_rawDATA (&gx, &gy, &gz, &ax, &ay, &az, Temp) != MPU6050_STATUS_OK)
		READ = SENSOR_READ_MPU6050_Fail;
	if (HMC5883_Get_RawMagn (&mx, &my, &mz)!= HMC5883_STATUS_OK)
		READ = SENSOR_READ_HMC5883_Fail;
	/////////////////////////////////////////////////
	//	Magn
	//add offset and scale to circle				
	Magn->x = 	((MagOffset.x + (float)mx) *	 MagScalerGain.x);
	Magn->y = 	((MagOffset.y + (float)my) *	 MagScalerGain.y);
	Magn->z = 	((MagOffset.z + (float)mz) *	 MagScalerGain.z);
	
	//rotate axis	
//	Convert_DEG_Dcm(&MagAxis, &R);	
//	Magn->x = mag.x*R.dcm0 + mag.y*R.dcm1 + mag.z*R.dcm2;
//	Magn->y = mag.x*R.dcm3 + mag.y*R.dcm4 + mag.z*R.dcm5;
//	Magn->z = mag.x*R.dcm6 + mag.y*R.dcm7 + mag.z*R.dcm8;
	/////////////////////////////////////////////////
	//	Gyro
	//Convert to rad/s
	Gyro->x	= ((float)gx - GyroBias.x*(*SampleTime))  / (float)EKFQUA_GYRO_Sensitivity * DEG_TO_RAD ;
	Gyro->y	= ((float)gy - GyroBias.y*(*SampleTime)) / (float)EKFQUA_GYRO_Sensitivity * DEG_TO_RAD;
	Gyro->z	= ((float)gz - GyroBias.z*(*SampleTime)) / (float)EKFQUA_GYRO_Sensitivity * DEG_TO_RAD;	
	/////////////////////////////////////////////////
	//	Acce
	//Convert 
	Acce->x	= ax ;
	Acce->y	= ay ;
	Acce->z	= az ;	
	Acce->x	/= (float)EKFQUA_ACCEL_Sensitivity * 10; //scale to 1 by dividing by 10
	Acce->y	/= (float)EKFQUA_ACCEL_Sensitivity * 10;
	Acce->z	/= (float)EKFQUA_ACCEL_Sensitivity * 10;		
	return READ;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////User function
//
void SENSOR_DataAlign(float_3D *Acce, float_3D *Gyro, float_3D *Magn, float_3D *DEG, Quaternion *Qua, DCM *dcm, float *dt, float* SenseTemp, float_3D *Velocity, float_3D *Position)
{	
	// Serial Link Out		
	DATA_OUT.FrameCnt			= (uint16_t)	(Framecount++);		
	DATA_OUT.Button			= (uint8_t)	(	(Button_Check(0) == IsPressed) | ((Button_Check(1) == IsPressed) <<1));
	if ((Button_Check(0)== IsPressed) || (Button_Check(1)== IsPressed)) RGB_Flash(RGB_WHITE);
	
	DATA_OUT.Accelx			= (int16_t)	(Acce->x  *	10);	
	DATA_OUT.Accely			= (int16_t)	(Acce->y  *	10);	
	DATA_OUT.Accelz			= (int16_t)	(Acce->z  *	10);	
	
	
	DATA_OUT.Gyrox			= (int16_t)	(Gyro->x 	*	10);	
	DATA_OUT.Gyroy 			= (int16_t)	(Gyro->y 	*	10);	
	DATA_OUT.Gyroz 			= (int16_t)	(Gyro->z 	*	10);	
	
	DATA_OUT.Magnx 			= (int16_t)	(Magn->x	*	10);
	DATA_OUT.Magny 			= (int16_t)	(Magn->y	*	10);
	DATA_OUT.Magnz 			= (int16_t)	(Magn->z	*	10);
	
	DATA_OUT.Eulerx 		= (int16_t)	(DEG->x 	* 100);				
	DATA_OUT.Eulery			= (int16_t)	(DEG->y 	*	100);	
	DATA_OUT.Eulerz			= (int16_t) (DEG->z		*	100);			

	DATA_OUT.Qua1				=	(int32_t)	(Qua->q1	*	1000000);
	DATA_OUT.Qua2				=	(int32_t)	(Qua->q2	*	1000000);
	DATA_OUT.Qua3				=	(int32_t)	(Qua->q3	*	1000000);
	DATA_OUT.Qua4				=	(int32_t)	(Qua->q4	*	1000000);	
	
	DATA_OUT.dcm0				=	(int32_t)	(dcm->dcm0		*	1000000);
	DATA_OUT.dcm1				=	(int32_t)	(dcm->dcm1		*	1000000);
	DATA_OUT.dcm2				=	(int32_t)	(dcm->dcm2		*	1000000);
	DATA_OUT.dcm3				=	(int32_t)	(dcm->dcm3		*	1000000);
	DATA_OUT.dcm4				=	(int32_t)	(dcm->dcm4		*	1000000);
	DATA_OUT.dcm5				=	(int32_t)	(dcm->dcm5		*	1000000);
	DATA_OUT.dcm6				=	(int32_t)	(dcm->dcm6		*	1000000);
	DATA_OUT.dcm7				=	(int32_t)	(dcm->dcm7		*	1000000);
	DATA_OUT.dcm8				=	(int32_t)	(dcm->dcm8		*	1000000);
	
	DATA_OUT.Velx				=	(int32_t)	(Velocity->x		*	1000000);
	DATA_OUT.Vely				=	(int32_t)	(Velocity->y		*	1000000);
	DATA_OUT.Velz				=	(int32_t)	(Velocity->z		*	1000000);
	
	DATA_OUT.Posx				=	(int32_t)	(Position->x		*	1000000);
	DATA_OUT.Posy				=	(int32_t)	(Position->y		*	1000000);
	DATA_OUT.Posz				=	(int32_t)	(Position->z		*	1000000);
	
	DATA_OUT.SenseDT 			= (uint32_t) ((*dt)	*	1000000);
	
	DATA_OUT.SenseTemp 			= (uint16_t) ((*SenseTemp)	*	100);
	DATA_OUT.SenseBat 			= (uint16_t) (Get_Voltage()	*	100);
	
	SerialLink_StartStream();
}
