#include <stdio.h>

#include "u2n_if.h"
u2n_if * g_op = NULL;

int u2n_if_register(u2n_if * op)
{
    if (NULL == g_op)
    {
        g_op = op;
    }
    if (g_op && g_op->init)
    {
        return g_op->init();
    }
    return 0;
}

int u2n_if_connect(const char* host, const char* port, int proto)
{
    if (g_op && g_op->connect)
    {
        return g_op->connect(host, port, proto);
    }
    return 0;
}

int u2n_if_recv_timeout(int32_t id, unsigned char* buf, size_t len,
                           unsigned int timeout)
{
    if (g_op && g_op->recv_timeout)
    {
        return g_op->recv_timeout(id, buf, len, timeout);
    }
    return 0;
}

int u2n_if_recv(int32_t id, unsigned char* buf, size_t len)
{
    if (g_op && g_op->recv)
    {
        return g_op->recv(id, buf, len);
    }
    return 0;
}

int u2n_if_send(int32_t id, const unsigned char* buf, size_t len)
{
    if (g_op && g_op->send)
    {
        return g_op->send(id, buf, len);
    }
    return 0;
}

int u2n_if_close(int32_t id)
{
    if (g_op && g_op->close)
    {
        return g_op->close(id);
    }
    return 0;
}
