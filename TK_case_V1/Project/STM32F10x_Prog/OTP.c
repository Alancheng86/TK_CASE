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

extern void Delay(u32 nCount);
extern void DelayKEY (u32 k); //按住，画面暂停
extern u8 MIPI_READ_DATA[10];
extern void DelayMs(__IO uint32_t nTime);
extern void ShowData(char *p,u16 dat);
extern u8 Flicker_OTP;
extern u8 MTP_PASS;
void ShowData_hex(char *p,u16 dat);
extern void SSD2828_inition_lcd();
/////////////////////add in OTP.C----------------------------
int Sensor_error_dot[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int Sensor_data_ok = 0;
#define Sensor_Err1 3000		////定义光感测量flicker的max值
#define Sensor_Err2	2			////定义光感测量flicker的min值。



u8 OTP_VALUE1 = 0;
u8 OTP_VALUE2 = 0;
u8 OTP_TIMES = 0;
u8 ID_OK = 1;   //默認ID未燒

/*******************************************************************************
在此處定義


*******************************************************************************/  
/////////------------080WQ18 JD9366正确ID值设定---------------------
u8 ID1_VALUE = 0X53;
u8 ID2_VALUE = 0X44;
u8 ID3_VALUE = 0X90;

u8 VCOM_register=0x80;   //设置vcom寄存器地址
u8 VCOM_Read_reg=0x02;   //读取vcom寄存器地址


u8 ID_register = 0xC3;    //设置ID寄存器地址 
///////////------------080WQ18 ILI6136S正确ID值设定---------------------
//u8 ID1_VALUE = 0X53;
//u8 ID2_VALUE = 0X44;
//u8 ID3_VALUE = 0X90;

//u8 VCOM_register=0x53;   //设置vcom寄存器地址
//u8 VCOM_Read_reg=0xe8;   //读取vcom寄存器地址


//u8 ID_register = 0xC3;    //设置ID寄存器地址 

//////------------GPM1461A0正确ID值设定---------------------
//u8 ID1_VALUE = 0XD5;
//u8 ID2_VALUE = 0X97;
//u8 ID3_VALUE = 0XCA;
//
//u8 VCOM_register=0xD9;   //设置vcom寄存器地址
//u8 ID_register = 0xC3;    //设置ID寄存器地址 
//u8 ID1_register = 0xDA;
//u8 ID2_register = 0xDB;
//u8 ID3_register = 0xDC;

u8 STEP=30;   //采样数组上限
u8 VCOMDH = 0x8E;   //手动调整起始地址
u8 VCOMDC = 0x7C;	  //自动模式起始地址
u8 VCOM_Temp=0x8E;    //保存中间值

//u8 VCOMDH = 0x8D;   //手动调整起始地址
//u8 VCOMDC = 0x80;	  //自动模式起始地址	 

u16 DATA_SUM1[30],DATA_SUMY[30],SURE[30];
u8 MTP_OVER=0; //该型号56H寄存器定义VCOM为默认值还是烧录值，故，在烧录前使用默认值初始化，
								//在烧录完成重新启动屏幕前使用，烧录值初始化     0--表示未作烧录动作   1--表示烧录动作完成
//======================================END define===================================

u16 count1=0,count2=0;
u16 VCOMDC1=0x0000;
u8  checktimes=0; 


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

void OTP_ID_OK(void)
{
		ShowTxt("48,250,800,ID = ");	  DelayMs(150);
		ShowTxt("48,370,800,OK");	    DelayMs(150);
}
void OTP_ID_NG(void)
{
		ShowTxt("48,250,800,ID = ");	  DelayMs(150);
  	ShowTxt("48,370,800,NG");	    DelayMs(150);
}

void OTP_ERROR_1(void)
{
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除
		ShowTxt("48,20,100,OTP 烧录校验错，可能烧录次数超限");	  Delay(100);
		while(1)
		{BEEP_Dudu();}
}

void OTP_ERROR_2(void)
{
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除
	{
		ShowTxt("48,20,100,OTP 数值超限，请重试或检查光感"); 	 Delay(100);
		while(1)
		{BEEP_Dudu();}
	}
}

void OTP_ERROR_3(void)
{
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除

	{
		ShowTxt("48,20,800, ");			    DelayMs(80);
		ShowTxt("48,20,100,OTP 数值异常,请正确放置光感"); 		Delay(100);
		while(1)
		{BEEP_Dudu();}
	}
}
void OTP_ERROR_4(void)
{
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除
	
	ShowTxt("48,20,700, ");			    DelayMs(80);
	ShowTxt("48,20,700,採樣值有異常");Delay(20);
	while(1)
	{BEEP_Dudu();}
}

void OTP_NO(void)
{	 
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除
	ShowTxt("48,20,99,  ");		   DelayMs(210);
	ShowTxt("48,20,100, OTP未烧入！");	  Delay(100);
	while(1);
	
}

void OTP_OK(void)
{	 
//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除
	
	ShowTxt("48,20,299, ");		   DelayMs(210);
	ShowTxt("48,20,100,OTP  OK！");	      Delay(100);
}

void OTP_adjust_result(u16 senser_val)
{	

//	Forward_scan();		DelayMs(210);//正掃，該函數可選擇性去除

	ShowTxt("48,20,299, ");		   DelayMs(80);
	ShowTxt("32,0,50, sener值");	  				 DelayMs(150);
	ShowData("32,150,50",senser_val) ;		   Delay(150);
	ShowTxt("32,0,0, VCOM值");	  				   DelayMs(150);
	ShowData_hex("32,150,0",VCOMDC1) ;		       Delay(150);
}

/*******************************************************************************
* Function Name  : void VCOM_CHECK(u8 vcom)   
* Description    : vcom 烧入复判
* Input          : vcom
* Output         : None
* Return         : None
*******************************************************************************/  

void VCOM_CHECK(u8 vcom)     
{
	  u8 OTP_VALUE2=0;  //vcom階次  
	  
	  Delay(10);
//	  MIPI_SPI_Write( 4,  0xFF, 0x61, 0x36, 0x01);
	  Delay(10);
    MIPI_SPI_Write( 2,  0xE0, 0x01);
    MIPI_SPI_Write( 2,  0x00, 0x00);
	  READ_IC(VCOM_register,0x00); 
	  OTP_VALUE2 =  MIPI_READ_DATA[0]; 
	  Delay(10);
	

	  VCOM_TIMES_Check(VCOM_Read_reg);
		   ///VCOM VALUE 
		SSD2828_VIDEO_MODE_HS();		  DelayMs(150);
	
		if(OTP_VALUE2 == vcom)
		{	
			showbmp (Flicker_OTP);	    DelayMs(150);
			OTP_OK();
		}
		else
		{					
			OTP_ERROR_1();   
		}	

}
/*******************************************************************************
* Function Name  : void CHECK_ID(void)
* Description    : ID 烧入复判
* Input          : None
* Output         : None
* Return         : ID_OK
*******************************************************************************/  
void CHECK_ID_again(void)
{
	  u8 mtp_count=0;  //ID判斷次數
  
   	while(Only_check_id() == 1)
		{		
			SSD2828_ENTER_LP_mode();
			MTP_ID();
      reset_lcd();	

			if(mtp_count>=1)
			{
	      Allcolor(1,0xFFFFFF);		   DelayMs(650);		//////WHITE		
				OTP_ID_NG();
				while(1)
			  {BEEP_Dudu();}	
			}
			DelayMs(100);
			mtp_count++;
		}
}


/*******************************************************************************
* Function Name  : u8 ID_CHECK(void)
* Description    : ID 燒入判斷
* Input          : None
* Output         : None
* Return         : ID_OK
*******************************************************************************/  

u8 ID_CHECK(void)
{
	
//	u8 ID1_READ,ID2_READ,ID3_READ;

//-----------------------------------------------------------------------------------	
//	READ_IC(ID1_register,0x00);   //回读ID1值
//	ID1_READ =   MIPI_READ_DATA[0];
//
//	READ_IC(ID2_register,0x00);   //回读ID2值
//	ID2_READ =   MIPI_READ_DATA[0];
//
//	READ_IC(ID3_register,0x00);   //回读ID3值
//	ID3_READ =   MIPI_READ_DATA[0];
//  MIPI_SPI_Write( 4,  0xFF, 0x61, 0x36, 0x01);  //该型号回读之前需切换page
	
    MIPI_SPI_Write( 2,  0x51, 0x03);  //该型号回读之前需切换page
    
//     MIPI_SPI_Write( 2,  0x00, 0x00); 
    READ_IC(VCOM_register,0x00);   //回读ID3值
	OTP_VALUE1 =   MIPI_READ_DATA[0];
	OTP_VALUE2 =   MIPI_READ_DATA[1];
//	VCOM_TIMES_Check(VCOM_Read_reg);
//-----------------------------------------------------------------------------------	
//	READ_IC(ID_register,  0x00);   //回读ID值
	SSD2828_VIDEO_MODE_HS();		  DelayMs(150);
	
//	if((MIPI_READ_DATA[0] == ID1_VALUE)&&(MIPI_READ_DATA[1] == ID2_VALUE)&&(MIPI_READ_DATA[2] == ID3_VALUE))
//	{
//		OTP_ID_OK();
//		ID_OK = 0;   //若ID已燒，返回0 
//	}
//	else
//	{
//		ID_OK = 1;
//	}
//	return ID_OK;
////------------------------------------------------------------------------------------	
}


void Check_once_reg(u8 reg_data,u8 reg_num)
{
	  unsigned char temp;
	  temp=MIPI_READ_DATA[0];
	  while((temp!=reg_data)&&(checktimes<5))
			{
				checktimes++;
				READ_IC_Once(reg_num,0x00);
				temp=MIPI_READ_DATA[0];
      }
		if((temp!=reg_data))
			{
					Only_STM32_Init_SSD2828();
					SSD2828_VIDEO_MODE_HS();
					ShowTxt("48,20,299, ");	  				             DelayMs(150);
					Allcolor(1,0xff00F0);		     DelayMs(150);//刷红色
				
				  ShowTxt      ("36,0,60, 应烧寄存器编号：");	  	DelayMs(120);
					ShowData_hex ("36,350,60", reg_num);  DelayMs(150); 
				  ShowTxt      ("36,0,120, 应烧寄存器值：");	  	DelayMs(120);
					ShowData_hex ("36,350,120", reg_data);  DelayMs(120); 
				  ShowTxt      ("36,0,180, 实际读取值：");	  	DelayMs(120);
					ShowData_hex ("36,350,180", temp);  DelayMs(120); 
					ShowTxt("50,100,600,OTP烧入异常！");	  	DelayMs(150); 
					while(1)
					{
						//BEEP_Dudu();
						GPIO_SetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  on
					  Delay(900);
					// GPIO_ResetBits(GPIOC, GPIO_Pin_1); 	 /////////beep  on
						//Delay(10);
					}	
			}
			
}

unsigned char MTP_Recogn(void)
{	
   SSD2828_ENTER_LP_mode();DelayMs(100);
//#=========Internal VGH/VGL(4C)========	  0x00
	 MIPI_SPI_Write(4 , 0xFF, 0x61 ,0x36, 0x08);  
	checktimes=0;  
	READ_IC_Once(0x4c,0x00); 
   Check_once_reg(0x00,0x4c);
//以下指令均位于Page7	
//#======GIP correction CLK Falling=======	0x05
//#GIP correction CLK Falling
	 MIPI_SPI_Write(4,  0xFF ,0x61, 0x36, 0x07); 
	 checktimes=0;  READ_IC_Once(0x1A,0x00);
	 Check_once_reg(0x05,0x1A);
//#======GIP correction CLK Rising======= 	0x1f
//#GIP correction RST Rising
	 checktimes=0;  READ_IC_Once(0x0B,0x00);	
	 Check_once_reg(0x1f,0x0B);
//#======GIP correction STV Falling=======  0x05
//#GIP correction STV Falling
	 checktimes=0;  READ_IC_Once(0x0D,0x00);
   Check_once_reg(0x05,0x0D);	
//#======GIP correction STV Rising=======	  0x03
//#GIP correction STV Rising
	checktimes=0;  READ_IC_Once(0x0A,0x00);	
	Check_once_reg(0x03,0x0A);
//#======GIP correction RST Falling======	   0x35
//#GIP correction RST Falling
	checktimes=0;  READ_IC_Once(0x0E,0x00);
  Check_once_reg(0x35,0x0E);	
//#GIP correction CLK Rising                0X1F
	checktimes=0;  READ_IC_Once(0x16,0x00);
	Check_once_reg(0x1f,0x16);
//#======GIP correction RST Rising======   0X1F
//#GIP correction CLK Rising
	checktimes=0;  READ_IC_Once(0x17,0x00); 
	Check_once_reg(0x1f,0x17);
//#======GIP STV width ======	
//####GIP correction STV width          0XEB
  checktimes=0;  READ_IC_Once(0x1C,0x00);
	Check_once_reg(0xeb,0x1C);
//#======GIP Rising(18)======	     0X05
//#GIP Rising 
	checktimes=0;  READ_IC_Once(0x18,0x00);
	Check_once_reg(0x05,0x18);
//#======GIP Rising(19)======	   0X00
//#GIP falling 
	checktimes=0;  READ_IC_Once(0x19,0x00);
	Check_once_reg(0x00,0x19);
	
	SSD2828_VIDEO_MODE_HS();
	MDelay(100);
	//ShowTxt("80,20,100,MTP_OK");	  				       DelayMs(1500);
}


u8 Only_check_id(void)
{
 //-----------------------------------------------------------------------------------	
	READ_IC(ID_register,  0x00);   //回读ID值	
	if((MIPI_READ_DATA[0] == ID1_VALUE)&&(MIPI_READ_DATA[1] == ID2_VALUE)&&(MIPI_READ_DATA[2] == ID3_VALUE))
	{
		ID_OK = 0;   //若ID已燒，返回0 
	}
	else
	{
		ID_OK = 1;
	}
	return ID_OK;
//------------------------------------------------------------------------------------	
}                             

u8 VCOM_TIMES_Check(u8 reg)   //OTM1284A专用
{
//	MIPI_SPI_Write( 4,  0xFF, 0x61, 0x36, 0x01);
    
    MIPI_SPI_Write( 2,  0xe0, 0x01);
    
    MIPI_SPI_Write( 2,  0x00, 0x00);
	READ_IC(reg,0x00);
	OTP_TIMES= MIPI_READ_DATA[0];
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

	int sss=0,yyy=0,aaa=0,temp;	  //temp 为加的阶数
	
	for (yyy=0;yyy<kk;yyy++)
	{SURE[sss]=vcomdcdata+aaa;
	
	STM32TOSSD2828_W_COM(0xb7);		//DCS mode, LP mode
	STM32TOSSD2828_W_DATA_16BITS(0x0752);	 
	Delay(10);

	////# VCOM Setting		
//	MIPI_SPI_Write(0x04,0xb6,SURE[sss],SURE[sss],0X00);		 ////set vcom
	VCOM_set(SURE[sss]);								   ////set vcom
	Delay(10);

	STM32TOSSD2828_W_COM(0xb7);		//Generic mode, HS video mode
	STM32TOSSD2828_W_DATA_16BITS(0x070B);
	Delay(145);

	SENSOR_Check(0x52);			/////S3		
	SENSOR_Check(0x72);			/////S2	
//SENSOR_Check(0x92);			/////S1


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
				tep1=DATA_SUM1[count1];
					if(tep1<tep2)
					{
							tep2=tep1;
							 flag=1;  //此处flag再次赋值是为了在一旦再次出现更小值的情况，清除flag计数 令后续可以找到真正的最小值
					}
					else   // if (tep1>tep2)       //如果找到了最低点，则继续寻找两次(flag=1~3)
					  	flag++;
					
					if ((flag==3) & (tep2<250))    //判断是否是真的最低点，再确认是否小于预定值，如果是则停止寻找以节约时间。
					{
						for(i=(count1+1);i<STEP;i++)
							DATA_SUM1[i]=500;                     //填充剩下的数组，否则后续处理会出错
					
							yyy=kk;     //改变判断条件，使本函数提前结束
					}
			}
		  //
	 //------2014.1.27----HFZ--END------	
				
				
				
	if(DATA_SUM1[count1] <= 180)
	{temp=1; }
	else if((DATA_SUM1[count1]>180)&&(DATA_SUM1[count1] < 280))
	{temp=2; }
	else if((DATA_SUM1[count1]>=280)&&(DATA_SUM1[count1] < 480))
	{temp=3; }
	else if((DATA_SUM1[count1]>=480)&&(DATA_SUM1[count1] < 680))
	{temp=5; }	
	else
	{temp=7; }

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
		STM32TOSSD2828_W_DATA_16BITS(0x0352);
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
//	while(Sensor_check())
//	{
//		OTP_ERROR_4();	
//	}
//==========================================================================		
//		VCOMDH=VCOMDC1;    //在光感调整到最佳之后进入手动验证
//		KEY_adjust();
//      while(1);


	////////內部測試用，暫停住畫面，以便進行flicker數據收集。。
	mm_KEYB10 = KEY_PAUSE;
	while(mm_KEYB10 == 0)
	{
		mm_KEYB10 = KEY_PAUSE;	
		Delay(20);
	}
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
* Function Name  : void Flicker_for_Stable(u8 hfz_vocm)
* Description    : 自动模式延时等待flicker稳定
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void Flicker_for_Stable(u8 hfz_vocm)
{
	u16 sec=0,flk_cek=0;
	u16 mm_KEYB1 = 1 ;
	
//	ShowTxt("60,20,210,正在计时");DelayMs(150); 
//	
	for(sec=0;sec<5;sec++)
	{
	DelayMs(999);
	}
	//	flk_cek=GET_FLICKER(); //过2分半钟采集flicker值
	VCOMDC=hfz_vocm-8;
  DelayMs(150); 	
	showbmp (Flicker_OTP);				DelayMs(250); 
	I2C_DATAL_COMPAREX();

//	while(mm_KEYB1)  //等待按键按下
//	{
//	mm_KEYB1  =  	KEY_PAUSE;
//	DelayMs(5);		 
//	}
//	while(mm_KEYB1==0)  //等待按键释放，并烧录
//	{
//	mm_KEYB1  =  	KEY_PAUSE;
//	DelayMs(1);		 
//	}
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
	//u8 sdcap=0;	
	I2C_DATAL_COMPAREX();
	DelayKEY(1);
	
	Flicker_for_Stable(VCOMDC1);
	
	if(DATA_SUM1[count2] <= 250)			 ///for check OTP is OK or NG....
	{
			if(DATA_SUM1[count2] >= 3)
			{
				SSD2828_ENTER_LP_mode(); //enter  LP mode
				Delay(6);
				{
					MTP(); 	
    			DelayMs(100);
/*****************************************/					
//          if(ID_OK==1)	//若ID一次未燒				
//					{MTP_ID();}
/*****************************************/					
					reset_lcd();				
					DelayMs(100);
				}	

//--------------------------OTP _id值燒入复判--------------------------------
//				  CHECK_ID_again();				
//--------------------------OTP _VCOM值燒入复判--------------------------------				
      		VCOM_CHECK(VCOMDC1);
//-----------------------------------------------------------------------------			
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
	
  VCOMDH=VCOM_Temp;   //0X8e
	
   while(OTP_FLAG)
   {
		mm_KEYB10 = 	KEY_UP;
		mm_KEYB1  =  	KEY_PAUSE;
		mm_KEYB11  =  KEY_DOWN;

		if(mm_KEYB10 == 0)
		{
			VCOMDH++;
//			STM32TOSSD2828_W_COM(0xb7);		//DCS mode, LP mode
//			STM32TOSSD2828_W_DATA_16BITS(0x0750);	 
			Delay(5);
	    
      VCOM_set(VCOMDH);
		  Delay(15);

//			STM32TOSSD2828_W_COM(0xb7);		//Generic mode, HS video mode
//			STM32TOSSD2828_W_DATA_16BITS(0x070B);
			Delay(50);
			showbmp (Flicker_OTP);				DelayMs(150); 	   //////Flicker_OTP專用
			ShowData_hex("32,100,400",VCOMDH) ;	  DelayMs(150);
		}
		if(mm_KEYB11 == 0)
		{
			VCOMDH--;
//			STM32TOSSD2828_W_COM(0xb7);		//DCS mode, LP mode
//			STM32TOSSD2828_W_DATA_16BITS(0x0750);	 
			Delay(5);
	    	
			VCOM_set(VCOMDH);	         
		  Delay(15);
			
//			STM32TOSSD2828_W_COM(0xb7);		//Generic mode, HS video mode
//			STM32TOSSD2828_W_DATA_16BITS(0x070B);
			Delay(50);
			showbmp (Flicker_OTP);				DelayMs(150); 	   //////Flicker_OTP專用
			ShowData_hex("32,100,400",VCOMDH) ;	  DelayMs(150);
			
		}
		if(mm_KEYB1 == 0)
		{
			if(MTP_PASS==1)
			{
				SSD2828_ENTER_LP_mode(); //enter  LP mode
				Delay(3);
				VCOMDC1  = VCOMDH ;
				MTP();
				DelayMs(100);
				MTP_OVER=1;			
				reset_lcd();	
//--------------------------OTP _VCOM值燒入复判-------------------------------				
				VCOM_CHECK(VCOMDH);
//-----------------------------------------------------------------------------	
				
				showbmp (Flicker_OTP);				DelayMs(100); 	   //////Flicker_OTP專用 			
				OTP_OK();
				
				switch(OTP_TIMES)
				{
					case 0x00: ShowTxt("60,20,1000,烧入0次");	   					DelayMs(150);   break;
					case 0x01: ShowTxt("60,20,1000,烧入1次");	   					DelayMs(150);   break; 
					case 0x03: ShowTxt("60,20,1000,烧入2次");	   					DelayMs(150);   break; 
					case 0x07: ShowTxt("60,20,1000,烧入3次");	            DelayMs(150);   break;		
				}
				while(1);			
			}
			else
			{
			   OTP_FLAG = 0;	
				 Delay(2);
				 //等待按键释放
				 	while(mm_KEYB1 == 0)
					{
					mm_KEYB1 = KEY_PAUSE; 
					}
					VCOM_Temp=VCOMDH;
			}				
		}
		Delay(50);			
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
