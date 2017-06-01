/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    20140101
  * @brief   Main program body Jason
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "string.h"
#include <stdio.h>
#include  "lib.h"
#include "platform_config.h"
#include "FAT.h"
#include "sdcard.h"
#include "gpio_key.h"
#include "keyscan_adc.h"
#include "lcd.h"
#include "IIC.h"
#include <T226.h>
#include "tsliic.h"
#include <V_SET.h>
#include <stdlib.h>
#include <hfz_secret.h>
#include <LVDS_I2C.h>
#include <LVDS_OTP.h>
#include "Mtransmit.h"

GPIO_InitTypeDef GPIO_InitStructure;
ErrorStatus HSEStartUpStatus;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static vu32 TimingDelay = 0;
extern BYTE FAT32_Enable; 
extern WORD SectorsPerClust;
extern u16 image_count ;
extern u16 image_all ;
SD_Error Status = SD_OK;
SD_CardInfo SDCardInfo;
u32  sd_Capacity;
SD_Error SD_InitAndConfig(void);
/* Private function prototypes -----------------------------------------------*/
extern  __IO uint32_t TimingDelay;
void DelayMs(__IO uint32_t nTime);
void RCC_Configuration (void);
void USART_GPIO_Configuration(void);
void NVIC_Configuration(void);
void SYSinitialize (void);
void  exit_hangler(void);
void ShowData(char *p,u16 dat);
void ShowData_hex(char *p,u16 dat);
void PassKey(void);
void IDD_IOIDD_show(void);
//#define ERR_NO_SPACE	0xff
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

////-------------******* -----*********** ---------------
////
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
u8 pic_num=1;

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

////===============================================================================
//u32 TESET=600; 


u16 HFZIDD,HFZIDDIO;
u16 HFZVDD,HFZVDDIO;
u8 CAL_OK=0;
u8 FLAG_RESET=0;

BYTE pucData[2];


extern u16 IDDCurrentOfSleep;  ////���˯�ߵ�����
extern u16 IDDIOCurrentOfSleep;
extern u8  FlagOfCurrentSleep;  ////˯�ߵ����ɼ���־����0ʱ��ζ���Ѿ��ɼ���˯�ߵ���
u16 Vshunt_limit=0x9900;  //100mA�������ķ���늉�ֵ  0~0x7fff ->iovdd     0xffff~0x8000 ->vdd

u16 IDD_WORK_LIMIT_VALUE  =         75000;				/////��Ʒ����IDD�����޶�ֵ ��λ��10uA
u16	IOIDD_WORK_LIMIT_VALUE  =		    75000;				/////��Ʒ����IDDIO�����޶�ֵ
u16	IDD_SLEEP_LIMIT_VALUE  =			  800;				/////��ƷSLEEP  IDD�����޶�ֵ ��λ��uA
u16	IOIDD_SLEEP_LIMIT_VALUE  =		  800;				/////��ƷSLEEP  IDDIO�����޶�ֵ

//u16 Vshunt_limit=0x9900;  //100mA�������ķ���늉�ֵ  0~0x7fff ->iovdd     0xffff~0x8000 ->vdd

//#define IDD_WORK_LIMIT_VALUE			75000				/////��Ʒ����IDD�����޶�ֵ
//#define	IOIDD_WORK_LIMIT_VALUE		75000				/////��Ʒ����IDDIO�����޶�ֵ
//#define	IDD_SLEEP_LIMIT_VALUE			1200				/////��ƷSLEEP  IDD�����޶�ֵ
//#define	IOIDD_SLEEP_LIMIT_VALUE		1200				/////��ƷSLEEP  IDDIO�����޶�ֵ

u8  HorA=1;  
u8 show_promise=0;
u16 VOTP_SET = 0;		   //////OTP��ѹ���ã������ã�������Ϊ0�������ʽ���ڼ��ѭ���С�����

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
	
/*------------------------------------USER  START-------------------------------------------*/	
	
