#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "agent_core_config.h"
#ifdef NOS_QUEUE_EN

typedef struct _queue {
  int queue_size;
  int item_size;
  unsigned char * pool;
  int r_idx;
  int w_idx;
}queue_t;

#define QUEUE_DEFS(name, q_size, i_size) \
  static unsigned char queue_##name##_buf[q_size][i_size] = {0}; \
  static queue_t name = { \
      .queue_size = q_size, \
      .item_size = i_size, \
      .pool = (unsigned char *)queue_##name##_buf,	\
      .r_idx = 0, \
      .w_idx = 0, \
   }

int queue_get_item_count(queue_t *q);
int queue_put(queue_t * q, void * item);
int queue_put_item(queue_t * q, void * item, int i_size);
int queue_get(queue_t * q, void * item);
int queue_get_item(queue_t * q, void * item, int * i_size);
#endif /* NOS_QUEUE_EN */

#endif /* __QUEUE_H__ */
