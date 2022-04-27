/**@file  port_i2c.h
* @brief  port_i2c interface declearation 
* @author  zhengwd
* @date  2021-4-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

#ifndef _PORT_I2C_H_
#define _PORT_I2C_H_

/***************************************************************************
* Include Header Files
***************************************************************************/
#include <stdint.h>
#include "types.h"
#include "config.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_spi.h"
#include "peripheral.h"
#include "ial_i2c.h"



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


/** @addtogroup PORT_I2C 
  * @{
  */


/* Private constants --------------------------------------------------------*/
/** @defgroup Port_I2c_Private_Constants Port_I2c Private Constants
  * @{
  */

/** @defgroup PORT_I2C_TIME_PARAMS    I2C Communication Time Params
  * @{
  */

//#define             PORT_I2C_HAL_TIMEOUT	 50       //ms
#define             PORT_I2C_HAL_TIMEOUT	 700       //ms
#define             PORT_I2C_SE_RST_LOW_DELAY        1000  /*!< RST复位时低电平持续时间T7:  1000us */  
#define		PORT_I2C_SE_RST_HIGH_DELAY       10000  /*!< RST复位后高电平持续时间T6:  10000us */  

//#define		PORT_I2C_SE_PWR_OFF_DEALY       5000      //us	 5ms
//#define		PORT_I2C_SE_PWR_ON_DEALY        5000      //us	 5ms

/**
  * @}
  */






/********************控制GPIO 定义*******************/
//SE0 RST 控制IO
#define PORT_I2C_SE0_RST_IO_PORT     GPIOA
#define PORT_I2C_SE0_RST_IO_PIN      GPIO_PIN_7
//#define PORT_I2C_SE0_RST_IO_PIN      GPIO_PIN_0
#define PORT_I2C_SE0_RST_IO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()

#define PORT_I2C_SE0_RST_LOW()       HAL_GPIO_WritePin(PORT_I2C_SE0_RST_IO_PORT,PORT_I2C_SE0_RST_IO_PIN,GPIO_PIN_RESET) 
#define PORT_I2C_SE0_RST_HIGH()      HAL_GPIO_WritePin(PORT_I2C_SE0_RST_IO_PORT,PORT_I2C_SE0_RST_IO_PIN,GPIO_PIN_SET)

/********************I2C 接口IO 定义*******************/
//#define PORT_I2C_ADDRESS_2A        (##I2C_SLAVE_ADRR##<<1)                //be carefull!   the addr is 0x2A.
#define PORT_I2C_ADDRESS_2A        (0x2A<<1)                //be carefull!   the addr is 0x2A.


/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 80 MHz */
/* This example use TIMING to 0x00D00E28 to reach 1 MHz speed (Rise time = 120ns, Fall time = 25ns) */

#define I2C_TIMING       0x5033050A//19:6 0x5033020D//1.3:1.2 0x50330708//2:1 0x5033040B//keyi 16:9 0x5033050A//0xC0330A0F 118KHz//0x50330409   400khz //0x50330409   //0x20702991 //0x20D00E1A//0x20D00E28

/* Definition for I2Cx clock resources */
#define I2Cx                            I2C1
#define RCC_PERIPHCLK_I2Cx              RCC_PERIPHCLK_I2C1
#define RCC_I2CxCLKSOURCE_SYSCLK        RCC_I2C1CLKSOURCE_SYSCLK
#define I2Cx_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 

#define I2Cx_FORCE_RESET()              __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET()            __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_8
#define I2Cx_SCL_GPIO_PORT              GPIOB
#define I2Cx_SDA_PIN                    GPIO_PIN_7
#define I2Cx_SDA_GPIO_PORT              GPIOB
#define I2Cx_SCL_SDA_AF                 GPIO_AF4_I2C1


/**
  * @}
  */


/* Exported types ------------------------------------------------------------*/
/** @defgroup Port_I2c_Exported_Types Port_I2c Exported Types
  * @{
  */

/**
  * @brief  Port I2C  Control Structure definition
  */
enum  PORT_I2C_CTRL
{
	PORT_I2C_CTRL_RST =   0x00000010,	/*!< RST 复位控制*/  
	PORT_I2C_CTRL_OTHER = 0x00000011       /*!< 其它控制*/  
} ;


enum PORT_I2C_TIMING_TYPE
{
	#if  1
	//----以下系统时钟为80 MHz时的I2C时钟设置参数-----
	PORT_I2C_TIMING_100K_16_9  = 0x50332C52,	  //SCL低电平:SCL高电平= 16/9, 速率: 100K
	PORT_I2C_TIMING_400K_16_9  = 0x50330812,	  //SCL低电平:SCL高电平= 16/9, 速率: 400K
	PORT_I2C_TIMING_100K_2_1  = 0x50332955,	  //SCL低电平:SCL高电平= 2/1, 速率: 100K
	PORT_I2C_TIMING_400K_2_1  = 0x50330713,	  //SCL低电平:SCL高电平= 2/1, 速率: 400K
	#else
	//----以下系统时钟为48 MHz时的I2C时钟设置参数-----
	PORT_I2C_TIMING_100K_16_9  = 0x50331A30,	  //SCL低电平:SCL高电平= 16/9, 速率: 100K
	PORT_I2C_TIMING_400K_16_9  = 0x5033050A,	  //SCL低电平:SCL高电平= 16/9, 速率: 400K
	PORT_I2C_TIMING_100K_2_1  = 0x50331733,	  //SCL低电平:SCL高电平= 2/1, 速率: 100K
	PORT_I2C_TIMING_400K_2_1  = 0x5033040B,	  //SCL低电平:SCL高电平= 2/1, 速率: 400K
	#endif
};


/**
  * @brief  I2C Set Structure definition
  */
typedef struct __I2C_SetDef
{ 
	int          aiI2cFds[2];  
	int          aiI2cSlave[2];  
} I2C_SetDef;


/**
  * @brief  Port I2C  Communication Params Structure definition
  */
typedef struct   _i2c_comm_param_t{
	I2C_HandleTypeDef *i2c_handle; 
	uint16_t slave_addr;  
	uint8_t slave_id;  
} i2c_comm_param_t, *i2c_comm_param_pointer;


/**
  * @}
  */


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
I2C_PERIPHERAL_DECLARE(I2C_PERIPHERAL_SE0);


/* Exported functions --------------------------------------------------------*/
/** @defgroup Port_I2c_Exported_Functions Port_I2c Exported Functions
  * @{
  */
extern se_error_t port_i2c_periph_delay(uint32_t us);
extern se_error_t port_i2c_periph_timer_start(util_timer_t *timer_start);
extern se_error_t port_i2c_periph_timer_differ(util_timer_t *timer_differ);
extern se_error_t port_i2c_periph_init (HAL_I2C_PERIPHERAL_STRUCT_POINTER periph) ;
extern se_error_t port_i2c_periph_deinit (HAL_I2C_PERIPHERAL_STRUCT_POINTER periph) ;
extern se_error_t port_i2c_periph_transmit(HAL_I2C_PERIPHERAL_STRUCT_POINTER periph, uint8_t *inbuf, uint32_t  inbuf_len) ;
extern se_error_t port_i2c_periph_receive(HAL_I2C_PERIPHERAL_STRUCT_POINTER periph, uint8_t *outbuf, uint32_t *outbuf_len) ;
extern se_error_t port_i2c_periph_control(HAL_I2C_PERIPHERAL_STRUCT_POINTER periph, uint32_t ctrlcode, uint8_t *inbuf, uint32_t  inbuf_len) ;

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


#endif /*_PORT_I2C_H_*/

