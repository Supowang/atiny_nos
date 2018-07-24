#ifndef __U2N_IF_H__
#define __U2N_IF_H__

#include "hw_uart.h"

#define U2N_RET_SUCC  0
#define U2N_RET_FAILED  -1

typedef struct _u2n_if {
    int32_t (*init)(void);
    int32_t (*connect)(const char* host, const char* port, int proto);
    int32_t (*send)(int32_t id , const uint8_t  *buf, uint32_t len);

    int32_t (*recv)(int32_t id, unsigned char* buf, size_t len);
    int32_t (*recv_timeout)(int32_t id, unsigned char* buf, size_t len, unsigned int timeout);
    int32_t (*close)(int32_t id );
}u2n_if;

extern u2n_if u2n_if_op;

int u2n_if_register(u2n_if * op);
int u2n_if_connect(const char* host, const char* port, int proto);
int u2n_if_recv_timeout(int32_t id, unsigned char* buf, size_t len,
                           unsigned int timeout);
int u2n_if_recv(int32_t id, unsigned char* buf, size_t len);
int u2n_if_send(int32_t id, const unsigned char* buf, size_t len);
int u2n_if_close(int32_t id);

#endif /*  __U2N_H__ */
