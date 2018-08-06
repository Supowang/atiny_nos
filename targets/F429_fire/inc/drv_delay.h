#ifndef __DRV_DELAY_H__
#define __DRV_DELAY_H__
#include "stm32f4xx.h"

#define OS_SYS_TICK_PER_SECOND  1000

void sys_tick_init(void);
void drv_delay_ms(unsigned int ms);

#endif
