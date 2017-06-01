#ifndef	_V_SET_h__

#define	__V_SET_h__

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include  "lib.h"
#include "lcd.h"
#include "platform_config.h"
#include "T226.h"     	//INA226电流检测，驱动文件


//////////////////###############################################################################
////////////////////VDD  设定函数
////////////////////VDD  设定函数
////////////////////VDD  设定函数
////////////////////VDD  设定函数
////////////////////VDD  设定函数
//////////////////###############################################################################
#define DEVICE_ADDRESS_M5	0x5E			   /////GND配置		  -----OK
//#define DEVICE_ADDRESS_M5	0x58			   /////VDD配置		 ///NG
//#define DEVICE_ADDRESS_M5	0x5C			   /////NC配置		 -----OK

#define AD_PIN_SYNC  GPIO_Pin_11
#define AD_PIN_SCLK  GPIO_Pin_8
#define AD_PIN_DIN  GPIO_Pin_9
#define AD_PIN_SDO  GPIO_Pin_7
#define GPIO_SYNC                                   GPIOA
#define GPIO_SCLK                                   GPIOA
#define GPIO_DIN                                   GPIOC
#define GPIO_SDO                                   GPIOC

#define AD_PIN_SCL  GPIO_Pin_8
#define AD_PIN_SDA  GPIO_Pin_9						//
#define GPIO_AD_SDA                                   GPIOC
#define GPIO_AD_SCL                                   GPIOA			 ////////////////-------vdd-enable

//------------------------------------------------------------
#define AD_TPSCL_L GPIO_ResetBits(GPIO_AD_SCL, AD_PIN_SCL)  //SCL
#define AD_TPSCL_H GPIO_SetBits  (GPIO_AD_SCL, AD_PIN_SCL)
#define AD_TPSDA_L GPIO_ResetBits(GPIO_AD_SDA, AD_PIN_SDA)  //SDA
#define AD_TPSDA_H GPIO_SetBits  (GPIO_AD_SDA, AD_PIN_SDA)




//////////////////###############################################################################
////////////////////VOTP  设定函数
////////////////////VOTP  设定函数
////////////////////VOTP  设定函数
////////////////////VOTP  设定函数
////////////////////VOTP  设定函数
//////////////////###############################################################################
#define DEVICE_ADDRESS_M1	0x30		/////////----0x18			   /////
///////// for M1 use
#define P557_M1_PIN_SCL  GPIO_Pin_4
#define P557_M1_PIN_SDA  GPIO_Pin_7						//
#define GPIO_P557_M1_SDA                                   GPIOA
#define GPIO_P557_M1_SCL                                   GPIOC
///////// for M1 use
#define P557_M1_TPSCL_L GPIO_ResetBits(GPIO_P557_M1_SCL, P557_M1_PIN_SCL)  //SCL
#define P557_M1_TPSCL_H GPIO_SetBits  (GPIO_P557_M1_SCL, P557_M1_PIN_SCL)
#define P557_M1_TPSDA_L GPIO_ResetBits(GPIO_P557_M1_SDA, P557_M1_PIN_SDA)  //SDA
#define P557_M1_TPSDA_H GPIO_SetBits  (GPIO_P557_M1_SDA, P557_M1_PIN_SDA)







//////////////////###############################################################################
//////////////////VDDIO设定函数
//////////////////VDDIO设定函数
//////////////////VDDIO设定函数
//////////////////VDDIO设定函数
//////////////////VDDIO设定函数
//////////////////###############################################################################
////////////////////VDDIO 设定函数
#define DEVICE_ADDRESS_M6	0x32		/////////----0x19			   /////
///////////// for M6 use
#define P557_PIN_SCL  GPIO_Pin_8
#define P557_PIN_SDA  GPIO_Pin_6						//
#define GPIO_M6_SDA                                   GPIOC
#define GPIO_M6_SCL                                   GPIOC
///////////// for M6 use
#define P557_TPSCL_L GPIO_ResetBits(GPIO_M6_SCL, P557_PIN_SCL)  //SCL
#define P557_TPSCL_H GPIO_SetBits  (GPIO_M6_SCL, P557_PIN_SCL)
#define P557_TPSDA_L GPIO_ResetBits(GPIO_M6_SDA, P557_PIN_SDA)  //SDA
#define P557_TPSDA_H GPIO_SetBits  (GPIO_M6_SDA, P557_PIN_SDA)




//-----------------------------------VDD/VDDIO/VOTP     Control  -------------------	
#define	VDD_ADJPin_EN	(1<<1)  //VDD_ADJ片选端口

#define	VDDIO_ADJPin_EN	(1<<15)  //VDDIO_ADJ片选端口
#define VDDIO_ADJ_EN(x)   GPIOB->ODR=(GPIOB->ODR&~VDDIO_ADJPin_EN)|(x ? VDDIO_ADJPin_EN:0)
#define	VOTP_ADJPin_EN	(1<<4)  //VOTP_ADJ片选端口
#define VOTP_EN(x)   GPIOA->ODR=(GPIOA->ODR&~VOTP_ADJPin_EN)|(x ? VOTP_ADJPin_EN:0)


void VDD_ADJ_EN(u8 vset);
void VDD_ADJ_SET(u32 VDD_settig);		 //VDD_ADJ_SET 参数取值范围270~600，对应电压范围2.7V~6V; 
void VDDIO_ADJ_SET(u32 vset);			 //VDDIO_ADJ_SET 参数取值范围160~360，对应电压范围1.6V~3.6V; 
void VOTP_ADJ_SET(u32 vset);			 //VOTP_ADJ_SET 参数取值范围150~2080，对应电压范围1.5V~14.1V; 



//-----------------------------------------------------------------------
#endif


