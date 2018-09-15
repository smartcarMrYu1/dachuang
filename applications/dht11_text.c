#include <rtthread.h>
#include <rtdevice.h>



static int dht11_read(int argc,char *argv[])
{
    rt_device_t dht_dev;
    float dht_buff[2];
    rt_kprintf("dht11 read text\n");
    
    //linux
    //查找设备
    //打开设备
    //读设备
    
    dht_dev = rt_device_find("dht11_0");
    if(dht_dev ==RT_NULL)
    {
        rt_kprintf("not find dht11_0! exit\n");
        return -1;
    }
    if(rt_device_open(dht_dev,RT_DEVICE_OFLAG_RDONLY) != RT_EOK)
    {
        rt_kprintf("open dht_dev fail!! exit.\n");
        return -1;
    }
    /* 读  比较特殊*/
    rt_memset(dht_buff,0,sizeof(dht_buff));
    rt_device_read(dht_dev,0,dht_buff,sizeof(dht_buff));
    rt_kprintf("tmp1:%d  tmp2:%d\n",(int)(dht_buff[0]),(int)(dht_buff[1]));

    rt_device_close(dht_dev);    
    //当不需要读写的时候，关闭设备
}
MSH_CMD_EXPORT(dht11_read,dht11 read text);
