




typedef enum 
{
		IMU_Status_OK										= 1,
    IMU_Status_FAIL 			 					= 0
}IMU_Status;





IMU_Status Initialize_MPU6050(void);
IMU_Status  Initialize_HMC5883L(void);



void IMU_Process (void);
IMU_Status Initialize_IMU (void);
