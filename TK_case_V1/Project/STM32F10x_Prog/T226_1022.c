/* Includes ------------------------------------------------------------------*/
//-----------------------------20141031-------------------------
#include <T226.h>
#include "stm32f10x.h"
#include <math.h>
#include "stm32f10x_usart.h"
#include "lib.h"
#include "platform_config.h"
#include "lcd.h"
#include "IIC.h"
#include <V_SET.h>


extern  void TWait(void); 
extern void ShowData(char *p,u16 dat);
extern void ShowData_ua2ma_4bitfloat(char *p,u16 dat);
extern void DelayMs(__IO uint32_t nTime);
extern void BACK_LIGHT_ON(void);
extern void BACK_LIGHT_OFF(void);
////////////9.1. Sample Code: Definitions
////////////These are the definitions for sample code.

extern BYTE pucData[2];
extern u16 Vshunt_limit; 

extern void VDD_ADJ_SET(u32 VDD_settig);		 //VDD_ADJ_SET ����ȡֵ��Χ270~600����Ӧ��ѹ��Χ2.7V~6V; 
extern void VDDIO_ADJ_SET(u32 vset);			 //VDDIO_ADJ_SET ����ȡֵ��Χ160~360����Ӧ��ѹ��Χ1.6V~3.6V; 
extern void VOTP_ADJ_SET(u32 vset);			 //VOTP_ADJ_SET ����ȡֵ��Χ150~2080����Ӧ��ѹ��Χ1.5V~14.1V; 
//extern void VDD_ADJ_EN(u8 vset);

u16 IDDCurrentOfSleep=0;   //���˯�ߵ�����
u16 IDDIOCurrentOfSleep=0; //���˯�ߵ�����
u8 FlagOfCurrentSleep=0;   //˯�ߵ����ɼ���־����0ʱ��ζ���Ѿ��ɼ���˯�ߵ���
//----------------------------------------
u16 IDD_UA_preset=1000;   //IDDϵ��   uA��
short int IDD_UA_decom=0; //IDD����ֵ uA��
u16 IDD_MA=1000;       //IDDϵ��   mA��
u16 IDDIO_UA=0x0003;   //��Уϵ������ģ��� 
u16 IDDIO_MA=1000;     //IDDIOϵ��   mA��
//----------------------------------------

u16 decom_a=0;
short int decom_b=0; 
u16 Preset1=0;
u16 Preset2=0;

u8 HFZ_COF=0;

u16 vddio_vol=300;
u16 vdd_vol=300;

extern u16  IDD_WORK_LIMIT_VALUE;       
extern u16	IOIDD_WORK_LIMIT_VALUE;		    
extern u16	IDD_SLEEP_LIMIT_VALUE;			 
extern u16	IOIDD_SLEEP_LIMIT_VALUE;		 

extern void USART_GPIO_Configuration(void);
extern void USART_REMAP_GPIO_Configuration(void);
extern void USART_MEASURE_DAIA(u16 data);
unsigned short Measure_IDD_once(u8 gears) ;  
u16 Measure_IDDIO_once(u8 gears); 
//// Psudo functions
void I2CSDASetInput(void); // Set I2C SDA signal as input
void I2CSDASetOutput(void); // Set I2C SDA signal as output
//=========================================================================

unsigned int M9_BUS_VOLTAGE(void)
{
	u32 temp;

	pucData[0] = 0x00;
  pucData[1] = 0x00;
	T226_I2CReceive(DEVICE_ADDRESS_M9, COMMAND_BUS_VOLTAGE_INDEX, pucData, 2);

	temp = ((pucData[0]<<8) + pucData[1])*125/100;

	return temp;	  ////mV
}

unsigned int M12_BUS_VOLTAGE(void)
{
	u32 temp;

	pucData[0] = 0x00;
  pucData[1] = 0x00;
	T226_I2CReceive(DEVICE_ADDRESS_M12, COMMAND_BUS_VOLTAGE_INDEX, pucData, 2);

	temp = ((pucData[0]<<8) + pucData[1])*125/100;

	return temp;	  ////mV
}

void T226_intion(void)
{
	T226_GPIO_Config();

   //////////////////M9    SETTING		  IDD
	pucData[0] = 0x43;
  pucData[1] = 0x27;
	T226_I2CSend(DEVICE_ADDRESS_M9, COMMAND_CONFIG_INDEX, pucData, 2);
	
	pucData[0] = 0x00;		   ////0x7FFF				  0x8001
  pucData[1] = 0x03;
	T226_I2CSend(DEVICE_ADDRESS_M9, COMMAND_MASK_INDEX, pucData, 2);

	pucData[0] = 0x01;
  pucData[1] = 0xf4;
	T226_I2CSend(DEVICE_ADDRESS_M9, COMMAND_CALIBRATION_INDEX, pucData, 2);

//	pucData[0] = Vshunt_limit>>8; 
//  pucData[1] = Vshunt_limit; 
//	T226_I2CSend(DEVICE_ADDRESS_M9, COMMAND_ALERT_LIMIT_INDEX, pucData, 2);
	
	
//	T226_I2CReceive(DEVICE_ADDRESS_M9, COMMAND_ID_INDEX, pucData, 2);	
//	//DelayMs(10);
//	if(pucData[0] != 0x22)
//	{
//		while(1)		   ///////�z�y��T226�p�ģ���ͣ��̎���߈󾯡�
//		{	BEEP_BIBI();	BEEP_ON();DelayMs(500);
//		}	
//	}
//	

//	T226_I2CReceive(DEVICE_ADDRESS_M12, COMMAND_ID_INDEX, pucData, 2);	
//	DelayMs(10);
//	if(pucData[0] != 0x22)
//	{
//		while(1)		   ///////�z�y��T226�p�ģ���ͣ��̎���߈󾯡�
//		{	BEEP_BIBI();	BEEP_ON();DelayMs(500);
//		}	
//	}
	

	//////////////////M12 IOIDD  SETTING		4127
	pucData[0] = 0x45;
  pucData[1] = 0x27;
	T226_I2CSend(DEVICE_ADDRESS_M12, COMMAND_CONFIG_INDEX, pucData, 2);

	pucData[0] = 0x00;		   ////������A���C��
    pucData[1] = 0x03;
	T226_I2CSend(DEVICE_ADDRESS_M12, COMMAND_MASK_INDEX, pucData, 2);

	pucData[0] = 0x01;
    pucData[1] = 0xF4;
	T226_I2CSend(DEVICE_ADDRESS_M12, COMMAND_CALIBRATION_INDEX, pucData, 2);

}



