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
#ifndef __HFZ_SECRET_H
#define __HFZ_SECRET_H

#define Master_PIN_SCL  GPIO_Pin_4
#define Master_PIN_SDA  GPIO_Pin_5
#define Master_PIN_CS   GPIO_Pin_9
#define Master_PIN_RET  GPIO_Pin_8

#define GPIO_Master   GPIOB
//------------------------------------------------------------
#define MSCL_L GPIO_ResetBits(GPIO_Master, Master_PIN_SCL)  //SCL
#define MSCL_H GPIO_SetBits(GPIO_Master, Master_PIN_SCL)
#define MSDA_L GPIO_ResetBits(GPIO_Master, Master_PIN_SDA)  //SDA
#define MSDA_H GPIO_SetBits(GPIO_Master, Master_PIN_SDA)
#define MCS_L GPIO_ResetBits(GPIO_Master, Master_PIN_CS)  //CS
#define MCS_H GPIO_SetBits(GPIO_Master, Master_PIN_CS)

#define MRET_L GPIO_ResetBits(GPIO_Master, Master_PIN_RET)  //RET
#define MRET_H GPIO_SetBits(GPIO_Master, Master_PIN_RET)

#define MSDAx GPIO_ReadInputDataBit(GPIO_Master,Master_PIN_SDA);

void Master_SPI_Configuration(void);
void Master_SDA_Input_Config(void);
void Master_SDA_Output_Config(void);


void Dly_1us (unsigned int nus);
void Dly_1ms (unsigned int nms);

void Master_Send_Data(unsigned short data);
unsigned char Master_Receive_Data(void);
void Master_Receive_Sync_ACK(void);
void Master_Send_Sync_ACK(void);
void Master_send_ACK(void);
void Master_send_NACK(void) ; 
void ADC1_Configuration(void);
void ADC1_GPIO_Config(void);
unsigned short ADC_Keyscan(void); 

unsigned char Random_convert(unsigned char num,unsigned short data);

unsigned char Cali_methods1(unsigned short data);
unsigned char Cali_methods2(unsigned short data);
unsigned char Cali_methods3(unsigned short data);
unsigned char Cali_methods4(unsigned short data);
unsigned char Cali_methods5(unsigned short data);
unsigned char Secret_Dolby(void);
#endif /* __MTRANSMIT_H */
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
