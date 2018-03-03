#include "BoardConfig.h"

#define Vreff 									3.3f
#define Rtop										10.0f				
#define Rbot										3.9f
#define	Resolution							4096

#define	VF_SlopeScale						1.01f
#define	VF_SlopeShift						0.0f

#define ADCtoVolt							 (Vreff*(Rtop+Rbot)/(Resolution*Rbot)) * VF_SlopeScale

#define VOlT_COM 0.999999999999f
///////////////////////////////////////////////////////////////

void Voltage_FB_Initiallize(void);
float Get_Voltage(void);