u16 Measure_IDD(u8 gears) 
{	
  signed short datax[200]={0};
	u8 times=0;   //��������
	u16 temp;
	u8 i,j;
	signed sum=0;
	
	if(gears==uA) //�P�]���ua�������m���ӕr������ݷ��  }
		 {	 
			   GPIO_SetBits(GPIOA, GPIO_Pin_0); 
//			 if(HFZ_COF==0)
//			 {
		     times=130;
		     DelayMs(200);	
//			 }
//       else
//			 times=10;			 
		 }
 	else { 
		      GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		
			    if(HFZ_COF==1)
			    {
		        times=130;
		        DelayMs(200);	
			    }
         else
				  {	 
		       times=12;  
		      }
		   } //��ͨ���ma�� }

	for(i=0;i<times;i++)
		 {	  
			  DelayMs(2);
    		datax[i]=Measure_IDD_once(gears);
			  datax[i+1] = 0;		 // DelayMs(2);
		 }
	
	for(i=0;i<times-1;i++)		  //����
		{
			for(j=i+1;j<times;j++)
			{
			    if(datax[i]<datax[j])
					{
					   temp=datax[i];
						 datax[i]=datax[j];
						 datax[j]=temp;						 
					}
			}
		}
	for(i=1;i<times-1;i++)		  //ȥ��С
		{
		  sum+=datax[i];
		}
		sum/=(times-2);
		temp=sum;
		
		if(temp>0x8000)
		{temp=0;}
		
		if(HFZ_COF==0)    //У׼ʱ����������mA��
		{
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		}
		return temp;	
}


unsigned short Measure_IDD_once(u8 gears)   //gears=1 -->mA�n   gears=0 -->uA�n
{
//	signed short int IDD_value=0;
    signed short  IDD_value=0;
	  u16 VSHUNT=0; 
    DelayMs(5); 
 	  T226_I2CReceive(DEVICE_ADDRESS_M9, COMMAND_SHUNT_VLOTAGE_SETTING, pucData, 2);	
	  VSHUNT=(pucData[0]<<8)+pucData[1];   //��С0x8000	

	  IDD_value=-VSHUNT;
	
	 if(gears==uA)
	 {	
   	  if (IDD_value<0)   ////ǿ����תΪ��ֵ
			 IDD_value=-IDD_value;	 		 
			 IDD_value=(IDD_UA_preset*IDD_value)/1000-IDD_UA_decom;		 
	 }
	//---------------------------------------------------------------------------------------------	 
	 else
	 {    
			if (IDD_value<0)   ////ǿ����תΪ��ֵ
			 IDD_value=-IDD_value;	 
			 IDD_value=(u16)((u32)(IDD_value*IDD_MA)/1000); 			 
   }				
    return IDD_value;
}
 

u16 Measure_IDDIO(u8 gears)
{
 	u16 data[120]={0};
	u8 times=0;   //��������
	u16 temp;
	u8 i,j;
	u32 sum=0;
	
	if(gears==uA)
		   {	GPIO_SetBits(GPIOA, GPIO_Pin_3);times=50;DelayMs(200);   } //�P�]���ua�������m���ӕr������ݷ��  
 	else {	
		      GPIO_ResetBits(GPIOA, GPIO_Pin_3);
					if(HFZ_COF==1)
			    {
		        times=130;
		        DelayMs(200);	
			    }
         else
				  {	 
		       times=12;  
		      }		      
		   } //��ͨ���ma�� }

	for(i=0;i<times;i++)
		{
    	data[i]=Measure_IDDIO_once(gears);
			data[i+1] = 0;
		}
	
	for(i=0;i<times-1;i++)		  //����
		{
			for(j=i+1;j<times;j++)
			  {
			     if(data[i]<data[j])
							{
								 temp=data[i];
								 data[i]=data[j];
								 data[j]=temp;						 
							}
			  }
		}
	for(i=1;i<times-1;i++)		  //ȥ��С
		{
		  sum+=data[i];
		}
		sum/=(times-2);
		temp=sum;
		
		if(temp>0x8000)
		{temp=0;}
		
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
		return temp;
}


u16 Measure_IDDIO_once(u8 gears)
{	 
	  u16 flagc;
    signed short IDDIO_value=0;

		DelayMs(3);
		T226_I2CReceive(DEVICE_ADDRESS_M12, COMMAND_SHUNT_VLOTAGE_SETTING, pucData, 2);	DelayMs(5); 
		IDDIO_value=(pucData[0]<<8)+pucData[1];

	  	if(gears==uA)
		  {	 
        if(IDDIO_value<0)   ////ǿ����תΪ��ֵ
				{IDDIO_value=-IDDIO_value;}  
 				
	       IDDIO_value=IDDIO_value-IDDIO_UA;		
		  }
//------------------------------------------------------------------------------------------------------		 
		 else
		 {
		    if(IDDIO_value<0)   ////ǿ����תΪ��ֵ
				{
				 IDDIO_value=-IDDIO_value;}   
		   	 IDDIO_value=(u16)((u32)(IDDIO_value*IDDIO_MA)/1000);		 
		 }	
		return IDDIO_value;
}

u8 Measure_I_5TimesForProtectIDDIO(u32 dat1)
{   u32 flagi=5*	dat1;
    u32 limit=0;
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);  //��ͨ���ma�� 
//#ifdef	MIPI_YELLOW_BOARD 
  limit= Measure_IDDIO_once(mA);
	limit+= Measure_IDDIO_once(mA);
	limit+= Measure_IDDIO_once(mA);
	limit+= Measure_IDDIO_once(mA);
	limit+= Measure_IDDIO_once(mA);
	if ( limit>flagi)
	{ VDDIO_ADJ_EN(0);			  //��ֹVDDIO���
	  VDD_ADJ_EN(0);		
	  
	  VOTP_EN(0);	
	 return 1;	
	}
