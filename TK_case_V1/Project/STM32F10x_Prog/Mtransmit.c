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
*��������void Senser_Master_SPI_Configuration(void)
*���ܣ�  �����µ�����ͨ�Źܽ�����
*���룺  
*����� 
***************************************************/
void Senser_Master_SPI_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 

  /*[��?KEYGPIO KEY7/8????3����?��??�꨺?] */
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
*��������void Senser_Master_SDA_Input_Config(void)
*���ܣ�  �����µ�����ͨ�Źܽ�����
*���룺  
*����� 
***************************************************/
void Senser_Master_SDA_Input_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 

  /*[��?KEYGPIO KEY7/8????3����?��??�꨺?] */
  GPIO_InitStructure.GPIO_Pin =   Senser_Master_PIN_SDA;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;   // 
  GPIO_Init(Senser_GPIO_Master_SDA, &GPIO_InitStructure);  
}
/**************************************************
*��������void Senser_Master_SDA_Output_Config(void)
*���ܣ�  �����µ�����ͨ�Źܽ�����
*���룺  
*����� 
***************************************************/
void Senser_Master_SDA_Output_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); 

  /*[��?KEYGPIO KEY7/8????3����?��??�꨺?] */
  GPIO_InitStructure.GPIO_Pin =   Senser_Master_PIN_SDA;
  GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP; // 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; // 
  GPIO_Init(Senser_GPIO_Master_SDA, &GPIO_InitStructure);  
	GPIO_SetBits(Senser_GPIO_Master_SDA,Senser_Master_PIN_SDA);
}

/*	====================== <<<<<    EEPROM	 >>>>>  ================	
*			0x20 : 	 У׼ϵ��								 
      0x21 :   У׼ϵ��

      0x30 ��  ����ģʽ
			0x31 :	 �������� 								  
/*********************************************************************
/*********************************************************************
*	====================== <<<<<    ָ���	 >>>>>  ================	
*												 <<<ң����>>>>                      �趨�ᱻ����
*	Assignment	Aָ�  ��  0xA0     ���ɼ��ģʽ�趨 			 												 
*				      		 				 0xA1     ����ͨ��ģʽ�趨
                           0xA2     �������趨
													 0xA3     �������趨
                         ==============================
                         <<< ��� >>>>  (����ͨ��ģʽʹ��)���趨�����ᱻ����
                           0xA4     ����ģʽ�������趨
                           0xA5     ����ģʽ�������趨
													 0xA6     ����ģʽ�������趨
													 0xA7     ����ģʽ�������趨			   
*	Assignment	Bָ�  �� 
													 0xB0     ��ʾwe love hfz
                           0xB1     ��ʾ����汾
													 0xB2     ��ʾ��ǰ����ģʽ 
									         0xB3     ��ʾ������ʣ���
													 0xB4     ��ʾ������ʣ���
*	Assignment	cָ�  ��  
													 0xC0     ��г�ʼ��  
													 0xC1     ����filcker(���ɼ��ģʽ�Ƽ�ʹ��)
													 0xC2     �������� (���ɼ��ģʽ�Ƽ�ʹ��)
							 				   	 0xC3     ����filcker������������		(����ͨ��ģʽʹ��)
													 0xC4     ����light ,����������     (����ͨ��ģʽʹ��) 
*********************************************************************/

/**************************************************
*��������void Senser_Master_Send_Data(unsigned char data)
*���ܣ�  ������µ����� ����ָ��
*���룺  ��λָ��
*����� 
***************************************************/
void Senser_Master_Send_Data(unsigned char data)
{
	   unsigned char  flag,temp,numi,tep0;
	   unsigned short count=0;

     Senser_Master_send_ACK();  //����SCL.ʹ�ӻ������ж�	   
	   Senser_Master_Send_Sync_ACK();  //����ͬ���źţ�����SCL��������

	   Senser_Dly_1ms(5);        //����ʱ�𶯣�
	   numi=8;
	   do
		 {
			if(data&0x80)
			{Senser_MSDA_H;}	//����?��1
      else
			{Senser_MSDA_L;}		//����?��0		
			data<<=1;				
			Senser_MSCL_H;	
			Senser_Dly_1ms(2);  //����ʱ���ó���10ms
	    Senser_MSCL_L;	
			Senser_Dly_1ms(2);  //����ʱ���ó���10ms
			Senser_MSDA_H;//��֤����״̬�£�SDAΪ����״̬
		 }while(--numi);	 
}

