#ifndef	__IICS_h__
#define	__IICS_h__

#include "stm32f10x.h"
//#include "stm32f10x_gpio.h"


#define PIN_SCL2  GPIO_Pin_14
#define PIN_SDA2  GPIO_Pin_13

#define ESCLS_L GPIO_ResetBits(GPIOA, PIN_SCL2)  //SCL
#define ESCLS_H GPIO_SetBits(GPIOA, PIN_SCL2)
#define ESDAS_L GPIO_ResetBits(GPIOA, PIN_SDA2)  //SDA
#define ESDAS_H GPIO_SetBits(GPIOA, PIN_SDA2)

 

unsigned char EEPROMBYTE_WriteS(unsigned int slave,unsigned int addr,unsigned char regdata);
unsigned char EEPROMBYTE_ReadS(unsigned int slave,unsigned int addr);
void EEPROM_GPIO_ConfigS(void);


#endif
