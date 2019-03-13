/*
 * @File:   dwin_cmd.c 
 * @Author: liu2guang 
 * @Date:   2018-04-22 14:52:10 
 * 
 * @LICENSE: MIT
 * https://github.com/liu2guang/dwin/blob/master/LICENSE
 * 
 * Change Logs: 
 * Date           Author       Notes 
 * 2018-04-24     liu2guang    update v2 framework. 
 */ 
 
#include "dwin_cmd.h" 
#include "finsh.h" 
#include "dwin.h"

static rt_uint32_t str2int(const char *str)
{
    int num = 0; 
    
    if(!strstr(str, "0x") && !strstr(str, "0X")) 
    {
        num = atoi(str); 
    }
    else
    {
        sscanf(str, "%x", &num); 
    }
    
    return num; 
} 

static void uasge(uint8_t argc, char **argv)
{
    rt_uint8_t index = 0; 
    
    DWIN_PRINT("\033[31mERR command format:\033[0m", argv[1]); 
    for(;index < argc; index++)
    {
        DWIN_PRINT(" %s", argv[index]); 
    }
    DWIN_PRINT("\n"); 
    
    DWIN_PRINT("The command format:\033[0m\n"); 
    DWIN_PRINT("  01. read reg or var        dwin -t r <reg|var> <addr> <len>\n"); 
    DWIN_PRINT("  02. write reg or var       dwin -t w <reg|var> <addr> <len> <data...>\n"); 
    DWIN_PRINT("  03. print version          dwin -s ver\n"); 
    DWIN_PRINT("  04. set or read backlight  dwin -s bl [level]\n"); 
    DWIN_PRINT("  05. buzz tick*10ms         dwin -s buzz <tick>\n");
    DWIN_PRINT("  06. read current page      dwin -s page \n");
    DWIN_PRINT("  07. jump specified page    dwin -s jump <page>\n"); 
    DWIN_PRINT("  08. en or disable touch    dwin -s touch <enable|disable>\n");
    DWIN_PRINT("  09. set or read rtc        dwin -s rtc [year] [mon] [day] [hour] [min] [sec]\n");
    DWIN_PRINT("  10. send keycode(0x01~FF)  dwin -s key <code>\n"); 
    DWIN_PRINT("  11. print parses info      dwin -d parse\n"); 
    DWIN_PRINT("  12. print pages info       dwin -d page\n"); 
}

static void uasge_t(uint8_t argc, char **argv)
{
    rt_uint8_t index = 0; 
    
    DWIN_PRINT("command format:", argv[1]); 
    for(;index < argc; index++)
    {
        DWIN_PRINT(" %s", argv[index]); 
    }
    DWIN_PRINT("\n"); 
    
    DWIN_PRINT(" command format:\033[0m\n"); 
    DWIN_PRINT("  01. read reg or var        dwin -t r <reg|var> <addr> <len>\n"); 
    DWIN_PRINT("  02. write reg or var       dwin -t w <reg|var> <addr> <len> <data...>\n"); 
}

static void uasge_s(uint8_t argc, char **argv)
{
    rt_uint8_t index = 0; 
    
    DWIN_PRINT("ERR command format:", argv[1]); 
    for(;index < argc; index++) 
    {
        DWIN_PRINT(" %s", argv[index]); 
    }
    DWIN_PRINT("\n"); 
    
    DWIN_PRINT("The command format:\033[0m\n"); 
    DWIN_PRINT("  01. print version          dwin -s ver\n"); 
    DWIN_PRINT("  02. set or read backlight  dwin -s bl [level]\n"); 
    DWIN_PRINT("  03. buzz tick*10ms         dwin -s buzz <tick>\n");
    DWIN_PRINT("  04. read current page      dwin -s page \n");
    DWIN_PRINT("  05. jump specified page    dwin -s jump <page>\n");
    DWIN_PRINT("  06. en or disable touch    dwin -s touch <enable|disable>\n");
    DWIN_PRINT("  07. set or read rtc        dwin -s rtc [year] [mon] [day] [hour] [min] [sec]\n");
    DWIN_PRINT("  08. send keycode(0x01~FF)  dwin -s key <code>\n");
}

static void uasge_d(uint8_t argc, char **argv)
{
    rt_uint8_t index = 0; 
    
    DWIN_PRINT("ERR command format:", argv[1]); 
    for(;index < argc; index++) 
    {
        DWIN_PRINT(" %s", argv[index]); 
    }
    DWIN_PRINT("\n"); 
    
    DWIN_PRINT("The command format:\n"); 
    DWIN_PRINT("  01. print register parse   dwin -d parse\n"); 
    DWIN_PRINT("  02. print create pase      dwin -d page\n"); 
}

