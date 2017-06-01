/******************** (C) COPYRIGHT 2010 GIANTPLUS ********************
* File Name          : keyscan_adc.h
* Author             : YIMING
* Version            : V1.0.0
* Date               : 2011.12.15
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __keyscan_adc_H
#define __keyscan_adc_H	  
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#define ADC_LIVES 8

extern void Keyscan_init(void);
//-----?值返回 0??按下，1--8?值正常---//
u8 Keyscan(void);
extern void Delay(u32 nCount);
void ADC_Configuration(void);

u8 Tempscan(void);
void temp_config(void);



u16 ADC_VOL_value(void);
#endif /* __keyscan_adc_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
