
#include <IIC.h>
#include <lcd.h>
#include <math.h>


#define uchar unsigned char

#define Uchar unsigned char
#define Uint  unsigned int

//////////////////////////-------

	void IICTWait(void);
	void IICStart(void);
	void IICStop(void);
	void IICSendTAck(unsigned char ack) ;          //确认接收
	unsigned char IICTestTAck(uchar tep) ;
	void IICSentByte(unsigned char bytedata) ;     //传送一个字节数据
	unsigned char IICReceiveByte(void) ;           //接收一个字节数据
	
	void PIN_SDA1_IICFLOAT(void);
	void PIN_SDA1_IICOUTPUT(void);


void Delay (u32 j);
/*******************************************************************************
* Function Name  : Delay_ms //延时N毫秒
* Description    : 延时
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*
void Delay (u32 k)  
{
  u32 i,j;
  for (i=0; i<k; i++)
  {
     for (j=0; j<10285; j++);
  }
}
*/
//---------------------------------
void IICTWait(void)				  //1=200ns
{
 volatile u32 i,j;
  for (i=0; i<3; i++)
  {
     for (j=0; j<50; j++);  		   //2020
  }
}
//---------------------------------
void EEPROM_GPIO_Config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//禁止jtag，以空出PB3,PB4,PA15
	
	//================================================
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin = PIN_SDA1;
		GPIO_Init(GPIO_EEPROM_SDA , &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Pin = PIN_SCL1; 
		GPIO_Init(GPIO_EEPROM_SCL , &GPIO_InitStructure);
		//----------------------------------------
}


void PIN_SDA1_IICFLOAT(void)
	{ 
		GPIO_InitTypeDef QGPIO_InitStructure;
		QGPIO_InitStructure.GPIO_Pin = PIN_SDA1;
	  QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	  GPIO_Init(GPIO_EEPROM_SDA, &QGPIO_InitStructure);
	} 
void PIN_SDA1_IICOUTPUT(void)
  {
		GPIO_InitTypeDef QGPIO_InitStructure;
	  QGPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	  QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	  QGPIO_InitStructure.GPIO_Pin =  PIN_SDA1;
	  GPIO_Init(GPIO_EEPROM_SDA, &QGPIO_InitStructure);
  }

/********************************************/

void IICStart(void)
{ 
	ESDA_H; 
	ESCL_H;
	IICTWait();
	ESDA_L;
        IICTWait();  //++++++++
	ESCL_L;
	IICTWait();
}
/********************************************/
void IICStop(void)
{	
	ESCL_L;
	ESDA_L;
	  
	ESCL_H; IICTWait();
	ESDA_H; 
	IICTWait();  //++++++++-----
	ESCL_L; IICTWait();
}
/********************************************/
void IICSendTAck(uchar ack)           //确认接收
{ 
	if (ack)
	     ESDA_H;
	else 
             ESDA_L;
        ESCL_H;
 	IICTWait();
	ESCL_L;
	IICTWait();
}

uchar IICTestTAck(uchar tep)         //响应应答信号  
{
	uchar ack=0;uchar flag;
  PIN_SDA1_IICFLOAT();  
  flag=tep;
  ESCL_H;
	IICTWait();
	if (GPIO_ReadInputDataBit(GPIO_EEPROM_SDA, PIN_SDA1))
	    ack=1;
	ESCL_L;
        if (flag==0)  //若flag为0，则意味接下来继续处于发送状态，所以还原端口为输出状态
	PIN_SDA1_IICOUTPUT();
	return ack;
}

void IICSentByte(unsigned char bytedata)          //传送一个字节数据
{    unsigned char i=8;
	do{
		//TSDA=bytedata&0x80;
		if (bytedata&0x80)
			 ESDA_H;
		   else 
			 ESDA_L;
                IICTWait();  //++++++++   
		ESCL_H;
		IICTWait();
		bytedata<<=1;
		ESCL_L;
           //     if (i>1) //判断是否传递最后一位数据，若是最后一位则TSCL拉低后不用等待
		IICTWait();
    	}while(--i);
}

unsigned char IICReceiveByte(void)             				//接收一个字节数据
{	unsigned char x=0,i=8;
	
  PIN_SDA1_IICFLOAT();
	do
	{	ESCL_H;
		IICTWait();
		x<<=1;
		if(GPIO_ReadInputDataBit(GPIO_EEPROM_SDA, PIN_SDA1))
			x++;
		ESCL_L;
               // if (i>1) //判断是否传递最后一位数据，若是最后一位则TSCL拉低后不用等待
	        IICTWait();
	 }while(--i);
	 
	PIN_SDA1_IICOUTPUT();
	return(x);
}

