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
#include "esp8266.h"

int main()
{
    nrf_gpio_range_cfg_output(18, 22);

    hw_init();

    u2n_if_register(&u2n_if_op);

    LED_ON(3);
    agent_tiny_entry();
}
