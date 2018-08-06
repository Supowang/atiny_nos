#include <stdio.h>
#include "drv_uart.h"

static uart_recv_event_hdl gf_hdl = NULL;

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  NVIC_InitStructure.NVIC_IRQChannel = NET_USART_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void Net_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(NET_USART_RX_GPIO_CLK|NET_USART_TX_GPIO_CLK,ENABLE);

  RCC_APB1PeriphClockCmd(NET_USART_CLK, ENABLE);
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = NET_USART_TX_PIN  ;  
  GPIO_Init(NET_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = NET_USART_RX_PIN;
  GPIO_Init(NET_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(NET_USART_RX_GPIO_PORT,NET_USART_RX_SOURCE,NET_USART_RX_AF);

  GPIO_PinAFConfig(NET_USART_TX_GPIO_PORT,NET_USART_TX_SOURCE,NET_USART_TX_AF);
  
  USART_InitStructure.USART_BaudRate = NET_USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(NET_USART, &USART_InitStructure); 
	
  NVIC_Configuration();
  
  USART_ClearFlag(NET_USART, USART_FLAG_TC);
  USART_ITConfig(NET_USART, USART_IT_RXNE, ENABLE);
	
  USART_Cmd(NET_USART, ENABLE);
}
void Usart_SendBuf( USART_TypeDef * pUSARTx, char *buf, int len)
{
	unsigned int i=0;
  for (i = 0; i < len; i++)
  {
      while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
      USART_SendData( pUSARTx, *(buf + i));
  } ;
}

void drv_uart_send(char * buf, int len)
{
    Usart_SendBuf(NET_USART, buf, len);
}
void NET_USART_IRQHandler()
{
    if (USART_GetITStatus(NET_USART, USART_IT_RXNE)){
        int ch = USART_ReceiveData(NET_USART);
        if (gf_hdl)gf_hdl(ch);
    }
}

void register_recv_evt_hdl(uart_recv_event_hdl h)
{
    gf_hdl = h;
}
