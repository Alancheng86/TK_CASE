//=========================================================================
#include <TSLIIC_REV01.h>
#include <lcd.h>
#include <math.h>
#define uchar unsigned char
//#define uint  unsigned int
#define Uchar unsigned char
#define Uint  unsigned int
void delay_10ns (u32 j);
unsigned char Read_Reg(unsigned char Device_N,unsigned char addr);

void Tsl_wait(void);
void TSL_Start(void);
void TSL_Stop(void);
void Send_TAck(unsigned char ack) ;          //确认接收
unsigned char Test_TAck(uchar tep) ;
unsigned char  Bri_Seg[4];
void TSL_SDA_FLOAT(void);
void TSL_SDA_OUTPUT(void);
void TSL_SentByte(unsigned char bytedata);
unsigned char TSL_ReceiveByte(void); 
unsigned char TSL_WriteReg(unsigned char Device_N,unsigned char addr,unsigned char regdata);
unsigned char Read_RegMuti(unsigned char Device_N,unsigned char *p);
unsigned int  Cal_culateLux(unsigned int iGain, unsigned int tInt, unsigned int ch0,unsigned int ch1);
void TSL2581Init_Bri(unsigned char Device_N);


#define S1_addr 0X92
#define S2_addr 0X72
#define S3_addr 0X52 

#define TSL2581
//#define TSL2560

//=================================================================================
void TSL_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = PIN_SCL | PIN_SDA;
    GPIO_Init(GPIO_Sensor, &GPIO_InitStructure);
}

void delay_10ns (u32 k)
{
  volatile u32 i,j;
  for (i=0; i<k; i++)
  {
     for (j=0; j<1; j++);			//32
  }
}

//---------------------------------
void Tsl_wait(void)
{
  delay_10ns(1);		   //11
}
//------------------------------------------

void TSL_SDA_FLOAT(void)
            {GPIO_InitTypeDef QGPIO_InitStructure;
             QGPIO_InitStructure.GPIO_Pin = PIN_SDA;
             QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;//IN_FLOATING;
             GPIO_Init(GPIO_Sensor, &QGPIO_InitStructure);
            } 
void TSL_SDA_OUTPUT(void)
       { GPIO_InitTypeDef QGPIO_InitStructure;
         QGPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
         QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
         QGPIO_InitStructure.GPIO_Pin =  PIN_SDA;
         GPIO_Init(GPIO_Sensor, &QGPIO_InitStructure);
       }
/********************************************/

void TSL_Start(void)
{ TzSDA_H;
	TzSCL_H;
	Tsl_wait();
	TzSDA_L;Tsl_wait();
	TzSCL_L;
	Tsl_wait();
}
/********************************************/
void TSL_Stop(void)
{	
	TzSCL_L;
	TzSDA_L;
	  
    TzSCL_H; Tsl_wait();
    TzSDA_H;
    TzSCL_L; Tsl_wait();
}
/********************************************/
void Send_TAck(uchar ack)           //确认接收
{ 
	
	if (ack)
	     TzSDA_H;
	else 
             TzSDA_L;
        TzSCL_H;
 	Tsl_wait();
	TzSCL_L;
	Tsl_wait();
}
uchar Test_TAck(uchar tep)           
{uchar ack=0;uchar flag;
  TSL_SDA_FLOAT();  
  flag=tep;
  TzSCL_H;
	Tsl_wait();
	if (GPIO_ReadInputDataBit(GPIO_Sensor, PIN_SDA))
	//if(TSDA)
		ack=1;
	TzSCL_L;
        if (flag==0)  //若flag为0，则意味接下来继续处于发送状态，所以还原端口为输出状态
	TSL_SDA_OUTPUT();
	return ack;
}
void TSL_SentByte(unsigned char bytedata)          //传送一个字节数据
{    unsigned char i=8;
	do{
		//TSDA=bytedata&0x80;
		if (bytedata&0x80)
			 TzSDA_H;
			 else 
			 	TzSDA_L;
		TzSCL_H;
		Tsl_wait();
		bytedata<<=1;
		TzSCL_L;
                if (i>1) //判断是否传递最后一位数据，若是最后一位则TSCL拉低后不用等待
		Tsl_wait();
    	}while(--i);
}
unsigned char TSL_ReceiveByte(void)             				//接收一个字节数据
{	unsigned char x=0,i=8;
	
  TSL_SDA_FLOAT();
	do
	{	TzSCL_H;
		Tsl_wait();
		x<<=1;
		if(GPIO_ReadInputDataBit(GPIO_Sensor, PIN_SDA))
			x++;
		TzSCL_L;
                if (i>1) //判断是否传递最后一位数据，若是最后一位则TSCL拉低后不用等待
		Tsl_wait();
	 }while(--i);
	 
	TSL_SDA_OUTPUT();
	return(x);
}



