/* Includes ------------------------------------------------------------------*/
#include <V_SET.h>


extern BYTE pucData[2];


//////////////////###############################################################################
////////////////////VDD  设定函数
////////////////////VDD  设定函数
////////////////////VDD  设定函数
////////////////////VDD  设定函数
////////////////////VDD  设定函数
//////////////////###############################################################################

#define COMMAND_NOP_INDEX				0x00
#define COMMAND_WRITE_RDAC_INDEX		0x01
#define COMMAND_READ_RDAC_INDEX			0x02
#define COMMAND_STORE_RDAC_SETTNG_INDEX		0x03
#define COMMAND_REFRESH_RDAC_INDEX			0x04
#define COMMAND_READ_OTP_CONTENT_INDEX		0x05
#define COMMAND_READ_OTP_INDEX			0x06
#define COMMAND_WRITE_CONTROL_INDEX		0x07
#define COMMAND_READ_CONTROL_INDEX		0x08
#define COMMAND_SHUTDOWN_INDEX			0x09

//// Psudo functions
void AD5274_5271_I2CSDASetInput(void); // Set I2C SDA signal as input
void AD5274_5271_I2CSDASetOutput(void); // Set I2C SDA signal as output


void AD5274_5271_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//禁止jtag，以空出PB3,PB4,PA15

     //================================================
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = AD_PIN_SYNC ;
	GPIO_Init(GPIO_SYNC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin =  AD_PIN_SCL;
	GPIO_Init(GPIO_SCLK, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = AD_PIN_SDO  ;
    GPIO_Init(GPIO_SDO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin =  AD_PIN_SDA ;
    GPIO_Init(GPIO_DIN, &GPIO_InitStructure);
      //----------------------------------------
}


void AD5274_5271_I2CSDASetInput(void)
{
	GPIO_InitTypeDef QGPIO_InitStructure;
             QGPIO_InitStructure.GPIO_Pin = AD_PIN_SDA;
             QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ; //GPIO_Mode_IN_FLOATING;
             GPIO_Init(GPIO_AD_SDA, &QGPIO_InitStructure);
} 
void AD5274_5271_I2CSDASetOutput(void)
{ 
	GPIO_InitTypeDef QGPIO_InitStructure;
         QGPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
         QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
         QGPIO_InitStructure.GPIO_Pin =  AD_PIN_SDA;
         GPIO_Init(GPIO_AD_SDA, &QGPIO_InitStructure);
}

// Start bit of I2C waveform
void AD5274_5271_I2CStart(void)
{
    AD5274_5271_I2CSDASetOutput();

    AD_TPSDA_H; 

    AD_TPSCL_H;
	  TWait();
    AD_TPSDA_L; 

	  TWait();

    AD_TPSCL_L; 

}

// Stop bit of I2C waveform
void AD5274_5271_I2CStop(void)
{
    AD5274_5271_I2CSDASetOutput();

    AD_TPSCL_H;
    AD_TPSDA_L;

    AD_TPSCL_H;
    TWait();
    AD_TPSDA_H;

    TWait();
}

// Send one byte from host to client
bool AD5274_5271_I2CSendByte(BYTE ucData)
{
	bool bRet;
	unsigned char i=8; 
	
	AD5274_5271_I2CSDASetOutput();
	
	
	
	do{
		
		if (ucData&0x80)
			 AD_TPSDA_H;
			 else 
			 	AD_TPSDA_L;
		 TWait();//-----------------------????
		AD_TPSCL_H;
		TWait();
		ucData<<=1;
		AD_TPSCL_L;
	            if (i>1) //判断是否传递最后一位数据，若是最后一位则TSCL拉低后不用等待
		TWait();
		}while(--i);
	
	AD5274_5271_I2CSDASetInput();
	
	bRet=FALSE;
	AD_TPSCL_H;
	TWait();
	if (GPIO_ReadInputDataBit(GPIO_AD_SDA, AD_PIN_SDA))
	bRet=TRUE;
	AD_TPSCL_L;
	TWait();
	return bRet;
 }
  

// Receive one byte form client to host
BYTE AD5274_5271_I2CReciveByte(bool bAck)
{
	BYTE ucRet,i;
	
	AD5274_5271_I2CSDASetInput();
	
	ucRet = 0;
	i=8;
	do
	{	AD_TPSCL_H;
		TWait();
		ucRet<<=1;
		if(GPIO_ReadInputDataBit(GPIO_AD_SDA, AD_PIN_SDA))
			ucRet++;
		AD_TPSCL_L;
	            if (i>1) //判断是否传递最后一位数据，若是最后一位则TSCL拉低后不用等待
		TWait();
	 }while(--i);
	
	AD5274_5271_I2CSDASetOutput();
	
	if (bAck==0)
	 AD_TPSDA_L;
	else AD_TPSDA_H;
	
	AD_TPSCL_H;
	TWait();
	AD_TPSCL_L;
	TWait();         
	return ucRet;
}

bool AD5274_5271_I2CTest(BYTE ucDeviceAddr)
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
bool AD5274_5271_I2CSend(BYTE ucDeviceAddr, BYTE ucIndex, BYTE* pucData, unsigned int unDataLength)
{
      unsigned int i;
      bool bRet = TRUE;

      AD5274_5271_I2CStart();

      if(AD5274_5271_I2CSendByte(ucDeviceAddr & 0xFE) == ACK)
        {
          ucIndex = (ucIndex<<2) + (pucData[0]&0x3F);
		  if(AD5274_5271_I2CSendByte(ucIndex) == ACK)
            {
              for(i = 0; i < unDataLength; i++)
              {
                  if(AD5274_5271_I2CSendByte(pucData[i+1]) == NAK)
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
      AD5274_5271_I2CStop();
      return bRet;
}
      // I2C receive data function
bool AD5274_5271_I2CReceive(BYTE ucDeviceAddr, BYTE ucIndex, BYTE* pucData, unsigned int unDataLength) //(器件地址，buffer类型，被读出数据保存位置，读出数据长度)
{
      unsigned int i;
      bool bRet = TRUE;
      AD5274_5271_I2CStart();
      if(AD5274_5271_I2CSendByte(ucDeviceAddr & 0xFE) == ACK)
        {
           ucIndex = (ucIndex<<2) + pucData[0];
		   if(AD5274_5271_I2CSendByte(ucIndex) == ACK)
           {
			   if(AD5274_5271_I2CSendByte(pucData[1]) == ACK)
			   {
				   AD5274_5271_I2CStop();		 
				   AD5274_5271_I2CStart();
	               if(AD5274_5271_I2CSendByte(ucDeviceAddr | 0x01) == ACK)
	               {
	                 for(i = 0; i < unDataLength-1; i++)
	                   {  pucData[i] = AD5274_5271_I2CReciveByte(ACK);
	                   }
	                 pucData[unDataLength - 1] = AD5274_5271_I2CReciveByte(NAK);
	               }
	               else
	               {          bRet = FALSE;           }
			   }
			   else
               {          bRet = FALSE;           }
           }
           else
           {           bRet = FALSE;           }
         }
      else
        {           bRet = FALSE;           }
      AD5274_5271_I2CStop();
      return bRet;
}


//////////////////###############################################################################
//////////////////VDDIO设定函数
//////////////////VDDIO设定函数
//////////////////VDDIO设定函数
//////////////////VDDIO设定函数
//////////////////VDDIO设定函数
//////////////////###############################################################################
void P557_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//禁止jtag，以空出PB3,PB4,PA15

    ///////////// for M6 use //================================================
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//	GPIO_InitStructure.GPIO_Pin = P557_PIN_SCL;
//	GPIO_Init(GPIO_M6_SCL, &GPIO_InitStructure);
//
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//    GPIO_InitStructure.GPIO_Pin =  P557_PIN_SDA;
//    GPIO_Init(GPIO_M6_SDA, &GPIO_InitStructure);
      //----------------------------------------


	///////// for M1 use //================================================
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = P557_M1_PIN_SCL;
	GPIO_Init(GPIO_P557_M1_SCL, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin =  P557_M1_PIN_SDA;
    GPIO_Init(GPIO_P557_M1_SDA, &GPIO_InitStructure);
      //----------------------------------------
}


void P557_I2CSDASetInput(void)
{
	GPIO_InitTypeDef QGPIO_InitStructure;
             QGPIO_InitStructure.GPIO_Pin = P557_PIN_SDA;
             QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; //GPIO_Mode_IN_FLOATING;
             GPIO_Init(GPIO_M6_SDA, &QGPIO_InitStructure);
} 
void P557_I2CSDASetOutput(void)
{ 
	GPIO_InitTypeDef QGPIO_InitStructure;
         QGPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
         QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
         QGPIO_InitStructure.GPIO_Pin =  P557_PIN_SDA;
         GPIO_Init(GPIO_M6_SDA, &QGPIO_InitStructure);
}

// Start bit of I2C waveform
void P557_I2CStart(void)
{
    P557_I2CSDASetOutput();

    P557_TPSDA_H; 

    P557_TPSCL_H;
	  TWait();
    P557_TPSDA_L; 

	  TWait();

    P557_TPSCL_L; 

}

// Stop bit of I2C waveform
void P557_I2CStop(void)
{
    P557_I2CSDASetOutput();

    P557_TPSCL_H;
    P557_TPSDA_L;

    P557_TPSCL_H;
    TWait();
    P557_TPSDA_H;

    TWait();
}

// Send one byte from host to client
bool P557_I2CSendByte(BYTE ucData)
{
	bool bRet;
	unsigned char i=8; 
	
	P557_I2CSDASetOutput();

	do{
		
		if (ucData&0x80)
			 P557_TPSDA_H;
			 else 
			 	P557_TPSDA_L;
		 TWait();//-----------------------????
		P557_TPSCL_H;
		TWait();
		ucData<<=1;
		P557_TPSCL_L;
                if (i>1) //判断是否传递最后一位数据，若是最后一位则TSCL拉低后不用等待
		TWait();
    	}while(--i);
	
	P557_I2CSDASetInput();
	
	bRet=FALSE;
	P557_TPSCL_H;
	TWait();
	if (GPIO_ReadInputDataBit(GPIO_M6_SDA, P557_PIN_SDA))
	bRet=TRUE;
	P557_TPSCL_L;
	TWait();
	return bRet;
}
  

// Receive one byte form client to host
BYTE P557_I2CReciveByte(bool bAck)
{
	BYTE ucRet,i;
	
	P557_I2CSDASetInput();
	
	ucRet = 0;
	i=8;
	do
	{	P557_TPSCL_H;
		TWait();
		ucRet<<=1;
		if(GPIO_ReadInputDataBit(GPIO_M6_SDA, P557_PIN_SDA))
			ucRet++;
		P557_TPSCL_L;
                if (i>1) //判断是否传递最后一位数据，若是最后一位则TSCL拉低后不用等待
		TWait();
	 }while(--i);

	P557_I2CSDASetOutput();
	
	if (bAck==0)
		P557_TPSDA_L;
	else P557_TPSDA_H;
	
	P557_TPSCL_H;
	TWait();
	P557_TPSCL_L;
	TWait();         
	return ucRet;
}

bool P557_I2CTest(BYTE ucDeviceAddr)
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
bool P557_I2CSend(BYTE ucDeviceAddr, BYTE ucIndex, BYTE* pucData, unsigned int unDataLength)
{
      unsigned int i;
      bool bRet = TRUE;
      P557_I2CStart();
      if(P557_I2CSendByte(ucDeviceAddr & 0xFE) == ACK)
        {
          if(P557_I2CSendByte(ucIndex) == ACK)
            {
              for(i = 0; i < unDataLength; i++)
              {
                  if(P557_I2CSendByte(pucData[i]) == NAK)
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
      P557_I2CStop();
      return bRet;
}
      // I2C receive data function
bool P557_I2CReceive(BYTE ucDeviceAddr, BYTE ucIndex, BYTE* pucData, unsigned int unDataLength) //(器件地址，buffer类型，被读出数据保存位置，读出数据长度)
{
      unsigned int i;
      bool bRet = TRUE;
      P557_I2CStart();
      if(P557_I2CSendByte(ucDeviceAddr & 0xFE) == ACK)
        {
           if(P557_I2CSendByte(ucIndex) == ACK)
           {

			   P557_I2CStop();		 ////Optional
			   P557_I2CStart();
               if(P557_I2CSendByte(ucDeviceAddr | 0x01) == ACK)
               {
                 for(i = 0; i < unDataLength - 1; i++)
                   {  pucData[i] = P557_I2CReciveByte(ACK);
                   }
                 pucData[unDataLength - 1] = P557_I2CReciveByte(NAK);
               }
               else
               {          bRet = FALSE;           }
           }
           else
           {           bRet = FALSE;           }
         }
      else
        {           bRet = FALSE;           }
      P557_I2CStop();
      return bRet;
}


//////////////////###############################################################################
////////////////////VOTP  设定函数
////////////////////VOTP  设定函数
////////////////////VOTP  设定函数
////////////////////VOTP  设定函数
////////////////////VOTP  设定函数
//////////////////###############################################################################
void P557_M1_I2CSDASetInput(void)
{
	GPIO_InitTypeDef QGPIO_InitStructure;
             QGPIO_InitStructure.GPIO_Pin = P557_M1_PIN_SDA;
             QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ; //GPIO_Mode_IN_FLOATING;
             GPIO_Init(GPIO_P557_M1_SDA, &QGPIO_InitStructure);
} 
void P557_M1_I2CSDASetOutput(void)
{ 
	GPIO_InitTypeDef QGPIO_InitStructure;
         QGPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
         QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
         QGPIO_InitStructure.GPIO_Pin =  P557_M1_PIN_SDA;
         GPIO_Init(GPIO_P557_M1_SDA, &QGPIO_InitStructure);
}

// Start bit of I2C waveform
void P557_M1_I2CStart(void)
{
    P557_M1_I2CSDASetOutput();

    P557_M1_TPSDA_H; 

    P557_M1_TPSCL_H;
	  TWait();
    P557_M1_TPSDA_L; 

	  TWait();

    P557_M1_TPSCL_L; 

}

// Stop bit of I2C waveform
void P557_M1_I2CStop(void)
{
    P557_M1_I2CSDASetOutput();

    P557_M1_TPSCL_H;
    P557_M1_TPSDA_L;

    P557_M1_TPSCL_H;
    TWait();
    P557_M1_TPSDA_H;

    TWait();
}

// Send one byte from host to client
bool P557_M1_I2CSendByte(BYTE ucData)
{
	bool bRet;
	unsigned char i=8; 
	
	P557_M1_I2CSDASetOutput();

	do{
		
		if (ucData&0x80)
			 P557_M1_TPSDA_H;
			 else 
			 	P557_M1_TPSDA_L;
		 TWait();//-----------------------????
		P557_M1_TPSCL_H;
		TWait();
		ucData<<=1;
		P557_M1_TPSCL_L;
                if (i>1) //判断是否传递最后一位数据，若是最后一位则TSCL拉低后不用等待
		TWait();
    	}while(--i);
	
	P557_M1_I2CSDASetInput();
	
	bRet=FALSE;
	P557_M1_TPSCL_H;
	TWait();
	if (GPIO_ReadInputDataBit(GPIO_P557_M1_SDA, P557_M1_PIN_SDA))
	bRet=TRUE;
	P557_M1_TPSCL_L;
	TWait();
	return bRet;
}
  

// Receive one byte form client to host
BYTE P557_M1_I2CReciveByte(bool bAck)
{
	BYTE ucRet,i;
	
	P557_M1_I2CSDASetInput();
	
	ucRet = 0;
	i=8;
	do
	{	P557_M1_TPSCL_H;
		TWait();
		ucRet<<=1;
		if(GPIO_ReadInputDataBit(GPIO_P557_M1_SDA, P557_M1_PIN_SDA))
			ucRet++;
		P557_M1_TPSCL_L;
                if (i>1) //判断是否传递最后一位数据，若是最后一位则TSCL拉低后不用等待
		TWait();
	 }while(--i);

	P557_M1_I2CSDASetOutput();
	
	if (bAck==0)
		P557_M1_TPSDA_L;
	else P557_M1_TPSDA_H;
	
	P557_M1_TPSCL_H;
	TWait();
	P557_M1_TPSCL_L;
	TWait();         
	return ucRet;
}

bool P557_M1_I2CTest(BYTE ucDeviceAddr)
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
bool P557_M1_I2CSend(BYTE ucDeviceAddr, BYTE ucIndex, BYTE* pucData, unsigned int unDataLength)
{
      unsigned int i;
      bool bRet = TRUE;
      P557_M1_I2CStart();
      if(P557_M1_I2CSendByte(ucDeviceAddr & 0xFE) == ACK)
        {
          if(P557_M1_I2CSendByte(ucIndex) == ACK)
            {
              for(i = 0; i < unDataLength; i++)
              {
                  if(P557_M1_I2CSendByte(pucData[i]) == NAK)
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
      P557_M1_I2CStop();
      return bRet;
}
      // I2C receive data function
bool P557_M1_I2CReceive(BYTE ucDeviceAddr, BYTE ucIndex, BYTE* pucData, unsigned int unDataLength) //(器件地址，buffer类型，被读出数据保存位置，读出数据长度)
{
      unsigned int i;
      bool bRet = TRUE;
      P557_M1_I2CStart();
      if(P557_M1_I2CSendByte(ucDeviceAddr & 0xFE) == ACK)
        {
           if(P557_M1_I2CSendByte(ucIndex) == ACK)
           {

			   P557_M1_I2CStop();		 ////Optional
			   P557_M1_I2CStart();
               if(P557_M1_I2CSendByte(ucDeviceAddr | 0x01) == ACK)
               {
                 for(i = 0; i < unDataLength - 1; i++)
                   {  pucData[i] = P557_M1_I2CReciveByte(ACK);
                   }
                 pucData[unDataLength - 1] = P557_M1_I2CReciveByte(NAK);
               }
               else
               {          bRet = FALSE;           }
           }
           else
           {           bRet = FALSE;           }
         }
      else
        {           bRet = FALSE;           }
      P557_M1_I2CStop();
      return bRet;
}













//////////////////###############################################################################
////////////////////VDD  设定函数
////////////////////VDD  设定函数
////////////////////VDD  设定函数
////////////////////VDD  设定函数
////////////////////VDD  设定函数
//////////////////###############################################################################
void VDD_ADJ_SET(u32 VDD_settig)
{
	AD5274_5271_GPIO_Config();

	////////对芯片进行复位动作。
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
	DelayMs(10);
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	DelayMs(10);
	GPIO_ResetBits(GPIOC, GPIO_Pin_7);
	DelayMs(10);
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	DelayMs(10);
///////////////正常设定电压参数。。
	pucData[0] = 0x00;
  	pucData[1] = 0x03;
	AD5274_5271_I2CSend(DEVICE_ADDRESS_M5, COMMAND_WRITE_CONTROL_INDEX, pucData, 1);
	DelayMs(10);
	
	////////////设定电压上下限，防止bug以及烧毁元件。
	if(VDD_settig<270)
	{	VDD_settig = 270;	}
	if(VDD_settig>620)
	{	VDD_settig = 620;	}

	VDD_settig = VDD_settig*10;

	VDD_settig = ((57600*923*8)/((VDD_settig-923)) - 80000)/625 ;

	pucData[0] = VDD_settig>>6;
	pucData[1] = VDD_settig<<2;
	AD5274_5271_I2CSend(DEVICE_ADDRESS_M5, COMMAND_WRITE_RDAC_INDEX, pucData, 1);


}


void VDD_ADJ_EN(u8 vset)
{
	u16 temp_Vset=0;

		///////////////////进行OTP判定，若产品没有进行过OTP，则进入OTP流程。
	pucData[0] = 0x00;
  	pucData[1] = 0x00;
	AD5274_5271_I2CReceive(DEVICE_ADDRESS_M5, COMMAND_READ_OTP_INDEX, pucData, 2);

	if(pucData[1]<1)
	{
		////////对芯片进行复位动作。
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);
		DelayMs(10);
		GPIO_SetBits(GPIOC, GPIO_Pin_7);
		DelayMs(10);
		GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		DelayMs(10);
		GPIO_SetBits(GPIOC, GPIO_Pin_7);
		DelayMs(10);

		pucData[0] = 0x00;
	  	pucData[1] = 0x03;
		AD5274_5271_I2CSend(DEVICE_ADDRESS_M5, COMMAND_WRITE_CONTROL_INDEX, pucData, 1);

		////////////////////OTP默認值設定為2.90V左右，0x0360--->2.90V
		pucData[0] = 0x03;
		pucData[1] = 0x60;
		AD5274_5271_I2CSend(DEVICE_ADDRESS_M5, COMMAND_WRITE_RDAC_INDEX, pucData, 1);

		pucData[0] = 0x00;
	  	pucData[1] = 0x00;
		AD5274_5271_I2CSend(DEVICE_ADDRESS_M5, COMMAND_STORE_RDAC_SETTNG_INDEX, pucData, 1);

	}
	
	if(vset==1)
	{
		GPIO_SetBits(GPIO_AD_SCL, VDD_ADJPin_EN); 
	}
	else
	{
		GPIO_ResetBits(GPIO_AD_SCL, VDD_ADJPin_EN); 	
	}
}

//////////////////###############################################################################
//////////////////VDDIO设定函数
//////////////////VDDIO设定函数
//////////////////VDDIO设定函数
//////////////////VDDIO设定函数
//////////////////VDDIO设定函数
//////////////////###############################################################################
void VDDIO_ADJ_SET(u32 vset)
{
	u8 V16V_EN,V800mV,V400mV,V200mV,V100mV,V50mV;

	V16V_EN = V800mV = V400mV = V200mV = V100mV = V50mV = 1;

	P557_GPIO_Config();

	pucData[0] = 0x00;
	P557_I2CSend(DEVICE_ADDRESS_M6, 0x02, pucData, 1);	
	
	pucData[0] = 0xFF;
	P557_I2CSend(DEVICE_ADDRESS_M6, 0x03, pucData, 1);
	
	if((vset%10)>0)
	{
		V50mV = 0;
	}

	vset = vset/10;

   ////////////设定电压上下限，防止bug以及烧毁元件。
	if(vset < 5)
	{	vset = 5;	}
	if(vset > 36)
	{	vset = 36;	}
		
	vset -= 5;

	if(vset >= 16)
	{
		V16V_EN = 0;
		vset -= 16;
	}
	
	if(vset >= 8)
	{
		V800mV = 0;
		vset -= 8;
	}

	if(vset >= 4)
	{
		V400mV = 0;
		vset -= 4;
	}

	if(vset >= 2)
	{
		V200mV = 0;
		vset -= 2;
	}

	if(vset >= 1)
	{
		V100mV = 0;
		vset -= 1;
	}

	////set output value
	pucData[0] = (V16V_EN<<2) + (V800mV<<3) + (V400mV<<4) + (V200mV<<5) + (V100mV<<6) + (V50mV<<7);
	P557_I2CSend(DEVICE_ADDRESS_M6, 0x01, pucData, 1);
	

	///set  input
	P557_I2CSend(DEVICE_ADDRESS_M6, 0x03, pucData, 1);

}

//////////////////###############################################################################
////////////////////VOTP  设定函数
////////////////////VOTP  设定函数
////////////////////VOTP  设定函数
////////////////////VOTP  设定函数
////////////////////VOTP  设定函数
//////////////////###############################################################################
void VOTP_ADJ_SET(u32 vset)
{
	u8 V6P4V2,V6P4V1,V3P2,V1P6,V0P8,V0P4,V0P2,V0P1;

	V6P4V2 = V6P4V1 = V3P2 = V1P6 = V0P8 = V0P4 = V0P2 = V0P1 = 1;

	P557_GPIO_Config();

	pucData[0] = 0x00;
	P557_M1_I2CSend(DEVICE_ADDRESS_M1, 0x02, pucData, 1);	
	
	pucData[0] = 0xFF;
	P557_M1_I2CSend(DEVICE_ADDRESS_M1, 0x03, pucData, 1);
	
	vset = vset/10;
	
	////////////设定电压上下限，防止bug以及烧毁元件。	
	if(vset < 14)
	{	vset = 14;	}	
	if(vset > 205)
	{	vset = 205;	}

	vset -= 14;
	
	if(vset >= 128)
	{
		V6P4V2 = 0;
		vset -= 64;
	}

	if(vset >= 64)
	{
		V6P4V1 = 0;
		vset -= 64;
	}

	if(vset >= 32)
	{
		V3P2 = 0;
		vset -= 32;
	}

	if(vset >= 16)
	{
		V1P6 = 0;
		vset -= 16;
	}
	
	if(vset >= 8)
	{
		V0P8 = 0;
		vset -= 8;
	}

	if(vset >= 4)
	{
		V0P4 = 0;
		vset -= 4;
	}

	if(vset >= 2)
	{
		V0P2 = 0;
		vset -= 2;
	}

	if(vset >= 1)
	{
		V0P1 = 0;
		vset -= 1;
	}


	////set output value
	pucData[0] = (V6P4V2<<7) + (V6P4V1<<6) + (V3P2<<5) + (V1P6<<4) + (V0P8<<3) + (V0P4<<2) + (V0P2<<1) + (V0P1<<0);
	P557_M1_I2CSend(DEVICE_ADDRESS_M1, 0x01, pucData, 1);
	

	///set  input
	P557_M1_I2CSend(DEVICE_ADDRESS_M1, 0x03, pucData, 1);
}




