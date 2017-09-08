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
#ifndef __OTP_H
#define __OTP_H

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"

#include "lcd.h"
#include  "lib.h"
#include "tsliic.h"



#define KEY_PAUSE       GPIOB->IDR&GPIO_Pin_12
#define KEY_UP      GPIOB->IDR&GPIO_Pin_13
#define KEY_DOWN      GPIOB->IDR&GPIO_Pin_14
#define KEY_AUTO_MODE      GPIOB->IDR&GPIO_Pin_1
#define KEY_MAN_MODE      GPIOB->IDR&GPIO_Pin_0


void I2C_DATAL_COMPARE(void);
void I2C_MEMSURE1(int kk, int vcomdcdata );
void I2C_DATAL_COMPAREX(void);
void I2C_DATAL_COMPARE(void);
void Auto_otp(void);
void OTP_NO(void);
void OTP_OK(void);
void OTP_ERROR_1(void);
void OTP_ERROR_2(void);
void OTP_ERROR_3(void);
void OTP_ERROR_4(void);


void OTP_ID_OK(void);
void OTP_ID_NG(void);
void Gamma_OK(void);
void Gamma_NG(void);

void OTP_adjust_result(u16 senser_val);
void Running_water_result();


u8 Gamma_Check(void);
void Gamma_READ(void);
u8 VCOM_TIMES_Check(void);
void VCOM_CHECK(u8 vcom); 
void VCOM_READ(void);
void ID_READ(void);
u8 ID_CHECK(void);

void KEY_adjust(void);

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/////////////////////add in OTP.h----------------------------
int Sensor_check(void);




#endif /* __OTP_H */
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
