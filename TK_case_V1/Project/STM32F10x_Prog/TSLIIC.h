#ifndef	__TSLIIC_h__
#define	__TSLIIC_h__


#include  "lib.h"
#include "stm32f10x.h"
#include "lcd.h"


#define  lvtimes_F 100			   //测Flicker时光感采样次数
#define DEARLAY_TIME		11		//////IIC接口延时定义



//============================
#define TSL_PIN_SCL  GPIO_Pin_10
#define TSL_PIN_SDA  GPIO_Pin_11
//#define TSL_PIN_SCL  GPIO_Pin_14
//#define TSL_PIN_SDA  GPIO_Pin_13
#define GPIO_Sensor                               GPIOB
#define RCC_APB2Periph_GPIO_TSLIIC RCC_APB2Periph_GPIOB
//------------------------------------------------------------
#define TSCL_L GPIO_ResetBits(GPIO_Sensor, TSL_PIN_SCL)  //SCL
#define TSCL_H GPIO_SetBits(GPIO_Sensor, TSL_PIN_SCL)
#define TSDA_L GPIO_ResetBits(GPIO_Sensor, TSL_PIN_SDA)  //SDA
#define TSDA_H GPIO_SetBits(GPIO_Sensor, TSL_PIN_SDA)

#define S1_addr 0X92
#define S2_addr 0X72
#define S3_addr 0X52 

#define ID_REG		0x1e		//////ID reg

#define	ID_TSL2581	0x00		////TSL2581
#define	ID_TSL2583	0x30		////TSL2583

unsigned short GET_FLICKER(void);

unsigned char sensor_check (unsigned char Device_N);
void TSL2561Init(unsigned char Device_N);
unsigned int GetBright(unsigned char Device_N);
unsigned int GetBright2(unsigned char Device_N);
void Sensor_GPIO_Config(void);


//--------------------------------------------------------------
#endif
