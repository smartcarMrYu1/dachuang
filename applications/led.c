#include <rtthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <rtdevice.h>


void led_thread(void *parameter)
{
	  rt_pin_mode(21, PIN_MODE_OUTPUT);
		rt_pin_write(21,0);
}



void light_led()
{
  rt_thread_t led;
	led = rt_thread_create("leght led",led_thread,NULL,512,15,20);
	if(led != RT_NULL)
		rt_thread_startup(led);
}
