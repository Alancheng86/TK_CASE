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
#ifndef __MTRANSMIT_H
#define __MTRANSMIT_H

#define Senser_Master_PIN_SCL  GPIO_Pin_15
#define Senser_Master_PIN_SDA  GPIO_Pin_6

#define Senser_GPIO_Master_SCL   GPIOB
#define Senser_GPIO_Master_SDA   GPIOC
//------------------------------------------------------------
#define Senser_MSCL_L GPIO_ResetBits(Senser_GPIO_Master_SCL, Senser_Master_PIN_SCL)  //SCL
#define Senser_MSCL_H GPIO_SetBits(Senser_GPIO_Master_SCL, Senser_Master_PIN_SCL)
#define Senser_MSDA_L GPIO_ResetBits(Senser_GPIO_Master_SDA, Senser_Master_PIN_SDA)  //SDA
#define Senser_MSDA_H GPIO_SetBits(Senser_GPIO_Master_SDA, Senser_Master_PIN_SDA)

#define Senser_MSDAx GPIO_ReadInputDataBit(Senser_GPIO_Master_SDA,Senser_Master_PIN_SDA);

void Senser_Master_SPI_Configuration(void);
void Senser_Master_SDA_Input_Config(void);
void Senser_Master_SDA_Output_Config(void);

void Senser_Dly_1us (unsigned int nus);
void Senser_Dly_1ms (unsigned int nms);

void Senser_Master_Send_Data(unsigned char data);
unsigned char Senser_Master_Receive_Data(void);
unsigned char Senser_Master_Receive_Sync_ACK(void) ;
void Senser_Master_Send_Sync_ACK(void);
unsigned char Senser_Master_send_ACK(void) ;

unsigned short ave_Bright(void);
void Measure_Brightness_Init(void);
unsigned short Receive_flicker(void);
void Niubee_Senser_initial(void);
void AUT_Flicker_Senser_initial(void);
#endif /* __MTRANSMIT_H */
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
