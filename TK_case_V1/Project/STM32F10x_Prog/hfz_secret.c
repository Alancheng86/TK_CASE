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
*��������void Master_SPI_Configuration(void)
*���ܣ�  ����STM8������ͨ�Źܽ�
*���룺  
*�����
***************************************************/

void Master_SPI_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//��ֹjtag���Կճ�PB3,PB4,PA15
  /*[��?KEYGPIO KEY7/8????3����?��??�꨺?] */
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
*��������void Master_SDA_Input_Config(void)
*���ܣ�  ����STM8������ͨ�Źܽ�
*���룺  
*�����
***************************************************/
void Master_SDA_Input_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 

  /*[��?KEYGPIO KEY7/8????3����?��??�꨺?] */
  GPIO_InitStructure.GPIO_Pin =   Master_PIN_SDA;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;   // 
  GPIO_Init(GPIO_Master, &GPIO_InitStructure);  
}
/**************************************************
*��������void Master_SDA_Output_Config(void)
*���ܣ�  ����STM8������ͨ�Źܽ�
*���룺  
*�����
***************************************************/
void Master_SDA_Output_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 

  /*[��?KEYGPIO KEY7/8????3����?��??�꨺?] */
  GPIO_InitStructure.GPIO_Pin =   Master_PIN_SDA;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP; // 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 
  GPIO_Init(GPIO_Master, &GPIO_InitStructure);  
	GPIO_SetBits(GPIOB,Master_PIN_SDA);
}


/**************************************************
*��������void Master_Send_Data(unsigned short data)
*���ܣ�  ������STM8�����ܷ���16λADC�����
*���룺  16λADC�����
*����� 
***************************************************/
void Master_Send_Data(unsigned short data)
{
	   unsigned char  flag,temp,numi,tep0;
	   unsigned short count=0;
     Master_SDA_Output_Config();
     Master_send_ACK();  //����C S	.ʹ�ӻ������ж�
//     Dly_1ms(2);	
//     MCS_L;	
	   Master_Send_Sync_ACK();  //����ͬ���źţ�����SCL��������

	   Dly_1ms(10);        //����ʱ�𶯣�
	   numi=16;
	   do
		 {
			if((data&0x8000)!=0)
			{MSDA_H;}	//����?��1
      else
			{MSDA_L;}		//����?��0		
			Dly_1ms(5);
			data<<=1;				
			MSCL_H;	
			Dly_1ms(5);
	    MSCL_L;	
			Dly_1ms(5);
			MSDA_H;//��֤����״̬�£�SDAΪ����״̬
		 }while(--numi);	 
}

