/**@file  port_spi.h
* @brief  port_spi_ioctl interface declearation 
* @author  zhengwd
* @date  2021-04-28
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
//#include "stm32l4xx_hal.h"
//#include "stm32l4xx_hal_spi.h"
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



#define             PORT_SPI_SE_RST_LOW_DELAY        1000  /*!< RST复位时低电平持续时间T7:  1000us */  
#define		PORT_SPI_SE_RST_HIGH_DELAY       10000  /*!< RST复位后高电平持续时间T6:  10000us */  

//#define		PORT_SPI_SE_PWR_OFF_DEALY       5000      //us	 5ms
//#define		PORT_SPI_SE_PWR_ON_DEALY        10000      //us	 10ms

/**
  * @}
  */


/** @defgroup PORT_SPI_COMM_MODE    SPI Communication  MODE
  * @{
  */

#define		PORT_SPI_COMM_MODE0    0   /*!<Mode 0： CPOL=0, CPHA=0 */ 
#define		PORT_SPI_COMM_MODE1    1   /*!<Mode 1： CPOL=0, CPHA=1 */ 
#define		PORT_SPI_COMM_MODE2    2   /*!<Mode 2： CPOL=1, CPHA=0 */ 
#define		PORT_SPI_COMM_MODE3    3   /*!<Mode 3： CPOL=1, CPHA=1 */ 


/**
  * @}
  */



/********************控制GPIO 定义*******************/
//SE0 RST 控制IO
//#define PORT_SPI_SE0_RST_IO      1        //全志H3 平台
#define PORT_SPI_SE0_RST_IO     436      //树莓派平台


#define PORT_SPI_SE0_RST_LOW()       port_gpio_write(PORT_SPI_SE0_RST_IO, LOW) 
#define PORT_SPI_SE0_RST_HIGH()      port_gpio_write(PORT_SPI_SE0_RST_IO, HIGH)

//SE0 CS 控制IO
//#define PORT_SPI_SE0_CS_IO      203         //全志H3 平台
#define PORT_SPI_SE0_CS_IO      437          //树莓派平台

#define PORT_SPI_SE0_CS_LOW()       port_gpio_write(PORT_SPI_SE0_CS_IO, LOW) 
#define PORT_SPI_SE0_CS_HIGH()      port_gpio_write(PORT_SPI_SE0_CS_IO, HIGH)


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
  * @brief  Port SPI  CS Control Mode Structure definition
  */
enum  PORT_SPI_CS_CTRL_MODE
{
	PORT_SPI_CS_CTRL_MODE_SOFT  = 0x01,  	   /*!< SOFT CTRL */  
	PORT_SPI_CS_CTRL_MODE_HARD  = 0x02,          /*!< HARD CTRL  */  
} ;

/**
  * @brief  SPI Set Structure definition
  */
typedef struct __SPI_SetDef
{
	uint8_t            bSpiBPW;
	uint16_t          sSpiDelay;    
	int          aiSpiSpeeds[2];    
	int          aiSpiFds[2];    
} SPI_SetDef;



/**
  * @brief  Port SPI  Communication Params Structure definition
  */
typedef struct   _spi_comm_param_t{
	SPI_SetDef *spi_handle;  /*!< SPI通信接口句柄 */  
	int channel;/*!< SPI通道 */    
	int mode; /*!< SPI通信模式 */    
	int speed; /*!< SPI通信速率 */    
	uint8_t bpw;  /*!< SPI通信数据位数 */  
	uint8_t slave_id;  /*!< 从设备ID值 */    
	uint8_t cs_mode;  /*!< CS 控制模式*/   
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

extern se_error_t port_spi_periph_delay(uint32_t us);
extern se_error_t port_spi_periph_timer_start(util_timer_t *timer_start);
extern se_error_t port_spi_periph_timer_differ(util_timer_t *timer_differ);
extern se_error_t port_spi_periph_init (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph) ;
extern se_error_t port_spi_periph_deinit (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph) ;
extern se_error_t port_spi_periph_chip_select (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, bool enable) ;
extern se_error_t port_spi_periph_transmit(HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, uint8_t *inbuf, uint32_t  inbuf_len) ;
extern se_error_t port_spi_periph_receive(HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, uint8_t *outbuf, uint32_t *outbuf_len) ;
extern se_error_t port_spi_periph_control(HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, uint32_t ctrlcode, uint8_t *inbuf, uint32_t  *inbuf_len) ;

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


#endif /*_PORT_SPI_H_*/

