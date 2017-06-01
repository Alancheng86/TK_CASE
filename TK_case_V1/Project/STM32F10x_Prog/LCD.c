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
#include "keyscan_adc.h"

extern void Delay(u32 nCount);
u8 MIPI_READ_DATA[10] = {0,0,0,0,0,0,0,0,0,0};
uint MIPI_Array[150];
u16 data = 0x400a;
extern u16 VOTP_SET;

extern u8 VCOM_register;
extern u8 ID_register;   
extern u8 ID1_VALUE;
extern u8 ID2_VALUE;
extern u8 ID3_VALUE;
extern u16 VCOMDC1;

/*******************************************************************************
在此處定義正確的電測畫面總數	 ，特殊畫面--》flick_otp\flicker_QC請正確定義其編號


*******************************************************************************/  

//------------------------------
    //畫面定義區
//------------------------------
u32 RED_bmp        =   0xff0000;
u32 GREEN_bmp      =   0x00ff00;
u32 BLUE_bmp       =   0x0000ff;
u32 WHITE_bmp      =   0xFFFFFF;
u32 BLACK_bmp      =   0x000000;
u32 GURY_45_bmp    =   0x2D2D2D;    //45灰
u32 GURY_60_bmp    =   0x3C3C3C;    //60灰
u32 GURY_128_bmp   =   0x7D7D7D;    //128灰









#define V480x800_SC7798






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

u16	 VPW=4;  //通常不需要调整
u16	 HPW=6;  //通常不需要调整

/////////////CLAN043LL  USE    inition/////
u8 PIC_NUM=10;       //電測畫面总数
u8 Flicker_OTP=1;    //OTP flicker 畫面編號
u8 Flicker_OQC=1;    //QC flicker  畫面編號
//u8 Flicker_OTP=9;    //OTP flicker 畫面編號
//u8 Flicker_OQC=10;    //QC flicker  畫面編號
//-------------------------------------------------------------------
//vcom阶次设定方式
void VCOM_set(u8 vcom)
{
//   MIPI_SPI_Write(0x04,0xb6,vcom,vcom,0X00);	   //set vcom	
}   
//ID值设定方式
void ID_set(void)
{
//   MIPI_SPI_Write(0x04,0xC3,0x55,0x44,0xF0);
}	
//--------------------------------------------------------------------

