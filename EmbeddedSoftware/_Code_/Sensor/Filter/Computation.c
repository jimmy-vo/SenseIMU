#include "Computation.h"
#include "Parameter.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "ff.h"
#include "arm_math.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////Variable
//



void Tilt_Compensate (float_3D *RAD, float_3D *Acce, float_3D *Magn)
{
	float xH, yH;
	
#ifdef EKFQUA_RESTRICT_PITCH
	RAD->x = atan2(Acce->y, Acce->z) ;
	RAD->y = atan(-Acce->x / sqrt(Acce->y * Acce->y + Acce->z * Acce->z)) ;
#else
	RAD->x = atan(Acce->y / sqrt(Acce->x * Acce->x + Acce->z * Acce->z)) ;
	RAD->y = atan2(-Acce->x, Acce->z) ;
#endif
	xH = Magn->x * cos(RAD->y) + Magn->y * sin(RAD->x) * sin(RAD->y) + Magn->z * sin(RAD->y) * cos(RAD->x);
	yH = Magn->y * cos(RAD->x) - Magn->z * sin(RAD->x) ;
	RAD->z = atan2(-yH,xH);
}


void Complementary_Estimate (float_3D *RAD, float_3D *Gyro, float_3D *Acce, float_3D *Magn, float sampletime)
{
	float_3D LastAngle;

	LastAngle.x = RAD->x;
	LastAngle.y = RAD->y;
	LastAngle.z = RAD->z;
	
	Tilt_Compensate (RAD, Acce, Magn);
			
	// Calculate the angle using a Complimentary filter	
	if (abs(RAD->z - LastAngle.z) > (90 * DEG_TO_RAD))//fix accidental transition yaw
	{			
		RAD->x 	 = Comp.x 	* (LastAngle.x 	+ Gyro->x * sampletime) 	+ (1-Comp.x) * RAD->x;
		RAD->y 	 = Comp.y 	* (LastAngle.y 	+ Gyro->y * sampletime) 	+ (1-Comp.y)* RAD->y;
		RAD->z  -= Gyro->z 		* sampletime;
	}
	else
	{
		RAD->x 	= Comp.x 	* (LastAngle.x 	+ Gyro->x * sampletime) 	+ (1-Comp.x) * RAD->x;
		RAD->y 	= Comp.y 	* (LastAngle.y 	+ Gyro->y * sampletime) 	+ (1-Comp.y)* RAD->y;
		RAD->z	= Comp.z	* (LastAngle.z	+ Gyro->z * sampletime) 	+ (1-Comp.z)  * RAD->z ;
	}
}

void Gyro_Integration (float_3D *RAD, float_3D *Gyro,  float sampletime)
{
	RAD->x  	+= Gyro->x 		* sampletime;
	RAD->y  += Gyro->y 		* sampletime;
	RAD->z  	+= Gyro->z 		* sampletime;
}

void Quaternion_UpdateRate (Quaternion *Out, float_3D *Gyro, Quaternion *In, float sampletime)
{
	Quaternion tmp;
	// Rate of change of quaternion from gyroscope
	tmp.q1 = 0.5f * (-In->q2 * Gyro->x - In->q3 * Gyro->y - In->q4 * Gyro->z) * sampletime;
	tmp.q2 = 0.5f * ( In->q1 * Gyro->x + In->q3 * Gyro->z - In->q4 * Gyro->y) * sampletime;
	tmp.q3 = 0.5f * ( In->q1 * Gyro->y - In->q2 * Gyro->z + In->q4 * Gyro->x) * sampletime;
	tmp.q4 = 0.5f * ( In->q1 * Gyro->z + In->q2 * Gyro->y - In->q3 * Gyro->x) * sampletime;
	
	Out->q1 += tmp.q1;
	Out->q2 += tmp.q2;
	Out->q3 += tmp.q3;
	Out->q4 += tmp.q4;
}

void Normalize_Quaternion (Quaternion *Qua)
{
	float norm = 0;
	
	norm += Qua->q1 * Qua->q1 + Qua->q2 * Qua->q2 + Qua->q3 * Qua->q3 + Qua->q4 * Qua->q4;
	
	if (norm>0)
	{
		norm = sqrt(norm);
		
		Qua->q1  /=  norm;
		Qua->q2  /=  norm;
		Qua->q3  /=  norm;
		Qua->q4  /=  norm;
	}
}

void Normalize_Four (float *q1, float *q2, float *q3, float *q4)
{
	float norm = 0;
	
	norm += *q1 * *q1;
	norm += *q2 * *q2;
	norm += *q3 * *q3;
	norm += *q4 * *q4;
	
	norm = sqrt(norm);
	
	*q1  /=  norm;
	*q2  /=  norm;
	*q3  /=  norm;
	*q4  /=  norm;
}

