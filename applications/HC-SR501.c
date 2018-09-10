/***********************************************
1��һ��ѭ���߳�
2��һ�����ڶ�ʱ��
3����ʱ���ص�������̷߳��ʼ�
4���߳�����ȴ������ʼ����յ��ʼ��󣬶�һ��IO

***********************************************/
#include <rtthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <rtdevice.h>

#define EVENT_W_PIN  (4)
#define DS18B20_PIN  (124)

static rt_mq_t hcsr_mq;
static rt_timer_t hcs_timer;
static rt_timer_t ds_timer;

static void HCSR501_timer_callback(void *parameter)   //�ص����������ļ�̣���֪ͨ������
{
    rt_uint8_t HCSR501_data = EVENT_W_PIN;

     if(rt_mq_send(hcsr_mq, &HCSR501_data, sizeof(HCSR501_data)) != RT_EOK)
     {
         rt_kprintf("F:%s L:%d err!  message quene1 full!\n,",__FUNCTION__,__LINE__);
         return;
     }
}
static void ds18b20_timer_callback(void *parameter)
{
     rt_uint8_t DS_data = DS18B20_PIN;
     if(rt_mq_send(hcsr_mq, &DS_data, sizeof(DS_data)) != RT_EOK)
     {
         rt_kprintf("F:%s L:%d err!  message quene2 full!\n,",__FUNCTION__,__LINE__);
         return;
     }
}

static void HCSR501_thread_entry(void *parameter)
{
    rt_uint8_t HCSR501_data1;
    rt_uint8_t pin_value;
    rt_pin_mode(EVENT_W_PIN,PIN_MODE_INPUT_PULLUP);
    while (1)
    {
        if (rt_mq_recv(hcsr_mq, &HCSR501_data1, sizeof(HCSR501_data1), RT_WAITING_FOREVER) == RT_EOK)
        {
            switch(HCSR501_data1)
            {
            case EVENT_W_PIN:
            {
                pin_value = rt_pin_read(EVENT_W_PIN);  
                //ds18b20_read();
                rt_kprintf("pin_value is low:%d  \n",pin_value);
            }
            case DS18B20_PIN:
            {
                pin_value = rt_pin_read(DS18B20_PIN);
                rt_kprintf("ds18b20_pin_value is low:%d\n",pin_value);
            }
            }
        }
    }
}

int HCSR501_part_init(void)
{
	rt_thread_t tid;

	hcsr_mq = rt_mq_create("all_mq",32,2,RT_IPC_FLAG_FIFO);
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
    
    ds_timer = rt_timer_create("ds18b20 timer",ds18b20_timer_callback,RT_NULL,1000,RT_TIMER_FLAG_PERIODIC);
    if(ds_timer == RT_NULL)
    {
        rt_kprintf("F:%s L:%d err! ds_timer create fail!\n,",__FUNCTION__,__LINE__);
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
     rt_timer_start(ds_timer);       
     return 0;
}


/*�ܽ�һ�㲽�裺
  1�����Ȱ���Ҫ���̡߳���ʱ������Ϣ���д�����
  2���������ʧ�ܣ���ɾ�������ͷ���Դ
*/
