/* Includes ------------------------------------------------------------------*/
#include <T226.h>
#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "lib.h"
#include "platform_config.h"
#include "lcd.h"
#include "IIC.h"


////////////9.1. Sample Code: Definitions
////////////These are the definitions for sample code.

extern BYTE pucData[2];
extern u16 Vshunt_limit; 

extern void VDD_ADJ_SET(u32 VDD_settig);		 //VDD_ADJ_SET 参数取值范围270~600，对应电压范围2.7V~6V; 
extern void VDDIO_ADJ_SET(u32 vset);			 //VDDIO_ADJ_SET 参数取值范围160~360，对应电压范围1.6V~3.6V; 
extern void VOTP_ADJ_SET(u32 vset);			 //VOTP_ADJ_SET 参数取值范围150~2080，对应电压范围1.5V~14.1V; 
extern void VDD_ADJ_EN(u8 vset);

signed short int IDD_UA=0;   // x-2134
u16 IDD_MA=100;       //此系数勿改!!
u16 IDDIO_UA=0;  //  x-5
u16 IDDIO_MA=100;  
u16 tesss;


u16 decom_a=0;
short int decom_b=0; 
u16 Preset1=0;
u16 Preset2=0;

u8 HFZ_COF=0;

u16 vddio_vol=300;
u16 vdd_vol=300;

//// Psudo functions
void I2CSDASetInput(void); // Set I2C SDA signal as input
void I2CSDASetOutput(void); // Set I2C SDA signal as output

extern void USART_GPIO_Configuration(void);
extern void USART_REMAP_GPIO_Configuration(void);

void T226_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//禁止jtag，以空出PB3,PB4,PA15

     //================================================
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = T226PIN_SCL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin =  T226PIN_SDA;
	GPIO_Init(GPIO_TP, &GPIO_InitStructure);
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
                if (i>1) //判断是否传递最后一位数据，若是最后一位则TSCL拉低后不用等待
		TWait();
    	}while(--i);
  
	T226_I2CSDASetInput();
	
	bRet=FALSE;
	TPSCL_H;
	TWait();
	if (GPIO_ReadInputDataBit(GPIO_TP, T226PIN_SDA))
	bRet=TRUE;
	TPSCL_L;
	TWait();
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
                if (i>1) //判断是否传递最后一位数据，若是最后一位则TSCL拉低后不用等待
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
bool T226_I2CReceive(BYTE ucDeviceAddr, BYTE ucIndex, BYTE* pucData, unsigned int unDataLength) //(器件地址，buffer类型，被读出数据保存位置，读出数据长度)
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

   //////////////////M9    SETTING
	pucData[0] = 0x41;
  pucData[1] = 0x27;
	T226_I2CSend(DEVICE_ADDRESS_M9, COMMAND_CONFIG_INDEX, pucData, 2);
	
	pucData[0] = 0x00;		   ////0x7FFF				  0x8001
  pucData[1] = 0x02;
	T226_I2CSend(DEVICE_ADDRESS_M9, COMMAND_MASK_INDEX, pucData, 2);

	pucData[0] = 0x01;
  pucData[1] = 0xf4;
	T226_I2CSend(DEVICE_ADDRESS_M9, COMMAND_CALIBRATION_INDEX, pucData, 2);

	pucData[0] = Vshunt_limit>>8; 
  pucData[1] = Vshunt_limit; 
	T226_I2CSend(DEVICE_ADDRESS_M9, COMMAND_ALERT_LIMIT_INDEX, pucData, 2);
	
	
	T226_I2CReceive(DEVICE_ADDRESS_M9, COMMAND_ID_INDEX, pucData, 2);	//DelayMs(10);
	if(pucData[0] == 0x22)
	{
		;
	}

	T226_I2CReceive(DEVICE_ADDRESS_M12, COMMAND_ID_INDEX, pucData, 2);	//DelayMs(10);
	if(pucData[0] == 0x22)
	{
		;
	}

	//////////////////M12   SETTING
	pucData[0] = 0x41;
  pucData[1] = 0x27;
	T226_I2CSend(DEVICE_ADDRESS_M12, COMMAND_CONFIG_INDEX, pucData, 2);

	pucData[0] = 0x00;		   ////不加入夾斷機制
  pucData[1] = 0x03;
	T226_I2CSend(DEVICE_ADDRESS_M12, COMMAND_MASK_INDEX, pucData, 2);

	pucData[0] = 0x01;
  pucData[1] = 0xF4;
	T226_I2CSend(DEVICE_ADDRESS_M12, COMMAND_CALIBRATION_INDEX, pucData, 2);

}

