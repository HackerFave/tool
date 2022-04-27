/**@file  port_stm32l433_util.h
* @brief  port_stm32l433_util interface declearation 
* @author  zhengwd
* @date  2021-04-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/


#ifndef _PORT_UTIL_H_
#define _PORT_UTIL_H_

#include <stdint.h>
#include "config.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"



/**************************************************************************
* Global Type Definition
***************************************************************************/

//SE  POWER  ¿ØÖÆIO
#define PORT_POWER_CTRL_PORT             GPIOB
#define PORT_POWER_CTRL_IO_PIN           GPIO_PIN_9
#define PORT_POWER_CTRL_IO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
	 


#ifdef _DEBUG

#if PORT_UART_PRINTF_ENABLE

#define PORT_USART1_CLK_ENABLE()              __HAL_RCC_LPUART1_CLK_ENABLE()

#define PORT_USART1_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define PORT_USART1_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define PORT_USART1_TX_GPIO_PORT   GPIOA   
#define PORT_USART1_TX_PIN         GPIO_PIN_2

#define PORT_USART1_RX_GPIO_PORT   GPIOA    
#define PORT_USART1_RX_PIN         GPIO_PIN_3

#define PORT_USART1_TX_AF          GPIO_AF8_LPUART1
#define PORT_USART1_RX_AF          GPIO_AF8_LPUART1

extern void port_printf_init(void);

#endif  //#if PORT_UART_PRINTF_ENABLE

extern int fputc(int ch, FILE *f) ;

#else
#define fputc(ch,f)

#endif   //#ifdef _DEBUG


/** @addtogroup SE_Driver
  * @{
  */

/** @addtogroup PORT 
  * @brief hardware  portable layer .
  * @{
  */


/** @addtogroup PORT_UTIL 
  * @{
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup Port_Util_Exported_Functions Port_Util Exported Functions
  * @{
  */


extern void port_error_handler(void);
extern void port_printf(const char *format, ...);
extern void port_power_on(void);
extern void port_power_off(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


#endif/*_PORT_UTIL_H*/
