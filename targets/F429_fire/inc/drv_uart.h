#ifndef __DRV_UART_H__
#define __DRV_UART_H__

#include "stm32f4xx.h"

/*******************************************************/
#define NET_USART                             USART3
#define NET_USART_CLK                         RCC_APB1Periph_USART3
#define NET_USART_BAUDRATE                    115200  

#define NET_USART_RX_GPIO_PORT                GPIOB
#define NET_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define NET_USART_RX_PIN                      GPIO_Pin_10
#define NET_USART_RX_AF                       GPIO_AF_USART2
#define NET_USART_RX_SOURCE                   GPIO_PinSource10

#define NET_USART_TX_GPIO_PORT                GPIOB
#define NET_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOB
#define NET_USART_TX_PIN                      GPIO_Pin_11
#define NET_USART_TX_AF                       GPIO_AF_USART2
#define NET_USART_TX_SOURCE                   GPIO_PinSource11

#define NET_USART_IRQHandler                  USART3_IRQHandler
#define NET_USART_IRQ                 		USART3_IRQn
/************************************************************/

typedef void (*uart_recv_event_hdl)(unsigned char ch);

void Net_USART_Config(void);
void drv_uart_send(char * buf, int len);
void register_recv_evt_hdl(uart_recv_event_hdl h);
#endif
