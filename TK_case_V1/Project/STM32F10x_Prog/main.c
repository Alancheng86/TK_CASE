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
extern void DelayKEY (u32 k); //按住，画面暂停
extern void KEYGPIO_Init(void);///*[把PB1/PB10配置成输入模式] 
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
extern u32 GURY_45_bmp;    //45灰
extern u32 GURY_60_bmp;    //60灰
extern u32 GURY_128_bmp;    //128灰

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


extern u16 IDDCurrentOfSleep;  ////存放睡眠电流用
extern u16 IDDIOCurrentOfSleep;
extern u8  FlagOfCurrentSleep;  ////睡眠电流采集标志，非0时意味着已经采集到睡眠电流
u16 Vshunt_limit=0x9900;  //100mA所對應的分流電壓值  0~0x7fff ->iovdd     0xffff~0x8000 ->vdd

u16 IDD_WORK_LIMIT_VALUE  =         75000;				/////产品工作IDD电流限定值 单位：10uA
u16	IOIDD_WORK_LIMIT_VALUE  =		    75000;				/////产品工作IDDIO电流限定值
u16	IDD_SLEEP_LIMIT_VALUE  =			  800;				/////产品SLEEP  IDD电流限定值 单位：uA
u16	IOIDD_SLEEP_LIMIT_VALUE  =		  800;				/////产品SLEEP  IDDIO电流限定值

//u16 Vshunt_limit=0x9900;  //100mA所對應的分流電壓值  0~0x7fff ->iovdd     0xffff~0x8000 ->vdd

//#define IDD_WORK_LIMIT_VALUE			75000				/////产品工作IDD电流限定值
//#define	IOIDD_WORK_LIMIT_VALUE		75000				/////产品工作IDDIO电流限定值
//#define	IDD_SLEEP_LIMIT_VALUE			1200				/////产品SLEEP  IDD电流限定值
//#define	IOIDD_SLEEP_LIMIT_VALUE		1200				/////产品SLEEP  IDDIO电流限定值

u8  HorA=1;  
u8 show_promise=0;
u16 VOTP_SET = 0;		   //////OTP电压设置，若不用，则设置为0，否则程式死于检测循环中。。。

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
  
	SYSinitialize(); //STM32 系统时钟初始化	
 //---------------------------------------
	Delay(10);
	
	KEYGPIO_Init();
	EEPROM_GPIO_Config();	
//////////----------------------------------------------------------------	
	mm_KEY_UP=KEY_UP;   //前進
	mm_KEY_DOWN=KEY_DOWN;   //後退
	mm_KEY_PAUSE=KEY_PAUSE;    //進入待機模式
//	GPIO_ResetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  off
	
	
	if((mm_KEY_UP==0)&&(mm_KEY_DOWN==0)&&(mm_KEY_PAUSE==0))
	{
	   while((mm_KEY_UP==0)||(mm_KEY_DOWN==0)||(mm_KEY_PAUSE==0))
		 {
			mm_KEY_UP=KEY_UP;   //前進
			mm_KEY_DOWN=KEY_DOWN;   //後退
			mm_KEY_PAUSE=KEY_PAUSE;    //進入待機模式		 
		 }			 
     
//		 STM_MINI_SET();
		 AUTO_CALIBRATION();
		
		 mm_KEY_UP = 1 ;
		 mm_KEY_DOWN = 1 ;
		 mm_KEY_PAUSE = 1;
	}
//---------------------------------ok---20150731  Alan--------------------	

   CAL_OK=EEPROMBYTE_Read(0x18);  //如果校准过，则使用校准参数，如果未校准，则使用默认参数
//   CAL_OK=EEPROMBYTE_Read(0x19);
   if(CAL_OK==0xcc)
   { GET_PRESENT(); } //获取校准参数
	 else
	 {
				
//				BEEP_ON();
//				while(1);

	 }
//--------------------------配合分割线----------------------------------------------		
	BACK_LIGHT_OFF(); //次函数包含了背光控制脚PC7的初始化
	BACK_LIGHT_ON();  //先执行 BACK_LIGHT_OFF();或PC7先初始化后，本函数才正常
	BACK_LIGHT_ON();
//***************************废话说完了好像************************************		
 
	STM32_Init_SSD2828();					 //----	---对SSD2828&LCD 初始化	
	KEYGPIO_Init();
	DelayMs(50);


   
    
