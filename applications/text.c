#include <rtthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <rtdevice.h>

#define EVENT_W_PIN (4)

static void pin_thread_entry(void *parameter)
{
    rt_uint8_t pin_value;
    rt_pin_mode(EVENT_W_PIN,PIN_MODE_INPUT_PULLDOWN);
    while (1)
    {
        // 检测到低电平，即按键 1 按下了
        if (rt_pin_read(EVENT_W_PIN) == PIN_LOW)
        {
            rt_kprintf("reve low level!\n");
        }

    }
}
int pin_text(void)
{
    rt_thread_t tid;
    tid = rt_thread_create("pin_text",pin_thread_entry,RT_NULL,512,15,20);
    if(tid == RT_EOK)
    {
        rt_kprintf("F:%s,L:%d  Thread create fail",__FUNCTION__,__LINE__);
        return -1;
    }
    rt_thread_startup(tid);

    return 0;
}

