
#include <IIC.h>
#include <lcd.h>
#include <math.h>
#define uchar unsigned char
#define Uchar unsigned char
#define Uint  unsigned int

void IICTWait(void);
void IICStart(void);
void IICStop(void);
void IICSendTAck(unsigned char ack) ;          //ȷ�Ͻ���
unsigned char IICTestTAck(uchar tep) ;
void IICSentByte(unsigned char bytedata) ;     //����һ���ֽ�����
unsigned char IICReceiveByte(void) ;           //����һ���ֽ�����

void PIN_SDA1_IICFLOAT(void);
void PIN_SDA1_IICOUTPUT(void);

void Delay (u32 j);
/*******************************************************************************
* Function Name  : Delay_ms //��ʱN����
* Description    : ��ʱ
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

//---------------------------------
void IICTWait(void)
{
 volatile u32 i,j;
  for (i=0; i<3; i++)
  {
     for (j=0; j<100; j++);  			 /////2020			 2014-02-25
  }
}
//---------------------------------
void EEPROM_GPIO_Config(void)
    {GPIO_InitTypeDef GPIO_InitStructure;
     //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_EEPROM | RCC_APB2Periph_AFIO, ENABLE);
      //================================================
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_InitStructure.GPIO_Pin = PIN_SCL1 | PIN_SDA1;
      GPIO_Init(GPIO_EEPROM, &GPIO_InitStructure);
      //----------------------------------------
    }
GPIO_InitTypeDef QGPIO_InitStructure;
void PIN_SDA1_IICFLOAT(void)
            {QGPIO_InitStructure.GPIO_Pin = PIN_SDA1;
             QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
             GPIO_Init(GPIO_EEPROM, &QGPIO_InitStructure);
            } 
void PIN_SDA1_IICOUTPUT(void)
       {
         QGPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
         QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
         QGPIO_InitStructure.GPIO_Pin =  PIN_SDA1;
         GPIO_Init(GPIO_EEPROM, &QGPIO_InitStructure);
       }

/********************************************/

void IICStart(void)
{ ESDA_H; 
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
void IICSendTAck(uchar ack)           //ȷ�Ͻ���
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
uchar IICTestTAck(uchar tep)         //��ӦӦ���ź�  
{uchar ack=0;uchar flag;
  PIN_SDA1_IICFLOAT();  
  flag=tep;
  ESCL_H;
	IICTWait();
	if (GPIO_ReadInputDataBit(GPIO_EEPROM, PIN_SDA1))
	    ack=1;
	ESCL_L;
        if (flag==0)  //��flagΪ0������ζ�������������ڷ���״̬�����Ի�ԭ�˿�Ϊ���״̬
	PIN_SDA1_IICOUTPUT();
	return ack;
}

void IICSentByte(unsigned char bytedata)          //����һ���ֽ�����
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
           //     if (i>1) //�ж��Ƿ񴫵����һλ���ݣ��������һλ��TSCL���ͺ��õȴ�
		IICTWait();
    	}while(--i);
}
unsigned char IICReceiveByte(void)             				//����һ���ֽ�����
{	unsigned char x=0,i=8;
	
  PIN_SDA1_IICFLOAT();
	do
	{	ESCL_H;
		IICTWait();
		x<<=1;
		if(GPIO_ReadInputDataBit(GPIO_EEPROM, PIN_SDA1))
			x++;
		ESCL_L;
               // if (i>1) //�ж��Ƿ񴫵����һλ���ݣ��������һλ��TSCL���ͺ��õȴ�
	        IICTWait();
	 }while(--i);
	 
	PIN_SDA1_IICOUTPUT();
	return(x);
}

//=========================================================================
unsigned char EEPROMBYTE_Write(unsigned int addr,unsigned char regdata)
{unsigned char sign,TEP;
       Delay(12);
		sign=0;
		
		TEP=addr>>8;
		TEP &=0X01;   //���24LC08
                TEP <<=1;
		TEP |=0XA0;   //CONTROL BYTE:1010 A2 * * W =10100000 ; ����A2=0
		
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
                    while(1);   //�ж������Ƿ���ȷд��,������
                  }	
		
		IICStop();
    return sign;
}
unsigned char EEPROMBYTE_Read(unsigned int addr)
{
	unsigned char sign=0,TEP;
	
	TEP=addr>>8;
		TEP &=0X03;   //���24LC08
                TEP <<=1;
		TEP |=0XA0;   //CONTROL BYTE:1010 A2 * * W =10100000 ; ����A2=0
		
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
	IICSentByte(0xA1);      ////CONTROL BYTE:1010 A2 A1 A0 R =10100001  MS�˴�����Ҫ�趨��ַ������û����ȷ˵��
	if(IICTestTAck(1))
	           sign+=0x08;
	
	sign=IICReceiveByte();
	//PIN_SDA1_IICOUTPUT();
	IICSendTAck(1);
	IICStop();
    return(sign);
}