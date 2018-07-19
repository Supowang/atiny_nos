#include <stdio.h>
#include <string.h>
#include "nrf51.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "app_uart.h"
#include "agent_tiny_demo.h"
#include "app_timer.h"
#include "app_mailbox.h"
#include "mem_manager.h"

APP_MAILBOX_DEF(mb, 3, 4);

#define UART_TX    	9
#define UART_RX			11
#define UART_CTS		10
#define UART_RTS		8
#define UART_HWFC   false

const app_uart_comm_params_t uart_params = {
	UART_RX,
	UART_TX,
	UART_RTS,
	UART_CTS,

	APP_UART_FLOW_CONTROL_DISABLED,
	UART_HWFC,
	UART_BAUDRATE_BAUDRATE_Baud38400
};

void uart_evt_hdl(app_uart_evt_t * e)
{
		static int led_0 = 18;

		switch(e->evt_type)
		{
			case APP_UART_DATA:
				nrf_gpio_pin_toggle(led_0);
				break;
			default:
				break;
			
	//		app_uart_put(e->data.value);
	}
}
uint32_t ch = '0';
uint8_t * buf = NULL;
APP_TIMER_DEF( my_timer);
APP_TIMER_DEF( my_timer2);
static void my_timer_handler(void * p_context)
{
    nrf_gpio_pin_toggle(18);
	app_mailbox_put(&mb, &ch);
	ch += 1;
	if (ch > 'z')
		ch = '0';
	
	buf = nrf_malloc(4);
}
static void my_timer2_handler(void * p_context)
{
	uint32_t c;
	
	nrf_gpio_pin_toggle(19);
	app_mailbox_get(&mb, &c);
	app_uart_put(c);
	if (buf)
	{
		nrf_free(buf);
		buf = NULL;
	}
}
#define STR "START:\r\n"

#include "nrf_drv_clock.h"
// Function starting the internal LFCLK oscillator.
// This is needed by RTC1 which is used by the application timer
// (When SoftDevice is enabled the LFCLK is always running and this is not needed).
static void lfclk_request(void)
{
	nrf_drv_clock_config_t c = NRF_DRV_CLOCK_DEAFULT_CONFIG;
    uint32_t err_code = nrf_drv_clock_init(&c);
    APP_ERROR_CHECK(err_code);
    nrf_drv_clock_lfclk_request();
}

int main()
{
//		int ret = 0;
		lfclk_request();
		APP_TIMER_INIT(0, 3, false);
		app_mailbox_create(&mb);
	
		nrf_gpio_cfg_output(22);
		nrf_gpio_cfg_output(18);
		nrf_gpio_cfg_output(19);
		app_uart_init(&uart_params, NULL, uart_evt_hdl, APP_IRQ_PRIORITY_LOW);

	  app_timer_create(&my_timer, APP_TIMER_MODE_REPEATED, my_timer_handler);
		app_timer_start(my_timer, APP_TIMER_TICKS(500, 0), NULL);

		app_timer_create(&my_timer2, APP_TIMER_MODE_REPEATED, my_timer2_handler);
		app_timer_start(my_timer2, APP_TIMER_TICKS(500, 0), NULL);
	
		app_uart_buf_put(STR, strlen(STR));
//		uint16_t ch = 'O';
		while(1)
		{
			//	power_manage();
		}
}
