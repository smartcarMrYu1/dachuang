#include <rtthread.h>
#include <rtdevice.h>

static int ds18b20_read(int argc,char *argv[])
{
    rt_device_t dev;
    float buff[1];
    rt_kprintf("ds18b20 read text\n");
    
    //linux
    //�����豸
    //���豸
    //���豸
    
    dev = rt_device_find("18b20_0");
    if(dev ==RT_NULL)
    {
        rt_kprintf("not find ds18b20_0! exit\n");
        return -1;
    }
		
    if(rt_device_open(dev,RT_DEVICE_OFLAG_RDONLY) != RT_EOK)
    {
        rt_kprintf("open dev fail!! exit.\n");
        return -1;
    }
    /* ��  �Ƚ�����*/
    rt_memset(buff,0,sizeof(buff));
    rt_device_read(dev,0,buff,sizeof(buff));
    rt_kprintf("tmp:%d\n",(int)(buff[0]));

    rt_device_close(dev);    
    //������Ҫ��д��ʱ�򣬹ر��豸
}
MSH_CMD_EXPORT(ds18b20_read,ds18b20 read text);

