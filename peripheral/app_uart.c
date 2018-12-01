#include <rthw.h>
#include <rtthread.h>

#include "app_uart.h"

#include "board.h"

static rt_thread_t uart_tid;

/* 串口接收事件标志 */
#define UART_RX_EVENT (1 << 0)

/* 事件控制块 */
static struct rt_event event;
/* 串口设备句柄 */
static rt_device_t uart_device = RT_NULL;

/* 回调函数 */
static rt_err_t uart_intput(rt_device_t dev, rt_size_t size)
{
    /* 发送事件 */
    rt_event_send(&event, UART_RX_EVENT);

    return RT_EOK;
}

rt_uint8_t uart_getchar(void)
{
    rt_uint32_t e;
    rt_uint8_t ch;

    /* 读取1字节数据 */
    while (rt_device_read(uart_device, 0, &ch, sizeof(ch)) != sizeof(ch))
    {
         /* 接收事件 */
        rt_event_recv(&event, UART_RX_EVENT,RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,RT_WAITING_FOREVER, &e);
    }

    return ch;
}
void uart_putchar(rt_uint8_t c)
{
    rt_size_t len = 0;
    rt_uint32_t timeout = 0;
    do
    {
        len = rt_device_write(uart_device, 0, &c, sizeof(c));
        timeout++;
    }
    while (len != sizeof(c) && timeout < 500);
}

void uart_putstring(rt_uint8_t *s)
{
    while(*s)
    {
        uart_putchar(*s++);
    }
}

rt_err_t uart_open(const char *name)
{
    rt_err_t res;

    uart_device = rt_device_find(name);   
    if (uart_device != RT_NULL)
    {   
        res = rt_device_set_rx_indicate(uart_device, uart_intput);
        if (res != RT_EOK)
        {
            rt_kprintf("set %s rx indicate error.%d\n",name,res);
            return -RT_ERROR;
        }

        res = rt_device_open(uart_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX );
        if (res != RT_EOK)
        {
            rt_kprintf("open %s device error.%d\n",name,res);
            return -RT_ERROR;
        }

        rt_event_init(&event, "event", RT_IPC_FLAG_FIFO); 
    }
    else
    {
        rt_kprintf("can't find %s device.\n",name);
        return -RT_ERROR;
    }

    return RT_EOK;
}