/* 只有开启调试模式, 才有该命令 */ 
static int dwin_cmd(uint8_t argc, char **argv) 
{
    if(argc < 2)
    {
        uasge(argc, argv); 
        return RT_EOK; 
    }
    else
    {
        if(!strcmp(argv[1], "-t") || !strcmp(argv[1], "--transfer")) 
        {
            /* 读寄存器 */ 
            if((!strcmp(argv[2], "r")) && (!strcmp(argv[3], "reg")) && (argc >= 6))
            {
                rt_uint8_t data[256] = {0}; 
                rt_uint8_t  len  = (rt_uint8_t )str2int(argv[5]); 
                rt_uint16_t addr = (rt_uint16_t)str2int(argv[4]); 
                
                DWIN_DBG("User read%dbyte(s) from 0x%.4xreg: \n", len, addr); 
                dwin_reg_read(addr, data, len); 
                
                return RT_EOK; 
            }
            
            /* 读变量 */ 
            else if((!strcmp(argv[2], "r")) && (!strcmp(argv[3], "var")) && (argc >= 6))
            {
                rt_uint16_t data[128] = {0}; 
                rt_uint8_t  len  = (rt_uint8_t )str2int(argv[5]); 
                rt_uint16_t addr = (rt_uint16_t)str2int(argv[4]); 
                
                DWIN_DBG("User read %d byte(s) from 0x%.4x var: \n", len, addr); 
                dwin_var_read(addr, data, len); 
                
                return RT_EOK; 
            }
            
            /* 写寄存器 */ 
            else if((!strcmp(argv[2], "w")) && (!strcmp(argv[3], "reg")) && (argc >= 7))
            {
                rt_uint8_t index = 0; 
                rt_uint8_t data[256] = {0}; 
                rt_uint8_t  len  = (rt_uint8_t )str2int(argv[5]); 
                rt_uint16_t addr = (rt_uint16_t)str2int(argv[4]); 
                
                for(index = 0; index < len; index++)
                {
                    data[index] = (rt_uint8_t)str2int(argv[6+index]);
                }
                
                DWIN_DBG("User write %d byte(s) from 0x%.4x reg: \n", len, addr); 
                dwin_reg_write(addr, data, len); 
                return RT_EOK; 
            }
            
            /* 写变量 */ 
            else if((!strcmp(argv[2], "w")) && (!strcmp(argv[3], "var")) && (argc >= 7))
            {
                rt_uint8_t index = 0; 
                rt_uint16_t data[128] = {0}; 
                rt_uint8_t  len  = (rt_uint8_t )str2int(argv[5]); 
                rt_uint16_t addr = (rt_uint16_t)str2int(argv[4]); 
                
                for(index = 0; index < len; index++)
                {
                    data[index] = (rt_uint16_t)str2int(argv[6+index]);
                }
                
                DWIN_DBG("User write %d byte(s) from 0x%.4xvar: \n", len, addr); 
                dwin_var_write(addr, data, len); 
                return RT_EOK; 
            } 
            
            else
            {
                uasge_t(argc, argv); 
                return RT_ERROR; 
            }
        }
        
        else if(!strcmp(argv[1], "-s") || !strcmp(argv[1], "--system")) 
        {
            if(!strcmp(argv[2], "ver"))
            {
                rt_uint32_t data = 0; 
                dwin_system_version(&data); 
            }
            
            else if(!strcmp(argv[2], "bl") && (argc == 3))
            {
                rt_uint8_t data = 0;
                dwin_system_get_backlight(&data); 
            }
            
            else if(!strcmp(argv[2], "bl") && (argc >= 4))
            {
                dwin_system_set_backlight((rt_uint8_t)str2int(argv[3])); 
            }
            
            else if(!strcmp(argv[2], "jump") && (argc >= 4))
            {
                extern rt_err_t dwin_page_jump_id(rt_uint16_t id); 
                dwin_page_jump_id((rt_uint16_t)str2int(argv[3])); 
            }
            
            else if(!strcmp(argv[2], "page") && (argc == 3))
            {
                rt_uint16_t data = 0;
                dwin_system_page(&data); 
            } 
            
            else if(!strcmp(argv[2], "buzz") && (argc >= 3))
            {
                dwin_system_buzz((rt_uint8_t)str2int(argv[3])); 
            }
            
            else if(!strcmp(argv[2], "touch") && (!strcmp(argv[3], "enable") || !strcmp(argv[3], "disable")))
            {
                rt_bool_t en = (!strcmp(argv[3], "enable")) ? RT_TRUE : RT_FALSE; 
                
                dwin_system_touch(en); 
            }
            
            else if(!strcmp(argv[2], "rtc"))
            {
                if(argc == 3)
                {
                    struct dwin_rtc rtc = {0}; 
                    dwin_system_get_rtc(&rtc); 
                }
                else
                {
                    struct dwin_rtc rtc = {0}; 
                    
                    /* 先读取RTC时间 */ 
                    dwin_system_get_rtc(&rtc); 
                    if(argc >= 4) {rtc.year   = (rt_uint16_t)str2int(argv[3]);}
                    if(argc >= 5) {rtc.month  = (rt_uint8_t )str2int(argv[4]);}
                    if(argc >= 6) {rtc.day    = (rt_uint8_t )str2int(argv[5]);}
                    if(argc >= 7) {rtc.hour   = (rt_uint8_t )str2int(argv[6]);}
                    if(argc >= 8) {rtc.minute = (rt_uint8_t )str2int(argv[7]);}
                    if(argc >= 9) {rtc.second = (rt_uint8_t )str2int(argv[8]);}
                    
                    dwin_system_set_rtc(rtc); 
                }
            }
            
            else if(!strcmp(argv[2], "key") && argc >= 4)
            {
                dwin_system_key((rt_uint8_t)str2int(argv[3])); 
            }
            
            else
            {
                uasge_s(argc, argv); 
                return RT_ERROR; 
            }
        }
        
        else if(!strcmp(argv[1], "-d") || !strcmp(argv[1], "--debug"))
        {
            if(!strcmp(argv[2], "parse"))
            {
                dwin_parse_register_info(); 
            } 
            
            else if(!strcmp(argv[2], "page"))
            {
                dwin_page_info(); 
            } 
            
            else
            {
                uasge_d(argc, argv); 
                return RT_ERROR; 
            }
        }
        
        else
        {
            uasge(argc, argv); 
            return RT_ERROR;
        }
    }
    
    return RT_EOK; 
}

MSH_CMD_EXPORT_ALIAS(dwin_cmd, dwin, The dwin develop and debug toolkit);
