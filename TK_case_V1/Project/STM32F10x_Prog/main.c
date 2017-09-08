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
#include "IICS.h"
#include "STORE.h"
#include <T226.h>
#include "tsliic.h"
#include "OTP.h"
#include <V_SET.h>
#include <stdlib.h>
#include "M2481_Control.h"
#include <TSLIIC_Brightness.h>
#include <TSLIIC_REV01.h>

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
void ShowData_hex(char *p,u16 dat) ;   //十六進制顯示

void PassKey(void);
void IDD_IOIDD_show(void);
void  BACK_LIGHT_OFF(void);	
void  BACK_LIGHT_ON(void);	
//#define ERR_NO_SPACE	0xff
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
////-------------******* -----*********** --------------------------------------- 
void Delay(u32 time);
void DelaySec(u32 time);
extern void DelayKEY (u32 k); //按住，画面暂停
extern void KEYGPIO_Init(void);///*[把PB1/PB10配置成输入模式] 
extern u8 MIPI_READ_DATA[10];
extern u8 OTP_VALUE1 ;
extern u8 OTP_VALUE2;
extern u8 OTP_TIMES;

extern u8 ID1_VALUE ;
extern u8 ID2_VALUE ;
extern u8 ID3_VALUE ;

extern u8 Gamma_VALUE1;	
extern u8 Gamma_VALUE2;
extern u8 Gamma_VALUE3;
extern u8 Gamma_VALUE4;	
//------------------------------
extern u8 ID_OK;
extern u8 PIC_NUM;
u8 Flag_sleep = 0;
u8 pic_num=1;      //显示画面计数用
//u8 KeyFlagPause=255;  //按键扫描计数用
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
extern u8 ID1_READ,ID2_READ,ID3_READ;
extern u32 ID_SHOW;
////===================预设分割线******一些有用需要经常改动的参数放到这里吧,比如电流预设上限之类的============== 
u16 HFZIDD,HFZIDDIO;
u16 HFZVDD,HFZVDDIO;
u8 CAL_OK=0;
u8 FLAG_RESET=0;
u8 show_promise=0;
BYTE pucData[2];

extern u16 IDDCurrentOfSleep;  ////存放睡眠电流用
extern u16 IDDIOCurrentOfSleep;
extern u8  FlagOfCurrentSleep;  ////睡眠电流采集标志，非0时意味着已经采集到睡眠电流

u16 Vshunt_limit=0x9900;  //100mA所對應的分流電壓值  0~0x7fff ->iovdd     0xffff~0x8000 ->vdd

//=========================================================================================
//白画面
u16 IDD_WORK_LIMIT_VALUE_WHITE  =         3730;				/////产品工作IDD电流限定值 单位：10uA
u16	IOIDD_WORK_LIMIT_VALUE_WHITE  =		    1806;				/////产品工作IDDIO电流限定值
//彩图画面
u16 IDD_WORK_LIMIT_VALUE_COLOR  =         4000;				/////产品工作IDD电流限定值 单位：10uA
u16	IOIDD_WORK_LIMIT_VALUE_COLOR  =		    2500;				/////产品工作IDDIO电流限定值
//睡眠
u16	IDD_SLEEP_LIMIT_VALUE  =			  40;				/////产品SLEEP  IDD电流限定值 单位：uA
u16	IOIDD_SLEEP_LIMIT_VALUE  =		  80;				/////产品SLEEP  IDDIO电流限定值
//背光定电流输出
u16 led0pwmval=0x003D; //0x40

//==========================================================================================
u8  HorA=0xFF;                                   //HorA --->  0为手动跑画面    ，  1为自动跑画面

void LCD_Vol_SET(void)
//+++++++++++++++++++++++++++++++++LCD电压设定++++++++++++++++++++++++++++++++++++++++ 	
{
	Vol_GPIO_Configuration();
	
	VDD_ADJ_SET(285);			 //VDD_ADJ_SET 参数取值范围270~620，对应电压范围2.7V~6.2V; 
	VDD_ADJ_EN(1);				 //使能VDD输出		

	VDDIO_ADJ_SET(205);			  //VDDIO_ADJ_SET 参数取值范围150~360,对应电压范围1.5V~3.6V; 但是输出值因为二极管压降的问题，会比设定值低约0.3V, 
	VDDIO_ADJ_EN(1);			  //使能VDDIO输出
  	
	VOTP_ADJ_SET(300);			  //VOTP_ADJ_SET 参数取值范围150~2080，对应电压范围1.5V~14.1V; 
	VOTP_EN(0);					  //使能VOTP输出	
}	
/*------------------------------------USER  START，万恶的分割线-------------------------------------------*/
//================================================================================================================

