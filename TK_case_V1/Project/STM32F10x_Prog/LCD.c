/******************** (C) COPYRIGHT 2010 KSGIANTLUS ********************
* File Name          : lcd.c
* Author             : JASON
* Version            : V1.0
* Date               : 2010.01.20
*************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "lcd.h"
#include <V_SET.h>
#include "T226.h"
#include "OTP.h"

extern void Delay(u32 nCount);
u8 MIPI_READ_DATA[10] = {0,0,0,0,0,0,0,0,0,0};
uint MIPI_Array[150];
u16 data = 0x400a;

extern u8 VCOM_register;
extern u8 ID_register;   
extern u8 ID1_VALUE;
extern u8 ID2_VALUE;
extern u8 ID3_VALUE;
extern u16 VCOMDC1;
extern void DelayMs(__IO uint32_t nTime);
extern u8 MTP_OVER;
/*******************************************************************************
ÔÚ´ËÌŽ¶¨ÁxÕý´_µÄëŠœy®‹Ãæ¿‚”µ	 £¬ÌØÊâ®‹Ãæ--¡·flick_otp\flicker_QCÕˆÕý´_¶¨ÁxÆä¾ŽÌ–


*******************************************************************************/  

//------------------------------
    //®‹Ãæ¶¨Áx…^
//------------------------------
u32 RED_bmp        =   0xff0000;
u32 GREEN_bmp      =   0x00ff00;
u32 BLUE_bmp       =   0x0000ff;
u32 WHITE_bmp      =   0xFFFFFF;
u32 BLACK_bmp      =   0x000000;
u32 GURY_45_bmp    =   0x2D2D2D;    //45»Ò
u32 GURY_60_bmp    =   0x3C3C3C;    //60»Ò
u32 GURY_128_bmp   =   0x7D7D7D;    //128»Ò


#define HXD800x1280_080WQ18_JD9366
//#define V480x800_SC7798
//#define V480x800_HX8369
//#define QHD540x960_HX8389
//#define HD720x1280
//#define HXD800x1280
//#define FHD1080x1920
//#define FHD1200x1920
// #define V480x800_GPM1461A0

//----------------------------------------------
#ifdef V480x800_HX8369       
//////mipi video mode setting
u16 value_HighSpeed = 400;
u16 value_Lane_select = 2;

////////timing  setting
u16  VDP= 800;     
u16  VBP= 16 ;    //16
u16  VFP= 19 ;    //19

u16  HDP= 480;     
u16  HBP= 58;     //	58
u16  HFP= 64;     //	64

u16	 VPW=4;  //Í¨³£²»ÐèÒªµ÷Õû
u16	 HPW=6;  //Í¨³£²»ÐèÒªµ÷Õû

/////////////CLAN050LL  USE    inition/////
u8 PIC_NUM=12;       //ëŠœy®‹Ãæ×ÜÊý
u8 Flicker_OTP=8;    //OTP flicker ®‹Ãæ¾ŽÌ–
u8 Flicker_OQC=1;    //QC flicker  ®‹Ãæ¾ŽÌ–
//-------------------------------------------------------------------
//vcom½×´ÎÉè¶¨·½Ê½
void VCOM_set(u8 vcom)
{
   MIPI_SPI_Write(0x03,0xb6,vcom,vcom);	   //set vcom	
}   
//IDÖµÉè¶¨·½Ê½
void ID_set(void)
{
   MIPI_SPI_Write(0x04,0xC3,0x55,0xC1,0x90);
}	
//--------------------------------------------------------------------

void SSD2828_inition_lcd(void)
{
//#=========================================================
//#  Main LDI Power ON Sequence
//#=========================================================
	//#password 
	MIPI_SPI_Write( 4,0xB9,0xFF,0x83,0x69);  
	Delay (1); 
	
	//#setting mipi 4-lane
	MIPI_SPI_Write( 16,0xBA,0x31,0x00,0x16,0xCA,0xB1,0x0A,0x00,0x10,0x28,0x02,0x21,0x21,0x9A,0x1A,0x8F); 
	Delay (1); 
	
	//#setting color 24-18-16bit
	MIPI_SPI_Write( 2,0x3A,0x70); 
	Delay (1); 
	
	//#setting GIP OK                                                                                                                                                                                         ========================================================================================= -----------------------------------------------------------------------------------------
	MIPI_SPI_Write( 93,0xD5,0x00,0x00,0x08,0x03,0x30,0x00,0x00,0x10,0x01,0x00,0x00,0x00,0x01,0x39,0x45,0x00,0x00,0x0C,0x44,0x39,0x47,0x05,0x00,0x02,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x37,0x59,0x18,0x10,0x00,0x00,0x05,0x00,0x00,0x40,0x28,0x69,0x40,0x00,0x00,0x00,0x00,0x00,0x26,0x49,0x08,0x00,0x00,0x00,0x04,0x00,0x00,0x51,0x38,0x79,0x51,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x07,0xF8,0x0F,0xFF,0xFF,0x07,0xF8,0x0F,0xFF,0xFF,0x00,0x20,0x5A);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
	//#                      1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   43   44   45   46   47   48   49   50   51   52   53   54   55   56   57   58   59   60   61   62   63   64   65   66   67   68   69   70   71   72   73   74   75   76   77   78   79   80   81   82   83   84   85   86   87   88   89   90   91   92
	//             (36,0xC1,0x03,0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78,0x80,0x88,0x90,0x98,0xA0,0xA8,0xB0,0xB8,0xC0,0xC8,0xD0,0xD8,0xE0,0xE8,0xF0,0xF8,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78,0x80,0x88,0x90,0x98,0xA0,0xA8,0xB0,0xB8,0xC0,0xC8,0xD0,0xD8,0xE0,0xE8,0xF0,0xF8,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,0x40,0x48,0x50,0x58,0x60,0x68,0x70); 
	Delay (1); 
	
	//#setting power
	MIPI_SPI_Write(13 ,0xB1,0x12,0x83,0x77,0x00,0x10,0x10,0x1E,0x1E,0x0C,0x1A,0x20,0xD6); 
	Delay (1); 
	
	//#Setting RGB INTERFACE
	MIPI_SPI_Write(5 ,0xB3,0x83,0x00,0x3A,0x17); 
	Delay (1); 
	
	//#Setting display cycle register 02-2dot inversion 00-column inversion 03-4dot inversion
	MIPI_SPI_Write( 2,0xB4,0x02); 
	Delay (1); 
	
	////#setting PANEL CHARACTERISTIC (FW)
	////#MIPI_SPI_Write(2,0xCC,0x02);  
	////#Delay (10); 
	
	//#setting PANEL CHARACTERISTIC (BW)
	MIPI_SPI_Write( 2,0xCC,0x0E);  
	Delay (1); 
	
//	MIPI_SPI_Write(0x04,0xC3,0x55,0xC0,0x90);

	//#setting ECO
	MIPI_SPI_Write(4 ,0xC6,0x41,0xFF,0x7C);  
	Delay (1); 
	
	//#setting MESSI
	MIPI_SPI_Write(2 ,0xEA,0x7A); 
	Delay (1); 
	
	//#setting source EQ
	MIPI_SPI_Write( 5,0xE3,0x0F,0x0F,0x11,0x11); 
	Delay (1); 
	
	//#setting source option
	MIPI_SPI_Write(7 ,0xC0,0x73,0x50,0x00,0x20,0xC4,0x00); 
	Delay (1); 
	
	//#setting Gamma curve DTC
	//#                       1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35 
	MIPI_SPI_Write( 36,0xE0,0x00,0x19,0x1F,0x36,0x3E,0x3F,0x2D,0x46,0x0A,0x11,0x11,0x14,0x15,0x15,0x16,0x12,0x16,0x00,0x19,0x1F,0x36,0x3E,0x3F,0x2C,0x45,0x09,0x0F,0x11,0x15,0x17,0x15,0x16,0x13,0x16,0x01); 
	Delay (1); 
	
	
	//#set Gamma correction reated setting-DGC ENABLE inital
	//spi.command 0x80); 
	//#                     //#1
	MIPI_SPI_Write( 101,0xC1,0x03,0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78,0x80,0x88,0x90,0x98,0xA0,0xA8,0xB0,0xB8,0xC0,0xC8,0xD0,0xD8,0xE0,0xE8,0xF0,0xF8,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,0x40,0x48,0x50,0x58,0x60,0x68,0x70,0x78,0x80,0x88,0x90,0x98,0xA0,0xA8,0xB0,0xB8,0xC0,0xC8,0xD0,0xD8,0xE0,0xE8,0xF0,0xF8,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x10,0x18,0x20,0x28,0x30,0x38,0x40,0x48,0x50,0x58,0x60,0x68,0x70); 
	MIPI_SPI_Write( 26,0x80,0x88,0x90,0x98,0xA0,0xA8,0xB0,0xB8,0xC0,0xC8,0xD0,0xD8,0xE0,0xE8,0xF0,0xF8,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00); 
	//spi.command 0x81); 
	Delay (1); 
//	///////-----------------debug-------------
//	MIPI_SPI_Write(0x04,0xC3,0x55,0xC0,0x90);
//	MTP_ID();	
	//#setting sleep out
	MIPI_SPI_Write( 1,0x11); 	
	Delay (200); 
	
	//#setting VCOM
	//MIPI_SPI_Write(3 ,0xB6,0xa3,0x9D); 
	//#                     set  set
//	Delay (10); 
	//#setting Display
	MIPI_SPI_Write( 1,0x29); 	
	Delay (20); 

}
void Forward_scan(void)
{
	SSD2828_ENTER_LP_mode();   	 Delay(5);
//#setting PANEL CHARACTERISTIC (FW)
  MIPI_SPI_Write(2,0xCC,0x02);  
  Delay (10); 
	SSD2828_VIDEO_MODE_HS();	 Delay(5);
	
}

void Backward_scan(void)
{
	SSD2828_ENTER_LP_mode();	Delay(5);
//#setting PANEL CHARACTERISTIC (BW)	
	MIPI_SPI_Write( 2,0xCC,0x0E);  
  Delay (10); 
	SSD2828_VIDEO_MODE_HS();	Delay(5);
}