void PG_inition_lcd(void)
{  
//	MIPI_SPI_Write(0x01,0x01);
////	READ_IC(0x0F,0x00);
//	Delay(25);
//	///////FOR CLAN043LL USE
//	/////#password(Enable user command) 
//	MIPI_SPI_Write(0x04,0xB9,0xF1,0x08,0x00);
//	///////#VDDD
//	Delay(2);
//	MIPI_SPI_Write(2,0xBC,0x67);
//	///////#set DSI (2Lane)
//	Delay(2);
//	MIPI_SPI_Write(19,0xBA,0x31,0x00,0x44,0x25,0x91,0x0A,0x00,0x00,0xC2,0x34,0x00,0x00,0x04,0x02,0x1D,0xB9,0xEE,0x40);
/////////#set panel(Enable ESD finction)	
//	Delay(2);
//	MIPI_SPI_Write(4,0xC6,0x00,0x00,0xFF);
/////////#set panel(Forward,TN)	
//	Delay(2);
//	MIPI_SPI_Write(2,0xCC,0x0C);
/////////#set DISP(480RGB*800)	
//	Delay(2);
//	MIPI_SPI_Write(2,0xB2,0x23);
/////////#set (column-00),(1dot-01),(2dot-02),(4dot-03),(8dot-04)	
//	Delay(2);
//	MIPI_SPI_Write(2,0xB4,0x00);
/////////#set RGB 
//	Delay(2);
//	MIPI_SPI_Write(9,0xB3,0x01,0x00,0x06,0x06,0x10,0x0A,0x45,0x40);
/////////#set GIP
//	Delay(2);
//	MIPI_SPI_Write(52,0xE9,0x00,0x00,0x08,0x03,0x2F,0x89,0x6A,0x12,0x31,0x23,0x48,0x0C,0x89,0x6A,0x47,0x02,0x04,0x00,0x00,0x00,0x00,0x20,0x88,0x88,0x40,0x28,0x69,0x48,0x88,0x88,0x80,0x88,0x88,0x51,0x38,0x79,0x58,0x88,0x88,0x81,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
/////////#set GIP
//	Delay(2);
//	MIPI_SPI_Write(37,0xEA,0x88,0x88,0x37,0x59,0x18,0x18,0x88,0x88,0x85,0x88,0x88,0x26,0x49,0x08,0x08,0x88,0x88,0x84,0x30,0x00,0x00,0xFF,0x00,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00);
/////////#set power
//	Delay(2);
//	MIPI_SPI_Write(8,0xB1,0x22,0x1B,0x1B,0xB7,0x22,0x02,0xA8);
//	Delay(2);
//	MIPI_SPI_Write(8,0xB1,0x22,0x1B,0x1B,0xB7,0x22,0x02,0xA8);
/////////#set Source EQ
//	Delay(2);
//	MIPI_SPI_Write(8,0xE3,0x02,0x02,0x02,0x02);
/////////#Set Power_ext
//	Delay(2);
//	MIPI_SPI_Write(3,0xB8,0x03,0x22);
/////////#VREF
//	Delay(2);
//	MIPI_SPI_Write(3,0xB5,0x09,0x09);
/////////#set Bias
//	Delay(2);
//	MIPI_SPI_Write(6,0xC0,0x73,0x50,0x00,0x08,0x70);
/////////#set gamma
//	Delay(2);
//	MIPI_SPI_Write(35,0xE0,0x00,0x00,0x00,0x04,0x04,0x0A,0x18,0x2B,0x05,0x0C,0x11,0x16,0x18,0x16,0x16,0x15,0x19,0x00,0x00,0x00,0x04,0x04,0x0A,0x18,0x2C,0x05,0x0C,0x12,0x16,0x18,0x16,0x17,0x16,0x19);
/////////#DGC
//	Delay(2);
//	MIPI_SPI_Write(128,0xC1,0x01,0x00,0x03,0x0B,0x17,0x20,0x26,0x30,0x36,0x3F,0x48,0x50,0x58,0x5F,0x68,0x71,0x79,0x82,0x89,0x91,0x99,
//						//R01   02   03   04   05   06   07   08   09   10   11   12   13   14   15   16   17   18   19   20   21   
//						 0xA1,0xA9,0xB0,0xB8,0xC0,0xC8,0xD0,0xD6,0xDC,0xE4,0xED,0xF7,0xFF,0x00,0x00,0x00,0xC0,0x0A,0x80,0x30,0xE8,
//						//22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41   42   
//						 0x00,0x00,0x03,0x0B,0x17,0x20,0x27,0x30,0x37,0x40,0x48,0x50,0x58,0x5F,0x67,0x70,0x78,0x80,0x88,0x90,0x98,
//					   //G43   44   45   46   47   48   49   50   51   52   53   54   55   56   57   58   59   60   61   62   63   
//						 0xA0,0xA8,0xAF,0xB7,0xBF,0xC7,0xCF,0xD5,0xDC,0xE4,0xED,0xF7,0xFF,0x00,0x00,0x00,0xC0,0x0A,0x80,0x30,0xE8,
//						//64   65   66   67   68   69   70   71   72   73   74   75   76   77   78   79   80   81   82   83   84   
//						 0x00,0x00,0x03,0x0D,0x19,0x23,0x29,0x31,0x38,0x41,0x48,0x4F,0x57,0x5D,0x65,0x6C,0x74,0x7B,0x84,0x8B,0x94,
//					    //B85   86   87   88   89   90   91   92   93   94   95   96   97   98   99  100  101  102  103  104  105   
//						 0x9B,0xA4,0xAA,0xB4,0xB9,0xC3,0xCC,0xD6,0xDC,0xE4,0xED,0xF7,0xFF,0x00,0x00,0x00,0xC0,0x0A,0x80,0x30,0xE8,0x00
//						//106  107  108  109  110  111  112  113  114  115  116  117  118  119  120  121  122  123  124  125  126  127
//						);

//	Delay(20);
//		 
////////#setting sleep out	
//	MIPI_SPI_Write(0x01,0x11);
//	Delay(150);
////	READ_IC(0x0F,0x00);
/////////#setting Display	
//	MIPI_SPI_Write(0x01,0x29);
//	Delay(20);
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
* Description    : otp -> vcom     //此處需根據不同型號進行OTP流程修改
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void MTP(void)
{
//	///display off
//	MIPI_SPI_Write(0x01,0x28);
//	Delay(50);			 //delay 40ms

//	MIPI_SPI_Write(0x02,0xb7,0x5a);
//	MDelay(20);			   ////delay 1ms

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x0d,0x00);		   ///////////////set otp_index[8:0]
//	MDelay(20);			   ////delay 1ms

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x0d,0x00);		   ///////////////set otp_index[8:0]
//	MDelay(20);

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x0d,0x00);		   ///////////////////set otp_mask[7:0]
//	MDelay(20);

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x0d,0x00);			/////////////set otp address
//	MDelay(20);

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x0d,0x01);		   //////////////////set otp_prog = 1;
//	MDelay(45);		 		///DELAY 11ms

