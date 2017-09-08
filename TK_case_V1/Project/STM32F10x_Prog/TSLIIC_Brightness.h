#ifndef	__TSLIIC_Brightness_h__
#define	__TSLIIC_Brightness_h__

#include "stm32f10x.h"
#define uchar u8
#define uint  u16
#define Uchar u8
#define Uint  u16
u32 LED_ave_Bright(void);	
float ave_Bright (void);				 //光感部分子程序，计算出光感读数

#endif
