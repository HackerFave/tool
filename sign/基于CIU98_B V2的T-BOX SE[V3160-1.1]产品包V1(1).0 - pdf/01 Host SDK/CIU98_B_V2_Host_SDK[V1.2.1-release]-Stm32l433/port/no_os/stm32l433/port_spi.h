/**@file  port_spi.h
* @brief  port_spi interface declearation 
* @author  zhengwd
* @date  2021-4-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

#ifndef _PORT_SPI_H_
#define _PORT_SPI_H_

/***************************************************************************
* Include Header Files
***************************************************************************/
#include <stdint.h>
#include "types.h"
#include "config.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_spi.h"
#include "peripheral.h"
#include "ial_spi.h"



/**************************************************************************
* Global Macro Definition
***************************************************************************/

/** @addtogroup SE_Driver
  * @{
  */

/** @addtogroup PORT 
  * @brief hardware  portable layer .
  * @{
  */


/** @addtogroup PORT_SPI 
  * @{
  */


/* Private constants --------------------------------------------------------*/
/** @defgroup Port_Spi_Private_Constants Port_Spi Private Constants
  * @{
  */

/** @defgroup PORT_SPI_TIME_PARAMS    SPI Communication Time Params
  * @{
  */

#define             PORT_SPI_HAL_TIMEOUT	         100      /*!< STM32L433 HAL库的数据发送接收超时时间: 100ms */   
#define             PORT_SPI_SE_RST_LOW_DELAY        1000 /*!< RST复位时低电平持续时间T7:  1000us */  
#define		        PORT_SPI_SE_RST_HIGH_DELAY       10000  /*!< RST复位后高电平持续时间T6:  10000us */  

//#define		PORT_SPI_SE_PWR_OFF_DEALY       5000      //us	 5ms
//#define		PORT_SPI_SE_PWR_ON_DEALY        5000      //us	 5ms

/**
  * @}
  */


/** @defgroup PORT_SPI_NSS_MODE    SPI Communication NSS MODE
  * @{
  */

#define		PORT_SPI_NSS_SOFT               0x00  /*!< 软模式 */  
#define		PORT_SPI_NSS_HARD_OUTPUT        0x01 /*!< 硬输出模式 */  

//#define		PORT_SPI_NSS_MODE    PORT_SPI_NSS_SOFT
#define		PORT_SPI_NSS_MODE    PORT_SPI_NSS_HARD_OUTPUT

/**
  * @}
  */


/********************控制GPIO 定义*******************/



//SE0 RST 控制IO
#define PORT_SPI_SE0_RST_IO_PORT     GPIOA
#define PORT_SPI_SE0_RST_IO_PIN      GPIO_PIN_7
#define PORT_SPI_SE0_RST_IO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()

#define PORT_SPI_SE0_RST_LOW()       HAL_GPIO_WritePin(PORT_SPI_SE0_RST_IO_PORT,PORT_SPI_SE0_RST_IO_PIN,GPIO_PIN_RESET) 
#define PORT_SPI_SE0_RST_HIGH()      HAL_GPIO_WritePin(PORT_SPI_SE0_RST_IO_PORT,PORT_SPI_SE0_RST_IO_PIN,GPIO_PIN_SET)


/********************SPI2 接口IO 定义*******************/
#define SPI2_CLK_ENABLE()                __HAL_RCC_SPI2_CLK_ENABLE()
#define SPI2_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI2_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI2_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI2_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#define SPI2_FORCE_RESET()               __HAL_RCC_SPI2_FORCE_RESET()
#define SPI2_RELEASE_RESET()             __HAL_RCC_SPI2_RELEASE_RESET()

/* Definition for SPIx Pins */
#define SPI2_CS_PIN                     GPIO_PIN_12
#define SPI2_CS_GPIO_PORT               GPIOB
#define SPI2_CS_AF                      GPIO_AF5_SPI2
#define SPI2_SCK_PIN                    GPIO_PIN_13
#define SPI2_SCK_GPIO_PORT              GPIOB
#define SPI2_SCK_AF                     GPIO_AF5_SPI2
#define SPI2_MISO_PIN                   GPIO_PIN_14
#define SPI2_MISO_GPIO_PORT             GPIOB
#define SPI2_MISO_AF                    GPIO_AF5_SPI2
#define SPI2_MOSI_PIN                   GPIO_PIN_15
#define SPI2_MOSI_GPIO_PORT             GPIOB
#define SPI2_MOSI_AF                    GPIO_AF5_SPI2	

#define PORT_SPI_SE0_CS_ON()    HAL_GPIO_WritePin(SPI2_CS_GPIO_PORT,SPI2_CS_PIN,GPIO_PIN_RESET) 
#define PORT_SPI_SE0_CS_OFF()   HAL_GPIO_WritePin(SPI2_CS_GPIO_PORT,SPI2_CS_PIN,GPIO_PIN_SET)

/**
  * @}
  */


/* Exported types ------------------------------------------------------------*/
/** @defgroup Port_Spi_Exported_Types Port_Spi Exported Types
  * @{
  */

/**
  * @brief  Port SPI  Control Structure definition
  */
enum  PORT_SPI_CTRL
{
	PORT_SPI_CTRL_RST =   0x00000010,	/*!< RST 复位控制*/
	PORT_SPI_CTRL_OTHER = 0x00000011       /*!< 其它控制*/  
} ;

/**
  * @brief  Port SPI  Communication Params Structure definition
  */
typedef struct   _spi_comm_param_t{
	SPI_HandleTypeDef *spi_handle;  /*!< SPI通信接口句柄 */  
	uint8_t slave_id;  /*!< 从设备ID值 */  
	uint8_t nss_mode; /*!< NSS模式 */     
} spi_comm_param_t, *spi_comm_param_pointer;

/**
  * @}
  */


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
SPI_PERIPHERAL_DECLARE(SPI_PERIPHERAL_SE0);


/* Exported functions --------------------------------------------------------*/
/** @defgroup Port_Spi_Exported_Functions Port_Spi Exported Functions
  * @{
  */

extern se_error_t port_spi_periph_init (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph) ;
extern se_error_t port_spi_periph_deinit (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph) ;
extern se_error_t port_spi_periph_chip_select (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, bool enable) ;
extern se_error_t port_spi_periph_transmit(HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, uint8_t *inbuf, uint32_t  inbuf_len) ;
extern se_error_t port_spi_periph_receive(HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, uint8_t *outbuf, uint32_t *outbuf_len) ;
extern se_error_t port_spi_periph_control(HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, uint32_t ctrlcode, uint8_t *inbuf, uint32_t  *inbuf_len) ;
extern se_error_t port_spi_periph_delay(uint32_t us);
extern se_error_t port_spi_periph_timer_start(util_timer_t *timer_start);
extern se_error_t port_spi_periph_timer_differ(util_timer_t *timer_differ);



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


#endif /*_PORT_SPI_H*/

