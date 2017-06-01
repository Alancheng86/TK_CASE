/******************** (C) COPYRIGHT 2010 GIANTPLUS ********************
* File Name          : KEYSCAN_ADC.c
* Author             : YIMING
* Version            : V1.0.0
* Date               : 2011.12.15
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "keyscan_adc.h"


void Keyscan_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;				 //配置輸入
	GPIO_Init(GPIOC, &GPIO_InitStructure);
   //------------------//
   ADC_Configuration();
}
void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure);
	
	ADC_RegularChannelConfig(ADC2, ADC_Channel_12, 1, ADC_SampleTime_13Cycles5);
	
	ADC_Cmd(ADC2, ENABLE);
	
	ADC_ResetCalibration(ADC2);
	
	while(ADC_GetResetCalibrationStatus(ADC2));
	
	ADC_StartCalibration(ADC2);
	
	while(ADC_GetCalibrationStatus(ADC2));
	

}
//-----?值返回 0??按下，1/2/3/5/6/7/8值正常---//
u8 Keyscan(void)
{
	u8 keynum;
	u16 AD_value,temps,AD_VOL;
	
	temps=ADC_LIVES;
	AD_VOL=330;
	keynum=0;
	while(temps--)
	{
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);
        AD_value=ADC_GetConversionValue(ADC2);
		
//		if(((AD_value)*AD_VOL/4096==0)&&((( AD_value)*AD_VOL/4096) < 10))
//         keynum=0;
   if(  (((AD_value)*AD_VOL/4096) == 0 )&&(((AD_value)*AD_VOL/4096) < 41))
      keynum=1;
   if(  (((AD_value)*AD_VOL/4096) > 41)&&(((AD_value)*AD_VOL/4096) < 83))
      keynum=2;
   if(  (((AD_value)*AD_VOL/4096) > 83)&&(((AD_value)*AD_VOL/4096) < 124))    
      keynum=3;
   if(  (((AD_value)*AD_VOL/4096) > 124)&&(((AD_value)*AD_VOL/4096) < 167))
      keynum=4;
      if(  (((AD_value)*AD_VOL/4096) > 167)&&(((AD_value)*AD_VOL/4096) < 206))
      keynum=5;
   if(  (((AD_value)*AD_VOL/4096) > 206)&&(((AD_value)*AD_VOL/4096) < 247))
      keynum=6;
   if(  (((AD_value)*AD_VOL/4096) > 247)&&(((AD_value)*AD_VOL/4096) < 289))
      keynum=7;
   if(  (((AD_value)*AD_VOL/4096) > 289)&&(((AD_value)*AD_VOL/4096) <=330))
      keynum=8;
     ADC_SoftwareStartConvCmd(ADC2, DISABLE);
	 }
	 return(keynum);
}

//**************************temp    charge*************************//
//FUNCTION:						show tempreture   by  stm32
//DATE:								2012/1/3
//BY :									
//******************************tem****************************
void temp_config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5);
		
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_TempSensorVrefintCmd(ENABLE);

	ADC_ResetCalibration(ADC1);
	
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	ADC_StartCalibration(ADC1);
	
	while(ADC_GetCalibrationStatus(ADC1));
}

//*************************************************************//
u8 Tempscan(void)
{
	u16 AD_value,temps;
	u32 Vtemp_sensor,Current_Temp,sdcap;
	u8 TCH[6];
	temps=ADC_LIVES;

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while(temps--)
	{
        AD_value=ADC_GetConversionValue(ADC1);

		Vtemp_sensor=(AD_value*330)/4096;
		Current_Temp=((142-Vtemp_sensor)*1000)/435+2500;
		Current_Temp=Current_Temp/100;

		sdcap=Current_Temp;
		TCH[3]=0;
/*		
		sdcap=Current_Temp;
	   TCH[3]=0;
	   TCH[2]=(sdcap%10+0x30);sdcap/=10;
	   TCH[1]=(sdcap%10+0x30);sdcap/=10;
	   TCH[0]=(sdcap%10+0x30);
	   LCD_DisplayStringLine(95,100,TCH);

	   sdcap=Vtemp_sensor;
	   TCH[4]=0;
	   TCH[3]=(sdcap%10+0x30);sdcap/=10;
	   TCH[2]=(sdcap%10+0x30);sdcap/=10;
	   TCH[1]=(sdcap%10+0x30);sdcap/=10;
	   TCH[0]=(sdcap%10+0x30);
	   LCD_DisplayStringLine(95,150,TCH);

	   sdcap=AD_value;
	   TCH[5]=0;
	   TCH[4]=(sdcap%10+0x30);sdcap/=10;
	   TCH[3]=(sdcap%10+0x30);sdcap/=10;
	   TCH[2]=(sdcap%10+0x30);sdcap/=10;
	   TCH[1]=(sdcap%10+0x30);sdcap/=10;
	   TCH[0]=(sdcap%10+0x30);
	   LCD_DisplayStringLine(95,200,TCH);
*/
	   }	
	ADC_SoftwareStartConvCmd(ADC1, DISABLE);	
 	return (u32)Current_Temp;
}
	
	

u16 ADC_VOL_value(void)
{
	u16 ADC_get_vol_value;
	u16 AD_value,temps,AD_VOL;
	

	Keyscan_init();

	temps=ADC_LIVES;
	AD_VOL=330;
	ADC_get_vol_value=0;
	while(temps--)
	{
		ADC_SoftwareStartConvCmd(ADC2, ENABLE);
        AD_value=ADC_GetConversionValue(ADC2);

		ADC_get_vol_value = ((AD_value)*AD_VOL/4096) * 10;	//9.1K  :   82K   萇郯饜离
		

     ADC_SoftwareStartConvCmd(ADC2, DISABLE);
	 }
	 
//	 if((ADC_get_vol_value > vol_set*(1+10/100))	|| (ADC_get_vol_value < vol_set*(1-10/100)))
//	 {	return 1;	}
//	 else
//	 {	 return 0; 		}
	 return ADC_get_vol_value;
}