#include <stdio.h>

#include "esp8266.h"
#include "hal_init.h"
#include "atiny_socket.h"
#if defined(WITH_AT_FRAMEWORK) && defined(USE_ESP8266)
QUEUE_DEFS(esp8266_q, ESP8266_Q_SIZE, ESP8266_ITEM_SIZE);

CMD esp8266_cmd[] = {
        {"AT\r\n", "OK"},
        {"AT+RESTORE\r\n", "ready"},
        {"ATE0\r\n", "OK"},
        {"AT+CWMODE_CUR", "OK"},
        {"AT+CWJAP_CUR", "OK"},
        {"AT+CIPMUX", "OK"},
        {"AT+CIPSTART", "OK"},
        {"AT+CIPSEND", "SEND OK"},
        {"AT+CIPCLOSE", "OK"},
        {NULL, NULL},
};

int esp8266_send_cmd(E_CMD e)
{
    if (e >= CMD_MAX)
        return -1;
        
    return hal_uart_send((uint8_t *)esp8266_cmd[e].cmd, strlen((char*)esp8266_cmd[e].cmd), (uint8_t *)esp8266_cmd[e].resp);
}

void esp8266_choose_net_mode(enum_net_mode m)
{
    if (m > STA_AP)
        return ;
    
    char cmd[64] = {0};
    snprintf(cmd, 64, "%s=%d\r\n", esp8266_cmd[CMD_CWMODE].cmd, m);
    hal_uart_send((uint8_t *)cmd, strlen(cmd), (uint8_t *)esp8266_cmd[CMD_CWMODE].resp);
    return ;
}

int esp8266_joinap(char * ssid, char * passwd)
{
    if (NULL == ssid || NULL == passwd)
        return -1;
        
    char cmd[64] = {0};
    snprintf(cmd, 64, "%s=\"%s\",\"%s\"\r\n", esp8266_cmd[CMD_JOINAP].cmd, ssid, passwd);
    return hal_uart_send((uint8_t *)cmd, strlen(cmd), (uint8_t *)esp8266_cmd[CMD_JOINAP].resp);
}

void esp8266_set_mux_mode(int mux_mode)
{
    char cmd[64] = {0};
    snprintf(cmd, 64, "%s=%d\r\n", esp8266_cmd[CMD_CIPMUX].cmd, mux_mode);
    hal_uart_send((uint8_t *)cmd, strlen(cmd), (uint8_t *)esp8266_cmd[CMD_CIPMUX].resp);
}

int32_t esp8266_event_handler(uint8_t byte)
{
    static int state = 0;  //0: get id, 1: get len, 2:get data
    static int len = 0;
    static char buf[1024] = {0};
    static int buf_count = 0;
    
    
    switch (state)
    {
        case 0:  //byte = ','
           len = 0;
            memset(buf, 0, 1024);
            buf_count = 0;
            state ++;
            break;

        case 1: //byte = len:
            if (byte >= '0' && byte <= '9')
                len = len * 10 + byte - '0';
            else
                state ++;
            break;
        case 2: //byte = data
            if (buf_count < len)
                buf[buf_count ++] = byte;
           
            if (buf_count == len)
            {
                state ++;
                queue_buf b;
                
                if (len > 0) 
                {
                    b.len = len;
                    b.addr = atiny_malloc(len);
                    memcpy(b.addr, buf, len);
                    
                    hal_mb_put(&b);
                }
            }
            break; 
        default:
            break;
    }
    if (state > 2)
    {
        state = 0;
        return 1;
    }
    
    return 0;
}

int32_t esp8266_init(void)
{
    drv_delay_ms(2000);
    net_event e = {
        .perfix = "+IPD",
        .suffix = NULL,
        .cb = esp8266_event_handler,
    };
    
    hal_mailbox_init(&esp8266_q);
    recv_event_register(&e);
    
//    esp8266_send_cmd(CMD_AT);
    esp8266_send_cmd(CMD_RST);
//    esp8266_send_cmd(CMD_ECHO_OFF);
    esp8266_choose_net_mode(STA);
    while(0 != esp8266_joinap(WIFI_SSID, WIFI_PASSWD))
    {
        LOGD("connecting wifi....");
    };
    esp8266_set_mux_mode(0);   //signle
    
    return 0;
}

int32_t esp8266_connect(const int8_t* host, const int8_t* port, int32_t proto)
{
    char cmd[64] = {0};

    snprintf(cmd, 64, "%s=\"%s\",\"%s\",%s\r\n", esp8266_cmd[CMD_CONN].cmd, proto == ATINY_PROTO_UDP? "UDP" : "TCP", host, port);
    return hal_uart_send((uint8_t *)cmd, strlen(cmd), (uint8_t *)esp8266_cmd[CMD_CONN].resp);
}

int32_t esp8266_recv_timeout(int32_t id, int8_t * buf, uint32_t len,
                           int32_t timeout)
{
    queue_buf b = {0, NULL};
    timeout /= 5;
    
    
    do{
        if (hal_mb_get(&b) == 0 && b.len > 0)
        {
            memcpy(buf, b.addr, b.len);
            atiny_free(b.addr);
            break;
        }
        if (timeout != AT_WAIT_FOREVER && timeout > 0)
        {
            drv_delay_ms(5);
            timeout --;
        }
    }while(timeout == AT_WAIT_FOREVER || timeout > 0);
    return b.len;
}

int32_t esp8266_recv(int32_t id, int8_t * buf, uint32_t len)
{
    esp8266_recv_timeout(id, buf, len, AT_WAIT_FOREVER);
    return len; 
}

int32_t esp8266_send(int32_t id , const uint8_t  *buf, uint32_t len)
{
    char cmd[64] = {0};

    snprintf(cmd, 64, "%s=%lu\r\n", esp8266_cmd[CMD_SEND].cmd, len);
         
    hal_uart_send((uint8_t *)cmd, strlen(cmd), (uint8_t *)">");
         
    hal_uart_send((uint8_t *)buf, len, (uint8_t *)esp8266_cmd[CMD_SEND].resp);
    return 0;
}

int32_t esp8266_close(int32_t id)
{
        char cmd[64] = {0};
        snprintf(cmd, 64, "%s\r\n", esp8266_cmd[CMD_CLOSE].cmd);
        return hal_uart_send((uint8_t *)cmd, strlen(cmd), (uint8_t *)esp8266_cmd[CMD_CLOSE].resp);
}

at_adaptor_api at_interface = {
    .init = esp8266_init,
    .connect = esp8266_connect,
    .send = esp8266_send,
    .recv_timeout = esp8266_recv_timeout,

    .recv = esp8266_recv,    
    .close = esp8266_close,    
};
#endif
