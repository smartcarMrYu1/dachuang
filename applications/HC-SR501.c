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

#define EVENT_W_PIN      (0x01<<0)
#define DS18B20_PIN      (0x01<<1)
#define EVENT_UNKNOW     (0x00)

static rt_mq_t hcsr_mq;
static rt_timer_t hcs_timer;

static void HCSR501_timer_callback(void *parameter)   //�ص����������ļ�̣���֪ͨ������
{
    rt_uint8_t HCSR501_data = EVENT_W_PIN;
    static int _tick = 0;
    
    if(_tick%4 == 0)                   //4�뷢������
    {
        HCSR501_data |=EVENT_UNKNOW;
    } 
    if(_tick%8 == 0)                   //8��~~~~~~~
    {
        HCSR501_data |= EVENT_W_PIN;
    }
    if(_tick%8 == 0)
    {
        HCSR501_data |= DS18B20_PIN;
    }
    if(_tick ++ > 10000)
    {
        _tick = 0;
    }
     if(rt_mq_send(hcsr_mq, &HCSR501_data, sizeof(HCSR501_data)) != RT_EOK)
     {
         rt_kprintf("F:%s L:%d err!  message quene1 full!\n,",__FUNCTION__,__LINE__);
         return;
     }
}

static void HCSR501_thread_entry(void *parameter)
{
    rt_uint8_t HCSR501_data1;
    
    rt_device_t pin_dev;
    rt_device_t ds18b20_dev;
    
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

    hcs_timer = rt_timer_create("HCSR501_timer",HCSR501_timer_callback,RT_NULL,500,RT_TIMER_FLAG_PERIODIC);
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
     rt_timer_start(ds_timer);       
     return 0;
}


/*�ܽ�һ�㲽�裺
  1�����Ȱ���Ҫ���̡߳���ʱ������Ϣ���д�����
  2���������ʧ�ܣ���ɾ�������ͷ���Դ
*/
