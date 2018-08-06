#include <stdio.h>
#include <string.h>
#include "drv_delay.h"

unsigned long long g_kerneltickscount = 0;
unsigned long long timing_delay_tick = 0;
void sys_tick_init()
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if (SysTick_Config(SystemCoreClock / OS_SYS_TICK_PER_SECOND))
	{ 
		/* Capture error */ 
		while (1);
	}
}

void SysTick_Handler(void)
{
	g_kerneltickscount ++;
	if (timing_delay_tick) 
		timing_delay_tick--;
}

void drv_delay_ms(unsigned int ms)
{
	timing_delay_tick = ms;
	while(timing_delay_tick);
}
