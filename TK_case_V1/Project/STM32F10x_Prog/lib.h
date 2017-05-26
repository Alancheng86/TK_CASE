#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#ifndef __LIB_H
#define __LIB_H

void showbmp (u8 i);
void xstart(void);	
void Allcolor (u8 r1,u32 color);
void ShowTxt(char *p);
void TransferTest(void);

//void comsend(char *p);
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#endif
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
