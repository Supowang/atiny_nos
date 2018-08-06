#ifndef __CONFIG_H__
#define __CONFIG_H__

//memory manager
#define  NOS_MEM_MANAGER_EN

#ifdef NOS_MEM_MANAGER_EN
#define BLOCK0_COUNT 32
#define BLOCK0_SIZE  8

#define BLOCK1_COUNT 32
#define BLOCK1_SIZE  32

#define BLOCK2_COUNT 16
#define BLOCK2_SIZE  128

#define BLOCK3_COUNT 3
#define BLOCK3_SIZE  1024
#endif

//timer
#define NOS_SOFTTIMER_EN
#ifdef NOS_SOFTTIMER_EN

#define OS_SYS_TICK_PER_SECOND 1000

#endif

//queue
#define NOS_QUEUE_EN

#endif /* __CONFIG_H__ */