/**@file  hal_spi.h
* @brief  port hal spi interface declearation    
* @author  zhengwd
* @date  2021-04-24
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

#ifndef _HAL_SPI_H_
#define _HAL_SPI_H_

#include "types.h"
#include "peripheral.h"
#include "util.h"

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


/** @addtogroup HAL_SPI 
  * @{
  */



/* peripheral define --------------------------------------------------------*/
/** @defgroup peripheral_spi_comm_declearation peripheral spi communication declearation
  * @{
  */


#define HAL_SPI_PERIPHERAL_STRUCT_POINTER PERIPHERAL_STRUCT_DEFINE(PERIPHERAL_SPI)*


//brief 外设通信驱动抽象接口

PERIPHERAL_STRUCT_DEFINE(PERIPHERAL_SPI) {
    peripheral periph;  // 外设接口
    int32_t (*init)       (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph);  //  初始化
    int32_t (*deinit)	  (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph);  //  终止化
    int32_t (*delay)(uint32_t us);  //微妙级延时
    int32_t (*timer_start)(util_timer_t *timerval);  //获取计时的起始时间
    int32_t (*timer_differ)(util_timer_t *timerval);  //比较时间差值，检查是否超时
    int32_t (*chip_select)(HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, bool enable);  //片选
    int32_t (*transmit)   (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, uint8_t *data, uint32_t  data_len);   //发送数据 
    int32_t (*receive)    (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, uint8_t *data, uint32_t *data_len);   // 接收数据 
    int32_t (*control)    (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, uint32_t ctrlcode, uint8_t *data, uint32_t  *data_len);  //收发控制命令
//spi_comm_param_pointer extra;
        void *extra;       //保存客户自定义的参数 
};

/** 定义SPI 外设实例变量名 */

#define SPI_PERIPHERAL_NAME(id) PERIPHERAL_NAME(PERIPHERAL_SPI, id)

/** 定义SPI 外设开始 */

#define SPI_PERIPHERAL_DEFINE_BEGIN(id) PERIPHERAL_DEFINE_BEGIN(PERIPHERAL_SPI, id)

/** 定义SPI 外设结束 */
#define SPI_PERIPHERAL_DEFINE_END() PERIPHERAL_DEFINE_END()

/** 注册SPI 外设*/
#define SPI_PERIPHERAL_REGISTER(id) PERIPHERAL_REGISTER(PERIPHERAL_SPI, id, PERIPHERAL_NAME(PERIPHERAL_SPI, id))

/** 声明SPI 外设 */
#define SPI_PERIPHERAL_DECLARE(id) PERIPHERAL_DECLARE(PERIPHERAL_SPI, id)  

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

#endif