//#endif 
	else return 0;
}
u8 Measure_I_5TimesForProtectIDD(u32 dat2)
{   u32 flagi=5*	dat2;
    u32 limit=0;
	 GPIO_ResetBits(GPIOA, GPIO_Pin_0); //��ͨ���ma�� 
  limit= Measure_IDD_once(mA);
	limit+= Measure_IDD_once(mA);
	limit+= Measure_IDD_once(mA);
	limit+= Measure_IDD_once(mA);
	limit+= Measure_IDD_once(mA);

	if ( limit>flagi)
	{ 			  
	  VDD_ADJ_EN(0);	 //��ֹVDD���	
	  VDDIO_ADJ_EN(0);
	  VOTP_EN(0);	
		return 1;
	}
	else return 0;
}

//===========================================================


void AUTO_CALIBRATION(void)
{ 
//	 u8 sign;   //1 wei "+"   ;  0 wei "-"
	u16 mm_KEY_PAUSE = 1;
  u16 mm_KEY_UP=1;	
	u16 mm_KEY_DOWN=1;

	u16 Tempc;
	u16 little_value;
	u8  coco=0;
	HFZ_COF=1;
	
  Tempc=EEPROMBYTE_Read(0x19); DelayMs(5);
/************ ����У׼��ѹ ��·��Ϊ3V**********************/
	VDD_ADJ_SET(vdd_vol);					
	VDD_ADJ_EN(1);
 	DelayMs(150);
	VDDIO_ADJ_SET(vddio_vol);			 
	VDDIO_ADJ_EN(1);			
	DelayMs(150);
/*******************************************/	
	
	USART_GPIO_Configuration();  //�л�Ϊ��ͨ����1ģʽ
	PA910SPISendValueofCurrent(" ,,",111);
	PA910SPISendValueofCurrent("---IDD_Calibration_Starting--:",54321);   //????????   	
	BEEP_BIBI();	  //���ȽЃ�����ʾ�M��У��
	PA910SPISendValueofCurrent("--uA-MODE1-:",0);   //????????  
/*******************************************************************/	
/*����,uA��,IDD·*/
//    GPIO_ResetBits(GPIOB, GPIO_Pin_9); 	//
		RELAY_EN(0); 
		DelayMs(150);
    VOTP_ADJ_SET(390);			  
    VOTP_EN(0); //
    DelayMs(150);		
		
		A0_EN(0);
		A1_EN(0);
/*******************************************************************/			
	 do
	 {
     Preset1=Measure_IDD(uA);  //  IDD 30uA ����	   y=ax+b
     USART_SEND_DEC(Preset1);
/****************************************
*             y1=ax1+b
****************************************/
     mm_KEY_UP=KEY_UP;	  
		 if( mm_KEY_UP==0)
		 {
			 vdd_vol++; 
       VDD_ADJ_SET(vdd_vol);					
	     VDD_ADJ_EN(1); 
//       while(mm_KEY_UP==0)
//	     {mm_KEY_UP=KEY_UP;}  	 
		 }
		 
     mm_KEY_DOWN=KEY_DOWN;
		 if( mm_KEY_DOWN==0)
		 {
			 vdd_vol--; 
       VDD_ADJ_SET(vdd_vol);					
	     VDD_ADJ_EN(1); 
//       while(mm_KEY_DOWN==0)
//	     {mm_KEY_DOWN=KEY_DOWN;}  	 
		 } 		 
		 mm_KEY_PAUSE=KEY_PAUSE;   
	 }while((mm_KEY_PAUSE!=0));	 
	 
	 
    while(mm_KEY_PAUSE==0)
	  {
			mm_KEY_PAUSE=KEY_PAUSE;
		} 
		
//    for(coco=0;coco<8;coco++)
//    {
//			 little_value=Measure_IDD(uA);
//			 if(little_value>Preset1)
//			 {Preset1=little_value;}	 	   
//		}	
		PA910SPISendValueofCurrent("-^-Preset1-^-:",Preset1);	
	  BEEP_BIBI();
   		
/****************************************
*             y2=ax2+b
****************************************/
		
		PA910SPISendValueofCurrent("--uA-MODE2-:",0);   //????????  
		A0_EN(1);
		A1_EN(0);	
		
	  do
	  {
     Preset2=Measure_IDD(uA);  //  IDD 50uA ����	
		 USART_SEND_DEC(Preset2);	
//--------------------------------------//     
       mm_KEY_UP=KEY_UP;	
			 if( mm_KEY_UP==0)
			 {
				 VDD_ADJ_SET(++vdd_vol);	  				
				 VDD_ADJ_EN(1);
//				 while(mm_KEY_UP==0)
//				 {mm_KEY_UP=KEY_UP;}  	 
			 }
		 
       mm_KEY_DOWN=KEY_DOWN;
			 if( mm_KEY_DOWN==0)
			 {
				 VDD_ADJ_SET(--vdd_vol);					
				 VDD_ADJ_EN(1); 
//				 while(mm_KEY_DOWN==0)
//				 {mm_KEY_DOWN=KEY_DOWN;}  	 
			 } 		 
		 mm_KEY_PAUSE=KEY_PAUSE;   
	  }while((mm_KEY_PAUSE!=0));	 	
		
    while(mm_KEY_PAUSE==0)
	  {
			mm_KEY_PAUSE=KEY_PAUSE;
		}  
//--------------------------------------//	
//		for(coco=0;coco<8;coco++)
//    {
//			 little_value=Measure_IDD(uA);
//			 if(little_value>Preset2)
//			 {Preset2=little_value;}	 	
//		}	
    PA910SPISendValueofCurrent("-^-Preset2-^-:",Preset2);
		
    decom_a=(u16)((abs(Preset2-Preset1)*1000)/(50-30));
		if(decom_a>1000)
		{
		  decom_a=1000;
		}	
		DelayMs(1);
		if(Preset1<Preset2)
		{
	  decom_b=Preset1-(30*decom_a+500)/1000;
		}
		else
		{
		decom_b=Preset2-(30*decom_a+500)/1000;
		}
	 
//////////////////////============24c02�洢��======		

	EEPROMBYTE_Write(0x10,decom_a>>8);   DelayMs(5); //uA��     
	EEPROMBYTE_Write(0x11,decom_a);	     DelayMs(5); 
		
  EEPROMBYTE_Write(0x12,decom_b>>8);   DelayMs(5); //uA��     
	EEPROMBYTE_Write(0x13,decom_b);	     DelayMs(5); 	
		
  if(Tempc!=0x19)
  {	
	EEPROMBYTE_Write(0x50,decom_a>>8);   DelayMs(5); //���ݱ���    
	EEPROMBYTE_Write(0x51,decom_a);	     DelayMs(5); 	 
	
  EEPROMBYTE_Write(0x52,decom_b>>8);   DelayMs(5); //���ݱ���    
	EEPROMBYTE_Write(0x53,decom_b);	     DelayMs(5); 	
  }	
 
	PA910SPISendValueofCurrent("-^-IDD_uA_pre-^-:",decom_a); 
  PA910SPISendValueofCurrent("-^-IDD_uA_dec-^-:",decom_b);
	
	BEEP_BIBI();
	 
/*******************************************************************/	
/*����,mA��,IDD·*/	
//    GPIO_SetBits(GPIOB, GPIO_Pin_9); //��300K����  ,mA��
	  RELAY_EN(1); 
		DelayMs(150);
    VOTP_ADJ_SET(390);			  
    VOTP_EN(0); //IDD·
    DelayMs(150);
    PA910SPISendValueofCurrent("--mA-MODE1-:",0);   //????????  
		A0_EN(0);
		A1_EN(1);	
/*******************************************************************/	
	 
	 do
	 {
     Preset1=Measure_IDD(mA);  //	50mA�Ļ���
		 USART_SEND_DEC(Preset1);
		 //--------------------------------------//     
     mm_KEY_UP=KEY_UP;	
		 if( mm_KEY_UP==0)
		 {
       VDD_ADJ_SET(++vdd_vol);					
	     VDD_ADJ_EN(1);
//       while(mm_KEY_UP==0)
//	     {mm_KEY_UP=KEY_UP;}  	 
		 }
		 
     mm_KEY_DOWN=KEY_DOWN;
		 if( mm_KEY_DOWN==0)
		 {
       VDD_ADJ_SET(--vdd_vol);					
	     VDD_ADJ_EN(1);  
//       while(mm_KEY_DOWN==0)
//	     {mm_KEY_DOWN=KEY_DOWN;}  	 
		 } 
//--------------------------------------//		
		 mm_KEY_PAUSE=KEY_PAUSE;  
		//�����ݷ��������� 
	 }while((mm_KEY_PAUSE!=0));   

	 
///////////////////////==========key����pass==========			 
    while(mm_KEY_PAUSE==0)
	  {
			mm_KEY_PAUSE=KEY_PAUSE;
		} 
    
//		for(coco=0;coco<8;coco++)
//    {
//			 little_value=Measure_IDD(mA);
//			 if(little_value>Preset1)
//			 {Preset1=little_value;}	 	   
//		}	
     decom_a=(u16)(5000000/(Preset1));            //50MA
		
///////////////////////============24c02�洢��======		
	 EEPROMBYTE_Write(0x14,decom_a>>8);  DelayMs(5);      
	 EEPROMBYTE_Write(0x15,decom_a);	 	DelayMs(5); 
		
   if(Tempc!=0x19)
   {			
	 EEPROMBYTE_Write(0x54,decom_a>>8);   DelayMs(5); //���ݱ���
	 EEPROMBYTE_Write(0x55,decom_a);	DelayMs(5); 			
   }				
	 VDD_ADJ_EN(0);
	 
   PA910SPISendValueofCurrent("-%-Preset1-*-:",Preset1);    //4008
	 
	 BEEP_BIBI();
		
	
//		 IDDIOһ·У׼��ʼ����
/*******************************************************************/
//    GPIO_ResetBits(GPIOB, GPIO_Pin_9); 	//uA�� 
//      RELAY_EN(0);	 
//		DelayMs(150);
//    VOTP_ADJ_SET(390);			  
//    GPIO_SetBits(GPIOA, GPIO_Pin_4);   //IDDIO·
//    DelayMs(150);
/*******************************************************************/	
//		
//	 do
//	 {
//    Preset1=Measure_IDDIO(uA);  // IDDIO  uA��	 
//		 //--------------------------------------//     
//     mm_KEY_UP=KEY_UP;	
//		 if( mm_KEY_UP==0)
//		 {
//			 vddio_vol+=5; 
//	     VDDIO_ADJ_SET(vddio_vol);			 
//	     VDDIO_ADJ_EN(1);			 
//       while(mm_KEY_UP==0)
//	     {mm_KEY_UP=KEY_UP;}  	 
//		 }
//		 
//     mm_KEY_DOWN=KEY_DOWN;
//		 if( mm_KEY_DOWN==0)
//		 {
//			 vddio_vol-=5; 
//	     VDDIO_ADJ_SET(vddio_vol);			 
//	     VDDIO_ADJ_EN(1);			 
//       while(mm_KEY_DOWN==0)
//	     {mm_KEY_DOWN=KEY_DOWN;}  	 
//		 } 
////--------------------------------------//				 
//		mm_KEY_PAUSE=KEY_PAUSE;  
//	 }while((mm_KEY_PAUSE!=0));	   	
//	 
//	 PA910SPISendValueofCurrent("-^-IOIDD_uA-^-:",Preset1);  
/////////////////////////==========key����pass==========			 
//    while(mm_KEY_PAUSE==0)
//	 {mm_KEY_PAUSE=KEY_PAUSE;} 
//	 
//    decom_a=50000/Preset1;   
//	 	
////////////////////////============24c02�洢��======			
//	 EEPROMBYTE_Write(0x14,decom_a>>8); DelayMs(5);  //uA��     
//	 EEPROMBYTE_Write(0x15,decom_a);	 	DelayMs(5);	

//	 EEPROMBYTE_Write(0x54,decom_a>>8); DelayMs(5); //���ݱ���
//	 EEPROMBYTE_Write(0x55,decom_a);	DelayMs(5); 	
//	 
//	 BEEP_BIBI();  
	 
/*******************************************************************/	
/*����,MA��,IDDIO·*/	
//		GPIO_SetBits(GPIOB, GPIO_Pin_9); 	//mA��
		RELAY_EN(1);
		DelayMs(150);
    VOTP_ADJ_SET(390);			  
    VOTP_EN(1);   //IDDIO·
    DelayMs(150);	
		PA910SPISendValueofCurrent("--mA-MODE2-:",0);   //????????  
		A0_EN(1);
		A1_EN(1);	
/*******************************************************************/		 
	 do
	 {
    Preset1=Measure_IDDIO(mA);  //	   mA��		 
    USART_SEND_DEC(Preset1);
//--------------------------------------//     
     mm_KEY_UP=KEY_UP;	
		 if( mm_KEY_UP==0)
		 {
			 vddio_vol+=5; 
	     VDDIO_ADJ_SET(vddio_vol);			 
	     VDDIO_ADJ_EN(1);			 
		 }
		 
     mm_KEY_DOWN=KEY_DOWN;
		 if( mm_KEY_DOWN==0)
		 {
			 vddio_vol-=5; 
	     VDDIO_ADJ_SET(vddio_vol);			 
	     VDDIO_ADJ_EN(1);			  
		 } 
//--------------------------------------//								 
		mm_KEY_PAUSE=KEY_PAUSE;  
	 }while((mm_KEY_PAUSE!=0));	 

       	
    while(mm_KEY_PAUSE==0)
	 {
		 mm_KEY_PAUSE=KEY_PAUSE;
	 } 	
	 
//    for(coco=0;coco<8;coco++)
//    {
//			 little_value=Measure_IDDIO(mA);
//			 if(little_value>Preset1)
//			 {Preset1=little_value;}	 	   
//		}	
    decom_a=(u16)(5000000/Preset1);            // IO IDD 50MA
  
//////////////////////==========key����pass==========			
	EEPROMBYTE_Write(0x16,decom_a>>8);  DelayMs(5); //uA��     
	EEPROMBYTE_Write(0x17,decom_a);	    DelayMs(5);
  if(Tempc!=0x19)
  {	
	EEPROMBYTE_Write(0x56,decom_a>>8);  DelayMs(5); //���ݱ���
	EEPROMBYTE_Write(0x57,decom_a);	    DelayMs(5); 		 
  }	
//////////////////////============24c02�洢��======		
  
	EEPROMBYTE_Write(0x18,0xcc);	      DelayMs(5); 	//У׼��ɱ�־   0xcc
  if(Tempc!=0x19)
  {	
	EEPROMBYTE_Write(0x19,0x19);        DelayMs(5);   //�������÷��   0x19
  }	 
	
	HFZ_COF=0;   //
	PA910SPISendValueofCurrent("-%-decom_a-*-:",decom_a);
	PA910SPISendValueofCurrent("-~-complete!!:",0);
	
	USART_REMAP_GPIO_Configuration(); 
	VDDIO_ADJ_EN(0);	//�ر�VDDIO��ѹ	
	
	BEEP_XiXi();   // ཱ�ལ���
}
//------------------------------------------------------------------
/*******************************************************
*
*              ������������������д��
*
*******************************************************/
void RESTORE_CALIBRATION(void)   
{ 

	u8 data_H,data_L;
	
  data_H=EEPROMBYTE_Read(0x50);      DelayMs(1);  
	data_L=EEPROMBYTE_Read(0x51);      DelayMs(1); 
  EEPROMBYTE_Write(0x10,data_H);     DelayMs(5);  //uA��     
	EEPROMBYTE_Write(0x11,data_L);	   DelayMs(5); 

  data_H=EEPROMBYTE_Read(0x52);      DelayMs(1);  
	data_L=EEPROMBYTE_Read(0x53);      DelayMs(1); 	
  EEPROMBYTE_Write(0x12,data_H);     DelayMs(5);  //IDD    
	EEPROMBYTE_Write(0x13,data_L);	 	 DelayMs(5); 	

  data_H=EEPROMBYTE_Read(0x54);      DelayMs(1); 
	data_L=EEPROMBYTE_Read(0x55);      DelayMs(1); 		
  EEPROMBYTE_Write(0x14,data_H);     DelayMs(5);  //uA��     
	EEPROMBYTE_Write(0x15,data_L);	   DelayMs(5); 

  data_H=EEPROMBYTE_Read(0x56);      DelayMs(1);  
	data_L=EEPROMBYTE_Read(0x57);      DelayMs(1); 		
  EEPROMBYTE_Write(0x16,data_H);     DelayMs(5);  //IDDIO    
	EEPROMBYTE_Write(0x17,data_L);	 	 DelayMs(5); 		
//////////////////////============24c02�洢��======		
  
//	EEPROMBYTE_Write(0x18,0xcc);	DelayMs(5); 	//У׼��ɱ�־   0xcc
	BEEP_XiXi();   // ཱ�ལ���
}