int main(void)  
{ 
	u16 mm_KEY_UP = 1 ;
	u16 mm_KEY_DOWN = 1 ;
	u16 mm_KEY_PAUSE = 1;
	
	u32  m=0;
	u32 sdcap=0;
  
	SYSinitialize(); //STM32 ϵͳʱ�ӳ�ʼ��	
 //---------------------------------------
	Delay(10);
	
	KEYGPIO_Init();
	EEPROM_GPIO_Config();	
//////////----------------------------------------------------------------	
	mm_KEY_UP=KEY_UP;   //ǰ�M
	mm_KEY_DOWN=KEY_DOWN;   //����
	mm_KEY_PAUSE=KEY_PAUSE;    //�M����Cģʽ
//	GPIO_ResetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  off
	
	
	if((mm_KEY_UP==0)&&(mm_KEY_DOWN==0)&&(mm_KEY_PAUSE==0))
	{
	   while((mm_KEY_UP==0)||(mm_KEY_DOWN==0)||(mm_KEY_PAUSE==0))
		 {
			mm_KEY_UP=KEY_UP;   //ǰ�M
			mm_KEY_DOWN=KEY_DOWN;   //����
			mm_KEY_PAUSE=KEY_PAUSE;    //�M����Cģʽ		 
		 }			 
     
//		 STM_MINI_SET();
		 AUTO_CALIBRATION();
		
		 mm_KEY_UP = 1 ;
		 mm_KEY_DOWN = 1 ;
		 mm_KEY_PAUSE = 1;
	}
//---------------------------------ok---20150731  Alan--------------------	

   CAL_OK=EEPROMBYTE_Read(0x18);  //���У׼������ʹ��У׼���������δУ׼����ʹ��Ĭ�ϲ���
//   CAL_OK=EEPROMBYTE_Read(0x19);
   if(CAL_OK==0xcc)
   { GET_PRESENT(); } //��ȡУ׼����
	 else
	 {
				
//				BEEP_ON();
//				while(1);

	 }
//--------------------------��Ϸָ���----------------------------------------------		
	BACK_LIGHT_OFF(); //�κ��������˱�����ƽ�PC7�ĳ�ʼ��
	BACK_LIGHT_ON();  //��ִ�� BACK_LIGHT_OFF();��PC7�ȳ�ʼ���󣬱�����������
	BACK_LIGHT_ON();
//***************************�ϻ�˵���˺���************************************		
 
	STM32_Init_SSD2828();					 //----	---��SSD2828&LCD ��ʼ��	
	KEYGPIO_Init();
	DelayMs(50);


   
    
while (1)
	{
//////-------------------

		Allcolor(0,WHITE_bmp);		     DelayMs(150);//ˢ��
//		while(1);

	  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//		while(Measure_IDD(mA) > IDD_WORK_LIMIT_VALUE)
//		{
//			Allcolor(1,WHITE_bmp);		     DelayMs(150);//ˢ��
//			IDD_IOIDD_show();
//			BEEP_Dudu();
//		}


//---------------------------------------------------------------------------		
		
////Ո�ڴ�̎���x�������@ʾ�ĮaƷӍϢ���ձ���β�������m���ӕr    ps*-->( min 50 ms )			
		
//		ShowTxt("48,20,299, ");	  				             DelayMs(150);
//		ShowTxt("48,20,299,107DEMO_V1");	  				   DelayMs(150);
//		ShowTxt("48,20,299, ");	  				             DelayMs(150);
//		DelayMs(550);

////////===================================���Ԅ��x��==========================================	
	 
/*�ք�ģʽ*/ 
//   mm_KEY_UP = KEY_UP;
//	 if(mm_KEY_UP == 0)
//	 {
//		  showbmp (Flicker_OTP);				DelayMs(250);
//		  ShowTxt("40,0,0,�ֶ�ģʽ");	
//		  while(mm_KEY_UP == 0)
//			{
//			 mm_KEY_UP = KEY_UP; 
//			}
//        LVDS_KEY_adjust(); //�ֶ�����
//	
//	 }
/*_____________________________________________________________________________________*/	
	 
/*�Ԅ�ģʽ*/	 
	 mm_KEY_DOWN = KEY_AUTO_MODE;
	 if(mm_KEY_DOWN == 0)
	 {	
//			showbmp (Flicker_OTP);				DelayMs(350);	   
//			Auto_otp();	 //�Զ�����
//            GAMMA_OTP_CHECK();
//        LVDS_WRITE_SPI(0x00,0x00);  ////page 0
//        LVDS_WRITE_SPI(0x1F,0x80); 
	 }
//////================================SELECT OFF=========================================== 	
//	while(1){
//     showbmp (Flicker_OTP);				DelayMs(350);
//    SHOW_READ_VALUE(); 
//    }
//    GAMMA_OTP_CHECK();
/*ѭ�h늜y����*/
//��lcd.c�ļ��ж��x���_��늜y���濂��	 �����⮋��--��flick_otp\flicker_QCՈ���_���x�侎̖
	LCDTEST:
	 
	 switch(pic_num)
	 {
	 	case 1:  showbmp (1);	   	 DelayMs(250);   break;	
		case 2:	 showbmp (2);	     DelayMs(250);   break;	///// 
		case 3:	 showbmp (3);	     DelayMs(250);   break; 	//
		case 4:	 showbmp (4);	     DelayMs(250);   break; 	
		case 5:  showbmp (5);	     DelayMs(250);   break;
		case 6:  showbmp (6);	     DelayMs(250);   break;
		case 7:  showbmp (7);	     DelayMs(250);   break;
		case 8:  showbmp (8);	   	 DelayMs(250);   break;
		case 9:  showbmp (9);	   	 DelayMs(250);   break;
    case 10:  showbmp (10);	   	 DelayMs(250);   break;
         
		 default:
             showbmp (pic_num);	   	 DelayMs(250);   break;

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
		ShowTxt("38,20,99,VDD:");	  				             DelayMs(150);
		ShowData("38,200,99",HFZVDD);    DelayMs(150);
			
	  HFZIDD=Measure_IDD(mA);		
		ShowTxt("38,20,149,IDD:");	  				             DelayMs(150);
		ShowData("38,200,149",HFZIDD);    DelayMs(150);
		
	  HFZVDDIO=M12_BUS_VOLTAGE();	  /////mV
		ShowTxt("38,20,199,VDDIO:");	  				           DelayMs(150);
		ShowData("38,200,199",HFZVDDIO);  DelayMs(150);
		
    HFZIDDIO=Measure_IDDIO(mA);
		ShowTxt("38,20,249,IDDIO:");	  				           DelayMs(150);
    ShowData("38,200,249",HFZIDDIO);	DelayMs(150);
    		
	}
}

void PassKey(void)
{
	u32 coun=0;
	u16 mm1,mm2,mm3;
  
	mm1=mm2=mm3=1;    //�M����Cģʽ

			
	 mm1=KEY_UP;   //ǰ�M
	 mm2=KEY_DOWN;   //����
	 mm3=KEY_PAUSE;    //�M����Cģʽ
	 DelayMs(1);
/*******�ֶ��ܻ���********/	 
	coco:	
   if(HorA==0)	
   {	
		 while((mm1!=0)&&(mm2!=0)&&(mm3!=0))
		 {
			mm1=KEY_DOWN;	
			mm2=KEY_UP;
			mm3=KEY_PAUSE;	
			DelayMs(50);  
				if(show_promise==1)
				{
					IDD_IOIDD_show();		
				}
        if((mm1==0)&&(mm2==0)&&(mm3!=0))      //���+/-����ͬʱ���� �������ֶ��ܻ���ģʽ
				{
					HorA=~HorA;
					BEEP_BIBI();
					while(mm1==0||mm2==0)   //�ȴ������ͷ�
					{
					mm1=KEY_DOWN;
					mm2=KEY_UP;
					}
				  goto coco;
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
//				Backward_scan();	 	    DelayMs(50);
			}
		}
		else if(mm3==0)
		{
        ;
		}	

		while(mm1==0||mm2==0||mm3==0)   //�ȴ������ͷ�
		{
			mm1=KEY_DOWN;
			mm2=KEY_UP;
			mm3=KEY_PAUSE;
		}	
   }
/********�Զ��ܻ���********/	 
	 else
	 {
		 do
		 {
	    mm1=KEY_UP;   //ǰ�M
      mm2=KEY_DOWN;   //����
	    mm3=KEY_PAUSE;    //�M����Cģʽ			 
			if(mm3==0)      //���pause�����£�������ͣ���ٰ�һ��������ܶ�
			{	   
					while((mm3==0)&&(mm2!=0)&&(mm1!=0))   //�ȴ������ͷ�
					{mm3=KEY_PAUSE;}
	//				DelayMs(500);	
					mm3=1;					
					while(mm3!=0)   //�ȴ��������ٴΰ��£�����������ܶ�
					{mm3=KEY_PAUSE;}
				
					while(mm3==0)   //�ȴ������ͷ�
					{mm3=KEY_PAUSE;}				
			}
			if((mm1==0)&&(mm2==0)&&(mm3!=0))      //���+/-����ͬʱ���� �������Զ��ܻ���ģʽ
	    {
					HorA=~HorA;
					BEEP_BIBI();
					while(mm1==0||mm2==0)   //�ȴ������ͷ�
					{
					mm1=KEY_DOWN;
					mm2=KEY_UP;
		      }
					goto coco;
	    }
			coun++; 
			DelayMs(1);
			if(show_promise==1)
			{
				 IDD_IOIDD_show();		
			}	
		 }while(coun<1600);
		 
//-----------------------------------		 
//	   if(pic_num==17)
//		 {
//			DelayMs(200);
//		 }
//     else if(pic_num==1)
//		 {
//      DelayMs(200);	
//		 }
//		 else
//		 {
//			DelayMs(200);
//		 }
		  
//-----------------------------------			
			
		 pic_num++;
		 if(pic_num>PIC_NUM)
		 {
			 pic_num=1;
//			 Sleep_Test(1500); 
		 }	 
	 }
	 
	 DelayMs(50);	
 	
}
/*******************************************************************************
* Function Name  : void ShowData_hex(char *p,u16 dat)   
* Description    : ʮ���M���@ʾ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  

void ShowData_hex(char *p,u16 dat)    //ʮ���M���@ʾ
	 { 
	  char str1[]="123,456,789";
		char str2[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		u8 j=0,tm1,tm2;
		u16 tep=dat;	
		strcpy(str1,p); 
			
		 do { str2[j]=str1[j];}
		 while (str1[++j] !=0) ;

       str2[j++] =44;   //,
		   str2[j++] =79;   
		   str2[j++] =120;
		   if((dat/16)<0x0a)				 
			 {str2[j++]=dat/16+0x30;}
			 else
			 {str2[j++]=dat/16+0x37;}
		   if((dat%16)<0x0a)	       			 
       {str2[j++]=dat%16+0x30;}
	     else
       {str2[j++]=dat%16+0x37;}
       str2[j]='\0';
		   
		 ShowTxt(str2);
	 } 
    
  
/*******************************************************************************
* Function Name  : void ShowData(char *p,u16 dat)      
* Description    : ʮ�M���@ʾ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/   
  
void ShowData(char *p,u16 dat)     //ʮ�M���@ʾ
	 { u8 i,j,k,m;
		 u16 tep;
	  char str1[]="123,456,789";
		char str2[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		char strt [8]; 
		strcpy(str1,p); 
		//strcpy(str2,"")

		tep= dat;
    if (tep==0)	i=1;
			else i=0;
		 while(tep)
		 { tep /=10;
			 i++;		 
		 }
		 j=0;
		 do { str2[j]=str1[j];j++;}
		 while (str1[j] !=0) ;
		 // {}
		 m=i+j; 
		 tep=dat;
		 for (k=0;k<i;k++)
		 {
			 str2[m]=tep%10+48;
			 m--;
			 tep /=10;
		 }
		 str2[m] =44;
		// j++;
		 ShowTxt(str2);
	 }
	//----------------------------- 
	
//-------------------------------------------------------------------------------------------------
void Delay(u32 time)
{
//	DelayMs(time);	
	Dly_10ns(4000*time);
}
 
//---------------------------------------�������������޸�------------------------------------------
//=================================================================================================
void DelayMs(__IO uint32_t nTime)
{ //__IO==volatile //uint32_t ==�޷���������
	u32 temp;
  SysTick->CTRL&=0xfffffffb;//bit2??,??????  HCLK/8	
	SysTick->LOAD=(u32)nTime*(72/8)*1000;//????(SysTick->LOAD?24bit)
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
void SYSinitialize (void)
{  
    RCC_Configuration();
    USART_GPIO_Configuration();
    SysTick_Config(72000);   //��Ϊδʹ���ⲿʱ�ӣ����Դ˴β���  ���� ; ==/SysTick_Config(SystemCoreClock/1000); == SysTick_Config(72000);
    NVIC_Configuration( );
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

/* Private functions ---------------------------------------------------------*/
SD_Error SD_InitAndConfig(void)
{
  Status = SD_Init();

  if (Status == SD_OK)
  {
    /*----------------- Read CSD/CID MSD registers ------------------*/
    Status = SD_GetCardInfo(&SDCardInfo);
  }
  else return 0;
  if (Status == SD_OK)
  {
    /*----------------- Select Card --------------------------------*/
    Status = SD_SelectDeselect((u32) (SDCardInfo.RCA << 16));
  }
  else return 0;
  if (Status == SD_OK)
  {
    /*----------------- Set BusWidth ------------------------------*/
    Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
  }
  else return 0;
  /* Set Device Transfer Mode to INTERRUPT to DMA */
  if (Status == SD_OK)
  {  
    Status = SD_SetDeviceMode(SD_DMA_MODE);//SD_DMA_MODE,SD_INTERRUPT_MODE
  }
	else return 0;
  return Status;
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
	u16 i,j;
	
	FLAG_RESET++;
	if(FLAG_RESET == 1)
	{
		while( EXTI_GetITStatus(EXTI_Line0)==RESET);
//		EXTI_ClearITPendingBit(EXTI_Line0);
		GPIOC->BRR=0x0080;
		GPIOA->BRR=0x0010;
		GPIOA->BRR=0x0002;
		GPIOB->BRR=0x8000;
		
		EXTI_ClearITPendingBit(EXTI_Line0);
//		EXTI_ClearFlag(EXTI_Line0);
	}
	else if(FLAG_RESET>1)
	{
//		FLAG_RESET = 0;
		__set_FAULTMASK(1);      // �ر������ж�
		NVIC_SystemReset();		 // ��λ
		
	}
	else
	{;}
//	EXTI_ClearITPendingBit(EXTI_Line0);
//	EXTI_ClearFlag(EXTI_Line0);
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
