/******************** (C) COPYRIGHT 2010 KSGIANTLUS ********************
* File Name          : lcd.c
* Author             : JASON
* Version            : V1.0
* Date               : 2010.01.20
*************************************************************************/
/* Includes ------------------------------------------------------------------*/


#include "stm32f10x.h"
#include "platform_config.h"
#include "Mtransmit.h"
#include <stdio.h>
#include <lcd.h>
#include "string.h"


unsigned short Receive_flicker(void);
unsigned short Test_table[30]={0};
unsigned short GGGG;
/**************************************************
*函数名：void Senser_Master_SPI_Configuration(void)
*功能：  胡凤章的秘密通信管脚配置
*输入：  
*输出： 
***************************************************/
void Senser_Master_SPI_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 

  /*[°?KEYGPIO KEY7/8????3éê?è??￡ê?] */
  GPIO_InitStructure.GPIO_Pin =   Senser_Master_PIN_SDA;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP; // 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 
  GPIO_Init(Senser_GPIO_Master_SDA, &GPIO_InitStructure);  
	GPIO_ResetBits(Senser_GPIO_Master_SDA,Senser_Master_PIN_SDA);
	
  GPIO_InitStructure.GPIO_Pin =   Senser_Master_PIN_SCL;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP; // 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 
  GPIO_Init(Senser_GPIO_Master_SCL, &GPIO_InitStructure);  
	
	DelayMs(15);
	GPIO_ResetBits(Senser_GPIO_Master_SCL,Senser_Master_PIN_SCL);
}
/**************************************************
*函数名：void Senser_Master_SDA_Input_Config(void)
*功能：  胡凤章的秘密通信管脚配置
*输入：  
*输出： 
***************************************************/
void Senser_Master_SDA_Input_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 

  /*[°?KEYGPIO KEY7/8????3éê?è??￡ê?] */
  GPIO_InitStructure.GPIO_Pin =   Senser_Master_PIN_SDA;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;   // 
  GPIO_Init(Senser_GPIO_Master_SDA, &GPIO_InitStructure);  
}
/**************************************************
*函数名：void Senser_Master_SDA_Output_Config(void)
*功能：  胡凤章的秘密通信管脚配置
*输入：  
*输出： 
***************************************************/
void Senser_Master_SDA_Output_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 

  /*[°?KEYGPIO KEY7/8????3éê?è??￡ê?] */
  GPIO_InitStructure.GPIO_Pin =   Senser_Master_PIN_SDA;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP; // 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; // 
  GPIO_Init(Senser_GPIO_Master_SDA, &GPIO_InitStructure);  
	GPIO_SetBits(Senser_GPIO_Master_SDA,Senser_Master_PIN_SDA);
}

/*	====================== <<<<<    EEPROM	 >>>>>  ================	
*			0x20 : 	 校准系数								 
      0x21 :   校准系数

      0x30 ：  工作模式
			0x31 :	 测量对象 								  
/*********************************************************************
/*********************************************************************
*	====================== <<<<<    指令表	 >>>>>  ================	
*												 <<<遥控器>>>>                      设定会被保存
*	Assignment	A指令集  ：  0xA0     自由检测模式设定 			 												 
*				      		 				 0xA1     主从通信模式设定
                           0xA2     测闪动设定
													 0xA3     测亮度设定
                         ==============================
                         <<< 测机 >>>>  (主从通信模式使用)，设定并不会被保存
                           0xA4     主从模式测闪动设定
                           0xA5     主从模式测亮度设定
													 0xA6     自由模式测闪动设定
													 0xA7     自由模式测亮度设定			   
*	Assignment	B指令集  ： 
													 0xB0     显示we love hfz
                           0xB1     显示程序版本
													 0xB2     显示当前工作模式 
									         0xB3     显示你个呆笔！！
													 0xB4     显示你个呆笔！！
*	Assignment	c指令集  ：  
													 0xC0     光感初始化  
													 0xC1     测量filcker(自由检测模式推荐使用)
													 0xC2     测量亮度 (自由检测模式推荐使用)
							 				   	 0xC3     测量filcker，并返回数据		(主从通信模式使用)
													 0xC4     测量light ,并返回数据     (主从通信模式使用) 
*********************************************************************/

/**************************************************
*函数名：void Senser_Master_Send_Data(unsigned char data)
*功能：  向胡凤章的秘密 发送指令
*输入：  八位指令
*输出： 
***************************************************/
void Senser_Master_Send_Data(unsigned char data)
{
	   unsigned char  flag,temp,numi,tep0;
	   unsigned short count=0;

     Senser_Master_send_ACK();  //拉高SCL.使从机进入中断	   
	   Senser_Master_Send_Sync_ACK();  //发送同步信号，即将SCL重新拉低

	   Senser_Dly_1ms(5);        //此延时勿动！
	   numi=8;
	   do
		 {
			if(data&0x80)
			{Senser_MSDA_H;}	//·￠?í1
      else
			{Senser_MSDA_L;}		//·￠?í0		
			data<<=1;				
			Senser_MSCL_H;	
			Senser_Dly_1ms(2);  //此延时不得超过10ms
	    Senser_MSCL_L;	
			Senser_Dly_1ms(2);  //此延时不得超过10ms
			Senser_MSDA_H;//保证闲置状态下，SDA为拉高状态
		 }while(--numi);	 
}

