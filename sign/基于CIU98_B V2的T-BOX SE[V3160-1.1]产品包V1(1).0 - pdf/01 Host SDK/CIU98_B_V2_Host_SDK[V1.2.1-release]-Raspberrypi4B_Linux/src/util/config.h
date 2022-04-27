/**@file  config.h
* @brief  Header file of config
* @author  liangww
* @date  2021-5-12
* @version  V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/



#ifndef _CONFIG_H_
#define _CONFIG_H_

//#include "main.h"

/**************************************************************************
* Global Macro Definition
***************************************************************************/



/** @addtogroup SE_Driver
  * @{
  */

/** @addtogroup UTIL
  * @{
  */


  
/* exported constants --------------------------------------------------------*/
/** @defgroup UTIL_Config_Exported_Constants UTIL_Config Exported Constants
  * @{
  */

#define  SPI_PERIPHERAL_SE0               0	/*!< 外设SE ID值*/ 
#define  I2C_PERIPHERAL_SE0               0	/*!< 外设SE ID值*/  

 
#define  _DEBUG                                              /*!< Log打印信息输出使能*/ 
#define  PORT_UART_PRINTF_ENABLE   0    /*!< 使用串口打印输出log时，需要将宏设置为1 */ 

/**
  * @}
  */

/**
  * @}
  */


/**
  * @}
  */


#endif  //_CONFIG_H_

