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
#if defined(WITH_AT_FRAMEWORK) && defined(USE_SIM900A)
#include "at_api_interface.h"
#else
#include "esp8266.h"
#endif

int main()
{
    nrf_gpio_range_cfg_output(18, 22);

    hw_init();

#if defined(WITH_AT_FRAMEWORK) && defined(USE_SIM900A)
    at_api_register(&at_interface);
#else
    u2n_if_register(&u2n_if_op);
#endif

    LED_ON(3);
    agent_tiny_entry();
}
