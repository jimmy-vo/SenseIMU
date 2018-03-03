#include <math.h>
#include "arm_math.h"  
#include "SENSOR_DATA.h"
#include "BoardConfig.h"

uint8_t Adaptive_MoveDetect(float  accX, float  accY, float  accZ );
void Adaptive_EstimateVelPos (float_3D *Velocity, float_3D *Position, DCM dcm, float_3D Acce, float dt);
void Adaptive_ResetVelPos (float_3D *Velocity, float_3D *Position);