//=========================================================================
unsigned char EEPROMBYTE_Write(unsigned int addr,unsigned char regdata)
{
	  unsigned char sign,TEP;
    Delay(1);
		sign=0;	
//		TEP=addr>>8;
//		TEP &=0X01;   //针对24LC08
//    TEP <<=1;

	  TEP =0XA2;   //CONTROL BYTE:1010 A2 * * W =10100000 ; 其中A2=0
	
		IICStart();
		IICSentByte(TEP);     //CONTROL BYTE:1010 A2 a9 a8 W =10100 a9 a8 0
		if(IICTestTAck(0))
		sign+=0x01;
				
		TEP=addr; 
		IICSentByte(TEP);
		if(IICTestTAck(0))
    { sign+=0x04;}
        
		IICSentByte(regdata);
 		if(IICTestTAck(0))
		{//IICTWait();IICTWait();IICTWait();Delay(20);
			sign+=0x08; 
//    while(1);   //判断数据是否正确写入,测试用
		}	
		
		IICStop();
    return sign;
}
unsigned char EEPROMBYTE_Read(unsigned int addr)
{
	unsigned char sign=0,TEP;
	
//	TEP=addr>>8;
//	TEP &=0X03;   //针对24LC08
//  TEP <<=1;

	TEP =0XA2;   //CONTROL BYTE:1010 A2 * * W =10100000 ; 其中A2=0
	
	IICStart();
	IICSentByte(TEP);      ////CONTROL BYTE:1010 A2 A1 A0 W =10100000
	if(IICTestTAck(0))
	   sign+=0x01;
	
	//TEP=addr>>8;
	//IICSentByte(TEP);
	//if(IICTestTAck(0))
	//sign+=0x02;
	TEP=addr; 	
	IICSentByte(TEP);
	if(IICTestTAck(0))
	sign+=0x04;
	
	IICStart();

	TEP = 0XA3;   //CONTROL BYTE:1010 A2 * * W =10100000 ; 其中A2=0

//	IICSentByte(0xA1);      ////CONTROL BYTE:1010 A2 A1 A0 R =10100001  MS此处不需要设定地址，资料没有明确说明
	IICSentByte(TEP);      ////CONTROL BYTE:1010 A2 A1 A0 R =10100001  MS此处不需要设定地址，资料没有明确说明
	if(IICTestTAck(1))
	sign+=0x08;
	
	sign=IICReceiveByte();
	//PIN_SDA1_IICOUTPUT();
	IICSendTAck(1);
	IICStop();
  return(sign);
}




#ifdef	EEPROM_K24C02

void TWait(void)
{
	IICTWait();	
}
void I2CSDASetOutput(void)
{
	PIN_SDA1_IICOUTPUT();
}
void I2CSDASetInput(void)
{
	PIN_SDA1_IICFLOAT();
}	
// Start bit of I2C waveform
void I2CStart(void)
{
    I2CSDASetOutput();

    TPSDA_H; 

    TPSCL_H;
	  TWait();
    TPSDA_L; 

	  TWait();

    TPSCL_L; 

}

// Stop bit of I2C waveform
void I2CStop(void)
{
    I2CSDASetOutput();

    TPSCL_H;
    TPSDA_L;

    TPSCL_H;
    TWait();
    TPSDA_H;

    TWait();
}

// Send one byte from host to client
bool I2CSendByte(BYTE ucData)
{
	bool bRet;
	unsigned char i=8; 
	
	I2CSDASetOutput();

	do{
		
		if (ucData&0x80)
			 TPSDA_H;
			 else 
			 	TPSDA_L;
		TPSCL_H;
		TWait();
		ucData<<=1;
		TPSCL_L;
//                if (i>1) //判断是否传递最后一位数据，若是最后一位则TSCL拉低后不用等待
		TWait();
    	}while(--i);
  
	I2CSDASetInput();
	
	bRet=FALSE;
	TPSCL_H;
	TWait();
	if (GPIO_ReadInputDataBit(GPIO_EEPROM, PIN_SDA1))
	bRet=TRUE;
	TPSCL_L;
	TWait();
	return bRet;
}
  

// Receive one byte form client to host
BYTE I2CReciveByte(bool bAck)
{
	BYTE ucRet,i;
	
	I2CSDASetInput();
	
	ucRet = 0;
	i=8;
	do
	{	TPSCL_H;
		TWait();
		ucRet<<=1;
		if(GPIO_ReadInputDataBit(GPIO_EEPROM, PIN_SDA1))
			ucRet++;
		TPSCL_L;
//                if (i>1) //判断是否传递最后一位数据，若是最后一位则TSCL拉低后不用等待
		TWait();
	 }while(--i);

	I2CSDASetOutput();
	
	if (bAck==0)
		TPSDA_L;
	else TPSDA_H;
	
	TPSCL_H;
	TWait();
	TPSCL_L;
	TWait();         
	return ucRet;
}

bool I2CTest(unsigned int cmd)
{
      bool bRet = TRUE;
      I2CStart();
      if(I2CSendByte(cmd & 0xFE) == ACK)
        {
			bRet = TRUE;          
        }
      else
        {
			bRet = FALSE;
        }
      I2CStop();
      return bRet;
}

unsigned char EEPROMBYTE_Write(unsigned int addr,unsigned char regdata)
{
	bool bRet = TRUE;
//	unsigned char i = 0xa0;

//	while(I2CTest(DEVICE_ADDRESS) == FALSE)
//	{
//		DEVICE_ADDRESS++;
//	}

      I2CStart();
      if(I2CSendByte(DEVICE_ADDRESS & 0xFE) == ACK)
        {
          if(I2CSendByte(addr) == ACK)
            {
                  if(I2CSendByte(regdata) == NAK)
                  {
                    bRet = FALSE;
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
      I2CStop();
      return bRet;
}
unsigned char EEPROMBYTE_Read(unsigned int addr)
{
	BYTE sign;
	bool bRet = TRUE;

      I2CStart();
      if(I2CSendByte(DEVICE_ADDRESS & 0xFE) == ACK)
        {
           if(I2CSendByte(addr) == ACK)
           {
               I2CStart();
               if(I2CSendByte(DEVICE_ADDRESS | 0x01) == ACK)
               {

                 sign  = I2CReciveByte(NAK);
               }
               else
               {          bRet = FALSE;           }
           }
           else
           {           bRet = FALSE;           }
         }
      else
        {           bRet = FALSE;           }
      I2CStop();


      return sign;
}

#endif
