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

rt_mq_t HCSR501_mq;

void HCSR501_thread_send(void *parameter)
{
	 int result;
	 rt_uint8_t HCSR501_data;
   
	while(1)
	{
		 HCSR501_data = rt_pin_read(77);
		 result = rt_mq_send(HCSR501_mq, &HCSR501_data, sizeof(HCSR501_data));
		 if ( result == -RT_EFULL)
		 {
				rt_kprintf("message queue full, delay 1s\n");
				rt_thread_delay(100);
		 }
 }
	   
}

void HCSR501_thread_reve(void *parameter)
{
	  rt_uint8_t HCSR501_data;
		rt_pin_mode(77,PIN_MODE_INPUT);
    while (1)
    {
        if (rt_mq_recv(HCSR501_mq, &HCSR501_data, sizeof(HCSR501_data), RT_WAITING_FOREVER)
                == RT_EOK)
        {
            rt_pin_read(77);
        }
        rt_thread_delay(10);
    }
}



void HCSR501_timer()
{
		rt_timer_t HCSR501_timer;
	  HCSR501_timer = rt_timer_create("HCSR501_timer",HCSR501_thread_send,RT_NULL,100,RT_TIMER_FLAG_PERIODIC);
	  if(HCSR501_timer != RT_NULL)
			rt_timer_start(HCSR501_timer);
}


void HCSR501_part()
{
	rt_thread_t HCSR;
	HCSR501_mq = rt_mq_create("HCSR501_mq",32,2,RT_IPC_FLAG_FIFO);
	HCSR501_timer();
	while(1)
	{
		 HCSR = rt_thread_create("HCSR",HCSR501_thread_reve,RT_NULL,512,14,30);
     if(HCSR != RT_NULL)
     rt_thread_startup(HCSR);			 
  }
}