while (1)
	{
//////-------------------

		Allcolor(0,WHITE_bmp);		     DelayMs(150);//刷白
//		while(1);

	  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//		while(Measure_IDD(mA) > IDD_WORK_LIMIT_VALUE)
//		{
//			Allcolor(1,WHITE_bmp);		     DelayMs(150);//刷白
//			IDD_IOIDD_show();
//			BEEP_Dudu();
//		}


//---------------------------------------------------------------------------		
		
////請在此處定義您所需顯示的產品訊息！務必在尾部加上適當延時    ps*-->( min 50 ms )			
		
//		ShowTxt("48,20,299, ");	  				             DelayMs(150);
//		ShowTxt("48,20,299,107DEMO_V1");	  				   DelayMs(150);
//		ShowTxt("48,20,299, ");	  				             DelayMs(150);
//		DelayMs(550);

////////===================================手自動選擇==========================================	
	 
/*手動模式*/ 
//   mm_KEY_UP = KEY_UP;
//	 if(mm_KEY_UP == 0)
//	 {
//		  showbmp (Flicker_OTP);				DelayMs(250);
//		  ShowTxt("40,0,0,手动模式");	
//		  while(mm_KEY_UP == 0)
//			{
//			 mm_KEY_UP = KEY_UP; 
//			}
//        LVDS_KEY_adjust(); //手动调整
//	
//	 }
/*_____________________________________________________________________________________*/	
	 
/*自動模式*/	 
	 mm_KEY_DOWN = KEY_AUTO_MODE;
	 if(mm_KEY_DOWN == 0)
	 {	
//			showbmp (Flicker_OTP);				DelayMs(350);	   
//			Auto_otp();	 //自动调整
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
/*循環電測畫面*/
//在lcd.c文件中定義正確的電測畫面總數	 ，特殊畫面--》flick_otp\flicker_QC請正確定義其編號
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
//    Allcolor(1,WHITE_bmp);		     DelayMs(50);//刷白	
    
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
  
	mm1=mm2=mm3=1;    //進入待機模式

			
	 mm1=KEY_UP;   //前進
	 mm2=KEY_DOWN;   //後退
	 mm3=KEY_PAUSE;    //進入待機模式
	 DelayMs(1);
/*******手动跑画面********/	 
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
        if((mm1==0)&&(mm2==0)&&(mm3!=0))      //如果+/-键按同时按下 ，进入手动跑画面模式
				{
					HorA=~HorA;
					BEEP_BIBI();
					while(mm1==0||mm2==0)   //等待按键释放
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

		while(mm1==0||mm2==0||mm3==0)   //等待按键释放
		{
			mm1=KEY_DOWN;
			mm2=KEY_UP;
			mm3=KEY_PAUSE;
		}	
   }
/********自动跑画面********/	 
	 else
	 {
		 do
		 {
	    mm1=KEY_UP;   //前進
      mm2=KEY_DOWN;   //後退
	    mm3=KEY_PAUSE;    //進入待機模式			 
			if(mm3==0)      //如果pause键按下，画面暂停，再按一次则继续跑动
			{	   
					while((mm3==0)&&(mm2!=0)&&(mm1!=0))   //等待按键释放
					{mm3=KEY_PAUSE;}
	//				DelayMs(500);	
					mm3=1;					
					while(mm3!=0)   //等待按键被再次按下，允许画面继续跑动
					{mm3=KEY_PAUSE;}
				
					while(mm3==0)   //等待按键释放
					{mm3=KEY_PAUSE;}				
			}
			if((mm1==0)&&(mm2==0)&&(mm3!=0))      //如果+/-键按同时按下 ，进入自动跑画面模式
	    {
					HorA=~HorA;
					BEEP_BIBI();
					while(mm1==0||mm2==0)   //等待按键释放
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
* Description    : 十六進制顯示
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  

void ShowData_hex(char *p,u16 dat)    //十六進制顯示
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
* Description    : 十進制顯示
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/   
  
void ShowData(char *p,u16 dat)     //十進制顯示
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
 
//---------------------------------------以下内容请勿修改------------------------------------------
//=================================================================================================
void DelayMs(__IO uint32_t nTime)
{ //__IO==volatile //uint32_t ==无符号整型数
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
    SysTick_Config(72000);   //因为未使用外部时钟，所以此次参数  待定 ; ==/SysTick_Config(SystemCoreClock/1000); == SysTick_Config(72000);
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
	//while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET){}  //某种情况下USART_FLAG_TC不能立刻被清零，所以换用一下语句
	
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
		__set_FAULTMASK(1);      // 关闭所有中端
		NVIC_SystemReset();		 // 复位
		
	}
	else
	{;}
//	EXTI_ClearITPendingBit(EXTI_Line0);
//	EXTI_ClearFlag(EXTI_Line0);
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