/**************************************************
*��������unsigned char Master_Receive_Data(void)
*���ܣ�  ������������STM8���ص�8λ����
*���룺  
*�����  8λ��������
***************************************************/
unsigned char Master_Receive_Data(void)
{
	   unsigned char  flag,temp,numi;
	   unsigned short count=0;
	   unsigned char  Data=0;
	
	   Master_SDA_Input_Config();
	   Dly_1ms(10);  //����ʱ�𶯣�
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
*��������void Master_Receive_Sync_ACK(void)
*���ܣ�  ��ȡͬ���źţ����ȴ��ӻ���SDA���ͣ�����һֱ�ȴ�
*���룺  
*�����  
***************************************************/
void Master_Receive_Sync_ACK(void)  //�ȴ��ӻ���SDA���ͣ�����һֱ�ȴ�
{
	unsigned short count=0;
	unsigned char i;
  Master_SDA_Input_Config();   //SDA�߽�����������
	
	MSCL_H;Dly_1ms(35);
  
	i=1;
	while(i!=0)   //�ȴ��ӻ�ͬ���ź� ,����SDA������
	{
	 i=MSDAx;
	}
	MSCL_L;Dly_1ms(5);	
	Master_SDA_Output_Config();
}

/**************************************************
*��������void Master_Send_Sync_ACK(void)
*���ܣ�  ��������ȷ���ź�	,��������SCL����������
*���룺  
*�����  
***************************************************/
void Master_Send_Sync_ACK(void)  //����ȷ���ź�	,��������SCL����������
{
	MSDA_L;
}

/**************************************************
*��������void Master_send_ACK(void) 
*���ܣ�  ����STM8�����жϣ�STM8�����ش�����
*���룺  
*�����  
***************************************************/
void Master_send_ACK(void)  //�䨮?��?����?��|��eD?o?
{
	MCS_H;	
}
/**************************************************
*��������void Master_send_NACK(void)
*���ܣ�  ���������жϹܽ�STM8�����ж�
*���룺  
*�����  
***************************************************/
void Master_send_NACK(void)  //�䨮?��?����?��|��eD?o?
{
	MCS_L;	
}
/**************************************************
*��������void Dly_1us (unsigned int nus)
*���ܣ�  ΢����ʱ
*���룺  
*�����  
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
*��������void Dly_1ms (unsigned int nms)	
*���ܣ�  ������ʱ
*���룺  
*�����  
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
*��������void Send_Key_code(unsigned short data)	
*���ܣ�  ����ADC����16λ�����
*���룺  
*�����  
***************************************************/
void Send_Key_code(unsigned short data)
{
	ADC_Keyscan(); 
}
/**************************************************
*��������void ADC1_GPIO_Config(void)
*���ܣ�  ADC�˿�����
*���룺  
*�����  
***************************************************/
void ADC1_GPIO_Config(void)
{
 GPIO_InitTypeDef GPIO_InitStructure; 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 
	
////////////////////ģ������  ADC6��ӦPA6   
  GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_6;		//ADC1 ͨ��1
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AIN;    // 
  GPIO_Init(GPIOA, &GPIO_InitStructure);       
}
/**************************************************
*��������void ADC1_Configuration(void)
*���ܣ�  ADC����
*���룺  
*�����  
***************************************************/
void ADC1_Configuration(void)
{

 NVIC_InitTypeDef NVIC_InitStructure;
 ADC_InitTypeDef ADC_InitStructure;

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
 RCC_ADCCLKConfig(RCC_PCLK2_Div6);//????ADC����?��=PCLK2/6=12MHz
 ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//ADC1o��ADC21�����?��?������1�����?�꨺?
 ADC_InitStructure.ADC_ScanConvMode=DISABLE;   //�̣������̨�����?��
 ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;	   //��?D?����?��
 ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;  //����?t�䣤����
 ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;   //����???? 
 ADC_InitStructure.ADC_NbrOfChannel=1;  //����?�������̨���y��??������?�������̨�6
 ADC_Init(ADC1,&ADC_InitStructure); 

		   //��1?��???����??D?��o��?��2?2???��??1�����̨�
//	if(i==0)
// ADC_RegularChannelConfig(ADC1,ADC_Channel_6,1,ADC_SampleTime_55Cycles5);  //2��?��?��?����???��??������?����?2��?��?�̡�???D??��????�䨮
//	else
//	ADC_RegularChannelConfig(ADC1,ADC_Channel_7,1,ADC_SampleTime_55Cycles5);	
// ADC_DiscModeChannelCountConfig(ADC1,1);
// ADC_DiscModeCmd(ADC1,ENABLE);
 ADC_Cmd(ADC1,ENABLE);
// ADC_DMACmd(ADC1,ENABLE);             //��1?��?��??����?��???����?ADC 
 ADC_ResetCalibration(ADC1);      //???????����?ADCD���???��??��
 while(ADC_GetResetCalibrationStatus(ADC1)!=RESET);  //�̨���y����3��
 ADC_StartCalibration(ADC1);    //?a��????��ADC��?D���?���䨬?
 while(ADC_GetCalibrationStatus(ADC1)!=RESET);   //�̨���y����3��

}

unsigned short ADC_Keyscan(void) 
{
  u16 temp; 	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);  //?��2????����y?Y��a??
	while(ADC_GetSoftwareStartConvStatus(ADC1)!=RESET);  //�̨���y???������3��
  
	while(!(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)));  //?a1��������?��������?
	temp=ADC_GetConversionValue(ADC1); 	
	return temp;
} 

/**************************************************
*��������unsigned char Cali_methods(unsigned short data)
*���ܣ�  hfz
*���룺  
*�����  
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
*��������unsigned char Cali_methods(unsigned short data)
*���ܣ�  hfz
*���룺  
*�����  
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
*��������unsigned char Cali_methods(unsigned short data)
*���ܣ�  hfz
*���룺  
*�����  
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
*��������unsigned char Cali_methods(unsigned short data)
*���ܣ�  hfz
*���룺  
*�����  
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
*��������unsigned char Cali_methods(unsigned short data)
*���ܣ�  hfz
*���룺  
*�����  
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
*��������unsigned char Cali_methods(unsigned short data)
*���ܣ�  hfz
*���룺  
*�����  
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
*��������unsigned char Secret_Dolby(void)
*���ܣ�  ���ļ��У�ֻ��Ҫ���ô˺������ɣ��������ù�
*���룺  
*�����  1��ʾ���ֳɹ���0��ʾ�����Լҵ�
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

