#include <stdio.h>
#include <string.h>
#include "hal_init.h"

#include "internals.h"
#include "atiny_socket.h"
#if defined(WITH_AT_FRAMEWORK)
#include "at_api_interface.h"
#endif

int agent_loop();

int user_main()
{
    return 0;
}
extern void Usart_SendBuf( USART_TypeDef * pUSARTx, char *buf, int len);

int main()
{
    hal_init();

#if defined(WITH_AT_FRAMEWORK) && (defined(USE_SIM900A) || defined(USE_ESP8266))
    at_api_register(&at_interface);
#else

#endif
    hal_timer_start();
    agent_tiny_entry();
    while(1)
    {
        agent_loop();
        /********* USER CODE start *******************/

        /********* USER CODE end *********************/
    };
}
