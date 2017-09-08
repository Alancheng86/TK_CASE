 /******************** (C) COPYRIGHT 2010 KSGIANTLUS ********************
* File Name          : lcd.c
* Author             : JASON
* Version            : V1.0
* Date               : 2010.01.20
*************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "lcd.h"
#include  "lib.h"
#include "tsliic.h"
#include "OTP.h"
#include "iics.h"
#include "store.h"
#include "math.h"
extern void Delay(u32 nCount);
extern void DelayKEY (u32 k); //按住，画面暂停
extern u8 MIPI_READ_DATA[10];
extern void DelayMs(__IO uint32_t nTime);
extern void ShowData(char *p,u16 dat);
extern u8 Flicker_OTP;
void ShowData_hex(char *p,u16 dat);
extern void SSD2828_inition_lcd();
extern void  BACK_LIGHT_OFF(void);	
extern void  BACK_LIGHT_ON(void);	
extern void Measure_Brightness_Init(void);
/////////////////////add in OTP.C----------------------------
int Sensor_error_dot[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int Sensor_data_ok = 0;
#define Sensor_Err1 3000		////定义光感测量flicker的max值
#define Sensor_Err2	2			////定义光感测量flicker的min值。



u8 OTP_VALUE1 = 0;
u8 OTP_VALUE2 = 0;
u8 OTP_TIMES = 0;

u8 Gamma_VALUE1=0;	
u8 Gamma_VALUE2=0;
u8 Gamma_VALUE3=0;
u8 Gamma_VALUE4=0;	
	
u8 ID_OK = 1;   //默認ID未燒

/*******************************************************************************
在此處定義


*******************************************************************************/  


u8 ID1_VALUE = 0X00;
u8 ID2_VALUE = 0X00;
u8 ID3_VALUE = 0X00;
u8 ID1_READ,ID2_READ,ID3_READ;
u32 ID_VAL=0;
u32 ID_SHOW=0;
//////------------060WD42默认ID值--------------------
u8 ID1_default = 0X00;
u8 ID2_default = 0X00;
u8 ID3_default = 0X00;


u8 VCOM_register=0x80;   //设置vcom寄存器地址
u8 VCOM_Read_reg=0xC5;   //读取vcom寄存器地址
u8 VCOM_Time_reg=0xe8;
//u8 ID_register = 0xC3;    //设置ID寄存器地址 
u8 ID1_register= 0xDA;   
u8 ID2_register= 0xDB;
u8 ID3_register= 0xDC;

u8 Gamma_reg1=0xA0;		//Positive_Gamma_first_reg=0xA0;
u8 Gamma_reg2=0xB3;		//Positive_Gamma_end_reg=0xB3;
	
u8 Gamma_reg3=0xC0;		//Negitive_Gamma_first_reg=0xC0;
u8 Gamma_reg4=0xD3;		//Negitive_Gamam_end_reg=0xD3;

u8 Gamma_default1 = 0x09;
u8 Gamma_default2 = 0x36;
u8 Gamma_default3 = 0x09;
u8 Gamma_default4 = 0x36;

u8 STEP=30;   //采样数组上限
u8 VCOMDH = 0x8E;   //手动调整起始地址
u8 VCOMDC = 0x7C;	  //自动模式起始地址
u8 VCOM_Temp=0x8E;    //保存中间值	 

u16 DATA_SUM1[30],DATA_SUMY[30],SURE[30];
u8 MTP_OVER=0; //该型号56H寄存器定义VCOM为默认值还是烧录值，故，在烧录前使用默认值初始化，
								//在烧录完成重新启动屏幕前使用，烧录值初始化     0--表示未作烧录动作   1--表示烧录动作完成
//======================================END define===================================

u16 count1=0,count2=0;
u16 VCOMDC1=0x0000;


void Soft_reset(void)
{
	LCD_RST(1) ;
	Delay(20);
	LCD_RST(0);//////////硬件复位
	Delay(20);
	LCD_RST(1) ;
	
//	Delay(200);  ////////延时200毫秒
	Delay(300);

////////////=============================================================================
	SSD2828_ENTER_LP_mode();
	Delay(20);
	
	
	SSD2828_inition_lcd();
	Delay(20);
	
	
	SSD2828_VIDEO_MODE_HS();
	Delay(20);	
}

void Enter_sleep_begin(void)
{
	SSD2828_ENTER_LP_mode(); //enter  LP mode
	Delay(20);
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
	Delay(150);
	
	STM32TOSSD2828_W_COM(0xb7);		//Generic mode, HS video mode
	STM32TOSSD2828_W_DATA_16BITS(0x0314);		//
	Delay(20);	
}

