/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    20140101
  * @brief   Main program body Jason
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "string.h"
#include <stdio.h>
#include  "lib.h"
#include "platform_config.h"
//#include "stdarg.h"
#include "gpio_key.h"
#include "lcd.h"
#include "IIC.h"
#include <T226.h>
#include "tsliic.h"
#include "OTP.h"
#include <V_SET.h>
#include <stdlib.h>

ErrorStatus HSEStartUpStatus;
/* Private typedef -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern  __IO uint32_t TimingDelay;
void DelayMs(__IO uint32_t nTime);
void RCC_Configuration (void);
void USART_GPIO_Configuration(void);
void USART_REMAP_GPIO_Configuration(void);
void NVIC_Configuration(void);
void SYSinitialize (void);
void  exit_hangler(void);
void ShowData(char *p,u16 dat);
void ShowData_ua2ma_4bitfloat(char *p,u16 dat);
void ShowData_uV2V_4bitfloat(char *p,u16 dat) ;
void ShowData_hex(char *p,u16 dat) ;   //ʮ���M���@ʾ

void PassKey(void);
void IDD_IOIDD_show(void);
void  BACK_LIGHT_OFF(void);	
void  BACK_LIGHT_ON(void);	
//#define ERR_NO_SPACE	0xff
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
////-------------******* -----*********** --------------------------------------- 
void Delay(u32 time);
extern void DelayKEY (u32 k); //��ס��������ͣ
extern void KEYGPIO_Init(void);///*[��PB1/PB10���ó�����ģʽ] 
extern u8 MIPI_READ_DATA[10];
extern u8 OTP_VALUE1 ;
extern u8 OTP_VALUE2;
extern u8 OTP_TIMES;

extern u8 ID1_VALUE ;
extern u8 ID2_VALUE ;
extern u8 ID3_VALUE ;

//------------------------------
extern u8 ID_OK;
extern u8 PIC_NUM;
u8 Flag_sleep = 0;
u8 pic_num=1;      //��ʾ���������
//u8 KeyFlagPause=255;  //����ɨ�������
extern u32 RED_bmp;
extern u32 GREEN_bmp;
extern u32 BLUE_bmp;
extern u32 WHITE_bmp;
extern u32 BLACK_bmp;
extern u32 GURY_45_bmp;    //45��
extern u32 GURY_60_bmp;    //60��
extern u32 GURY_128_bmp;    //128��

extern u8 Flicker_OTP;
extern u8 Flicker_OQC;
//------------------------------
extern u8 VCOM_register;
extern u8 ID_register;   

////===================Ԥ��ָ���******һЩ������Ҫ�����Ķ��Ĳ����ŵ������,�������Ԥ������֮���============== 
u16 HFZIDD,HFZIDDIO;
u16 HFZVDD,HFZVDDIO;
u8 CAL_OK=0;
u8 FLAG_RESET=0;
u8 MTP_PASS=0;
u8 show_promise=0;
BYTE pucData[2];

extern u16 IDDCurrentOfSleep;  ////���˯�ߵ�����
extern u16 IDDIOCurrentOfSleep;
extern u8  FlagOfCurrentSleep;  ////˯�ߵ����ɼ���־����0ʱ��ζ���Ѿ��ɼ���˯�ߵ���
u16 Vshunt_limit=0x9900;  //100mA�������ķ���늉�ֵ  0~0x7fff ->iovdd     0xffff~0x8000 ->vdd

u16 IDD_WORK_LIMIT_VALUE  =         20000;				/////��Ʒ����IDD�����޶�ֵ ��λ��10uA
u16	IOIDD_WORK_LIMIT_VALUE  =		    20000;				/////��Ʒ����IDDIO�����޶�ֵ
u16	IDD_SLEEP_LIMIT_VALUE  =			  10200;				/////��ƷSLEEP  IDD�����޶�ֵ ��λ��uA
u16	IOIDD_SLEEP_LIMIT_VALUE  =		  10500;				/////��ƷSLEEP  IDDIO�����޶�ֵ
#define	MIPI_YELLOW_BOARD 
void LCD_Vol_SET(void)
//+++++++++++++++++++++++++++++++++LCD��ѹ�趨++++++++++++++++++++++++++++++++++++++++ 	
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
	                                         | RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//��ֹjtag���Կճ�PB3,PB4,PA15
		
	///*[��GPIOB ��Ӧ�˿����ó����ģʽ] */  PB15 for VDDIO EN
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//GPIO����ٶ�50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	///*[��PA ��Ӧ�˿����ó����ģʽ] */     PA1 for VDD EN
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//GPIO����ٶ�50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	VDD_ADJ_SET(335);			 //VDD_ADJ_SET ����ȡֵ��Χ270~620����Ӧ��ѹ��Χ2.7V~6.2V; 
	VDD_ADJ_EN(1);				 //ʹ��VDD���		
	GPIO_SetBits(GPIOA, GPIO_Pin_1);  //������ʹ�ܣ��˴�����������ڵĿӣ��������޸�

	VDDIO_ADJ_SET(205);			  //VDDIO_ADJ_SET ����ȡֵ��Χ150~360,��Ӧ��ѹ��Χ1.5V~3.6V; �������ֵ��Ϊ������ѹ�������⣬����趨ֵ��Լ0.3V, 
	VDDIO_ADJ_EN(1);			  //ʹ��VDDIO���
  
	VOTP_ADJ_SET(390);			  //VOTP_ADJ_SET ����ȡֵ��Χ150~2080����Ӧ��ѹ��Χ1.5V~14.1V; 
	VOTP_EN(0);					  //ʹ��VOTP���	
}	
/*------------------------------------USER  START�����ķָ���-------------------------------------------*/
//================================================================================================================

