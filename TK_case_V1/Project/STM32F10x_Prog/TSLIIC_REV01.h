#ifndef	__TSLIIC_REV01_h__
#define	__TSLIIC_REV01_h__

#include "stm32f10x.h"


 //============================
#define PIN_SCL  GPIO_Pin_10
#define PIN_SDA  GPIO_Pin_11
#define GPIO_Sensor                               GPIOB
#define RCC_APB2Periph_GPIO_TSLIIC RCC_APB2Periph_GPIOB
//------------------------------------------------------------
#define TzSCL_L GPIO_ResetBits(GPIO_Sensor, PIN_SCL)  //SCL
#define TzSCL_H GPIO_SetBits(GPIO_Sensor, PIN_SCL)
#define TzSDA_L GPIO_ResetBits(GPIO_Sensor, PIN_SDA)  //SDA
#define TzSDA_H GPIO_SetBits(GPIO_Sensor, PIN_SDA)

#define S1_addr 0X92
#define S2_addr 0X72
#define S3_addr 0X52 

void TSL_GPIO_Config(void);
void delay_10ns (u32 k);
unsigned char TSL_WriteReg(unsigned char Device_N,unsigned char addr,unsigned char regdata);
unsigned char  Sensor_Check(unsigned char Device_N);
void TSL_Init(unsigned char Device_N);
unsigned int Get_Bright(unsigned char Device_N);
void Reset_Senser(void);
void Measure_Brightness_Init(void);

//--------------------------------------------------------------
#endif