/*******************************************************************************
* Function Name  : 一些提示函數，可根據實際需要調整顯示位置
* Description    : 函數尾部請務必加入適當延時
* Input          : 字體放大倍數 ,X坐標，Y坐標 ，文字內容
* Output         : None
* Return         : None
*******************************************************************************/  
void Gamma_OK(void)
{
	  ShowTxt("48,20,299, ");	  			DelayMs(150);
		ShowTxt("48,20,1099,Gamma = ");	DelayMs(150);
  	ShowTxt("48,260,1099,OK");	    DelayMs(150);
}	
void Gamma_NG(void)
{
	  ShowTxt("48,20,299, ");	  			DelayMs(150);
		ShowTxt("48,200,1099,Gamma = ");DelayMs(150);
  	ShowTxt("48,470,1099,NG");	    DelayMs(150);
}	
void OTP_ID_OK(void)
{
		ShowTxt("60,20,1180,ID = ");	  DelayMs(100);
		ShowTxt("60,160,1180,OK");	    DelayMs(100);
}
void OTP_ID_NG(void)
{
	  ShowTxt("48,20,299, ");	  			DelayMs(150);
		ShowTxt("48,350,1100,ID = ");	  DelayMs(150);
  	ShowTxt("48,470,1100,NG");	    DelayMs(150);
}

void OTP_ERROR_1(void)
{
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除
		ShowTxt("60,20,100,OTP 烧录校验错，可能烧录次数超限");	  Delay(100);
		while(1)
		{BEEP_Dudu();}
}

void OTP_ERROR_2(void)
{
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除
	{
		ShowTxt("60,20,100,OTP 数值超限，请重试或检查光感"); 	 Delay(100);
		while(1)
		{BEEP_Dudu();}
	}
}

void OTP_ERROR_3(void)
{
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除
	{
		ShowTxt("60,20,800, ");			    DelayMs(80);
		ShowTxt("60,20,100,OTP 数值异常,请正确放置光感"); 		Delay(100);
		while(1)
		{BEEP_Dudu();}
	}
}
void OTP_ERROR_4(void)
{
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除
	
	ShowTxt("60,20,700, ");			    DelayMs(80);
	ShowTxt("60,20,700,採樣值有異常");Delay(20);
	while(1)
	{BEEP_Dudu();}
}

void OTP_NO(void)
{	 
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除
	ShowTxt("60,20,99,  ");		   DelayMs(210);
	ShowTxt("60,20,100, OTP未烧入！");	  Delay(100);
	while(1);
}

void OTP_OK(void)
{	 
	SSD2828_VIDEO_MODE_HS();DelayMs(15);   
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除	
	ShowTxt("60,20,299, ");		   DelayMs(210);
	ShowTxt("60,20,100,OTP  OK！");	      Delay(100);
}

void OTP_adjust_result(u16 senser_val)
{	
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除
	ShowTxt("48,20,299, ");		   DelayMs(80);
	ShowTxt("32,0,50, sener值");	  				 DelayMs(150);
	ShowData("32,150,50",senser_val) ;		   Delay(150);
	ShowTxt("32,0,0, VCOM值");	  				   DelayMs(150);
	ShowData_hex("32,150,0",VCOMDC1) ;		   Delay(150);
}

void Running_water_result()
{	
	u32 ID_VAL_OTP=0;
    //===============================	
  ID_VAL=SLAVE_EEPROM_COUNT_NUM();	//存入内存模块的值
//================================
	ID_VAL_OTP=ID_VAL+0xC00000;
//	060WA42客户规定：ID1:C0~FFID2:00~FFID3:00~FF
	ID1_VALUE = ID_VAL_OTP>>16;      DelayMs(5); 
	ID2_VALUE = ID_VAL_OTP>>8 ;      DelayMs(5); 
	ID3_VALUE = ID_VAL_OTP ;         DelayMs(5); 
	ShowTxt("32,0,100,ID_VALUE");	  		 DelayMs(120);   //提示本次即将烧入的ID值
//	ShowData("32,200,150",ID_VAL) ;		     DelayMs(150);
	ShowData_hex("32,180,100",ID1_VALUE) ;		   Delay(120);	
	ShowData_hex("32,280,100",ID2_VALUE) ;		   Delay(120);
	ShowData_hex("32,380,100",ID3_VALUE) ;		   Delay(120);
}
//-------------------------------显示区结束---------------------------------------