int main(void)  
{ u16 cird [100]; 
	u16 cc;
	u16 mm_KEY_UP = 1 ;
	u16 mm_KEY_DOWN = 1 ;
	u16 mm_KEY_PAUSE = 1;
	u8 CurrentRetryflag=0;
	u8 ID_RIGHT;
//	u8 PicNumTemp=1;
//	u32  m=0;
//	u32 sdcap=0;  
	SYSinitialize(); //STM32 ϵͳʱ�ӳ�ʼ��	 
	DelayMs(10);
	//-----------------------------�ϻ�1�ָ���---------------------------

	KEYGPIO_Init();							 //----	---�԰��I���M�г�ʼ��	
	EEPROM_GPIO_Config();					 //----	---��оƬ�M������	
	EEpromRead_CurrentCalibration();		 //----��ȡEEprom�洢��ϵ��������У׼
	
	mm_KEY_UP=KEY_UP;   //ǰ�M
	mm_KEY_DOWN=KEY_DOWN;   //����
	mm_KEY_PAUSE=KEY_PAUSE;    //�M����Cģʽ
	GPIO_ResetBits(GPIOC, GPIO_Pin_1); 	 //---beep  off--------$#@#&^&&^%^%$###	
//-------------------------------�ϻ�2�ָ���---------------------------	
	T226_intion();  //ע�⣺��ѹ����ܵ�STM32��226ͬʱ����,��������INA226
	LCD_Vol_SET();		///////LCD 늉��O��,,,,//ע�⣺��ѹ����ܵ�STM32��226ͬʱ����
	
//----------VDD������z�y 	-----IOVDD������z�y-------------------------
//--����ʱ��������20000/100=200ma����Ϊ��Դ��·���жϵ�Դ��������һ��2�����У�ͬʱ������˸	
	while (Measure_I_5TimesForProtectIDD(20000)|	Measure_I_5TimesForProtectIDDIO(20000))	
    {	
			BEEP_ON();DelayMs(500);BEEP_BIBI();//	BEEP_ON();DelayMs(1000);BEEP_BIBI();
			BACK_LIGHT_OFF();DelayMs(300);BACK_LIGHT_ON();DelayMs(300);	BACK_LIGHT_OFF();DelayMs(300);BACK_LIGHT_ON();
      if( CurrentRetryflag==0)  //��ֹ������������������
			{LCD_Vol_SET();CurrentRetryflag++;}
      else 
			{
				 while  (1)
				 {BACK_LIGHT_OFF();DelayMs(30);BACK_LIGHT_ON();DelayMs(30);}
      }			
    }			
//--------------------------��Ϸָ���----------------------------------------------		
	  BACK_LIGHT_OFF(); //�κ��������˱�����ƽ�PC7�ĳ�ʼ��
		BACK_LIGHT_ON();  //��ִ�� BACK_LIGHT_OFF();��PC7�ȳ�ʼ���󣬱�����������
//***************************�ϻ�˵���˺���************************************
				 
		STM32_Init_SSD2828();					 //----	---��SSD2828&LCD ��ʼ��	
	  EXTI_Configuration();        
		
while (1)
	{
		Allcolor(0,WHITE_bmp);		     DelayMs(100);//ˢ��

		////////���´��a�Ȳ��yԇ�á�����
//	mm_KEY_UP = KEY_UP;
//	if(mm_KEY_UP == 0)
//	{
//		SSD2828_ENTER_LP_mode();
//		ID_set();  				   ///////��춮aƷ���ѽ����ID�ˣ��oID��䛙C���M����C���������r�R��ID��䛳�ʽ��C����������������
////////		MTP();
//		MTP_ID();
////////		reset_lcd();
//	}
//		Forward_scan();				         DelayMs(50);//���ߣ�ԓ�����ɸ������H��Ҫȥ��

//--------------------------------------------------------------------------------		
////�Д�ID�Ƿ�δ��	 	
//    	CHECK_ID_again();
//		mm_KEY_DOWN = KEY_AUTO_MODE;
//		ID_RIGHT=ID_CHECK();
//		if((mm_KEY_DOWN!=0)&&(ID_RIGHT==1))
//		{	
//		  OTP_ID_NG();
//			while(1)
//			{
//      	BEEP_Dudu();
//			}			
//		}
//		
//--------------------------------------------------------------------------------	
		
////Ո�ڴ�̎���x�������@ʾ�ĮaƷӍϢ���ձ���β�������m���ӕr    ps*-->( min 50 ms )			
//2015.4.11���ͺ�WDTҪ��ֻ��VCOM

		//ShowTxt("48,20,299, ");	  				             DelayMs(150);
		ShowTxt("60,20,0,052WA51_V1_FT8006M");	  				       DelayMs(150);	
    //ShowTxt("60,20,80,TK_test");	  				 DelayMs(150);		
		//ShowTxt("48,20,299, ");	  				             DelayMs(150);
		
//		ShowData_hex("30,450,1000", OTP_VALUE1);			   DelayMs(80);
//		ShowData_hex("30,450,1030", OTP_VALUE2);			   DelayMs(80);
//		ShowData_hex("30,450,1060", OTP_TIMES);			     DelayMs(80);
	
		switch(OTP_TIMES)
		{
			case 0x00: ShowTxt("60,20,1000,����0��");	   					DelayMs(150);   break;
		  case 0x01: ShowTxt("60,20,1000,����1��");	   					DelayMs(150);   break; 
		  case 0x03: ShowTxt("60,20,1000,����2��");	   					DelayMs(150);   break; 
			case 0x07: ShowTxt("60,20,1000,����3��");	            DelayMs(150);   break;		
		}
		DelayKEY(250);
		
/************************2014.10.30**************************************/
//		mm_KEY_DOWN = KEY_AUTO_MODE;
//		if(mm_KEY_DOWN==0)
//		{	
//    enter_sleep_mode();
//		DelayMs(1500);
//		DelayKEY(2);//�������쳣ʱʹ��
//		
//		SLEEP_CAL_TEST(); 
//		
//    exit_sleep_mode();
//    DelayMs(5);
//		}
/**************************************************************/

//////////===================================���Ԅ��x��==========================================	
////   Backward_scan();	 	    DelayMs(50);   //���ߣ�ԓ�����ɸ������H��Ҫȥ��
///* 
///*�ք�ģʽ*/ 
   mm_KEY_UP = KEY_UP;
	 if(mm_KEY_UP == 0)
	 {
		  showbmp (Flicker_OTP);				DelayMs(250);
		  ShowTxt("40,0,0,�ֶ�ģʽ");	
		  while(mm_KEY_UP == 0)
			{
			 mm_KEY_UP = KEY_UP; 
			}
            MTP_PASS=1;
            KEY_adjust();
	 }
/*_____________________________________________________________________________________*/	
	 
/*�Ԅ�ģʽ*/	 
	 mm_KEY_DOWN = KEY_AUTO_MODE;
	 if(mm_KEY_DOWN == 0)
	 {	
			showbmp (Flicker_OTP);				DelayMs(350);
			Auto_otp();	
		  
		  switch(OTP_TIMES)
		  {
			case 0x00: ShowTxt("60,20,1000,����0��");	   					DelayMs(150);   break;
		  case 0x01: ShowTxt("60,20,1000,����1��");	   					DelayMs(150);   break; 
		  case 0x03: ShowTxt("60,20,1000,����2��");	   					DelayMs(150);   break; 
			case 0x07: ShowTxt("60,20,1000,����3��");	            DelayMs(150);   break;
		  }	 	
			while(1);
//			{
//   		   BACK_LIGHT_OFF();DelayMs(1000);BACK_LIGHT_ON();DelayMs(1000);
//			} 	 
	 }
//////================================SELECT OFF=========================================== 	
	 
	 
//////�Д�VCOM�Ƿ�δ��	 
//////----------------------------------------------------------------------	 
	 else

	 {
//		VCOM_TIMES_Check(0xE8);
//	 	if(OTP_TIMES == 0x00)     //���һ��δ������ʾOTPδ��   
//		{	                         //��IC�o���x�Δ����ܣ��Ɍ�̓���ȃ�������
//			OTP_NO();	
//		}		 
	 }	
//*/
//----------------------------------------------------------------------
	DelayMs(200);   
//	  Backward_scan();	 	    DelayMs(250);   //���ߣ�ԓ�����ɸ������H��Ҫȥ��
//    MTP_Recogn();
/*ѭ�h늜y����*/
//��lcd.c�ļ��ж��x���_��늜y���濂��	 �����⮋��--��flick_otp\flicker_QCՈ���_���x�侎̖
	LCDTEST:
	 
	switch(pic_num)
	 {
		case 1:       //�ڶ���FLICKER������ϲ�����¼
		{
				showbmp (1);	                  DelayMs(120); 
//				MTP_PASS=0;
//				KEY_adjust();			
//			pic_num=2;	
            break;
		}
		case 2:	 showbmp (2);               DelayMs(500);   break;	/////white
		case 3:	 showbmp (3);             NORML_CAL_TEST();  DelayMs(500);   break; 	//black
		case 4:	 showbmp (4);               DelayMs(120);   break; 	//baizong
		case 5:  showbmp (5);            /*   NORML_CAL_TEST(); */       DelayMs(120);   break;
		case 6:  showbmp (6);               DelayMs(120);   break;
		case 7:  showbmp (7);               DelayMs(120);   break;  
		case 8:  showbmp (8);               DelayMs(120);   break;	
		case 9:  showbmp (9);               DelayMs(500);   break;	
		case 10: showbmp (10);              DelayMs(300);   break;	
		case 11: showbmp (11);              DelayMs(300);   break;	
		case 12: showbmp (12);              DelayMs(120);   break;	
		case 13: showbmp (13);              DelayMs(120);   break;	
		case 14: showbmp (14);              DelayMs(120);   break;	
		case 15: showbmp (15);              DelayMs(120);   break;	
		case 16: showbmp (16);              DelayMs(300);   break;	
		case 17: showbmp (17);              DelayMs(120);   break;	
		case 18: showbmp (18);              DelayMs(300);   break;	
		case 19:        //�ڶ���FLICKER������ϲ���¼
		{
//				showbmp (19);              DelayMs(120);   
//				MTP_PASS=1;
//				KEY_adjust();				
			pic_num++;	
            break;	
		}
		default:
			showbmp (pic_num);              DelayMs(300);   break;
	 } 
	 PassKey();
			 
	 goto LCDTEST;

	 }
}  
  
 /*******************************************************************************/
 /*------------------------------------USER  END-------------------------------------------*/