/*******************************************************
*
*              ����У׼ϵ��д�뱸����
*
*******************************************************/
void STORE_CALIBRATION(void)   
{ 

	u8 data_H,data_L;
	
  data_H=EEPROMBYTE_Read(0x10);      DelayMs(1);  
	data_L=EEPROMBYTE_Read(0x11);      DelayMs(1); 
  EEPROMBYTE_Write(0x50,data_H);     DelayMs(5);  //uA��     
	EEPROMBYTE_Write(0x51,data_L);	   DelayMs(5); 

  data_H=EEPROMBYTE_Read(0x12);      DelayMs(1);  
	data_L=EEPROMBYTE_Read(0x13);      DelayMs(1); 	
  EEPROMBYTE_Write(0x52,data_H);     DelayMs(5);  //IDD    
	EEPROMBYTE_Write(0x53,data_L);	 	 DelayMs(5); 	

  data_H=EEPROMBYTE_Read(0x14);      DelayMs(1); 
	data_L=EEPROMBYTE_Read(0x15);      DelayMs(1); 		
  EEPROMBYTE_Write(0x54,data_H);     DelayMs(5);  //uA��     
	EEPROMBYTE_Write(0x55,data_L);	   DelayMs(5); 

  data_H=EEPROMBYTE_Read(0x16);      DelayMs(1);  
	data_L=EEPROMBYTE_Read(0x17);      DelayMs(1); 		
  EEPROMBYTE_Write(0x56,data_H);     DelayMs(5);  //IDDIO    
	EEPROMBYTE_Write(0x57,data_L);	 	 DelayMs(5); 		
//////////////////////============24c02�洢��======		
  
	EEPROMBYTE_Write(0x19,0x19);	     DelayMs(5); 	//У׼��ɱ�־   0xcc
	
	BEEP_HuHu();
	BEEP_XiXi();   // ཱ�ལ���
	
}


