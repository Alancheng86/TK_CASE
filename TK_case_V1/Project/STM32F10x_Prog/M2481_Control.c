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
extern unsigned short led0pwmval;//��ʼ���ȵ���ֵ

/***************************************
* ������ʹ�ܽ�����
* �ø߿��������ߵ���
***************************************/
void M2481_EN_Pin_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
	
////	/*[��GPIOB ��Ӧ�˿����ó����ģʽ] */
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO����ٶ�50MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
		/*[��GPIOB ��Ӧ�˿����ó����ģʽ] */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO����ٶ�50MHz
	GPIO_Init(GPIOA	, &GPIO_InitStructure);	
	
	GPIO_SetBits(GPIOA,GPIO_Pin_12);
}
/***************************************
* M2481ʹ�ܹܽż�PWM�ܽ�����
*  
***************************************/
void PWM_Pin_configuration()
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
	
	/*[��GPIOB ��Ӧ�˿����ó����ģʽ] */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;      //��Ӧ��ʱ��4ͨ��4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO����ٶ�50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/***************************************
*  PWMʱ������
*  
***************************************/
void TIM4_Configuration(void)
{ 
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  TIM_DeInit(TIM4);		   //��ҪҲ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
  TIM_TimeBaseStructure.TIM_Period=0x008F;	//Ԥװ��ֵ��Ƚϲ���ֵ�������ɷ��䣬��һ������ѹֵ�ɴ�0~���ֵ������Ȼ��ѹ
  TIM_TimeBaseStructure.TIM_Prescaler=1;
  TIM_TimeBaseStructure.TIM_ClockDivision=0;
  TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
 //���ﶨʱ���жϿ��Բ��ã�
//  TIM_ClearFlag(TIM3,TIM_FLAG_Update);	 //ʹ��֮ǰ���֮ǰ������
//  TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
//  TIM_Cmd(TIM3,ENABLE);

 //PWM��ʼ��
  TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable; //����ͨ����Ч
	TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;//ʹ�ܻ��������
//	TIM_OCInitStructure.TIM_Pulse=300;	  //����װ�벶��ȽϼĴ���������ֵ������ȡֵ������0x0000��0xffff֮�䡣
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//������
  TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;        //���������״̬       
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_Low;  ////������� �����������������������
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);	  //�����TIM_OC2�ͺ�����ʹ��ͨ��2

  TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable); // Ԥװ��ʹ��//������ʹ��ͨ��2
  TIM_Cmd(TIM4,ENABLE);

//	 TIM_ARRPreloadConfig(TIM1, ENABLE) ;   //ARPEʹ�� 
//   TIM1->CCER|=1<<4;   //OC2 ���ʹ��
//   TIM_CtrlPWMOutputs(TIM1,ENABLE); //�߼�ʱ������
//   TIM_ITConfig(TIM1, TIM_IT_CC2 , ENABLE); 

}

/***************************************
*  ���е�0--1mA��1--40mA
*  ���Ʊ�������
***************************************/	
 void M2481_PWM_scan(unsigned char duty)
	{ 
	u16 led0pwmval_1ma;
//  static unsigned short led0pwmval=0x0000; //Ĭ��������40mA
  static u8 dir=1;	//dirΪ1���������ǿ���𽥼���
  if(duty==1)		    //40mA ~1.5mA
	{	
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
		DelayMs(1);   
		if(dir) //ֵԽ��Խ��
		{
			if(led0pwmval>=0x0000&&led0pwmval<=0x000e){led0pwmval+=3;}   //0x0000~0x0005��������û�������
			else{led0pwmval++;}
		}	
	
		else 
		{
			if(led0pwmval>0x0003&&led0pwmval<0x000e){led0pwmval-=3;}
			else{led0pwmval--;}	
		}
		
		if(led0pwmval>0x0050)dir=0;     //0x0050��0x008f�ǳ�ʱ��İ�����û��Ҫ�ټ�����ȥ�� 
		if(led0pwmval==0)dir=1;	
    led0pwm=led0pwmval;
		TIM_SetCompare4(TIM4,led0pwmval);   //TIM_SetCompare1 ��Ӧͨ��1  ֵԽ��ռ�ձ�Խ��
	} 
	else if(duty==0)	
	{ 
		led0pwmval_1ma=0x0001;
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
		DelayMs(1);  
	  TIM_SetCompare4(TIM4,led0pwmval_1ma);   //TIM_SetCompare1 ��Ӧͨ��1  ֵԽ��ռ�ձ�Խ��
	}//0.8~1mA
	else if(duty==2)
	{
	  GPIO_SetBits(GPIOA,GPIO_Pin_12);
		DelayMs(1);  
	  TIM_SetCompare4(TIM4,led0pwmval);   //TIM_SetCompare1 ��Ӧͨ��1  ֵԽ��ռ�ձ�Խ�	
	}
}	
	
	
void Adjust_Output_Current(unsigned char duty)
{
			unsigned char coc;
			coc=duty;
	    ShowTxt("45,10,10,���ڵ�������...");	   					DelayMs(150);
			Reset_Senser();
	    Measure_Brightness_Init();
	    DelayMs(10);
			M2481_PWM_scan(coc);         //�״β���
			DelayMs(5); 
			hfz_light=ave_Bright();
			if((hfz_light>470)||(hfz_light<300))  //������������������£��Զ��Ѿ�����500�ˣ��򲻵���
			{                                     //�����Ȳ��ܵ���380��������Ϊ���û�з��õ�λ
					do																//������Ϊ350����ΪĳЩ��Ʒ�������ֻ��400,��Ҫ����senser��������
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
			ShowTxt("45,600,10,��ϣ�");	  DelayMs(150);
}	
	