void IDD_IOIDD_show(void)
{
	{	
//    Allcolor(1,WHITE_bmp);		     DelayMs(50);//ˢ��	
    
		HFZVDD=M9_BUS_VOLTAGE();	  /////mV
		ShowTxt("38,20,99,VDD:(V)");	  				             DelayMs(150);
		ShowData_uV2V_4bitfloat("38,200,99",HFZVDD);         DelayMs(150);		    
			
	  HFZIDD=Measure_IDD(mA);		
		ShowTxt("38,20,149,IDD:(mA)");	  				             DelayMs(150);
		ShowData_ua2ma_4bitfloat("38,200,149",HFZIDD);         DelayMs(150);
		
	  HFZVDDIO=M12_BUS_VOLTAGE();	  /////mV
		ShowTxt("38,20,199,VDDIO:(V)");	  				           DelayMs(150);
		ShowData_uV2V_4bitfloat("38,250,199",HFZVDDIO);     DelayMs(150);
		
    HFZIDDIO=Measure_IDDIO(mA);  
		ShowTxt("38,20,249,IDDIO:(mA)");	  				           DelayMs(150);
		ShowData_ua2ma_4bitfloat("38,250,249",HFZIDDIO);   DelayMs(150);
		
		if (FlagOfCurrentSleep)
				{ 
					ShowTxt("38,20,299,IDD��������:(uA)"  );	               DelayMs(150);
					ShowData("38,260,349",IDDCurrentOfSleep);        DelayMs(150);
					ShowTxt("38,20,399,IDDIO��������:(uA)  ");	   					 DelayMs(250);
					//��һ��Ƚ���֣����ShowTxt("38,20,399,IOVDD��������");,���޷���ʾ��Ī�����
					ShowData("38,260,449",IDDIOCurrentOfSleep);      DelayMs(150);	        
				}
    show_promise=0;				
	}
}