/**************************************************
*��������unsigned char Senser_Master_Receive_Data(void)
*���ܣ� ���պ����µ����� ���صĲ�������
*���룺  
*����� ����8λ����
***************************************************/
unsigned char Senser_Master_Receive_Data(void)
{
	   unsigned char  flag,temp,numi,Data=0;
	   unsigned short count=0;
	   Senser_Master_SDA_Input_Config();
	   Senser_Dly_1ms(5);  //����ʱ�𶯣�
	   numi=8;
	   do
		 {
		   Senser_MSCL_H;
       Senser_Dly_1ms(2); //����ʱ���ó���10ms
		
			 Data<<=1;		
       temp=Senser_MSDAx;			
			 if(temp!=0)
			 {Data+=0x01;}
			
		   Senser_MSCL_L;
       Senser_Dly_1ms(2);	//����ʱ���ó���10ms	
			 
		 }while(--numi);
     Senser_Master_SDA_Output_Config();		 
     return Data;
}

/**************************************************
*��������unsigned char Senser_Master_Receive_Sync_ACK(void)
*���ܣ� ���պ����µ����� ������ͬ���źţ�����SDA���ͣ�����һֱ�ȴ�
*���룺  
*����� 
***************************************************/
unsigned char Senser_Master_Receive_Sync_ACK(void)  //�ȴ��ӻ���SDA���ͣ�����һֱ�ȴ�
{
	unsigned short count=0;
	unsigned char i;
	Senser_Master_SDA_Input_Config();   //SDA�߽�����������
	i=1;   //Senser_Dly_1ms(1);
	Senser_MSCL_H;
  Senser_Dly_1ms(6);
	while(i!=0)   //�ȴ��ӻ�ͬ���ź� ,����SDA������
	{
	 i=Senser_MSDAx;
	}
	Senser_MSCL_L;
  Senser_Dly_1ms(1);	
	Senser_Master_SDA_Output_Config();
}

/**************************************************
*��������void Senser_Master_Send_Sync_ACK(void) 
*���ܣ� ��������ȷ���ź�
*���룺  
*����� 
***************************************************/
void Senser_Master_Send_Sync_ACK(void)  //����ȷ���ź�	,���ȴ�������SCL����������
{
	Senser_MSCL_L;
}

/**************************************************
*��������unsigned char Senser_Master_send_ACK(void) 
*���ܣ� ����ͬ���źţ���ʹ�����µ����� �����жϣ�����ָ��
*���룺  
*����� 
***************************************************/
unsigned char Senser_Master_send_ACK(void)  //�䨮?��?����?��|��eD?o?
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
*��������unsigned short ave_Bright(void)
*���ܣ�  ��ȡlight ֵ
*���룺  
*����� 
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
*������  void Measure_Brightness_Init(void)
*���ܣ�  �������ͨ��ģʽ����������
*���룺  
*����� 
***************************************************/
void Measure_Brightness_Init(void)
{		
		Senser_Master_Send_Data(0xA5);		
		Senser_Dly_1ms(25);			

		Senser_Master_Send_Data(0xC0);				
		Senser_Dly_1ms(25);	
}


/**************************************************
*��������unsigned short Receive_flicker(void)
*���ܣ�  ��ȡflicker ֵ
*���룺  
*����� 
***************************************************/

unsigned short Receive_flicker(void)
{
	  
    unsigned char eer1,eer2;
	
    Senser_Master_Send_Data(0xc3);
		Senser_Master_Receive_Sync_ACK(); //��ȡͬ���źţ�׼����������
		eer1=Senser_Master_Receive_Data();
		eer2=Senser_Master_Receive_Data();
		Senser_Dly_1ms(2);	
		GGGG=(eer1<<8)+eer2;			
    return 	GGGG;
}
/**************************************************
*��������void Niubee_Senser_initial(void)
*���ܣ�  ���ó�����ͨ��ģʽ
*���룺  �Զ�OTPǰ�����
*����� 
***************************************************/
void Niubee_Senser_initial(void)
{		
		Senser_Master_Send_Data(0xA4);		
		Senser_Dly_1ms(25);			

		Senser_Master_Send_Data(0xC0);				
		Senser_Dly_1ms(25);	
}
/**************************************************
*��������void AUT_Flicker_Senser_initial(void)
*���ܣ�  ���ó����ɼ��ģʽ
*���룺  �ֶ�OTPǰ�����
*����� 
***************************************************/

void AUT_Flicker_Senser_initial(void)
{
		Senser_Master_Send_Data(0xA6);		
		Senser_Dly_1ms(25);			
		Senser_Master_Send_Data(0xC0);				
		Senser_Dly_1ms(25);	
}

/******************* (C) COPYRIGHT 2015 STMicroelectronics *****END OF FILE****/