void GET_PRESENT(void)    //��ȡУ׼ϵ��
{
	 u8 data_H,data_L;
	
   data_H=EEPROMBYTE_Read(0x10);  DelayMs(1);    //
	 data_L=EEPROMBYTE_Read(0x11);  DelayMs(1);
	 IDD_UA_preset=(data_H<<8)+data_L;     DelayMs(1);
   
   data_H=EEPROMBYTE_Read(0x12);  DelayMs(1);    //
	 data_L=EEPROMBYTE_Read(0x13);  DelayMs(1);
	 IDD_UA_decom=(data_H<<8)+data_L;     DelayMs(1);
	
	 data_H=EEPROMBYTE_Read(0x14);  DelayMs(1);
	 data_L=EEPROMBYTE_Read(0x15);  DelayMs(1);
	 IDD_MA=(data_H<<8)+data_L;	    DelayMs(1);
	
//	 data_H=EEPROMBYTE_Read(0x14);  DelayMs(1);   //IDDIO_uA�n��У
//	 data_L=EEPROMBYTE_Read(0x15);  DelayMs(1);
//	 IDDIO_UA=(data_H<<8)+data_L;	  DelayMs(1);
	
	 data_H=EEPROMBYTE_Read(0x16);  DelayMs(1);
	 data_L=EEPROMBYTE_Read(0x17);  DelayMs(1);
   IDDIO_MA=(data_H<<8)+data_L;	  DelayMs(1);
		
}

