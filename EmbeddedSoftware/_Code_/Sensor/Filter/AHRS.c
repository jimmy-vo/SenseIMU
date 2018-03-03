#include "AHRS.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "Timing_Manage.h"
#include "SDCARD.h"
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

//AHRS
static Quaternion AHRS_Qua;
static float  eInt[3];



void Initialize_AHRS (Quaternion *Qua)
{			
	AHRS_Qua.q1 = 1;
	AHRS_Qua.q2 = 0;
	AHRS_Qua.q3 = 0;
	AHRS_Qua.q4 = 0;
}


int AHRS_EstimateAttitude (Quaternion *Qua, float_3D *Gyro, float_3D *Acce, float_3D *Magn, float sampletime)
{
	float norm;	
	float hx, hy, bx, bz;
	float vx, vy, vz, wx, wy, wz;
	float ex, ey, ez;
	float pa, pb, pc;
	float ax, ay, az;
	float mx, my, mz;
	float gx, gy, gz;		
	float q1 = AHRS_Qua.q1; 
	float	q2 = AHRS_Qua.q2;
	float	q3 = AHRS_Qua.q3; 
	float	q4 = AHRS_Qua.q4;  
	float	q1q1, q1q2, q1q3, q1q4, q2q2, q2q3, q2q4,  q3q3, q3q4, q4q4;

	// Update Data
	ax = Acce->x;		ay = Acce->y;		az = Acce->z; 
	gx = Gyro->x; 	gy = Gyro->y; 	gz = Gyro->z; 
	mx = Magn->x;		my = Magn->y;		mz = Magn->z; 

	// Normalise accelerometer measurement
	norm = (float)sqrt(ax * ax + ay * ay + az * az);
	if (norm == 0.0f) return 0; // handle NaN
	norm = 1 / norm;        // use reciprocal for division
	ax *= norm;
	ay *= norm;
	az *= norm;

	// Normalise magnetometer measurement
	norm = (float)sqrt(mx * mx + my * my + mz * mz);
	if (norm == 0.0f) return 0; // handle NaN
	norm = 1 / norm;        // use reciprocal for division
	mx *= norm;
	my *= norm;
	mz *= norm;

	// Auxiliary variables to avoid repeated arithmetic
	q1q1 = q1 * q1;			q1q2 = q1 * q2;			 q1q3 = q1 * q3;			 q1q4 = q1 * q4;
											q2q2 = q2 * q2;			 q2q3 = q2 * q3;			 q2q4 = q2 * q4;
																					 q3q3 = q3 * q3;			 q3q4 = q3 * q4;
																																 q4q4 = q4 * q4;   

	// Reference direction of Earth's magnetic field
	hx = 2.0f * mx * (0.5f - q3q3 - q4q4) + 2.0f * my * (q2q3 - q1q4) + 2.0f * mz * (q2q4 + q1q3);
	hy = 2.0f * mx * (q2q3 + q1q4) + 2.0f * my * (0.5f - q2q2 - q4q4) + 2.0f * mz * (q3q4 - q1q2);
	bx = (float)sqrt((hx * hx) + (hy * hy));
	bz = 2.0f * mx * (q2q4 - q1q3) + 2.0f * my * (q3q4 + q1q2) + 2.0f * mz * (0.5f - q2q2 - q3q3);

	// Estimated direction of gravity and magnetic field
	vx = 2.0f * (q2q4 - q1q3);
	vy = 2.0f * (q1q2 + q3q4);
	vz = q1q1 - q2q2 - q3q3 + q4q4;
	wx = 2.0f * bx * (0.5f - q3q3 - q4q4) + 2.0f * bz * (q2q4 - q1q3);
	wy = 2.0f * bx * (q2q3 - q1q4) + 2.0f * bz * (q1q2 + q3q4);
	wz = 2.0f * bx * (q1q3 + q2q4) + 2.0f * bz * (0.5f - q2q2 - q3q3);  

	// Error is cross product between estimated direction and measured direction of gravity
	ex = (ay * vz - az * vy) + (my * wz - mz * wy);
	ey = (az * vx - ax * vz) + (mz * wx - mx * wz);
	ez = (ax * vy - ay * vx) + (mx * wy - my * wx);
	if (AHRS_Ki > 0.0f)
	{
		eInt[0] += ex;      // accumulate integral error
		eInt[1] += ey;
		eInt[2] += ez;
	}
	else
	{
		eInt[0] = 0.0f;     // prevent integral wind up
		eInt[1] = 0.0f;
		eInt[2] = 0.0f;
	}

	// Apply feedback terms
	gx = gx + AHRS_Kp * ex + AHRS_Ki * eInt[0];
	gy = gy + AHRS_Kp * ey + AHRS_Ki * eInt[1];
	gz = gz + AHRS_Kp * ez + AHRS_Ki * eInt[2];

	// Integrate rate of change of quaternion
	pa = q2;
	pb = q3;
	pc = q4;
	q1 = q1 + (-q2 * gx - q3 * gy - q4 * gz) * 	(0.5f * sampletime);
	q2 = pa + (q1 * gx + pb * gz - pc * gy) * 	(0.5f * sampletime);
	q3 = pb + (q1 * gy - pa * gz + pc * gx) * 	(0.5f * sampletime);
	q4 = pc + (q1 * gz + pa * gy - pb * gx) * 	(0.5f * sampletime);

	// Normalise quaternion
	norm = (float)sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);
	norm = 1.0f / norm;
	Qua->q1 = AHRS_Qua.q1 = q1 * norm;
	Qua->q2 = AHRS_Qua.q2 = q2 * norm;
	Qua->q3 = AHRS_Qua.q3 = q3 * norm;
	Qua->q4 = AHRS_Qua.q4 = q4 * norm;
	
	return 1;
}

