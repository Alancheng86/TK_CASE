#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include  "lib.h"
#include "platform_config.h"
#include "lcd.h"

//=============================================================================================
extern void Delay(u32 time);


//----------------------------------------
void DelayKEY (u32 k); //按住，画面暂停
void KEYGPIO_Init(void);///*[把PB1/PB10配置成输入模式] */
#define KEY_PAUSE     GPIOB->IDR&GPIO_Pin_12
#define KEY_UP        GPIOB->IDR&GPIO_Pin_13
#define KEY_DOWN      GPIOB->IDR&GPIO_Pin_14

/*******************************************************************************
* Function Name  : READ_SSD2828_onebyte
* Description    : read SSD2825/8 register value
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 READ_SSD2828_onebyte(u8 CMD)  //  读2828寄存器，判断其是否正常
{  u16 mm=0;
	u16 i;
	u8 Parameter=0xfa;
	u8 mm_L=0;
	u8 mm_H=0;

	SSD2828_CSX0(0);
	SSD2828_SDI(0);
	SSD2828_SCL(0);
	SSD2828_SCL(1);//Delay(5);

	for (i=0;i<8;i++)     // 
	{ 
	if( !(CMD & 0x80))
		SSD2828_SDI(0);
	else SSD2828_SDI(1);
		SSD2828_SCL(0);
		SSD2828_SCL(1);	//Delay(5);
	CMD=CMD<<1;
	}

	SSD2828_SDI(0);
	SSD2828_SCL(0);
	SSD2828_SCL(1);	 //Delay(5);

	for (i=0;i<8;i++)     // 
	{ 
	if( !(Parameter & 0x80))
		SSD2828_SDI(0);
	else SSD2828_SDI(1);
		SSD2828_SCL(0);
		SSD2828_SCL(1);//Delay(5);
	Parameter=Parameter<<1;
	}

	for (i=0;i<8;i++)     // 
         { 
                SSD2828_SCL(0);
				SSD2828_SCL(1);
				mm_L<<=1;

				mm_L=mm_L+GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
			//	Delay(5);
         }
	for (i=0;i<8;i++)     // 
         { 
                SSD2828_SCL(0);
				SSD2828_SCL(1);
				mm_H<<=1;
				
				mm_H=mm_H+GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
			//	Delay(5);
         }
	mm = mm_H;
	mm = mm_L + (mm<<8);

	SSD2828_CSX0(1);


	return mm;
}

u16 SSD2825_READ_AnyByte(u8 CMD)   // 连续读读2828寄存器，判断其是否正常
{
	u8 i;
	u8 mm_L=0,mm_H=0;
	u16 mm=0;	

	SSD2828_CSX0(0);
	MDelay(1);
	SSD2828_SDI(0);
	SSD2828_SCL(0);	MDelay(1);
	SSD2828_SCL(1);

	for (i=0;i<8;i++)     // 
	{ 
	if( !(CMD & 0x80))
		SSD2828_SDI(0);
	else SSD2828_SDI(1);
		SSD2828_SCL(0);
		SSD2828_SCL(1);
	CMD=CMD<<1;
	}
		 
	for (i=0;i<8;i++)     // 
         { 
                SSD2828_SCL(0);	  MDelay(1);
				SSD2828_SCL(1);
				mm_L<<=1;

				mm_L=mm_L+GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
				MDelay(1);
         }
	for (i=0;i<8;i++)     // 
         { 
                SSD2828_SCL(0);	  MDelay(1);
				SSD2828_SCL(1);
				mm_H<<=1;
				
				mm_H=mm_H+GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
				MDelay(1);
         }
	mm = mm_H;
	mm = mm_L + (mm<<8);



	return mm;		 	 		 	
}

void STM32TOSSD2828_W_COM(u8 CMD)
{
	u8 i;

	SSD2828_CSX0(0);
	SSD2828_SDI(0);
	SSD2828_SCL(0);
	SSD2828_SCL(1);

	for (i=0;i<8;i++)     // 
	{ 
	if( !(CMD & 0x80))
		SSD2828_SDI(0);
	else SSD2828_SDI(1);
		SSD2828_SCL(0);
		SSD2828_SCL(1);
	CMD=CMD<<1;
	}

	SSD2828_CSX0(1);
}


void STM32TOSSD2828_W_DATA_16BITS(u16 DATA2)  //16 bit
{
	u8 i;
	u8 data=0;

	data = DATA2;

	SSD2828_CSX0(0);
	SSD2828_SDI(1);
	SSD2828_SCL(0);
	SSD2828_SCL(1);

	for (i=0;i<8;i++)     // 
	{ 
	if( !(data & 0x80))
		SSD2828_SDI(0);
	else SSD2828_SDI(1);
		SSD2828_SCL(0);
		SSD2828_SCL(1);
	data=data<<1;
	}

	data = (DATA2>>8);

	SSD2828_SDI(1);
	SSD2828_SCL(0);
	SSD2828_SCL(1);

	for (i=0;i<8;i++)     // 
	{ 
	if( !(data & 0x80))
		SSD2828_SDI(0);
	else SSD2828_SDI(1);
		SSD2828_SCL(0);
		SSD2828_SCL(1);
	data=data<<1;
	}

	SSD2828_CSX0(1);
}

void STM32TOSSD2828_W_DATA_8BITS(u8 DATA)
{
	u8 i;

	SSD2828_CSX0(0);
	SSD2828_SDI(1);
	SSD2828_SCL(0);
	SSD2828_SCL(1);

	for (i=0;i<8;i++)     // 
	{ 
	if( !(DATA & 0x80))
		SSD2828_SDI(0);
	else SSD2828_SDI(1);
		SSD2828_SCL(0);
		SSD2828_SCL(1);
	DATA=DATA<<1;
	}

	SSD2828_CSX0(1);
}

//------------------------------------------------------------------------------
void V3ADJ_SET(char vset,char plus50mv)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	if ((vset<21)|(vset>33)) vset=30; 	
//	vset=vset-21;
//	{
//	   ////////////////////////800mV
//	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_4 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	////////////////////////400mV
//	GPIO_InitStructure.GPIO_Pin =    GPIO_Pin_5 ;
//	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	////////////////////////200mV
//	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_6 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//
//	////////////////////////100mV
//	GPIO_InitStructure.GPIO_Pin =    GPIO_Pin_7 ;
//	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	if( plus50mv)
//		{////////////////////////50mV
//		GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0 ;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOB, &GPIO_InitStructure);	
//			 GPIO_ResetBits(GPIOB,GPIO_Pin_0);																						   
//		}
//	else 
//	   {   GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0 ;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//		GPIO_Init(GPIOB, &GPIO_InitStructure);
//	   }
//	}
//
//	if (vset>=8)
//	  {
//	   	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_4 ;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
//		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
//	    vset -=8;
//	  }
//    if (vset>=4)
//	   
//	   {
//	   	GPIO_InitStructure.GPIO_Pin =    GPIO_Pin_5 ;
//		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
//		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
//	    vset -=4;
//	   }
//	if (vset>=2)
//	   { GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_6 ;
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
//		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
//	     vset -=2;
//	   }
//	 if (vset==1)
//	      {
//		   	GPIO_InitStructure.GPIO_Pin =    GPIO_Pin_7 ;
//			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//			GPIO_Init(GPIOA, &GPIO_InitStructure);
//			GPIO_ResetBits(GPIOA,GPIO_Pin_7);
//		    vset -=1;
//		  } 
//		  else		  
//		  {;}

	
}

/*******************************************************************************
* Function Name  : LCD_CtrlLinesConfig
* Description    : Configures LCD Control lines (FSMC Pins) in alternate function
                   Push-Pull mode.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_CtrlLinesConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
	                                         | RCC_APB2Periph_AFIO, ENABLE); 
	//#define GPIO_Remap_SWJ_JTAGDisable ((u32)0x00300200)  /* JTAG-DP Disabled and SW-DP Enabled */ 禁止JTAG,以空出PB3,PB4
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//禁止jtag，以空出PB3,PB4,PA15
	
	
	/*[把GPIOB 对应端口配置成输出模式] */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_4  | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9  ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO最高速度50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*[把PA 对应端口配置成输出模式] */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 |  GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO最高速度50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	/*[把PC 对应端口配置成输出模式] */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4| GPIO_Pin_7  | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO最高速度50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	/*[把PD 对应端口配置成输出模式] */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO最高速度50MHz
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	/*[把PB  SDO 对应端口配置成输入模式] */
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*[把PA 对应端口配置成输入模式] */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*[把PC 对应端口配置成输入模式] */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
}

