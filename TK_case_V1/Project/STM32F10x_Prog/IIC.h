#ifndef	__IIC_h__
#define	__IIC_h__

//#include "stm32f10x_lib.h"
#include "stm32f10x.h"
//#include  "lib.h"


#define DEVICE_ADDRESS   0xa2	 ////for 24C02 use


#define PIN_SCL1  GPIO_Pin_15
#define PIN_SDA1  GPIO_Pin_3
#define GPIO_EEPROM_SDA                                  GPIOB
#define GPIO_EEPROM_SCL                                  GPIOA
#define RCC_APB2Periph_GPIO_EEPROM    RCC_APB2Periph_GPIOA

#define ESCL_L GPIO_ResetBits(GPIO_EEPROM_SCL, PIN_SCL1)  //SCL
#define ESCL_H GPIO_SetBits(GPIO_EEPROM_SCL, PIN_SCL1)
#define ESDA_L GPIO_ResetBits(GPIO_EEPROM_SDA , PIN_SDA1)  //SDA
#define ESDA_H GPIO_SetBits(GPIO_EEPROM_SDA , PIN_SDA1)

//-----------------------------------
void EEPROM_GPIO_Config(void);
//----------For  24lc64------------------
unsigned char EEPROMBYTE_Write(unsigned int addr,unsigned char regdata);
unsigned char EEPROMBYTE_Read(unsigned int addr);
unsigned char EEPROMfloat_Write(unsigned int addr,float gdata);
float EEPROMfloat_Read(unsigned int addr);


#endif
