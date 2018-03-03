#include "Dev_TIMER.h"
#include "Timing_Manage.h"
#include "RGB.h"
#include "Serial_Link.h"

#define CRR_default 		0

IRQn_Type 														ITHandler;		
NVIC_InitTypeDef 											NVIC_InitStructure;
TIM_OCInitTypeDef											TIM_OCInitStructure;	
		
uint16_t Capture_1, Capture_2, Capture_3, Capture_4, Capture_5, Capture_6, Capture_7, 
				Capture_8, Capture_9, Capture_10, Capture_11;

void Initialize_TIMER (TIM_TypeDef* TIMx, uint16_t PERIOD, uint16_t Prescaler)
{
	TIM_TimeBaseInitTypeDef 							TIM_TimeBaseStructure;	
	
	if 			(TIMx == TIM1 )		
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
		SerialLink_Console(Highlevel, "TIM1 Config");
	}
	else if (TIMx == TIM2 )		
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
		SerialLink_Console(Highlevel, "TIM2 Config");
	}
	else if (TIMx == TIM3 )		
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
		SerialLink_Console(Highlevel, "TIM3 Config");
	}
	else if (TIMx == TIM4 )		
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
		SerialLink_Console(Highlevel, "TIM4 Config");
	}
	else if (TIMx == TIM5 )		
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
		SerialLink_Console(Highlevel, "TIM5 Config");
	}
	else if (TIMx == TIM6 )		
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);
		SerialLink_Console(Highlevel, "TIM6 Config");
	}
	else if (TIMx == TIM7 )		
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7 , ENABLE);
		SerialLink_Console(Highlevel, "TIM7 Config");
	}
	else if (TIMx == TIM8 )		
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 , ENABLE);
		SerialLink_Console(Highlevel, "TIM8 Config");
	}
	else if (TIMx == TIM9 )		
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9 , ENABLE);
		SerialLink_Console(Highlevel, "TIM9 Config");
	}
	else if (TIMx == TIM10)		
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
		SerialLink_Console(Highlevel, "TIM10 Config");
	}
	else if (TIMx == TIM11)		
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
		SerialLink_Console(Highlevel, "TIM11 Config");
	}
	else if (TIMx == TIM12)		
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
		SerialLink_Console(Highlevel, "TIM12 Config");
	}
	else if (TIMx == TIM13)		
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
		SerialLink_Console(Highlevel, "TIM13 Config");
	}
	else if (TIMx == TIM14)		
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
		SerialLink_Console(Highlevel, "TIM14 Config");
	}

	/* Time base configuration */		
	TIM_TimeBaseStructure.TIM_Period				 = PERIOD-1;
	TIM_TimeBaseStructure.TIM_Prescaler			 = Prescaler-1;
	TIM_TimeBaseStructure.TIM_ClockDivision	 = 0;
	TIM_TimeBaseStructure.TIM_CounterMode 	 = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
}


void Initialize_TIMER_PWM( TIM_TypeDef* TIMx, uint8_t TIM_Channel ,uint16_t CRR)
{
	
	TIM_OCInitTypeDef											TIM_OCInitStructure;	
	TIM_BDTRInitTypeDef 									TIM_BDTRInitStruct;
	
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode 				=	 	TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState		= 	TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState 	= 	TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse					= 	CRR;
	TIM_OCInitStructure.TIM_OCPolarity  	= 	TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity  	= 	TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState 	=		TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState	=		TIM_OCNIdleState_Reset;
		
	if ((TIM_Channel&TIMER_CH1) == TIMER_CH1)  	
	{
		TIM_OC1Init(TIMx, &TIM_OCInitStructure);
		SerialLink_Console(Lowlevel, "Channel 1: PWM mode");
	}
	if ((TIM_Channel&TIMER_CH2) == TIMER_CH2) 	 	
	{
		TIM_OC2Init(TIMx, &TIM_OCInitStructure);
		SerialLink_Console(Lowlevel, "Channel 2: PWM mode");
	}
	if ((TIM_Channel&TIMER_CH3) == TIMER_CH3) 	 	
	{
		TIM_OC3Init(TIMx, &TIM_OCInitStructure);
		SerialLink_Console(Lowlevel, "Channel 3: PWM mode");
	}
	if ((TIM_Channel&TIMER_CH4) == TIMER_CH4) 	 	
	{
		TIM_OC4Init(TIMx, &TIM_OCInitStructure);
		SerialLink_Console(Lowlevel, "Channel 4: PWM mode");
	}
	
	else if ((TIMx == TIM1 ) || (TIMx == TIM8 ))						//required for timers 1 or 8
		{
			TIM_BDTRStructInit(&TIM_BDTRInitStruct);
			TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
			TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
			TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Enable;
			TIM_BDTRConfig(TIMx, &TIM_BDTRInitStruct);
		}	
}