//	//////MTP   OVER ,,,OTP  Disable
//	MIPI_SPI_Write(0x02,0xb7,0xff);
//	MDelay(20);	

//	MIPI_SPI_Write(0x01,0x29);
//	Delay(50);			 //delay 40ms
}

/*******************************************************************************
* Function Name  : void MTP_ID(void)
* Description    : otp -> ID       //此處需根據不同型號進行OTP流程修改
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  

void MTP_ID(void)
{
		///display off
//	MIPI_SPI_Write(0x01,0x28);
//	Delay(50);			 //delay 40ms

//	MIPI_SPI_Write(0x02,0xb7,0x5a);
//	MDelay(20);			   ////delay 1ms

//	/////烧录otp的0x01的ID1
//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x01,0x00);		   ///////////////set otp_index[8:0]
//	MDelay(20);			   ////delay 1ms

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x01,0x00);		   ///////////////set otp_index[8:0]
//	MDelay(20);

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x01,0x00);		   ///////////////////set otp_mask[7:0]
//	MDelay(20);

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x01,0x00);			/////////////set otp address
//	MDelay(20);

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x01,0x01);		   //////////////////set otp_prog = 1;
//	MDelay(45);		 		///DELAY 11ms


//	/////烧录otp的0x01的ID2
//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x02,0x00);		   ///////////////set otp_index[8:0]
//	MDelay(20);			   ////delay 1ms

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x02,0x00);		   ///////////////set otp_index[8:0]
//	MDelay(20);

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x02,0x00);		   ///////////////////set otp_mask[7:0]
//	MDelay(20);

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x02,0x00);			/////////////set otp address
//	MDelay(20);

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x02,0x01);		   //////////////////set otp_prog = 1;
//	MDelay(45);		 		///DELAY 11ms


//	/////烧录otp的0x01的ID3
//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x03,0x00);		   ///////////////set otp_index[8:0]
//	MDelay(20);			   ////delay 1ms

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x03,0x00);		   ///////////////set otp_index[8:0]
//	MDelay(20);

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x03,0x00);		   ///////////////////set otp_mask[7:0]
//	MDelay(20);

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x03,0x00);			/////////////set otp address
//	MDelay(20);

//	MIPI_SPI_Write(0x06,0xb7,0x5a,0x00,0x00,0x03,0x01);		   //////////////////set otp_prog = 1;
//	MDelay(45);		 		///DELAY 11ms


//	//////MTP   OVER ,,,OTP  Disable
//	MIPI_SPI_Write(0x06,0xb7,0xff,0x00,0x00,0x00,0x00);
//	MDelay(20);	

//	MIPI_SPI_Write(0x01,0x29);
//	Delay(50);			 //delay 40ms
}

#endif


void STM_MINI_SET(void)
{
	LCD_CtrlLinesConfig();		Delay(10);  
	LCD_RST(0) ;  //默认为0，置1后会影响LCD VDD 电压
	Delay(20);  
	VDD_ADJ_EN(0); //默认为0，置1后会影响LCD VDD 电压
	SSD2828_SHUT(0);
	SSD2828_RST(1);
	SSD2828_CSX0(1);
	SSD2828_SCL(1);
	SSD2828_SDI(1);
	 		 
	GPIO_SetBits(GPIOA, GPIO_Pin_1);  //
	Delay(30);             // 程序多次复位时，保证复位时间
	

	T226_intion();


	VDD_ADJ_SET(305);			 //VDD_ADJ_SET 参数取值范围270~620，对应电压范围2.7V~6.2V; 
	VDD_ADJ_EN(1);				 //使能VDD输出

//	VDD_ADJ_EN(0);
	
	VDDIO_ADJ_SET(200);		  //VDDIO_ADJ_SET 参数取值范围150~360，对应电压范围1.5V~3.6V; 
//	VDDIO_ADJ_EN(1);			  //使能VDDIO输出
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);  //
		
		
	VOTP_EN(0);	
//	VOTP_ADJ_SET(490);			  //VOTP_ADJ_SET 参数取值范围150~2080，对应电压范围1.5V~14.1V; 
//	VOTP_EN(1);					  //使能VOTP输出

	Delay(30); 
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
	u16 VOTP_GET = 0;
	

	VHPW=(VPW<<8)+HPW;
	VHBP=((VBP)<<8)+(HBP);	
	VHFP=(VFP<<8)+HFP;

	LCD_CtrlLinesConfig();		Delay(10);  
//	LCD_RST(0) ;  //默认为0，置1后会影响LCD VDD 电压
	Delay(20);  
	VDD_ADJ_EN(0); //默认为0，置1后会影响LCD VDD 电压

	 		 

	VDD_ADJ_EN(1); //默认为0，置1后会影响LCD VDD 电压

	GPIO_ResetBits(GPIOA, GPIO_Pin_0);  //	    ok
	GPIO_SetBits(GPIOA, GPIO_Pin_0);  //		ok
	Delay(30);             // 程序多次复位时，保证复位时间
	
//	while(1);
//	T226_intion();
	
	VDD_ADJ_SET(305);			 //VDD_ADJ_SET 参数取值范围270~620，对应电压范围2.7V~6.2V; 
	VDD_ADJ_EN(1);				 //使能VDD输出

	VOTP_ADJ_SET(0);			  //VOTP_ADJ_SET 参数取值范围150~2080，对应电压范围1.5V~14.1V; 
	VOTP_EN(1);					  //使能VOTP输出
//	VOTP_GET = ADC_VOL_value();
//	while(( VOTP_GET<  VOTP_SET*90/100) || (VOTP_GET > VOTP_SET*110/100))
//	{
//		VOTP_ADJ_SET(VOTP_SET);			  //VOTP_ADJ_SET 参数取值范围150~2080，对应电压范围1.5V~14.1V; 
//		VOTP_EN(1);					  //使能VOTP输出	
//		VOTP_GET = ADC_VOL_value();
//	}
//	Delay(30); 

//=============================开机检测是否大电流==========================
 /* while(Measure_IDD_for_start(mA) > 11000)				////if IDD > 110mA,,,,,power  off
	{
		BEEP_Dudu();	 /////////beep  on
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);  // 关闭VDD电压	
		VOTP_EN(0);		
		BEEP_ON();	 /////////beep  on
		while(1);
	}		
	*/


