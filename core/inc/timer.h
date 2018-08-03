#ifndef __TIMER_H__
#define __TIMER_H__

#include "config.h"
#ifdef NOS_SOFTTIMER_EN

#define TIMER_STOPPED  0
#define TIMER_RUNNING  1

#define TIMER_MODE_SINGLE  0
#define TIMER_MODE_REPEAT  1

#define TIMER_MS2TICK(ms)    ((ms * OS_SYS_TICK_PER_SECOND) / 1000)

typedef int (*callback)(void * arg);

typedef struct _timer {
  struct _timer *next;
  char running;
  char mode; //single or repeat
  int timeout_ticks;
  int cycle_ticks;
  callback cb;
  void * arg;
}soft_timer_t;

int timer_timeout_sched(void);
int timer_start(soft_timer_t *tmr);
int timer_stop(soft_timer_t *tmr);
int timer_init(soft_timer_t * tmr, int mode, int timeout_ms, callback cb);

#endif /* NOS_SOFTTIMER */
#endif /* __TIMER_H__ */
