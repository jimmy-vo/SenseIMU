#include "Adaptive.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "Timing_Manage.h"
#include "Computation.h"
#include "Serial_Link.h"
#include "Parameter.h"
#include "RGB.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ff.h"

float stationary_Mag[2];

float_3D Scale_Acce;

typedef struct
{
	float_3D Y[2];
	float_3D X[2];
}HIGHTPASSS;

HIGHTPASSS HP_Vel;
HIGHTPASSS HP_Pos;


#define cutoff 0.1f

uint8_t Adaptive_MoveDetect(float  accX, float  accY, float  accZ )
{
	//Detect Stationary	
	stationary_Mag[0] =	stationary_Mag[1];
	stationary_Mag[1]= sqrt(accX * accX + accY * accY + accZ * accZ);	
	
	if (sqrt(accX * accX + accY * accY + accZ * accZ) >  Thres_Acce ) 
		return 1;
	else		
		return 0;
}
void HighPassFilter(HIGHTPASSS *HP, float *dt, float_3D *newinput)
{
	float alpha = 1/(2*PI * Thres_Gyro * (*dt) + 1);
	
	HP->X[0].x = HP->X[1].x;
	HP->X[0].y = HP->X[1].y;
	HP->X[0].z = HP->X[1].z;
	HP->X[1].x = newinput->x;
	HP->X[1].y = newinput->y;
	HP->X[1].z = newinput->z;
	HP->Y[0].x = HP->Y[1].x;
	HP->Y[0].y = HP->Y[1].y;
	HP->Y[0].z = HP->Y[1].z;		
		
	(HP->Y[1].x) = alpha * (HP->Y[0].x + HP->X[1].x - HP->X[0].x);
	(HP->Y[1].y) = alpha * (HP->Y[0].y + HP->X[1].y - HP->X[0].y);
	(HP->Y[1].z) = alpha * (HP->Y[0].z + HP->X[1].z - HP->X[0].z);
}

void Adaptive_EstimateVelPos (float_3D *Velocity, float_3D *Position, DCM dcm, float_3D Acce, float dt)
{			//Scale accelerometer measurement to g		 dcm describing sensor relative to Earth
	if (Adaptive_MoveDetect(Acce.x, Acce.y, Acce.z ))
	{			
		RGB_Flash(RGB_GREEN);
		
		//normalize to scale it gravity unit
		Normalize_Three(&Acce.x, &Acce.y, &Acce.z);
		
		//turn to earth frame
		Scale_Acce.x = Acce.x*dcm.dcm0 + Acce.y*dcm.dcm1 + Acce.z*dcm.dcm2;
		Scale_Acce.y = Acce.x*dcm.dcm3 + Acce.y*dcm.dcm4 + Acce.z*dcm.dcm5;
		Scale_Acce.z = Acce.x*dcm.dcm6 + Acce.y*dcm.dcm7 + Acce.z*dcm.dcm8;
									
		//intergrate v[k]  = a[k]*T + v[k-1] => convert to m/s^2
		Velocity->x = HP_Vel.X[1].x + ((Scale_Acce.x + 0) * 9.81f) * dt ;
		Velocity->y = HP_Vel.X[1].y + ((Scale_Acce.y + 0) * 9.81f) * dt ;
		Velocity->z = HP_Vel.X[1].z + ((Scale_Acce.z + 1) * 9.81f) * dt ;
				
		//high pass filter
		HighPassFilter(&HP_Vel, &dt, Velocity);	
		Velocity->x = HP_Vel.X[1].x;
		Velocity->y = HP_Vel.X[1].y;
		Velocity->z = HP_Vel.X[1].z;
						
		//intergrate x = v*t + x0
		Position->x = HP_Pos.X[1].x + Velocity->x * dt ;
		Position->y = HP_Pos.X[1].y + Velocity->y * dt ;
		Position->z = HP_Pos.X[1].z + Velocity->z * dt ; 

		//high pass filter
		HighPassFilter(&HP_Pos, &dt, Position);			 
		Position->x = HP_Pos.X[1].x;
		Position->y = HP_Pos.X[1].y;
		Position->z = HP_Pos.X[1].z;
	}
	else 
	{
		Velocity->x = 0;
		Velocity->y = 0;
		Velocity->z = 0;
		HighPassFilter(&HP_Vel, &dt, Velocity);	
	}
}

void Adaptive_ResetVelPos (float_3D *Velocity, float_3D *Position)
{
	Velocity->x = Velocity->y = Velocity->z = 0;	
	HP_Vel.X[0].x = HP_Vel.X[0].y = HP_Vel.X[0].z = 0;
	HP_Vel.X[1].x = HP_Vel.X[1].y = HP_Vel.X[1].z = 0;
	HP_Vel.Y[0].x = HP_Vel.Y[0].y = HP_Vel.Y[0].z = 0;
	HP_Vel.Y[1].x = HP_Vel.Y[1].y = HP_Vel.Y[1].z = 0;
	
	Position->x = Position->y = Position->z = 0;	
	HP_Pos.X[0].x = HP_Pos.X[0].y = HP_Pos.X[0].z = 0;
	HP_Pos.X[1].x = HP_Pos.X[1].y = HP_Pos.X[1].z = 0;
	HP_Pos.Y[0].x = HP_Pos.Y[0].y = HP_Pos.Y[0].z = 0;
	HP_Pos.Y[1].x = HP_Pos.Y[1].y = HP_Pos.Y[1].z = 0;
	
}
