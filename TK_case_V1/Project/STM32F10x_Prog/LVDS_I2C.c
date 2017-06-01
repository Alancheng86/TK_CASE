/* Includes ------------------------------------------------------------------*/
#include <LVDS_I2C.h>



//-===============================================================================================================================
//---------------------------------
void iic_delay(void)				  //1=200ns
{
 volatile u32 i,j;
  for (i=0; i<3; i++)
  {
     for (j=0; j<50; j++);  		   //2020
  }
}
//---------------------------------
void LVDS_IIC_GPIO_Config(void)
    {
	    GPIO_InitTypeDef GPIO_InitStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
      //================================================
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_InitStructure.GPIO_Pin = LVDS_SDA;
      GPIO_Init(GPIO_LVDS_SDA, &GPIO_InitStructure);
      //----------------------------------------
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_InitStructure.GPIO_Pin = LVDS_SCL;
      GPIO_Init(GPIO_LVDS_SCL, &GPIO_InitStructure);	

      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
      GPIO_InitStructure.GPIO_Pin = LVDS_ROM_RLB;
      GPIO_Init(GPIO_LVDS_ROM_RLB, &GPIO_InitStructure);	    			
    }

void LVDS_SDA_IICFLOAT(void)
		{
		GPIO_InitTypeDef QGPIO_InitStructure;
		QGPIO_InitStructure.GPIO_Pin = LVDS_SDA;
		QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIO_LVDS_SDA, &QGPIO_InitStructure);
		} 
void LVDS_SDA_IICOUTPUT(void)
       {
		 GPIO_InitTypeDef QGPIO_InitStructure;
         QGPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
         QGPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
         QGPIO_InitStructure.GPIO_Pin =  LVDS_SDA;
         GPIO_Init(GPIO_LVDS_SDA, &QGPIO_InitStructure);
       }

/********************************************/

void LVDS_IICStart(void)
{   
    LVDS_SDA_H; 
	LVDS_SCL_H;
	iic_delay();
	LVDS_SDA_L;
        iic_delay();  //++++++++
	LVDS_SCL_L;
	iic_delay();
}
/********************************************/
void LVDS_IICStop(void)
{	
//	LVDS_SCL_L;
	LVDS_SDA_L;
	  
    LVDS_SCL_H; iic_delay();
    LVDS_SDA_H; 
    iic_delay();  //++++++++-----
    LVDS_SCL_L; iic_delay();
}
/********************************************/
void LVDS_IICSendTAck(uchar ack)           //è·è??óê?
{ 
	
	if (ack)
	     LVDS_SDA_H;
	else 
             LVDS_SDA_L;
        LVDS_SCL_H;
 	iic_delay();
	LVDS_SCL_L;
	iic_delay();
}
uchar LVDS_IICTestTAck(uchar tep)         //?ìó|ó|′eD?o?  
{
	
	uchar ack=0;uchar flag;
//	LVDS_SDA_IICFLOAT();  
	flag=tep;
	LVDS_SCL_H;
	iic_delay();
//	if (GPIO_ReadInputDataBit(GPIO_LVDS_SDA, PIN_SDA2))
//	    ack=1;
	LVDS_SCL_L;
//    if (flag==0)  //è?flag?a0￡??òòa???ó??à′?ìD?′|óú·￠?í×′ì?￡??ùò??1?-???ú?aê?3?×′ì?
//	LVDS_SDA_IICOUTPUT();
	return 1;
}

void LVDS_IICSentByte(unsigned char bytedata)          //′??íò???×??úêy?Y
{    unsigned char i=8;
	do{
		//TSDA=bytedata&0x80;
		if (bytedata&0x80)
			 LVDS_SDA_H;
		   else 
			 LVDS_SDA_L;
                iic_delay();  //++++++++   
		LVDS_SCL_H;
		iic_delay();
		bytedata<<=1;
		LVDS_SCL_L;
           //     if (i>1) //?D??ê?·?′?μY×?oóò???êy?Y￡?è?ê?×?oóò????òTSCLà-μíoó2?ó?μè′y
		iic_delay();
    	}while(--i);
}
unsigned char LVDS_IICReceiveByte(void)             				//?óê?ò???×??úêy?Y
{	unsigned char x=0,i=8;
	
  LVDS_SDA_IICFLOAT();
	do
	{	LVDS_SCL_H;
		iic_delay();
		x<<=1;
		if(GPIO_ReadInputDataBit(GPIO_LVDS_SDA, LVDS_SDA))
			x++;
		LVDS_SCL_L;
               // if (i>1) //?D??ê?·?′?μY×?oóò???êy?Y￡?è?ê?×?oóò????òTSCLà-μíoó2?ó?μè′y
	        iic_delay();
	 }while(--i);
	 
	LVDS_SDA_IICOUTPUT();
	return(x);
}

