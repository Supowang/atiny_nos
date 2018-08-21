#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#ifdef NOS_QUEUE_EN

#define LOG(fmt, arg...) printf(fmt"\n", ##arg)

int queue_get_item_count(queue_t *q)
{
  int count = 0;
  if (NULL == q)
    return -1;

  if (q->r_idx == q->w_idx){
    count = 0;
  } else if (q->r_idx < q->w_idx) {
    count = q->w_idx - q->r_idx;
  } else {
    count = q->item_size - q->w_idx + (q->r_idx + 1);
  }
  return count;
}

int queue_put(queue_t * q, void * item)
{
  return queue_put_item(q, item, q->item_size);
}

int queue_put_item(queue_t * q, void * item, int i_size)
{
  int count = 0;
  
  if (NULL == q || NULL == item || i_size <= 0)
    return -1;

  count = queue_get_item_count(q);
  if (count < 0 || count >= (q->item_size - 1))
    return 0;

  memcpy(q->pool + q->w_idx * q->item_size, item, i_size);

  q->w_idx ++;
  if (q->w_idx >= q->queue_size){
    q->w_idx = 0;
  } 
  return 0;
}

int queue_get(queue_t * q, void * item)
{
  int size;
  return queue_get_item(q, item, &size);
}

int queue_get_item(queue_t *q, void * item, int * i_size)
{
  int count = 0;
  if (NULL == q || NULL == item || NULL == i_size)
    return -1;

  count = queue_get_item_count(q);
  if (count <= 0)
    return 0;

  *i_size = q->item_size;
  memcpy(item, q->pool + q->item_size * q->r_idx, q->item_size);

  q->r_idx ++;
  if (q->r_idx >= q->queue_size)
    q->r_idx = 0;

  return 0;
}

#endif /* NOS_QUEUE_EN */
