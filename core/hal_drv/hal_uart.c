#include <stdio.h>
#include <string.h>
#include "hal_uart.h"
#include "drv_delay.h"

typedef enum {
    TX_TRANSING,
    TX_DONE,
}TX_STATE;


static uint8_t uart_recv_buf[UART_RECV_BUF_LEN] = {0};
static uint32_t uart_recv_idx = 0;

static uint8_t uart_event_buf[UART_EVENT_BUF_LEN] = {0};
static uint32_t uart_event_idx = 0;

static uint8_t * resp_str = NULL;
static uint8_t tx_state = TX_TRANSING;

static net_event event_list[5];
static uint8_t event_count = 0;

static event_callback event_cb = NULL;

void recv_event_register(net_event * e)
{    
    if (event_count >= 5)
        return;
    
    memcpy(&event_list[event_count++], e, sizeof(net_event));
}

void uart_evt_hdl(unsigned char ch)
{
    int list_idx = 0;
      
    if (event_cb)
    {
        if (event_cb(ch)) //when return not 0, mean that event data handler done!
            event_cb = NULL;
    }
 #if 1
    uart_event_buf[uart_event_idx++] = ch;
    for (list_idx = 0; list_idx < event_count; list_idx++)
    {
        if (uart_event_idx >= strlen(event_list[list_idx].perfix) && 
            0 == memcmp(uart_event_buf + uart_event_idx - strlen(event_list[list_idx].perfix) , event_list[list_idx].perfix, strlen(event_list[list_idx].perfix)))
        {
            event_cb = event_list[list_idx].cb;
            break;
        }
    }
    #endif
    uart_recv_buf[uart_recv_idx++] = ch;
    if (resp_str && uart_recv_idx >= strlen((char*)resp_str) &&
        0 == memcmp(uart_recv_buf + uart_recv_idx - strlen((char *)resp_str) , resp_str, strlen((char *) resp_str)))
    {
        tx_state = TX_DONE;
        memset(uart_recv_buf, 0, UART_RECV_BUF_LEN);
        uart_recv_idx = 0;

        //reset uart_event_buf
        memset(uart_event_buf, 0, UART_EVENT_BUF_LEN);
        uart_event_idx = 0;
    }

}

int32_t hal_uart_init()
{
    int ret = 0;
    
    #ifdef DEBUG_UART_ON
    Debug_USART_Config(); //debug uart init
    #endif
    Net_USART_Config();  //usart drv init
    
    register_recv_evt_hdl(uart_evt_hdl);
    return ret;
}

#define RESP_TIMEOUT 30000
int32_t hal_uart_send(uint8_t * buf, uint32_t len, uint8_t * resp)
{
    uint32_t timeout_ms = RESP_TIMEOUT / 5; // 30s
    int ret = 0;

    resp_str = resp;
    tx_state = TX_TRANSING;
    drv_uart_send((char*)buf, len);
    while(resp && tx_state != TX_DONE && timeout_ms){
        drv_delay_ms(5);
        timeout_ms--;
    };
    resp_str = NULL;
    
    if (0 == timeout_ms)
        ret = -1;

    LOGD("ret = %d", ret);
    return ret;
}

