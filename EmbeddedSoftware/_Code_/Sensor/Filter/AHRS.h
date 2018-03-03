#include <math.h>
#include "arm_math.h"  
#include "SENSOR_DATA.h"
#include "BoardConfig.h"


void Initialize_AHRS (Quaternion *Qua);
int AHRS_EstimateAttitude (Quaternion *Qua, float_3D *Gyro, float_3D *Acce, float_3D *Magn, float sampletime);

