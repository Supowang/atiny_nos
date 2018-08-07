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

#ifdef WITH_DTLS
#define BLOCK3_COUNT 8
#define BLOCK3_SIZE  512

#define BLOCK4_COUNT 2
#define BLOCK4_SIZE  1024

#define BLOCK5_COUNT 2
#define BLOCK5_SIZE  0x2000
#else
#define BLOCK3_COUNT 3
#define BLOCK3_SIZE  1024
#endif

#endif
//timer
#define NOS_SOFTTIMER_EN
#ifdef NOS_SOFTTIMER_EN

#define OS_SYS_TICK_PER_SECOND 1000

#endif

//queue
#define NOS_QUEUE_EN

//debug config
#define DEBUG_UART_ON


#endif /* __CONFIG_H__ */
