 /******************** (C) COPYRIGHT 2010 KSGIANTLUS ********************
* File Name          : lcd.c
* Author             : JASON
* Version            : V1.0
* Date               : 2010.01.20
*************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "lcd.h"
#include "lib.h"

#include "LVDS_I2C.h"
#include <LVDS_OTP.h>
#include <Mtransmit.h>
#include <V_SET.h>
#include "math.h"


/* Includes ------------------------------------------------------------------*/
#include "lcd.h"
#include  "lib.h"
#include "tsliic.h"
#include "OTP.h"
#include "LVDS_I2C.h"
 unsigned char vcom11,vcom22;
extern void Delay(u32 nCount);
extern void DelayKEY (u32 k); //按住，画面暂停
extern u8   MIPI_READ_DATA[10];
extern void DelayMs(__IO uint32_t nTime);
extern void ShowData(char *p,u16 dat);
extern u8 Flicker_OTP;

/////////////////////add in OTP.C----------------------------
int Sensor_error_dot[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int Sensor_data_ok = 0;
#define Sensor_Err1 3000	////定义光感测量flicker的max值
#define Sensor_Err2	2			////定义光感测量flicker的min值。

u8 OTP_VALUE1 = 0;
u8 OTP_VALUE2 = 0;
u8 OTP_TIMES = 0;

/*******************************************************************************
在此處定義

*******************************************************************************/  

#define STEPX 0x07   //采样数组容量上限	
#define STEPZ 0x7f   //采样数组容量上限	
	
u16 DATA_SUMX[STEPX],SUREX[STEPX];
u16 DATA_SUMZ[STEPZ],SUREZ[STEPZ];	
	
u16 CounT1=0,CounT2=0,CounT3=0,CounT4=0;
u16 VCOMDC1=0x0000;


u8 VCOMDCX_reg=0x08;   //粗调VCOM寄存器
u8 VCOMDCZ_reg=0x0A;   //微调VCOM寄存器

u8 VCOMDCX=0;   //粗调值
u8 VCOMDCZ=0;   //微调值
u8 VCOMC=0x00;  //粗调起始阶次
u8 VCOMF=0x01;  //微调起始阶次

void LVDS_MTP(void);
void LVDS_KEY_adjust(void);
void LVDS_VCOM_SET_Subtle(u8 vcomx);
void LVDS_VDOM_SETX(u8 vcomx);
void LVDS_HFZ_SETX(u8 vcomx);

extern void DelayMs(__IO uint32_t nTime);
extern void ShowData(char *p,u16 dat);
extern void ShowData_hex(char *p,u16 dat);



void LVDS_MTP(void)
{
	LVDS_ROM_RLB_H;
	DelayMs(50); 
	LVDS_BYTE_Write(0x70,0x00,0x08);   //′?μ÷?μé?è?
	DelayMs(120); 
	//LVDS_BYTE_Write(0x70,0x10,0x80);
	//DelayMs(120); 
	//LVDS_BYTE_Write(0x70,0x0B,0x08);
	//DelayMs(120); 
	LVDS_BYTE_Write(0x70,0x6f,0x10);
	DelayMs(100); 
	LVDS_BYTE_Write(0x70,0x6c,0x88);//
	DelayMs(100); 
	LVDS_BYTE_Write(0x70,0x6D,VCOMDCX);//
	DelayMs(100); 

	LVDS_BYTE_Write(0x70,0x6F,0x12);   
	DelayMs(100); 
	LVDS_BYTE_Write(0x70,0x6f,0x10);
	DelayMs(100); 
	LVDS_BYTE_Write(0x70,0x6c,0x00);//
	DelayMs(100); 
	LVDS_BYTE_Write(0x70,0x6f,0x00);
	DelayMs(100); 
	//=============================================
	LVDS_BYTE_Write(0x70,0x00,0x08);   //??μ÷?μé?è?
	DelayMs(120); 
	//LVDS_BYTE_Write(0x70,0x10,0x80);
	//DelayMs(120); 
	//LVDS_BYTE_Write(0x70,0x0B,0x08);
	//DelayMs(120); 
	LVDS_BYTE_Write(0x70,0x6f,0x10);
	DelayMs(100); 
	LVDS_BYTE_Write(0x70,0x6c,0x8A);//
	DelayMs(100); 
	LVDS_BYTE_Write(0x70,0x6D,VCOMDCZ);//
	DelayMs(100); 

	LVDS_BYTE_Write(0x70,0x6F,0x12);//
	DelayMs(100); 
	LVDS_BYTE_Write(0x70,0x6f,0x10);
	DelayMs(100); 
	LVDS_BYTE_Write(0x70,0x6c,0x00);//
	DelayMs(100); 
	LVDS_BYTE_Write(0x70,0x6f,0x00);
	DelayMs(100); 
	LVDS_ROM_RLB_L;
	DelayMs(100);
}


void LVDS_VCOM_SETX(u8 vcomx)    //粗调设定
{	
	VCOMDCX=0x80+(vcomx&0X07);	
	LVDS_BYTE_Write(0x70,0x08,VCOMDCX);
	DelayMs(5); 	
}

void LVDS_VCOM_SET_Subtle(u8 vcomx)   //微调设定
{
	VCOMDCZ=vcomx;	
	LVDS_BYTE_Write(0x70,0x0a,VCOMDCZ);
	DelayMs(15); 	
}

void LVDS_HFZ_SETX(u8 vcomx)
{
u8 adjust_vcom;
adjust_vcom=0x80+vcomx;
	
LVDS_ROM_RLB_H;
DelayMs(5); 

LVDS_BYTE_Write(0x70,0x08,adjust_vcom);
DelayMs(15); 

//GPIO_ResetBits(GPIOA,GPIO_Pin_12);
//DelayMs(5); 	
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
/**************************************************
*函数名：void LVDS_KEY_adjust(void)
*功能：  手动OTP
*输入：  
*输出： 
***************************************************/

void LVDS_KEY_adjust(void)
{
	u16 mm_KEYB10 = 1 ;
	u16 mm_KEYB1 = 1 ;
	u16 mm_KEYB11 = 1 ;

//=========粗调==========================	 	
		u8 OTP_FLAG = 1;
	
	  AUT_Flicker_Senser_initial();
	  DelayMs(15); 
		LVDS_ROM_RLB_H;
		DelayMs(15); 
	
		LVDS_VCOM_SETX(VCOMC);
		LVDS_VCOM_SET_Subtle(VCOMF);
	
   while(OTP_FLAG)
   {
			mm_KEYB10 = 	KEY_UP;
			mm_KEYB1  =  	KEY_PAUSE;
			mm_KEYB11  =  	KEY_DOWN;
            LVDS_VCOM_SETX(VCOMC);
			if(mm_KEYB10 == 0)
			{
				LVDS_ROM_RLB_H;
				showbmp (Flicker_OTP);	    DelayMs(150);
				if(VCOMC==0x07)
				{
				VCOMC=0x06;
				}
				 VCOMC++;

				 LVDS_VCOM_SETX(VCOMC);
				 ShowData_hex("32,180,100",VCOMC) ;		   DelayMs(150);
				 LVDS_ROM_RLB_L;
			}
			if(mm_KEYB11 == 0)
			{
				LVDS_ROM_RLB_H;
				showbmp (Flicker_OTP);	    DelayMs(150);
				if(VCOMC==0x00)
				{
				VCOMC=0x01;
				}
				 VCOMC--; 
				 LVDS_VCOM_SETX(VCOMC);
				 ShowData_hex("32,180,100",VCOMC) ;		   DelayMs(150);
				 LVDS_ROM_RLB_L;
			}
			if(mm_KEYB1 == 0)
			{
 			   LVDS_MTP();
				 OTP_FLAG=0;
				 DelayMs(500);
			}	
		}
//=========细调==========================	 
	  OTP_FLAG=1;
		while(OTP_FLAG==1)
		{
			mm_KEYB10 = 	KEY_UP;
			mm_KEYB1  =  	KEY_PAUSE;
			mm_KEYB11  =  	KEY_DOWN;

			if(mm_KEYB10 == 0)
			{
				LVDS_ROM_RLB_H;
				showbmp (Flicker_OTP);	    DelayMs(150);
				if(VCOMF==0x7F)
				{
				VCOMF=0x7E;
				}
				 VCOMF++;

				 LVDS_VCOM_SET_Subtle(VCOMF);
				 ShowData_hex("32,180,100",VCOMF) ;		   DelayMs(150);
				 LVDS_ROM_RLB_L;
			}
			if(mm_KEYB11 == 0)
			{
				LVDS_ROM_RLB_H;
				showbmp (Flicker_OTP);	    DelayMs(150);
				if(VCOMF==0x00)
				{
				VCOMF=0x01;
				}
				 VCOMF--; 
				 LVDS_VCOM_SET_Subtle(VCOMF);
				 ShowData_hex("32,180,100",VCOMF) ;		   DelayMs(150);
				 
				 LVDS_ROM_RLB_L;
			}
			if(mm_KEYB1 == 0)
			{
					LVDS_MTP();
					OTP_FLAG=0;
			}
		}		
}






/*******************************************************************************
* Function Name  : 一些提示函數，可根據實際需要調整顯示位置
* Description    : 函數尾部請務必加入適當延時
* Input          : 字體放大倍數 ,X坐標，Y坐標 ，文字內容
* Output         : None
* Return         : None
*******************************************************************************/  

void OTP_ID_OK(void)
{
		ShowTxt("48,20,700,ID = ");	  DelayMs(150);
		ShowTxt("48,170,700,OK");	    DelayMs(150);
}
void OTP_ID_NG(void)
{
		ShowTxt("48,20,700,ID = ");	  DelayMs(150);
  	ShowTxt("48,170,700,NG");	    DelayMs(150);
}

void OTP_ERROR_1(void)
{
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除
	{
		ShowTxt("48,20,100,OTP 烧录校验错，可能烧录次数超限");	  Delay(100);
		while(1);
	}
}

void OTP_ERROR_2(void)
{
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除
	{
		ShowTxt("48,20,100,OTP 数值超限，请重试或检查光感"); 	 Delay(100);
		while(1);
	}
}

void OTP_ERROR_3(void)
{
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除

	{
		ShowTxt("48,20,800, ");			    DelayMs(80);
		ShowTxt("48,20,100,OTP 数值异常,请正确放置光感"); 		Delay(100);
		while(1);
	}
}
void OTP_ERROR_4(void)
{
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除
	
	ShowTxt("48,20,700, ");			    DelayMs(80);
	ShowTxt("48,20,700,採樣值有異常");Delay(20);
	while(1);
}

void OTP_ERROR_5(void)
{
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除
	{
		ShowTxt("48,20,100,OTP 烧录校验错误");	  Delay(100);
		while(1);
	}
}

void OTP_NO(void)
{	 
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除

	{
	ShowTxt("48,20,99, ");		   DelayMs(210);
	ShowTxt("48,20,100,OTP  未烧入！");	  Delay(100);
	while(1);
	}
}

void OTP_OK(void)
{	 
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除
	
	ShowTxt("48,20,299, ");		   DelayMs(210);
	ShowTxt("48,20,100,OTP  OK！");	      Delay(100);
}

void OTP_adjust_resultX(u16 senser_val)
{	

//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除

	ShowTxt("48,20,299, ");		   DelayMs(80);
	ShowTxt("32,0,50, sener值");	  				 DelayMs(150);
	ShowData("32,150,50",senser_val) ;		   Delay(150);
	ShowTxt("32,0,0, VCOM值");	  				   DelayMs(150);
	ShowData_hex("32,150,0",VCOMDCX) ;		       Delay(150);
}
void OTP_adjust_resultZ(u16 senser_val)
{	

//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除

	ShowTxt("48,20,299, ");		   DelayMs(80);
	ShowTxt("32,0,50, sener值");	  				 DelayMs(150);
	ShowData("32,150,50",senser_val) ;		   Delay(150);
	ShowTxt("32,0,0, VCOM值");	  				   DelayMs(150);
	ShowData_hex("32,150,0",VCOMDCZ) ;		       Delay(150);
}


void SHOW_READ_VALUE(void)
{
   LVDS_WRITE_SPI(0x00,0x00);  ////page 0
    
    vcom11 = LVDS_READ_SPI(0x01);
    ShowData_hex("32,650,10",0x01) ;		   Delay(150);
    ShowData_hex("32,750,10",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x02);
    ShowData_hex("32,650,50",0x02) ;		   Delay(150);
    ShowData_hex("32,750,50",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x03);
    ShowData_hex("32,650,90",0x03) ;		   Delay(150);
    ShowData_hex("32,750,90",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x04);
    ShowData_hex("32,650,130",0x04) ;		   Delay(150);
    ShowData_hex("32,750,130",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x05);
    ShowData_hex("32,650,170",0x05) ;		   Delay(150);
    ShowData_hex("32,750,170",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x06);
    ShowData_hex("32,650,210",0x06) ;		   Delay(150);
    ShowData_hex("32,750,210",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x07);
    ShowData_hex("32,650,250",0x07) ;		   Delay(150);
    ShowData_hex("32,750,250",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x08);
    ShowData_hex("32,650,290",0x08) ;		   Delay(150);
    ShowData_hex("32,750,290",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x09);
    ShowData_hex("32,650,330",0x09) ;		   Delay(150);
    ShowData_hex("32,750,330",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x0A);
    ShowData_hex("32,650,370",0x0A) ;		   Delay(150);
    ShowData_hex("32,750,370",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x0B);
    ShowData_hex("32,650,410",0x0B) ;		   Delay(150);
    ShowData_hex("32,750,410",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x0C);
    ShowData_hex("32,650,450",0x0C) ;		   Delay(150);
    ShowData_hex("32,750,450",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x0D);
    ShowData_hex("32,650,490",0x0D) ;		   Delay(150);
    ShowData_hex("32,750,490",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x0E);
    ShowData_hex("32,650,530",0x0E) ;		   Delay(150);
    ShowData_hex("32,750,530",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x0F);
    ShowData_hex("32,650,570",0x0F) ;		   Delay(150);
    ShowData_hex("32,750,570",vcom11) ;		   Delay(150);
    
    
    vcom11 = LVDS_READ_SPI(0x10);
    ShowData_hex("32,650,610",0x10) ;		   Delay(150);
    ShowData_hex("32,750,610",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x11);
    ShowData_hex("32,650,650",0x11) ;		   Delay(150);
    ShowData_hex("32,750,650",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x12);
    ShowData_hex("32,650,690",0x12) ;		   Delay(150);
    ShowData_hex("32,750,690",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x13);
    ShowData_hex("32,650,730",0x13) ;		   Delay(150);
    ShowData_hex("32,750,730",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x14);
    ShowData_hex("32,650,770",0x14) ;		   Delay(150);
    ShowData_hex("32,750,770",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x15);
    ShowData_hex("32,650,810",0x15) ;		   Delay(150);
    ShowData_hex("32,750,810",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x16);
    ShowData_hex("32,650,850",0x16) ;		   Delay(150);
    ShowData_hex("32,750,850",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x17);
    ShowData_hex("32,650,890",0x17) ;		   Delay(150);
    ShowData_hex("32,750,890",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x18);
    ShowData_hex("32,650,930",0x18) ;		   Delay(150);
    ShowData_hex("32,750,930",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x19);
    ShowData_hex("32,650,970",0x19) ;		   Delay(150);
    ShowData_hex("32,750,970",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x1A);
    ShowData_hex("32,650,1010",0x1A) ;		   Delay(150);
    ShowData_hex("32,750,1010",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x1B);
    ShowData_hex("32,650,1050",0x1B) ;		   Delay(150);
    ShowData_hex("32,750,1050",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x1C);
    ShowData_hex("32,650,1090",0x1C) ;		   Delay(150);
    ShowData_hex("32,750,1090",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x1D);
    ShowData_hex("32,650,1130",0x1D) ;		   Delay(150);
    ShowData_hex("32,750,1130",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x1E);
    ShowData_hex("32,650,1170",0x1E) ;		   Delay(150);
    ShowData_hex("32,750,1170",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x1F);
    ShowData_hex("32,650,1210",0x1F) ;		   Delay(150);
    ShowData_hex("32,750,1210",vcom11) ;		   Delay(150);
}


void I2C_MEMSURE1_Z(int kk  , int vcomdcdata)    //微调
{
	int i=0,tep1=0,tep2=0,flag=0; //2014.1.27  HFZ

	int sss=0,yyy=0,aaa=0,temp;	  //temp 为加的阶数
	
	for (yyy=0;yyy<kk;yyy++)
	{SUREZ[sss]=vcomdcdata+aaa;
	
  LVDS_VCOM_SET_Subtle(SUREZ[sss]); 
	DelayMs(100);

	DATA_SUMZ[CounT3] =	Receive_flicker()/10;	 ///get flicker value

  //------2014.1.27--HFZ-START-------
		if (flag==0)                             //判断是否找到了最低点
			{
				tep2=tep1=DATA_SUMZ[CounT3];
				flag=1;
			}
		else
			{ 
				tep1=DATA_SUMZ[CounT3];
					if(tep1<tep2)
					{
							tep2=tep1;
							 flag=1;  //此处flag再次赋值是为了在一旦再次出现更小值的情况，清除flag计数 令后续可以找到真正的最小值
					}
					else   // if (tep1>tep2)       //如果找到了最低点，则继续寻找两次(flag=1~3)
					  	flag++;
					
					if ((flag==3) & (tep2<35))    //判断是否是真的最低点，再确认是否小于预定值，如果是则停止寻找以节约时间。
					{
						for(i=(CounT3+1);i<STEPZ;i++)
							DATA_SUMZ[i]=100;                     //填充剩下的数组，否则后续处理会出错
					
							yyy=kk;     //改变判断条件，使本函数提前结束
					}
			}
		  //
	 //------2014.1.27----HFZ--END------	
				
				
				
	if(DATA_SUMZ[CounT3] <= 35)
	{temp=1; }
	else if((DATA_SUMZ[CounT3]>35)&&(DATA_SUMZ[CounT3] < 55))
	{temp=2; }
	else if((DATA_SUMZ[CounT3]>=55)&&(DATA_SUMZ[CounT3] < 70))
	{temp=3; }
	else if((DATA_SUMZ[CounT3]>=70)&&(DATA_SUMZ[CounT3] < 100))
	{temp=5;}	
	else
	{temp=7;}

	CounT3=CounT3+1;
	aaa=aaa+temp;
	sss=sss+1;
	}
}


void I2C_MEMSURE1_X(int kk  , int vcomdcdata)    //微调
{
	int i=0,tep1=0,tep2=0,flag=0; //2014.1.27  HFZ

	int sss=0,yyy=0,aaa=0,temp;	  //temp 为加的阶数
	
	for (yyy=0;yyy<kk;yyy++)
	{SUREX[sss]=vcomdcdata+aaa;
	
	LVDS_VCOM_SETX(SUREX[sss]); 
	DelayMs(100);

	DATA_SUMX[CounT1] =	Receive_flicker()/10;	 ///get flicker value,除10是为了将小数点位去除，比如屏幕显示502.6，即返回5026，将.6去除，方便程序员对后续数据做处理，不然脑子会乱掉
  
  //------2014.1.27--HFZ-START-------
		if (flag==0)                             //判断是否找到了最低点
			{
				tep2=tep1=DATA_SUMX[CounT1];
				flag=1;
			}
		else
			{ 
				tep1=DATA_SUMX[CounT1];
					if(tep1<tep2)
					{
							tep2=tep1;
							 flag=1;  //此处flag再次赋值是为了在一旦再次出现更小值的情况，清除flag计数 令后续可以找到真正的最小值
					}
					else   // if (tep1>tep2)       //如果找到了最低点，则继续寻找两次(flag=1~3)
					  	flag++;
					
					if ((flag==3) & (tep2<200))    //判断是否是真的最低点，再确认是否小于预定值，如果是则停止寻找以节约时间。
					{
						for(i=(CounT1+1);i<STEPX;i++)
							DATA_SUMX[i]=1500;                     //填充剩下的数组，否则后续处理会出错
					
							yyy=kk;     //改变判断条件，使本函数提前结束
					}
			}
		  //
	 //------2014.1.27----HFZ--END------	
				
				
				
//	if(DATA_SUMX[CounT3] <= 100)
//	{temp=1; }
//	else if((DATA_SUMX[CounT3]>100)&&(DATA_SUMX[CounT3] < 150))
//	{temp=2; }
//	else if((DATA_SUMX[CounT3]>=150)&&(DATA_SUMX[CounT3] < 250))
//	{temp=3; }
//	else if((DATA_SUMX[CounT3]>=250)&&(DATA_SUMX[CounT3] < 450))
//	{temp=5;}	
//	else
//	{temp=7;}
			
	temp=1;    //粗调直接1阶一跳		

	CounT1=CounT1+1;
	aaa=aaa+temp;
	sss=sss+1;
	}
}

void OTP_Dispose_X(void)
{
	u16 senser_value=0;
	u16 mm_KEYB10 = 1 ;

	I2C_MEMSURE1_X(STEPX , VCOMC);
	I2C_DATAL_COMPARE_X();
	CounT1=0;
	
/*在最小值附近停下来。。。后续开启OTP功能即可。。*/
	 VCOMDCX = SUREX[CounT2];				 //挑选出最好的VCOM的阶次，下CODE，看效果。=
	 LVDS_VCOM_SETX(VCOMDCX); 
	 Delay(100);

//=========================================================================  
	senser_value = DATA_SUMX[CounT2];		////挑选出最小的FLICKER值，显示出来。
		
	OTP_adjust_resultX(senser_value);


///////新增自动OTP数据的判读，判断光感采集到的微笑曲线是否合格，减少OTP误判。。。。
//	while(Sensor_check())
//	{
//		OTP_ERROR_4();	
//	}
//==========================================================================		
}


void OTP_Dispose_Z(void)
{
	u16 senser_value=0;
	u16 mm_KEYB10 = 1 ;

	I2C_MEMSURE1_Z(STEPZ , VCOMF);
	I2C_DATAL_COMPARE_Z();
	CounT3=0;
	
/*在最小值附近停下来。。。后续开启OTP功能即可。。*/
	 VCOMDCZ = SUREZ[CounT4];				 //挑选出最好的VCOM的阶次，下CODE，看效果。=
	 LVDS_VCOM_SET_Subtle(VCOMDCZ); 
	 Delay(100);

//=========================================================================  
	senser_value = DATA_SUMZ[CounT4];		////挑选出最小的FLICKER值，显示出来。
		
	OTP_adjust_resultZ(senser_value);


///////新增自动OTP数据的判读，判断光感采集到的微笑曲线是否合格，减少OTP误判。。。。
//	while(Sensor_check())
//	{
//		OTP_ERROR_4();	
//	}
//==========================================================================		
}



void I2C_DATAL_COMPARE_X(void)
{
		int yy=1,zz=0;
	  while(yy<CounT1 && zz<CounT1)
	 {
		if(	DATA_SUMX[zz]>DATA_SUMX[yy])
		{
			CounT2=yy;zz=zz+1;	
		}
		else
		{
			CounT2=zz;yy=yy+1;
		}
	}	
}

void I2C_DATAL_COMPARE_Z(void)
{
		int yy=1,zz=0;
	  while(yy<CounT3 && zz<CounT3)
	 {
		if(	DATA_SUMZ[zz]>DATA_SUMZ[yy])
		{
			CounT4=yy;zz=zz+1;	
		}
		else
		{
			CounT4=zz;yy=yy+1;
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
	u8 sdcap=0;
	
//    vcom11=LVDS_BYTE_Read(0x70,VCOMDCX_reg);DelayMs(15); //粗调值
//    vcom11=LVDS_BYTE_Read(0x70,0x00);DelayMs(15); //粗调值  0x08
//    vcom11=LVDS_BYTE_Read(0x70,0x01);DelayMs(15); //粗调值  0xad
//    vcom11=LVDS_BYTE_Read(0x70,0x02);DelayMs(15); //粗调值  0xFD
//    vcom11=LVDS_BYTE_Read(0x70,0x03);DelayMs(15); //粗调值  0X20
//    vcom11=LVDS_BYTE_Read(0x70,0x04);DelayMs(15); //粗调值  0X08
//    vcom11=LVDS_BYTE_Read(0x70,0x05);DelayMs(15); //粗调值  0XCE
//    vcom11=LVDS_BYTE_Read(0x70,0x06);DelayMs(15); //粗调值  0XC7
//    vcom11=LVDS_BYTE_Read(0x70,0x07);DelayMs(15); //粗调值  0X31
//    vcom11=LVDS_BYTE_Read(0x70,0x09);DelayMs(15); //粗调值  0X3B
//		vcom22=LVDS_BYTE_Read(0x70,VCOMDCZ_reg);DelayMs(15); //细调值
    
    
    
//    LVDS_WRITE_SPI(0x00,0x00);  ////page 0
//    LVDS_WRITE_SPI(0x1F,0x80); 
    
    SHOW_READ_VALUE();
    
    
    
	ShowTxt("40,20,100,粗调");	DelayMs(80);
	OTP_Dispose_X();  //粗调
	
	showbmp (Flicker_OTP);	   			DelayMs(150); 
	ShowTxt("40,20,100,微调");	DelayMs(80);
	OTP_Dispose_Z();  //微调
	
	DelayKEY(1);

	if(DATA_SUMZ[CounT4] <= 45)			 ///for check OTP is OK or NG....
	{
			if(DATA_SUMZ[CounT4] >= 1)
			{
			//	Delay(6);
				{
					LVDS_MTP();
					DelayMs(200);
					VDD_ADJ_EN(0);	
					DelayMs(200);
					VDD_ADJ_SET(305);			 //VDD_ADJ_SET 参数取值范围270~620，对应电压范围2.7V~6.2V; 
				  VDD_ADJ_EN(1);	
					DelayMs(200);				
				}
				
				Check_mtp();
			}
			else	   //////////if VCOM value not found perfect,goto error
			{
				 OTP_ERROR_3();	  Delay(100);	 
				 while (1);
			}
	}
	else	   //////////if VCOM value not found perfect,goto error
	{
		 OTP_ERROR_2();	  Delay(100);
		 while (1);
	}	
}


int Sensor_check()
{
	int i;
	int Sensor_check=0;
	int data_k = 0;
	
	if(CounT2<3)
	{
		Sensor_data_ok = 1;	
	}


	for(i=0;i<CounT2;i++)
	{
		Sensor_error_dot[i]=(DATA_SUMZ[i]- DATA_SUMZ[CounT2]);
		if(i>0)
		{
			if(Sensor_error_dot[i] >= Sensor_error_dot[i-1])
			{
				Sensor_check++;
			}	
		}
	}

	if(Sensor_check>(CounT2/2-1))
	{
		Sensor_data_ok = 1;	
	}

	data_k = 0; /////零参数，以便后续调用。

	return Sensor_data_ok;
}

void Check_mtp(void)
{
   
	
		vcom11=LVDS_BYTE_Read(0x70,VCOMDCX_reg);DelayMs(15); //粗调值
		vcom22=LVDS_BYTE_Read(0x70,VCOMDCZ_reg);DelayMs(15); //细调值
	  
	if((vcom11==VCOMDCX)&&(vcom22==VCOMDCZ))
	{
				OTP_OK();	  Delay(100);	 
	}

}

u8 GAMMA_OTP_CHECK(void)
{
     LVDS_WRITE_SPI(0x00,0x07);  ////page 7
    
    vcom11 = LVDS_READ_SPI(0x01);
    ShowData_hex("32,650,10",0x01) ;		   Delay(150);
    ShowData_hex("32,750,10",vcom11) ;		   Delay(150);
    
    if(vcom11 != 0x15)
    {       OTP_ERROR_5();	  Delay(100);	    while(1);   }
    
    LVDS_WRITE_SPI(0x00,0x00);  ////page 0
    
    vcom11 = LVDS_READ_SPI(0x1E);
    ShowData_hex("32,650,110",0x1E) ;		   Delay(150);
    ShowData_hex("32,750,110",vcom11) ;		   Delay(150);
    
    vcom11 = LVDS_READ_SPI(0x1F);
    ShowData_hex("32,650,150",0x1F) ;		   Delay(150);
    ShowData_hex("32,750,150",vcom22) ;		   Delay(150);
    
    if((vcom11 == 0x52)&&(vcom22 == 0x80))
    {   OTP_OK();	  Delay(100);	    return 0;}
    else
    {   OTP_ERROR_5();	  Delay(100);	 while(1);    }
    
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
		
	static float gamma_save[53][7];   //存放18条曲线的7个点数据的标准值
	static float gamasum[53];         //7点数据差值汇总 
	static float gamma_light_measure[9]={0};
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
	Delay(1900);
//========================================	
	
	Allcolor(1,WHITE_bmp);       Delay(1900);
	light_tep = ave_Bright();		
	gamma_255 = light_tep;
	
	Allcolor(1,BLACK_bmp);        DelayMs(190);
	
//	Allcolor(1,GURY_32_bmp);      Delay(1500);       //从此笔数据开始，亮度开始偏大 
//	light_tep = ave_Bright();		
//	gamma_32 = light_tep;
//	
//	Allcolor(1,GURY_64_bmp);      Delay(1500);       //从此笔数据开始，亮度开始偏大 
//	light_tep = ave_Bright();		
//	gamma_64 = light_tep;
	
	
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
	
  Allcolor(1,WHITE_bmp);       Delay(1900);
	light_tep = ave_Bright();		
	gamma_255 = light_tep;
	
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
	
	ShowData("60,10,60", temp_s);									DelayMs(180);	
	
	ShowTxt("30,20,580, gamma_255 =");	   					DelayMs(180);
	ShowData("30,260,580", gamma_light_measure[8]);                    DelayMs(180);
	
	if((temp_s > 25)||(temp_s <= 3)||( gamma_light_measure[8] < 360)||(gamma_light_measure[1]  > 15))
		{
			ShowTxt("60,120,120, gamma  =  NG");	 	DelayMs(180);
			while(1);
		}	
	///////自动OTP的光感初始化。。。	
		Niubee_Senser_initial(); 
		
	return temp_s;
}


