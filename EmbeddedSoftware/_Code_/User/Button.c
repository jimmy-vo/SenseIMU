#include "Button.h"
#include "Timing_Manage.h"

typedef struct
{
	GPIO_TypeDef* 		PORT;
	uint32_t 					PIN;
	GPIOPuPd_TypeDef	PULL;
}GPIO_DEV;

GPIO_DEV GPIOLIST[2];



void Setup_BUTTON(GPIO_TypeDef* PORT, uint32_t GPIO_Pin, GPIOPuPd_TypeDef PULL)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	  
	/* Enable lock */
	if (PORT == GPIOA)	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA , ENABLE);
	if (PORT == GPIOB)	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
	if (PORT == GPIOC)	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);
	if (PORT == GPIOD)	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
	
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_PuPd  = PULL;
			GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin;
			GPIO_Init(PORT, &GPIO_InitStructure);
}

void Button_Initialize (void)
{
	uint8_t cnt;
	GPIOLIST[0].PORT =  GPIOA;
	GPIOLIST[0].PIN	 =  GPIO_Pin_6;
	GPIOLIST[0].PULL	 =  GPIO_PuPd_UP;
	GPIOLIST[1].PORT =  GPIOA;
	GPIOLIST[1].PIN	 =  GPIO_Pin_7;
	GPIOLIST[1].PULL	 =  GPIO_PuPd_UP;
	
	for(cnt = 0; cnt < sizeof (GPIOLIST); cnt ++)
		Setup_BUTTON(GPIOLIST[cnt].PORT, GPIOLIST[cnt].PIN, GPIOLIST[cnt].PULL);
		Delay_ms(1000);
}

///////////////////////////////////////////////////////////////////////////////////////////
// BUTTON
//
BOTTON Button_Check(uint8_t num)
{
	if ((!GPIO_ReadInputDataBit(GPIOLIST[num].PORT, GPIOLIST[num].PIN)) && (GPIOLIST[num].PULL == GPIO_PuPd_UP))
		return IsPressed;
	if ((GPIO_ReadInputDataBit(GPIOLIST[num].PORT, GPIOLIST[num].PIN)) &&  (GPIOLIST[num].PULL == GPIO_PuPd_DOWN))
		return IsPressed;
		
	return IsReleased;
}

void Button_ReadList (uint8_t * INPUT)
{
	uint8_t cnt;
	for (cnt = 0; cnt < 3; cnt ++)
		INPUT[cnt] = !GPIO_ReadInputDataBit(GPIOLIST[cnt].PORT, GPIOLIST[cnt].PIN);
}
