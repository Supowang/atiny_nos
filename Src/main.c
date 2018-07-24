#include <stdio.h>
#include <string.h>
#if 0
#include "nrf51.h"
#include "nrf_delay.h"
#include "app_timer.h"
#include "app_mailbox.h"
#include "mem_manager.h"
#endif
#include "sys_init.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

#include "internals.h"
#include "atiny_socket.h"

#if defined WITH_AT_FRAMEWORK
#include "at_api_interface.h"
#else
#include "esp8266.h"
#endif


int main()
{
    nrf_gpio_range_cfg_output(18, 22);

    hw_init();

//    hw_uart_send((uint8_t *)__func__, strlen(__func__));

#if defined(WITH_AT_FRAMEWORK) && defined(USE_SIM900A)
    at_api_register(&at_interface);
#else

    u2n_if_register(&u2n_if_op);
#endif

//    atiny_net_connect("192.168.1.111", "5683", 1);

//    atiny_net_send(0, "hello", 5);
//    u2n_if_close(0);
    LED_ON(3);
    agent_tiny_entry();
    while(1)
    {
        //    power_manage();
       unsigned char buf[64] = {0};
       int ret = atiny_net_recv_timeout(NULL, buf, 64, 3000);
       if (ret > 0)
           atiny_net_send(0, (const unsigned char*)buf, strlen((const char*)buf));
        nrf_delay_ms(1000);
    }
}