/*******************************************************************************
* Function Name  : void MTP(void)
* Description    : otp -> vcom     //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void MTP(void)
{

	MIPI_SPI_Write( 4,0xB9,0xFF,0x83,0x69);  
	Delay (10); 
	

	MIPI_SPI_Write(4 ,0xB6,VCOMDC1,VCOMDC1,0x00); 	 ////set vcom		
	///display off
	MIPI_SPI_Write(0x01,0x28);
	Delay(60);			 //delay 40ms

	SSD2828_VIDEO_MODE_HS();   Delay(15);

	SSD2828_ENTER_LP_mode();Delay(5);

	MIPI_SPI_Write(0x02,0xb7,0xAA);
	MDelay(20);			   ////delay 1ms

	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x0d,0x00);
	Delay(100);			   ////delay 1ms

	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x0d,0x00);		   ///////////////////set otp_mask[7:0]
	Delay(100);

	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x0d,0x00);			/////////////set otp address
	Delay(100);

	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x0d,0x01);		   //////////////////set otp_prog = 1;
	Delay(100);		 		///DELAY 11ms

//////MTP   OVER ,,,OTP  Disable
	MIPI_SPI_Write(0x02,0xb7,0xff);
	MDelay(20);	
	
	MDelay(200);			   ////delay 1ms	

	MIPI_SPI_Write(0x01,0x29);
	Delay(50);			 //delay 40ms
}

/*******************************************************************************
* Function Name  : void MTP_ID(void)
* Description    : otp -> ID       //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  

void MTP_ID(void)
{
	MIPI_SPI_Write( 4,0xB9,0xFF,0x83,0x69);  
  	Delay (10); 

		///display off
	MIPI_SPI_Write(0x01,0x28);
	Delay(60);			 //delay 40ms

	SSD2828_VIDEO_MODE_HS();   Delay(15);

	SSD2828_ENTER_LP_mode();Delay(5);


	MIPI_SPI_Write( 4,0xB9,0xFF,0x83,0x69);  

	MDelay(200);			   ////delay 1ms	
	
	MIPI_SPI_Write(0x02,0xb7,0xAA);
	MDelay(20);			   ////delay 1ms



	/////ÉÕÂ¼otpµÄ0x01µÄID1
	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x01,0x00);		   ///////////////set otp_index[8:0]
	MDelay(20);			   ////delay 1ms

	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x01,0x00);		   ///////////////set otp_index[8:0]
	MDelay(20);

	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x01,0x00);		   ///////////////////set otp_mask[7:0]
	MDelay(20);

	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x01,0x00);			/////////////set otp address
	MDelay(20);

	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x01,0x01);		   //////////////////set otp_prog = 1;
	MDelay(45);		 		///DELAY 11ms


//	/////ÉÕÂ¼otpµÄ0x01µÄID2
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x02,0x00);		   ///////////////set otp_index[8:0]
//	MDelay(20);			   ////delay 1ms
//
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x02,0x00);		   ///////////////set otp_index[8:0]
//	MDelay(20);
//
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x02,0x00);		   ///////////////////set otp_mask[7:0]
//	MDelay(20);
//
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x02,0x00);			/////////////set otp address
//	MDelay(20);
//
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x02,0x01);		   //////////////////set otp_prog = 1;
//	MDelay(45);		 		///DELAY 11ms
//
//
//	/////ÉÕÂ¼otpµÄ0x01µÄID3
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x03,0x00);		   ///////////////set otp_index[8:0]
//	MDelay(20);			   ////delay 1ms
//
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x03,0x00);		   ///////////////set otp_index[8:0]
//	MDelay(20);
//
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x03,0x00);		   ///////////////////set otp_mask[7:0]
//	MDelay(20);
//
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x03,0x00);			/////////////set otp address
//	MDelay(20);
//
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x03,0x01);		   //////////////////set otp_prog = 1;
//	MDelay(45);		 		///DELAY 11ms


	//////MTP   OVER ,,,OTP  Disable
	MIPI_SPI_Write(0x02,0xb7,0xff);
	MDelay(20);	
	
	MDelay(200);			   ////delay 1ms	

	MIPI_SPI_Write(0x01,0x29);
	Delay(50);			 //delay 40ms
}

#endif

#ifdef V480x800_GPM1461A0

//////mipi video mode setting
u16 value_HighSpeed = 360;
u16 value_Lane_select = 2;

////////timing  setting
u16  VDP= 854;     
u16  VBP= 16 ;    //
u16  VFP= 19 ;    //

u16  HDP= 480;     
u16  HBP= 58;     //	
u16  HFP= 64;     //	

u16	 VPW=4;  //Í¨³£²»ÐèÒªµ÷Õû
u16	 HPW=6;  //Í¨³£²»ÐèÒªµ÷Õû


/////////////CLAN043LL  USE    inition/////
u8 PIC_NUM=11;       //ëŠœy®‹Ãæ×ÜÊý
u8 Flicker_OTP=3;    //OTP flicker ®‹Ãæ¾ŽÌ–
u8 Flicker_OQC=2;    //QC flicker  ®‹Ãæ¾ŽÌ–
//-------------------------------------------------------------------
//vcom½×´ÎÉè¶¨·½Ê½
void VCOM_set(u8 vcom)
{
//   MIPI_SPI_Write(0x04,0xb6,vcom,vcom,0X00);	   //set vcom	
}   
//IDÖµÉè¶¨·½Ê½
void ID_set(void)
{
//   MIPI_SPI_Write(0x04,0xC3,0x55,0x44,0xF0);
}	
/////////////GPM1461A0  USE    inition/////

void SSD2828_inition_lcd(void)
{
	//////////////////////////////////////////////////////////////////////
//Panel:CPT_045LG 480X854
//VPNL :3.3V
//VDDI :3.3V
//Pump :X2
//VGH  :15.38V
//VGL  :-7.85V  <- The flicker is minimum if VGL is -7.85V
//GVDD :4.5V    <- To change voltage from 5V to 4.8V Because of gray level inversion
//NGVDD:-4.5V   <- To change voltage from -5V to -4.8V Because of gray level inversion
//////////////////////////////////////////////////////////////////////
//--------------Command 2 enable---------------------//
		MIPI_SPI_Write(0x02,0x00,0x00);
		MIPI_SPI_Write(0x04,0xFF,0x80,0x09,0x01);
		
		MIPI_SPI_Write(0x02,0x00,0x80);
		MIPI_SPI_Write(0x03,0xFF,0x80,0x09);
		
		MIPI_SPI_Write(0x02,0x00,0x03);
		MIPI_SPI_Write(0x02,0xFF,0x01);
		//--------------Panel setting---------------------//
		MIPI_SPI_Write(0x02,0x00,0x90);
		MIPI_SPI_Write(0x07,0xC5,0x16,0x82,0x02,0x03,0x33,0x34);//ripple pass
		
		//MIPI_SPI_Write(0x02,0x00,0x90);
		//MIPI_SPI_Write(0x07,0xC5,0x96,0x82,0x02,0x03,0x33,0x34);//audible noise pass
		
		//MIPI_SPI_Write(0x02,0x00,0x90);
		//MIPI_SPI_Write(0x04,0xC5,0x96,0x82,0x00);//audible noise NG
		
		MIPI_SPI_Write(0x02,0x00,0xB2);
		MIPI_SPI_Write(0x02,0xC5,0x08);
		
		MIPI_SPI_Write(0x02,0x00,0x00);
		MIPI_SPI_Write(0x03,0xD8,0x6f,0x6f);
		
//		MIPI_SPI_Write(0x02,0x00,0x00);
//		MIPI_SPI_Write(0x02,0xD9,0x33);
		
		MIPI_SPI_Write(0x02,0x00,0x82);
		MIPI_SPI_Write(0x02,0xC5,0xA3);
		
		MIPI_SPI_Write(0x02,0x00,0x81);
		MIPI_SPI_Write(0x02,0xC1,0x55);
		
		MIPI_SPI_Write(0x02,0x00,0xB4);
		MIPI_SPI_Write(0x03,0xC0,0x50,0x18);
		
		MIPI_SPI_Write(0x02,0x00,0x81);
		MIPI_SPI_Write(0x02,0xC4,0x83);
		
		MIPI_SPI_Write(0x02,0x00,0x92);//480x854
		MIPI_SPI_Write(0x02,0xB3,0x45);
		
		MIPI_SPI_Write(0x02,0x00,0x90);
		MIPI_SPI_Write(0x03,0xB3,0x02,0x20);
		
		MIPI_SPI_Write(0x02,0x00,0x90);
		MIPI_SPI_Write(0x07,0xC0,0x00,0x44,0x00,0x00,0x00,0x03);
		
		MIPI_SPI_Write(0x02,0x00,0xA0);
		MIPI_SPI_Write(0x02,0xC1,0xEA);
		
		MIPI_SPI_Write(0x02,0x00,0xC0);
		MIPI_SPI_Write(0x02,0xC5,0x00);
		
		MIPI_SPI_Write(0x02,0x00,0x8B);
		MIPI_SPI_Write(0x02,0xB0,0x40);
		
		MIPI_SPI_Write(0x02,0x00,0xB1);
		MIPI_SPI_Write(0x02,0xB0,0x80);
		
		MIPI_SPI_Write(0x02,0x00,0x88);
		MIPI_SPI_Write(0x02,0xC4,0x80);
		
		MIPI_SPI_Write(0x02,0x00,0xA6);
		MIPI_SPI_Write(0x04,0xC1,0x01,0x00,0x00);
		
		MIPI_SPI_Write(0x02,0x00,0xB2);
		MIPI_SPI_Write(0x05,0xF5,0x15,0x00,0x15,0x00);
		//--------------Gamma setting------------------------//
		MIPI_SPI_Write(0x02,0x00,0x00); 
		MIPI_SPI_Write(0x11,0xE1,0x05,0x0e,0x14,0x0d,0x06,0x0f,0x0b,0x0a,0x04,0x07,0x0c,0x08,0x0f,0x10,0x0a,0x05);
		                         
		MIPI_SPI_Write(0x02,0x00,0x00); 
		MIPI_SPI_Write(0x11,0xE2,0x05,0x0e,0x14,0x0d,0x06,0x0f,0x0b,0x0a,0x04,0x07,0x0c,0x08,0x0f,0x10,0x0a,0x05); 
		//----------------ID setting-----------------//
		//MIPI_SPI_Write(0x02,0x00,0x00);
		//MIPI_SPI_Write(0x02,0xD0,0xD5);
		//
		//MIPI_SPI_Write(0x02,0x00,0x00);
		//MIPI_SPI_Write(0x03,0xD1,0x80,0xCA);
		//---------------GOA setting-----------------//
		
		MIPI_SPI_Write(0x02,0x00,0x80);
		MIPI_SPI_Write(0x07,0xCE,0x87,0x03,0x00,0x86,0x03,0x00);
		
		MIPI_SPI_Write(0x02,0x00,0x90);
		MIPI_SPI_Write(0x07,0xCE,0x33,0x55,0x00,0x33,0x56,0x00);
		
		MIPI_SPI_Write(0x02,0x00,0xA0);
		MIPI_SPI_Write(0x0F,0xCE,0x38,0x03,0x03,0x54,0x00,0x00,0x00,0x38,0x02,0x03,0x55,0x00,0x00,0x00);
		
		MIPI_SPI_Write(0x02,0x00,0xB0);
		MIPI_SPI_Write(0x0F,0xCE,0x38,0x01,0x03,0x56,0x00,0x00,0x00,0x38,0x00,0x03,0x57,0x00,0x00,0x00);
		
		MIPI_SPI_Write(0x02,0x00,0xC0);
		MIPI_SPI_Write(0x0F,0xCE,0x30,0x00,0x03,0x58,0x00,0x00,0x00,0x30,0x01,0x03,0x59,0x00,0x00,0x00);
		
		MIPI_SPI_Write(0x02,0x00,0xD0);
		MIPI_SPI_Write(0x0F,0xCE,0x30,0x02,0x03,0x5A,0x00,0x00,0x00,0x30,0x03,0x03,0x5B,0x00,0x00,0x00);
		
		MIPI_SPI_Write(0x02,0x00,0xC0);
		MIPI_SPI_Write(0x0B,0xCF,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x00,0x10);
		
		MIPI_SPI_Write(0x02,0x00,0xC0);
		MIPI_SPI_Write(0x10,0xCB,0x00,0x14,0x14,0x14,0x14,0x00,0x00,0x14,0x14,0x14,0x14,0x00,0x00,0x00,0x00);
		
		MIPI_SPI_Write(0x02,0x00,0xD0);
		MIPI_SPI_Write(0x10,0xCB,0x00,0x00,0x00,0x00,0x00,0x00,0x14,0x14,0x14,0x14,0x00,0x00,0x14,0x14,0x14);
		
		MIPI_SPI_Write(0x02,0x00,0xE0);
		MIPI_SPI_Write(0x0B,0xCB,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
		
		MIPI_SPI_Write(0x02,0x00,0x80);
		MIPI_SPI_Write(0x0B,0xCC,0x00,0x26,0x25,0x02,0x06,0x00,0x00,0x0A,0x0c,0x0e);
		
		MIPI_SPI_Write(0x02,0x00,0x90);
		MIPI_SPI_Write(0x10,0xCC,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x26,0x25,0x01,0x05);
		
		MIPI_SPI_Write(0x02,0x00,0xA0);
		MIPI_SPI_Write(0x10,0xCC,0x00,0x00,0x09,0x0b,0x0d,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
		
		MIPI_SPI_Write(0x02,0x00,0xB0);
		MIPI_SPI_Write(0x0b,0xCC,0x00,0x25,0x26,0x05,0x01,0x00,0x00,0x0D,0x0B,0x09);
		
		MIPI_SPI_Write(0x02,0x00,0xC0);
		MIPI_SPI_Write(0x10,0xCC,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x25,0x26,0x06,0x02);
		
		MIPI_SPI_Write(0x02,0x00,0xD0);
		MIPI_SPI_Write(0x10,0xCC,0x00,0x00,0x0E,0x0C,0x0A,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
		
		
		MIPI_SPI_Write(0x02,0x00,0x89);//source pre-charge ON
		MIPI_SPI_Write(0x02,0xc4,0x00);
		
		MIPI_SPI_Write(0x02,0x00,0x87);//source pre-charge ON
		MIPI_SPI_Write(0x02,0xc4,0x00);
		
		MIPI_SPI_Write(0x02,0x00,0xa2);//source pre-charge width
		MIPI_SPI_Write(0x04,0xc0,0x0b,0x1a,0x02);
		
		MIPI_SPI_Write(0x02,0x00,0xc6);//MIPI read delay
		MIPI_SPI_Write(0x02,0xb0,0x03);
		
		MIPI_SPI_Write(0x02,0x00,0x80);//blanking & sleep in source pull-low
		MIPI_SPI_Write(0x02,0xc4,0x30);
		
		MIPI_SPI_Write(0x02,0x00,0x8a);//blanking & sleep in source pull-low
		MIPI_SPI_Write(0x02,0xc4,0x40);
		
		MIPI_SPI_Write(0x02,0x00,0xb6);//sleep out power on seq. 
		MIPI_SPI_Write(0x02,0xf5,0x06);
		
		MIPI_SPI_Write(0x02,0x00,0x92);//sleep out power on seq. 
		MIPI_SPI_Write(0x08,0xf5,0x0e,0x14,0x0e,0x14,0x00,0x14,0x00);
		
		MIPI_SPI_Write(0x02,0x00,0xba);//sleep out power on seq. 
		MIPI_SPI_Write(0x03,0xf5,0x00,0x00);
		
		//---------------------------------------------------//
		//MIPI_SPI_Write(0x02,0x00,0x00);
		//MIPI_SPI_Write(0x04,0xFF,0xFF,0xFF,0xFF);  
		//---------------------------------------------------//
		MIPI_SPI_Write(0x02,0x00,0x91);
		MIPI_SPI_Write(0x02,0xB3,0x20);    
		
		Delay( 20);
		MIPI_SPI_Write(0x01,0x11);
		
		Delay( 250);
		MIPI_SPI_Write(0x01,0x29);  		
}

void Forward_scan(void)
{
//	SSD2828_ENTER_LP_mode();   	 Delay(5);
//	MIPI_SPI_Write(2,0xCC,0x00);
//	SSD2828_VIDEO_MODE_HS();	 Delay(5);
}

void Backward_scan(void)
{
//	SSD2828_ENTER_LP_mode();	Delay(5);
//	MIPI_SPI_Write(2,0xCC,0x0C);
//	SSD2828_VIDEO_MODE_HS();	Delay(5);
}

/*******************************************************************************
* Function Name  : void MTP(void)
* Description    : otp -> vcom     //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void MTP(void)
{

//	MIPI_SPI_Write( 4,0xB9,0xFF,0x83,0x69);  
//	Delay (10); 
//	
//
//	MIPI_SPI_Write(4 ,0xB6,VCOMDC1,VCOMDC1,0x00); 	 ////set vcom		
//	///display off
//	MIPI_SPI_Write(0x01,0x28);
//	Delay(60);			 //delay 40ms
//
//	SSD2828_VIDEO_MODE_HS();   Delay(15);
//
//	SSD2828_ENTER_LP_mode();Delay(5);
//
//	MIPI_SPI_Write(0x02,0xb7,0xAA);
//	MDelay(20);			   ////delay 1ms
//
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x0d,0x00);
//	Delay(100);			   ////delay 1ms
//
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x0d,0x00);		   ///////////////////set otp_mask[7:0]
//	Delay(100);
//
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x0d,0x00);			/////////////set otp address
//	Delay(100);
//
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x0d,0x01);		   //////////////////set otp_prog = 1;
//	Delay(100);		 		///DELAY 11ms
//
////////MTP   OVER ,,,OTP  Disable
//	MIPI_SPI_Write(0x02,0xb7,0xff);
//	MDelay(20);	
//	
//	MDelay(200);			   ////delay 1ms	
//
//	MIPI_SPI_Write(0x01,0x29);
//	Delay(50);			 //delay 40ms
}

/*******************************************************************************
* Function Name  : void MTP_ID(void)
* Description    : otp -> ID       //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  

void MTP_ID(void)
{
//	MIPI_SPI_Write( 4,0xB9,0xFF,0x83,0x69);  
//  	Delay (10); 
//
//		///display off
//	MIPI_SPI_Write(0x01,0x28);
//	Delay(60);			 //delay 40ms
//
//	SSD2828_VIDEO_MODE_HS();   Delay(15);
//
//	SSD2828_ENTER_LP_mode();Delay(5);
//
//
//	MIPI_SPI_Write( 4,0xB9,0xFF,0x83,0x69);  
//
//	MDelay(200);			   ////delay 1ms	
//	
//	MIPI_SPI_Write(0x02,0xb7,0xAA);
//	MDelay(20);			   ////delay 1ms
//
//
//
//	/////ÉÕÂ¼otpµÄ0x01µÄID1
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x01,0x00);		   ///////////////set otp_index[8:0]
//	MDelay(20);			   ////delay 1ms
//
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x01,0x00);		   ///////////////set otp_index[8:0]
//	MDelay(20);
//
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x01,0x00);		   ///////////////////set otp_mask[7:0]
//	MDelay(20);
//
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x01,0x00);			/////////////set otp address
//	MDelay(20);
//
//	MIPI_SPI_Write(0x06,0xb7,0xAA,0x00,0x00,0x01,0x01);		   //////////////////set otp_prog = 1;
//	MDelay(45);		 		///DELAY 11ms
//
//
//	//////MTP   OVER ,,,OTP  Disable
//	MIPI_SPI_Write(0x02,0xb7,0xff);
//	MDelay(20);	
//	
//	MDelay(200);			   ////delay 1ms	
//
//	MIPI_SPI_Write(0x01,0x29);
//	Delay(50);			 //delay 40ms
}

#endif

#ifdef V480x800_SC7798
//////mipi video mode setting
u16 value_HighSpeed = 380;
u16 value_Lane_select = 2;

////////timing  setting
u16  VDP= 800;     
u16  VBP= 16 ;    //
u16  VFP= 19 ;    //

u16  HDP= 480;     
u16  HBP= 58;     //	
u16  HFP= 64;     //	

u16	 VPW=4;  //Í¨³£²»ÐèÒªµ÷Õû
u16	 HPW=6;  //Í¨³£²»ÐèÒªµ÷Õû

/////////////CLAN043LL  USE    inition/////
u8 PIC_NUM=13;       //ëŠœy®‹Ãæ×ÜÊý
u8 Flicker_OTP=3;    //OTP flicker ®‹Ãæ¾ŽÌ–
u8 Flicker_OQC=2;    //QC flicker  ®‹Ãæ¾ŽÌ–
//-------------------------------------------------------------------
//vcom½×´ÎÉè¶¨·½Ê½
void VCOM_set(u8 vcom)
{
   MIPI_SPI_Write(0x04,0xb6,vcom,vcom,0X00);	   //set vcom	
}   
//IDÖµÉè¶¨·½Ê½
void ID_set(void)
{
   MIPI_SPI_Write(0x04,0xC3,0x55,0x44,0xF0);
}	
//--------------------------------------------------------------------

void SSD2828_inition_lcd(void)
{  
	MIPI_SPI_Write(0x01,0x01);
//	READ_IC(0x0F,0x00);
	Delay(25);
	///////FOR CLAN043LL USE
	/////#password(Enable user command) 
	MIPI_SPI_Write(0x04,0xB9,0xF1,0x08,0x00);
	///////#VDDD
	Delay(2);
	MIPI_SPI_Write(2,0xBC,0x67);
	///////#set DSI (2Lane)
	Delay(2);
	MIPI_SPI_Write(19,0xBA,0x31,0x00,0x44,0x25,0x91,0x0A,0x00,0x00,0xC2,0x34,0x00,0x00,0x04,0x02,0x1D,0xB9,0xEE,0x40);
///////#set panel(Enable ESD finction)	
	Delay(2);
	MIPI_SPI_Write(4,0xC6,0x00,0x00,0xFF);
///////#set panel(Forward,TN)	
	Delay(2);
	MIPI_SPI_Write(2,0xCC,0x0C);
///////#set DISP(480RGB*800)	
	Delay(2);
	MIPI_SPI_Write(2,0xB2,0x23);
///////#set (column-00),(1dot-01),(2dot-02),(4dot-03),(8dot-04)	
	Delay(2);
	MIPI_SPI_Write(2,0xB4,0x00);
///////#set RGB 
	Delay(2);
	MIPI_SPI_Write(9,0xB3,0x01,0x00,0x06,0x06,0x10,0x0A,0x45,0x40);
///////#set GIP
	Delay(2);
	MIPI_SPI_Write(52,0xE9,0x00,0x00,0x08,0x03,0x2F,0x89,0x6A,0x12,0x31,0x23,0x48,0x0C,0x89,0x6A,0x47,0x02,0x04,0x00,0x00,0x00,0x00,0x20,0x88,0x88,0x40,0x28,0x69,0x48,0x88,0x88,0x80,0x88,0x88,0x51,0x38,0x79,0x58,0x88,0x88,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
///////#set GIP
	Delay(2);
	MIPI_SPI_Write(37,0xEA,0x88,0x88,0x37,0x59,0x18,0x18,0x88,0x88,0x85,0x88,0x88,0x26,0x49,0x08,0x08,0x88,0x88,0x84,0x30,0x00,0x00,0xFF,0x00,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
///////#set power
	Delay(2);
	MIPI_SPI_Write(8,0xB1,0x22,0x1B,0x1B,0xB7,0x22,0x02,0xA8);
	Delay(2);
	MIPI_SPI_Write(8,0xB1,0x22,0x1B,0x1B,0xB7,0x22,0x02,0xA8);
///////#set Source EQ
	Delay(2);
	MIPI_SPI_Write(8,0xE3,0x02,0x02,0x02,0x02);
///////#Set Power_ext
	Delay(2);
	MIPI_SPI_Write(3,0xB8,0x03,0x22);
///////#VREF
	Delay(2);
	MIPI_SPI_Write(3,0xB5,0x09,0x09);
///////#set Bias
	Delay(2);
	MIPI_SPI_Write(6,0xC0,0x73,0x50,0x00,0x08,0x70);
///////#set gamma
	Delay(2);
	MIPI_SPI_Write(35,0xE0,0x00,0x00,0x00,0x04,0x04,0x0A,0x18,0x2B,0x05,0x0C,0x11,0x16,0x18,0x16,0x16,0x15,0x19,0x00,0x00,0x00,0x04,0x04,0x0A,0x18,0x2C,0x05,0x0C,0x12,0x16,0x18,0x16,0x17,0x16,0x19);
///////#DGC
	Delay(2);
	MIPI_SPI_Write(128,0xC1,0x01,0x00,0x03,0x0B,0x17,0x20,0x26,0x30,0x36,0x3F,0x48,0x50,0x58,0x5F,0x68,0x71,0x79,0x82,0x89,0x91,0x99,
						//R01   02   03   04   05   06   07   08   09   10   11   12   13   14   15   16   17   18   19   20   21   
						 0xA1,0xA9,0xB0,0xB8,0xC0,0xC8,0xD0,0xD6,0xDC,0xE4,0xED,0xF7,0xFF,0x00,0x00,0x00,0xC0,0x0A,0x80,0x30,0xE8,
						//22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   
						 0x00,0x00,0x03,0x0B,0x17,0x20,0x27,0x30,0x37,0x40,0x48,0x50,0x58,0x5F,0x67,0x70,0x78,0x80,0x88,0x90,0x98,
					   //G43   44   45   46   47   48   49   50   51   52   53   54   55   56   57   58   59   60   61   62   63   
						 0xA0,0xA8,0xAF,0xB7,0xBF,0xC7,0xCF,0xD5,0xDC,0xE4,0xED,0xF7,0xFF,0x00,0x00,0x00,0xC0,0x0A,0x80,0x30,0xE8,
						//64   65   66   67   68   69   70   71   72   73   74   75   76   77   78   79   80   81   82   83   84   
						 0x00,0x00,0x03,0x0D,0x19,0x23,0x29,0x31,0x38,0x41,0x48,0x4F,0x57,0x5D,0x65,0x6C,0x74,0x7B,0x84,0x8B,0x94,
					    //B85   86   87   88   89   90   91   92   93   94   95   96   97   98   99  100  101  102  103  104  105   
						 0x9B,0xA4,0xAA,0xB4,0xB9,0xC3,0xCC,0xD6,0xDC,0xE4,0xED,0xF7,0xFF,0x00,0x00,0x00,0xC0,0x0A,0x80,0x30,0xE8,0x00
						//106  107  108  109  110  111  112  113  114  115  116  117  118  119  120  121  122  123  124  125  126  127
						);

	Delay(20);
		 
//////#setting sleep out	
	MIPI_SPI_Write(0x01,0x11);
	Delay(150);
//	READ_IC(0x0F,0x00);
///////#setting Display	
	MIPI_SPI_Write(0x01,0x29);
	Delay(20);
}

void Forward_scan(void)
{
	SSD2828_ENTER_LP_mode();   	 Delay(5);
	MIPI_SPI_Write(2,0xCC,0x00);
	SSD2828_VIDEO_MODE_HS();	 Delay(5);
}

void Backward_scan(void)
{
	SSD2828_ENTER_LP_mode();	Delay(5);
	MIPI_SPI_Write(2,0xCC,0x0C);
	SSD2828_VIDEO_MODE_HS();	Delay(5);
}

/*******************************************************************************
* Function Name  : void MTP(void)
* Description    : otp -> vcom     //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void MTP(void)
{
	///display off
	MIPI_SPI_Write(0x01,0x28);
	Delay(50);			 //delay 40ms

	MIPI_SPI_Write(0x02,0xb7,0x5a);
	MDelay(20);			   ////delay 1ms

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x0d,0x00);		   ///////////////set otp_index[8:0]
	MDelay(20);			   ////delay 1ms

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x0d,0x00);		   ///////////////set otp_index[8:0]
	MDelay(20);

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x0d,0x00);		   ///////////////////set otp_mask[7:0]
	MDelay(20);

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x0d,0x00);			/////////////set otp address
	MDelay(20);

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x0d,0x01);		   //////////////////set otp_prog = 1;
	MDelay(45);		 		///DELAY 11ms

	//////MTP   OVER ,,,OTP  Disable
	MIPI_SPI_Write(0x02,0xb7,0xff);
	MDelay(20);	

	MIPI_SPI_Write(0x01,0x29);
	Delay(50);			 //delay 40ms
}

/*******************************************************************************
* Function Name  : void MTP_ID(void)
* Description    : otp -> ID       //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  

void MTP_ID(void)
{
		///display off
	MIPI_SPI_Write(0x01,0x28);
	Delay(50);			 //delay 40ms

	MIPI_SPI_Write(0x02,0xb7,0x5a);
	MDelay(20);			   ////delay 1ms

	/////ÉÕÂ¼otpµÄ0x01µÄID1
	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x01,0x00);		   ///////////////set otp_index[8:0]
	MDelay(20);			   ////delay 1ms

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x01,0x00);		   ///////////////set otp_index[8:0]
	MDelay(20);

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x01,0x00);		   ///////////////////set otp_mask[7:0]
	MDelay(20);

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x01,0x00);			/////////////set otp address
	MDelay(20);

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x01,0x01);		   //////////////////set otp_prog = 1;
	MDelay(45);		 		///DELAY 11ms


	/////ÉÕÂ¼otpµÄ0x01µÄID2
	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x02,0x00);		   ///////////////set otp_index[8:0]
	MDelay(20);			   ////delay 1ms

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x02,0x00);		   ///////////////set otp_index[8:0]
	MDelay(20);

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x02,0x00);		   ///////////////////set otp_mask[7:0]
	MDelay(20);

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x02,0x00);			/////////////set otp address
	MDelay(20);

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x02,0x01);		   //////////////////set otp_prog = 1;
	MDelay(45);		 		///DELAY 11ms


	/////ÉÕÂ¼otpµÄ0x01µÄID3
	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x03,0x00);		   ///////////////set otp_index[8:0]
	MDelay(20);			   ////delay 1ms

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x03,0x00);		   ///////////////set otp_index[8:0]
	MDelay(20);

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x03,0x00);		   ///////////////////set otp_mask[7:0]
	MDelay(20);

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x03,0x00);			/////////////set otp address
	MDelay(20);

	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x03,0x01);		   //////////////////set otp_prog = 1;
	MDelay(45);		 		///DELAY 11ms


	//////MTP   OVER ,,,OTP  Disable
	MIPI_SPI_Write(0x06,0xb7,0xff,0x00,0x00,0x00,0x00);
	MDelay(20);	

	MIPI_SPI_Write(0x01,0x29);
	Delay(50);			 //delay 40ms
}

#endif




//================================================================================================
#ifdef QHD540x960_HX8389
//////mipi video mode setting
u16 value_HighSpeed = 480;
u16 value_Lane_select = 2;

u16  VDP= 960;      
u16 VBP= 10;   //
u16 VFP= 9;

u16  HDP= 540;
u16 HBP= 66;   //12
u16 HFP= 22;	

u16	 HPW=20;  //Í¨³£²»ÐèÒªµ÷Õû

u16	 VPW=4;  //Í¨³£²»ÐèÒªµ÷Õû
/////////////CLAN050LQ  USE    inition/////
u8 PIC_NUM=14;       //ëŠœy®‹Ãæ×ÜÊý
u8 Flicker_OTP=3;    //OTP flicker ®‹Ãæ¾ŽÌ–
u8 Flicker_OQC=1;    //QC flicker  ®‹Ãæ¾ŽÌ–
//-------------------------------------------------------------------
//vcom½×´ÎÉè¶¨·½Ê½
void VCOM_set(u8 vcom)
{
   MIPI_SPI_Write(4,0xB6,vcom,vcom,0x00);
}   
//IDÖµÉè¶¨·½Ê½
void ID_set(void)
{
   ////#B9 Set EXTC
	MIPI_SPI_Write(4,0xB9,0xFF,0x83,0x89);
	////#                   1    2    3
//	Delay (2);
	MIPI_SPI_Write(0x04,0xC3,0x53,0x44,0x90);
}	

void SSD2828_inition_lcd(void)    //IC³õÊ¼»¯
{
	
	////#Sleep out
	MIPI_SPI_Write(1,0x11);
	DelayMs (300);
	
	////#B9 Set EXTC
	MIPI_SPI_Write(4,0xB9,0xFF,0x83,0x89);
	////#                   1    2    3
	DelayMs (2);
	
	////#B1 Set Power
	MIPI_SPI_Write(21,0xB1,0x7F,0x10,0x10,0xD2,0x32,0x80,0x10,0xF0,0x56,0x80,0x20,0x20,0xF8,0xAA,0xAA,0xA1,0x00,0x80,0x30,0x00);
	////#                    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20
	////# 1->AP,2->VSPR,3->VSNR,4->VSP,5->VSN,8->VGH,9->VGL
	DelayMs (2);
	
	////#B2 Set Display 
	MIPI_SPI_Write(11,0xB2,0x82,0x50,0x05,0x07,0xF0,0x38,0x11,0x64,0x5D,0x09);
	////#                    1    2    3    4    5    6    7    8    9   10 
	DelayMs (2);
	
	////#B4 Set Panel Driving
	MIPI_SPI_Write(12,0xB4,0x66,0x66,0x66,0x70,0x00,0x00,0x18,0x76,0x28,0x76,0xA8);
	////#                    1    2    3    4    5    6    7    8    9   10   11   
	DelayMs (2);
	
	////#BF Set Power Option
	MIPI_SPI_Write(5,0xBF,0x05,0x50,0x00,0x3E);
	////#                   1    2    3    4
	DelayMs (2);
	
	////#D3 Set GIP2 
	MIPI_SPI_Write(36,0xD3,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x32,0x10,0x00,0x00,0x00,0x03,0xC6,0x03,0xC6,0x00,0x00,0x00,0x00,0x35,0x33,0x04,0x04,0x37,0x00,0x00,0x00,0x05,0x08,0x00,0x00,0x0A,0x00,0x01);
	////#                    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35
	DelayMs (2);
	
	////#D5 Set GIP3 (Forward)
	MIPI_SPI_Write(39,0xD5,0x18,0x18,0x18,0x18,0x19,0x19,0x18,0x18,0x20,0x21,0x24,0x25,0x18,0x18,0x18,0x18,0x00,0x01,0x04,0x05,0x02,0x03,0x06,0x07,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18);
	////#                   1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17  18    19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38    
	DelayMs (2);
	
	////#D6 Set GIP3 (Backward)
	MIPI_SPI_Write(39,0xD6,0x18,0x18,0x18,0x18,0x18,0x18,0x19,0x19,0x25,0x24,0x21,0x20,0x18,0x18,0x18,0x18,0x07,0x06,0x03,0x02,0x05,0x04,0x01,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18);
	////#                    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38    
	DelayMs (2);
	
	////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#                
	////#B6 Set VCOM Voltage
//	MIPI_SPI_Write(3,0xB6,0x70,0x70);
//	MIPI_SPI_Write(4 ,0xB6,VCOMDC1,VCOMDC1,0x00);
	////#                       1    2    
	////#                      FW   BW
	////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#////#
	DelayMs (2);
	
	////#CC Set Panel Scan Direction
	MIPI_SPI_Write(2,0xCC,0x0E);    ////#FORWARD=02,BACKWARD=0E
	////#                       1
	DelayMs (2);
	
	////#D2 Set Power
	MIPI_SPI_Write(2,0xD2,0x33);
	////#                       1
	DelayMs(2);
	
	////#C0 Set GAMMA OP
	MIPI_SPI_Write(5,0xC0,0x30,0x17,0x00,0x03);
	////#                       1    2    3    4
	DelayMs (2);
	
	////#C7 Set TCON Timing
	MIPI_SPI_Write(5,0xC7,0x00,0x80,0x00,0xC0);
	////#                       1    2    3    4
	DelayMs (2);
	
	////#B7 Set Internal TE
	MIPI_SPI_Write(4,0xB7,0x20,0x80,0x00);
	////#                       1    2    3
	DelayMs (2);
	
	////#E0 Set GAMMA
	MIPI_SPI_Write(43,0xE0,0x00,0x02,0x06,0x38,0x3C,0x3F,0x1B,0x46,0x06,0x09,0x0C,0x17,0x10,0x13,0x16,0x13,0x14,0x08,0x13,0x15,0x19,0x00,0x02,0x06,0x37,0x3C,0x3F,0x1A,0x45,0x05,0x09,0x0B,0x16,0x0F,0x13,0x15,0x13,0x14,0x07,0x12,0x14,0x18);
	////#                       1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42
	////#                    P255 P251 P247   P8   P4   P0 P204  P52 P243 P236 P228 P216 P180 P156 P132 P100  P76  P40  P28  P20  P12 N255 N251 N247   N8   N4   N0 N204  N52 N243 N236 N228 N216 N180 N156 N132 N100  N76  N40  N28  N20  N12
	DelayMs (2);

//	//#Sleep out
//	  MIPI_SPI_Write(1,0x11);
//	  DelayMs (200);
	
	//#Display On
	  MIPI_SPI_Write(1,0x29);
	  DelayMs (50);
}



void Forward_scan(void)           //ÕýÉ¨
{
//    #FORWARD=02,BACKWARD=0E
	SSD2828_ENTER_LP_mode();   	 DelayMs(5);
	MIPI_SPI_Write(2,0xCC,0x02);
	SSD2828_VIDEO_MODE_HS();	 DelayMs(5);	
}
void Backward_scan(void)          //·´É¨
{
//    #FORWARD=02,BACKWARD=0E
	SSD2828_ENTER_LP_mode();   	 DelayMs(5);
	MIPI_SPI_Write(2,0xCC,0x0E);
	SSD2828_VIDEO_MODE_HS();	 DelayMs(5);
		
}	
/*******************************************************************************
* Function Name  : void MTP(void)
* Description    : otp -> vcom     //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void MTP(void)
{

//	LCD_RST(1) ;
//	MDelay(50);
//	LCD_RST(0);//////////Ó²¼þ¸´Î»
//	MDelay(50);
//	LCD_RST(1) ;
//	DelayMs(500);  
  	
	MIPI_SPI_Write( 4,0xB9,0xFF,0x83,0x89);  
	DelayMs (10);
	
	MIPI_SPI_Write( 21,0xB1,0x7F,0x10,0x10,0xD2,0x32,0x80,0x10,0xF0,0x56,0x80,0x20,0x20,0xF8,0xAA,0xAA,0xA1,0x00,0x80,0x30,0x00);
	
	MIPI_SPI_Write( 39,0xD5,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18);
//////#                       1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17  18    19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38
	
	MIPI_SPI_Write( 39,0xD6,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18);
	DelayMs (10);

	MIPI_SPI_Write(0x01,0x11);
	DelayMs(200);

//	MIPI_SPI_Write(4 ,0xB6,0x3f,0x3f,0x00); 	 ////set vcom	
	MIPI_SPI_Write(4 ,0xB6,VCOMDC1,VCOMDC1,0x00); 	 ////set vcom		


//	MIPI_SPI_Write(0x04,0xC3,0x53,0x44,0x90);
//	DelayMs(10);			   ////delay 10ms

	
	/////#Set Inertmal power 8.25V to OTP_PWR
	/////#WRITE VCOM 
	MIPI_SPI_Write(0x07,0xBB,0xAA,0x55,0x00,0x0D,0X00,0x10);
	DelayMs(200);	
	
	MIPI_SPI_Write(0x07,0xBB,0xAA,0x55,0x00,0x0D,0x00,0x11);
	DelayMs(100);	

	MIPI_SPI_Write(0x07,0xBB,0xAA,0x55,0x00,0x0D,0x00,0x00);
	DelayMs(400);							  	 


}

/*******************************************************************************
* Function Name  : void MTP_ID(void)
* Description    : otp -> ID       //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  

void MTP_ID(void)
{
//	LCD_RST(1) ;
//	MDelay(50);
//	LCD_RST(0);//////////Ó²¼þ¸´Î»
//	MDelay(50);
//	LCD_RST(1) ;
//	DelayMs(500);  
  	
	MIPI_SPI_Write( 4,0xB9,0xFF,0x83,0x89);  
	DelayMs (10);
	
	MIPI_SPI_Write( 21,0xB1,0x7F,0x10,0x10,0xD2,0x32,0x80,0x10,0xF0,0x56,0x80,0x20,0x20,0xF8,0xAA,0xAA,0xA1,0x00,0x80,0x30,0x00);
	
	MIPI_SPI_Write( 39,0xD5,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18);
//////#                       1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17  18    19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38
	
	MIPI_SPI_Write( 39,0xD6,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18);
	DelayMs (10);

	MIPI_SPI_Write(0x01,0x11);
	DelayMs(200);


	MIPI_SPI_Write(0x04,0xC3,0x53,0x44,0x90);
	DelayMs(10);			   ////delay 10ms

	
	/////#Set Inertmal power 8.25V to OTP_PWR
	/////#WRITE VCOM 
	MIPI_SPI_Write(0x07,0xBB,0xAA,0x55,0x00,0x00,0X00,0x10);
	DelayMs(200);	
	
	MIPI_SPI_Write(0x07,0xBB,0xAA,0x55,0x00,0x00,0x00,0x11);
	DelayMs(100);	

	MIPI_SPI_Write(0x07,0xBB,0xAA,0x55,0x00,0x00,0x00,0x00);
	DelayMs(400);		
}


#endif

#ifdef HXD800x1280_080WQ18_JD9366
//////mipi video mode setting
u16 value_HighSpeed = 580;      ///520
u16 value_Lane_select = 4;
   
u16  VDP= 1280;      
u16  VBP= 18;    //25   3     16    12
u16  VFP= 8;    //35   8     18    26   12

u16  HDP= 800;     
u16  HBP= 80;     //	60   59    56      45
u16  HFP= 40;     //	80   16    58       46

u16	 HPW=40;  //Í¨³£²»ÐèÒªµ÷Õû  5           20
u16	 VPW=4;  //Í¨³£²»ÐèÒªµ÷Õû  5            4
/////////////080WQ USE    inition/////
u8 PIC_NUM=13;       //ëŠœy®‹Ãæ×ÜÊý
u8 Flicker_OTP=1;    //OTP flicker ®‹Ãæ¾ŽÌ–
u8 Flicker_OQC=1;    //QC flicker  ®‹Ãæ¾ŽÌ–
//-------------------------------------------------------------------
//vcom½×´ÎÉè¶¨·½Ê½
void VCOM_set(u8 vcom)
{
    
    MIPI_SPI_Write( 2, 0xE0,0x01); 
    MIPI_SPI_Write( 2, 0x01,0X55);
    MIPI_SPI_Write( 2, 0xE0,0x00); 
    DelayMs( 20);
    
     MIPI_SPI_Write( 2, 0xE0,0x01); 
    
    MIPI_SPI_Write( 2, 0x00,0x00); 
    MIPI_SPI_Write( 2, 0x01,vcom);

}   
//IDÖµÉè¶¨·½Ê½
void ID_set(void)
{

}		
void SSD2828_inition_lcd(void)    //IC³õÊ¼»¯
{
//#=========================================================
//#  Main LDI Power ON Sequence
//#=========================================================

MIPI_SPI_Write( 2, 0xe0,0x00);

//#//--- password  ----//
MIPI_SPI_Write( 2, 0xe1,0x93);
MIPI_SPI_Write( 2, 0xe2,0x65);
MIPI_SPI_Write( 2, 0xe3,0xf8);
MIPI_SPI_Write( 2, 0x80,0x03);  //#for ESD

//#//--- page1  ----//
MIPI_SPI_Write( 2, 0xe0,0x01);

//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#
//#//set vcom
//MIPI_SPI_Write( 2, 0x00,0x00);
//MIPI_SPI_Write( 2, 0x01,0xb7);   //#//0xb5(VCOM??)
//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#

//#mipi.write 0x37,0x08 //#(????????) 
//#MIPI_SPI_Write( 2, 0x00,0x00  //#(IC register shift) 
//#mipi.write 0x06,0x01  
//#mipi.read 

//#//set gamma power, vgmp,vgmn,vgsp,vgsn
MIPI_SPI_Write( 2, 0x17,0x00);
MIPI_SPI_Write( 2, 0x18,0xcf);    //#source 4.7v
MIPI_SPI_Write( 2, 0x19,0x01);
MIPI_SPI_Write( 2, 0x1a,0x00);
MIPI_SPI_Write( 2, 0x1b,0xcf);  
MIPI_SPI_Write( 2, 0x1c,0x01);

//#//set gate power
MIPI_SPI_Write( 2, 0x1f,0x3e);    //#VGH
MIPI_SPI_Write( 2, 0x20,0x28);    //#VGL
MIPI_SPI_Write( 2, 0x21,0x28);
MIPI_SPI_Write( 2, 0x22,0x0e);

MIPI_SPI_Write( 2, 0x24,0x08);  //#//0xC8, [7]:vgh_en=0,[6]:vgl_en=0,[5]:avdd_en=0,[4]:avee_en=0


//#//set rgbcyc
MIPI_SPI_Write( 2, 0x37,0x29);	//#//[5:4]enz[1:0]=10, [3]ss=1, [0]bgr=1
MIPI_SPI_Write( 2, 0x38,0x05);	//#//jdt=101 zigzag inversion
MIPI_SPI_Write( 2, 0x39,0x08);	//#//rgb_n_eq1, modify 20140806
MIPI_SPI_Write( 2, 0x3a,0x12);	//#//rgb_n_eq2, modify 20140806
MIPI_SPI_Write( 2, 0x3c,0x78);	//#//set eq3 for te_h
MIPI_SPI_Write( 2, 0x3d,0xff);	//#//set chgen_on, modify 20140827 
MIPI_SPI_Write( 2, 0x3e,0xff);	//#//set chgen_off, modify 20140827 
MIPI_SPI_Write( 2, 0x3f,0xff);	//#//set chgen_off2, modify 20140827




//#//set tcon
MIPI_SPI_Write( 2, 0x40,0x06);	//#//rso=800 pixels
MIPI_SPI_Write( 2, 0x41,0xa0);	//#//ln=640->1280 line
MIPI_SPI_Write( 2, 0x43,0x15);	//#//vfp  21
MIPI_SPI_Write( 2, 0x44,0x12);	//#//vbp   18
MIPI_SPI_Write( 2, 0x45,0x50);	//#//hbp   80
MIPI_SPI_Write( 2, 0x4b,0x04);

//#//--- power voltage  ----//
MIPI_SPI_Write( 2, 0x55,0x0f);	//#//dcdcm=1111, external pwoer ic
MIPI_SPI_Write( 2, 0x56,0x01);
MIPI_SPI_Write( 2, 0x57,0x89);
MIPI_SPI_Write( 2, 0x58,0x0a);
MIPI_SPI_Write( 2, 0x59,0x2a);	//#//vcl = -2.9v
MIPI_SPI_Write( 2, 0x5a,0x31);	//#//vgh = 19v
MIPI_SPI_Write( 2, 0x5b,0x15);	//#//vgl = -11v

//#//--- gamma ++  ----//
MIPI_SPI_Write( 2, 0x5d,0x7c);
MIPI_SPI_Write( 2, 0x5e,0x50);
MIPI_SPI_Write( 2, 0x5f,0x3b);
MIPI_SPI_Write( 2, 0x60,0x2b);
MIPI_SPI_Write( 2, 0x61,0x25);
MIPI_SPI_Write( 2, 0x62,0x15);
MIPI_SPI_Write( 2, 0x63,0x1a);
MIPI_SPI_Write( 2, 0x64,0x04);
MIPI_SPI_Write( 2, 0x65,0x1c);
MIPI_SPI_Write( 2, 0x66,0x1a);
MIPI_SPI_Write( 2, 0x67,0x19);
MIPI_SPI_Write( 2, 0x68,0x36);
MIPI_SPI_Write( 2, 0x69,0x27);
MIPI_SPI_Write( 2, 0x6a,0x2f);
MIPI_SPI_Write( 2, 0x6b,0x23);
MIPI_SPI_Write( 2, 0x6c,0x21);
MIPI_SPI_Write( 2, 0x6d,0x17);
MIPI_SPI_Write( 2, 0x6e,0x05);
MIPI_SPI_Write( 2, 0x6f,0x00);

//#//--- gamma --  ----//
MIPI_SPI_Write( 2, 0x70,0x7c);
MIPI_SPI_Write( 2, 0x71,0x50);
MIPI_SPI_Write( 2, 0x72,0x3b);
MIPI_SPI_Write( 2, 0x73,0x2b);
MIPI_SPI_Write( 2, 0x74,0x25);
MIPI_SPI_Write( 2, 0x75,0x15);
MIPI_SPI_Write( 2, 0x76,0x1a);
MIPI_SPI_Write( 2, 0x77,0x04);
MIPI_SPI_Write( 2, 0x78,0x1c);
MIPI_SPI_Write( 2, 0x79,0x1a);
MIPI_SPI_Write( 2, 0x7a,0x19);
MIPI_SPI_Write( 2, 0x7b,0x36);
MIPI_SPI_Write( 2, 0x7c,0x27);
MIPI_SPI_Write( 2, 0x7d,0x2f);
MIPI_SPI_Write( 2, 0x7e,0x23);
MIPI_SPI_Write( 2, 0x7f,0x21);
MIPI_SPI_Write( 2, 0x80,0x17);
MIPI_SPI_Write( 2, 0x81,0x05);
MIPI_SPI_Write( 2, 0x82,0x00);


//#//page2, for gip
MIPI_SPI_Write( 2, 0xe0,0x02);

//#//gip_l pin mapping
MIPI_SPI_Write( 2, 0x00,0x00);
MIPI_SPI_Write( 2, 0x01,0x04);   
MIPI_SPI_Write( 2, 0x02,0x08); 
MIPI_SPI_Write( 2, 0x03,0x05); 
MIPI_SPI_Write( 2, 0x04,0x09); 
MIPI_SPI_Write( 2, 0x05,0x06); 
MIPI_SPI_Write( 2, 0x06,0x0a); 
MIPI_SPI_Write( 2, 0x07,0x07); 
MIPI_SPI_Write( 2, 0x08,0x0b); 
MIPI_SPI_Write( 2, 0x09,0x1f); 
MIPI_SPI_Write( 2, 0x0a,0x1f); 
MIPI_SPI_Write( 2, 0x0b,0x1f); 
MIPI_SPI_Write( 2, 0x0c,0x1f); 
MIPI_SPI_Write( 2, 0x0d,0x1f); 
MIPI_SPI_Write( 2, 0x0e,0x1f); 
MIPI_SPI_Write( 2, 0x0f,0x17); 
MIPI_SPI_Write( 2, 0x10,0x37); 
MIPI_SPI_Write( 2, 0x11,0x10); 
MIPI_SPI_Write( 2, 0x12,0x1f); 
MIPI_SPI_Write( 2, 0x13,0x1f); 
MIPI_SPI_Write( 2, 0x14,0x1f); 
MIPI_SPI_Write( 2, 0x15,0x1f); 

//#//gip_r pin mapping
MIPI_SPI_Write( 2, 0x16,0x00);
MIPI_SPI_Write( 2, 0x17,0x04);   
MIPI_SPI_Write( 2, 0x18,0x08); 
MIPI_SPI_Write( 2, 0x19,0x05); 
MIPI_SPI_Write( 2, 0x1a,0x09); 
MIPI_SPI_Write( 2, 0x1b,0x06); 
MIPI_SPI_Write( 2, 0x1c,0x0a); 
MIPI_SPI_Write( 2, 0x1d,0x07); 
MIPI_SPI_Write( 2, 0x1e,0x0b); 
MIPI_SPI_Write( 2, 0x1f,0x1f); 
MIPI_SPI_Write( 2, 0x20,0x1f); 
MIPI_SPI_Write( 2, 0x21,0x1f); 
MIPI_SPI_Write( 2, 0x22,0x1f); 
MIPI_SPI_Write( 2, 0x23,0x1f); 
MIPI_SPI_Write( 2, 0x24,0x1f); 
MIPI_SPI_Write( 2, 0x25,0x17); 
MIPI_SPI_Write( 2, 0x26,0x37); 
MIPI_SPI_Write( 2, 0x27,0x10); 
MIPI_SPI_Write( 2, 0x28,0x1f); 
MIPI_SPI_Write( 2, 0x29,0x1f); 
MIPI_SPI_Write( 2, 0x2a,0x1f); 
MIPI_SPI_Write( 2, 0x2b,0x1f); 

//#//gip timing  
MIPI_SPI_Write( 2, 0x58,0x01); 
MIPI_SPI_Write( 2, 0x59,0x00); 
MIPI_SPI_Write( 2, 0x5a,0x00); 
MIPI_SPI_Write( 2, 0x5b,0x00); 
MIPI_SPI_Write( 2, 0x5c,0x0C); //#//stv_s0
MIPI_SPI_Write( 2, 0x5d,0x60); 
MIPI_SPI_Write( 2, 0x5e,0x00); 
MIPI_SPI_Write( 2, 0x5f,0x00); 
MIPI_SPI_Write( 2, 0x60,0x30); 
MIPI_SPI_Write( 2, 0x61,0x00); 
MIPI_SPI_Write( 2, 0x62,0x00); 
MIPI_SPI_Write( 2, 0x63,0x03); //#//stv_on
MIPI_SPI_Write( 2, 0x64,0x6a); //#//stv_off
MIPI_SPI_Write( 2, 0x65,0x45); 
MIPI_SPI_Write( 2, 0x66,0x14); //#// for G1280 1 line delay with RST
MIPI_SPI_Write( 2, 0x67,0x73); 
MIPI_SPI_Write( 2, 0x68,0x10); //#//ckv_s0
MIPI_SPI_Write( 2, 0x69,0x06); //#//ckv_on
MIPI_SPI_Write( 2, 0x6a,0x6a); //#//ckv_off
MIPI_SPI_Write( 2, 0x6b,0x00); 
MIPI_SPI_Write( 2, 0x6c,0x00); 
MIPI_SPI_Write( 2, 0x6d,0x03); 
MIPI_SPI_Write( 2, 0x6e,0x00); 
MIPI_SPI_Write( 2, 0x6f,0x08); 
MIPI_SPI_Write( 2, 0x70,0x00); 
MIPI_SPI_Write( 2, 0x71,0x00); 
MIPI_SPI_Write( 2, 0x72,0x06); 
MIPI_SPI_Write( 2, 0x73,0x7b); 
MIPI_SPI_Write( 2, 0x74,0x00); 
MIPI_SPI_Write( 2, 0x75,0x80); 
MIPI_SPI_Write( 2, 0x76,0x00); 
MIPI_SPI_Write( 2, 0x77,0x05); 
MIPI_SPI_Write( 2, 0x78,0x1b); //#//flm_off
MIPI_SPI_Write( 2, 0x79,0x00); 
MIPI_SPI_Write( 2, 0x7a,0x00); 
MIPI_SPI_Write( 2, 0x7b,0x00); 
MIPI_SPI_Write( 2, 0x7c,0x00); 
MIPI_SPI_Write( 2, 0x7d,0x03); 
MIPI_SPI_Write( 2, 0x7e,0x7b); 

//#//page3
MIPI_SPI_Write( 2, 0xe0,0x03);
MIPI_SPI_Write( 2, 0x98,0x3e);	//#//from 2e to 3e, led on

//#//page4  for ESD
MIPI_SPI_Write( 2, 0xe0,0x04);
MIPI_SPI_Write( 2, 0x09,0x10);  //#//sweep blanking source level=gnd
MIPI_SPI_Write( 2, 0x2b,0x2b);
MIPI_SPI_Write( 2, 0x2e,0x44);	

//#Page0
MIPI_SPI_Write( 2, 0xE0,0x00);
MIPI_SPI_Write( 2, 0xE6,0x02);  //#for ESD
MIPI_SPI_Write( 2, 0xE7,0x02);  //#for ESD
DelayMs	( 5);
                           
//#SLP OUT                   
MIPI_SPI_Write( 1, 0x11);      
DelayMs( 120);                 

//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#                           
//MIPI_SPI_Write( 2, 0xE0,0x01); 
//MIPI_SPI_Write( 2, 0x01,0xC5); //#???????VCOM 
//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#Set__//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#

//MIPI_SPI_Write( 2, 0xE0,0x00); 

//#DISP ON
MIPI_SPI_Write( 1, 0x29);  
DelayMs	( 5);

//#--- TE----//#
MIPI_SPI_Write( 2, 0x35,0x00);
//#MIPI_SPI_Write( 2, 0xB6,0x32,0x32);

}
void SSD2828_inition_lcd_Check(void)    //IC³õÊ¼»¯
{
//#=========================================================
//#  Main LDI Power ON Sequence
//#=========================================================

MIPI_SPI_Write( 2, 0xe0,0x00);

//#//--- password  ----//
MIPI_SPI_Write( 2, 0xe1,0x93);
MIPI_SPI_Write( 2, 0xe2,0x65);
MIPI_SPI_Write( 2, 0xe3,0xf8);
MIPI_SPI_Write( 2, 0x80,0x03);  //#for ESD

//#//--- page1  ----//
MIPI_SPI_Write( 2, 0xe0,0x01);

//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#
//#//set vcom
//MIPI_SPI_Write( 2, 0x00,0x00);
//MIPI_SPI_Write( 2, 0x01,0xb7);   //#//0xb5(VCOM??)
//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#

//#mipi.write 0x37,0x08 //#(????????) 
//#MIPI_SPI_Write( 2, 0x00,0x00  //#(IC register shift) 
//#mipi.write 0x06,0x01  
//#mipi.read 

//#//set gamma power, vgmp,vgmn,vgsp,vgsn
MIPI_SPI_Write( 2, 0x17,0x00);
MIPI_SPI_Write( 2, 0x18,0xcf);    //#source 4.7v
MIPI_SPI_Write( 2, 0x19,0x01);
MIPI_SPI_Write( 2, 0x1a,0x00);
MIPI_SPI_Write( 2, 0x1b,0xcf);  
MIPI_SPI_Write( 2, 0x1c,0x01);

//#//set gate power
MIPI_SPI_Write( 2, 0x1f,0x3e);    //#VGH
MIPI_SPI_Write( 2, 0x20,0x28);    //#VGL
MIPI_SPI_Write( 2, 0x21,0x28);
MIPI_SPI_Write( 2, 0x22,0x0e);

MIPI_SPI_Write( 2, 0x24,0x08);  //#//0xC8, [7]:vgh_en=0,[6]:vgl_en=0,[5]:avdd_en=0,[4]:avee_en=0


//#//set rgbcyc
MIPI_SPI_Write( 2, 0x37,0x29);	//#//[5:4]enz[1:0]=10, [3]ss=1, [0]bgr=1
MIPI_SPI_Write( 2, 0x38,0x05);	//#//jdt=101 zigzag inversion
MIPI_SPI_Write( 2, 0x39,0x08);	//#//rgb_n_eq1, modify 20140806
MIPI_SPI_Write( 2, 0x3a,0x12);	//#//rgb_n_eq2, modify 20140806
MIPI_SPI_Write( 2, 0x3c,0x78);	//#//set eq3 for te_h
MIPI_SPI_Write( 2, 0x3d,0xff);	//#//set chgen_on, modify 20140827 
MIPI_SPI_Write( 2, 0x3e,0xff);	//#//set chgen_off, modify 20140827 
MIPI_SPI_Write( 2, 0x3f,0xff);	//#//set chgen_off2, modify 20140827




//#//set tcon
MIPI_SPI_Write( 2, 0x40,0x06);	//#//rso=800 pixels
MIPI_SPI_Write( 2, 0x41,0xa0);	//#//ln=640->1280 line
MIPI_SPI_Write( 2, 0x43,0x15);	//#//vfp   21
MIPI_SPI_Write( 2, 0x44,0x12);	//#//vbp    18
MIPI_SPI_Write( 2, 0x45,0x50);	//#//hbp    80
MIPI_SPI_Write( 2, 0x4b,0x04);

//#//--- power voltage  ----//
MIPI_SPI_Write( 2, 0x55,0x0f);	//#//dcdcm=1111, external pwoer ic
MIPI_SPI_Write( 2, 0x56,0x01);
MIPI_SPI_Write( 2, 0x57,0x89);
MIPI_SPI_Write( 2, 0x58,0x0a);
MIPI_SPI_Write( 2, 0x59,0x2a);	//#//vcl = -2.9v
MIPI_SPI_Write( 2, 0x5a,0x31);	//#//vgh = 19v
MIPI_SPI_Write( 2, 0x5b,0x15);	//#//vgl = -11v

//#//--- gamma ++  ----//
MIPI_SPI_Write( 2, 0x5d,0x7c);
MIPI_SPI_Write( 2, 0x5e,0x50);
MIPI_SPI_Write( 2, 0x5f,0x3b);
MIPI_SPI_Write( 2, 0x60,0x2b);
MIPI_SPI_Write( 2, 0x61,0x25);
MIPI_SPI_Write( 2, 0x62,0x15);
MIPI_SPI_Write( 2, 0x63,0x1a);
MIPI_SPI_Write( 2, 0x64,0x04);
MIPI_SPI_Write( 2, 0x65,0x1c);
MIPI_SPI_Write( 2, 0x66,0x1a);
MIPI_SPI_Write( 2, 0x67,0x19);
MIPI_SPI_Write( 2, 0x68,0x36);
MIPI_SPI_Write( 2, 0x69,0x27);
MIPI_SPI_Write( 2, 0x6a,0x2f);
MIPI_SPI_Write( 2, 0x6b,0x23);
MIPI_SPI_Write( 2, 0x6c,0x21);
MIPI_SPI_Write( 2, 0x6d,0x17);
MIPI_SPI_Write( 2, 0x6e,0x05);
MIPI_SPI_Write( 2, 0x6f,0x00);

//#//--- gamma --  ----//
MIPI_SPI_Write( 2, 0x70,0x7c);
MIPI_SPI_Write( 2, 0x71,0x50);
MIPI_SPI_Write( 2, 0x72,0x3b);
MIPI_SPI_Write( 2, 0x73,0x2b);
MIPI_SPI_Write( 2, 0x74,0x25);
MIPI_SPI_Write( 2, 0x75,0x15);
MIPI_SPI_Write( 2, 0x76,0x1a);
MIPI_SPI_Write( 2, 0x77,0x04);
MIPI_SPI_Write( 2, 0x78,0x1c);
MIPI_SPI_Write( 2, 0x79,0x1a);
MIPI_SPI_Write( 2, 0x7a,0x19);
MIPI_SPI_Write( 2, 0x7b,0x36);
MIPI_SPI_Write( 2, 0x7c,0x27);
MIPI_SPI_Write( 2, 0x7d,0x2f);
MIPI_SPI_Write( 2, 0x7e,0x23);
MIPI_SPI_Write( 2, 0x7f,0x21);
MIPI_SPI_Write( 2, 0x80,0x17);
MIPI_SPI_Write( 2, 0x81,0x05);
MIPI_SPI_Write( 2, 0x82,0x00);


//#//page2, for gip
MIPI_SPI_Write( 2, 0xe0,0x02);

//#//gip_l pin mapping
MIPI_SPI_Write( 2, 0x00,0x00);
MIPI_SPI_Write( 2, 0x01,0x04);   
MIPI_SPI_Write( 2, 0x02,0x08); 
MIPI_SPI_Write( 2, 0x03,0x05); 
MIPI_SPI_Write( 2, 0x04,0x09); 
MIPI_SPI_Write( 2, 0x05,0x06); 
MIPI_SPI_Write( 2, 0x06,0x0a); 
MIPI_SPI_Write( 2, 0x07,0x07); 
MIPI_SPI_Write( 2, 0x08,0x0b); 
MIPI_SPI_Write( 2, 0x09,0x1f); 
MIPI_SPI_Write( 2, 0x0a,0x1f); 
MIPI_SPI_Write( 2, 0x0b,0x1f); 
MIPI_SPI_Write( 2, 0x0c,0x1f); 
MIPI_SPI_Write( 2, 0x0d,0x1f); 
MIPI_SPI_Write( 2, 0x0e,0x1f); 
MIPI_SPI_Write( 2, 0x0f,0x17); 
MIPI_SPI_Write( 2, 0x10,0x37); 
MIPI_SPI_Write( 2, 0x11,0x10); 
MIPI_SPI_Write( 2, 0x12,0x1f); 
MIPI_SPI_Write( 2, 0x13,0x1f); 
MIPI_SPI_Write( 2, 0x14,0x1f); 
MIPI_SPI_Write( 2, 0x15,0x1f); 

//#//gip_r pin mapping
MIPI_SPI_Write( 2, 0x16,0x00);
MIPI_SPI_Write( 2, 0x17,0x04);   
MIPI_SPI_Write( 2, 0x18,0x08); 
MIPI_SPI_Write( 2, 0x19,0x05); 
MIPI_SPI_Write( 2, 0x1a,0x09); 
MIPI_SPI_Write( 2, 0x1b,0x06); 
MIPI_SPI_Write( 2, 0x1c,0x0a); 
MIPI_SPI_Write( 2, 0x1d,0x07); 
MIPI_SPI_Write( 2, 0x1e,0x0b); 
MIPI_SPI_Write( 2, 0x1f,0x1f); 
MIPI_SPI_Write( 2, 0x20,0x1f); 
MIPI_SPI_Write( 2, 0x21,0x1f); 
MIPI_SPI_Write( 2, 0x22,0x1f); 
MIPI_SPI_Write( 2, 0x23,0x1f); 
MIPI_SPI_Write( 2, 0x24,0x1f); 
MIPI_SPI_Write( 2, 0x25,0x17); 
MIPI_SPI_Write( 2, 0x26,0x37); 
MIPI_SPI_Write( 2, 0x27,0x10); 
MIPI_SPI_Write( 2, 0x28,0x1f); 
MIPI_SPI_Write( 2, 0x29,0x1f); 
MIPI_SPI_Write( 2, 0x2a,0x1f); 
MIPI_SPI_Write( 2, 0x2b,0x1f); 

//#//gip timing  
MIPI_SPI_Write( 2, 0x58,0x01); 
MIPI_SPI_Write( 2, 0x59,0x00); 
MIPI_SPI_Write( 2, 0x5a,0x00); 
MIPI_SPI_Write( 2, 0x5b,0x00); 
MIPI_SPI_Write( 2, 0x5c,0x0C); //#//stv_s0
MIPI_SPI_Write( 2, 0x5d,0x60); 
MIPI_SPI_Write( 2, 0x5e,0x00); 
MIPI_SPI_Write( 2, 0x5f,0x00); 
MIPI_SPI_Write( 2, 0x60,0x30); 
MIPI_SPI_Write( 2, 0x61,0x00); 
MIPI_SPI_Write( 2, 0x62,0x00); 
MIPI_SPI_Write( 2, 0x63,0x03); //#//stv_on
MIPI_SPI_Write( 2, 0x64,0x6a); //#//stv_off
MIPI_SPI_Write( 2, 0x65,0x45); 
MIPI_SPI_Write( 2, 0x66,0x14); //#// for G1280 1 line delay with RST
MIPI_SPI_Write( 2, 0x67,0x73); 
MIPI_SPI_Write( 2, 0x68,0x10); //#//ckv_s0
MIPI_SPI_Write( 2, 0x69,0x06); //#//ckv_on
MIPI_SPI_Write( 2, 0x6a,0x6a); //#//ckv_off
MIPI_SPI_Write( 2, 0x6b,0x00); 
MIPI_SPI_Write( 2, 0x6c,0x00); 
MIPI_SPI_Write( 2, 0x6d,0x03); 
MIPI_SPI_Write( 2, 0x6e,0x00); 
MIPI_SPI_Write( 2, 0x6f,0x08); 
MIPI_SPI_Write( 2, 0x70,0x00); 
MIPI_SPI_Write( 2, 0x71,0x00); 
MIPI_SPI_Write( 2, 0x72,0x06); 
MIPI_SPI_Write( 2, 0x73,0x7b); 
MIPI_SPI_Write( 2, 0x74,0x00); 
MIPI_SPI_Write( 2, 0x75,0x80); 
MIPI_SPI_Write( 2, 0x76,0x00); 
MIPI_SPI_Write( 2, 0x77,0x05); 
MIPI_SPI_Write( 2, 0x78,0x1b); //#//flm_off
MIPI_SPI_Write( 2, 0x79,0x00); 
MIPI_SPI_Write( 2, 0x7a,0x00); 
MIPI_SPI_Write( 2, 0x7b,0x00); 
MIPI_SPI_Write( 2, 0x7c,0x00); 
MIPI_SPI_Write( 2, 0x7d,0x03); 
MIPI_SPI_Write( 2, 0x7e,0x7b); 

//#//page3
MIPI_SPI_Write( 2, 0xe0,0x03);
MIPI_SPI_Write( 2, 0x98,0x3e);	//#//from 2e to 3e, led on

//#//page4  for ESD
MIPI_SPI_Write( 2, 0xe0,0x04);
MIPI_SPI_Write( 2, 0x09,0x10);  //#//sweep blanking source level=gnd
MIPI_SPI_Write( 2, 0x2b,0x2b);
MIPI_SPI_Write( 2, 0x2e,0x44);	

//#Page0
MIPI_SPI_Write( 2, 0xE0,0x00);
MIPI_SPI_Write( 2, 0xE6,0x02);  //#for ESD
MIPI_SPI_Write( 2, 0xE7,0x02);  //#for ESD

                           
//#SLP OUT                   
MIPI_SPI_Write( 1, 0x11);      
DelayMs( 120);                 

//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#                           
//MIPI_SPI_Write( 2, 0xE0,0x01); 
//MIPI_SPI_Write( 2, 0x01,0xC5); //#???????VCOM 
//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#Set__//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#

//MIPI_SPI_Write( 2, 0xE0,0x00); 

//#DISP ON
MIPI_SPI_Write( 1, 0x29);  
DelayMs	( 5);

//#--- TE----//#
MIPI_SPI_Write( 2, 0x35,0x00);
//#MIPI_SPI_Write( 2, 0xB6,0x32,0x32);
	
	
}
void Forward_scan(void)           //ÕýÉ¨
{;}
void Backward_scan(void)          //·´É¨
{;}	
/*******************************************************************************
* Function Name  : void MTP(void)
* Description    : otp -> vcom     //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void MTP(void)
{	

//#====================================================================
//#OTP internal power Flow
//#====================================================================
MIPI_SPI_Write( 2, 0xE0,0x00);        
MIPI_SPI_Write( 2, 0xE1,0x93);
MIPI_SPI_Write( 2, 0xE2,0x65);
MIPI_SPI_Write( 2, 0xE3,0xF8);

MIPI_SPI_Write( 2, 0xE5,0x00);
MIPI_SPI_Write( 1, 0x28); 
DelayMs( 10);    
MIPI_SPI_Write( 1, 0x10);             
DelayMs( 140);

MIPI_SPI_Write( 2, 0xE0,0x01);        
MIPI_SPI_Write( 2, 0x1D,0x01);
MIPI_SPI_Write( 2, 0x1E,0x03);
MIPI_SPI_Write( 2, 0xEC,0x02);
MIPI_SPI_Write( 2, 0xE0,0x00);        
MIPI_SPI_Write( 1, 0x11);             
DelayMs( 14);

MIPI_SPI_Write( 2, 0xE0,0x01);        
MIPI_SPI_Write( 2, 0x00,0x00); 

//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#  
MIPI_SPI_Write( 2, 0x01,VCOMDC1);        //#0xb5,??VCOM?
//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#Set__//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#//#

MIPI_SPI_Write( 2, 0xE0,0x00);       
MIPI_SPI_Write( 2, 0x84,0x5A); 
MIPI_SPI_Write( 2, 0x85,0xA5); 
DelayMs( 120);
MIPI_SPI_Write( 2, 0xEA,0x00); 
DelayMs( 120);
MIPI_SPI_Write( 2, 0xEB,0x26);        //#???O?27...
DelayMs( 120);
MIPI_SPI_Write( 2, 0xEC,0x01);        //#external 0x01

DelayMs( 120);
MIPI_SPI_Write( 2, 0xE0,0x00);       
MIPI_SPI_Write( 2, 0x84,0x00); 
MIPI_SPI_Write( 2, 0x85,0x00);

DelayMs( 120	);
	
//	//#===============================================================================
////#  Main LDI Power ON Sequence
////#===============================================================================

//MIPI_SPI_Write( 1, 0x11);                    
//Delay(120);

////#Set Program OTP Address and Data
//MIPI_SPI_Write(4, 0xFF, 0x61, 0x36, 0x01); 

////#Supply Extemal 7.5V to VPP
////gpio.write 0x44
//VOTP_ADJ_SET(775);			
//VOTP_EN(1);
//	
//Delay(380);

////#=============Vcom E0============
//MIPI_SPI_Write( 2, 0xE0, VCOMDC1);        //ÕýÉ¨VCOM
//MIPI_SPI_Write( 2, 0xE1, 0x05);        //·´É¨VCOM
//MIPI_SPI_Write( 2, 0xE2, 0x00);

////#Step 3 : Enable OTP Program Key
//MIPI_SPI_Write( 2, 0xE3, 0x55);
//MIPI_SPI_Write( 2, 0xE4, 0xAA);
//MIPI_SPI_Write( 2, 0xE5, 0x66);
//Delay(50);

////#sleep out
//MIPI_SPI_Write( 1, 0x11);

//DelayMs(20);
////#=========Internal VGH/VGL(4C)========
//MIPI_SPI_Write( 2, 0xE0, 0x00 );  
//MIPI_SPI_Write( 2, 0xE1, 0xF4);
//MIPI_SPI_Write( 2, 0xE2, 0x01);

////#Step 3 : Enable OTP Program Key
//MIPI_SPI_Write( 2, 0xE3, 0x55);
//MIPI_SPI_Write( 2, 0xE4, 0xAA);
//MIPI_SPI_Write( 2, 0xE5, 0x66);

////#sleep out
//MIPI_SPI_Write( 1, 0x11);

//DelayMs(20);

////#======GIP correction CLK Falling=======
//MIPI_SPI_Write( 2, 0xE0, 0x05 );	  
//MIPI_SPI_Write( 2, 0xE1, 0x42);
//MIPI_SPI_Write( 2, 0xE2, 0x01);

////#Step 3 : Enable OTP Program Key
//MIPI_SPI_Write( 2, 0xE3, 0x55);
//MIPI_SPI_Write( 2, 0xE4, 0xAA);
//MIPI_SPI_Write( 2, 0xE5, 0x66);

////#sleep out
//MIPI_SPI_Write( 1, 0x11);

//DelayMs(20);

////#======GIP correction CLK Rising======= 
//MIPI_SPI_Write( 2, 0xE0, 0x1F);   
//MIPI_SPI_Write( 2, 0xE1, 0x3E);
//MIPI_SPI_Write( 2, 0xE2, 0x01);

////#Step 3 : Enable OTP Program Key
//MIPI_SPI_Write( 2, 0xE3, 0x55);
//MIPI_SPI_Write( 2, 0xE4, 0xAA);
//MIPI_SPI_Write( 2, 0xE5, 0x66);

////#sleep out
//MIPI_SPI_Write( 1, 0x11);

//DelayMs(20);

////#=====page7, 17h=1Fh ======
//MIPI_SPI_Write( 2, 0xE0, 0x1F); 
//MIPI_SPI_Write( 2, 0xE1, 0x3F);
//MIPI_SPI_Write( 2, 0xE2, 0x01);

////#Step 3 : Enable OTP Program Key
//MIPI_SPI_Write( 2, 0xE3, 0x55);
//MIPI_SPI_Write( 2, 0xE4, 0xAA);
//MIPI_SPI_Write( 2, 0xE5, 0x66);

////#sleep out
//MIPI_SPI_Write( 1, 0x11);

//DelayMs(20);


////#======GIP correction STV Falling======= 
//MIPI_SPI_Write( 2, 0xE0, 0x05);   
//MIPI_SPI_Write( 2, 0xE1, 0x35);
//MIPI_SPI_Write( 2, 0xE2, 0x01);

////#Step 3 : Enable OTP Program Key
//MIPI_SPI_Write( 2, 0xE3, 0x55);
//MIPI_SPI_Write( 2, 0xE4, 0xAA);
//MIPI_SPI_Write( 2, 0xE5, 0x66);

////#sleep out
//MIPI_SPI_Write( 1, 0x11);

//DelayMs(20);

////#======GIP correction STV Rising=======
//MIPI_SPI_Write( 2, 0xE0, 0x03 );  
//MIPI_SPI_Write( 2, 0xE1, 0x32);
//MIPI_SPI_Write( 2, 0xE2, 0x01);

////#Step 3 : Enable OTP Program Key
//MIPI_SPI_Write( 2, 0xE3, 0x55);
//MIPI_SPI_Write( 2, 0xE4, 0xAA);
//MIPI_SPI_Write( 2, 0xE5, 0x66);

////#sleep out
//MIPI_SPI_Write( 1, 0x11);

//DelayMs(20);

////#======GIP correction RST Falling======
//MIPI_SPI_Write( 2, 0xE0, 0x35 );  
//MIPI_SPI_Write( 2, 0xE1, 0x36);
//MIPI_SPI_Write( 2, 0xE2, 0x01);

////#Step 3 : Enable OTP Program Key
//MIPI_SPI_Write( 2, 0xE3, 0x55);
//MIPI_SPI_Write( 2, 0xE4, 0xAA);
//MIPI_SPI_Write( 2, 0xE5, 0x66);

////#sleep out
//MIPI_SPI_Write( 1, 0x11);

//DelayMs(20);

////#======GIP correction RST Rising======
//MIPI_SPI_Write( 2, 0xE0, 0x1F);   
//MIPI_SPI_Write( 2, 0xE1, 0x33);
//MIPI_SPI_Write( 2, 0xE2, 0x01);

////#Step 3 : Enable OTP Program Key
//MIPI_SPI_Write( 2, 0xE3, 0x55);
//MIPI_SPI_Write( 2, 0xE4, 0xAA);
//MIPI_SPI_Write( 2, 0xE5, 0x66);

////#sleep out
//MIPI_SPI_Write( 1, 0x11);

//DelayMs(20);

////#======GIP STV width ======
//MIPI_SPI_Write( 2, 0xE0, 0xEB);   
//MIPI_SPI_Write( 2, 0xE1, 0x44);
//MIPI_SPI_Write( 2, 0xE2, 0x01);

////#Step 3 : Enable OTP Program Key
//MIPI_SPI_Write( 2, 0xE3, 0x55);
//MIPI_SPI_Write( 2, 0xE4 ,0xAA);
//MIPI_SPI_Write( 2, 0xE5, 0x66);

////#sleep out
//MIPI_SPI_Write( 1, 0x11);

//DelayMs(20);


////#======GIP Rising(18)======
//MIPI_SPI_Write( 2, 0xE0 ,0x05 );  
//MIPI_SPI_Write( 2, 0xE1, 0x40);
//MIPI_SPI_Write( 2, 0xE2, 0x01);

////#Step 3 : Enable OTP Program Key
//MIPI_SPI_Write( 2, 0xE3, 0x55);
//MIPI_SPI_Write( 2, 0xE4, 0xAA);
//MIPI_SPI_Write( 2, 0xE5, 0x66);

////#sleep out
//MIPI_SPI_Write( 1, 0x11);
//DelayMs(20);

////#======GIP Rising(19)======
//MIPI_SPI_Write( 2, 0xE0, 0x00 );  
//MIPI_SPI_Write( 2, 0xE1, 0x41);
//MIPI_SPI_Write( 2, 0xE2, 0x01);

////#Step 3 : Enable OTP Program Key
//MIPI_SPI_Write( 2, 0xE3, 0x55);
//MIPI_SPI_Write( 2, 0xE4 ,0xAA);
//MIPI_SPI_Write( 2, 0xE5, 0x66);

////#sleep out
//MIPI_SPI_Write( 1, 0x11);

//DelayMs(20);
////#Cut off 7.5V to VPP
////gpio.write 0x04
//VOTP_ADJ_SET(330);			  //¹Ø±ÕÍâ²¿OTPµçÑ¹
//VOTP_EN(0);

//DelayMs(100);

////#Reset IC
////gpio.write 0x00		
////MDelay(10);					
////gpio.write 0x04
////MDelay(10);

////#Power off 
////board.power.off vio
////board.power.off vdd
////board.power.off led  	
}

/*******************************************************************************
* Function Name  : void MTP_ID(void)
* Description    : otp -> ID       //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  

void MTP_ID(void)
{	;	}	
#endif



#ifdef HXD800x1280
//////mipi video mode setting
u16 value_HighSpeed = 610;
u16 value_Lane_select = 4;
   
u16  VDP= 1280;      
u16  VBP= 12;    //25   3     16
u16  VFP= 15;    //35   8     18

u16  HDP= 800;     
u16  HBP= 45;     //	60   59    56
u16  HFP= 46;     //	80   16    58

u16	 HPW=5;  //Í¨³£²»ÐèÒªµ÷Õû  5
u16	 VPW=4;  //Í¨³£²»ÐèÒªµ÷Õû  5
/////////////080WQ USE    inition/////
u8 PIC_NUM=10;       //ëŠœy®‹Ãæ×ÜÊý
u8 Flicker_OTP=1;    //OTP flicker ®‹Ãæ¾ŽÌ–
u8 Flicker_OQC=1;    //QC flicker  ®‹Ãæ¾ŽÌ–
//-------------------------------------------------------------------
//vcom½×´ÎÉè¶¨·½Ê½
void VCOM_set(u8 vcom)
{
   MIPI_SPI_Write(4, 0xFF, 0x61, 0x36, 0x01);   //ÇÐ»»ÖÁpage1
	 MIPI_SPI_Write(2, 0x56, 0x00); 
	 MIPI_SPI_Write(2, 0x53, vcom);   //µ±OTPÒÑ¾­ÉÕÈë³É¹¦ºó£¬NVMÎ»»á×Ô¶¯ÖÃ1
}   
//IDÖµÉè¶¨·½Ê½
void ID_set(void)
{

}		
void SSD2828_inition_lcd(void)    //IC³õÊ¼»¯
{
//#Turn On Internal VGH/L Circuit
//#R4Ch_00h: Enable ; R4Ch_08h : Disable
MIPI_SPI_Write(4 , 0xFF, 0x61 ,0x36, 0x08);
MIPI_SPI_Write(2,  0x4C, 0x00);

//#Page1 for POS gamma correction
MIPI_SPI_Write(4, 0xFF, 0x61, 0x36, 0x01);
MIPI_SPI_Write(2,  0xA0, 0x05);
MIPI_SPI_Write(2,  0xA1 ,0x05);
MIPI_SPI_Write(2,  0xA2, 0x06);
MIPI_SPI_Write(2,  0xA3 ,0x0d);
MIPI_SPI_Write(2,  0xA4 ,0x04);
MIPI_SPI_Write(2,  0xA5, 0x07);
MIPI_SPI_Write(2,  0xA6, 0x0f);
MIPI_SPI_Write(2,  0xA7 ,0x14);
MIPI_SPI_Write(2,  0xA8, 0x1F);
MIPI_SPI_Write(2,  0xA9 ,0x28);
MIPI_SPI_Write(2,  0xAA, 0x31);
MIPI_SPI_Write(2,  0xAB ,0x39);
MIPI_SPI_Write(2,  0xAC ,0x39);
MIPI_SPI_Write(2,  0xAD, 0x31);
MIPI_SPI_Write(2,  0xAE, 0x2e);
MIPI_SPI_Write(2,  0xAF ,0x2f);
MIPI_SPI_Write(2,  0xB0 ,0x3c);
//#Page1 for NEG gamma correction
MIPI_SPI_Write(4,  0xFF ,0x61, 0x36, 0x01);
MIPI_SPI_Write(2,  0xC0 ,0x05);
MIPI_SPI_Write(2,  0xC1, 0x05);
MIPI_SPI_Write(2,  0xC2, 0x06);
MIPI_SPI_Write(2,  0xC3, 0x0d);
MIPI_SPI_Write(2,  0xC4, 0x04);
MIPI_SPI_Write(2,  0xC5 ,0x07);
MIPI_SPI_Write(2,  0xC6, 0x0f);
MIPI_SPI_Write(2,  0xC7, 0x14);
MIPI_SPI_Write(2,  0xC8, 0x1f);
MIPI_SPI_Write(2,  0xC9 ,0x28);
MIPI_SPI_Write(2,  0xCA, 0x31);
MIPI_SPI_Write(2,  0xCB ,0x39);
MIPI_SPI_Write(2,  0xCC ,0x39);
MIPI_SPI_Write(2,  0xCD, 0x31);
MIPI_SPI_Write(2,  0xCE, 0x2e);
MIPI_SPI_Write(2,  0xCF, 0x2f);
MIPI_SPI_Write(2,  0xD0, 0x3c);

//#Source_Out Performnace Enhancement
MIPI_SPI_Write(4,  0xFF ,0x61 ,0x36, 0x08); 
MIPI_SPI_Write(2,  0xE9, 0x0B);

MIPI_SPI_Write(4,  0xFF ,0x61, 0x36, 0x06); 
MIPI_SPI_Write(2,  0x72, 0x01);

//#VGH Voltage change to 16.75V
MIPI_SPI_Write(4,  0xFF, 0x61, 0x36, 0x08); 
MIPI_SPI_Write(2,  0x93, 0x08);

//#VGL Voltage change to -11.16V
MIPI_SPI_Write(2,  0x8E, 0x12);

//#VGH pump ratio
MIPI_SPI_Write(4,  0xFF, 0x61, 0x36, 0x08); 
MIPI_SPI_Write(2,  0x76, 0xB4);

//#VGL pump ratio
MIPI_SPI_Write(2,  0x78 ,0x02);

//# VGH / VGL pump CLK
MIPI_SPI_Write(4,  0xFF, 0x61, 0x36, 0x01); 
MIPI_SPI_Write(2,  0x42, 0x43); 

//#sdt correction
MIPI_SPI_Write(4,  0xFF, 0x61 ,0x36, 0x01); 
MIPI_SPI_Write(2,  0x60 ,0x14);

//#GIP correction CLK Falling
MIPI_SPI_Write(4,  0xFF ,0x61, 0x36, 0x07); 
MIPI_SPI_Write(2 , 0x1A, 0x05);

//#GIP correction CLK Rising
MIPI_SPI_Write(4,  0xFF ,0x61 ,0x36, 0x07); 
MIPI_SPI_Write(2,  0x16, 0x1F);

//#GIP correction CLK Rising
MIPI_SPI_Write(4,  0xFF, 0x61, 0x36 ,0x07); 
MIPI_SPI_Write(2,  0x17, 0x1F);

//#GIP Rising 
MIPI_SPI_Write(4 , 0xFF ,0x61 ,0x36, 0x07); 
MIPI_SPI_Write(2, 0x18, 0x05);

//#GIP falling 
MIPI_SPI_Write(4 , 0xFF, 0x61 ,0x36, 0x07 );
MIPI_SPI_Write(2,  0x19 ,0x00);

//#GIP correction STV Falling
MIPI_SPI_Write(4,  0xFF, 0x61 ,0x36 ,0x07 );
MIPI_SPI_Write(2 , 0x0D ,0x05);

//#GIP correction STV Rising
MIPI_SPI_Write(4,  0xFF, 0x61, 0x36, 0x07 );
MIPI_SPI_Write(2 , 0x0A,0x03);

//#GIP correction RST Falling
MIPI_SPI_Write(4,  0xFF ,0x61, 0x36, 0x07); 
MIPI_SPI_Write(2,  0x0E, 0x35);

//#GIP correction RST Rising
MIPI_SPI_Write(4,  0xFF, 0x61 ,0x36, 0x07 );
MIPI_SPI_Write(3 , 0x0B, 0x1F);

//####GIP correction STV width
MIPI_SPI_Write(4,  0xFF ,0x61, 0x36 ,0x07);
MIPI_SPI_Write(2 , 0x1C, 0xEB);



//#??GIP ??VGOFF??? 
MIPI_SPI_Write(4,  0xFF, 0x61, 0x36, 0x08);             
MIPI_SPI_Write(2,  0x6C, 0x02);	
MIPI_SPI_Write(2,  0x5F, 0x0F);	 
MIPI_SPI_Write(2 , 0xAB, 0x24);


//#Sleep out & TE enable
MIPI_SPI_Write(4,  0xFF, 0x61 ,0x36, 0x00);
MIPI_SPI_Write(2 , 0x35 ,0x01);
DelayMs(20);
MIPI_SPI_Write(2,0x11,0x00);
DelayMs(150);

//#Display on
MIPI_SPI_Write(2,0x29,0x00);
DelayMs(150);

//mipi.video.enable 

//#-------------------------------------------------------------------------------
//# Send Image data
//#-------------------------------------------------------------------------------
//image.display 1

}
void SSD2828_inition_lcd_Check(void)    //IC³õÊ¼»¯
{
//#===============================================================================
//#  Main LDI Power ON Sequence
//#===============================================================================

//#Page8 for Blanking OSC*2 
MIPI_SPI_Write(4,  0xFF, 0x61, 0x36, 0x08);    
MIPI_SPI_Write(2,  0x1C, 0xA0);

//# 53H for Vcom correction, # 50H for Vreg1, # 51H for Vreg2
MIPI_SPI_Write(4,  0xFF, 0x61 ,0x36, 0x01);

MIPI_SPI_Write(2,  0x50, 0x85);
MIPI_SPI_Write(2,  0x51 ,0x85);

//#Page1 for POS gamma correction
MIPI_SPI_Write(4,  0xFF, 0x61, 0x36, 0x01);
MIPI_SPI_Write(2,  0xA0, 0x05);
MIPI_SPI_Write(2,  0xA1, 0x05);
MIPI_SPI_Write(2,  0xA2, 0x06);
MIPI_SPI_Write(2,  0xA3, 0x0d);
MIPI_SPI_Write(2 , 0xA4, 0x04);
MIPI_SPI_Write(2,  0xA5, 0x07);
MIPI_SPI_Write(2 , 0xA6, 0x0f);
MIPI_SPI_Write(2,  0xA7, 0x14);
MIPI_SPI_Write(2 , 0xA8, 0x1F);
MIPI_SPI_Write(2 , 0xA9, 0x28);
MIPI_SPI_Write(2 , 0xAA, 0x31);
MIPI_SPI_Write(2,  0xAB, 0x39);
MIPI_SPI_Write(2,  0xAC, 0x39);
MIPI_SPI_Write(2,  0xAD, 0x31);
MIPI_SPI_Write(2,  0xAE, 0x2e);
MIPI_SPI_Write(2,  0xAF, 0x2f);
MIPI_SPI_Write(2,  0xB0, 0x3c);
//#Page1 for NEG gamma correction
MIPI_SPI_Write(4,  0xFF, 0x61 ,0x36, 0x01);
MIPI_SPI_Write(2,  0xC0, 0x05);
MIPI_SPI_Write(2,  0xC1, 0x05);
MIPI_SPI_Write(2,  0xC2, 0x06);
MIPI_SPI_Write(2,  0xC3, 0x0d);
MIPI_SPI_Write(2,  0xC4, 0x04);
MIPI_SPI_Write(2,  0xC5, 0x07);
MIPI_SPI_Write(2,  0xC6, 0x0f);
MIPI_SPI_Write(2,  0xC7, 0x14);
MIPI_SPI_Write(2,  0xC8, 0x1f);
MIPI_SPI_Write(2,  0xC9, 0x28);
MIPI_SPI_Write(2,  0xCA, 0x31);
MIPI_SPI_Write(2,  0xCB, 0x39);
MIPI_SPI_Write(2 , 0xCC, 0x39);
MIPI_SPI_Write(2,  0xCD, 0x31);
MIPI_SPI_Write(2,  0xCE, 0x2e);
MIPI_SPI_Write(2,  0xCF, 0x2f);
MIPI_SPI_Write(2,  0xD0, 0x3c);

//#Source_Out Performnace Enhancement
MIPI_SPI_Write(4,  0xFF, 0x61, 0x36, 0x08); 
MIPI_SPI_Write(2,  0xE9, 0x0B);

MIPI_SPI_Write(4,  0xFF, 0x61, 0x36, 0x06 );
MIPI_SPI_Write(2,  0x72, 0x01);

//#VGH Voltage change to 16.75V
MIPI_SPI_Write(4,  0xFF ,0x61, 0x36, 0x08); 
MIPI_SPI_Write(2,  0x93, 0x08);

//#VGL Voltage change to -11.16V
MIPI_SPI_Write(2,  0x8E, 0x12);


//#VGH pump ratio
MIPI_SPI_Write(4,  0xFF, 0x61, 0x36 ,0x08 );
MIPI_SPI_Write(2,  0x76, 0xB4);
//#VGL pump ratio
MIPI_SPI_Write(2,  0x78, 0x02);

//# VGH / VGL pump CLK
MIPI_SPI_Write(4,  0xFF ,0x61, 0x36, 0x01 );
MIPI_SPI_Write(2,  0x42, 0x43 );

//#sdt correction
MIPI_SPI_Write(4,  0xFF, 0x61 ,0x36, 0x01); 
MIPI_SPI_Write(2,  0x60, 0x14);

//#??GIP ??VGOFF??? 
MIPI_SPI_Write(4,  0xFF, 0x61 ,0x36, 0x08 );            
MIPI_SPI_Write(2,  0x6C, 0x02 );	
MIPI_SPI_Write(2,  0x5F, 0x0F	); 
MIPI_SPI_Write(2,  0xAB, 0x24);


//#Sleep out & TE enable
MIPI_SPI_Write(4,  0xFF, 0x61, 0x36, 0x00);
MIPI_SPI_Write(2,  0x35, 0x01);
DelayMs(20);
MIPI_SPI_Write(2,  0x11, 0x00);

DelayMs(150);

//#Display on
MIPI_SPI_Write(2,  0x29, 0x00);
DelayMs(150);
	
	
}
void Forward_scan(void)           //ÕýÉ¨
{;}
void Backward_scan(void)          //·´É¨
{;}	
/*******************************************************************************
* Function Name  : void MTP(void)
* Description    : otp -> vcom     //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void MTP(void)
{	
//#===============================================================================
//#  Main LDI Power ON Sequence
//#===============================================================================

MIPI_SPI_Write( 1, 0x11);                    
Delay(120);

//#Set Program OTP Address and Data
MIPI_SPI_Write(4, 0xFF, 0x61, 0x36, 0x01); 

//#Supply Extemal 7.5V to VPP
//gpio.write 0x44
VOTP_ADJ_SET(775);			
VOTP_EN(1);
	
Delay(380);

//#=============Vcom E0============
MIPI_SPI_Write( 2, 0xE0, VCOMDC1);        //ÕýÉ¨VCOM
MIPI_SPI_Write( 2, 0xE1, 0x05);        //·´É¨VCOM
MIPI_SPI_Write( 2, 0xE2, 0x00);

//#Step 3 : Enable OTP Program Key
MIPI_SPI_Write( 2, 0xE3, 0x55);
MIPI_SPI_Write( 2, 0xE4, 0xAA);
MIPI_SPI_Write( 2, 0xE5, 0x66);
Delay(50);

//#sleep out
MIPI_SPI_Write( 1, 0x11);

DelayMs(20);
//#=========Internal VGH/VGL(4C)========
MIPI_SPI_Write( 2, 0xE0, 0x00 );  
MIPI_SPI_Write( 2, 0xE1, 0xF4);
MIPI_SPI_Write( 2, 0xE2, 0x01);

//#Step 3 : Enable OTP Program Key
MIPI_SPI_Write( 2, 0xE3, 0x55);
MIPI_SPI_Write( 2, 0xE4, 0xAA);
MIPI_SPI_Write( 2, 0xE5, 0x66);

//#sleep out
MIPI_SPI_Write( 1, 0x11);

DelayMs(20);

//#======GIP correction CLK Falling=======
MIPI_SPI_Write( 2, 0xE0, 0x05 );	  
MIPI_SPI_Write( 2, 0xE1, 0x42);
MIPI_SPI_Write( 2, 0xE2, 0x01);

//#Step 3 : Enable OTP Program Key
MIPI_SPI_Write( 2, 0xE3, 0x55);
MIPI_SPI_Write( 2, 0xE4, 0xAA);
MIPI_SPI_Write( 2, 0xE5, 0x66);

//#sleep out
MIPI_SPI_Write( 1, 0x11);

DelayMs(20);

//#======GIP correction CLK Rising======= 
MIPI_SPI_Write( 2, 0xE0, 0x1F);   
MIPI_SPI_Write( 2, 0xE1, 0x3E);
MIPI_SPI_Write( 2, 0xE2, 0x01);

//#Step 3 : Enable OTP Program Key
MIPI_SPI_Write( 2, 0xE3, 0x55);
MIPI_SPI_Write( 2, 0xE4, 0xAA);
MIPI_SPI_Write( 2, 0xE5, 0x66);

//#sleep out
MIPI_SPI_Write( 1, 0x11);

DelayMs(20);

//#=====page7, 17h=1Fh ======
MIPI_SPI_Write( 2, 0xE0, 0x1F); 
MIPI_SPI_Write( 2, 0xE1, 0x3F);
MIPI_SPI_Write( 2, 0xE2, 0x01);

//#Step 3 : Enable OTP Program Key
MIPI_SPI_Write( 2, 0xE3, 0x55);
MIPI_SPI_Write( 2, 0xE4, 0xAA);
MIPI_SPI_Write( 2, 0xE5, 0x66);

//#sleep out
MIPI_SPI_Write( 1, 0x11);

DelayMs(20);


//#======GIP correction STV Falling======= 
MIPI_SPI_Write( 2, 0xE0, 0x05);   
MIPI_SPI_Write( 2, 0xE1, 0x35);
MIPI_SPI_Write( 2, 0xE2, 0x01);

//#Step 3 : Enable OTP Program Key
MIPI_SPI_Write( 2, 0xE3, 0x55);
MIPI_SPI_Write( 2, 0xE4, 0xAA);
MIPI_SPI_Write( 2, 0xE5, 0x66);

//#sleep out
MIPI_SPI_Write( 1, 0x11);

DelayMs(20);

//#======GIP correction STV Rising=======
MIPI_SPI_Write( 2, 0xE0, 0x03 );  
MIPI_SPI_Write( 2, 0xE1, 0x32);
MIPI_SPI_Write( 2, 0xE2, 0x01);

//#Step 3 : Enable OTP Program Key
MIPI_SPI_Write( 2, 0xE3, 0x55);
MIPI_SPI_Write( 2, 0xE4, 0xAA);
MIPI_SPI_Write( 2, 0xE5, 0x66);

//#sleep out
MIPI_SPI_Write( 1, 0x11);

DelayMs(20);

//#======GIP correction RST Falling======
MIPI_SPI_Write( 2, 0xE0, 0x35 );  
MIPI_SPI_Write( 2, 0xE1, 0x36);
MIPI_SPI_Write( 2, 0xE2, 0x01);

//#Step 3 : Enable OTP Program Key
MIPI_SPI_Write( 2, 0xE3, 0x55);
MIPI_SPI_Write( 2, 0xE4, 0xAA);
MIPI_SPI_Write( 2, 0xE5, 0x66);

//#sleep out
MIPI_SPI_Write( 1, 0x11);

DelayMs(20);

//#======GIP correction RST Rising======
MIPI_SPI_Write( 2, 0xE0, 0x1F);   
MIPI_SPI_Write( 2, 0xE1, 0x33);
MIPI_SPI_Write( 2, 0xE2, 0x01);

//#Step 3 : Enable OTP Program Key
MIPI_SPI_Write( 2, 0xE3, 0x55);
MIPI_SPI_Write( 2, 0xE4, 0xAA);
MIPI_SPI_Write( 2, 0xE5, 0x66);

//#sleep out
MIPI_SPI_Write( 1, 0x11);

DelayMs(20);

//#======GIP STV width ======
MIPI_SPI_Write( 2, 0xE0, 0xEB);   
MIPI_SPI_Write( 2, 0xE1, 0x44);
MIPI_SPI_Write( 2, 0xE2, 0x01);

//#Step 3 : Enable OTP Program Key
MIPI_SPI_Write( 2, 0xE3, 0x55);
MIPI_SPI_Write( 2, 0xE4 ,0xAA);
MIPI_SPI_Write( 2, 0xE5, 0x66);

//#sleep out
MIPI_SPI_Write( 1, 0x11);

DelayMs(20);


//#======GIP Rising(18)======
MIPI_SPI_Write( 2, 0xE0 ,0x05 );  
MIPI_SPI_Write( 2, 0xE1, 0x40);
MIPI_SPI_Write( 2, 0xE2, 0x01);

//#Step 3 : Enable OTP Program Key
MIPI_SPI_Write( 2, 0xE3, 0x55);
MIPI_SPI_Write( 2, 0xE4, 0xAA);
MIPI_SPI_Write( 2, 0xE5, 0x66);

//#sleep out
MIPI_SPI_Write( 1, 0x11);
DelayMs(20);

//#======GIP Rising(19)======
MIPI_SPI_Write( 2, 0xE0, 0x00 );  
MIPI_SPI_Write( 2, 0xE1, 0x41);
MIPI_SPI_Write( 2, 0xE2, 0x01);

//#Step 3 : Enable OTP Program Key
MIPI_SPI_Write( 2, 0xE3, 0x55);
MIPI_SPI_Write( 2, 0xE4 ,0xAA);
MIPI_SPI_Write( 2, 0xE5, 0x66);

//#sleep out
MIPI_SPI_Write( 1, 0x11);

DelayMs(20);
//#Cut off 7.5V to VPP
//gpio.write 0x04
VOTP_ADJ_SET(330);			  //¹Ø±ÕÍâ²¿OTPµçÑ¹
VOTP_EN(0);

DelayMs(100);

//#Reset IC
//gpio.write 0x00		
//MDelay(10);					
//gpio.write 0x04
//MDelay(10);

//#Power off 
//board.power.off vio
//board.power.off vdd
//board.power.off led  	
}

/*******************************************************************************
* Function Name  : void MTP_ID(void)
* Description    : otp -> ID       //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  

void MTP_ID(void)
{	;	}	
#endif

#ifdef HD720x1280
//////mipi video mode setting
u16 value_HighSpeed = 480;
u16 value_Lane_select = 2;

u16  VDP= 1280;      
u16  VBP= 11 ;    //
u16  VFP= 6 ;    //

u16  HDP= 720;     
u16  HBP= 88;     //	
u16  HFP= 84;     //	

u16	 HPW=8;  //Í¨³£²»ÐèÒªµ÷Õû

u16	 VPW=4;  //Í¨³£²»ÐèÒªµ÷Õû
	
/////////////CLAN050LQ  USE    inition/////
u8 PIC_NUM=14;       //ëŠœy®‹Ãæ×ÜÊý
u8 Flicker_OTP=3;    //OTP flicker ®‹Ãæ¾ŽÌ–
u8 Flicker_OQC=2;    //QC flicker  ®‹Ãæ¾ŽÌ–
//-------------------------------------------------------------------
//vcom½×´ÎÉè¶¨·½Ê½
void VCOM_set(u8 vcom)
{;}   
//IDÖµÉè¶¨·½Ê½
void ID_set(void)
{;}		
void SSD2828_inition_lcd(void)    //IC³õÊ¼»¯
{;}
void Forward_scan(void)           //ÕýÉ¨
{;}
void Backward_scan(void)          //·´É¨
{;}	
/*******************************************************************************
* Function Name  : void MTP(void)
* Description    : otp -> vcom     //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void MTP(void)
{	;	}

/*******************************************************************************
* Function Name  : void MTP_ID(void)
* Description    : otp -> ID       //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  

void MTP_ID(void)
{	;	}
	
#endif

#ifdef FHD1080x1920
//////mipi video mode setting
u16 value_HighSpeed = 480;
u16 value_Lane_select = 2;

u16  VDP= 1920;      
u16  VBP= 16 ;    //	  VBP
u16  VFP= 14 ;    //

u16  HDP= 1080;     
u16  HBP= 80;     //	  HBP
u16  HFP= 40;     //

u16	 VPW=4;  //Í¨³£²»ÐèÒªµ÷Õû
u16	 HPW=8;  //Í¨³£²»ÐèÒªµ÷Õû  

/////////////CLAN050LQ  USE    inition/////
u8 PIC_NUM=14;       //ëŠœy®‹Ãæ×ÜÊý
u8 Flicker_OTP=3;    //OTP flicker ®‹Ãæ¾ŽÌ–
u8 Flicker_OQC=2;    //QC flicker  ®‹Ãæ¾ŽÌ–
//-------------------------------------------------------------------
//vcom½×´ÎÉè¶¨·½Ê½
void VCOM_set(u8 vcom)
{;}   
//IDÖµÉè¶¨·½Ê½
void ID_set(void)
{;}		
void SSD2828_inition_lcd(void)    //IC³õÊ¼»¯
{;}
void Forward_scan(void)           //ÕýÉ¨
{;}
void Backward_scan(void)          //·´É¨
{;}	
/*******************************************************************************
* Function Name  : void MTP(void)
* Description    : otp -> vcom     //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void MTP(void)
{	;	}

/*******************************************************************************
* Function Name  : void MTP_ID(void)
* Description    : otp -> ID       //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  

void MTP_ID(void)
{	;	} 

#endif

#ifdef FHD1200x1920
//////mipi video mode setting
u16 value_HighSpeed = 480;
u16 value_Lane_select = 2;

u16  VDP= 1200;      
u16  VBP= 6 ;    //	  VBP
u16  VFP= 4 ;    //

u16  HDP= 1920;     
u16  HBP= 88;     //	  HBP
u16  HFP= 84;     //	

u16	 VPW=2;  //Í¨³£²»ÐèÒªµ÷Õû
u16	 HPW=4;  //Í¨³£²»ÐèÒªµ÷Õû  

	
/////////////CLAN050LQ  USE    inition/////
u8 PIC_NUM=14;       //ëŠœy®‹Ãæ×ÜÊý
u8 Flicker_OTP=3;    //OTP flicker ®‹Ãæ¾ŽÌ–
u8 Flicker_OQC=2;    //QC flicker  ®‹Ãæ¾ŽÌ–
//-------------------------------------------------------------------
//vcom½×´ÎÉè¶¨·½Ê½
void VCOM_set(u8 vcom)
{;}   
//IDÖµÉè¶¨·½Ê½
void ID_set(void)
{;}		
void SSD2828_inition_lcd(void)    //IC³õÊ¼»¯
{;}
void Forward_scan(void)           //ÕýÉ¨
{;}
void Backward_scan(void)          //·´É¨
{;}	
/*******************************************************************************
* Function Name  : void MTP(void)
* Description    : otp -> vcom     //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void MTP(void)
{	;	}

/*******************************************************************************
* Function Name  : void MTP_ID(void)
* Description    : otp -> ID       //´ËÌŽÐè¸ù“þ²»Í¬ÐÍÌ–ßMÐÐOTPÁ÷³ÌÐÞ¸Ä
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  

void MTP_ID(void)
{	;	}
	
#endif



void STM_MINI_SET(void)
{
LCD_CtrlLinesConfig();		Delay(10);  
	LCD_RST(0) ;  //Ä¬ÈÏÎª0£¬ÖÃ1ºó»áÓ°ÏìLCD VDD µçÑ¹
	DelayMs(20);  
	VDD_ADJ_EN(0); //Ä¬ÈÏÎª0£¬ÖÃ1ºó»áÓ°ÏìLCD VDD µçÑ¹
	SSD2828_SHUT(0);
	SSD2828_RST(1);
	SSD2828_CSX0(1);
	SSD2828_SCL(1);
	SSD2828_SDI(1);
	 		 
	GPIO_SetBits(GPIOA, GPIO_Pin_1);  //
	DelayMs(30);             // ³ÌÐò¶à´Î¸´Î»Ê±£¬±£Ö¤¸´Î»Ê±¼ä
	

	T226_intion();


	VDD_ADJ_SET(305);			 //VDD_ADJ_SET ²ÎÊýÈ¡Öµ·¶Î§270~620£¬¶ÔÓ¦µçÑ¹·¶Î§2.7V~6.2V; 
	VDD_ADJ_EN(1);				 //Ê¹ÄÜVDDÊä³ö

//	VDD_ADJ_EN(0);
	
	VDDIO_ADJ_SET(185);			  //VDDIO_ADJ_SET ²ÎÊýÈ¡Öµ·¶Î§150~360£¬¶ÔÓ¦µçÑ¹·¶Î§1.5V~3.6V; 
//	VDDIO_ADJ_EN(1);			  //Ê¹ÄÜVDDIOÊä³ö
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);  //
		
		
	VOTP_EN(0);	
//	VOTP_ADJ_SET(490);			  //VOTP_ADJ_SET ²ÎÊýÈ¡Öµ·¶Î§150~2080£¬¶ÔÓ¦µçÑ¹·¶Î§1.5V~14.1V; 
//	VOTP_EN(1);					  //Ê¹ÄÜVOTPÊä³ö

	DelayMs(30); 
}
/*******************************************************************************
* Function Name  : STM32_Init_SSD2828
* Description    : Initializes the LCD.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void STM32_Init_SSD2828(void)
{ 
	u16 VHPW,VHBP,VHFP;
	u16 cym_KEY; 

	VHPW=(VPW<<8)+HPW;
	VHBP=((VBP+0)<<8)+(HBP+0);	
	VHFP=((VFP+0)<<8)+(HFP+0);

	LCD_CtrlLinesConfig();		DelayMs(10);  
	LCD_RST(0) ;  //Ä¬ÈÏÎª0£¬ÖÃ1ºó»áÓ°ÏìLCD VDD µçÑ¹
	DelayMs(20);  
	//VDD_ADJ_EN(0); //Ä¬ÈÏÎª0£¬ÖÃ1ºó»áÓ°ÏìLCD VDD µçÑ¹
	SSD2828_SHUT(0);
	SSD2828_RST(1);
	SSD2828_CSX0(1);
	SSD2828_SCL(1);
	SSD2828_SDI(1);
	 		 
	//GPIO_SetBits(GPIOA, GPIO_Pin_1);  //
	DelayMs(30);             // ³ÌÐò¶à´Î¸´Î»Ê±£¬±£Ö¤¸´Î»Ê±¼ä
	

//	T226_intion();


//	VDD_ADJ_SET(290);			 //VDD_ADJ_SET ²ÎÊýÈ¡Öµ·¶Î§270~620£¬¶ÔÓ¦µçÑ¹·¶Î§2.7V~6.2V; 
//	VDD_ADJ_EN(1);				 //Ê¹ÄÜVDDÊä³ö


//	VDDIO_ADJ_SET(200);			  //VDDIO_ADJ_SET ²ÎÊýÈ¡Öµ·¶Î§150~360£¬¶ÔÓ¦µçÑ¹·¶Î§1.5V~3.6V; 
//	VDDIO_ADJ_EN(1);			  //Ê¹ÄÜVDDIOÊä³ö

//	VOTP_ADJ_SET(490);			  //VOTP_ADJ_SET ²ÎÊýÈ¡Öµ·¶Î§150~2080£¬¶ÔÓ¦µçÑ¹·¶Î§1.5V~14.1V; 
//	VOTP_EN(1);					  //Ê¹ÄÜVOTPÊä³ö

//	DelayMs(30); 

	LCD_RST(1); 
	DelayMs(50);	
	LCD_RST(0);   SSD2828_RST(0) ; 	DelayMs(50);	      //Ó²¼þ¸´Î»   
	LCD_RST(1) ;  SSD2828_RST(1);	DelayMs(150);

////=============================¿ª»ú¼ì²âÊÇ·ñ´óµçÁ÷==========================
//  if(Measure_IDD(mA) > 11000)				////if IDD > 110mA,,,,,power  off
//	{
//		BEEP_Dudu();	 /////////beep  on
//		VDD_ADJ_EN(0);	
//		VDDIO_ADJ_EN(0);	
//		VOTP_EN(0);		
//		BEEP_ON();	 /////////beep  on
//		while(1);
//	}		
//	

//----------------------------check 2828ÊÇ·ñÕý³£º-------------

	while( READ_SSD2828_onebyte(0XB0) != 0x2828)
		{
			LCD_RST(0);//Ó²¼þ¸´Î»
			SSD2828_RST(0) ;
			DelayMs(50);	
			LCD_RST(1) ;
			SSD2828_RST(1);//Ó²¼þ¸´Î»
			DelayMs(150);
	
			STM32TOSSD2828_W_COM(0xc0);
			STM32TOSSD2828_W_DATA_16BITS(0x0100);		
			DelayMs(50);
		}

	{
		STM32TOSSD2828_W_COM(0xB1);
		STM32TOSSD2828_W_DATA_16BITS(VHPW);		//Vertical sync and horizontal sync active period 

		STM32TOSSD2828_W_COM(0xB2);
		STM32TOSSD2828_W_DATA_16BITS(VHBP);		//Vertical and horizontal back porch period 


		STM32TOSSD2828_W_COM(0xB3);
		STM32TOSSD2828_W_DATA_16BITS(VHFP);		//Vertical and horizontal front porch period 

		STM32TOSSD2828_W_COM(0xB4);
		STM32TOSSD2828_W_DATA_16BITS(HDP);		//Horizontal active period 
//		X = READ_SSD2828_onebyte(0xB4);

		STM32TOSSD2828_W_COM(0xB5);
		STM32TOSSD2828_W_DATA_16BITS(VDP);		//Vertical active period 
//		Y = READ_SSD2828_onebyte(0xB5);

		STM32TOSSD2828_W_COM(0xB6);		//Video mode and video pixel format
		STM32TOSSD2828_W_DATA_16BITS(0x000B);		//24bit 			   

		STM32TOSSD2828_W_COM(0xDE);		
		STM32TOSSD2828_W_DATA_16BITS(value_Lane_select-1);		//MIPI lane select  

		STM32TOSSD2828_W_COM(0xd6);	
		STM32TOSSD2828_W_DATA_16BITS(0x00F5);		//Color order and endianess 

		STM32TOSSD2828_W_COM(0xb8);		//VC register 
		STM32TOSSD2828_W_DATA_16BITS(0x0000);

		STM32TOSSD2828_W_COM(0xb9);		//PLL disable 
		STM32TOSSD2828_W_DATA_16BITS(0x0000);
		DelayMs(5);

		data = value_HighSpeed/12;

		if(value_HighSpeed < 125 )
		{
			data =  data;	
		}
		else if(value_HighSpeed < 250 )
		{
			data = 0x4000 + data;	
		}
		else  if(value_HighSpeed < 500 )
		{
			data = 0x8000 + data;	
		}
		else
		{
			data = 0xc000 + data;	
		}
			
		STM32TOSSD2828_W_COM(0xba);		//PLL setting 
		STM32TOSSD2828_W_DATA_16BITS(data);

		STM32TOSSD2828_W_COM(0xbb);		//LP clock divider 
		STM32TOSSD2828_W_DATA_16BITS(0x0006);			//////8.8MHZ	
		DelayMs(5);

		STM32TOSSD2828_W_COM(0xb9);		//PLL enable 
		STM32TOSSD2828_W_DATA_16BITS(0xc001);		//8·ÖÆµ	£¬SYS_CLKÊä³ö24/8=3MHZ
		DelayMs(5);
	}

	SSD2828_ENTER_LP_mode();
	DelayMs(5); 

  //-------------------------------------------------------------------
  //-------------------------------------------------------------------	

	cym_KEY = KEY_AUTO_MODE;
	if(cym_KEY== 0)	//OTPÄ£Ê½
	{
		if(MTP_OVER==0)
		{	
	  SSD2828_inition_lcd();    //³õÊ¼»¯IC
		}
		else
		{
	  SSD2828_inition_lcd_Check(); 	
		}
	}
	else
	{	
	SSD2828_inition_lcd_Check(); 
	}
	DelayMs(5); 

	SSD2828_VIDEO_MODE_HS();
	DelayMs(5);			//	SSD2828_VIDEO_MODE_HS();

}

void Only_STM32_Init_SSD2828(void)
{ 
	u16 VHPW,VHBP,VHFP;
	u16 cym_KEY; 

	VHPW=(VPW<<8)+HPW;
	VHBP=((VBP)<<8)+(HBP);	
	VHFP=(VFP<<8)+HFP;

	LCD_CtrlLinesConfig();		DelayMs(10);  
	LCD_RST(0) ;  //Ä¬ÈÏÎª0£¬ÖÃ1ºó»áÓ°ÏìLCD VDD µçÑ¹
	DelayMs(20);  
	//VDD_ADJ_EN(0); //Ä¬ÈÏÎª0£¬ÖÃ1ºó»áÓ°ÏìLCD VDD µçÑ¹
	SSD2828_SHUT(0);
	SSD2828_RST(1);
	SSD2828_CSX0(1);
	SSD2828_SCL(1);
	SSD2828_SDI(1);
	 		 
	//GPIO_SetBits(GPIOA, GPIO_Pin_1);  //
	DelayMs(30);             // ³ÌÐò¶à´Î¸´Î»Ê±£¬±£Ö¤¸´Î»Ê±¼ä
	

//	T226_intion();


//	VDD_ADJ_SET(290);			 //VDD_ADJ_SET ²ÎÊýÈ¡Öµ·¶Î§270~620£¬¶ÔÓ¦µçÑ¹·¶Î§2.7V~6.2V; 
//	VDD_ADJ_EN(1);				 //Ê¹ÄÜVDDÊä³ö


//	VDDIO_ADJ_SET(200);			  //VDDIO_ADJ_SET ²ÎÊýÈ¡Öµ·¶Î§150~360£¬¶ÔÓ¦µçÑ¹·¶Î§1.5V~3.6V; 
//	VDDIO_ADJ_EN(1);			  //Ê¹ÄÜVDDIOÊä³ö

//	VOTP_ADJ_SET(490);			  //VOTP_ADJ_SET ²ÎÊýÈ¡Öµ·¶Î§150~2080£¬¶ÔÓ¦µçÑ¹·¶Î§1.5V~14.1V; 
//	VOTP_EN(1);					  //Ê¹ÄÜVOTPÊä³ö

//	DelayMs(30); 

	LCD_RST(1); 
	DelayMs(50);	
	LCD_RST(0);   SSD2828_RST(0) ; 	DelayMs(50);	      //Ó²¼þ¸´Î»   
	LCD_RST(1) ;  SSD2828_RST(1);	DelayMs(150);

////=============================¿ª»ú¼ì²âÊÇ·ñ´óµçÁ÷==========================
//  if(Measure_IDD(mA) > 11000)				////if IDD > 110mA,,,,,power  off
//	{
//		BEEP_Dudu();	 /////////beep  on
//		VDD_ADJ_EN(0);	
//		VDDIO_ADJ_EN(0);	
//		VOTP_EN(0);		
//		BEEP_ON();	 /////////beep  on
//		while(1);
//	}		
//	

//----------------------------check 2828ÊÇ·ñÕý³£º-------------

	while( READ_SSD2828_onebyte(0XB0) != 0x2828)
		{
			LCD_RST(0);//Ó²¼þ¸´Î»
			SSD2828_RST(0) ;
			DelayMs(50);	
			LCD_RST(1) ;
			SSD2828_RST(1);//Ó²¼þ¸´Î»
			DelayMs(150);
	
			STM32TOSSD2828_W_COM(0xc0);
			STM32TOSSD2828_W_DATA_16BITS(0x0100);		
			DelayMs(50);
		}

	{
		STM32TOSSD2828_W_COM(0xB1);
		STM32TOSSD2828_W_DATA_16BITS(VHPW);		//Vertical sync and horizontal sync active period 

		STM32TOSSD2828_W_COM(0xB2);
		STM32TOSSD2828_W_DATA_16BITS(VHBP);		//Vertical and horizontal back porch period 


		STM32TOSSD2828_W_COM(0xB3);
		STM32TOSSD2828_W_DATA_16BITS(VHFP);		//Vertical and horizontal front porch period 

		STM32TOSSD2828_W_COM(0xB4);
		STM32TOSSD2828_W_DATA_16BITS(HDP);		//Horizontal active period 
//		X = READ_SSD2828_onebyte(0xB4);

		STM32TOSSD2828_W_COM(0xB5);
		STM32TOSSD2828_W_DATA_16BITS(VDP);		//Vertical active period 
//		Y = READ_SSD2828_onebyte(0xB5);

		STM32TOSSD2828_W_COM(0xB6);		//Video mode and video pixel format
		STM32TOSSD2828_W_DATA_16BITS(0x000B);		//24bit 			   

		STM32TOSSD2828_W_COM(0xDE);		
		STM32TOSSD2828_W_DATA_16BITS(value_Lane_select-1);		//MIPI lane select  

		STM32TOSSD2828_W_COM(0xd6);	
		STM32TOSSD2828_W_DATA_16BITS(0x0001);		//Color order and endianess 

		STM32TOSSD2828_W_COM(0xb8);		//VC register 
		STM32TOSSD2828_W_DATA_16BITS(0x0000);

		STM32TOSSD2828_W_COM(0xb9);		//PLL disable 
		STM32TOSSD2828_W_DATA_16BITS(0x0000);
		DelayMs(5);

		data = value_HighSpeed/12;

		if(value_HighSpeed < 125 )
		{
			data =  data;	
		}
		else if(value_HighSpeed < 250 )
		{
			data = 0x4000 + data;	
		}
		else  if(value_HighSpeed < 500 )
		{
			data = 0x8000 + data;	
		}
		else
		{
			data = 0xc000 + data;	
		}
			
		STM32TOSSD2828_W_COM(0xba);		//PLL setting 
		STM32TOSSD2828_W_DATA_16BITS(data);

		STM32TOSSD2828_W_COM(0xbb);		//LP clock divider 
		STM32TOSSD2828_W_DATA_16BITS(0x0006);			//////8.8MHZ	
		DelayMs(5);

		STM32TOSSD2828_W_COM(0xb9);		//PLL enable 
		STM32TOSSD2828_W_DATA_16BITS(0xc001);		//8·ÖÆµ	£¬SYS_CLKÊä³ö24/8=3MHZ
		DelayMs(5);
	}

	SSD2828_ENTER_LP_mode();
	DelayMs(5); 
  //-------------------------------------------------------------------
  //-------------------------------------------------------------------	
	SSD2828_inition_lcd();    //³õÊ¼»¯IC
	SSD2828_VIDEO_MODE_HS();
	DelayMs(5);			//	SSD2828_VIDEO_MODE_HS();
}

void SSD2828_VIDEO_MODE_HS(void)  //²»Í¬·Ö±æÂÊµÄLCDÐèÒª²»Í¬µÄ´«ÊäËÙ¶È£¬Ðè×öÕë¶Ôµ÷Õû
{	
//	STM32TOSSD2828_W_COM(0xb8);		//VC register 
//	STM32TOSSD2828_W_DATA_16BITS(0x0000);
//
//	STM32TOSSD2828_W_COM(0xb9);		//PLL disable 
//	STM32TOSSD2828_W_DATA_16BITS(0x0000);
//	DelayMs(5);
//
//	STM32TOSSD2828_W_COM(0xba);		//PLL setting 
//	STM32TOSSD2828_W_DATA_16BITS(data);
//
//	STM32TOSSD2828_W_COM(0xbb);		//LP clock divider 
//	STM32TOSSD2828_W_DATA_16BITS(0x000A);			
////	DelayMs(5);
//
//	STM32TOSSD2828_W_COM(0xb9);		//PLL enable 
//	STM32TOSSD2828_W_DATA_16BITS(0xc001);		//8·ÖÆµ	£¬SYS_CLKÊä³ö24/8=3MHZ
//	DelayMs(5);
	
	STM32TOSSD2828_W_COM(0xb7);		//Generic mode, HS video mode
    STM32TOSSD2828_W_DATA_16BITS(0x070b);		 //HS video +  short packet 
	DelayMs(5);

}

void SSD2828_ENTER_LP_mode(void)  //LPÄ£Ê½ËÙ¶ÈÍ¨ÓÃ
{
//	STM32TOSSD2828_W_COM(0xb8);		//VC register 
//	STM32TOSSD2828_W_DATA_16BITS(0x0000);
//
//	STM32TOSSD2828_W_COM(0xb9);		//PLL disable 
//	STM32TOSSD2828_W_DATA_16BITS(0x0000);
//	DelayMs(5);
//
//	STM32TOSSD2828_W_COM(0xba);		//PLL setting 
//	STM32TOSSD2828_W_DATA_16BITS(data);
//
//	STM32TOSSD2828_W_COM(0xbb);		//LP clock divider 
//	STM32TOSSD2828_W_DATA_16BITS(0x000A);			
////	DelayMs(5);
//
//	STM32TOSSD2828_W_COM(0xb9);		//PLL enable 
//	STM32TOSSD2828_W_DATA_16BITS(0xc001);		//8·ÖÆµ	£¬SYS_CLKÊä³ö24/8=3MHZ
//	DelayMs(5);

	STM32TOSSD2828_W_COM(0xb7);		//DCS mode, LP mode 
    STM32TOSSD2828_W_DATA_16BITS(0x0752);	
	DelayMs(5);
}
void SSD2828_ENTER_READ_LCDIC_mode(void)
{
	STM32TOSSD2828_W_COM(0xb7);
	STM32TOSSD2828_W_DATA_16BITS(0x03c2);	
	DelayMs(5);
}

u8 READ_IC(u8 cmd,u8 cmd1)  //¶Á2828´ÓLCD icÖÐÈ¡»ØÀ´µÄÖµ
{
	u16 Value=0;



xxxx1:
	SSD2828_ENTER_LP_mode();

	DelayMs(100);

	STM32TOSSD2828_W_COM(0xc4);		//enable BTA
	STM32TOSSD2828_W_DATA_16BITS(0x0001);

	STM32TOSSD2828_W_COM(0xc1);		//Max. return packet is 255 bytes
	STM32TOSSD2828_W_DATA_16BITS(0x000a);

	STM32TOSSD2828_W_COM(0xc0);
	STM32TOSSD2828_W_DATA_16BITS(0x0001);

	MDelay(200);

	SSD2828_ENTER_READ_LCDIC_mode();
	MDelay(200);

	TC358768_DCS_write_1A_1P(cmd,cmd1);
	MDelay(200);

	if(((READ_SSD2828_onebyte(0xc6))&0x01)==1)
	{
		MDelay(200);
		Value = READ_SSD2828_onebyte(0xff);	
		MIPI_READ_DATA[0] = Value;
		MIPI_READ_DATA[1] = Value>>8;

		Value = SSD2825_READ_AnyByte(0xfA);
		MIPI_READ_DATA[2] = Value;
		MIPI_READ_DATA[3] = Value>>8;

		Value = SSD2825_READ_AnyByte(0xfA);
		MIPI_READ_DATA[4] = Value;
		MIPI_READ_DATA[5] = Value>>8;

		Value = SSD2825_READ_AnyByte(0xfA);
		MIPI_READ_DATA[6] = Value;
		MIPI_READ_DATA[7] = Value>>8;
		
		Value = SSD2825_READ_AnyByte(0xfA);
		MIPI_READ_DATA[8] = Value;
		MIPI_READ_DATA[9] = Value>>8;

//		SSD2828_ENTER_LP_mode();
		SSD2828_VIDEO_MODE_HS();
		MDelay(100);
	}
	else
	{
		MDelay(200);
		goto xxxx1;
	}
	
	return Value;	
}

u8 READ_IC_Once(u8 cmd,u8 cmd1)  //¶Á2828´ÓLCD icÖÐÈ¡»ØÀ´µÄÖµ
{
	u16 Value=0;

xxxx1:
	STM32TOSSD2828_W_COM(0xb7);		//enable BTA
	STM32TOSSD2828_W_DATA_16BITS(0x0382);
	DelayMs(15);
	
	STM32TOSSD2828_W_COM(0xBB);		//enable BTA
	STM32TOSSD2828_W_DATA_16BITS(0x0005);
  DelayMs(15);
	STM32TOSSD2828_W_COM(0xc4);		//enable BTA
	STM32TOSSD2828_W_DATA_16BITS(0x0001);
  DelayMs(15);
	STM32TOSSD2828_W_COM(0xc1);		//Max. return packet is 255 bytes
	STM32TOSSD2828_W_DATA_16BITS(0x0002);
  DelayMs(15);
	STM32TOSSD2828_W_COM(0xc0);
	STM32TOSSD2828_W_DATA_16BITS(0x0001);

	DelayMs(15);

	TC358768_DCS_write_1A_1P(cmd,cmd1);
	DelayMs(15);

	STM32TOSSD2828_W_COM(0xd4);
	STM32TOSSD2828_W_DATA_16BITS(0x00FA);
	DelayMs(15);
	if(((READ_SSD2828_onebyte(0xc6))&0x01)==1)
	{
		DelayMs(15);
		Value = READ_SSD2828_onebyte(0xff);	
		MIPI_READ_DATA[0] = Value;
		MIPI_READ_DATA[1] = Value>>8;

//		Value = SSD2825_READ_AnyByte(0xfA);
//		MIPI_READ_DATA[2] = Value;
//		MIPI_READ_DATA[3] = Value>>8;

//		Value = SSD2825_READ_AnyByte(0xfA);
//		MIPI_READ_DATA[4] = Value;
//		MIPI_READ_DATA[5] = Value>>8;

//		Value = SSD2825_READ_AnyByte(0xfA);
//		MIPI_READ_DATA[6] = Value;
//		MIPI_READ_DATA[7] = Value>>8;
//		
//		Value = SSD2825_READ_AnyByte(0xfA);
//		MIPI_READ_DATA[8] = Value;
//		MIPI_READ_DATA[9] = Value>>8;

//		SSD2828_ENTER_LP_mode();
//		SSD2828_VIDEO_MODE_HS();
		DelayMs(15);
	}
	else
	{
		DelayMs(15);
		goto xxxx1;
	}
	
	return Value;	
}


u8 READ_IC_A(u8 cmd,u8 cmd1)  //¶Á2828´ÓLCD icÖÐÈ¡»ØÀ´µÄÖµ
{
	u16 Value=0;

	MIPI_SPI_Write(0x02,0x00,cmd1);
	DelayMs(10);

xxxx1:
	SSD2828_ENTER_LP_mode();

	MDelay(100);

	STM32TOSSD2828_W_COM(0xc4);		//enable BTA
	STM32TOSSD2828_W_DATA_16BITS(0x0001);

	STM32TOSSD2828_W_COM(0xc1);		//Max. return packet is 255 bytes
	STM32TOSSD2828_W_DATA_16BITS(0x000a);

	STM32TOSSD2828_W_COM(0xc0);
	STM32TOSSD2828_W_DATA_16BITS(0x0001);

	MDelay(200);

	SSD2828_ENTER_READ_LCDIC_mode();
	MDelay(200);

	MIPI_SPI_Write(0x02,cmd);
	DelayMs(5);
//	TC358768_DCS_write_1A_1P(cmd,cmd1);
	MDelay(200);

	if(((READ_SSD2828_onebyte(0xc6))&0x01)==1)
	{
		MDelay(200);
		Value = READ_SSD2828_onebyte(0xff);	
		MIPI_READ_DATA[0] = Value;
		MIPI_READ_DATA[1] = Value>>8;

		Value = SSD2825_READ_AnyByte(0xfA);
		MIPI_READ_DATA[2] = Value;
		MIPI_READ_DATA[3] = Value>>8;

		Value = SSD2825_READ_AnyByte(0xfA);
		MIPI_READ_DATA[4] = Value;
		MIPI_READ_DATA[5] = Value>>8;

		Value = SSD2825_READ_AnyByte(0xfA);
		MIPI_READ_DATA[6] = Value;
		MIPI_READ_DATA[7] = Value>>8;
		
		Value = SSD2825_READ_AnyByte(0xfA);
		MIPI_READ_DATA[8] = Value;
		MIPI_READ_DATA[9] = Value>>8;

		SSD2828_ENTER_LP_mode();
		MDelay(100);
	}
	else
	{
		MDelay(500);
		goto xxxx1;
	}
	
	return Value;	
}



/*******************************************************************************
* Function Name  : MIPI_SPI_Write
* Description    : MIPI_SPI_Write register value
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MIPI_Protocol_Write(void)
{
    uint k=0,j=1;
    uint Word_Count=(MIPI_Array[0]);
    

		STM32TOSSD2828_W_COM(0xBC);    
         STM32TOSSD2828_W_DATA_16BITS(Word_Count);  
         STM32TOSSD2828_W_COM(0xBF);     

         for(k=0;k<Word_Count;k++)
         {
            STM32TOSSD2828_W_DATA_8BITS(MIPI_Array[j]);
            j++;
         }  

}
 
void MIPI_SPI_Write(uint i,...)
{
    uint MIPI_Counter;
     va_list num_list;
     va_start(num_list,i);
     MIPI_Array[0]=i;
    for(MIPI_Counter=1;MIPI_Counter<=i;MIPI_Counter++)
    {
        MIPI_Array[MIPI_Counter]=(va_arg(num_list,uint));
    }
    MIPI_Protocol_Write();
    va_end(num_list);   
	DelayMs(1);
	    
 }


/*******************************************************************************
* Function Name  : READ_SSD2828_onebyte
* Description    : read SSD2825/8 register value
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//////////ÎªÁËÖ±½ÓÊ¹ÓÃFAEÌá¹©µÄ³õÊ¼»¯º¯Êý£¬ÖÆ×÷Ò»Ð©º¯ÊýÍ·À´¼æÈÝFAEµÄ³õÊ¼»¯¡£
void MDelay(u32 num)
{
	u32 stime=0;
	u8 i=0;

	for(stime=0;stime<num;stime++)
	{
		for(i=0;i<14;i++)
		{;}
	}
}




void TC358768_DCS_write_1A_1P(u8 cmd,u8 data)    //ÌØ¶¨LCD IC ³õÊ¼»¯ÓÃ
{
	STM32TOSSD2828_W_COM(0xBC);                          // Packet Size Control Register
    STM32TOSSD2828_W_DATA_16BITS(0x0002);                       //½ÓÏÂÀ´Ð´Èë2¸ö×Ö½ÚµÄÊý¾Ý
            STM32TOSSD2828_W_COM(0xbf);
    STM32TOSSD2828_W_DATA_8BITS(cmd);  					//
    STM32TOSSD2828_W_DATA_8BITS(data);	
	MDelay(20);
}

void TC358768_DCS_write_1A_0P(u8 cmd)     //ÌØ¶¨LCD IC ³õÊ¼»¯ÓÃ
{
	STM32TOSSD2828_W_COM(0xBC);                          // Packet Size Control Register
    STM32TOSSD2828_W_DATA_16BITS(0x0001);                       //½ÓÏÂÀ´Ð´Èë1¸ö×Ö½ÚµÄÊý¾Ý
            STM32TOSSD2828_W_COM(0xbf);
    STM32TOSSD2828_W_DATA_8BITS(cmd);  					//
//	MDelay(20);	
}




void SSD2828_101FD05(void)
{
	  TC358768_DCS_write_1A_0P(0x11);	
                     DelayMs(4);

    TC358768_DCS_write_1A_0P(0x29);
        MDelay(1);
}	
	





void SEND_IC_REG_OnlyFor045LF(u8 CMD)  //045LF ³õÊ¼»¯ÓÃ
{
	STM32TOSSD2828_W_COM(0xBC);
    STM32TOSSD2828_W_DATA_16BITS(0X01);
    STM32TOSSD2828_W_COM(0xbf);
    
	STM32TOSSD2828_W_DATA_8BITS(CMD);
    STM32TOSSD2828_W_DATA_8BITS(0x00);
}
 
void SEND_DATA_OnlyFor045LF(int sum,u8 data_buf[])
{
	int data_num=0;
	int temp=0;
	
	STM32TOSSD2828_W_COM(0xBC);
    STM32TOSSD2828_W_DATA_16BITS(sum);
    STM32TOSSD2828_W_COM(0xbf); 
	for(data_num=0;data_num<sum;data_num++)
	{
		temp = data_buf[data_num];
		STM32TOSSD2828_W_DATA_8BITS(temp);
	}
}

void SSD2828_MIPI_MODE_CHANGE_OnlyFor045LF(int CMD)
{
	STM32TOSSD2828_W_COM(0xb7);
	
	if(CMD == 0x39)
	{
		//Generic mode, HS video mode
		STM32TOSSD2828_W_DATA_16BITS(0x0752);
	}
	else if(CMD == 0x29)
	{
		//Generic mode, HS video mode
		STM32TOSSD2828_W_DATA_16BITS(0x0702);
	}
	else if(CMD == 0x05)
	{
		STM32TOSSD2828_W_DATA_16BITS(0x0352);
	}
	else if(CMD == 0x15)
	{
		STM32TOSSD2828_W_DATA_16BITS(0x0752);
	}
	else
	{
		STM32TOSSD2828_W_DATA_16BITS(0x0302);
	}
}
 



void enter_sleep_mode(void)
{
	SSD2828_ENTER_LP_mode(); //enter  LP mode
	DelayMs(20);
////////ic  display off  and  in sleep mode  setting

	STM32TOSSD2828_W_COM(0xBC);		               
	STM32TOSSD2828_W_DATA_16BITS(0x0001);						 
	STM32TOSSD2828_W_COM(0xbf);
	STM32TOSSD2828_W_DATA_8BITS(0x28);
	Delay(150);

	STM32TOSSD2828_W_COM(0xBC);		               
	STM32TOSSD2828_W_DATA_16BITS(0x0001);						 
	STM32TOSSD2828_W_COM(0xbf);
	STM32TOSSD2828_W_DATA_8BITS(0x10);
	DelayMs(50);
//2825 CHANGE MODE  IN SLEEP
	STM32TOSSD2828_W_COM(0xc0);		  //SSD2825 SOFT RESET
	STM32TOSSD2828_W_DATA_16BITS(0x0100);
	Delay(20);

	STM32TOSSD2828_W_COM(0xB6);		//Video mode and video pixel format
	STM32TOSSD2828_W_DATA_16BITS(0x0006);		//18bit 

	STM32TOSSD2828_W_COM(0xDE);		
	STM32TOSSD2828_W_DATA_16BITS(0x0001);		//MIPI lane select  

	STM32TOSSD2828_W_COM(0xd6);	
	STM32TOSSD2828_W_DATA_16BITS(0x0000);		//Color order and endianess 


	STM32TOSSD2828_W_COM(0xb8);		//VC register 
	STM32TOSSD2828_W_DATA_16BITS(0x0080);

	STM32TOSSD2828_W_COM(0xb9);		//PLL disable 
	STM32TOSSD2828_W_DATA_16BITS(0x0000);

	STM32TOSSD2828_W_COM(0xba);		//PLL setting 
	STM32TOSSD2828_W_DATA_16BITS(0x400A);				//Fout=(Fin/MS)*NF,MS=0,NF=8,Fout=(12/1)*8=96MHZ

	STM32TOSSD2828_W_COM(0xbb);		//LP clock divider 
	STM32TOSSD2828_W_DATA_16BITS(0x0001);				//LP clock=Fout/LPD/8,LPD=2,LP clock=96/2/8=6MHZ
	
	STM32TOSSD2828_W_COM(0xb9);		//PLL enable 
	STM32TOSSD2828_W_DATA_16BITS(0x0001);
	Delay(10);

	STM32TOSSD2828_W_COM(0xb8);		//VC register 
	STM32TOSSD2828_W_DATA_16BITS(0x0000);

	STM32TOSSD2828_W_COM(0xb7);		//Generic mode, HS video mode
	STM32TOSSD2828_W_DATA_16BITS(0x0314);		//		
	Delay(20);

	LCD_RST(0) ;  //Ä¬ÈÏÎª0£¬ÖÃ1ºó»áÓ°ÏìLCD VDD µçÑ¹
}



void exit_sleep_mode(void)
{	
		STM32_Init_SSD2828();
}

void reset_lcd(void)
{
	enter_sleep_mode();	 DelayMs(200);
	exit_sleep_mode();
}


void BEEP_ON(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  on
	DelayMs(150);
}

void BEEP_OFF(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  off
	DelayMs(150);
}

void BEEP_Dudu(void)
{
	u8 xxxxxxxx;
	
	for(xxxxxxxx=0;xxxxxxxx<3;xxxxxxxx++)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  on
		Delay(150);
		GPIO_ResetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  on
		Delay(150);
		
		GPIO_SetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  on
		Delay(150);
		GPIO_ResetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  on
		Delay(150);
		
		Delay(550);
	}
	
}

void BEEP_BIBI(void)
{
	u8 xxxxxxxx;
	
	for(xxxxxxxx=0;xxxxxxxx<2;xxxxxxxx++)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  on
		Delay(50);
		GPIO_ResetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  on
		Delay(50);
		
		GPIO_SetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  on
		Delay(50);
		GPIO_ResetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  on
		Delay(50);
		
		Delay(550);
	}
}

void BEEP_XiXi(void)
{
	u8 xx;	
	for(xx=15;xx>1;xx--)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  on
		Delay(xx*xx);
		GPIO_ResetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  on
		Delay(xx*xx);	
	}
}

void BEEP_HuHu(void)
{
	u8 xx;	
	for(xx=0;xx<15;xx++)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  on
		Delay(xx*xx);
		GPIO_ResetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  on
		Delay(xx*xx);	
	}
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
