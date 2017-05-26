#ifndef	_V_SET_h__

#define	__V_SET_h__

#include "stm32f10x.h"
//#include "stm32f10x_usart.h"
//#include  "lib.h"
//#include "lcd.h"
//#include "platform_config.h"
#include "T226.h"     	//INA226������⣬�����ļ�
//#include "stm32f10x.h"

//-----------------------------------VDD/VDDIO/VOTP     Control  -------------------	
#define	VDD_ADJPin_EN	(1<<1)  //VDD_ADJƬѡ�˿�
#define VDD_ADJ_EN(x)   GPIOA->ODR=(GPIOA->ODR&~VDD_ADJPin_EN)|(x ? VDD_ADJPin_EN:0)

#define	VDDIO_ADJPin_EN	(1<<15)  //VDDIO_ADJƬѡ�˿�
#define VDDIO_ADJ_EN(x)   GPIOB->ODR=(GPIOB->ODR&~VDDIO_ADJPin_EN)|(x ? VDDIO_ADJPin_EN:0)

#define	VOTP_ADJPin_EN	(1<<4)  //VOTP_ADJƬѡ�˿�
#define VOTP_EN(x)   GPIOA->ODR=(GPIOA->ODR&~VOTP_ADJPin_EN)|(x ? VOTP_ADJPin_EN:0)

#define	RELAY_Pin_EN	(1<<12)  //VOTP_ADJƬѡ�˿�
#define RELAY_EN(x)   GPIOA->ODR=(GPIOA->ODR&~RELAY_Pin_EN)|(x ? RELAY_Pin_EN:0)

#define	LS138_A0_EN  (1<<8)
#define A0_EN(x)   GPIOA->ODR=(GPIOA->ODR&~LS138_A0_EN)|(x ? LS138_A0_EN:0)

#define	LS138_A1_EN  (1<<9)
#define A1_EN(x)   GPIOA->ODR=(GPIOA->ODR&~LS138_A1_EN)|(x ? LS138_A1_EN:0)

void VDD_ADJ_SET(u32 VDD_settig);		 //VDD_ADJ_SET ����ȡֵ��Χ270~600����Ӧ��ѹ��Χ2.7V~6V; 
void VDDIO_ADJ_SET(u32 vset);			 //VDDIO_ADJ_SET ����ȡֵ��Χ160~360����Ӧ��ѹ��Χ1.6V~3.6V; 
void VOTP_ADJ_SET(u32 vset);			 //VOTP_ADJ_SET ����ȡֵ��Χ150~2080����Ӧ��ѹ��Χ1.5V~14.1V; 

//-----------------------------------------------------------------------
#endif


