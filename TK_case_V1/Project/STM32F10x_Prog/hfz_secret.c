/******************** (C) COPYRIGHT 2010 KSGIANTLUS ********************
* File Name          : lcd.c
* Author             : JASON
* Version            : V1.0
* Date               : 2010.01.20
*************************************************************************/
/* Includes ------------------------------------------------------------------*/


#include "stm32f10x.h"
#include "hfz_secret.h"
#include <stdio.h>

/**************************************************
*函数名：void Master_SPI_Configuration(void)
*功能：  配置STM8的秘密通信管脚
*输入：  
*输出：
***************************************************/

void Master_SPI_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//禁止jtag，以空出PB3,PB4,PA15
  /*[°?KEYGPIO KEY7/8????3éê?è??￡ê?] */
  GPIO_InitStructure.GPIO_Pin =   Master_PIN_SDA|Master_PIN_SCL|Master_PIN_CS|Master_PIN_RET;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP; // 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 
  GPIO_Init(GPIO_Master, &GPIO_InitStructure);  
	GPIO_ResetBits(GPIOB,Master_PIN_CS);
	GPIO_SetBits(GPIOB,Master_PIN_RET);
	GPIO_SetBits(GPIOB,Master_PIN_SDA);
	GPIO_SetBits(GPIOB,Master_PIN_SCL);
	
}

/**************************************************
*函数名：void Master_SDA_Input_Config(void)
*功能：  配置STM8的秘密通信管脚
*输入：  
*输出：
***************************************************/
void Master_SDA_Input_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 

  /*[°?KEYGPIO KEY7/8????3éê?è??￡ê?] */
  GPIO_InitStructure.GPIO_Pin =   Master_PIN_SDA;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;   // 
  GPIO_Init(GPIO_Master, &GPIO_InitStructure);  
}
/**************************************************
*函数名：void Master_SDA_Output_Config(void)
*功能：  配置STM8的秘密通信管脚
*输入：  
*输出：
***************************************************/
void Master_SDA_Output_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 

  /*[°?KEYGPIO KEY7/8????3éê?è??￡ê?] */
  GPIO_InitStructure.GPIO_Pin =   Master_PIN_SDA;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP; // 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 
  GPIO_Init(GPIO_Master, &GPIO_InitStructure);  
	GPIO_SetBits(GPIOB,Master_PIN_SDA);
}


/**************************************************
*函数名：void Master_Send_Data(unsigned short data)
*功能：  主机向STM8的秘密发送16位ADC随机数
*输入：  16位ADC随机数
*输出： 
***************************************************/
void Master_Send_Data(unsigned short data)
{
	   unsigned char  flag,temp,numi,tep0;
	   unsigned short count=0;
     Master_SDA_Output_Config();
     Master_send_ACK();  //拉高C S	.使从机进入中断
//     Dly_1ms(2);	
//     MCS_L;	
	   Master_Send_Sync_ACK();  //发送同步信号，即将SCL重新拉低

	   Dly_1ms(10);        //此延时勿动！
	   numi=16;
	   do
		 {
			if((data&0x8000)!=0)
			{MSDA_H;}	//·￠?í1
      else
			{MSDA_L;}		//·￠?í0		
			Dly_1ms(5);
			data<<=1;				
			MSCL_H;	
			Dly_1ms(5);
	    MSCL_L;	
			Dly_1ms(5);
			MSDA_H;//保证闲置状态下，SDA为拉高状态
		 }while(--numi);	 
}

/**************************************************
*函数名：unsigned char Master_Receive_Data(void)
*功能：  主机接收来自STM8返回的8位数据
*输入：  
*输出：  8位处理后的数
***************************************************/
unsigned char Master_Receive_Data(void)
{
	   unsigned char  flag,temp,numi;
	   unsigned short count=0;
	   unsigned char  Data=0;
	
	   Master_SDA_Input_Config();
	   Dly_1ms(10);  //此延时勿动！
	   numi=8;
	   do
		 {
		   MSCL_H;
       Dly_1ms(5);
		
			 Data<<=1;		
       temp=MSDAx;			
			 if(temp!=0)
			 {Data+=0x01;}
		   MSCL_L;
       Dly_1ms(5);		
			 
		 }while(--numi);
     Master_SDA_Output_Config();		 
     return Data;
}

/**************************************************
*函数名：void Master_Receive_Sync_ACK(void)
*功能：  获取同步信号，即等待从机将SDA拉低，否则一直等待
*输入：  
*输出：  
***************************************************/
void Master_Receive_Sync_ACK(void)  //等待从机将SDA拉低，否则一直等待
{
	unsigned short count=0;
	unsigned char i;
  Master_SDA_Input_Config();   //SDA线接有上拉电阻
	
	MSCL_H;Dly_1ms(35);
  
	i=1;
	while(i!=0)   //等待从机同步信号 ,即将SDA线拉低
	{
	 i=MSDAx;
	}
	MSCL_L;Dly_1ms(5);	
	Master_SDA_Output_Config();
}

