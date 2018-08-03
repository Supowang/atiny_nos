#ifndef __DRV_TIMER_H__
#define __DRV_TIMER_H__
#include "stm32f4xx.h"

//TIM gernel

#define GENERAL_TIM           		TIM2
#define GENERAL_TIM_CLK       		RCC_APB1Periph_TIM2

#define GENERAL_TIM_IRQn			TIM2_IRQn
#define GENERAL_TIM_IRQHandler    	TIM2_IRQHandler

typedef int(*timer_sched_handler)(void);

void TIMx_Configuration(void);
int register_timx_handler(timer_sched_handler h);
#endif /* __DRV_TIMER_H__ */