//  //-------------------------------------------------------------------
//	PG_inition_lcd();    //初始化IC
	Delay(5); 


}


void SSD2828_VIDEO_MODE_HS(void)  //不同分辨率的LCD需要不同的传输速度，需做针对调整
{	
//	STM32TOSSD2828_W_COM(0xb8);		//VC register 
//	STM32TOSSD2828_W_DATA_16BITS(0x0000);
//
//	STM32TOSSD2828_W_COM(0xb9);		//PLL disable 
//	STM32TOSSD2828_W_DATA_16BITS(0x0000);
//	Delay(5);
//
//	STM32TOSSD2828_W_COM(0xba);		//PLL setting 
//	STM32TOSSD2828_W_DATA_16BITS(data);
//
//	STM32TOSSD2828_W_COM(0xbb);		//LP clock divider 
//	STM32TOSSD2828_W_DATA_16BITS(0x000A);			
////	Delay(5);
//
//	STM32TOSSD2828_W_COM(0xb9);		//PLL enable 
//	STM32TOSSD2828_W_DATA_16BITS(0xc001);		//8分频	，SYS_CLK输出24/8=3MHZ
//	Delay(5);
	
	STM32TOSSD2828_W_COM(0xb7);		//Generic mode, HS video mode
	STM32TOSSD2828_W_DATA_16BITS(0x070b);		 //HS video +  short packet 
	Delay(5);

}

