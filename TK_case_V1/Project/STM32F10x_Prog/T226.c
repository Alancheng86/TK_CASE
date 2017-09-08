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
   u16 Preset=0;
signed short int decom=0;
//// Psudo functions
void I2CSDASetInput(void); // Set I2C SDA signal as input
void I2CSDASetOutput(void); // Set I2C SDA signal as output


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
	
	
	T226_I2CReceive(DEVICE_ADDRESS_M9, COMMAND_ID_INDEX, pucData, 2);	DelayMs(10);
	if(pucData[0] == 0x22)
	{
		;
	}
	else
	{
		DelayMs(10);

		while(1);		   ///////檢測到T226損壞，暫停此處或者報警。
		
	}	

	T226_I2CReceive(DEVICE_ADDRESS_M12, COMMAND_ID_INDEX, pucData, 2);	DelayMs(10);
	if(pucData[0] == 0x22)
	{
		;
	}
	else
	{
		DelayMs(10);

		while(1);		   ///////檢測到T226損壞，暫停此處或者報警。
		
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
	
	 if(gears==uA)
	 {
			GPIO_SetBits(GPIOA, GPIO_Pin_0);   
			T226_I2CReceive(DEVICE_ADDRESS_M9, COMMAND_SHUNT_VLOTAGE_SETTING, pucData, 2);	DelayMs(5); 
			VSHUNT=(pucData[0]<<8)+pucData[1];   //最小0x8000	
		 
			IDD_value=~VSHUNT;

			IDD_value=IDD_value - IDD_UA;	
		 
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
	 u8 sign;   //1 wei "+"   ;  0 wei "-"
	 u16 mm_KEY_PAUSE = 1;	

	BEEP_BIBI();	  //喇叭叫兩聲，提示進入校準
	 do
	 {
   		 Preset=Measure_IDD(uA);  //50uA 环境
		mm_KEY_PAUSE=KEY_PAUSE;  		           
//		ShowTxt("38,20,299,IDD:");	  		DelayMs(150);
//		ShowData("38,200,299",Preset);    DelayMs(150); 

	 }while((mm_KEY_PAUSE!=0));
//==========key——pass==========		 
    while(mm_KEY_PAUSE==0)
	 {mm_KEY_PAUSE=KEY_PAUSE;} 
	 
	 decom=Preset-50;                 //2221
//============20c02存储区======		

	EEPROMBYTE_Write(0x10,decom>>8);   DelayMs(5); //uA档     
	EEPROMBYTE_Write(0x11,decom);	DelayMs(5); 
	 
	 BEEP_BIBI();
//-------------------------------------------------   
	 do
	 {
    Preset=Measure_IDD(mA);  //	50mA的环境
		mm_KEY_PAUSE=KEY_PAUSE;  
	 }while((mm_KEY_PAUSE!=0));    //4008
//==========key——pass==========			 
    while(mm_KEY_PAUSE==0)
	 {mm_KEY_PAUSE=KEY_PAUSE;}  
    decom=(u16)(500000/Preset);            //50MA
		
//============20c02存储区======		
  EEPROMBYTE_Write(0x12,decom>>8);  DelayMs(5);      
	EEPROMBYTE_Write(0x13,decom);	 	DelayMs(5); 	

	BEEP_BIBI();
	 
	GPIO_ResetBits(GPIOA, GPIO_Pin_1);  // 关闭VDD电压。
	 
	 GPIO_SetBits(GPIOB, GPIO_Pin_15);  //开启IOVDD电压。
//---------------------------------------------------------		     
	 do
	 {
    Preset=Measure_IDDIO(uA);  //   uA档
		mm_KEY_PAUSE=KEY_PAUSE;  
	 }while((mm_KEY_PAUSE!=0));
//==========key——pass==========			 
    while(mm_KEY_PAUSE==0)
	 {mm_KEY_PAUSE=KEY_PAUSE;} 
	 
    decom=Preset-50;
	 	
//============20c02存储区======			
  EEPROMBYTE_Write(0x14,decom>>8); DelayMs(5);  //uA档     
	EEPROMBYTE_Write(0x15,decom);	 	DelayMs(5);	
	
	BEEP_BIBI();  
	 do
	 {
    Preset=Measure_IDDIO(mA);  //	   mA档
		mm_KEY_PAUSE=KEY_PAUSE;  
	 }while((mm_KEY_PAUSE!=0));
//==========key——pass==========			 
    while(mm_KEY_PAUSE==0)
	 {mm_KEY_PAUSE=KEY_PAUSE;} 	 

    decom=(u16)(500000/Preset);            //50MA

//==========key——pass==========			
	EEPROMBYTE_Write(0x16,decom>>8);  DelayMs(5); //uA档     
	EEPROMBYTE_Write(0x17,decom);	DelayMs(5);
	 
	
//============20c02存储区======		
  
		EEPROMBYTE_Write(0x18,0xcc);	DelayMs(5); 	//校准完成标志   0xcc
		BEEP_BIBI();
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
	
	 data_H=EEPROMBYTE_Read(0x14);
	 data_L=EEPROMBYTE_Read(0x15);
	 IDDIO_UA=(data_H<<8)+data_L;	 
	
	 data_H=EEPROMBYTE_Read(0x16);
	 data_L=EEPROMBYTE_Read(0x17);
   IDDIO_MA=(data_H<<8)+data_L;	
		
//		data_H=EEPROMBYTE_Read(0x18);
//		data_L=EEPROMBYTE_Read(0x19);
//		IDDIO_MA=(data_H<<8)+data_L;	
}