void PassKey(void)
{
	u16 mm1,mm2,mm3;

	mm1=mm2=mm3=1;    //�M����Cģʽ

			
	 mm1=KEY_UP;   //ǰ�M
	 mm2=KEY_DOWN;   //����
	 mm3=KEY_PAUSE;    //�M����Cģʽ
	 DelayMs(1);
	
	 while((mm1!=0)&&(mm2!=0)&&(mm3!=0))
	 {
		mm1=KEY_DOWN;	
		mm2=KEY_UP;
		mm3=KEY_PAUSE;	
			if(show_promise==1)
			{
				IDD_IOIDD_show();		
			}			
	 }
	 if(mm1==0)
	 {			 
		pic_num++;	 
		if(pic_num>PIC_NUM)
		{
			pic_num=1;
		}
	}
	else if(mm2==0)
	{	
		pic_num--;	   
		if(pic_num==0)
		{
			pic_num=PIC_NUM;
		}
	}
	else if(mm3==0)
	{
//		enter_sleep_mode();
//		DelayMs(1500);
//		
//    SLEEP_CAL_TEST();    //������������
//		
//		mm3=1;				
//		while(mm3!=0)
//		{
//			mm3=KEY_PAUSE;
//		}		
//		exit_sleep_mode();
//		DelayMs(50); 
		;  //��������
	}			 
	while(mm1==0||mm2==0||mm3==0)   //�ȴ������ͷ�
	{
		mm1=KEY_DOWN;
		mm2=KEY_UP;
		mm3=KEY_PAUSE;
	}	
	
	DelayMs(50);		 
} 