/*******************************************************************************
* Function Name  : void VCOM_CHECK(u8 vcom)   
* Description    : vcom 烧入复判
* Input          : vcom
* Output         : None
* Return         : None
*******************************************************************************/  
void VCOM_READ(void)
{
	  MIPI_SPI_Write( 2,  0x51, 0x03);  //该型号回读之前需切换page
    
//     MIPI_SPI_Write( 2,  0x00, 0x00); 
    READ_IC(VCOM_register,0x00);   //回读ID3值
	OTP_VALUE1 =   MIPI_READ_DATA[0];
	OTP_VALUE2 =   MIPI_READ_DATA[1];
//	VCOM_TIMES_Check(VCOM_Read_reg);
//-----------------------------------------------------------------------------------	
//	READ_IC(ID_register,  0x00);   //回读ID值
//	SSD2828_VIDEO_MODE_HS();		  DelayMs(150);
    
//	  VCOM_TIMES_Check();           //VCOM times 
}

void VCOM_CHECK(u8 vcom)     
{	  
		if(OTP_VALUE1 != vcom)
		{	
			SSD2828_VIDEO_MODE_HS();
	    DelayMs(10);
			Allcolor(1,0xffffff);		   DelayMs(650);		//////WHITE		
			OTP_ERROR_1();   
			while(1)
			{BEEP_Dudu();}		
		}	
//		else			
//		{	
//			OTP_OK();
//		}
}
/*******************************************************************************
* Function Name  : u8 VCOM_TIMES_Check(void) 
* Description    : VCOM燒入次數判斷
* Input          : None
* Output         : None
* Return         : 
*******************************************************************************/  
u8 VCOM_TIMES_Check(void)   //OTM1284A专用
{
	MIPI_SPI_Write( 4, 0xFF, 0x98, 0x81, 0x01); //4
	READ_IC(VCOM_Time_reg,0x00);
	OTP_TIMES= MIPI_READ_DATA[0];
}


void Gamma_READ(void)
{
	MIPI_SPI_Write( 4, 0xFF, 0x98, 0x81, 0x01);  //该型号回读之前需切换page
  DelayMs(5);	
//-------------------------------	
	READ_IC(Gamma_reg1,0x00);
	Gamma_VALUE1=MIPI_READ_DATA[0];

	READ_IC(Gamma_reg2,0x00);
	Gamma_VALUE2=MIPI_READ_DATA[0];

	READ_IC(Gamma_reg3,0x00);
	Gamma_VALUE3=MIPI_READ_DATA[0];
	
	READ_IC(Gamma_reg4,0x00);
	Gamma_VALUE4=MIPI_READ_DATA[0];
}

/*******************************************************************************
* Function Name  : u8 Gamma_Check(void)
* Description    : Gamma燒入判斷
* Input          : None
* Output         : None
* Return         : 
*******************************************************************************/  
u8 Gamma_Check(void)
{
//-------------------------------		
  if((Gamma_VALUE1!=Gamma_default1)||(Gamma_VALUE2!=Gamma_default2)||(Gamma_VALUE3!=Gamma_default3)||(Gamma_VALUE4!=Gamma_default4))
	{
		  SSD2828_VIDEO_MODE_HS();
	    DelayMs(10);
			Allcolor(1,0xffffff);		   DelayMs(650);		//////WHITE		
			Gamma_NG();
			while(1)
			{BEEP_Dudu();}	
	}
//	else
//	{
//	    Gamma_OK();
//	}
}

void ID_READ(void)
{
//-----------------------------------------------------------------------------------		
//回读方式1	
//	READ_IC(ID_register,  0x00);   //回读ID值
//-----------------------------------------------------------------------------------		
//回读方式2	
	MIPI_SPI_Write( 4, 0xFF, 0x98, 0x81, 0x00);
	DelayMs(5);	
	READ_IC(ID1_register,0x00);   //回读ID1值        //orise //ili
	ID1_READ =   MIPI_READ_DATA[0];
  
	READ_IC(ID2_register,0x00);   //回读ID2值
	ID2_READ =   MIPI_READ_DATA[0];

	READ_IC(ID3_register,0x00);   //回读ID3值
	ID3_READ =   MIPI_READ_DATA[0];                 
	//ID烧入成功后，不必反复再烧
	if((ID1_READ != ID1_default)||(ID2_READ != ID2_default)||(ID3_READ  != ID3_default))
	{
		ID_OK = 0;   //若ID已燒，返回0 
	}
	else
	{	
		ID_OK = 1;
	}	
//-----------------------------------------------------------------------------------	
}
/*******************************************************************************
* Function Name  : u8 ID_CHECK(void)
* Description    : ID燒入判斷
* Input          : None
* Output         : None
* Return         : ID_OK
*******************************************************************************/  

u8 ID_CHECK(void)
{ 

	if((ID1_READ == ID1_default)&&(ID2_READ == ID2_default)&&(ID3_READ == ID3_default))
	{
			SSD2828_VIDEO_MODE_HS();
	    DelayMs(10);
			Allcolor(1,0xffffff);		   DelayMs(650);		//////WHITE		
		  OTP_ID_NG();
			while(1)
			{BEEP_Dudu();}	
	}
//	return ID_OK;
//------------------------------------------------------------------------------------	
}

                           

