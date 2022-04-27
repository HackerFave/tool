/**@file  port_config.h
* @brief  port_config interface declearation 
* @author  zhengwd
* @date  2021-04-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/


#ifndef _PORT_CONFIG_H_
#define _PORT_CONFIG_H_

#include <stdint.h>
#include "config.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"





/**************************************************************************
* Global Type Definition
***************************************************************************/
/* Uncomment the line below to select your USB clock source */
/*#define USE_USB_CLKSOURCE_PLL 1 */
#define USE_USB_CLKSOURCE_CRSHSI48 1
	//#define USB
#if !defined (USE_USB_CLKSOURCE_PLL) && !defined (USE_USB_CLKSOURCE_CRSHSI48)
 #error "Missing USB clock definition"
#endif


//SE  POWER  ¿ØÖÆIO
#define PORT_POWER_CTRL_PORT             GPIOB
#define PORT_POWER_CTRL_IO_PIN           GPIO_PIN_9
#define PORT_POWER_CTRL_IO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
	 
#define PORT_POWER_CTRL_ON()       HAL_GPIO_WritePin(PORT_POWER_CTRL_PORT,PORT_POWER_CTRL_IO_PIN,GPIO_PIN_RESET) 
#define PORT_POWER_CTRL_OFF()      HAL_GPIO_WritePin(PORT_POWER_CTRL_PORT,PORT_POWER_CTRL_IO_PIN,GPIO_PIN_SET)

//SPI  LED  ¿ØÖÆIO
#define PORT_SPI_LED_CTRL_PORT             GPIOC
#define PORT_SPI_LED_CTRL_IO_PIN           GPIO_PIN_7
#define PORT_SPI_LED_CTRL_IO_CLK_ENABLE()  __HAL_RCC_GPIOC_CLK_ENABLE()
	 
#define PORT_SPI_LED_CTRL_ON()       HAL_GPIO_WritePin(PORT_SPI_LED_CTRL_PORT,PORT_SPI_LED_CTRL_IO_PIN,GPIO_PIN_RESET) 
#define PORT_SPI_LED_CTRL_OFF()      HAL_GPIO_WritePin(PORT_SPI_LED_CTRL_PORT,PORT_SPI_LED_CTRL_IO_PIN,GPIO_PIN_SET)

//I2C  LED  ¿ØÖÆIO
#define PORT_I2C_LED_CTRL_PORT             GPIOA
#define PORT_I2C_LED_CTRL_IO_PIN           GPIO_PIN_8
#define PORT_I2C_LED_CTRL_IO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
	 
#define PORT_I2C_LED_CTRL_ON()       HAL_GPIO_WritePin(PORT_I2C_LED_CTRL_PORT,PORT_I2C_LED_CTRL_IO_PIN,GPIO_PIN_RESET) 
#define PORT_I2C_LED_CTRL_OFF()      HAL_GPIO_WritePin(PORT_I2C_LED_CTRL_PORT,PORT_I2C_LED_CTRL_IO_PIN,GPIO_PIN_SET)


//Debug   GPIO
#define PORT_DEBUG_IO_PORT             GPIOA
#define PORT_DEBUG_IO_PIN           GPIO_PIN_6
#define PORT_DEBUG_IO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
	 
#define PORT_DEBUG_IO_HIGH()       HAL_GPIO_WritePin(PORT_DEBUG_IO_PORT,PORT_DEBUG_IO_PIN,GPIO_PIN_SET) 
#define PORT_DEBUG_IO_LOW()      HAL_GPIO_WritePin(PORT_DEBUG_IO_PORT,PORT_DEBUG_IO_PIN,GPIO_PIN_RESET)


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


/** @addtogroup PORT_CONFIG 
  * @{
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup Port_Config_Exported_Functions Port_Config Exported Functions
  * @{
  */

extern void port_gpio_init(void);
extern void port_mcu_init(void);
extern void port_system_clock_config(void);


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


#endif/*_PORT_CONFIG_H*/