/**************************************************
*函数名：void Master_Send_Sync_ACK(void)
*功能：  主机发送确认信号	,即主机将SCL线重新拉低
*输入：  
*输出：  
***************************************************/
void Master_Send_Sync_ACK(void)  //主机确认信号	,即主机将SCL线重新拉低
{
	MSDA_L;
}

/**************************************************
*函数名：void Master_send_ACK(void) 
*功能：  触发STM8进入中断（STM8上升沿触发）
*输入：  
*输出：  
***************************************************/
void Master_send_ACK(void)  //′ó?ú?óê?ó|′eD?o?
{
	MCS_H;	
}
/**************************************************
*函数名：void Master_send_NACK(void)
*功能：  重新拉低中断管脚STM8进入中断
*输入：  
*输出：  
***************************************************/
void Master_send_NACK(void)  //′ó?ú?óê?ó|′eD?o?
{
	MCS_L;	
}
/**************************************************
*函数名：void Dly_1us (unsigned int nus)
*功能：  微秒延时
*输入：  
*输出：  
***************************************************/
void Dly_1us (unsigned int nus)
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
/**************************************************
*函数名：void Dly_1ms (unsigned int nms)	
*功能：  毫秒延时
*输入：  
*输出：  
***************************************************/
void Dly_1ms (unsigned int nms)			 //??1ms?
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
*函数名：void Send_Key_code(unsigned short data)	
*功能：  利用ADC产生16位随机数
*输入：  
*输出：  
***************************************************/
void Send_Key_code(unsigned short data)
{
	ADC_Keyscan(); 
}
/**************************************************
*函数名：void ADC1_GPIO_Config(void)
*功能：  ADC端口配置
*输入：  
*输出：  
***************************************************/
void ADC1_GPIO_Config(void)
{
 GPIO_InitTypeDef GPIO_InitStructure; 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 
	
////////////////////模拟输入  ADC6对应PA6   
  GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_6;		//ADC1 通道1
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AIN;    // 
  GPIO_Init(GPIOA, &GPIO_InitStructure);       
}
/**************************************************
*函数名：void ADC1_Configuration(void)
*功能：  ADC配置
*输入：  
*输出：  
***************************************************/
void ADC1_Configuration(void)
{

 NVIC_InitTypeDef NVIC_InitStructure;
 ADC_InitTypeDef ADC_InitStructure;

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
 RCC_ADCCLKConfig(RCC_PCLK2_Div6);//????ADCê±?ó=PCLK2/6=12MHz
 ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//ADC1oíADC21¤×÷?ú?àá￠1¤×÷?￡ê?
 ADC_InitStructure.ADC_ScanConvMode=DISABLE;   //μ￥í¨μàé¨?è
 ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;	   //á?D?é¨?è
 ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;  //èí?t′￥·￠
 ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;   //óò???? 
 ADC_InitStructure.ADC_NbrOfChannel=1;  //é¨?èí¨μàêy￡??íé¨?èí¨μà6
 ADC_Init(ADC1,&ADC_InitStructure); 

		   //ê1?ü???è′??D?÷oí?ú2?2???μ??1í¨μà
//	if(i==0)
// ADC_RegularChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_55Cycles5);  //2é?ù?ü?úμ???±??áó°?ìμ?2é?ù?μ￡???D??μ????′ó
//	else
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,1,ADC_SampleTime_55Cycles5);	
// ADC_DiscModeChannelCountConfig(ADC1,1);
// ADC_DiscModeCmd(ADC1,ENABLE);
 ADC_Cmd(ADC1,ENABLE);
// ADC_DMACmd(ADC1,ENABLE);             //ê1?ü?ò??ê§?ü???¨μ?ADC 
 ADC_ResetCalibration(ADC1);      //???????¨μ?ADCD￡×???′??÷
 while(ADC_GetResetCalibrationStatus(ADC1)!=RESET);  //μè′yíê3é
 ADC_StartCalibration(ADC1);    //?aê????¨ADCμ?D￡×?×′ì?
 while(ADC_GetCalibrationStatus(ADC1)!=RESET);   //μè′yíê3é

}