/*******************************************************************************
* Function Name  : void SENSOR_Check(u8 Device_N)
* Description    : 检查光感是否工作正常
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  

void SENSOR_Check(u8 Device_N)
{
	u16 temp1 =0;

	temp1 = sensor_check(Device_N);
	if(temp1 == 0)	  
	{	
//		Delay(1);	
		;
	}

	else
	{
		while(1)
		{
			if(Device_N == S3_addr)	  {		ShowTxt("48,100,80,S3  ");		}
			if(Device_N == S2_addr)	  {		ShowTxt("48,140,80,S2  ");		}
			if(Device_N == S1_addr)	  {		ShowTxt("48,180,80,S1  ");		}
			DelayMs(200); 				            
			ShowTxt("48,20,300,sensor  error");  DelayMs(150);
					//显示sensor error，警示光感失灵，暂停在此处。
			while(1);
		}
	}	
}


void I2C_MEMSURE1(int kk  , int vcomdcdata)
{
	int i=0,tep1=0,tep2=0,flag=0; //2014.1.27  HFZ
  int error_times=0;
	int sss=0,yyy=0,aaa=0,temp;	  //temp 为加的阶数
	
	for (yyy=0;yyy<kk;yyy++)
	{SURE[sss]=vcomdcdata+aaa;
	
	STM32TOSSD2828_W_COM(0xb7);		//DCS mode, LP mode
	STM32TOSSD2828_W_DATA_16BITS(0x0750);	 
	Delay(20);

	////# VCOM Setting		
	VCOM_set(SURE[sss]);								   ////set vcom
	Delay(12);

	STM32TOSSD2828_W_COM(0xb7);		//Generic mode, HS video mode
	STM32TOSSD2828_W_DATA_16BITS(0x070B);
	Delay(145);

	SENSOR_Check(0x52);			/////S3		
	SENSOR_Check(0x72);			/////S2	
//SENSOR_Check(0x92);			/////S1

RE_Sample:
	DATA_SUM1[count1] =	GET_FLICKER();	 ///get flicker value
/////////對光感採集的flicker數據進行初步判斷，---------2014-08-29   alan
//	if((DATA_SUM1[count1] >= Sensor_Err1)||(DATA_SUM1[count1] < Sensor_Err2 ))
//	{
//		OTP_ERROR_3();	
//	}
//	Delay(1);
///////////*------新增一部分光感ID检验功能，一旦光感失效，即停止在此处。-----*/
  //------2014.1.27--HFZ-START-------
		if (flag==0)                             //判断是否找到了最低点
			{
				tep2=tep1=DATA_SUM1[count1];
				flag=1;
			}
		else
			{ 
				tep1=DATA_SUM1[count1];  //第二次采样的值
//-------------2015年2月7日--cf--------------------------				
				if((tep1>10000)&&(error_times<3))
				{
					 error_times++;    //且连续三次出现异常值，就认为光感没有放对位置    
				   goto RE_Sample;  //如果单次采样值在10000以上  ， 直接认为是异常值，并重测
				}	
				else
				{
					if(tep1<10000)    //直到采集到合理值才重新归零
					{error_times=0;}
				}
//-------------------------------------------------------------------				
					if(tep1<tep2)
					{
							tep2=tep1;
							 flag=1;  //此处flag再次赋值是为了在一旦再次出现更小值的情况，清除flag计数 令后续可以找到真正的最小值
					}
					else   // if (tep1>tep2)       //如果找到了最低点，则继续寻找两次(flag=1~3)
					  	flag++;
					
					if ((flag==3) & (tep2<170))    //判断是否是真的最低点，再确认是否小于预定值，如果是则停止寻找以节约时间。
					{
						for(i=(count1+1);i<STEP;i++)
							DATA_SUM1[i]=500;                     //填充剩下的数组，否则后续处理会出错
					
							yyy=kk;     //改变判断条件，使本函数提前结束
					}
			}
		  //
	 //------2014.1.27----HFZ--END------	
				
				
				
	if(DATA_SUM1[count1] <= 225)
	{temp=1; }
	else if((DATA_SUM1[count1]>225)&&(DATA_SUM1[count1] < 300))
	{temp=2; }
	else if((DATA_SUM1[count1]>=300)&&(DATA_SUM1[count1] < 430))
	{temp=3; }
	else if((DATA_SUM1[count1]>=430)&&(DATA_SUM1[count1] < 580))
	{temp=5;}	
	else
	{temp=7;}

	count1=count1+1;
	aaa=aaa+temp;
	sss=sss+1;
	}
}
void I2C_DATAL_COMPAREX(void)
{
	u16 senser_value=0;
	u16 mm_KEYB10 = 1 ;
  
	GET_FLICKER();
	I2C_MEMSURE1(STEP , VCOMDC);
	I2C_DATAL_COMPARE();
	count1=0;
	
/*在最小值附近停下来。。。后续开启OTP功能即可。。*/
		VCOMDC1 = SURE[count2];				 //挑选出最好的VCOM的阶次，下CODE，看效果。
	
		STM32TOSSD2828_W_COM(0xb7);		//DCS mode, LP mode
		STM32TOSSD2828_W_DATA_16BITS(0x0752);
		Delay(20);
			
//	  MIPI_SPI_Write(0x04,0xb6,VCOMDC1,VCOMDC1,0X00);		 ////set vcom	
    VCOM_set(VCOMDC1);
		Delay(20);

		STM32TOSSD2828_W_COM(0xb7);		//Generic mode, HS video mode
		STM32TOSSD2828_W_DATA_16BITS(0x070B);
	  Delay(20);
//=========================================================================  
		senser_value = DATA_SUM1[count2];		////挑选出最小的FLICKER值，显示出来。
		
		OTP_adjust_result(senser_value);
		
///////新增自动OTP数据的判读，判断光感采集到的微笑曲线是否合格，减少OTP误判。。。。
	while(Sensor_check())
	{
		OTP_ERROR_4();	
	}
////	while(1);
	if(ID_OK == 1)
	{	
		ID_Model_Lifetimes_CHECK();	//检查模块是否能够正常写入
		Running_water_result();     //获取流水ID信息   
		DelayMs(850);
	}				
	  
//==========================================================================		
//		VCOMDH=VCOMDC1;    //在光感调整到最佳之后进入手动验证
//		KEY_adjust();


	////////內部測試用，暫停住畫面，以便進行flicker數據收集。。
//	mm_KEYB10 = KEYB10;
//	while(mm_KEYB10 != 0)
//	{
//		mm_KEYB10 = KEYB10;	
//		Delay(20);
//	}
}
void I2C_DATAL_COMPARE(void)
{
		int yy=1,zz=0;
	while(yy<count1 && zz<count1)
	{
		if(	DATA_SUM1[zz]>DATA_SUM1[yy])
		{
			count2=yy;zz=zz+1;	
		}
		else
		{
			count2=zz;yy=yy+1;
		}
	}	
}

