/**@file  port_i2c.h
* @brief  port_i2c interface declearation 
* @author  zhengwd
* @date  2021-04-28
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



#define             PORT_I2C_SE_RST_LOW_DELAY        1000  /*!< RST��λʱ�͵�ƽ����ʱ��T7:  1000us */  
#define		PORT_I2C_SE_RST_HIGH_DELAY       10000  /*!< RST��λ��ߵ�ƽ����ʱ��T6:  10000us */  

//#define		PORT_I2C_SE_PWR_OFF_DEALY       5000      //us	 5ms
//#define		PORT_I2C_SE_PWR_ON_DEALY        5000      //us	 5ms

/**
  * @}
  */






/********************����GPIO ����*******************/
//SE0 RST ����IO
//#define PORT_I2C_SE0_RST_IO      1       //ȫ־H3 ƽ̨
#define PORT_I2C_SE0_RST_IO     18      //��ݮ��ƽ̨

#define PORT_I2C_SE0_RST_LOW()       port_gpio_write(PORT_I2C_SE0_RST_IO, LOW) 
#define PORT_I2C_SE0_RST_HIGH()      port_gpio_write(PORT_I2C_SE0_RST_IO, HIGH)



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
	PORT_I2C_CTRL_RST =   0x00000010,	/*!< RST ��λ����*/  
	PORT_I2C_CTRL_OTHER = 0x00000011       /*!< ��������*/  
} ;

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
	I2C_SetDef *i2c_handle;  /*!< I2Cͨ�Žӿھ�� */  
	int channel;/*!< I2Cͨ�� */    
	int slave; /*!< I2C���豸��ַ*/    
	int speed; /*!< I2C ͨ������ */    
	int timeout;  /*!< I2C���ͳ�ʱʱ�� */  
	int retry;  /*!< I2C�ظ����ʹ���*/   
	uint8_t slave_id;  /*!< ���豸IDֵ */  
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

