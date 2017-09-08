#ifndef	__STORE_h__

#define	__STORE_h__

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include  "lib.h"
#include "platform_config.h"

//******************************************************************************
//				for   use   2014-11-25
//******************************************************************************


void Init_External_EEPROM(void);

u32 SLAVE_EEPROM_COUNT_NUM(void);
void SAVE_SLAVE_EEPROM_COUNT_NUM(u32 datat);
u32 GET_SLAVE_EEPROM_COUNT_NUM();
u32 START_SLAVE_EEPROM_NUM(void);
u8 GET_SLAVE_EEPROM_COUNT_SIGNAL(u8 adrr);

void ID_Model_Lifetimes_CHECK(void);
u32 Check_CRC_Code(void);
void WRITE_SLAVE_EEPROM_KEY(void);

void GET_SLAVE_EEPROM_KEY();
u8 SLAVE_EEPROM_KEY_CHECK();

u8 RENEW_SLAVE_EEPROM_NUM(u32 count);
void initial_Slave_model(void);

void Delays(u32 nCount);

void  SLAVE_Write_Data(u8 slave,u8 adrr,u32 data);
u32  SLAVE_Read_Data(u8 slave,u8 adrr);
void Read_LCD_MODEL(void);
//-----------------------------------------------------------------------
#endif

