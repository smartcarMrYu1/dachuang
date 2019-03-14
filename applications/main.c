/*
 * File      : main.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2015-07-29     Arda.Fu      first implementation
 */
#include <rtthread.h>
#include <board.h>
#include <dwin.h>

extern rt_uint16_t ds_buffer[1];

/* 页面 */ 
#define PAGE_NUM 3
dwin_page_t page[PAGE_NUM]; 

dwin_num_input_t input; 
dwin_button_t    login; 
dwin_qrcode_t    qrcode; 
dwin_gbk_t       gbk; 
dwin_icon_t      icon; 
dwin_num_t       num;


int main(void)
{
    /* user app entry */
		rt_uint16_t index = 0; 
  	rt_uint16_t *ds_rev_buf = NULL;
	  *ds_rev_buf = ds_buffer[0]; 
    
    for(index = 0; index < sizeof(page)/sizeof(dwin_page_t); index++)
    {
        page[index] = dwin_page_create(index); 
    }

		//第三页写数字
		num = dwin_num_create(page[2],0x1001,DWIN_NUM_TYPE_U16);
		dwin_num_set_value_u16(num,ds_buffer[0]);
		
//		if(dwin_var_write(0x1001,ds_rev_buf,sizeof(ds_buffer)) == RT_EOK)  
//		{
//        rt_kprintf("ds_buffer write data success !\n");				
//		}
		
    return dwin_run(0);
}
