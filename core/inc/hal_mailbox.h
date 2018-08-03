#ifndef __HW_MAILBOX_H__
#define __HW_MAILBOX_H__

#include "queue.h"

void hal_mailbox_init(queue_t * mb);
unsigned int hal_mb_put(void * item);
unsigned int hal_mb_get(void * item);

#endif /* __HW_MAILBOX_H__ */
