#include <math.h>
#include "arm_math.h"  
#include "BoardConfig.h"
#include "SENSOR_DATA.h"


void Initialize_EKFDCM (float_3D *Acce, float_3D *Magn);
void DCMKF_Estimate(DCM *Kalman_dcm, float_3D *Gyro, float_3D *Acce, float_3D *Magn, float sampletime);
