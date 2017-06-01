/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : lcd.h
* Author             : MCD Application Team
* Version            : V1.1.2
* Date               : 09/22/2008
* Description        : This file contains all the functions prototypes for the
*                      lcd firmware driver.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H
#define __LCD_H

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"
#include "stdarg.h"


#define uint  u16
//========================================= GPIO CONFIG ===================================================

//----------------- 端口定义: STM32 --> SSD2828  ----------------
#define	SSD2828_LCDPin_CSX0	(1<<5)   
#define	SSD2828_Pin_RST	(1<<4)        //硬复位  	      
#define	SSD2828_LCDPin_SCL	(1<<12)   //时钟端口	       
#define	SSD2828_LCDPin_SDI	(1<<2)    //数据端口
#define	SSD2828Pin_SHUT	(1<<11)   
#define	LCDPin_RST	(1<<8)            //硬复位	

#define SSD2828_RST(x)    GPIOB->ODR=(GPIOB->ODR&~SSD2828_Pin_RST)|(x ? SSD2828_Pin_RST:0)
#define SSD2828_CSX0(x)   GPIOB->ODR=(GPIOB->ODR&~SSD2828_LCDPin_CSX0)|(x ? SSD2828_LCDPin_CSX0:0)													    
#define SSD2828_SCL(x)    GPIOC->ODR=(GPIOC->ODR&~SSD2828_LCDPin_SCL) |(x ? SSD2828_LCDPin_SCL:0)
#define SSD2828_SDI(x)    GPIOD->ODR=(GPIOD->ODR&~SSD2828_LCDPin_SDI)|(x ? SSD2828_LCDPin_SDI:0)
#define SSD2828_SHUT(x)   GPIOC->ODR=(GPIOC->ODR&~SSD2828Pin_SHUT)|(x ? SSD2828Pin_SHUT:0)
#define LCD_RST(x)        GPIOB->ODR=(GPIOB->ODR&~LCDPin_RST)|(x ? LCDPin_RST:0)	
//-----------------------------------V3ADJ Control Line -------------------	
#define	V3ADJPin_EN	(1<<0)  //V3ADJ片选端口
#define V3ADJ_EN(x)   GPIOA->ODR=(GPIOA->ODR&~V3ADJPin_EN)|(x ? V3ADJPin_EN:0)


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/*----- High layer function -----*/
/*----- Low layer function -----*/
void LCD_CtrlLinesConfig(void);
void EXTI_Configuration();
void V3ADJ_SET(char vset,char plus50mv);
void STM_MINI_SET(void);
void STM32_Init_SSD2828(void);
void PG_inition_lcd(void);
void SSD2828_Inition(void);
void MIPI_SPI_Write(uint i,...);

void STM32TOSSD2828_W_COM(u8 CMD);
void STM32TOSSD2828_W_DATA_8BITS(u8 DATA);
void STM32TOSSD2828_W_DATA_16BITS(u16 DATA2);

void SSD2828_ENTER_LP_mode(void);
void SSD2828_VIDEO_MODE_HS(void);

void Forward_scan(void);
void Backward_scan(void);

u16  READ_SSD2828(u8 CMD);
u8 READ_IC(u8 cmd,u8 cmd1);
u8 READ_IC_A(u8 cmd,u8 cmd1);  //读2828从LCD ic中取回来的值

void MDelay(u32 num);
void TC358768_DCS_write_1A_1P(u8 cmd,u8 data);
void TC358768_DCS_write_1A_0P(u8 cmd);
void SSD2828_101FD05(void);

void SSD2828_VIDEO_MODE_HS(void);
void SSD2828_ENTER_LP_mode(void);
void SSD2828_ENTER_READ_LCDIC_mode(void);
void STM32TOSSD2828_W_COM(u8 CMD)  ;
void STM32TOSSD2828_W_COM(u8 CMD) ;
void STM32TOSSD2828_W_DATA_16BITS(u16 DATA2);  //16 bit	  
u16 SSD2825_READ_AnyByte(u8 CMD) ;  //  读2828寄存器，读多个Byte，判断其是否正常
u16 READ_SSD2828_onebyte(u8 CMD) ; //  读2828寄存器，读1Byte，判断其是否正常
void enter_sleep_mode(void);
void exit_sleep_mode(void);
void reset_lcd (void);

void SEND_DATA_OnlyFor045LF(int sum,u8 data_buf[]);








#define Line0          0
#define Line1          24
#define Line2          48
#define Line3          72
#define Line4          96
#define Line5          120
#define Line6          144
#define Line7          168
#define Line8          192
#define Line9          216













//===========================
void ID_set(void);
void VCOM_set(u8 vcom);
void MTP(void);
void MTP_ID(void);

//============================


void BEEP_ON(void);
void BEEP_OFF(void);
void BEEP_Dudu(void);
void BEEP_BIBI(void);
void BEEP_XiXi(void);
void BEEP_HuHu(void);
void BACK_LIGHT_OFF(void);
void BACK_LIGHT_ON(void);

#endif /* __LCD_H */
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