int main(void)  
{ 
	u16 cird [100]; u16 cc;
	u16 mm_KEY_UP = 1 ;
	u16 mm_KEY_DOWN = 1 ;
	u16 mm_KEY_PAUSE = 1;
	u16 delay_time=0,kkk;
	
	
	u8 ID_RIGHT;
//	u8 PicNumTemp=1;
//	u32  m=0;
//	u32 sdcap=0;  
	SYSinitialize(); //STM32 系统时钟初始化	 
	DelayMs(10);
	//-----------------------------废话1分割线---------------------------

	KEYGPIO_Init();							 //----	---对按鍵板進行初始化	
	
	EEPROM_GPIO_Config();					 //----	---对芯片進行配置	
	EEpromRead_CurrentCalibration();		 //----读取EEprom存储的系数及电流校准
	
	mm_KEY_UP=KEY_UP;   //前進
	mm_KEY_DOWN=KEY_DOWN;   //後退
	mm_KEY_PAUSE=KEY_PAUSE;    //進入待機模式
	GPIO_ResetBits(GPIOC, GPIO_Pin_1); 	 //---beep  off--------$#@#&^&&^%^%$###	
//-------------------------------废话2分割线---------------------------	
	T226_intion();  //注意：电压输出受到STM32和226同时控制,故先配置INA226
	LCD_Vol_SET();		///////LCD 電壓設定,,,,//注意：电压输出受到STM32和226同时控制
	
//----------VDD大電流檢測 	-----IOVDD大電流檢測-------------------------
//--若此时电流超过20000/100=200ma则认为电源短路，切断电源，蜂鸣器一长2短鸣叫，同时背光闪烁	
  POWER_ON_CHECK();		
//--------------------------配合分割线----------------------------------------------		
	BACK_LIGHT_OFF(); //次函数包含了背光控制脚PC7的初始化
	BACK_LIGHT_ON();  //先执行 BACK_LIGHT_OFF();或PC7先初始化后，本函数才正常
//***************************废话说完了好像************************************

	STM32_Init_SSD2828();					 //----	---对SSD2828&LCD 初始化	
	TSL_GPIO_Config();
//	//-----------------------------背光驱动板---------------------------------------
     
	 M2481_EN_Pin_Configuration();	
   PWM_Pin_configuration();  //配置PWM 
	 TIM4_Configuration();			//配置PWM  
	 M2481_PWM_scan(1); 
////--------------------------------------------------------------------------------			
//	 EXTI_Configuration();    
  // Init_External_EEPROM();	       //注：此型号无法Debug，若要Debug，请屏蔽此句，但流水ID模块失效

	
while (1)
	{	
		
    Allcolor(1,WHITE_bmp);		     DelayMs(100);//刷白 	DelayKEY(2);	
		
		//	GAMMA_LINE_CHECK();	
		
		
		
//调光		
//		mm_KEY_DOWN = KEY_AUTO_MODE;
//		if(mm_KEY_DOWN==0)
//		{			
//			Adjust_Output_Current(1);
//			DelayKEY(5);
//		}
		
		////////以下代碼內部測試用。。。
//	mm_KEY_UP = KEY_UP;
//	if(mm_KEY_UP == 0)
//	{
//		SSD2828_ENTER_LP_mode();
//		ID_set();  				   ///////由於產品均已經燒錄ID了，無ID燒錄機會進行驗證。。。暫時擱置ID燒錄程式驗證！！！！！！！！
////////		MTP();
//		MTP_ID();
////////		reset_lcd();
//	}
//		Forward_scan();				         DelayMs(50);//正掃，該函數可根據實際需要去除

//--------------------------------------------------------------------------------		
//*************************************
////判斷ID or gamma是否未燒	 	           // 流水ID
//		ID_READ();      //ID    回读
//		Gamma_READ();   //Gamma 回读
		VCOM_READ();    //Vcom  回读
////*************************************
//		mm_KEY_DOWN = KEY_AUTO_MODE;
//		if(mm_KEY_DOWN!=0)
//		{	
//			Gamma_Check();
//			ID_CHECK();    		
//		}
		SSD2828_VIDEO_MODE_HS();
	  DelayMs(10);
//--------------------------------------------------------------------------------	
		
////請在此處定義您所需顯示的產品訊息！務必在尾部加上適當延時    ps*-->( min 50 ms )			
		ShowTxt("48,20,299, ");	  				             DelayMs(150);
        ShowTxt("60,20,0,052WA51_V1");	  				       DelayMs(150);	
        ShowTxt("60,20,100,FT8006M");	  				       DelayMs(150);	
		ShowTxt("48,20,299, ");	  				             DelayMs(150);
		
		ShowData_hex("30,450,1000", OTP_VALUE1);			   DelayMs(80);
//		ShowData_hex("30,450,1030", OTP_VALUE2);			   DelayMs(80);
//		ShowData_hex("30,450,1060", OTP_TIMES);			     DelayMs(80);
	
		switch(OTP_VALUE1)
		{
			case 0xA1: ShowTxt("60,20,1000,烧入0次");	   					DelayMs(150);   break;
//		    case 0x01: ShowTxt("60,20,1000,烧入1次");	   					DelayMs(150);   break; 
//		    case 0x03: ShowTxt("60,20,1000,烧入2次");	   					DelayMs(150);   break; 
//			case 0x07: ShowTxt("60,20,1000,烧入3次");	                    DelayMs(150);   break;
            default:
                ShowTxt("60,20,1000,已烧入");	   					DelayMs(150);   break; 
                break;
		}
		DelayKEY(350);
	
/////************************2014.10.30**************************************/
//		mm_KEY_DOWN = KEY_AUTO_MODE;
//		if(mm_KEY_DOWN==0)
//		{	
//		showbmp (17);							DelayMs(180);  //测量彩图电流
//    NORML_CAL_TEST(IDD_WORK_LIMIT_VALUE_COLOR,IOIDD_WORK_LIMIT_VALUE_COLOR); 	
//			
//    enter_sleep_mode();
//		DelayMs(1500);
//		DelayKEY(2);//供分析异常时使用
//		
//		SLEEP_CAL_TEST(IDD_SLEEP_LIMIT_VALUE,IOIDD_SLEEP_LIMIT_VALUE); 
//		
//    exit_sleep_mode();
//    DelayMs(5);
//		}
///**************************************************************/
		
////////===================================手自動選擇==========================================	
//   Backward_scan();	 	    DelayMs(50);   //反掃，該函數可根據實際需要去除
	 
/*手動模式*/ 
   mm_KEY_UP = KEY_UP;
	 if(mm_KEY_UP == 0)
	 {
		  showbmp (Flicker_OTP);				DelayMs(250);
		  ShowTxt("40,0,0,手动模式");	
		  while(mm_KEY_UP == 0)
			{
			 mm_KEY_UP = KEY_UP; 
			}
	    KEY_adjust();
	 }
/*_____________________________________________________________________________________*/	
	 
/*自動模式*/	 
	 mm_KEY_DOWN = KEY_AUTO_MODE;
	 if(mm_KEY_DOWN == 0)
	 {	
			showbmp (Flicker_OTP);				DelayMs(350);
      //延遲15sec
//			DelaySec(15);
			
			Auto_otp();	
            switch(OTP_VALUE1)
            {
                case 0xA1: ShowTxt("60,20,1000,烧入0次");	   					DelayMs(150);   break;
    //		    case 0x01: ShowTxt("60,20,1000,烧入1次");	   					DelayMs(150);   break; 
    //		    case 0x03: ShowTxt("60,20,1000,烧入2次");	   					DelayMs(150);   break; 
    //			case 0x07: ShowTxt("60,20,1000,烧入3次");	                    DelayMs(150);   break;
                default:
                    ShowTxt("60,20,1000,已烧入");	   					DelayMs(150);   break; 
                    break;
            }
//			switch(OTP_TIMES)
//			{
//			case 0x00: ShowTxt("60,20,1010,烧入0次");	   					DelayMs(150);   break;
//		  case 0xC1: ShowTxt("60,20,1010,烧入1次");	   					DelayMs(150);   break; 
//		  case 0xC3: ShowTxt("60,20,1010,烧入2次");	   					DelayMs(150);   break; 
//			case 0xC7: ShowTxt("60,20,1010,烧入3次");	            DelayMs(150);   break;
//			default:   ShowTxt("60,20,1010,烧入4次");	            DelayMs(150);   break;
//			}		 
			DelayKEY(350);
		 
			while(1);
//			{
//			   BACK_LIGHT_OFF();DelayMs(1000);BACK_LIGHT_ON();DelayMs(1000);
//			} 	 
	 }
//////================================SELECT OFF=========================================== 	
	 

//////判斷VCOM是否未燒	 
//////----------------------------------------------------------------------	 
	 else
	 {
		VCOM_TIMES_Check();   //回读vcom times
		SSD2828_VIDEO_MODE_HS();DelayMs(50);	 
	 	if(OTP_TIMES == 0x00)     //如果一次未燒，提示OTP未燒   
		{	                         //若IC無回讀次數功能，可將虛線內內容屏蔽
			OTP_NO();	
		}		 
	 }	 
//----------------------------------------------------------------------
	  DelayMs(250);   
//	Backward_scan();	 	    DelayMs(50);   //反掃，該函數可根據實際需要去除

/*循環電測畫面*/
//在lcd.c文件中定義正確的電測畫面總數	 ，特殊畫面--》flick_otp\flicker_QC請正確定義其編號
	LCDTEST:
	 switch(pic_num)
	 {
		case 1:  showbmp (1);     					DelayMs(120);   M2481_PWM_scan(2);  break;  //此换面至少停顿5秒 
	  case 2:  showbmp (2);     					DelayMs(120);   break;	//此换面至少停顿5秒		 
		case 3:  showbmp (3);     					DelayMs(120);   break;
		case 4:  showbmp (4);     					DelayMs(120);   break;
		case 5:  showbmp (5);     					DelayMs(120);   break;
		case 6:  showbmp (Flicker_OQC);	    DelayMs(120);   break;			
		case 7:  showbmp (7);               DelayMs(120);   break;	//此换面至少停顿5秒
		case 8:	 showbmp (8);               DelayMs(120);   break;	
		case 9:	 showbmp (9);               DelayMs(120);   break; 	//NGLINE		 
		case 10: showbmp (10);              DelayMs(120);   break;	
		case 11: showbmp (11);              DelayMs(120);   break;
		case 12: showbmp (12);              DelayMs(120);   break;
		case 13: showbmp (13);              DelayMs(120);   break;
		case 14: showbmp (14);              DelayMs(120);   break;	 
		case 15: showbmp (15);              DelayMs(120);   break;	//此换面至少停顿5秒
    case 16: showbmp (16);							DelayMs(120);   break;	
    case 17: showbmp (17);							DelayMs(120);   M2481_PWM_scan(2);   break;	
    case 18: M2481_PWM_scan(0);					showbmp (18);DelayMs(120);     break;			

   //%&^%&^%&^%&^%&%&^%&^%&^%*&%

	GAMMA_LINE_CHECK();	

	 }
	 
	 PassKey();   //HorA --->  0为手动跑画面    ，  1为自动跑画面
			 
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
					ShowTxt("38,20,299,IDD待机电流:(uA)"  );	               DelayMs(150);
					ShowData("38,260,349",IDDCurrentOfSleep);        DelayMs(150);
					ShowTxt("38,20,399,IDDIO待机电流:(uA)  ");	   					 DelayMs(250);
					//这一句比较奇怪，如果ShowTxt("38,20,399,IOVDD待机电流");,则无法显示，莫名其妙啊
					ShowData("38,260,449",IDDIOCurrentOfSleep);      DelayMs(150);	        
				}
    show_promise=0;				
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
			enter_sleep_mode();
			DelayMs(1500);
			
			SLEEP_CAL_TEST(IDD_SLEEP_LIMIT_VALUE,IOIDD_SLEEP_LIMIT_VALUE);    //测量待机电流
			
			mm3=1;				
			while(mm3!=0)
			{
				mm3=KEY_PAUSE;
			}		
			exit_sleep_mode();
			DelayMs(50); 
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
	   if(pic_num==17)
		 {
			DelayMs(7000);
		 }
     else if(pic_num==1)
		 {
      DelayMs(8000);	
		 }
		 else
		 {
			DelayMs(800);
		 }
		  
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

void BACK_LIGHT_ON(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_7);  // 
}

void BACK_LIGHT_OFF(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
	                                         | RCC_APB2Periph_AFIO, ENABLE); 
	//#define GPIO_Remap_SWJ_JTAGDisable ((u32)0x00300200)  /* JTAG-DP Disabled and SW-DP Enabled */ 禁止JTAG,以空出PB3,PB4
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//禁止jtag，以空出PB3,PB4,PA15
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//GPIO最高速度50MHz
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
void DelaySec(u32 time)
{
		  //延遲15sec
	    u32 i;
		  for(i=0;i<time;i++)
			{
				DelayMs(1000);
			}
}
 
//---------------------------------------以下内容请勿修改------------------------------------------
//=================================================================================================
void DelayMs(__IO uint32_t nTime)
{ //__IO==volatile //uint32_t ==无符号整型数
  TimingDelay = nTime;
  while(TimingDelay != 0); 
}
void SYSinitialize (void)
{  RCC_Configuration();
	 NVIC_Configuration();
   USART_REMAP_GPIO_Configuration(); 
   SysTick_Config(72000);   //因为未使用外部时钟，所以此次参数  待定 ; ==/SysTick_Config(SystemCoreClock/1000); == SysTick_Config(72000);
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
