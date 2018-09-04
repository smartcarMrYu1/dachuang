/***********************************************
1、一个循环线程
2、一个周期定时器
3、定时器回调里面给线程发邮件
4、线程里面等待接受邮件，收到邮件后，读一次IO

***********************************************/
#include <rtthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <rtdevice.h>

#define EVENT_W_PIN  (0x01)
#define HCSR_PIN     (4)

static rt_mq_t hcsr_mq;
static rt_timer_t hcs_timer;

static void HCSR501_timer_callback(void *parameter)   //回调函数尽量的简短，起到通知的作用
{
    rt_uint8_t HCSR501_data = EVENT_W_PIN;

     if(rt_mq_send(hcsr_mq, &HCSR501_data, sizeof(HCSR501_data)) != RT_EOK)
     {
         rt_kprintf("F:%s L:%d err!  message quene full!\n,",__FUNCTION__,__LINE__);
         return;
     }
}

static void HCSR501_thread_entry(void *parameter)
{
    rt_uint8_t HCSR501_data;
    rt_uint8_t pin_value;
    rt_pin_mode(HCSR_PIN,PIN_MODE_INPUT);
    while (1)
    {
        if (rt_mq_recv(hcsr_mq, &HCSR501_data, sizeof(HCSR501_data), RT_WAITING_FOREVER) == RT_EOK)
        {
            switch(HCSR501_data)
            {
            case EVENT_W_PIN:
            {
                pin_value = rt_pin_read(HCSR_PIN);   
                rt_kprintf("pin_value:%d",&pin_value);
            }
            }
        }
    //        rt_thread_delay(10);
    }
}

int HCSR501_part_init(void)
{
	rt_thread_t tid;

	hcsr_mq = rt_mq_create("hcsr_mq",16,2,RT_IPC_FLAG_FIFO);
    if(hcsr_mq == RT_NULL)
    {
        rt_kprintf("F:%s L:%d err! mq create fail!\n,",__FUNCTION__,__LINE__);
        return -1;
    }

    hcs_timer = rt_timer_create("HCSR501_timer",HCSR501_timer_callback,RT_NULL,100,RT_TIMER_FLAG_PERIODIC);
    if(hcs_timer == RT_NULL)
    {
        rt_kprintf("F:%s L:%d err! hcs_timer create fail!\n,",__FUNCTION__,__LINE__);
        rt_mq_delete(hcsr_mq);
        return -1;
    }
    
     tid = rt_thread_create("tid",HCSR501_thread_entry,RT_NULL,512,14,30);
     if(tid == RT_NULL)
     {
        rt_mq_delete(hcsr_mq);
        rt_timer_delete(hcs_timer);
        rt_kprintf("F:%s L:%d err! tid create fail!\n,",__FUNCTION__,__LINE__);
        return -1;
     }
     
     rt_thread_startup(tid);
     rt_timer_start(hcs_timer);     
     return 0;
}


/*总结一般步骤：
  1、首先把需要的线程、定时器、消息队列创建好
  2、如果创建失败，则删除，以释放资源
*/
