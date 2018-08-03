#include <stdio.h>
#include <string.h>
#include "drv_timer.h"

timer_sched_handler gf_sched_h = NULL;

void TIMx_Configuration(void);
static void TIM_Mode_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  //open TIMx_clk x[6.7]
  RCC_APB1PeriphClockCmd(GENERAL_TIM_CLK, ENABLE); 

  //increase to period and generate an update or interrupt
  TIM_TimeBaseStructure.TIM_Period = 10-1;       
	
  //genernal timer clk source TIMxCLK = HCLK / 2 = 90 MHz
  // Timer frequence=TIMxCLK/(TIM_Prescaler+1)=10000Hz
  TIM_TimeBaseStructure.TIM_Prescaler = 9000-1;	
  // 
  TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
  // counter mode
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	
	// init timer TIMx, x[1,8]
	TIM_TimeBaseInit(GENERAL_TIM, &TIM_TimeBaseStructure);
	
	// clear flag
	TIM_ClearFlag(GENERAL_TIM, TIM_FLAG_Update);
	
	// open interrupt
	TIM_ITConfig(GENERAL_TIM,TIM_IT_Update,ENABLE);
	
	// enable
	TIM_Cmd(GENERAL_TIM, ENABLE);	
}

static void TIMx_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // set interrupt group
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// set imterrupt source
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQn; 	
		//
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  //
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void TIMx_Configuration(void)
{
  TIMx_NVIC_Configuration();
  
  TIM_Mode_Config();
}

void  GENERAL_TIM_IRQHandler (void)
{
	if ( TIM_GetITStatus( GENERAL_TIM, TIM_IT_Update) != RESET ) 
	{	
        if (NULL != gf_sched_h)gf_sched_h();
		TIM_ClearITPendingBit(GENERAL_TIM , TIM_IT_Update);  		 
	}
}
int register_timx_handler(timer_sched_handler h)
{
	gf_sched_h = h;
    return 0;
}
