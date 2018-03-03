#include "BoardConfig.h"

#define EKFQUA_TIM																		TIM10
#define EKFQUA_Period																0xffff 				
#define EKFQUA_Prescaler															16800
//#define EKFQUA_CCR1																	40						//min 4	 = max 2.5KHZ
#define EKFQUA_CCR1																	10000						//min 4	 = max 2.5KHZ


#define RGBProcess_TIM																	TIM11
#define RGBProcess_Period																0xffff 				
#define RGBProcess_Prescaler														1680
#define RGBProcess_CCR																	3000						//min 4	 = max 2.5KHZ
////////////////////////////////////////////////////////////////////////////////
				#define FREQ_100kHz 10
				#define FREQ_30kHz  33
				#define FREQ_20kHz  50
				#define FREQ_10kHz  100
				#define FREQ_1kHz   1000
				#define FREQ_500Hz  2000
				#define FREQ_200Hz  5000

				#define FREQ_100Hz  10000
				#define FREQ_75Hz   15000
				#define FREQ_50Hz   20000
				#define FREQ_20Hz   50000
				#define FREQ_10Hz   100000
				#define FREQ_1Hz		1000000
////////////////////////////////////////////////////////////////////////////////


void Initialize_Timer(void);

void Delay_ms (uint32_t ms);
void Initialize_SystemTick (void );
uint64_t 	Get_msTick(void);
uint64_t Get_usTick(void);
uint8_t 	Check_msTick(uint64_t TickBase, uint64_t Time);
