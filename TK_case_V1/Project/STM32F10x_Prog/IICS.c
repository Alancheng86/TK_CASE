#include <IICS.h>
#include <lcd.h>
#include <math.h>
#define uchar unsigned char
#define uint  unsigned int
#define Uchar unsigned char
#define Uint  unsigned int

void IICTWaitS(void);
void IICStartS(void);
void IICStopS(void);
void IICSendTAckS(unsigned char ack) ;          //确认接收
unsigned char IICTestTAckS(uchar tep) ;
void IICSentByteS(unsigned char bytedata) ;     //传送一个字节数据
unsigned char IICReceiveByteS(void) ;           //接收一个字节数据

void PIN_SDA1_IICFLOATS(void);
void PIN_SDA1_IICOUTPUTS(void);

/*******************************************************************************
* Function Name  : Delay_ms //延时N毫秒
* Description    : 延时
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*
void DelayS (u32 k)  
{
  u32 i,j;
  for (i=0; i<k; i++)
  {
     for (j=0; j<10285; j++);
  }
}
*/
//---------------------------------
void IICTWaitS(void)				  //1=200ns
{
 volatile u32 i,j;
  for (i=0; i<3; i++)
  {
     for (j=0; j<50; j++);  		   //2020
  }
}
//---------------------------------
void EEPROM_GPIO_ConfigS(void)
    {
	    GPIO_InitTypeDef GPIO_InitStructure;
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);//禁止jtag，以空出PB3,PB4,PA15
      //================================================
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_InitStructure.GPIO_Pin = PIN_SDA2 | PIN_SCL2;
      GPIO_Init(GPIOA, &GPIO_InitStructure);
      //----------------------------------------
    }

void PIN_SDA1_IICFLOATS(void)
		{
		GPIO_InitTypeDef QGPIO_InitStructure;
		QGPIO_InitStructure.GPIO_Pin = PIN_SDA2;
		QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &QGPIO_InitStructure);
		} 
void PIN_SDA1_IICOUTPUTS(void)
       {
		 GPIO_InitTypeDef QGPIO_InitStructure;
         QGPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
         QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
         QGPIO_InitStructure.GPIO_Pin =  PIN_SDA2;
         GPIO_Init(GPIOA, &QGPIO_InitStructure);
       }

/********************************************/

void IICStartS(void)
{   
    ESDAS_H; 
	ESCLS_H;
	IICTWaitS();
	ESDAS_L;
        IICTWaitS();  //++++++++
	ESCLS_L;
	IICTWaitS();
}
/********************************************/
void IICStopS(void)
{	
	ESCLS_L;
	ESDAS_L;
	  
    ESCLS_H; IICTWaitS();
    ESDAS_H; 
    IICTWaitS();  //++++++++-----
    ESCLS_L; IICTWaitS();
}
/********************************************/
void IICSendTAckS(uchar ack)           //确认接收
{ 
	
	if (ack)
	     ESDAS_H;
	else 
             ESDAS_L;
        ESCLS_H;
 	IICTWaitS();
	ESCLS_L;
	IICTWaitS();
}
uchar IICTestTAckS(uchar tep)         //响应应答信号  
{
	
	uchar ack=0;uchar flag;
//	PIN_SDA1_IICFLOATS();  
	flag=tep;
	ESCLS_H;
	IICTWaitS();
//	if (GPIO_ReadInputDataBit(GPIOA, PIN_SDA2))
//	    ack=1;
	ESCLS_L;
//    if (flag==0)  //若flag为0，则意味接下来继续处于发送状态，所以还原端口为输出状态
//	PIN_SDA1_IICOUTPUTS();
	return 1;
}

void IICSentByteS(unsigned char bytedata)          //传送一个字节数据
{    unsigned char i=8;
	do{
		//TSDA=bytedata&0x80;
		if (bytedata&0x80)
			 ESDAS_H;
		   else 
			 ESDAS_L;
                IICTWaitS();  //++++++++   
		ESCLS_H;
		IICTWaitS();
		bytedata<<=1;
		ESCLS_L;
           //     if (i>1) //判断是否传递最后一位数据，若是最后一位则TSCL拉低后不用等待
		IICTWaitS();
    	}while(--i);
}
unsigned char IICReceiveByteS(void)             				//接收一个字节数据
{	unsigned char x=0,i=8;
	
  PIN_SDA1_IICFLOATS();
	do
	{	ESCLS_H;
		IICTWaitS();
		x<<=1;
		if(GPIO_ReadInputDataBit(GPIOA, PIN_SDA2))
			x++;
		ESCLS_L;
               // if (i>1) //判断是否传递最后一位数据，若是最后一位则TSCL拉低后不用等待
	        IICTWaitS();
	 }while(--i);
	 
	PIN_SDA1_IICOUTPUTS();
	return(x);
}

unsigned char EEPROMBYTE_WriteS(unsigned int slave,unsigned int addr,unsigned char regdata)
{
	  unsigned char sign,TEP;

		sign=0;	
//		TEP=addr>>8;
//		TEP &=0X01;   //针对24LC08
//    TEP <<=1;

	  TEP =slave;   //CONTROL BYTE:1010 A2 * * W =10100000 ; 其中A2=0
	
		IICStartS();
		IICSentByteS(TEP);     //CONTROL BYTE:1010 A2 a9 a8 W =10100 a9 a8 0
		if(IICTestTAckS(0))
		sign+=0x01;
				
		TEP=addr; 
		IICSentByteS(TEP);
		if(IICTestTAckS(0))
    { sign+=0x04;}
        
		IICSentByteS(regdata);
 		if(IICTestTAckS(0))
		{//IICTWaitS();IICTWaitS();IICTWaitS();DelayS(20);
			sign+=0x08; 
//    while(1);   //判断数据是否正确写入,测试用
		}	
		
		IICStopS();
    return sign;
}
unsigned char EEPROMBYTE_ReadS(unsigned int slave,unsigned int addr)
{
	unsigned char sign=0,TEP;
	
//	TEP=addr>>8;
//	TEP &=0X03;   //针对24LC08
//  TEP <<=1;

	TEP =slave;   //CONTROL BYTE:1010 A2 * * W =10100000 ; 其中A2=0
	
	IICStartS();
	IICSentByteS(TEP);      ////CONTROL BYTE:1010 A2 A1 A0 W =10100000
	if(IICTestTAckS(0))
	   sign+=0x01;
	
	//TEP=addr>>8;
	//IICSentByte(TEP);
	//if(IICTestTAck(0))
	//sign+=0x02;
	TEP=addr; 	
	IICSentByteS(TEP);
	if(IICTestTAckS(0))
	sign+=0x04;
	
	IICStartS();

	TEP = slave+1;   //CONTROL BYTE:1010 A2 * * W =10100000 ; 其中A2=0

//	IICSentByteS(0xA1);      ////CONTROL BYTE:1010 A2 A1 A0 R =10100001  MS此处不需要设定地址，资料没有明确说明
	IICSentByteS(TEP);      ////CONTROL BYTE:1010 A2 A1 A0 R =10100001  MS此处不需要设定地址，资料没有明确说明
	if(IICTestTAckS(1))
	sign+=0x08;
	
	sign=IICReceiveByteS();
	//PIN_SDA1_IICOUTPUTS();
	IICSendTAckS(1);
	IICStopS();
    return(sign);
}
