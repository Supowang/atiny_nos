#ifndef __HW_UART_H__
#define __HW_UART_H__

#include "drv_uart.h"
#include "drv_debug_uart.h"

#if 0
#define UART_DEBUG
#else
#define UART_MODULE
#endif

#ifdef UART_DEBUG
#define UART_TX         9
#define UART_RX         11
#define UART_CTS        10
#define UART_RTS        8
#define UART_HWFC       false
#endif

#ifdef UART_MODULE
#define UART_TX         6
#define UART_RX         5
#define UART_CTS        7
#define UART_RTS        12
#define UART_HWFC       false
#endif

#define UART_RECV_BUF_LEN    1024
#define UART_EVENT_BUF_LEN    1024

typedef int (*event_callback)(unsigned char byte);

typedef struct _net_event {
    char * perfix;
    char * suffix;
    event_callback cb;
}net_event;
    

int hal_uart_init(void);
int hal_uart_send(unsigned char * buf, unsigned int len, unsigned char * resp);
void recv_event_register(net_event * e);
void uart_evt_hdl(unsigned char ch);
#endif /* __HW_UART_H__ */
