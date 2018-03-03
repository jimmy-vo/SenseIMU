#include "RGB.h"
#include "stdio.h"
#include "Timing_Manage.h"
#include "Dev_TIMER.h"
#include "Dev_USART.h"
#include "Serial_Link.h"

static RGB_typedef RGB_LOOP;
static RGB_Type RGB_FLASH = RGB_OFF;

#define RGB_3 							100
#define RGB_2  							50
#define RGB_1  							0

static uint8_t RGB_MATRIX_Circle [RGB_SIZE] = 
{		
	RGB_3,RGB_3, RGB_3, RGB_2, RGB_1,RGB_1, RGB_1, RGB_1,RGB_1,RGB_2, RGB_3, RGB_2,
	RGB_1,RGB_2, RGB_3, RGB_3, RGB_3,RGB_3, RGB_3, RGB_2,RGB_1,RGB_1, RGB_1, RGB_1, 
	RGB_1,RGB_1, RGB_1, RGB_1, RGB_1,RGB_2, RGB_3, RGB_2,RGB_3,RGB_3, RGB_3, RGB_2,
};
static uint8_t RGB_MATRIX_WHITE [2*3] =
{	
	RGB_3, RGB_1, 
	RGB_3, RGB_1, 
	RGB_3, RGB_1, 
};
static uint8_t RGB_MATRIX_RED [2*3] =
{	
	RGB_3, RGB_1, 
	RGB_1, RGB_1, 
	RGB_1, RGB_1, 
};
static uint8_t RGB_MATRIX_GREEN [2*3] =
{	
	RGB_1, RGB_1, 
	RGB_3, RGB_1, 
	RGB_1, RGB_1, 
};
static uint8_t RGB_MATRIX_BLUE [2*3] =
{	
	RGB_1, RGB_1, 
	RGB_1, RGB_1, 
	RGB_3, RGB_1, 
};

void RGB_Flash(RGB_Type type)
{
	RGB_FLASH = type;
}

void RGB_Set(RGB_Type type)
{
	int i;
	switch (type)  
	{
		case RGB_Circle:
									RGB_LOOP.brightness	= 3;
									RGB_LOOP.speed				=	5;			
									RGB_LOOP.Size 				= RGB_SIZE/3;
									for (i=0; i<RGB_LOOP.Size*3 ; i++)					RGB_LOOP.Type[i] = RGB_MATRIX_Circle[i]/RGB_LOOP.speed;		
									break;									
		case RGB_OFF:									
									RGB_LOOP.brightness	=	0;
									break;
		case RGB_RED_T:	
									RGB_LOOP.brightness	=	10;
									RGB_LOOP.speed				=	100;	
									RGB_LOOP.Size 				= 2;
									for (i=0; i<RGB_LOOP.Size*3 ; i++)					RGB_LOOP.Type[i] = RGB_MATRIX_RED[i]/RGB_LOOP.speed;	
									break;
		case RGB_GREEN_T:	
									RGB_LOOP.brightness	=	10;
									RGB_LOOP.speed				=	12;	
									RGB_LOOP.Size 				= 2;
									for (i=0; i<RGB_LOOP.Size*3 ; i++)					RGB_LOOP.Type[i] = RGB_MATRIX_GREEN[i]/RGB_LOOP.speed;	
									break;									
		case RGB_BLUE_T:
									RGB_LOOP.brightness	=	10;
									RGB_LOOP.speed				=	12;	
									RGB_LOOP.Size 				= 2;
									for (i=0; i<RGB_LOOP.Size*3 ; i++)					RGB_LOOP.Type[i] = RGB_MATRIX_BLUE[i]/RGB_LOOP.speed;		
									break;
		case RGB_WHITE_T:
									RGB_LOOP.brightness	=	10;
									RGB_LOOP.speed				=	12;	
									RGB_LOOP.Size 				= 2;
									for (i=0; i<RGB_LOOP.Size*3 ; i++)					RGB_LOOP.Type[i] = RGB_MATRIX_WHITE[i]/RGB_LOOP.speed;	
									break;
		case RGB_YELLOW_T:
									RGB_LOOP.brightness	=	10;
									RGB_LOOP.speed				=	12;		
									RGB_LOOP.Size 				= 2;
									for (i=0; i<RGB_LOOP.Size*3 ; i++)					RGB_LOOP.Type[i] = (RGB_MATRIX_RED[i]+RGB_MATRIX_GREEN[i])/RGB_LOOP.speed;	
									break;	
		case RGB_MAGENTA_T:
									RGB_LOOP.brightness	=	10;
									RGB_LOOP.speed				=	12;		
									RGB_LOOP.Size 				= 2;
									for (i=0; i<RGB_LOOP.Size*3 ; i++)					RGB_LOOP.Type[i] = (RGB_MATRIX_RED[i]+RGB_MATRIX_BLUE[i])/RGB_LOOP.speed;	
									break;
		case RGB_CYAN_T:
									RGB_LOOP.brightness	=	10;
									RGB_LOOP.speed				=	12;		
									RGB_LOOP.Size 				= 2;
									for (i=0; i<RGB_LOOP.Size*3 ; i++)					RGB_LOOP.Type[i] = (RGB_MATRIX_GREEN[i]+RGB_MATRIX_BLUE[i])/RGB_LOOP.speed;	
									break;
		default:			break;
	}
	RGB_LOOP.Scale = (RGB_PERIOD * (float) RGB_LOOP.brightness /100 * RGB_LOOP.speed);	
}		