void EEpromRead_CurrentCalibration(void)
{
	u8 CAL_OK=0;
	u16 mm_KEY_UP = 1 ;
	u16 mm_KEY_DOWN = 1 ;
	u16 mm_KEY_PAUSE = 1;


	mm_KEY_UP=KEY_UP;   //ǰ�M
	mm_KEY_DOWN=KEY_DOWN;   //����
	mm_KEY_PAUSE=KEY_PAUSE;   
	BEEP_OFF(); 	 /////////beep  off
 
	if((mm_KEY_UP==0)&&(mm_KEY_DOWN==0)&&(mm_KEY_PAUSE==0))  //�жϰ����Ƿ�ͬʱ����
	{
	   while((mm_KEY_UP==0)||(mm_KEY_DOWN==0)) //�����Ƿ��ͷ�
		 {
			mm_KEY_UP=KEY_UP;   //ǰ�M
			mm_KEY_DOWN=KEY_DOWN;   //���� 
		 }
/**********************************************************************************/		 
	 DelayMs(1500);	
	 mm_KEY_UP=KEY_UP;   //ǰ�M
	 mm_KEY_DOWN=KEY_DOWN;   //����
	 mm_KEY_PAUSE=KEY_PAUSE;  
		 
	if((mm_KEY_UP!=0)&&(mm_KEY_DOWN!=0)&&(mm_KEY_PAUSE==0))  //�жϰ����Ƿ�ͬʱ����
	   {
			 while((mm_KEY_UP==0)||(mm_KEY_DOWN==0)||(mm_KEY_PAUSE==0)) //�����Ƿ��ͷ�
			 {
				mm_KEY_UP=KEY_UP;   //ǰ�M
				mm_KEY_DOWN=KEY_DOWN;   //����
				mm_KEY_PAUSE=KEY_PAUSE;    //�M����Cģʽ		 
			 }		 
		   STORE_CALIBRATION();  //��ȷ��OK��ϵ�����뱸������
	   }     		 
/**********************************************************************************/	    
     //------ �趨һ����ѹ��У׼ʹ��	--------------------	 
		 T226_intion();  //ע�⣺��ѹ����ܵ�STM32��226ͬʱ����,��������INA226
	   LCD_Vol_SET();		///////LCD 늉��O��,,,,//ע�⣺��ѹ����ܵ�STM32��226ͬʱ����
		 
		 AUTO_CALIBRATION();  //У׼
	}
//==================================================================================	   
	if((mm_KEY_UP==0)&&(mm_KEY_DOWN==0))  //ͬʱ��������ɫ����
	{
	   while((mm_KEY_UP==0)||(mm_KEY_DOWN==0)) //�����Ƿ��ͷ�
		 {
			mm_KEY_UP=KEY_UP;   //ǰ�M
			mm_KEY_DOWN=KEY_DOWN;   //����
		 }	
	   RESTORE_CALIBRATION(); 
	 }	 
//------------------------------------------------------------------	
   CAL_OK=EEPROMBYTE_Read(0x18);  //���У׼������ʹ��У׼���������δУ׼����ʹ��Ĭ�ϲ���
  
   if(CAL_OK==0xcc) 
   { GET_PRESENT(); } //��ȡУ׼����
	else
	 {				
				BEEP_ON();
		 
//	      VDD_ADJ_EN(0);				 //�ر�VDD���		
//	      GPIO_ResetBits(GPIOA, GPIO_Pin_1);  //�ر�VDD���	������ʹ�� 
//	      VDDIO_ADJ_EN(0);			  //�ر�VDDIO���
//  	    VOTP_EN(0);					  //�ر�VOTP���	
		   
				while(1)
        {BACK_LIGHT_OFF();DelayMs(500);BACK_LIGHT_ON();DelayMs(500);}
	 }
}