u16 Measure_IDD_for_start(u8 gears)
{
	  u16 data[10]={0};
		u8 times=10;    //采样次数10此
		
		u16 temp;
		u8 i,j;
		u32 sum=0;

		 data[1]=Measure_IDD_once(gears);

		for(i=0;i<times;i++)
		{
    		data[i]=Measure_IDD_once(gears);
		}
		for(i=0;i<times-1;i++)
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
		for(i=2;i<times-2;i++)
		{
		  sum+=data[i];
		}
		sum/=(times-4);
		temp=sum;
		return temp;
	
}

u16 Measure_IDD(u8 gears)
{
	  u16 data[100]={0};
		u8 times=100;    //采样次数10此
		
		u16 temp;
		u8 i,j;
		u32 sum=0;

		 data[1]=Measure_IDD_once(gears);

		for(i=0;i<times;i++)
		{
    		data[i]=Measure_IDD_once(gears);
			data[i+1] = 0;
		}
		for(i=0;i<times-1;i++)
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
		for(i=2;i<times-2;i++)
		{
		  sum+=data[i];
		}
		sum/=(times-4);
		temp=sum;
		return temp;
	
}


u16 Measure_IDDIO(u8 gears)
{
	  u16 data[100]={0};
		u8 times=100;   //采样次数10次
		
		u16 temp;
		u8 i,j;
		u32 sum=0;
	
		for(i=0;i<times;i++)
		{
    		data[i]=Measure_IDDIO_once(gears);
			data[i+1] = 0;
		}
		for(i=0;i<times-1;i++)
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
		for(i=2;i<times-2;i++)
		{
		  sum+=data[i];
		}
		sum/=(times-4);
		temp=sum;
		return temp;
}


u16 Measure_IDD_once(u8 gears)   //gears=1 -->mA檔   gears=0 -->uA檔
{
  // signed short int IDD_value=0;
    u16 IDD_value=0;
   u16 VSHUNT=0; 
  // u16 tcf;

	
	 if(gears==uA)
	 {
			GPIO_SetBits(GPIOA, GPIO_Pin_0);   
			T226_I2CReceive(DEVICE_ADDRESS_M9, COMMAND_SHUNT_VLOTAGE_SETTING, pucData, 2);	DelayMs(5); 
			VSHUNT=(pucData[0]<<8)+pucData[1];   //最小0x8000	
		 
			IDD_value=~VSHUNT;

		   // IDD_
			IDD_value=IDD_value - IDD_UA;	//71



		 // if(IDD_value>0xf000)
		//	IDD_value=0;	
			
			DelayMs(1);			 
	 }
	//---------------------------------------------------------------------------------------------	 
	 else
	 {
	  GPIO_ResetBits(GPIOA, GPIO_Pin_0);  //關閉光耦，ma档   打開，短接電阻 測ma檔
		T226_I2CReceive(DEVICE_ADDRESS_M9, COMMAND_SHUNT_VLOTAGE_SETTING, pucData, 2);	DelayMs(5); 
		VSHUNT=(pucData[0]<<8)+pucData[1];   //最小0x8000
		  
			IDD_value=~VSHUNT;
//=========================如若未校准，为保护开机大电流，大概配一个系数==================		 
			IDD_value=(u16)((u32)(IDD_value*IDD_MA)/100); 
			tesss=IDD_value;		
		  DelayMs(1);	
	//=========================mA档大电流保护	  ================================ 
//			  if(IDD_value>11000)
//				{ 				
//					while(1)
//					{ 
////--------------------------------------------------------------------------------------------------------						
//						T226_I2CReceive(DEVICE_ADDRESS_M12, COMMAND_MASK_INDEX, pucData, 2);  DelayMs(20);
//						pucData[0] = 0x40;		   ////0x7FFF				  0x8001
//						pucData[1] = 0x01;
//						T226_I2CSend(DEVICE_ADDRESS_M12, COMMAND_MASK_INDEX, pucData, 2);	DelayMs(20);

////--------------------------------------------------------------------------------------------------------						
//						T226_I2CReceive(DEVICE_ADDRESS_M9, COMMAND_MASK_INDEX, pucData, 2);  DelayMs(20);
//						pucData[0] = 0x40;		   ////0x7FFF				  0x8001
//						pucData[1] = 0x01;
//						T226_I2CSend(DEVICE_ADDRESS_M9, COMMAND_MASK_INDEX, pucData, 2);	DelayMs(20);
//						
//						T226_I2CReceive(DEVICE_ADDRESS_M9, COMMAND_SHUNT_VLOTAGE_SETTING, pucData, 2);	DelayMs(20); 
//            VSHUNT=(pucData[0]<<8)+pucData[1];   //最小0x8000
//						if(VSHUNT==0x0000)
//						{while(1);}
//          }  				
//				}
	//----------------------------------------------------------------------------------------------	 
   }				
    return IDD_value;
}

