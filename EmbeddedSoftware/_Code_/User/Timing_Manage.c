#include "Timing_Manage.h"
#include "Dev_TIMER.h"
#include "Serial_Link.h"
#include <stdio.h>
#include "RGB.h"

__IO uint64_t TickCnt = 0;
uint64_t cnt;


uint32_t capture = 0;
uint32_t us_Tick;			
RCC_ClocksTypeDef RCC_Clocks;

void Initialize_Timer(void)
	{				
		SerialLink_Console(Event, "Get Current Clock.");
		RCC_GetClocksFreq(&RCC_Clocks);	
		SerialLink_Console(EventInfo, "SYSCLK %d MHz.", (uint32_t) RCC_Clocks.SYSCLK_Frequency/1000000);
		SerialLink_Console(EventInfo, "HCLK   %d MHz.", (uint32_t) RCC_Clocks.HCLK_Frequency/1000000);
		SerialLink_Console(EventInfo, "PCLK1  %d MHz.", (uint32_t) RCC_Clocks.PCLK1_Frequency/1000000);
		SerialLink_Console(EventInfo, "PCLK2  %d MHz.", (uint32_t) RCC_Clocks.PCLK2_Frequency/1000000);
		
		SerialLink_Console(Event, "Initialize Timer...");		
	
		Initialize_TIMER 						(RGBProcess_TIM, RGBProcess_Period,	RGBProcess_Prescaler);
		Initialize_TIMER_INTERRUPT	(RGBProcess_TIM, TIMER_CH1 , RGBProcess_CCR, 0, 0);		
		TIM_Cmd(RGBProcess_TIM, ENABLE);		
		
		SerialLink_Console(EventInfo, "Set up Systick at 1000 KHz.");
		SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 1000000); 	
	}	
	
	
void Delay_ms (uint32_t ms)
{
	uint64_t CurTick = Get_msTick();
	while(Check_msTick(CurTick,ms)==0);
}
		
uint64_t Get_msTick(void)
{
    return us_Tick/1000;
}

uint64_t Get_usTick(void)
{
    return us_Tick;
}

uint8_t Check_msTick(uint64_t TickBase, uint64_t Time)
{
    uint64_t CurTick;
				
    CurTick = Get_msTick();
    
    if (CurTick > TickBase)
    {
        if (CurTick >= (TickBase + Time))
        {
            return 1;
        }
    }
    else
    {
        if (CurTick >= ((uint64_t)(TickBase + Time)))
        {
            return 1;
        }
    }
    return 0;
}	

	
void SysTick_Handler()
{	
	us_Tick++;
}

