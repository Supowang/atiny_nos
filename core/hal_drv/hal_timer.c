#include <stdio.h>
#include <string.h>
#include "hal_timer.h"

static soft_timer_t t1, t2;
void app_data_report(void);

static int data_report_handler(void * p_context)
{   
    app_data_report();

    return 0;
}
static int atiny_step_loop(void * p_context)
{
    return 0;
}

void hal_timer_init()
{
    TIMx_Configuration();
    #ifdef NOS_SOFTTIMER_EN
    register_timx_handler(timer_timeout_sched);
    timer_init(&t1, TIMER_MODE_REPEAT, 2000, data_report_handler, NULL);
 //   timer_init(&t2, TIMER_MODE_REPEAT, 5000, atiny_step_loop, NULL);
    #else
    
    #endif
}

void hal_timer_start()
{
    #ifdef NOS_SOFTTIMER_EN
    timer_start(&t1);
 //   timer_start(&t2);
    #else
    
    #endif
}
