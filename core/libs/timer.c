#include <stdio.h>
#include <string.h>
#include "timer.h"

#ifdef NOS_SOFTTIMER_EN
#define LOG(fmt, arg...) printf(fmt"\n", ##arg)

static soft_timer_t * tmr_list = NULL;

int timer_list_insert(soft_timer_t * tmr)
{
  if (NULL == tmr_list){
    tmr_list = tmr;
  } else {
    soft_timer_t * pre_tmr = tmr_list;
    soft_timer_t * cur_tmr = tmr_list->next;

    if (tmr_list->timeout_ticks >= tmr->timeout_ticks){
      tmr->next = tmr_list;
      tmr_list = tmr;
    } else {
      while(NULL != cur_tmr && cur_tmr->timeout_ticks < tmr->timeout_ticks){
	pre_tmr = cur_tmr;
	cur_tmr = cur_tmr->next;	
      }

      if (NULL != cur_tmr){
	tmr->next = cur_tmr;
	pre_tmr->next = tmr;
      } else {
	pre_tmr->next = tmr;
	tmr->next = NULL;
      }
    }
  }
  return 0;
}

int timer_list_remove(soft_timer_t * tmr)
{
  soft_timer_t * pre_tmr = tmr_list;
  soft_timer_t * cur_tmr = tmr_list;

  if (tmr == tmr_list){
    tmr_list = tmr_list->next;
    return 0;
  }

  cur_tmr = cur_tmr->next;
  while(NULL != cur_tmr){
    if (cur_tmr == tmr){
      pre_tmr->next = cur_tmr->next;
      return 0;
    } else {
      pre_tmr = cur_tmr;
      cur_tmr = cur_tmr->next;
    }
  }
  return 0;
}

//called by tick irq handler.
int timer_timeout_sched()
{
  soft_timer_t * cur_tmr = tmr_list;

  while(NULL != cur_tmr) {

    if (cur_tmr->running == TIMER_RUNNING){
      cur_tmr->timeout_ticks--;
    
      if (cur_tmr->timeout_ticks <= 0 && cur_tmr->cb){
        cur_tmr->cb(cur_tmr->arg);
      
        if (cur_tmr->mode == TIMER_MODE_REPEAT){
	  cur_tmr->timeout_ticks = cur_tmr->cycle_ticks;
        } else {
	  cur_tmr->running = TIMER_STOPPED;
        }
      }
    }

    cur_tmr = cur_tmr->next;      
  };
  return 0;
}

int timer_start(soft_timer_t * tmr)
{
  if (NULL == tmr)
    return -1;

  if (tmr->cycle_ticks <= 0 || NULL == tmr->cb)
    return -2;

  tmr->timeout_ticks += tmr->cycle_ticks;
  tmr->running = TIMER_RUNNING;
  timer_list_insert(tmr);
  return 0;
}

int timer_stop(soft_timer_t * tmr)
{
  if (NULL == tmr)
    return -1;

  timer_list_remove(tmr);
  tmr->running = TIMER_STOPPED;
  return 0;
}

int timer_init(soft_timer_t * tmr, int mode, int timeout_ms, callback cb)
{
  if (NULL == tmr || NULL == cb || timeout_ms <= 0)
    return -1;

  tmr->mode = mode;
  tmr->timeout_ticks = 0;
  tmr->cycle_ticks = TIMER_MS2TICK(timeout_ms);
  tmr->running = TIMER_STOPPED;
  tmr->cb = cb;
  tmr->next = NULL;

  return 0;
  
}

#endif /* NOS_SOFTTIMER_EN */
