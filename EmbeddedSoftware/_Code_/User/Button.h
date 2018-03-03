#include "BoardConfig.h"


typedef enum
{
	IsPressed,
	IsReleased
}BOTTON;

BOTTON Button_Check(uint8_t num);
void Button_Initialize (void);
void INPUT_ReadList (uint8_t * INPUT);