unsigned char TSL_WriteReg(unsigned char Device_N,unsigned char addr,unsigned char regdata)
{unsigned char sign,device_n;
        device_n=Device_N;
		sign=0;
		TSL_Start();
		TSL_SentByte(device_n);
		if(Test_TAck(0))
			sign+=0x01;
		TSL_SentByte(0x80|addr); //Command Register+addr
		if(Test_TAck(0))
			sign+=0x02;
		TSL_SentByte(regdata);
		if(Test_TAck(0))
			sign+=0x04;
		TSL_Stop();
		return sign;

}
unsigned char Read_Reg(unsigned char Device_N,unsigned char addr)
{
	unsigned char sign=0,device_n;
        device_n=Device_N;
	TSL_Start();
	TSL_SentByte(device_n);
	if(Test_TAck(0))
		sign+=0x01;
	TSL_SentByte(0x80+addr);
	if(Test_TAck(0))
		sign+=0x02;
	TSL_Start();
	TSL_SentByte(device_n+1);
	if(Test_TAck(1))
		sign+=0x04;
	sign=TSL_ReceiveByte();
	
	Send_TAck(1);
	TSL_Stop();
    return(sign);
}
unsigned char Read_RegMuti(unsigned char Device_N,unsigned char *p)
{
	unsigned char sign=0;
#ifdef TSL2560
	unsigned char addr=12;		   /////TSL2561/2560
#endif
#ifdef TSL2581
	unsigned char addr=0x14;		   /////TSL2581/2583
#endif
	unsigned char i=4;
	unsigned char device_n=Device_N;
       	TSL_Start();
	TSL_SentByte(device_n);
	if(Test_TAck(0))
		sign+=0x01;
#ifdef TSL2560
	TSL_SentByte(0x80+addr);			/////TSL2561/2560
#endif

#ifdef TSL2581
	TSL_SentByte(0xA0+addr);		    /////TSL2581/2583
#endif
	if(Test_TAck(0))
		sign+=0x02;
	TSL_Start();
	TSL_SentByte(device_n+1);
	if(Test_TAck(1))
		sign+=0x04;
	for(;;)
	{
		*p=TSL_ReceiveByte();
		p++;
		i--;
		if(!i) 
			break;
		Send_TAck(0); //?
	}
	Send_TAck(1);
	TSL_Stop();
    return(sign);
}
		

