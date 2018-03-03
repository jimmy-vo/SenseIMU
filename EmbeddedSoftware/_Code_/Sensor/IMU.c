#include "IMU.h"
#include "Dev_I2C.h"
#include "Timing_Manage.h"
#include "Serial_Link.h"
#include "MPU6050.h"
#include "HMC5883L.h"

//float_3D Acce_Angle;

static void IMU_delay(u32 nCount)
{
	u32 index = 0;
	for(index = (168000 * nCount); index != 0; index--);
}

//float_3D IMU_Get_Accel_Angles(Acce* acceleration)
//{
//	Acce_Angle.x = 57.295*atan((float)acceleration->y/ sqrt(pow((float)acceleration->z,2)+pow((float)acceleration->x,2)));
//	Acce_Angle.y = 57.295*atan((float)-acceleration->x/ sqrt(pow((float)acceleration->z,2)+pow((float)acceleration->y,2)));	
//	return Acce_Angle;
//}	

IMU_Status Initialize_MPU6050(void)		
{    
}
IMU_Status  Initialize_HMC5883L(void)
{
}





