#ifndef	__M2481_CONTROL_h__

#define	__M2481_CONTROL_h__

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include  "lib.h"
#include "platform_config.h"

void M2481_EN_Pin_Configuration(void);
void PWM_Pin_configuration(void);
void TIM4_Configuration(void);   	//PB9对应TIM4通道4
 void M2481_PWM_scan(unsigned char duty);
void Adjust_Output_Current(unsigned char duty);

#endif