void SSD2828_ENTER_LP_mode(void)  //LP模式速度通用
{
//	STM32TOSSD2828_W_COM(0xb8);		//VC register 
//	STM32TOSSD2828_W_DATA_16BITS(0x0000);
//
//	STM32TOSSD2828_W_COM(0xb9);		//PLL disable 
//	STM32TOSSD2828_W_DATA_16BITS(0x0000);
//	Delay(5);
//
//	STM32TOSSD2828_W_COM(0xba);		//PLL setting 
//	STM32TOSSD2828_W_DATA_16BITS(data);
//
//	STM32TOSSD2828_W_COM(0xbb);		//LP clock divider 
//	STM32TOSSD2828_W_DATA_16BITS(0x000A);			
////	Delay(5);
//
//	STM32TOSSD2828_W_COM(0xb9);		//PLL enable 
//	STM32TOSSD2828_W_DATA_16BITS(0xc001);		//8分频	，SYS_CLK输出24/8=3MHZ
//	Delay(5);

	STM32TOSSD2828_W_COM(0xb7);		//DCS mode, LP mode
	STM32TOSSD2828_W_DATA_16BITS(0x0752);	 
	Delay(5);
}
void SSD2828_ENTER_READ_LCDIC_mode(void)
{
	STM32TOSSD2828_W_COM(0xb7);
	STM32TOSSD2828_W_DATA_16BITS(0x03c2);	
	Delay(5);
}

