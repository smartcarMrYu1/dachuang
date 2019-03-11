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

/* 页面 */ 
#define PAGE_NUM 3
dwin_page_t page[PAGE_NUM]; 

dwin_num_input_t input; 
dwin_button_t    login; 
dwin_qrcode_t    qrcode; 
dwin_gbk_t       gbk; 
dwin_icon_t      icon; 


static void input_callback(rt_uint32_t value)
{
    rt_kprintf("User input password is %.8d.\n", value); 
}

static void login_callback(void)
{
    rt_kprintf("Login.\n");
}

int main(void)
{
    /* user app entry */
//		rt_uint16_t index = 0; 
//    
//    for(index = 0; index < sizeof(page)/sizeof(dwin_page_t); index++)
//    {
//        page[index] = dwin_page_create(index); 
//    }
//    
//    /* 全局控件 */ 
//    gbk = dwin_gbk_create(DWIN_ALL_PAGE, DWIN_VAR_ADDR(0x0000), 100); 
//    dwin_gbk_show_string(gbk, "电信学院"); 
////    
//    /* 页面0 */ 
//    qrcode = dwin_qrcode_create(page[0], 0x0300, 100); 
//    dwin_qrcode_show_url(qrcode, "https://github.com/liu2guang/dwin"); 
//    
//    icon = dwin_icon_create(page[0], DWIN_VAR_ADDR(0x400), 6); 
//    dwin_icon_set_index(icon, 5); 
//    
//    /* 页面1 */ 
//    input = dwin_num_input_create(page[1], DWIN_VAR_ADDR(0x0034), input_callback); 
//    login = dwin_button_create   (page[1], DWIN_VAR_ADDR(0x0038), login_callback); 
    
    return dwin_run(0);
}