u16 Measure_IDDIO_once(u8 gears)
{
    u16 IDDIO_value=0;
    u16 VSHUNT=0;
  
		if(gears==uA)
		 {
			GPIO_SetBits(GPIOA, GPIO_Pin_3);  //打开光耦，ua档  			 
			T226_I2CReceive(DEVICE_ADDRESS_M12, COMMAND_SHUNT_VLOTAGE_SETTING, pucData, 2);	DelayMs(5); 

			IDDIO_value=(pucData[0]<<8)+pucData[1];
			IDDIO_value=IDDIO_value-IDDIO_UA;	
		  if(IDDIO_value>0xf000)
			IDDIO_value=0;
      tesss=IDDIO_value;				
		 }
//------------------------------------------------------------------------------------------------------		 
		 else
		 {
			GPIO_ResetBits(GPIOA, GPIO_Pin_3);  //關閉光耦，ma档 
			T226_I2CReceive(DEVICE_ADDRESS_M12, COMMAND_SHUNT_VLOTAGE_SETTING, pucData, 2);	DelayMs(5); 
			 
			IDDIO_value=(pucData[0]<<8)+pucData[1];
			IDDIO_value=(u16)((u32)(IDDIO_value*IDDIO_MA)/100);
			tesss=IDDIO_value;	
		 }
	
		return IDDIO_value;
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
/************ 配置校准电压 两路均为3V**********************/
	VDD_ADJ_SET(vdd_vol);					
	VDD_ADJ_EN(1);
 	DelayMs(150);
//	VDDIO_ADJ_SET(vddio_vol);			 
//	VDDIO_ADJ_EN(1);			
//	DelayMs(150);
/*******************************************/	
	
	USART_GPIO_Configuration();  //切换为普通串口1模式
	PA910SPISendValueofCurrent(" ,,",111);
	PA910SPISendValueofCurrent("---IDD_Calibration_Starting--:",54321);   //????????   	
	BEEP_BIBI();	  //喇叭叫兩聲，提示進入校準
	PA910SPISendValueofCurrent("--uA-MODE1-:",0);   //????????  
/*******************************************************************/	
/*配置,uA档,IDD路*/
//    GPIO_ResetBits(GPIOB, GPIO_Pin_9); 	//
//		RELAY_EN(0); 
//		DelayMs(150);
//    VOTP_ADJ_SET(390);			  
//    VOTP_EN(0); //
//    DelayMs(150);		
		
//		A0_EN(0);
//		A1_EN(0);
/*******************************************************************/			
	 do
	 {
     Preset1=Measure_IDD(uA);  //  IDD 30uA 环境	   y=ax+b
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
//		A0_EN(1);
//		A1_EN(0);	
		
	  do
	  {
     Preset2=Measure_IDD(uA);  //  IDD 50uA 环境	
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
	 
//////////////////////============24c02存储区======		

	EEPROMBYTE_Write(0x10,decom_a>>8);   DelayMs(5); //uA档     
	EEPROMBYTE_Write(0x11,decom_a);	     DelayMs(5); 
		
  EEPROMBYTE_Write(0x12,decom_b>>8);   DelayMs(5); //uA档     
	EEPROMBYTE_Write(0x13,decom_b);	     DelayMs(5); 	
		
  if(Tempc!=0x19)
  {	
	EEPROMBYTE_Write(0x50,decom_a>>8);   DelayMs(5); //数据备份    
	EEPROMBYTE_Write(0x51,decom_a);	     DelayMs(5); 	 
	
  EEPROMBYTE_Write(0x52,decom_b>>8);   DelayMs(5); //数据备份    
	EEPROMBYTE_Write(0x53,decom_b);	     DelayMs(5); 	
  }	
 
	PA910SPISendValueofCurrent("-^-IDD_uA_pre-^-:",decom_a); 
  PA910SPISendValueofCurrent("-^-IDD_uA_dec-^-:",decom_b);
	
	BEEP_BIBI();
	 
/*******************************************************************/	
/*配置,mA档,IDD路*/	
//    GPIO_SetBits(GPIOB, GPIO_Pin_9); //将300K短起  ,mA档
//	  RELAY_EN(1); 
//		DelayMs(150);
//    VOTP_ADJ_SET(390);			  
//    VOTP_EN(0); //IDD路
//    DelayMs(150);
    PA910SPISendValueofCurrent("--mA-MODE1-:",0);   //????????  
//		A0_EN(0);
//		A1_EN(1);	
/*******************************************************************/	
	 
	 do
	 {
     Preset1=Measure_IDD(mA);  //	50mA的环境
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
		//将数据发送至串口 
	 }while((mm_KEY_PAUSE!=0));   

	 
///////////////////////==========key——pass==========			 
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
		
///////////////////////============24c02存储区======		
	 EEPROMBYTE_Write(0x14,decom_a>>8);  DelayMs(5);      
	 EEPROMBYTE_Write(0x15,decom_a);	 	DelayMs(5); 
		
   if(Tempc!=0x19)
   {			
	 EEPROMBYTE_Write(0x54,decom_a>>8);   DelayMs(5); //数据备份
	 EEPROMBYTE_Write(0x55,decom_a);	DelayMs(5); 			
   }				
	 VDD_ADJ_EN(0);
	 
   PA910SPISendValueofCurrent("-%-Preset1-*-:",Preset1);    //4008
	 
	 BEEP_BIBI();
		
	
//		 IDDIO一路校准开始！！
/*******************************************************************/
//    GPIO_ResetBits(GPIOB, GPIO_Pin_9); 	//uA档 
//      RELAY_EN(0);	 
//		DelayMs(150);
//    VOTP_ADJ_SET(390);			  
//    GPIO_SetBits(GPIOA, GPIO_Pin_4);   //IDDIO路
//    DelayMs(150);
/*******************************************************************/	
//		
//	 do
//	 {
//    Preset1=Measure_IDDIO(uA);  // IDDIO  uA档	 
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
/////////////////////////==========key——pass==========			 
//    while(mm_KEY_PAUSE==0)
//	 {mm_KEY_PAUSE=KEY_PAUSE;} 
//	 
//    decom_a=50000/Preset1;   
//	 	
////////////////////////============24c02存储区======			
//	 EEPROMBYTE_Write(0x14,decom_a>>8); DelayMs(5);  //uA档     
//	 EEPROMBYTE_Write(0x15,decom_a);	 	DelayMs(5);	

//	 EEPROMBYTE_Write(0x54,decom_a>>8); DelayMs(5); //数据备份
//	 EEPROMBYTE_Write(0x55,decom_a);	DelayMs(5); 	
//	 
//	 BEEP_BIBI();  
	 
///*******************************************************************/	
///*配置,uA档,IDDIO路*/	
////		GPIO_SetBits(GPIOB, GPIO_Pin_9); 	//mA档
//		RELAY_EN(1);
//		DelayMs(150);
//    VOTP_ADJ_SET(390);			  
//    VOTP_EN(1);   //IDDIO路
//    DelayMs(150);	
//		PA910SPISendValueofCurrent("--mA-MODE2-:",0);   //????????  
//		A0_EN(1);
//		A1_EN(1);	
///*******************************************************************/		 
//	 do
//	 {
//    Preset1=Measure_IDDIO(mA);  //	   mA档		 
//    USART_SEND_DEC(Preset1);
////--------------------------------------//     
//     mm_KEY_UP=KEY_UP;	
//		 if( mm_KEY_UP==0)
//		 {
//			 vddio_vol+=5; 
//	     VDDIO_ADJ_SET(vddio_vol);			 
//	     VDDIO_ADJ_EN(1);			 
//		 }
//		 
//     mm_KEY_DOWN=KEY_DOWN;
//		 if( mm_KEY_DOWN==0)
//		 {
//			 vddio_vol-=5; 
//	     VDDIO_ADJ_SET(vddio_vol);			 
//	     VDDIO_ADJ_EN(1);			  
//		 } 
////--------------------------------------//								 
//		mm_KEY_PAUSE=KEY_PAUSE;  
//	 }while((mm_KEY_PAUSE!=0));	 

//       	
//    while(mm_KEY_PAUSE==0)
//	 {
//		 mm_KEY_PAUSE=KEY_PAUSE;
//	 } 	
	 
//    for(coco=0;coco<8;coco++)
//    {
//			 little_value=Measure_IDDIO(mA);
//			 if(little_value>Preset1)
//			 {Preset1=little_value;}	 	   
//		}	
//    decom_a=(u16)(5000000/Preset1);            // IO IDD 50MA
//  
////////////////////////==========key——pass==========			
//	EEPROMBYTE_Write(0x16,decom_a>>8);  DelayMs(5); //uA档     
//	EEPROMBYTE_Write(0x17,decom_a);	    DelayMs(5);
//  if(Tempc!=0x19)
//  {	
//	EEPROMBYTE_Write(0x56,decom_a>>8);  DelayMs(5); //数据备份
//	EEPROMBYTE_Write(0x57,decom_a);	    DelayMs(5); 		 
//  }	
//////////////////////============24c02存储区======		
  
	EEPROMBYTE_Write(0x18,0xcc);	      DelayMs(5); 	//校准完成标志   0xcc
  if(Tempc!=0x19)
  {	
	EEPROMBYTE_Write(0x19,0x19);        DelayMs(5);   //出厂设置封存   0x19
  }	 
	
	HFZ_COF=0;   //
	PA910SPISendValueofCurrent("-%-decom_a-*-:",decom_a);
	PA910SPISendValueofCurrent("-~-complete!!:",0);
	
	USART_REMAP_GPIO_Configuration(); 
//	VDDIO_ADJ_EN(0);	//关闭VDDIO电压	
	
	BEEP_XiXi();   // 嘟比嘟！！
}

void RESTORE_CALIBRATION(void)   
{ 

	u8 data_H,data_L;
	
  data_H=EEPROMBYTE_Read(0x50);      DelayMs(1);  
	data_L=EEPROMBYTE_Read(0x51);      DelayMs(1); 
  EEPROMBYTE_Write(0x10,data_H);     DelayMs(5);  //uA档     
	EEPROMBYTE_Write(0x11,data_L);	   DelayMs(5); 

  data_H=EEPROMBYTE_Read(0x52);      DelayMs(1);  
	data_L=EEPROMBYTE_Read(0x53);      DelayMs(1); 	
  EEPROMBYTE_Write(0x12,data_H);     DelayMs(5);  //IDD    
	EEPROMBYTE_Write(0x13,data_L);	 	 DelayMs(5); 	

//  data_H=EEPROMBYTE_Read(0x54);      DelayMs(1); 
//	data_L=EEPROMBYTE_Read(0x55);      DelayMs(1); 		
//  EEPROMBYTE_Write(0x14,data_H);     DelayMs(5);  //uA档     
//	EEPROMBYTE_Write(0x15,data_L);	   DelayMs(5); 

//  data_H=EEPROMBYTE_Read(0x56);      DelayMs(1);  
//	data_L=EEPROMBYTE_Read(0x57);      DelayMs(1); 		
//  EEPROMBYTE_Write(0x16,data_H);     DelayMs(5);  //IDDIO    
//	EEPROMBYTE_Write(0x17,data_L);	 	 DelayMs(5); 		
//////////////////////============24c02存储区======		
  
//	EEPROMBYTE_Write(0x18,0xcc);	DelayMs(5); 	//校准完成标志   0xcc
	BEEP_XiXi();   // 嘟比嘟！！
}



/*******************************************************
*
*              将新校准系数写入备份区
*
*******************************************************/
void STORE_CALIBRATION(void)   
{ 

	u8 data_H,data_L;
	
  data_H=EEPROMBYTE_Read(0x10);      DelayMs(1);  
	data_L=EEPROMBYTE_Read(0x11);      DelayMs(1); 
  EEPROMBYTE_Write(0x50,data_H);     DelayMs(5);  //uA档     
	EEPROMBYTE_Write(0x51,data_L);	   DelayMs(5); 

  data_H=EEPROMBYTE_Read(0x12);      DelayMs(1);  
	data_L=EEPROMBYTE_Read(0x13);      DelayMs(1); 	
  EEPROMBYTE_Write(0x52,data_H);     DelayMs(5);  //IDD    
	EEPROMBYTE_Write(0x53,data_L);	 	 DelayMs(5); 	

//  data_H=EEPROMBYTE_Read(0x14);      DelayMs(1); 
//	data_L=EEPROMBYTE_Read(0x15);      DelayMs(1); 		
//  EEPROMBYTE_Write(0x54,data_H);     DelayMs(5);  //uA档     
//	EEPROMBYTE_Write(0x55,data_L);	   DelayMs(5); 

//  data_H=EEPROMBYTE_Read(0x16);      DelayMs(1);  
//	data_L=EEPROMBYTE_Read(0x17);      DelayMs(1); 		
//  EEPROMBYTE_Write(0x56,data_H);     DelayMs(5);  //IDDIO    
//	EEPROMBYTE_Write(0x57,data_L);	 	 DelayMs(5); 		
//////////////////////============24c02存储区======		
  
	EEPROMBYTE_Write(0x19,0x19);	     DelayMs(5); 	//校准完成标志   0xcc
	
	BEEP_HuHu();
	BEEP_XiXi();   // 嘟比嘟！！
	
}



void GET_PRESENT(void)    //获取校准系数
{
	 u8 data_H,data_L;
     data_H=EEPROMBYTE_Read(0x10);
	 data_L=EEPROMBYTE_Read(0x11);
	 IDD_UA=(data_H<<8)+data_L;
	
	 data_H=EEPROMBYTE_Read(0x12);
	 data_L=EEPROMBYTE_Read(0x13);
	 IDD_MA=(data_H<<8)+data_L;	  
	
//	 data_H=EEPROMBYTE_Read(0x14);
//	 data_L=EEPROMBYTE_Read(0x15);
//	 IDDIO_UA=(data_H<<8)+data_L;	 
//	
//	 data_H=EEPROMBYTE_Read(0x16);
//	 data_L=EEPROMBYTE_Read(0x17);
//   IDDIO_MA=(data_H<<8)+data_L;	
//		
//		data_H=EEPROMBYTE_Read(0x18);
//		data_L=EEPROMBYTE_Read(0x19);
//		IDDIO_MA=(data_H<<8)+data_L;	
}




void PA910SPISendValueofCurrent(char *p,u32 datcf)
/*********************************************************************************************************
*USART_GPIO_Configuration(); 执行之后才可以运行本函数从PA9 PA10以串口方式吐出数据，串口波特率：115200
*本函数运行之后需要执行USART_REMAP_GPIO_Configuration(); 以恢复原本设置
*void PA910SPISendValueofCurrent(char *p,u32 datcf)  使用范例:PA910SPISendValueofCurrent("---C.mA---:",0x03e8);
*参数*p 为字符类指针,datcf为16位变量，执行后串口收到数据为：---C.mA---:0x03e8 <=> 1000
**注意，16进制显示时，32bit数据的高16位会自动舍弃，10进制部分不受影响
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
						if(tep_h<0x0a)		// ?a陏?
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
//       USART_ReceiveData(USARTx);
       USART_SendData(USARTx, 32);            //
       while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){} 
       USART_SendData(USARTx, 32);            //
       while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET){}		
		}

//=================================================================================================