//---------------------------------------------------------  ������������
void SLEEP_CAL_TEST(void)
{
	IDDCurrentOfSleep=Measure_IDD(uA);
	if(IDDCurrentOfSleep> IDD_SLEEP_LIMIT_VALUE)
	{
		exit_sleep_mode();                            DelayMs(150);  
		Allcolor(1,0xffffff);		                      DelayMs(150);//ˢ��
		ShowTxt("38,20,99,IDD������������");	        DelayMs(150);
		ShowData("38,200,149",IDDCurrentOfSleep);     DelayMs(150);	
		while(1)
		{//	BACK_LIGHT_OFF();DelayMs(300);BACK_LIGHT_ON();DelayMs(300);
		 BEEP_Dudu();}
	}

	IDDIOCurrentOfSleep=Measure_IDDIO(uA);
	FlagOfCurrentSleep=1;
	if(IDDIOCurrentOfSleep > IOIDD_SLEEP_LIMIT_VALUE)
	{
		
		exit_sleep_mode();                            DelayMs(150);  
		Allcolor(1,0xffffff);		                      DelayMs(150);//ˢ��
		ShowTxt("38,20,99,IDDIO������������");	      DelayMs(150);
    ShowData("38,250,149",IDDIOCurrentOfSleep);   DelayMs(150);			
		while(1)
		{//BACK_LIGHT_OFF();DelayMs(300);BACK_LIGHT_ON();DelayMs(300);
		 BEEP_Dudu();}
	}
}

//---------------------------------------------------------  ������������
void NORML_CAL_TEST(void)
{   
	  u16 chenf;
	  chenf=Measure_IDD(mA);
		if(chenf > IDD_WORK_LIMIT_VALUE)
		{
			Allcolor(1,0xffffff);		                                        DelayMs(150);//ˢ��
		  ShowTxt("38,20,149,IDD:(mA)");	  				                      DelayMs(150);
		  ShowData_ua2ma_4bitfloat("38,200,149",chenf);                   DelayMs(150);			
			ShowTxt("38,20,99,IDD��������ֵ");                              DelayMs(150);
      while(1)			
			{BEEP_Dudu();}
		}	
	  
		chenf=Measure_IDDIO(mA);
		if(chenf > IOIDD_WORK_LIMIT_VALUE)
		{
			Allcolor(1,0xffffff);		     DelayMs(150);//ˢ��
		  ShowTxt("38,20,149,IDDIO:(mA)");	  				                    DelayMs(150);
		  ShowData_ua2ma_4bitfloat("38,250,149",chenf);                   DelayMs(150);
			ShowTxt("38,20,99,IDDIO��������ֵ");                            DelayMs(150);
      while(1)			
			{BEEP_Dudu();}
		}
}

void PA910SPISendValueofCurrent(char *p,u32 datcf)
/*********************************************************************************************************
*USART_GPIO_Configuration(); ִ��֮��ſ������б�������PA9 PA10�Դ��ڷ�ʽ�³����ݣ����ڲ����ʣ�115200
*����������֮����Ҫִ��USART_REMAP_GPIO_Configuration(); �Իָ�ԭ������
*void PA910SPISendValueofCurrent(char *p,u32 datcf)  ʹ�÷���:PA910SPISendValueofCurrent("---C.mA---:",0x03e8);
*����*p Ϊ�ַ���ָ��,datcfΪ16λ������ִ�к󴮿��յ�����Ϊ��---C.mA---:0x03e8 <=> 1000
**ע�⣬16������ʾʱ��32bit���ݵĸ�16λ���Զ�������10���Ʋ��ֲ���Ӱ��
*********************************************************************************************************/	
{
  //-------------------
	  u8 i=0,j=0,k=0,model=1;	  
		u8 tep_H,tep_L,tep_h,tep_l ;
		u8 data_H,data_L;
	  u32 data,HFX_Dec,f;	
	//--------------------
	char tx_tep;   
	
	tx_tep = *p; 
  data=datcf;	

	USART_SendData(USARTx, 10); 

  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}		
	
	while(tx_tep!='\0')                                       
    {USART_SendData(USARTx, tx_tep);          
     while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}  
     p++;
		 tx_tep = *p ;
		}
	//---------------------------------?????buxudong 0-----------------------------------------------
	
		Luanxie:
		
		if(model==1)
					{		
						data_H=data>>8;
						data_L=data;
						
						tep_H=data_H>>4&0x0f;
						tep_L=data_H&0x0f;
						
						tep_h=data_L>>4&0x0f;
						tep_l=data_L&0x0f;
						
						USART_SendData(USART1, 48);	 	  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}		 
						USART_SendData(USART1, 120);		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	
						if(tep_H<0x0a)		// ???
							{USART_SendData(USART1, tep_H+0x30); 
							 while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	
							}
						else						//????
							{USART_SendData(USART1, tep_H+0x37); 
							 while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	
							}
						if(tep_L<0x0a)
							{USART_SendData(USART1, tep_L+0x30); 	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	
							}
						else
							{USART_SendData(USART1, tep_L+0x37); 
							 while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	
							}
						if(tep_h<0x0a)		// ?a�y?
							{USART_SendData(USART1, tep_h+0x30); 	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	
							}
						else						//?a???
							{USART_SendData(USART1, tep_h+0x37); 
							 while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	
							}
						if(tep_l<0x0a)
							{USART_SendData(USART1, tep_l+0x30); 	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	
							}
						else
							{USART_SendData(USART1, tep_l+0x37); 
							while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	
							}
				 }
	  else
				  {
						 HFX_Dec=data;
						 HFX_Dec/=10;
						 for ( i=1; HFX_Dec>0; i++, HFX_Dec/=10 ) ;
						 k=i; HFX_Dec=data;
						 for (j=0;j<i;j++)
								 {  f= pow(10,(k-1));
										tep_H=HFX_Dec/f;
										k--;	
										HFX_Dec-=f*tep_H;					 
										{
										 USART_SendData(USARTx, (tep_H+48));            //
										 while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	
										}
								 }
		 
	        }

     if (model) 
		     {model=0;
					 USART_SendData(USARTx,60);           
					while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	
				  USART_SendData(USARTx,61);           
					while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	
						USART_SendData(USARTx,62);           
					while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	
				  goto Luanxie;
				 }
					
					
					//-----------------?????------buxudong 0-------------------------------------------------		
  USART_SendData(USARTx, 10);            //
  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){} 
  USART_SendData(USARTx, 10);            //
  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}		
		 
}

