/* Includes ------------------------------------------------------------------*/
#include <M2481_Control.h>
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "lib.h"
#include "platform_config.h"
#include "lcd.h"
#include "IIC.h"
#include <TSLIIC_Brightness.h>
#include "TSLIIC_REV01.h"
#include "TSLIIC.h"
////////////9.1. Sample Code: Definitions
////////////These are the definitions for sample code.
extern void DelayMs(__IO uint32_t nTime);

#define S1_addr 0X92
#define S2_addr 0X72
#define S3_addr 0X52 

unsigned short led0pwm=0;
unsigned short hfz_light=0;
extern unsigned short led0pwmval;//ÆðÊ¼ÁÁ¶ÈµçÁ÷Öµ

/***************************************
* ¹âµçñîºÏÊ¹ÄÜ½ÅÅäÖÃ
* ÖÃ¸ß¿ªÆô²¢³ý¸ßµç×è
***************************************/
void M2481_EN_Pin_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
	
////	/*[°ÑGPIOB ¶ÔÓ¦¶Ë¿ÚÅäÖÃ³ÉÊä³öÄ£Ê½] */
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO×î¸ßËÙ¶È50MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		/*[°ÑGPIOB ¶ÔÓ¦¶Ë¿ÚÅäÖÃ³ÉÊä³öÄ£Ê½] */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO×î¸ßËÙ¶È50MHz
	GPIO_Init(GPIOA	, &GPIO_InitStructure);	
	
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
}
/***************************************
* M2481Ê¹ÄÜ¹Ü½Å¼æPWM¹Ü½ÅÅäÖÃ
*  
***************************************/
void PWM_Pin_configuration()
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
	
	/*[°ÑGPIOB ¶ÔÓ¦¶Ë¿ÚÅäÖÃ³ÉÊä³öÄ£Ê½] */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;      //¶ÔÓ¦¶¨Ê±Æ÷4Í¨µÀ4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO×î¸ßËÙ¶È50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/***************************************
*  PWMÊ±ÖÓÅäÖÃ
*  
***************************************/
void TIM4_Configuration(void)
{ 
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_DeInit(TIM4);		   //²»ÒªÒ²°Õ
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
  TIM_TimeBaseStructure.TIM_Period=0x008F;	//Ô¤×°ÔÚÖµÓë±È½Ï²¶»ñÖµ¿ÉÒÔ×ÔÓÉ·ÖÅä£¬ÈôÒ»Ñù£¬µçÑ¹Öµ¿É´Ó0~×î´óÖµ¡££¬²»È»µçÑ¹
  TIM_TimeBaseStructure.TIM_Prescaler=1;
  TIM_TimeBaseStructure.TIM_ClockDivision=0;
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
 //ÕâÀï¶¨Ê±Æ÷ÖÐ¶Ï¿ÉÒÔ²»ÓÃ£¡
//  TIM_ClearFlag(TIM3,TIM_FLAG_Update);	 //Ê¹ÓÃÖ®Ç°Çå³ýÖ®Ç°µÄÉèÖÃ
//  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
//  TIM_Cmd(TIM3,ENABLE);

 //PWM³õÊ¼»¯
  TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable; //ÕýÏòÍ¨µÀÓÐÐ§
	TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;//Ê¹ÄÜ»¥²¹¶ËÊä³ö
//	TIM_OCInitStructure.TIM_Pulse=300;	  //ÉèÖÃ×°Èë²¶»ñ±È½Ï¼Ä´æÆ÷µÄÂö³åÖµ¡£ËüµÄÈ¡Öµ±ØÐëÔÚ0x0000µ½0xffffÖ®¼ä¡£
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//ËÀÇøºó»¥
  TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;        //ËÀÇøºóÊä³ö×´Ì¬       
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;  ////Êä³ö¼«ÐÔ £¬ÏÈÁÁºóÃð£¬ÏÈÃðºóÁÁµÄÇø±ð
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);	  //Õâ¾äÖÐTIM_OC2¾Íº­¸ÇÁËÊ¹ÓÃÍ¨µÀ2

  TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable); // Ô¤×°ÔØÊ¹ÄÜ//º­¸ÇÁËÊ¹ÓÃÍ¨µÀ2
  TIM_Cmd(TIM4,ENABLE);