unsigned short ADC_Keyscan(void) 
{
  u16 temp; 	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);  //?ú2????ˉêy?Y×a??
	while(ADC_GetSoftwareStartConvStatus(ADC1)!=RESET);  //μè′y???ˉíê3é
  
	while(!(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)));  //?a1ê±é¨?èíê±?
	temp=ADC_GetConversionValue(ADC1); 	
	return temp;
} 

/**************************************************
*函数名：unsigned char Cali_methods(unsigned short data)
*功能：  hfz
*输入：  
*输出：  
***************************************************/

unsigned char Cali_methods1(unsigned short data)
{
   unsigned char GVO;
   unsigned char tep1,tep2;
   data+=0x1912;
   tep1=data&0x0f;
   tep2=(data>>12)&0x0f;
   GVO=(tep1<<4)|(tep2);  
   return GVO;
}
/**************************************************
*函数名：unsigned char Cali_methods(unsigned short data)
*功能：  hfz
*输入：  
*输出：  
***************************************************/

unsigned char Cali_methods2(unsigned short data)
{
   unsigned char GVO;
   unsigned char tep1,tep2;
   data+=0x1234;
   tep1=data&0x0f;
   tep2=(data>>12)&0x0f;
   GVO=(tep2<<4)|(tep1);
   return GVO;
}
/**************************************************
*函数名：unsigned char Cali_methods(unsigned short data)
*功能：  hfz
*输入：  
*输出：  
***************************************************/
unsigned char Cali_methods3(unsigned short data)
{
   unsigned char GVO;
   unsigned char tep1,tep2;
   data+=0xc0c0;
   tep1=(data>>4)&0x0f;
   tep2=(data>>8)&0x0f;
   GVO=(tep1<<4)|(tep2);
   return GVO;
}
/**************************************************
*函数名：unsigned char Cali_methods(unsigned short data)
*功能：  hfz
*输入：  
*输出：  
***************************************************/
unsigned char Cali_methods4(unsigned short data)
{
   unsigned char GVO;
   unsigned char tep1,tep2;
   data+=0x1314;
   tep1=(data>>4)&0x0f;
   tep2=(data>>8)&0x0f;
   GVO=(tep2<<4)|(tep1);
   return GVO;
}
/**************************************************
*函数名：unsigned char Cali_methods(unsigned short data)
*功能：  hfz
*输入：  
*输出：  
***************************************************/
unsigned char Cali_methods5(unsigned short data)
{
   unsigned char GVO;
   unsigned char tep1,tep2;
   data+=0x4321;
   tep1=data>>12&0x0f;
   tep2=(data>>8)&0x0f;
   GVO=(tep2<<4)|(tep1);
   return GVO;
}
/**************************************************
*函数名：unsigned char Cali_methods(unsigned short data)
*功能：  hfz
*输入：  
*输出：  
***************************************************/
unsigned char Random_convert(unsigned char num,unsigned short data) 
{
	  unsigned char Cortex;
	  unsigned char temp;
    temp=num;	
		switch(temp)
		{
			case 0: Cortex=Cali_methods1(data);  break;
			case 1: Cortex=Cali_methods1(data);  break;
			case 2: Cortex=Cali_methods2(data);  break;
		  case 3: Cortex=Cali_methods2(data);  break;
      case 4: Cortex=Cali_methods3(data);  break;
      case 5:	Cortex=Cali_methods3(data);  break; 
			case 6: Cortex=Cali_methods4(data);  break;
		  case 7: Cortex=Cali_methods4(data);  break;
      case 8: Cortex=Cali_methods5(data);  break;
      case 9:	Cortex=Cali_methods5(data);  break; 				
		}
	return Cortex;	
}
/**************************************************
*函数名：unsigned char Secret_Dolby(void)
*功能：  此文件中，只需要调用此函数即可，其他不用管
*输入：  
*输出：  1表示握手成功，0表示不是自家的
***************************************************/
unsigned char Secret_Dolby(void)
{
	unsigned char HHH,SSS,JJJ,CCC;
	unsigned short GGG=0;	
	
	Master_SPI_Configuration();
	Master_SDA_Input_Config();

	GPIO_ResetBits(GPIOB,Master_PIN_RET);
	Dly_1ms(10);	
	GPIO_SetBits(GPIOB,Master_PIN_RET);
  Dly_1ms(10);
	
  ADC1_GPIO_Config();
	ADC1_Configuration();
  GGG=ADC_Keyscan();
	Master_Send_Data(GGG);
	HHH=GGG%10; 

	SSS=Random_convert(HHH,GGG); 
	//-------------------------------
	Master_Receive_Sync_ACK();
	
	CCC=Master_Receive_Data();
	if(CCC==SSS)
	{
			JJJ=1;
	}
	else
	{
			JJJ=0;
	}
	Master_send_NACK();
	return JJJ;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

