#include <math.h>
#include "arm_math.h"  
#include "SENSOR_DATA.h"
#include "BoardConfig.h"


void Tilt_Compensate (float_3D *RAD, float_3D *Acce, float_3D *Magn);
void Complementary_Estimate (float_3D *Euler, float_3D *Gyro, float_3D *Acce, float_3D *Magn, float sampletime);
void Gyro_Integration (float_3D *RAD, float_3D *Gyro,  float sampletime);
void Quaternion_UpdateRate (Quaternion *Out, float_3D *Gyro, Quaternion *In, float sampletime);

void Normalize_Quaternion (Quaternion *Qua);
void Normalize_Four (float *q1, float *q2, float *q3, float *q4);
void Normalize_Three (float *q1, float *q2, float *q3);


void Convert_Rad_Quaternion(float_3D *RAD, Quaternion *Q);
void Convert_Deg_Quaternion(float_3D *DEG, Quaternion *Q);

void Convert_Deg_Rad(float_3D *DEG, float_3D *RAD);
void Convert_Rad_Deg(float_3D *RAD, float_3D *DEG);

void Convert_Dcm_Rad(DCM *dcm, float_3D *RAD);
void Convert_Dcm_Deg(DCM *dcm, float_3D *DEG);

void Convert_Quaternion_Rad(Quaternion *Q, float_3D *RAD);
void Convert_Quaternion_Deg(Quaternion *Q, float_3D *DEG);

void Convert_Quaternion_Dcm(Quaternion *Q, DCM *dcm);


void Convert_DEG_Dcm(float_3D *DEG, DCM *dcm);
void Convert_RAD_Dcm(float_3D *RAD, DCM *dcm);