//	 TIM_ARRPreloadConfig(TIM1, ENABLE) ;   //ARPEÊ¹ÄÜ 
//   TIM1->CCER|=1<<4;   //OC2 Êä³öÊ¹ÄÜ
//   TIM_CtrlPWMOutputs(TIM1,ENABLE); //¸ß¼¶Ê±ÖÓÌØÓÐ
//   TIM_ITConfig(TIM1, TIM_IT_CC2 , ENABLE); 

}

/***************************************
*  ¿ÉÇÐµµ0--1mA£»1--40mA
*  ¿ØÖÆ±³¹âÁÁ¶È
***************************************/	
 void M2481_PWM_scan(unsigned char duty)
	{ 
	u16 led0pwmval_1ma;
//  static unsigned short led0pwmval=0x0000; //Ä¬ÈÏÊä³ö×î´ó40mA
  static u8 dir=1;	//dirÎª1£¬Êä³öµçÁ÷Ç¿¶ÈÖð½¥¼õÈõ
  if(duty==1)		    //40mA ~1.5mA
	{	
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
		DelayMs(1);   
		if(dir) //ÖµÔ½´óÔ½°µ
		{
			if(led0pwmval>=0x0000&&led0pwmval<=0x000e){led0pwmval+=3;}   //0x0000~0x0005ÄÚÁÁ¶ÈÊÇÃ»ÓÐÇø±ðµÄ
			else{led0pwmval++;}
		}	
	
		else 
		{
			if(led0pwmval>0x0003&&led0pwmval<0x000e){led0pwmval-=3;}
			else{led0pwmval--;}	
		}
		
		if(led0pwmval>0x0050)dir=0;     //0x0050¡«0x008fÊÇ³¤Ê±¼äµÄ°µÆÁ£¬Ã»±ØÒªÔÙ¼ÆÊýÏÂÈ¥¡£ 
		if(led0pwmval==0)dir=1;	
    led0pwm=led0pwmval;
		TIM_SetCompare4(TIM4,led0pwmval);   //TIM_SetCompare1 ¶ÔÓ¦Í¨µÀ1  ÖµÔ½´ó£¬Õ¼¿Õ±ÈÔ½´ó
	} 
	else if(duty==0)	
	{ 
		led0pwmval_1ma=0x0001;
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
		DelayMs(1);  
	  TIM_SetCompare4(TIM4,led0pwmval_1ma);   //TIM_SetCompare1 ¶ÔÓ¦Í¨µÀ1  ÖµÔ½´ó£¬Õ¼¿Õ±ÈÔ½´ó
	}//0.8~1mA
	else if(duty==2)
	{
	  GPIO_SetBits(GPIOA,GPIO_Pin_12);
		DelayMs(1);  
	  TIM_SetCompare4(TIM4,led0pwmval);   //TIM_SetCompare1 ¶ÔÓ¦Í¨µÀ1  ÖµÔ½´ó£¬Õ¼¿Õ±ÈÔ½´	
	}
}	
	
	
void Adjust_Output_Current(unsigned char duty)
{
			unsigned char coc;
			coc=duty;
	    ShowTxt("45,10,10,ÕýÔÚµ÷ÕûÁÁ¶È...");	   					DelayMs(150);
			Reset_Senser();
	    Measure_Brightness_Init();
	    DelayMs(10);
			M2481_PWM_scan(coc);         //Ê×´Î²ÉÑù
			DelayMs(5); 
			hfz_light=ave_Bright();
			if((hfz_light>470)||(hfz_light<300))  //Èç¹û×î´óÊä³öµçÁ÷»·¾³ÏÂ£¬»Ô¶ÈÒÑ¾­µÍÓÚ500ÁË£¬Ôò²»µ÷Õû
			{                                     //ÇÒÁÁ¶È²»ÄÜµÍÓÚ380£¬·ñÔòÈÏÎª¹â¸ÐÃ»ÓÐ·ÅÖÃµ½Î»
					do																//µ×ÏÞÉèÎª350ÊÇÒòÎªÄ³Ð©²úÆ·×î´óÁÁ¶ÈÖ»ÓÐ400,ÇÒÒª¿¼ÂÇsenser±¾ÉíµÄÎó²î
					{
					M2481_PWM_scan(coc);
					DelayMs(1);  
					hfz_light=ave_Bright();
					}while(!((hfz_light>=355)&&(hfz_light<=475)));	
			}	
			Reset_Senser();
			TSL2561Init(S1_addr);
			TSL2561Init(S2_addr);
			TSL2561Init(S3_addr);
			ShowTxt("45,600,10,Íê±Ï£¡");	  DelayMs(150);
}	
	