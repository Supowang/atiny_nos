#ifndef __MEM_MANAGER_H__
#define __MEM_MANAGER_H__

#include "agent_core_config.h"
#ifdef NOS_MEM_MANAGER_EN
#ifndef BLOCK0_COUNT
#define BLOCK0_COUNT 0
#define BLOCK0_SIZE  0
#endif

#ifndef BLOCK1_COUNT
#define BLOCK1_COUNT 0
#define BLOCK1_SIZE  0
#endif

#ifndef BLOCK2_COUNT
#define BLOCK2_COUNT 0
#define BLCOK2_SIZE  0
#endif

#ifndef BLOCK3_COUNT
#define BLOCK3_COUNT 0
#define BLOCK3_SIZE  0
#endif

#ifndef BLOCK4_COUNT
#define BLOCK4_COUNT 0
#define BLOCK4_SIZE  0
#endif

#ifndef BLOCK5_COUNT
#define BLOCK5_COUNT 0
#define BLOCK5_SIZE  0
#endif

#ifndef BLOCK6_COUNT
#define BLOCK6_COUNT 0
#define BLOCK6_SIZE  0
#endif

//functions
void mem_manager_init(void);
void * mem_malloc(int size);
void mem_free(void *p);

#endif /* NOS_MEM_MANAGER_EN */

#endif /* __MEM_MANAGER_H__ */
