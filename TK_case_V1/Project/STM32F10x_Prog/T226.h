#ifndef	__T226_h__

#define	__T226_h__

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include  "lib.h"
#include "platform_config.h"

#define uchar u8
#define uint  u16
#define Uchar u8
#define Uint  u16

#define LOW 0
#define HIGH 1
#define ACK LOW
#define NAK HIGH

#define mA 1
#define uA 0

#define DEVICE_ADDRESS_M12	0x80
#define DEVICE_ADDRESS_M9	0x8A


#define COMMAND_CONFIG_INDEX	0x00
#define COMMAND_SHUNT_VLOTAGE_SETTING	0x01
#define COMMAND_BUS_VOLTAGE_INDEX	0x02
#define COMMAND_POWER_INDEX	0x03
#define COMMAND_CURRENT_INDEX	0x04		  /////current	   REG
#define COMMAND_CALIBRATION_INDEX	0x05
#define COMMAND_MASK_INDEX	0x06
#define COMMAND_ALERT_LIMIT_INDEX	0x07
#define COMMAND_ID_INDEX	0xFF
/* Includes ------------------------------------------------------------------*/
//******************************************************************************
//				for   use   2014-10-24
//******************************************************************************
typedef   unsigned char   BYTE;
typedef   unsigned short  WORD;
typedef   unsigned long   DWORD;
typedef enum {FALSE = 0, TRUE = !FALSE} bool;

#define T226PIN_SCL  GPIO_Pin_4
#define T226PIN_SDA  GPIO_Pin_7						//
#define GPIO_TP                                   GPIOA
#define RCC_APB2Periph_GPIO_TPIIC  RCC_APB2Periph_GPIOA
//------------------------------------------------------------
#define TPSCL_L GPIO_ResetBits(GPIOC, T226PIN_SCL)  //SCL
#define TPSCL_H GPIO_SetBits  (GPIOC, T226PIN_SCL)
#define TPSDA_L GPIO_ResetBits(GPIO_TP, T226PIN_SDA)  //SDA
#define TPSDA_H GPIO_SetBits  (GPIO_TP, T226PIN_SDA)





#define KEY_PAUSE       GPIOB->IDR&GPIO_Pin_12
#define KEY_UP      GPIOB->IDR&GPIO_Pin_13
#define KEY_DOWN      GPIOB->IDR&GPIO_Pin_14


void T226_GPIO_Config(void);


void EEpromRead_CurrentCalibration(void);
u8 Measure_I_5TimesForProtectIDD(u32 dat1);
u8 Measure_I_5TimesForProtectIDDIO(u32 dat1);






bool T226_I2CSend(BYTE ucDeviceAddr, BYTE ucIndex, BYTE* pucData, unsigned int unDataLength);
bool T226_I2CReceive(BYTE ucDeviceAddr, BYTE ucIndex, BYTE* pucData, unsigned int unDataLength);

unsigned int M9_BUS_VOLTAGE(void);
unsigned int M12_BUS_VOLTAGE(void);
void T226_intion(void);


void USART_SEND_DEC(u16 data);
void PA910SPISendValueofCurrent(char *p,u32 datcf);
/*********************************************************************************************************
*USART_GPIO_Configuration(); 执行之后才可以运行本函数从PA9 PA10以串口方式吐出数据，串口波特率：115200
*本函数运行之后需要执行USART_REMAP_GPIO_Configuration(); 以恢复原本设置
*void PA910SPISendValueofCurrent(char *p,u32 datcf)  使用范例:PA910SPISendValueofCurrent("---C.mA---:",0x03e8);
*参数*p 为字符类指针,datcf为16位变量，执行后串口收到数据为：---C.mA---:0x03e8 <=> 1000
*注意，16进制显示时，32bit数据的高16位会自动舍弃，10进制部分不受影响
*********************************************************************************************************/


u16 Measure_IDD(u8 gears);
u16 Measure_IDDIO(u8 gears);

//u16 Measure_IDD_once(u8 gears);
//u16 Measure_IDDIO_once(u8 gears);
void RESTORE_CALIBRATION(void);
void STORE_CALIBRATION(void);   
void AUTO_CALIBRATION(void);
void GET_PRESENT(void);

void NORML_CAL_TEST(void);
void SLEEP_CAL_TEST(void);
//-----------------------------------------------------------------------
#endif