/*******************************************************************************
* Function Name  : void Auto_otp(void)
* Description    : 自动模式
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void Auto_otp(void)
{
	//u8 sdcap=0;//该型号灰度  ：  480+-25

	I2C_DATAL_COMPAREX();
	DelayKEY(1);
	
	if(DATA_SUM1[count2] <= 145)			 ///for check OTP is OK or NG....   
	{
			if(DATA_SUM1[count2] >= 1)
			{
				SSD2828_ENTER_LP_mode(); //enter  LP mode				
				Delay(6);
				{			
					MTP(); 
					DelayMs(100);
					MTP_OVER=1;
					reset_lcd();				
					DelayMs(100);
				}	
				  showbmp (Flicker_OTP);	    DelayMs(120);
//				  Init_External_EEPROM();     DelayMs(5);
//--------------------------OTP _id值燒入复判--------------------------------
//					if(ID_OK == 1)
//					{		
//						ID_READ();
//						ID_CHECK();						
//						DelayMs(2);
//						SAVE_SLAVE_EEPROM_COUNT_NUM(ID_VAL);  //若ID烧入OK，更新存储模块信息！
//			      DelayMs(50); 	
//          }						
//--------------------------OTP _VCOM值燒入复判--------------------------------	
          VCOM_READ();					  //回读再复判
      		VCOM_CHECK(VCOMDC1);
//---------------------------OTP_Gamma值复判---------------------------------	
//					Gamma_READ();
//					Gamma_Check();
//---------------------------------------------------------------------------  
			       					
          OTP_OK();					
			}
			else	   //////////if VCOM value not found perfect,goto error
			{
				 OTP_ERROR_3();	  
			}
	}
	else	   //////////if VCOM value not found perfect,goto error
	{
		 OTP_ERROR_2();	 
	}	
}

/*******************************************************************************
* Function Name  : void KEY_adjust(void)
* Description    : 手动模式
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  

void KEY_adjust(void)
{
	u16 mm_KEYB10 = 1 ;
	u16 mm_KEYB1 = 1 ;
	u16 mm_KEYB11 = 1 ;

	u8 OTP_FLAG = 1;

   while(OTP_FLAG)
   {
		mm_KEYB10 = 	KEY_UP;
		mm_KEYB1  =  	KEY_PAUSE;
		mm_KEYB11  =  	KEY_DOWN;

		if(mm_KEYB10 == 0)
		{
			VCOMDH++;
//			STM32TOSSD2828_W_COM(0xb7);		//DCS mode, LP mode
//			STM32TOSSD2828_W_DATA_16BITS(0x0750);	 
			Delay(5);
	    	
			VCOM_set(VCOMDH);	 ////set vcom		         
		  Delay(20);

//			STM32TOSSD2828_W_COM(0xb7);		//Generic mode, HS video mode
//			STM32TOSSD2828_W_DATA_16BITS(0x070B);
			showbmp (Flicker_OTP);				DelayMs(150); 	   //////Flicker_OTP專用
			ShowData_hex("32,100,400",VCOMDH) ;	  DelayMs(150);
			Delay(200);
		}
		if(mm_KEYB11 == 0)
		{
			VCOMDH--;
//			STM32TOSSD2828_W_COM(0xb7);		//DCS mode, LP mode
//			STM32TOSSD2828_W_DATA_16BITS(0x0750);	 
			Delay(5);
	    	
			VCOM_set(VCOMDH);	 ////set vcom		         
		  Delay(20);
			
//			STM32TOSSD2828_W_COM(0xb7);		//Generic mode, HS video mode
//			STM32TOSSD2828_W_DATA_16BITS(0x070B);
			showbmp (Flicker_OTP);				DelayMs(150); 	   //////Flicker_OTP專用
			ShowData_hex("32,100,400",VCOMDH) ;	  DelayMs(150);
			Delay(200);
		}
		if(mm_KEYB1 == 0)
		{
				SSD2828_ENTER_LP_mode(); //enter  LP mode				
				Delay(6);
				{
//					ID_OK=0;  //ID不烧
					
					MTP(); 
					DelayMs(100);
					MTP_OVER=1;
					reset_lcd();				
					DelayMs(100);
				}	
				  showbmp (Flicker_OTP);	    DelayMs(150);
//--------------------------OTP _id值燒入复判--------------------------------
//					if(ID_OK == 1)
//					{		
////						ID_READ();
////						ID_CHECK();
//							
//						DelayMs(5);
////						SAVE_SLAVE_EEPROM_COUNT_NUM(ID_VAL);  //若ID烧入OK，更新存储模块信息！
//			      DelayMs(5); 	
//          }						
//--------------------------OTP _VCOM值燒入复判--------------------------------	
          VCOM_READ();					  //回读再复判
      		VCOM_CHECK(VCOMDC1);
//---------------------------OTP_Gamma值复判---------------------------------	
//					Gamma_READ();
//					Gamma_Check();
//---------------------------------------------------------------------------  
			       					
          OTP_OK();	
					while(1);
		}
		Delay(150);	
		
	
	}
}


int Sensor_check()
{
	int i;
	int Sensor_check=0;
	int data_k = 0;
	
	if(count2<3)
	{
		Sensor_data_ok = 1;	
	}


	for(i=0;i<count2;i++)
	{
		Sensor_error_dot[i]=(DATA_SUM1[i]- DATA_SUM1[count2]);
		if(i>0)
		{
			if(Sensor_error_dot[i] >= Sensor_error_dot[i-1])
			{
				Sensor_check++;
			}	
		}
	}

	if(Sensor_check>(count2/2-1))
	{
		Sensor_data_ok = 1;	
	}

	data_k = 0; /////零参数，以便后续调用。

	return Sensor_data_ok;
}


//--------------------------------------------------
u8 GAMMA_LINE_CHECK(void)
{ 
	u32 RED_bmp        =   0xff0000;
	u32 GREEN_bmp      =   0x00ff00;
	u32 BLUE_bmp       =   0x0000ff;
	u32 WHITE_bmp      =   0xFFFFFF;
	u32 BLACK_bmp      =   0x000000;

	u32 GURY_32_bmp    =   0x202020;    //32灰
	u32 GURY_64_bmp    =   0x404040;    //64灰
	u32 GURY_96_bmp    =   0x606060;    //96灰
	u32 GURY_128_bmp   =   0x808080;    //128灰
	u32 GURY_160_bmp   =   0xA0A0A0;    //160灰
	u32 GURY_192_bmp   =   0xC0C0C0;    //192灰
	u32 GURY_224_bmp   =   0xE0E0E0;    //224灰

	float gamma_0=0,gamma_32=0, gamma_64=0,gamma_96=0, gamma_128=0,gamma_160=0, gamma_192=0, gamma_224=0, gamma_255=0;
	u8 temp_g=0; 	u8 temp_s=0;	
	u8 yy=1,zz=0;
	float light_tep=0;
		
	float gamma_save[53][7];   //存放18条曲线的7个点数据的标准值
	float gamasum[53];         //7点数据差值汇总 
	float gamma_light_measure[9]={0};
	//	float gamma_a,gamma_b;
	float gamma_gray[8]={32,64,96,128,160,192,224,255};
//	double gamma_line[22]={1.8, 1.85, 1.9, 1.95,2.0, 2.05,2.1,2.15, 2.2, 2.25,2.3 ,2.35, 2.4 ,2.45, 2.5 ,2.55, 2.6 ,2.65, 2.7 ,2.75, 2.8};
static	double gamma_line[53]=
	{1.76, 1.78,
	1.80,1.82,1.84,1.86,1.88,	
	1.9, 1.92,1.94,1.96,1.98,
	2.0, 2.02,2.04,2.06,2.08,
	2.1, 2.12,2.14,2.16,2.18, 
	2.2, 2.22,2.24,2.26,2.28,
	2.3 ,2.32,2.34,2.36,2.38, 
	2.4 ,2.42,2.44,2.46,2.48, 
	2.5 ,2.52,2.54,2.56,2.58, 
	2.6 ,2.62,2.64,2.66,2.68, 
	2.7 ,2.72,2.74,2.76,2.78, 
	2.8};
//------------------------------------!!!!
	Measure_Brightness_Init();  //光感初始化
//========================================	
	
	Allcolor(1,WHITE_bmp);       Delay(1900);
	light_tep = ave_Bright();		
	gamma_255 = light_tep;
	
	Allcolor(1,BLACK_bmp);        DelayMs(190);
	Allcolor(1,GURY_96_bmp);      Delay(1500);       //从此笔数据开始，亮度开始偏大 
	light_tep = ave_Bright();		
	gamma_96 = light_tep;
	
	Allcolor(1,GURY_128_bmp);     Delay(1500);
	light_tep = ave_Bright();		
	gamma_128 = light_tep;
	
	Allcolor(1,GURY_160_bmp);     Delay(1500);
	light_tep = ave_Bright();		
	gamma_160 = light_tep;
	
	Allcolor(1,GURY_192_bmp);     Delay(1500);
	light_tep = ave_Bright();		
	gamma_192 = light_tep;
	
	Allcolor(1,GURY_224_bmp);     Delay(1500);
	light_tep = ave_Bright();		
	gamma_224 = light_tep;
	

	
	
	
	
//  Allcolor(1,WHITE_bmp);     DelayMs(320);
////while (1)	 
//	{
//	Allcolor(1,WHITE_bmp);     DelayMs(20);
//	light_tep = LED_ave_Bright();				
//	ShowData("60,260,580", light_tep);
//    DelayMs(100);
//	}	
		
//	Allcolor(1,GURY_224_bmp);     Delay(1500);
//	light_tep = ave_Bright();		
//	gamma_224 = light_tep;
//	
//	Allcolor(1,GURY_192_bmp);     Delay(1500);
//	light_tep = ave_Bright();		
//	gamma_192 = light_tep;
//	
//	Allcolor(1,GURY_160_bmp);     Delay(1500);
//	light_tep = ave_Bright();		
//	gamma_160 = light_tep;
//	
//	Allcolor(1,GURY_128_bmp);     Delay(1500);
//	light_tep = ave_Bright();		
//	gamma_128 = light_tep;
//	
//	Allcolor(1,GURY_96_bmp);      Delay(1500);       //从此笔数据开始，亮度开始偏大 
//	light_tep = ave_Bright();		
//	gamma_96 = light_tep;
	
//	Allcolor(1,GURY_64_bmp);    Delay(1500);      //此笔数据未使用
//	light_tep = ave_Bright();		   
//	gamma_64 = light_tep;
////	
//	Allcolor(1,GURY_32_bmp);     Delay(1500);       //此笔数据未使用
//	light_tep = ave_Bright();		
//	gamma_32 = light_tep;
//	
//	Allcolor(1,BLACK_bmp);     DelayMs(190);      //此笔数据未使用
//	light_tep = ave_Bright();		
//	gamma_0 = light_tep;
///----采集9个点数据	

//	double gamma_gray[10]={32,64,96,128,160,192,224,255};
//double gamma_line[12]={1.8, 1.9, 2.0, 2.1, 2.2, 2.3 , 2.4 , 2.5 , 2.6 , 2.7 , 2.8};
		
	gamma_light_measure[8] = gamma_255;
	gamma_light_measure[7] = gamma_224;

  gamma_light_measure[6] = gamma_192;      //gamma_224*((float)(gamma_224-gamma_192)/(312-218));
	
	gamma_light_measure[5] = gamma_160 ; 
	
	gamma_light_measure[4] = gamma_128;
	//gamma_128= gamma_light_measure[4];
	
	gamma_light_measure[3] =  gamma_96;
	//gamma_96 = gamma_light_measure[3];
	
	gamma_light_measure[2] =  gamma_64;
	//gamma_64 = gamma_light_measure[2];
	
	gamma_light_measure[1] =  gamma_32;
	//gamma_32 = gamma_light_measure[1];
	
	gamma_light_measure[0] = gamma_0;
//////////----收集9个点数据	
	
	for(temp_g=0;temp_g<7;temp_g++)
	{
		gamma_gray[temp_g] = gamma_gray[temp_g] / gamma_gray[7];
	}
	
	for(temp_g=0;temp_g<53;temp_g++)
	{
//		gamma_a = 0;
//		gamma_b = 0;
//	    gamma_a = 0.1255;
		gamma_save[temp_g][0] = pow(gamma_gray[0],gamma_line[temp_g]);      ///32
		gamma_save[temp_g][1] = pow(gamma_gray[1],gamma_line[temp_g]);      ///64
		gamma_save[temp_g][2] = pow(gamma_gray[2],gamma_line[temp_g]);      ///96
		gamma_save[temp_g][3] = pow(gamma_gray[3],gamma_line[temp_g]);      ///128
		gamma_save[temp_g][4] = pow(gamma_gray[4],gamma_line[temp_g]);      ///160
		gamma_save[temp_g][5] = pow(gamma_gray[5],gamma_line[temp_g]);      ///192
		gamma_save[temp_g][6] = pow(gamma_gray[6],gamma_line[temp_g]);      ///224
		
	}
	
	for(temp_g=0;temp_g<53;temp_g++)   //for(temp_g=0;temp_g<9;temp_g++)
	{
//		gamasum[temp_g] = fabs(gamma_save[temp_g][0] - gamma_light_measure[1]/gamma_light_measure[8]);      ///32 
//		gamasum[temp_g] += fabs(gamma_save[temp_g][1] - gamma_light_measure[2]/gamma_light_measure[8]);      ///64
		//若启用上两笔数据则需要修改前面采样数据的 32 和 64 阶部分
		gamasum[temp_g] = fabs(gamma_save[temp_g][2] - gamma_light_measure[3]/gamma_light_measure[8]);       ///96
		gamasum[temp_g] += fabs(gamma_save[temp_g][3] - gamma_light_measure[4]/gamma_light_measure[8]);      ///128
		gamasum[temp_g] += fabs(gamma_save[temp_g][4] - gamma_light_measure[5]/gamma_light_measure[8]);      ///160
		gamasum[temp_g] += fabs(gamma_save[temp_g][5] - gamma_light_measure[6]/gamma_light_measure[8]);      ///192
		gamasum[temp_g] += fabs(gamma_save[temp_g][6] - gamma_light_measure[7]/gamma_light_measure[8]);      ///224
	}
	
	
	temp_g = 53;
	{
		yy = 1;
		zz = 0;
		while(yy<temp_g && zz<temp_g)
		{
			if(	gamasum[zz]>gamasum[yy])
			{
				temp_s=yy;zz=zz+1;	
			}
			else
			{
				temp_s=zz;yy=yy+1;
			}
		}	
	}
	//temp_s存放最小值所在的数组
	


	Allcolor(1,WHITE_bmp);     DelayMs(180);
     // temp_s = temp_s;
	
	
//	if(temp_s > 4)
//	{
//		temp_s = temp_s -1;
//	}
//	if(temp_s < 4)
//	{
//		temp_s = temp_s +1;
//	}
	
	ShowData("60,10,60", temp_s);									DelayMs(180);	
//		ShowData("30,160,20", gamasum[temp_s]*10000);                    DelayMs(150);
	
	ShowTxt("30,20,540, gamma_32 =");	   					DelayMs(180);
	ShowData_ua2ma_4bitfloat("30,260,540", gamma_light_measure[1]);                    DelayMs(180);
	
	ShowTxt("30,20,580, gamma_255 =");	   					DelayMs(180);
	ShowData_ua2ma_4bitfloat("30,260,580", gamma_light_measure[8]);                    DelayMs(180);
	
	if((temp_s > 25)||(temp_s <= 3)||( gamma_light_measure[8] < 360)||(gamma_light_measure[1]  > 15))
		{
			ShowTxt("60,120,120, gamma  =  NG");	 	DelayMs(180);
			while(1);
		}	
	///////自动OTP的光感初始化。。。	
	TSL2561Init(S1_addr);
	TSL2561Init(S2_addr);
	TSL2561Init(S3_addr);
	return temp_s;
}


