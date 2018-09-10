#ifndef __STM32_DRV_DS18B20_H_
#define __STM32_DRV_DS18B20_H_


/****************************************************************************
*�ṹ��
****************************************************************************/
typedef struct _gpio_des
{
	GPIO_TypeDef *gpio;
	rt_uint32_t pin;
}gpio_des;

#define DS18B20_Dout_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOG_CLK_ENABLE()
#define DS18B20_Dout_PORT                           GPIOG
#define DS18B20_Dout_PIN                            GPIO_PIN_9

/***********************   DS18B20 �����궨��  ****************************/
#define DS18B20_Dout_LOW()                          HAL_GPIO_WritePin(DS18B20_Dout_PORT,DS18B20_Dout_PIN,GPIO_PIN_RESET) 
#define DS18B20_Dout_HIGH()                         HAL_GPIO_WritePin(DS18B20_Dout_PORT,DS18B20_Dout_PIN,GPIO_PIN_SET)
#define DS18B20_Data_IN()                           HAL_GPIO_ReadPin(DS18B20_Dout_PORT,DS18B20_Dout_PIN)

/****************************************************************************
*�����б�
****************************************************************************/
int ds18b20_drv_init(void);   /* ע������ */


#endif




//#ifndef __ONEWIRE_DS18B20_H__
//#define        __ONEWIRE_DS18B20_H__

///* ����ͷ�ļ� ----------------------------------------------------------------*/
//#include "stm32f1xx_hal.h"

///* ���Ͷ��� ------------------------------------------------------------------*/
///* �궨�� -------------------------------------------------------------------*/
///***********************   DS18B20 �������Ŷ���  **************************/
//#define DS18B20_Dout_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOD_CLK_ENABLE()
//#define DS18B20_Dout_PORT                           GPIOD
//#define DS18B20_Dout_PIN                            GPIO_PIN_3

///***********************   DS18B20 �����궨��  ****************************/
//#define DS18B20_Dout_LOW()                          HAL_GPIO_WritePin(DS18B20_Dout_PORT,DS18B20_Dout_PIN,GPIO_PIN_RESET) 
//#define DS18B20_Dout_HIGH()                         HAL_GPIO_WritePin(DS18B20_Dout_PORT,DS18B20_Dout_PIN,GPIO_PIN_SET)
//#define DS18B20_Data_IN()                                  HAL_GPIO_ReadPin(DS18B20_Dout_PORT,DS18B20_Dout_PIN)

///* ��չ���� ------------------------------------------------------------------*/
///* �������� ------------------------------------------------------------------*/
//uint8_t DS18B20_Init(void);
//void DS18B20_ReadId( uint8_t * ds18b20_id);
//float DS18B20_GetTemp_SkipRom(void);
//float DS18B20_GetTemp_MatchRom(uint8_t *ds18b20_id);

//#endif /* __ONEWIRE_DS18B20_H__ */
///******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/

