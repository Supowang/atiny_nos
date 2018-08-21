#include <stdio.h>
#include "drv_debug_uart.h"

#ifdef DEBUG_UART_ON

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void Debug_USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK,ENABLE);

  RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN  ;  
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE,DEBUG_USART_RX_AF);

  GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);
  
  USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(DEBUG_USART, &USART_InitStructure); 
	
  NVIC_Configuration();
  
  USART_ClearFlag(DEBUG_USART, USART_FLAG_TC);
//  USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);
	
  USART_Cmd(DEBUG_USART, ENABLE);
}


/* define fputc */
#if defined ( __CC_ARM ) || defined ( __ICCARM__ )  /* KEIL and IAR: printf will call fputc to print */
int fputc(int ch, FILE *f)
{
    while(USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TC) == RESET);
	USART_SendData(DEBUG_USART, ch);
    return ch;
}

#elif defined ( __GNUC__ )  /* GCC: printf will call _write to print */

void debug_usart_SendBuf( USART_TypeDef * pUSARTx, char *buf, int len)
{
	unsigned int i=0;
  for (i = 0; i < len; i++)
  {
      while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
      USART_SendData( pUSARTx, *(buf + i));
  } ;
}

__attribute__((used)) int _write(int fd, char *ptr, int len)
{
    (void)debug_usart_SendBuf(DEBUG_USART, ptr, len);
    return len;
}
#endif

#endif /* debug_uart_on */

