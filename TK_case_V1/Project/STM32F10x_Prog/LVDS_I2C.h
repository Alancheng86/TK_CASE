#ifndef	__LVDS_I2C_h__

#define	__LVDS_I2C_h__

#include "stm32f10x.h"
#include "T226.h"     	//INA226电流检测，驱动文件


#define READ    1
#define WRITE   0


#define ST5821_Slave_Address 0x70		/////////----0x19			   /////
///////////// for M6 use
#define LVDS_SCL  GPIO_Pin_10
#define LVDS_SDA  GPIO_Pin_11						 
#define GPIO_LVDS_SDA                                   GPIOB
#define GPIO_LVDS_SCL                                   GPIOB

#define LVDS_SCL_L GPIO_ResetBits(GPIO_LVDS_SCL, LVDS_SCL)  //SCL
#define LVDS_SCL_H GPIO_SetBits  (GPIO_LVDS_SCL, LVDS_SCL)
#define LVDS_SDA_L GPIO_ResetBits(GPIO_LVDS_SDA, LVDS_SDA)  //SDA
#define LVDS_SDA_H GPIO_SetBits  (GPIO_LVDS_SDA, LVDS_SDA)

//===临时定义RLB管脚，不确定之后的型号会不会用到
#define LVDS_ROM_RLB   GPIO_Pin_5
#define GPIO_LVDS_ROM_RLB                               GPIOC
#define LVDS_ROM_RLB_L GPIO_ResetBits(GPIO_LVDS_ROM_RLB, LVDS_ROM_RLB)  //ROM_RLB   
#define LVDS_ROM_RLB_H GPIO_SetBits  (GPIO_LVDS_ROM_RLB, LVDS_ROM_RLB)

//-----------------------------------VDD/VDDIO/VOTP     Control  -------------------	
#define	LVDS_LCDPin_CSX0	(1<<5)   	      
#define	LVDS_LCDPin_SCL	(1<<10)   //时钟端口	       
#define	LVDS_LCDPin_SDI	(1<<11)    //数据端口

#define LVDS_CSX0(x)   GPIOC->ODR=(GPIOC->ODR&~LVDS_LCDPin_CSX0)|(x ? LVDS_LCDPin_CSX0:0)													    
#define LVDS_SCK(x)    GPIOB->ODR=(GPIOB->ODR&~LVDS_LCDPin_SCL) |(x ? LVDS_LCDPin_SCL:0)
#define LVDS_SDI(x)    GPIOB->ODR=(GPIOB->ODR&~LVDS_LCDPin_SDI)|(x ? LVDS_LCDPin_SDI:0)
//-----------------------------------------------------------------------
//----------------------------IF : IIC ------------------------
//-----------------------------------------------------------------------
void LVDS_IIC_GPIO_Config(void);
void LVDS_SDA_IICFLOAT(void);
void LVDS_SDA_IICOUTPUT(void);
void iic_delay(void);
void LVDS_IICStart(void);
void LVDS_IICStop(void);
void LVDS_IICSendTAck(uchar ack);
uchar LVDS_IICTestTAck(uchar tep);	
void LVDS_IICSentByte(unsigned char bytedata)  ;
unsigned char LVDS_IICReceiveByte(void) ;
unsigned char LVDS_BYTE_Write(unsigned int slave,unsigned int addr,unsigned char regdata);
unsigned char LVDS_BYTE_Read(unsigned int slave,unsigned int addr);
//-----------------------------------------------------------------------
//----------------------------IF : spi ------------------------
//-----------------------------------------------------------------------


u16 LVDS_READ_SPI(u8 CMD);
void STM32TO_LVDSSPI_W_DATA_16BITS(u16 DATA2);
void LVDS_SPI(char RorW,char slave_adress,int data);
void LVDS_WRITE_SPI(char slave_adress,int data);
//-----------------------------------------------------------------------
#endif


