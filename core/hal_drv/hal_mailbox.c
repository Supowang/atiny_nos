#include <stdio.h>
#include "queue.h"

static queue_t * gp_mb;

void hal_mailbox_init(void * mb)
{
    #ifdef NOS_QUEUE_EN
    gp_mb = (queue_t * )mb;
    #else
    
    #endif
}

unsigned int hal_mb_put(void * item)
{
    #ifdef NOS_QUEUE_EN
    return queue_put(gp_mb, item);
    #else
    
    #endif
}

unsigned int hal_mb_get(void * item)
{
    #ifdef NOS_QUEUE_EN
    return queue_get(gp_mb, item);
    #else
    
    #endif
}