void EXTI_Configuration()
{	
	EXTI_InitTypeDef EXTI_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;

	EXTI_GenerateSWInterrupt(EXTI_Line0);
	EXTI_InitStructure.	EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

//----设定按键接口--------------

void KEYGPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//禁止jtag，以空出PB3,PB4,PA15
	
	/*[把KEYGPIO 配置成输入模式] */
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;    // 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	/*[把GPIOB 对应端口配置成输出模式] */
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_10  |GPIO_Pin_11 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//GPIO最高速度50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);


//	/*[把KEYGPIO 配置成输入模式] */
//	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_10 | GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING;    // 
//	GPIO_Init(GPIOB, &GPIO_InitStructure); 

//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//		/*[把KEYGPIO 配置成输入模式] */
//	
		GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_1 ;
		GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_Out_PP;    // 
		GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
}

void DelayKEY (u32 k)
{ 
	volatile u16 m=1;    
	volatile u32 j;

      for (j=0; j<k; j++)
         {  
		 	m=KEY_PAUSE;	
			Delay(2);
            while(m==0)
              {
			  	m=KEY_PAUSE; 
			  	Delay(20);
			  }            
         }	
}

void USART_REMAP_GPIO_Configuration(void)    //PB6\PB7
{
 	USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	 /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 57600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USARTx, &USART_InitStructure);
	
	 /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
  //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	
	USART_Cmd(USARTx, ENABLE);
	
	//-------------------------------------------------------------------------
#if defined(USE_USART1_REMAP)  
  /* Enable the USART1 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
#endif
  
  /* Configure USARTx  Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOx, &GPIO_InitStructure);

  /* Configure USARTx  Rx as input floating */
  GPIO_InitStructure.GPIO_Pin =  GPIO_RxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOx, &GPIO_InitStructure);
}

void USART_GPIO_Configuration(void)
{ USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	 /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 57600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USARTx, &USART_InitStructure);
	
	 /* Enable USART1 Receive and Transmit interrupts */
  USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
  //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	
	USART_Cmd(USARTx, ENABLE);
	
	//-------------------------------------------------------------------------
#if defined(USE_USART1_REMAP)  
  /* Enable the USART1 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
#endif
  
  /* Configure USARTx  Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_TxPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOx, &GPIO_InitStructure);

  /* Configure USARTx  Rx as input floating */
  GPIO_InitStructure.GPIO_Pin =  GPIO_RxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOx, &GPIO_InitStructure);
}