///////TSL2561/2560
//#ifdef TSL2560
///*
//For 0    < CH1/CH0 =< 0.50  Lux = 0.0304 * CH0 - 0.062 * CH0 * ((CH1/CH0)1.4)
//For 0.50 < CH1/CH0 =< 0.61 	Lux = 0.0224 * CH0 - 0.031 * CH1
//For 0.61 < CH1/CH0 =< 0.80 	Lux = 0.0128 * CH0 - 0.0153 * CH1
//For 0.80 < CH1/CH0 =< 1.30 	Lux = 0.00146* CH0 - 0.00112* CH1
//For 		CH1/CH0 > 1.30  Lux = 0
//*/
//#define LUX_SCALE 14 // scale by 2^14
//#define RATIO_SCALE 9 // scale ratio by 2^9
//#define CH_SCALE 10 // scale channel values by 2^10
//#define CHSCALE_TINT0 0x7517 // 322/11 * 2^CH_SCALE
//#define CHSCALE_TINT1 0x0fe7 // 322/81 * 2^CH_SCALE
////...................................................
//// T Package coefficients
////...................................................
//// For Ch1/Ch0=0.00 to 0.50
//// Lux/Ch0=0.0304-0.062*((Ch1/Ch0)^1.4)
//// piecewise approximation
//// For Ch1/Ch0=0.00 to 0.125:
//// Lux/Ch0=0.0304-0.0272*(Ch1/Ch0)
////
//// For Ch1/Ch0=0.125 to 0.250:
//// Lux/Ch0=0.0325-0.0440*(Ch1/Ch0)
////
//// For Ch1/Ch0=0.250 to 0.375:
//// Lux/Ch0=0.0351-0.0544*(Ch1/Ch0)
////
//// For Ch1/Ch0=0.375 to 0.50:
//// Lux/Ch0=0.0381-0.0624*(Ch1/Ch0)
////
//// For Ch1/Ch0=0.50 to 0.61:
//// Lux/Ch0=0.0224-0.031*(Ch1/Ch0)
////
//// For Ch1/Ch0=0.61 to 0.80:
//// Lux/Ch0=0.0128-0.0153*(Ch1/Ch0)
////
//// For Ch1/Ch0=0.80 to 1.30:
//// Lux/Ch0=0.00146-0.00112*(Ch1/Ch0)
////
//// For Ch1/Ch0>1.3:
//// Lux/Ch0=0
////...................................................
//#define K1T 0x0040 // 0.125 * 2^RATIO_SCALE
//#define B1T 0x01f2 // 0.0304 * 2^LUX_SCALE
//#define M1T 0x01be // 0.0272 * 2^LUX_SCALE
//#define K2T 0x0080 // 0.250 * 2^RATIO_SCALE
//#define B2T 0x0214 // 0.0325 * 2^LUX_SCALE
//#define M2T 0x02d1 // 0.0440 * 2^LUX_SCALE
//#define K3T 0x00c0 // 0.375 * 2^RATIO_SCALE
//#define B3T 0x023f // 0.0351 * 2^LUX_SCALE
//#define M3T 0x037b // 0.0544 * 2^LUX_SCALE
//#define K4T 0x0100 // 0.50 * 2^RATIO_SCALE
//#define B4T 0x0270 // 0.0381 * 2^LUX_SCALE
//#define M4T 0x03fe // 0.0624 * 2^LUX_SCALE
//#define K5T 0x0138 // 0.61 * 2^RATIO_SCALE
//#define B5T 0x016f // 0.0224 * 2^LUX_SCALE
//#define M5T 0x01fc // 0.0310 * 2^LUX_SCALE
//#define K6T 0x019a // 0.80 * 2^RATIO_SCALE
//#define B6T 0x00d2 // 0.0128 * 2^LUX_SCALE
//#define M6T 0x00fb // 0.0153 * 2^LUX_SCALE
//#define K7T 0x029a // 1.3 * 2^RATIO_SCALE
//#define B7T 0x0018 // 0.00146 * 2^LUX_SCALE
//#define M7T 0x0012 // 0.00112 * 2^LUX_SCALE
//#define K8T 0x029a // 1.3 * 2^RATIO_SCALE
//#define B8T 0x0000 // 0.000 * 2^LUX_SCALE
//#define M8T 0x0000 // 0.000 * 2^LUX_SCALE
//// lux equation approximation without floating point calculations
////////////////////////////////////////////////////////////////////////////////
//// Routine: unsigned int Cal_culateLux(unsigned int ch0, unsigned int ch0, int iType)
////
//// Description: Calculate the approximate illuminance (lux) given the raw
//// channel values of the TSL2560. The equation if implemented
//// as a piece.wise linear approximation.
////
//// Arguments: unsigned int iGain . gain, where 0:1X, 1:16X
//// unsigned int tInt . integration time, where 0:13.7mS, 1:100mS, 2:402mS,
//// 3:Manual
//// unsigned int ch0 . raw channel value from channel 0 of TSL2560
//// unsigned int ch1 . raw channel value from channel 1 of TSL2560
//// unsigned int iType . package type (T or CS)
////
//// Return: unsigned int . the approximate illuminance (lux)
////
////////////////////////////////////////////////////////////////////////////////
//unsigned int Cal_culateLux(unsigned int iGain, unsigned int tInt, unsigned int ch0,unsigned int ch1)
//{
////........................................................................
//// first, scale the channel values depending on the gain and integration time
//// 16X, 402mS is nominal.
//// scale if integration time is NOT 402 msec
//unsigned long chScale;
//unsigned long channel1;
//unsigned long channel0;
//unsigned long temp;
//unsigned long lux;
//unsigned long ratio1 = 0;
//unsigned long ratio;//
//unsigned int b, m;
//	switch (tInt)
//		{
//		case 0: chScale = CHSCALE_TINT0;
//				break;						// 13.7 msec
//		case 1: chScale = CHSCALE_TINT1;
//				break;						// 101 msec
//		default:chScale = (1 << CH_SCALE);
//				break;						// assume no scaling
//		}
//	if (!iGain) 							// scale if gain is NOT 16X
//		chScale = chScale << 4; 			// scale 1X to 16X
//											// scale the channel values
//	channel0 = (ch0 * chScale) >> CH_SCALE;
//	channel1 = (ch1 * chScale) >> CH_SCALE;
////........................................................................
//// find the ratio of the channel values (Channel1/Channel0)
//// protect against divide by zero
//	if (channel0 != 0) 
//		ratio1 = (channel1 << (RATIO_SCALE+1)) / channel0;
//	ratio = (ratio1 + 1) >> 1;					// round the ratio value

