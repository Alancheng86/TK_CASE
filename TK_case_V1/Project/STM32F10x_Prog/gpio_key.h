/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : lcd.h
* Author             : MCD Application Team
* Version            : V1.1.2
* Date               : 09/22/2008
* Description        : 
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_KEY_H__
#define __GPIO_KEY_H__
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
 #define uint u16 
//-------------*******--LCD端口定义-------*********** ---------------
//因为DEMO板与测试版不同，故针对DEMO板，"#define STM32DEMO" ,否则把下面的语句注释掉即可
#define STM32DEMO

#ifdef STM32DEMO
  #define	SSD1963Pin_CS	  (1<<2)   //SSD1963片选端口  	         
//  #define	LCDPin_RST	(1<<0)   //硬复位		 
#else  
  #define	SSD1963Pin_CS	  (1<<0)   //SSD1963片选端口  	        
//  #define	LCDPin_RST	(1<<2)   //硬复位	 
#endif 

#define	LCDPin_RS	(1<<3)  //数据/命令选择端口	Pa.3
#define	LCDPin_RD	(1<<4)  // 
#define	LCDPin_WR	(1<<5)  // 
#define	LCDPin_CS	(1<<1)  //

#define LCD_RS(x)   GPIOA->ODR=(GPIOA->ODR&~LCDPin_RS)|(x ? LCDPin_RS:0)
#define SSD1963_CS(x)   GPIOA->ODR=(GPIOA->ODR&~SSD1963Pin_CS)|(x ? SSD1963Pin_CS:0)													    
//#define LCD_RST(x)  GPIOA->ODR=(GPIOA->ODR&~LCDPin_RST)|(x ? LCDPin_RST:0)	
#define LCD_RD(x)   GPIOA->ODR=(GPIOA->ODR&~LCDPin_RD) |(x ? LCDPin_RD:0)
#define LCD_WR(x)   GPIOA->ODR=(GPIOA->ODR&~LCDPin_WR)|(x ? LCDPin_WR:0)
#define LCD_CS(x)   GPIOA->ODR=(GPIOA->ODR&~LCDPin_CS)|(x ? LCDPin_CS:0)		
//#define DATAOUT(x) GPIOB->ODR=(GPIOB->ODR&0xffff00FF)|((x)&0xff00);	  
#define DATAOUT(x) GPIO_Write(GPIOB,(x))
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


//=============================================
//#define S1_addr 0X92
//#define S2_addr 0X72
//#define S3_addr 0X52 
//unsigned char sensor_check (unsigned char Device_N);
//void TSL2561Init(unsigned char Device_N);
//unsigned int GetBright(unsigned char Device_N);
//unsigned int GetBright2(unsigned char Device_N);


/* LCD color */
#define   Black         000000     		 /* 黑色：    0,   0,     0 */
#define   BLUE          0000FF         /* 蓝色：    0,   0,   255 */
#define   GREEN         00FF00         /* 绿色：    0,   255,   0 */
#define   RED           FF0000         /* 红色：    255,   0,   0 */
#define   White         FFFFFF      	 /* 白色：    255, 255, 255 */

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

#endif /* __GPIO_KEY_H__ */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