unsigned char LVDS_BYTE_Write(unsigned int slave,unsigned int addr,unsigned char regdata)
{
	  unsigned char sign,TEP;

		sign=0;	
//		TEP=addr>>8;
//		TEP &=0X01;   //????24LC08
//    TEP <<=1;

	  TEP =slave;   //CONTROL BYTE:1010 A2 * * W =10100000 ; ???DA2=0
	
		LVDS_IICStart();
		LVDS_IICSentByte(TEP);     //CONTROL BYTE:1010 A2 a9 a8 W =10100 a9 a8 0
		if(LVDS_IICTestTAck(0))
		sign+=0x01;
				
		TEP=addr; 
		LVDS_IICSentByte(TEP);
		if(LVDS_IICTestTAck(0))
    { sign+=0x04;}
        
		LVDS_IICSentByte(regdata);
 		if(LVDS_IICTestTAck(0))
		{//iic_delay();iic_delay();iic_delay();DelayS(20);
			sign+=0x08; 
//    while(1);   //?D??êy?Yê?·??yè·D′è?,2aê?ó?
		}	
		
		LVDS_IICStop();
    return sign;
}
unsigned char LVDS_BYTE_Read(unsigned int slave,unsigned int addr)
{
	unsigned char sign=0,TEP;
	
//	TEP=addr>>8;
//	TEP &=0X03;   //????24LC08
//  TEP <<=1;

	TEP =slave;   //CONTROL BYTE:1010 A2 * * W =10100000 ; ???DA2=0
	
	LVDS_IICStart();
	LVDS_IICSentByte(TEP);      ////CONTROL BYTE:1010 A2 A1 A0 W =10100000
	if(LVDS_IICTestTAck(0))
	   sign+=0x01;
	
	//TEP=addr>>8;
	//IICSentByte(TEP);
	//if(IICTestTAck(0))
	//sign+=0x02;
	TEP=addr; 	
	LVDS_IICSentByte(TEP);
	if(LVDS_IICTestTAck(0))
	sign+=0x04;
	
	LVDS_IICStart();

	TEP = slave+1;   //CONTROL BYTE:1010 A2 * * W =10100000 ; ???DA2=0

//	LVDS_IICSentByte(0xA1);      ////CONTROL BYTE:1010 A2 A1 A0 R =10100001  MS′?′|2?Dèòaéè?¨μ??·￡?×êá???óD?÷è·?μ?÷
	LVDS_IICSentByte(TEP);      ////CONTROL BYTE:1010 A2 A1 A0 R =10100001  MS′?′|2?Dèòaéè?¨μ??·￡?×êá???óD?÷è·?μ?÷
	if(LVDS_IICTestTAck(1))
	sign+=0x08;
	
	sign=LVDS_IICReceiveByte();
	//LVDS_SDA_IICOUTPUT();
	LVDS_IICSendTAck(1);
	LVDS_IICStop();
    return(sign);
}




void STM32TO_LVDSSPI_W_DATA_16BITS(u16 DATA2)  //16 bit
{
	u8 i;

	LVDS_ROM_RLB_L;

	for (i=0;i<16;i++)     // 
	{ 
	if( !(DATA2 & 0x8000))
		LVDS_SDA_L;
	else LVDS_SDA_H;
		LVDS_SCL_L;
		LVDS_SCL_H;
	DATA2=DATA2<<1;
	}

	LVDS_ROM_RLB_H;
}

u16 LVDS_READ_SPI(u8 CMD)  //  SPI寄存器，判断其是否正常
{  
    u16 mm=0;
	u16 i;
	u8 Parameter=0x00;
	u8 mm_L=0;
	u8 mm_H=0;

	LVDS_CSX0(0);
	LVDS_SDI(1);
	LVDS_SCK(0);
	LVDS_SCK(1);//Delay(5);
    
    Parameter = CMD&0x1F + 0x80 ;
    
	for (i=0;i<7;i++)     // 
	{ 
	if( !(Parameter & 0x40))
		LVDS_SDI(0);
	else LVDS_SDI(1);
		LVDS_SCK(0);
		LVDS_SCK(1);	//Delay(5);
	Parameter=Parameter<<1;
	}


	LVDS_SDA_IICFLOAT();
    Delay(5);
    
    for (i=0;i<8;i++)     // 
         { 
                LVDS_SCK(0);Delay(5);
				LVDS_SCK(1);
				mm_H<<=1;
				Delay(5);
				mm_H=mm_H+GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
				Delay(5);
         }
	mm = mm_H;


	LVDS_CSX0(1);
    LVDS_SDA_IICOUTPUT();

	return mm;
}

void LVDS_SPI(char RorW,char slave_adress,int data)
{
    u16 data_sum=0;
    
    data_sum = (RorW << 15) + (0x00 << 13) + ((slave_adress&0x1F) << 8) + (0x00FF&data) ;
    
    STM32TO_LVDSSPI_W_DATA_16BITS(data_sum);
}

void LVDS_WRITE_SPI(char slave_adress,int data)
{
    u16 data_sum=0;
    
    data_sum = (WRITE << 15) + (0x00 << 13) + ((slave_adress&0x1F) << 8) + (0x00FF&data) ;
    
    STM32TO_LVDSSPI_W_DATA_16BITS(data_sum);
}

//u8 LVDS_READ_SPI(char slave_adress,int data)
//{
//    u16 data_sum=0;
//    
//    data_sum = (READ << 15) + (0x00 << 13) + ((slave_adress&0x1F) << 8) + (0x00FF&data) ;
//    
//    STM32TO_LVDSSPI_W_DATA_16BITS(data_sum);
//}