u8 READ_IC(u8 cmd,u8 cmd1)  //读2828从LCD ic中取回来的值
{
	u16 Value=0;



xxxx1:
	SSD2828_ENTER_LP_mode();

	Delay(100);

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

u8 READ_IC_A(u8 cmd,u8 cmd1)  //读2828从LCD ic中取回来的值
{
	u16 Value=0;

	MIPI_SPI_Write(0x02,0x00,cmd1);
	Delay(10);

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
	Delay(5);
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
	Delay(1);
	    
 }


/*******************************************************************************
* Function Name  : READ_SSD2828_onebyte
* Description    : read SSD2825/8 register value
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//////////为了直接使用FAE提供的初始化函数，制作一些函数头来兼容FAE的初始化。
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




void TC358768_DCS_write_1A_1P(u8 cmd,u8 data)    //特定LCD IC 初始化用
{
	STM32TOSSD2828_W_COM(0xBC);                          // Packet Size Control Register
    STM32TOSSD2828_W_DATA_16BITS(0x0002);                       //接下来写入2个字节的数据
            STM32TOSSD2828_W_COM(0xbf);
    STM32TOSSD2828_W_DATA_8BITS(cmd);  					//
    STM32TOSSD2828_W_DATA_8BITS(data);	
	MDelay(20);
}

void TC358768_DCS_write_1A_0P(u8 cmd)     //特定LCD IC 初始化用
{
	STM32TOSSD2828_W_COM(0xBC);                          // Packet Size Control Register
    STM32TOSSD2828_W_DATA_16BITS(0x0001);                       //接下来写入1个字节的数据
            STM32TOSSD2828_W_COM(0xbf);
    STM32TOSSD2828_W_DATA_8BITS(cmd);  					//
//	MDelay(20);	
}




void SSD2828_101FD05(void)
{
	  TC358768_DCS_write_1A_0P(0x11);	
                     Delay(4);

    TC358768_DCS_write_1A_0P(0x29);
        MDelay(1);
}	
	





void SEND_IC_REG_OnlyFor045LF(u8 CMD)  //045LF 初始化用
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
	Delay(20);
////////ic  display off  and  in sleep mode  setting

//	STM32TOSSD2828_W_COM(0xBC);		               
//	STM32TOSSD2828_W_DATA_16BITS(0x0001);						 
//	STM32TOSSD2828_W_COM(0xbf);
//	STM32TOSSD2828_W_DATA_8BITS(0x28);
//	Delay(150);

	STM32TOSSD2828_W_COM(0xBC);		               
	STM32TOSSD2828_W_DATA_16BITS(0x0001);						 
	STM32TOSSD2828_W_COM(0xbf);
	STM32TOSSD2828_W_DATA_8BITS(0x10);
	Delay(50);
//2825 CHANGE MODE  IN SLEEP
//	STM32TOSSD2828_W_COM(0xc0);		  //SSD2825 SOFT RESET
//	STM32TOSSD2828_W_DATA_16BITS(0x0100);
//	Delay(20);

//	STM32TOSSD2828_W_COM(0xB6);		//Video mode and video pixel format
//	STM32TOSSD2828_W_DATA_16BITS(0x0006);		//18bit 

//	STM32TOSSD2828_W_COM(0xDE);		
//	STM32TOSSD2828_W_DATA_16BITS(0x0001);		//MIPI lane select  

//	STM32TOSSD2828_W_COM(0xd6);	
//	STM32TOSSD2828_W_DATA_16BITS(0x0000);		//Color order and endianess 


//	STM32TOSSD2828_W_COM(0xb8);		//VC register 
//	STM32TOSSD2828_W_DATA_16BITS(0x0080);

//	STM32TOSSD2828_W_COM(0xb9);		//PLL disable 
//	STM32TOSSD2828_W_DATA_16BITS(0x0000);

//	STM32TOSSD2828_W_COM(0xba);		//PLL setting 
//	STM32TOSSD2828_W_DATA_16BITS(0x400A);				//Fout=(Fin/MS)*NF,MS=0,NF=8,Fout=(12/1)*8=96MHZ

//	STM32TOSSD2828_W_COM(0xbb);		//LP clock divider 
//	STM32TOSSD2828_W_DATA_16BITS(0x0001);				//LP clock=Fout/LPD/8,LPD=2,LP clock=96/2/8=6MHZ
//	
//	STM32TOSSD2828_W_COM(0xb9);		//PLL enable 
//	STM32TOSSD2828_W_DATA_16BITS(0x0001);
//	Delay(10);

//	STM32TOSSD2828_W_COM(0xb8);		//VC register 
//	STM32TOSSD2828_W_DATA_16BITS(0x0000);

//	STM32TOSSD2828_W_COM(0xb7);		//Generic mode, HS video mode
//	STM32TOSSD2828_W_DATA_16BITS(0x0314);		//		
//	Delay(20);

	LCD_RST(0) ;  //默认为0，置1后会影响LCD VDD 电压
}



void exit_sleep_mode(void)
{	
		STM32_Init_SSD2828();
}

void reset_lcd(void)
{
	exit_sleep_mode();
}


void BEEP_ON(void)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_0); 	 /////////beep  on
	Delay(150);
}

void BEEP_OFF(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_0); 	 /////////beep  off
	Delay(150);
}

void BEEP_Dudu(void)
{
	u8 xxxxxxxx;
	
	for(xxxxxxxx=0;xxxxxxxx<3;xxxxxxxx++)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_0); 	 /////////beep  on
		Delay(150);
		GPIO_ResetBits(GPIOC, GPIO_Pin_0); 	 /////////beep  on
		Delay(150);
		
		GPIO_SetBits(GPIOC, GPIO_Pin_0); 	 /////////beep  on
		Delay(150);
		GPIO_ResetBits(GPIOC, GPIO_Pin_0); 	 /////////beep  on
		Delay(150);
		
		Delay(550);
	}
	
}

void BEEP_BIBI(void)
{
	u8 xxxxxxxx;
	
	for(xxxxxxxx=0;xxxxxxxx<2;xxxxxxxx++)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_0); 	 /////////beep  on
		Delay(50);
		GPIO_ResetBits(GPIOC, GPIO_Pin_0); 	 /////////beep  on
		Delay(50);
		
		GPIO_SetBits(GPIOC, GPIO_Pin_0); 	 /////////beep  on
		Delay(50);
		GPIO_ResetBits(GPIOC, GPIO_Pin_0); 	 /////////beep  on
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

void BACK_LIGHT_OFF(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
	                                         | RCC_APB2Periph_AFIO, ENABLE); 
	//#define GPIO_Remap_SWJ_JTAGDisable ((u32)0x00300200)  /* JTAG-DP Disabled and SW-DP Enabled */ 禁止JTAG,以空出PB3,PB4
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//禁止jtag，以空出PB3,PB4,PA15
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//GPIO最高速度50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);  // 
}	

void BACK_LIGHT_ON(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_3);  // 
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