//	if ((ratio > 0) && (ratio <= K1T))			// is ratio <= eachBreak ?
//		{b=B1T; m=M1T;}
//	else 
//		if (ratio <= K2T)
//			{b=B2T; m=M2T;}
//		else 
//			if (ratio <= K3T)
//				{b=B3T; m=M3T;}
//			else 
//				if (ratio <= K4T)
//					{b=B4T; m=M4T;}
//				else 
//					if (ratio <= K5T)
//						{b=B5T; m=M5T;}
//					else
//						if (ratio <= K6T)
//							{b=B6T; m=M6T;}
//						else
//							if (ratio <= K7T)
//								{b=B7T; m=M7T;}
//							else 
//								if (ratio > K8T)
//									{b=B8T; m=M8T;}
//	//temp = ((channel0 * b) - (channel1 * m));
//        if(((channel0 * b) < (channel1 * m)))
//          temp=0;
//        else
//          temp = ((channel0 * b) - (channel1 * m));
//	//if (temp < 0) 							// do not allow negative lux value
//	//	temp = 0;
//	temp += (1 << (LUX_SCALE-1));			// round lsb (2^(LUX_SCALE.1))

//	lux = temp >> LUX_SCALE;				// strip off fractional portion
//	return(lux);
//}
////-----------
//#endif
 


#ifdef TSL2581
//****************************************************************************
//
// Copyright 2004?2008 TAOS, Inc.
//
// THIS CODE AND INFORMATION IS PROVIDED ”AS IS” WITHOUT WARRANTY OF ANY
// KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
// PURPOSE.
//
// Module Name:
// lux.cpp
//
//****************************************************************************
#define LUX_SCALE 16 // scale by 2^16
#define RATIO_SCALE 9 // scale ratio by 2^9
//---------------------------------------------------
// Integration time scaling factors
//---------------------------------------------------
#define CH_SCALE 16 // scale channel values by 2^16
#define NOM_INTEG_CYCLE 148 // Nominal 400 ms integration.  See Timing Register
//---------------------------------------------------
// Gain scaling factors
//---------------------------------------------------
#define CH0GAIN128X 107 // 128X gain scalar for Ch0
#define CH1GAIN128X 115 // 128X gain scalar for Ch1
//---------------------------------------------------
// FN Package coefficients
//---------------------------------------------------
// For Ch1/Ch0=0.00 to 0.30:
//           Lux=0.130*Ch0?0.240*Ch1
//
// For Ch1/Ch0=0.30 to 0.38:
//           Lux=0.1649*Ch0?0.3562*Ch1
//
// For Ch1/Ch0=0.38 to 0.45:
//           Lux=0.0974*Ch0?0.1786*Ch1
//
// For Ch1/Ch0=0.45 to 0.54:
//           Lux=0.062*Ch0?0.10*Ch1
//
// For Ch1/Ch0>0.54:
//           Lux/Ch0=0
//
//???????????????????????????????????????????????????
#define K1C 0x009A // 0.30 * 2^RATIO_SCALE
#define B1C 0x2148 // 0.130 * 2^LUX_SCALE
#define M1C 0x3d71 // 0.240 * 2^LUX_SCALE
#define K2C 0x00c3 // 0.38 * 2^RATIO_SCALE
#define B2C 0x2a37 // 0.1649 * 2^LUX_SCALE
#define M2C 0x5b30 // 0.3562 * 2^LUX_SCALE
#define K3C 0x00e6 // 0.45 * 2^RATIO_SCALE
#define B3C 0x18ef // 0.0974 * 2^LUX_SCALE
#define M3C 0x2db9 // 0.1786 * 2^LUX_SCALE
#define K4C 0x0114 // 0.54 * 2^RATIO_SCALE
#define B4C 0x0fdf // 0.062 * 2^LUX_SCALE
#define M4C 0x199a // 0.10 * 2^LUX_SCALE
#define K5C 0x0114 // 0.54 * 2^RATIO_SCALE
#define B5C 0x0000 // 0.00000 * 2^LUX_SCALE
#define M5C 0x0000 // 0.00000 * 2^LUX_SCALE