void Normalize_Three (float *q1, float *q2, float *q3)
{
	float norm = 0;
	
	norm += *q1 * *q1;
	norm += *q2 * *q2;
	norm += *q3 * *q3;
	
	norm = sqrt(norm);
	
	*q1  /=  norm;
	*q2  /=  norm;
	*q3  /=  norm;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////SENSORFormulas
//
void Convert_Deg_Rad(float_3D *DEG, float_3D *RAD)
{	
   RAD->x  = DEG->x		*	DEG_TO_RAD;
   RAD->y = DEG->y	*	DEG_TO_RAD;
   RAD->z   = DEG->z		*	DEG_TO_RAD;
}

void Convert_Rad_Deg(float_3D *RAD, float_3D *DEG)
{	
   DEG->x  = RAD->x		*	RAD_TO_DEG;
   DEG->y = RAD->y	*	RAD_TO_DEG;
   DEG->z   = RAD->z		*	RAD_TO_DEG;
}

void Convert_Rad_Quaternion(float_3D *RAD, Quaternion *Q)
{
  float Roll  =  (RAD->x / 2);
  float Pitch =  (RAD->y / 2);
  float Yaw   =  (RAD->z / 2);
	
	Q->q1 = cos(Roll)*cos(Pitch)*cos(Yaw) + sin(Roll)*sin(Pitch)*sin(Yaw);
	Q->q2 = sin(Roll)*cos(Pitch)*cos(Yaw) - cos(Roll)*sin(Pitch)*sin(Yaw);
	Q->q3 = cos(Roll)*sin(Pitch)*cos(Yaw) + sin(Roll)*cos(Pitch)*sin(Yaw);
	Q->q4 = cos(Roll)*cos(Pitch)*sin(Yaw) - sin(Roll)*sin(Pitch)*cos(Yaw);
}

void Convert_Deg_Quaternion(float_3D *DEG, Quaternion *Q)
{
	float_3D RAD;
	Convert_Deg_Rad (DEG, &RAD);
	Convert_Rad_Quaternion(&RAD, Q);
}

void Convert_Dcm_Rad(DCM *dcm, float_3D *RAD)
{	
	RAD->x 			= atan2(dcm->dcm7,dcm->dcm8) 		;   
	RAD->y 			= -asin(dcm->dcm6)							;         
	RAD->z				= atan2(dcm->dcm3, dcm->dcm0)		;	
}

void Convert_Dcm_Deg(DCM *dcm, float_3D *DEG)
{		
	float_3D RAD;
	Convert_Dcm_Rad(dcm, &RAD);	
	Convert_Rad_Deg (&RAD, DEG);
}

void Convert_Quaternion_Rad(Quaternion *Q, float_3D *RAD)
{    	
	RAD->x  	= atan2 (2*(Q->q1*Q->q2 + Q->q3*Q->q4), 1 - 2*(Q->q2*Q->q2 + Q->q3*Q->q3));
  RAD->y  = asin  (2*(Q->q1*Q->q3 - Q->q2*Q->q4)) 											            ;
  RAD->z 		= atan2 (2*(Q->q1*Q->q4 + Q->q2*Q->q3), 1 - 2*(Q->q3*Q->q3 + Q->q4*Q->q4));
}

void Convert_Quaternion_Deg(Quaternion *Q, float_3D *DEG)
{
	float_3D RAD;
	Convert_Quaternion_Rad(Q, &RAD);	
	Convert_Rad_Deg (&RAD, DEG);
}

void Convert_Quaternion_Dcm(Quaternion *Q, DCM *dcm)
{//    	QUAtoDCM(Q') = QUAtoDCM(Q)'  rotation matrix describing sensor relative to Earth
	float q0 = 		Q->q1;
	float q1 =  	-Q->q2;
	float q2 =  	-Q->q3;
	float q3 =  	-Q->q4;

	dcm->dcm0 = 2 * q0 * q0 - 1 + 2 * q1 * q1;
	dcm->dcm1 = 2 * (q1 * q2 + q0 * q3);
	dcm->dcm2 = 2 * (q1 * q3 - q0 * q2);
	
	dcm->dcm3 = 2 * (q1 * q2 - q0 * q3);
	dcm->dcm4 = 2 * q0 * q0 - 1 + 2 * q2 * q2;
	dcm->dcm5 = 2 * (q2 * q3 + q0 * q1);
	
	dcm->dcm6 = 2 * (q1 * q3 + q0 * q2);
	dcm->dcm7 = 2 * (q2 * q3 - q0 * q1);
	dcm->dcm8 = 2 * q0 * q0 - 1 + 2 * q3 * q3;
}


void Convert_RAD_Dcm(float_3D *RAD, DCM *dcm)
{// ZYX	
   dcm->dcm0 = cos(RAD->z)*cos(RAD->y);
   dcm->dcm1 = -sin(RAD->z)*cos(RAD->x) + cos(RAD->z)*sin(RAD->y)*sin(RAD->x);
   dcm->dcm2 = sin(RAD->z)*sin(RAD->x) + cos(RAD->z)*sin(RAD->y)*cos(RAD->x);
   dcm->dcm3 = sin(RAD->z)*cos(RAD->y);
   dcm->dcm4 = cos(RAD->z)*cos(RAD->x) + sin(RAD->z)*sin(RAD->y)*sin(RAD->x);
   dcm->dcm5 = -cos(RAD->z)*sin(RAD->x) + sin(RAD->z)*sin(RAD->y)*cos(RAD->x);

   dcm->dcm6 = -sin(RAD->y);
   dcm->dcm7 = cos(RAD->y)*sin(RAD->x);
   dcm->dcm8 = cos(RAD->y)*cos(RAD->x);
}

void Convert_DEG_Dcm(float_3D *DEG, DCM *dcm)
{
	float_3D RAD;	
	Convert_Deg_Rad (DEG, &RAD);
	Convert_RAD_Dcm(&RAD, dcm);
}
