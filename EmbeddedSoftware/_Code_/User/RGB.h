#include "BoardConfig.h"

#define RGB_SIZE		12*3


#define RGB_CCR										0
#define RGB_PRESCALER							168
#define RGB_PERIOD								2000


typedef struct 
{
	uint8_t Current;
	uint8_t Location;
}LED_typedef;

typedef struct 
{
	uint8_t 			Type[RGB_SIZE];	
	uint8_t 			Size;
	uint8_t 			cnt;
	uint8_t 			Check;
	uint8_t 			brightness;
	uint8_t 			speed;
	float					Scale;
	LED_typedef		RED;
	LED_typedef		GREEN;
	LED_typedef		BLUE;
}RGB_typedef;

typedef enum
{
	RGB_OFF,
	RGB_Circle,	
	RGB_RED,
	RGB_GREEN,
	RGB_BLUE,
	RGB_WHITE,
	RGB_RED_T,
	RGB_GREEN_T,
	RGB_BLUE_T,
	RGB_YELLOW_T,
	RGB_MAGENTA_T,
	RGB_CYAN_T,
	RGB_WHITE_T,
}RGB_Type;

void RGB_Initialize (void);
void RGB_Flash(RGB_Type type);
void Initialize_DIGITAL (void);
void RGB_Process(void);
void RGB_Set(RGB_Type type);
void RGB_Update ( int8_t RED, int8_t GREEN, int8_t BLUE);