void RGB_Update ( int8_t RED, int8_t GREEN, int8_t BLUE)
{	
		RED_PERIOD_UPDATE		(RGB_TIM,	(uint16_t)	(	RED		));
		GREEN_PERIOD_UPDATE	(RGB_TIM,	(uint16_t)	(	GREEN	));
		BLUE_PERIOD_UPDATE	(RGB_TIM, (uint16_t)	(	BLUE	));
}

void RGB_Process(void)
{
		RGB_LOOP.Check=0;	
		if 	(RGB_LOOP.cnt >= RGB_LOOP.Size)  					
		{
			RGB_LOOP.cnt = 0;	
			RGB_LOOP.RED.Location = 0;
			RGB_LOOP.GREEN.Location = RGB_LOOP.Size;
			RGB_LOOP.BLUE.Location = RGB_LOOP.Size*2;
		}
		
		if (RGB_LOOP.GREEN.Current == RGB_LOOP.Type[RGB_LOOP.GREEN.Location])  RGB_LOOP.Check++;
		else
				{
					if 			(RGB_LOOP.GREEN.Current	<	RGB_LOOP.Type[RGB_LOOP.GREEN.Location]) RGB_LOOP.GREEN.Current++;
					else if	(RGB_LOOP.GREEN.Current	>	RGB_LOOP.Type[RGB_LOOP.GREEN.Location]) RGB_LOOP.GREEN.Current--;
				}
		if (RGB_LOOP.RED.Current == RGB_LOOP.Type[RGB_LOOP.RED.Location])	 RGB_LOOP.Check++;
		else
				{
					if 			(RGB_LOOP.RED.Current	<	RGB_LOOP.Type[RGB_LOOP.RED.Location]) RGB_LOOP.RED.Current ++;
					else if	(RGB_LOOP.RED.Current	>	RGB_LOOP.Type[RGB_LOOP.RED.Location]) RGB_LOOP.RED.Current--;
				}		
		if (RGB_LOOP.BLUE.Current == RGB_LOOP.Type[RGB_LOOP.BLUE.Location])  RGB_LOOP.Check++;
		else
				{
					if 			(RGB_LOOP.BLUE.Current	<	RGB_LOOP.Type[RGB_LOOP.BLUE.Location]) RGB_LOOP.BLUE.Current++;
					else if	(RGB_LOOP.BLUE.Current	>	RGB_LOOP.Type[RGB_LOOP.BLUE.Location]) RGB_LOOP.BLUE.Current--;
				}				
				
		if (RGB_LOOP.Check>=3)  
		{
			RGB_LOOP.cnt++;
			RGB_LOOP.GREEN.Location++;
			RGB_LOOP.RED.Location++;
			RGB_LOOP.BLUE.Location++;
		}
		switch (RGB_FLASH)
		{
			case RGB_RED:
				RED_PERIOD_UPDATE		(RGB_TIM,	(uint16_t)	(RGB_PERIOD));
				GREEN_PERIOD_UPDATE	(RGB_TIM,	(uint16_t)	(0));
				BLUE_PERIOD_UPDATE	(RGB_TIM, (uint16_t)	(0));
				RGB_LOOP.RED.Current = 50;
				RGB_LOOP.GREEN.Current = 0;
				RGB_LOOP.BLUE.Current = 0;
				RGB_FLASH = RGB_OFF;
				break;
			case RGB_GREEN:
				RED_PERIOD_UPDATE		(RGB_TIM,	(uint16_t)	(0));
				GREEN_PERIOD_UPDATE	(RGB_TIM,	(uint16_t)	(RGB_PERIOD));
				BLUE_PERIOD_UPDATE	(RGB_TIM, (uint16_t)	(0));
				RGB_LOOP.RED.Current = 0;
				RGB_LOOP.GREEN.Current = 50;
				RGB_LOOP.BLUE.Current = 0;
				RGB_FLASH = RGB_OFF;
				break;
			case RGB_BLUE:
				RED_PERIOD_UPDATE		(RGB_TIM,	(uint16_t)	(0));
				GREEN_PERIOD_UPDATE	(RGB_TIM,	(uint16_t)	(0));
				BLUE_PERIOD_UPDATE	(RGB_TIM, (uint16_t)	(RGB_PERIOD));
				RGB_LOOP.RED.Current = 0;
				RGB_LOOP.GREEN.Current = 0;
				RGB_LOOP.BLUE.Current = 50;
				RGB_FLASH = RGB_OFF;
				break;
			case RGB_WHITE:
				RED_PERIOD_UPDATE		(RGB_TIM,	(uint16_t)	(RGB_PERIOD));
				GREEN_PERIOD_UPDATE	(RGB_TIM,	(uint16_t)	(RGB_PERIOD));
				BLUE_PERIOD_UPDATE	(RGB_TIM, (uint16_t)	(RGB_PERIOD));
				RGB_LOOP.RED.Current = 50;
				RGB_LOOP.GREEN.Current = 50;
				RGB_LOOP.BLUE.Current = 50;
				RGB_FLASH = RGB_OFF;
				break;
			default:
				RED_PERIOD_UPDATE		(RGB_TIM,	(uint16_t)	(RGB_LOOP.Scale	*	RGB_LOOP.RED.Current		/	100));
				GREEN_PERIOD_UPDATE	(RGB_TIM,	(uint16_t)	(RGB_LOOP.Scale	*	RGB_LOOP.GREEN.Current	/	100));
				BLUE_PERIOD_UPDATE	(RGB_TIM, (uint16_t)	(RGB_LOOP.Scale	*	RGB_LOOP.BLUE.Current	/	100));
				break;
		}
}


void RGB_Initialize (void)
{
	GPIO_InitTypeDef 											GPIO_InitStructure;
	
	/* clock enable */
	RCC_AHB1PeriphClockCmd(RGB_GPIO_CLK, ENABLE);
	
	/* GPIO Configuration */
	GPIO_InitStructure.GPIO_Pin 		= RGB_PIN;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 		= GPIO_PuPd_NOPULL ;
	GPIO_Init(RGB_PORT, &GPIO_InitStructure); 

	/* Connect TIM pins to AF */  
	GPIO_PinAFConfig(GREEN_PORT	, GREEN_PinSource	, GREEN_GPIO_AF);
	GPIO_PinAFConfig(RED_PORT		, RED_PinSource		, RED_GPIO_AF); 
	GPIO_PinAFConfig(BLUE_PORT	, BLUE_PinSource	, BLUE_GPIO_AF);

	Initialize_TIMER						(RGB_TIM, RGB_PERIOD, RGB_PRESCALER);
	Initialize_TIMER_PWM				(RGB_TIM,  TIMER_CH1 | TIMER_CH2 | TIMER_CH3 , RGB_CCR);	
	TIM_Cmd(RGB_TIM, ENABLE);	
	
}