// lux equation approximation without floating point calculations
//////////////////////////////////////////////////////////////////////////////
// Routine: unsigned int Cal_culateLux(unsigned int ch0, unsigned int ch0, int iType)
//
// Description: Calculate the approximate illuminance (lux) given the raw
//              channel values of the TSL2583. The equation if implemented
//              as a piece?wise linear approximation.
//
// Arguments: unsigned int iGain ? gain, where 0:1X, 1:8X, 2:16X, 3:128X
//            unsigned int tIntCycles ? INTEG_CYCLES defined in Timing Register
//            unsigned int ch0 ? raw channel value from channel 0 of TSL2583
//            unsigned int ch1 ? raw channel value from channel 1 of TSL2583
//            unsigned int iType ? package type (1:CS)
//
// Return:    unsigned int ? the approximate illuminance (lux)
//
//////////////////////////////////////////////////////////////////////////////
unsigned int Cal_culateLux(unsigned int iGain, unsigned int tIntCycles, unsigned int ch0,
unsigned int ch1)
{
//????????????????????????????????????????????????????????????????????????
// first, scale the channel values depending on the gain and integration time
// 1X, 400ms is nominal setting
	unsigned long chScale0;
	unsigned long chScale1;
	unsigned long channel1;
	unsigned long channel0;
	unsigned long temp;
	unsigned long lux;
	unsigned long ratio1 = 0;
	unsigned int b, m;
	unsigned long ratio = (ratio1 + 1) >> 1;

// No scaling if nominal integration (148 cycles or 400 ms) is used
	if (tIntCycles == NOM_INTEG_CYCLE)
		chScale0 = (1 << CH_SCALE);
	else
		chScale0 = (NOM_INTEG_CYCLE << CH_SCALE) / tIntCycles;
	switch (iGain)
	{
		case 0: // 1x gain
		chScale1 = chScale0;     // No scale.  Nominal setting
		break;
		case 1: // 8x gain
		chScale0 = chScale0 >> 3;  // Scale/multiply value by 1/8
		chScale1 = chScale0;
		break;
		case 2: // 16x gain
		chScale0 = chScale0 >> 4;  // Scale/multiply value by 1/16
		chScale1 = chScale0;
		break;
		case 3: // 128x gain
		chScale1 = chScale0 / CH1GAIN128X; //Ch1 gain correction factor applied
		chScale0 = chScale0 / CH0GAIN128X; //Ch0 gain correction factor applied
		break;
	}
// scale the channel values
	channel0 = (ch0 * chScale0) >> CH_SCALE;
	channel1 = (ch1 * chScale1) >> CH_SCALE;
//????????????????????????????????????????????????????????????????????????
// find the ratio of the channel values (Channel1/Channel0)
// protect against divide by zero

	if (channel0 != 0) ratio1 = (channel1 << (RATIO_SCALE+1)) / channel0;
// round the ratio value

// is ratio <= eachBreak?

//	switch (iType)
	{
//		case 1: // CS package
		if ((ratio >= 0) && (ratio <= K1C))
			{b=B1C; m=M1C;}
		else if (ratio <= K2C)
			{b=B2C; m=M2C;}
		else if (ratio <= K3C)
			{b=B3C; m=M3C;}
		else if (ratio <= K4C)
			{b=B4C; m=M4C;}
		else if (ratio > K5C)
			{b=B5C; m=M5C;}
//		break;
	}

	temp = ((channel0 * b) - (channel1 * m));
	// round lsb (2^(LUX_SCALE?1))
	temp += (1 << (LUX_SCALE-1));
	// strip off fractional portion
	lux = temp >> LUX_SCALE;
	
	return(lux);
}
#endif

