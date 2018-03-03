#include "BoardConfig.h"


#define	TIMER_CH1  1
#define	TIMER_CH2  2
#define	TIMER_CH3  4
#define	TIMER_CH4  8


void Initialize_TIMER (TIM_TypeDef* TIMx, uint16_t PERIOD, uint16_t Prescaler);

void Initialize_TIMER_INTERRUPT( TIM_TypeDef* TIMx, uint8_t TIM_Channel ,uint16_t CRR, uint8_t Preemption, uint8_t Sub);
void Initialize_TIMER_PWM( TIM_TypeDef* TIMx, uint8_t channel ,uint16_t CRR);