void USART_SEND_DEC(u16 data)
		{
			 u16 HFX_Dec;
			 u8 i,k,j,tep_H,f;
			 HFX_Dec=data;
			 HFX_Dec/=10;
			 for ( i=1; HFX_Dec>0; i++, HFX_Dec/=10 ) ;
			 k=i; HFX_Dec=data;
			 for (j=0;j<i;j++)
					 {  f= pow(10,(k-1));
							tep_H=HFX_Dec/f;
							k--;	
							HFX_Dec-=f*tep_H;					 
							{
							 USART_SendData(USARTx, (tep_H+48));            //
							 while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}	
							}
					 }
       USART_SendData(USARTx, 32);            //
       while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){} 
       USART_SendData(USARTx, 32);            //
       while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}		
		}

//=================================================================================================

void T226_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//��ֹjtag���Կճ�PB3,PB4,PA15

     //================================================
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = T226PIN_SCL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin =  T226PIN_SDA;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
      //----------------------------------------
}


void T226_I2CSDASetInput(void)
{
	GPIO_InitTypeDef QGPIO_InitStructure;
	 QGPIO_InitStructure.GPIO_Pin = T226PIN_SDA;
	 QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; //GPIO_Mode_IN_FLOATING;
	 GPIO_Init(GPIO_TP, &QGPIO_InitStructure);
} 
void T226_I2CSDASetOutput(void)
{ 
	GPIO_InitTypeDef QGPIO_InitStructure;
	 QGPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	 QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	 QGPIO_InitStructure.GPIO_Pin =  T226PIN_SDA;
	 GPIO_Init(GPIO_TP, &QGPIO_InitStructure);
}

// Start bit of I2C waveform
void T226_I2CStart(void)
{
    T226_I2CSDASetOutput();
    TPSDA_H; 
    TPSCL_H;
	  TWait(); 
    TPSDA_L; 
	  TWait();
    TPSCL_L; 
}

// Stop bit of I2C waveform
void T226_I2CStop(void)
{
    T226_I2CSDASetOutput();
    TPSCL_H;
    TPSDA_L;
    TPSCL_H;
    TWait();
    TPSDA_H;
    TWait();
}

// Send one byte from host to client
bool T226_I2CSendByte(BYTE ucData)
{
	bool bRet;
	unsigned char i=8; 

	T226_I2CSDASetOutput();
	do{	
		if (ucData&0x80)
			 TPSDA_H;
			 else 
			 	TPSDA_L;
		 TWait();//-----------------------????
		TPSCL_H;
		TWait();
		ucData<<=1;
		TPSCL_L;
                if (i>1) //�ж��Ƿ񴫵����һλ���ݣ��������һλ��TSCL���ͺ��õȴ�
		TWait();
    	}while(--i);
  
	T226_I2CSDASetInput();
	
	bRet=FALSE;
	TPSCL_H;
	TWait();
	if (GPIO_ReadInputDataBit(GPIO_TP, T226PIN_SDA))
	bRet=TRUE;
	TPSCL_L;
	TWait();	 //Լ4��us
	return bRet;
}
  

// Receive one byte form client to host
BYTE T226_I2CReciveByte(bool bAck)
{
	BYTE ucRet,i;
	
	T226_I2CSDASetInput();
	
	ucRet = 0;
	i=8;
	do
	{	TPSCL_H;
		TWait();
		ucRet<<=1;
		if(GPIO_ReadInputDataBit(GPIO_TP, T226PIN_SDA))
			ucRet++;
		TPSCL_L;
                if (i>1) //�ж��Ƿ񴫵����һλ���ݣ��������һλ��TSCL���ͺ��õȴ�
		TWait();
	 }while(--i);

	T226_I2CSDASetOutput();
	
	if (bAck==0)
		TPSDA_L;
	else TPSDA_H;
	
	TPSCL_H;
	TWait();
	TPSCL_L;
	TWait();         
	return ucRet;
}

bool T226_I2CTest(BYTE ucDeviceAddr)
{
///////////////////have something  wrong    .....
//      bool bRet = TRUE;
//      I2CStart();
//      if(I2CSendByte(ucDeviceAddr & 0xFE) == ACK)
//        {bRet = TRUE;          
//        }
//      else
//        {
//          bRet = FALSE;
//        }
//      I2CStop();
//      return bRet;
	
}

// I2C send data fuction
bool T226_I2CSend(BYTE ucDeviceAddr, BYTE ucIndex, BYTE* pucData, unsigned int unDataLength)
{
      unsigned int i;
      bool bRet = TRUE;
      T226_I2CStart();
      if(T226_I2CSendByte(ucDeviceAddr & 0xFE) == ACK)
        {
          if(T226_I2CSendByte(ucIndex) == ACK)
            {
              for(i = 0; i < unDataLength; i++)
              {
                  if(T226_I2CSendByte(pucData[i]) == NAK)
                  {
                    bRet = FALSE;
                    break;
                  }
              }
            }
          else
            {
             bRet = FALSE;
            }
        }
      else
      {
      bRet = FALSE;
      }
      T226_I2CStop();
      return bRet;
}
      // I2C receive data function
bool T226_I2CReceive(BYTE ucDeviceAddr, BYTE ucIndex, BYTE* pucData, unsigned int unDataLength) //(������ַ��buffer���ͣ����������ݱ���λ�ã��������ݳ���)
{
      unsigned int i;
      bool bRet = TRUE;
      T226_I2CStart();
      if(T226_I2CSendByte(ucDeviceAddr & 0xFE) == ACK)
        {
           if(T226_I2CSendByte(ucIndex) == ACK)
           {

//			   T226_I2CStop();		 ////Optional
			   T226_I2CStart();
               if(T226_I2CSendByte(ucDeviceAddr | 0x01) == ACK)
               {
                 for(i = 0; i < unDataLength - 1; i++)
                   {  pucData[i] = T226_I2CReciveByte(ACK);
                   }
                 pucData[unDataLength - 1] = T226_I2CReciveByte(NAK);
               }
               else
               {          bRet = FALSE;           }
           }
           else
           {           bRet = FALSE;           }
         }
      else
        {           bRet = FALSE;           }
      T226_I2CStop();
      return bRet;
}