void BACK_LIGHT_ON(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_7);  // 
}

void BACK_LIGHT_OFF(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
	                                         | RCC_APB2Periph_AFIO, ENABLE); 
	//#define GPIO_Remap_SWJ_JTAGDisable ((u32)0x00300200)  /* JTAG-DP Disabled and SW-DP Enabled */ ��ֹJTAG,�Կճ�PB3,PB4
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//��ֹjtag���Կճ�PB3,PB4,PA15
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//GPIO����ٶ�50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);  // 
}	 
//-------------------------------------------------------------------------------------------------

void Delay(u32 time)
{
//	u32 x,y;
//	for (y=0;y<time;y++)
//	{
//	for (x=0;x<4096;x++)
//		{  ;
//		}
//	}
	Dly_10ns(4000*time);
}

 
//---------------------------------------�������������޸�------------------------------------------
//=================================================================================================
void DelayMs(__IO uint32_t nTime)
{ //__IO==volatile //uint32_t ==�޷���������
  TimingDelay = nTime;
  while(TimingDelay != 0); 
}
void SYSinitialize (void)
{  
	 RCC_Configuration();
	 NVIC_Configuration();
   USART_REMAP_GPIO_Configuration(); 
   SysTick_Config(72000);   //��Ϊδʹ���ⲿʱ�ӣ����Դ˴β���  ���� ; ==/SysTick_Config(SystemCoreClock/1000); == SysTick_Config(72000);
   }

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval : None
  */
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USART Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
//RCC_Configuration
void RCC_Configuration(void)
{    	
	/* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
 	
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }
  
  /* Enable AFIO clocks */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE);	
	

//#if defined(USE_USART3)  
#if defined(USE_USART2)
	   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOx , ENABLE);
	   RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);
		 
#else	 
	   RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOx , ENABLE);
#endif
	
	  RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO , ENABLE);
	  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE);  //=============2014.1.6
}


//#ifdef __GNUC__
//  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
//     set to 'Yes') calls __io_putchar() */
//  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif /* __GNUC__ */
  
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{ //u8 i1; i1=0;
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USARTx, (uint8_t) ch);
	//while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET){}  //ĳ�������USART_FLAG_TC�������̱����㣬���Ի���һ�����
	
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}
  //{i1++;}

  return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 
void EXTI0_IRQHandler(void)
{
//	u16 i,j;
	
  while( EXTI_GetITStatus(EXTI_Line0)==RESET);	
  EXTI_ClearITPendingBit(EXTI_Line0);
//	GPIOC->BRR=0x0080;
//	GPIOA->BRR=0x0010;
//  GPIOA->BRR=0x0002;
//  GPIOB->BRR=0x8000;

  show_promise=1;
	EXTI_ClearFlag(EXTI_Line0);
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
