#ifndef __HW_TIMER_H__
#define __HW_TIMER_H__
#include "drv_timer.h"
#include "timer.h"

#define APP_TIMER_PERSCALER          0
#define APP_TIMER_QSIZE              3


void hal_timer_init(void);
void hal_timer_start(void);
#endif /* __HW_TIMER_H__ */
