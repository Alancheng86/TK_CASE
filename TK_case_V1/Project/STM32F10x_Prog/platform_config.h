/**
  ******************************************************************************
  * @file    USART/HalfDuplex/platform_config.h 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Evaluation board specific configuration file.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */



  

//#define   USE_USART1
//#define   USE_USART2 

/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */
#if !defined (USE_USART1) && !defined (USE_USART2) &&  !defined (USE_USART1_REMAP) 
 
 #define USE_USART1_REMAP
#endif


/* Define the STM32F10x hardware depending on the used evaluation board */

#if defined(USE_USART1_REMAP) 

  #define  GPIOx                    GPIOB
  #define  RCC_APB2Periph_GPIOx     RCC_APB2Periph_GPIOB
  #define  GPIO_TxPin               GPIO_Pin_6
  #define  GPIO_RxPin               GPIO_Pin_7
  #define  RCC_APB2Periph_USARTx    RCC_APB2Periph_USART1
	#define  USARTx                   USART1
	
	#define USARTx_IRQn              USART1_IRQn
	
#elif defined USE_USART1 
  #define  GPIOx                    GPIOA
  #define  RCC_APB2Periph_GPIOx     RCC_APB2Periph_GPIOA
  #define  GPIO_TxPin               GPIO_Pin_9
  #define  GPIO_RxPin               GPIO_Pin_10
  #define  RCC_APB2Periph_USARTx    RCC_APB2Periph_USART1
	#define  USARTx                   USART1
	#define  USARTx_IRQn              USART1_IRQn
	
#elif defined USE_USART2
  #define  GPIOx                    GPIOA
  #define  RCC_APB2Periph_GPIOx     RCC_APB2Periph_GPIOA
  #define  GPIO_TxPin               GPIO_Pin_2
  #define  GPIO_RxPin               GPIO_Pin_3

	#define  USARTx                   USART2
	#define  USARTx_IRQn              USART2_IRQn
#endif  


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