void Initialize_TIMER_INTERRUPT( TIM_TypeDef* TIMx, uint8_t TIM_Channel ,uint16_t CRR, uint8_t Preemption, uint8_t Sub)
{				
	if 			(TIMx == TIM1 )		ITHandler = TIM1_CC_IRQn;
	else if (TIMx == TIM2 )		ITHandler = TIM2_IRQn;
	else if (TIMx == TIM3 )		ITHandler = TIM3_IRQn;
	else if (TIMx == TIM4 )		ITHandler = TIM4_IRQn;
	else if (TIMx == TIM5 )		ITHandler = TIM5_IRQn;
	else if (TIMx == TIM6 )		ITHandler = TIM6_DAC_IRQn;
	else if (TIMx == TIM7 )		ITHandler = TIM7_IRQn;
	else if (TIMx == TIM8 )		ITHandler = TIM8_CC_IRQn;
	else if (TIMx == TIM8 )		ITHandler = TIM8_CC_IRQn;
	else if (TIMx == TIM9 )		ITHandler = TIM1_BRK_TIM9_IRQn;
	else if (TIMx == TIM10 )	ITHandler = TIM1_UP_TIM10_IRQn;
	else if (TIMx == TIM11 )	ITHandler = TIM1_TRG_COM_TIM11_IRQn;
	
	/* Enable the TIM gloabal Interrupt */
	NVIC_PriorityGroupConfig(NVIC_GROUP_DEFAULT);
	
	NVIC_InitStructure.NVIC_IRQChannel =  ITHandler	;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Preemption;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = Sub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
			
	TIM_OCInitStructure.TIM_OCMode 				= TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity 		= TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse 				= CRR;
	
			
	if ((TIM_Channel&TIMER_CH1) == TIMER_CH1)  			/* Output Compare Timing Mode configuration: Channel1 */
		{
			TIM_OC1Init(TIMx, &TIM_OCInitStructure);
			TIM_ITConfig(TIMx, TIM_IT_CC1 , ENABLE);
			SerialLink_Console(Lowlevel, "Channel 1: Output Compare mode");
		}		
	if ((TIM_Channel&TIMER_CH2) == TIMER_CH2) 			/* Output Compare Timing Mode configuration: Channel2 */
		{
			TIM_OC2Init(TIMx, &TIM_OCInitStructure);
			TIM_ITConfig(TIMx, TIM_IT_CC2 , ENABLE);
			SerialLink_Console(Lowlevel, "Channel 2: Output Compare mode");
		}
	
	if ((TIM_Channel&TIMER_CH3) == TIMER_CH3) 			/* Output Compare Timing Mode configuration: Channel3 */
		{
			TIM_OC3Init(TIMx, &TIM_OCInitStructure);
			TIM_ITConfig(TIMx, TIM_IT_CC3 , ENABLE);
			SerialLink_Console(Lowlevel, "Channel 3: Output Compare mode");
		}
	if ((TIM_Channel&TIMER_CH4) == TIMER_CH4) 			/* Output Compare Timing Mode configuration: Channel4 */
		{
			TIM_OC4Init(TIMx, &TIM_OCInitStructure);
			TIM_ITConfig(TIMx, TIM_IT_CC4 , ENABLE);
			SerialLink_Console(Lowlevel, "Channel 4: Output Compare mode");
		}	
}




void TIM1_CC_IRQHandler(void)
{	
}

void TIM2_IRQHandler(void)
{

}

void TIM3_IRQHandler(void)
{
	
}

void TIM4_IRQHandler(void)
{
	
}

void TIM5_IRQHandler(void)
{
	
}

void TIM6_DAC_IRQHandler(void)
{
	
}

void TIM7_IRQHandler(void)
{
	
}
void TIM8_CC_IRQHandler(void)
{
	
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
	
}
void TIM1_UP_TIM10_IRQHandler(void)
{		
	if (TIM_GetITStatus(TIM10, TIM_IT_CC1) != RESET)			
		{
			TIM_ClearITPendingBit(TIM10, TIM_IT_CC1);
			////////////////////////////////////////////////	
				
			////////////////////////////////////////////////	
			Capture_10 = TIM_GetCapture1(TIM10);
			TIM_SetCompare1(TIM10, Capture_10 + EKFQUA_CCR1);
		}
}
void TIM1_TRG_COM_TIM11_IRQHandler(void)
{	
	if (TIM_GetITStatus(TIM11, TIM_IT_CC1) != RESET)			
		{
			TIM_ClearITPendingBit(TIM11, TIM_IT_CC1);
			////////////////////////////////////////////////	
				RGB_Process();
			////////////////////////////////////////////////	
			Capture_11 = TIM_GetCapture1(TIM11);
			TIM_SetCompare1(TIM11, Capture_11 + RGBProcess_CCR);
		}
}