void TSL_Init(unsigned char Device_N)
{

	TSL_WriteReg(Device_N,0,0x01);//resv ,resv,resv,resv,gain,manual,resv,integ0,integ1
	delay_10ns(300);
	TSL_WriteReg(Device_N,7,0x03);			 //
	TSL_WriteReg(Device_N,1,0xff);			 //0xff	 ed
	TSL_WriteReg(Device_N,0,0x03);			 //0x03
	delay_10ns(300);

}
unsigned char  Sensor_Check(unsigned char Device_N)
{
	uchar tep1,tep2;
/*----------for TSL2583/1 use------------*/

		
		tep1=Read_Reg(Device_N,0)&0x0f;
		tep2=Read_Reg(Device_N,0x12);				 ////0x93
		tep2=Read_Reg(Device_N,0x1e)&0x3f;				/////1eh   -----sensor ID
		if ((tep1==0x03)&&(tep2==0x00))			 ////TSL2581
			return 0;
		else if ((tep1==0x03)&&(tep2==0x30))	 ////TSL2583
			return 0; 
		else
			return 1;
}


//===============================================================
unsigned int Get_Bright(unsigned char Device_N)
{
   unsigned int ch0,ch1;
//   u16 temp1,temp2,data1,data2;
//   u16 data=0;
   u8 temp=0;

	Read_RegMuti(Device_N,Bri_Seg);
	ch0=Bri_Seg[1]*256+Bri_Seg[0];
	ch1=Bri_Seg[3]*256+Bri_Seg[2];	
  
	return Cal_culateLux(0,255,ch0,ch1);
	
//	temp=1;  //测面板亮度1倍增益
// 
//	if(temp == 1){	 return Cal_culateLux(0,255,ch0,ch1); }
//	else if(temp == 2){	 return Cal_culateLux(1,1,ch0,ch1); }
//	else
//	{
//		return Cal_culateLux(3,2,ch0,ch1);	   /////3,1,CH0,CH1
//	}
}

void TSL2581Init_Bri(unsigned char Device_N)
{
	  TSL_WriteReg(Device_N,0,0x00);//resv ,resv,resv,resv,gain,manual,resv,integ0,integ1
		delay_10ns(300);   //POWER OFF
	  TSL_WriteReg(Device_N,0,0x01);//resv ,resv,resv,resv,gain,manual,resv,integ0,integ1
		delay_10ns(300);    //POWER ON
		TSL_WriteReg(Device_N,7,0x00);			 //测量白画面亮度增益	   16*1
		delay_10ns(300);
		TSL_WriteReg(Device_N,1,0x01);			 //采样周期
		TSL_WriteReg(Device_N,0,0x03);			 //ADC EN
		delay_10ns(300);
}
void Measure_Brightness_Init(void)
{
		TSL2581Init_Bri(S1_addr);delay_10ns(500);
		TSL2581Init_Bri(S2_addr);delay_10ns(500);
	  TSL2581Init_Bri(S3_addr);delay_10ns(500);
}
//===========================================================================================
void Reset_Senser(void)
{
	  TSL_WriteReg(S1_addr,0,0x00);//resv ,resv,resv,resv,gain,manual,resv,integ0,integ1
		delay_10ns(300);
	  TSL_WriteReg(S2_addr,0,0x00);//resv ,resv,resv,resv,gain,manual,resv,integ0,integ1
		delay_10ns(300);
		TSL_WriteReg(S3_addr,0,0x00);//resv ,resv,resv,resv,gain,manual,resv,integ0,integ1
		delay_10ns(300);
}

unsigned int Get_Bright2(unsigned char Device_N)
{
   unsigned int ch0,ch1;
	Read_RegMuti(Device_N,Bri_Seg);
	ch0=Bri_Seg[1]*256+Bri_Seg[0];
	ch1=Bri_Seg[3]*256+Bri_Seg[2];
	if(ch0<3)
		{
		if(ch0==0&&ch1==0)
			TSL_Init(Device_N);
		return 1;
		}
	else
		{
		if((Bri_Seg[0]&Bri_Seg[1]&Bri_Seg[2]&Bri_Seg[3])==0xff)
			return 60000;
		else
			return Cal_culateLux(0,1,ch0,ch1);
		}
}
//-------------





