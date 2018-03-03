#include "BoardConfig.h"



typedef enum
{
	ALGORITHM_TiltComp 	= 4,
	ALGORITHM_GyroInteg = 5,
	ALGORITHM_Comple 		= 6,
	ALGORITHM_AHRS 	  	= 7,
	ALGORITHM_DCMkf 		= 8,
	ALGORITHM_DCMakf 		= 9,
}ALGORITHM;


typedef enum 
{
		ALGORITHM_OK											,
    ALGORITHM_SensorInit 			 				,
    ALGORITHM_AlgoInit 			 					,
    ALGORITHM_ReadError 			 					,
    ALGORITHM_DTError 			 					,
    ALGORITHM_FAIL 			 							,
}ALGORITHM_Status;


#define Algorithm_errordt   1.2f

void Algorithm_Switch (ALGORITHM algo);
ALGORITHM_Status Algorithm_Orientiation (void);
void Algorithm_Reset (void);
