/*
 * File      : app_uart.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2012, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 *2017-12-15      DQL          the first version 
 */

#include <rthw.h>
#include <rtthread.h>

#include "app_uart.h"


/* 串口接收事件标志 */
#define UART_RX_EVENT (4 << 0)



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

rt_uint8_t uart_getchar(rt_device_t uart_device)
{
    rt_uint32_t e;
    rt_uint8_t ch;
    
    /* 读取1字节数据 */
    while (rt_device_read(uart_device, 0, &ch, 1) != 1)
    {
         /* 接收事件 */
        rt_event_recv(&event, UART_RX_EVENT,RT_EVENT_FLAG_AND | RT_EVENT_FLAG_CLEAR,RT_WAITING_FOREVER, &e);
    }

    return ch;
}
void uart_putchar(const rt_uint8_t c)
{
    rt_size_t len = 0;
    rt_uint32_t timeout = 0;
    do
    {
        len = rt_device_write(uart_device, 0, &c, 1);
        timeout++;
    }
    while (len != 1 && timeout < 500);
}

//void uart_putstring(rt_device_t device,rt_uint8_t *s)
//{
//    rt_device_write(device,0,s,sizeof(s));
//}

void data_read(rt_device_t device,rt_uint8_t* buff, int len)
{
    rt_uint8_t uart_rx_byte;
    rt_uint8_t count = 0;
    
    do{
        uart_rx_byte = uart_getchar(device);
        buff[count] = uart_rx_byte;
        count ++;       
    }while((uart_rx_byte != '\n') && (count <= len));
    
    buff[count] = 0;
}


rt_err_t uart_open(const char *name)
{
    rt_err_t res;
                                 
    /* 查找系统中的串口设备 */
    uart_device = rt_device_find(name);   
    /* 查找到设备后将其打开 */
    if (uart_device != RT_NULL)
    {   
        res = rt_device_set_rx_indicate(uart_device, uart_intput);
        /* 检查返回值 */
        if (res != RT_EOK)
        {
            rt_kprintf("set %s rx indicate error.%d\n",name,res);
            return -RT_ERROR;
        }
				

        /* 打开设备，以可读写、中断方式 */
        res = rt_device_open(uart_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);       
        /* 检查返回值 */
        if (res != RT_EOK)
        {
            rt_kprintf("open %s device error.%d\n",name,res);
            return -RT_ERROR;
        }

    }
    else
    {
        rt_kprintf("can't find %s device.\n",name);
        return -RT_ERROR;
    }

    /* 初始化事件对象 */
    rt_event_init(&event, "event", RT_IPC_FLAG_FIFO); 
    
    return RT_EOK;
}