/**************************************************
*函数名：unsigned char Senser_Master_Receive_Data(void)
*功能： 接收胡凤章的秘密 返回的采样数据
*输入：  
*输出： 接收8位数据
***************************************************/
unsigned char Senser_Master_Receive_Data(void)
{
	   unsigned char  flag,temp,numi,Data=0;
	   unsigned short count=0;
	   Senser_Master_SDA_Input_Config();
	   Senser_Dly_1ms(5);  //此延时勿动！
	   numi=8;
	   do
		 {
		   Senser_MSCL_H;
       Senser_Dly_1ms(2); //此延时不得超过10ms
		
			 Data<<=1;		
       temp=Senser_MSDAx;			
			 if(temp!=0)
			 {Data+=0x01;}
			
		   Senser_MSCL_L;
       Senser_Dly_1ms(2);	//此延时不得超过10ms	
			 
		 }while(--numi);
     Senser_Master_SDA_Output_Config();		 
     return Data;
}

/**************************************************
*函数名：unsigned char Senser_Master_Receive_Sync_ACK(void)
*功能： 接收胡凤章的秘密 发来的同步信号，即将SDA拉低，否则一直等待
*输入：  
*输出： 
***************************************************/
unsigned char Senser_Master_Receive_Sync_ACK(void)  //等待从机将SDA拉低，否则一直等待
{
	unsigned short count=0;
	unsigned char i;
	Senser_Master_SDA_Input_Config();   //SDA线接有上拉电阻
	i=1;   //Senser_Dly_1ms(1);
	Senser_MSCL_H;
  Senser_Dly_1ms(6);
	while(i!=0)   //等待从机同步信号 ,即将SDA线拉低
	{
	 i=Senser_MSDAx;
	}
	Senser_MSCL_L;
  Senser_Dly_1ms(1);	
	Senser_Master_SDA_Output_Config();
}

/**************************************************
*函数名：void Senser_Master_Send_Sync_ACK(void) 
*功能： 发送主机确认信号
*输入：  
*输出： 
***************************************************/
void Senser_Master_Send_Sync_ACK(void)  //主机确认信号	,即等待主机将SCL线重新拉低
{
	Senser_MSCL_L;
}

/**************************************************
*函数名：unsigned char Senser_Master_send_ACK(void) 
*功能： 发送同步信号，即使胡凤章的秘密 进入中断，接收指令
*输入：  
*输出： 
***************************************************/
unsigned char Senser_Master_send_ACK(void)  //′ó?ú?óê?ó|′eD?o?
{
	Senser_MSCL_H;	
}

void Senser_Dly_1us (u32 nus)
{
	u32 temp;	 
    SysTick->CTRL&=0xfffffffb;//bit2??,??????  HCLK/8	
	SysTick->LOAD=nus*(72/8); //????	  		 
	SysTick->VAL=0x00;        //?????
	SysTick->CTRL=0x01 ;      //???? 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//??????   
	SysTick->CTRL=0x00;       //?????
	SysTick->VAL =0X00;       //?????	 
}
void Senser_Dly_1ms (u32 nms)			 //??1ms?
{
	u32 temp;
  SysTick->CTRL&=0xfffffffb;//bit2??,??????  HCLK/8	
	SysTick->LOAD=(u32)nms*(72/8)*1000;//????(SysTick->LOAD?24bit)
	SysTick->VAL =0x00;           //?????
	SysTick->CTRL=0x01 ;          //????  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//??????   
	SysTick->CTRL=0x00;       //?????
	SysTick->VAL =0X00;       //?????	  	
}

/**************************************************
*函数名：unsigned short ave_Bright(void)
*功能：  获取light 值
*输入：  
*输出： 
***************************************************/

unsigned short ave_Bright(void)
{
	  unsigned short GGGG;
    unsigned char eer1,eer2;
	
    Senser_Master_Send_Data(0xc4);
		Senser_Master_Receive_Sync_ACK(); //??????,??????
		eer1=Senser_Master_Receive_Data();
		eer2=Senser_Master_Receive_Data();
		Senser_Dly_1ms(1);	
		GGGG=(eer1<<8)+eer2;			
    return 	GGGG;
}
/**************************************************
*函数名  void Measure_Brightness_Init(void)
*功能：  配成主从通信模式，测量亮度
*输入：  
*输出： 
***************************************************/
void Measure_Brightness_Init(void)
{		
		Senser_Master_Send_Data(0xA5);		
		Senser_Dly_1ms(25);			

		Senser_Master_Send_Data(0xC0);				
		Senser_Dly_1ms(25);	
}


/**************************************************
*函数名：unsigned short Receive_flicker(void)
*功能：  获取flicker 值
*输入：  
*输出： 
***************************************************/

unsigned short Receive_flicker(void)
{
	  
    unsigned char eer1,eer2;
	
    Senser_Master_Send_Data(0xc3);
		Senser_Master_Receive_Sync_ACK(); //获取同步信号，准备接收数据
		eer1=Senser_Master_Receive_Data();
		eer2=Senser_Master_Receive_Data();
		Senser_Dly_1ms(2);	
		GGGG=(eer1<<8)+eer2;			
    return 	GGGG;
}
/**************************************************
*函数名：void Niubee_Senser_initial(void)
*功能：  配置成主从通信模式
*输入：  自动OTP前需调用
*输出： 
***************************************************/
void Niubee_Senser_initial(void)
{		
		Senser_Master_Send_Data(0xA4);		
		Senser_Dly_1ms(25);			

		Senser_Master_Send_Data(0xC0);				
		Senser_Dly_1ms(25);	
}
/**************************************************
*函数名：void AUT_Flicker_Senser_initial(void)
*功能：  配置成自由检测模式
*输入：  手动OTP前需调用
*输出： 
***************************************************/

void AUT_Flicker_Senser_initial(void)
{
		Senser_Master_Send_Data(0xA6);		
		Senser_Dly_1ms(25);			
		Senser_Master_Send_Data(0xC0);				
		Senser_Dly_1ms(25);	
}

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
