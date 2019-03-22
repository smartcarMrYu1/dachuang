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
#include "public_def.h"

/* ҳ�� */ 

dwin_page_t page[PAGE_NUM];

dwin_num_input_t input; 
dwin_button_t    login1,login2,login3,login4,login5,login6,login7; 
dwin_qrcode_t    qrcode; 
dwin_gbk_t       gbk; 
dwin_icon_t      icon; 
dwin_num_t       num;

static void login_callback(void)
{
		rt_kprintf("Login.\n");
}

static void login1_cb(void)
{
	  led1_light();
}
static void login2_cb(void)
{
		led2_light();
}

int main(void)
{
    /* user app entry */
		rt_uint16_t index = 0; 
    
    for(index = 0; index < sizeof(page)/sizeof(dwin_page_t); index++)
    {
        page[index] = dwin_page_create(index); 
    }
    /* ����4 ���� */
		login1 = dwin_button_create(page[3], 0x1024, login_callback); 
		login2 = dwin_button_create(page[3], 0x1026, login_callback); 
		login3 = dwin_button_create(page[3], 0x1028, login_callback); 
		login4 = dwin_button_create(page[3], 0x102A, login_callback); 
		/* ����5 ���� */
		login5 = dwin_button_create(page[4], 0x102C, login_callback); 
		/* ����6 ���� */
		login6 = dwin_button_create(page[3], 0x102E, login_callback); 
		login7 = dwin_button_create(page[3], 0x1030, login_callback); 
		

		
    return dwin_run(0);
}
