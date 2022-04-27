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


//brief ����ͨ����������ӿ�

PERIPHERAL_STRUCT_DEFINE(PERIPHERAL_SPI) {
    peripheral periph;  // ����ӿ�
    int32_t (*init)       (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph);  //  ��ʼ��
    int32_t (*deinit)	  (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph);  //  ��ֹ��
    int32_t (*delay)(uint32_t us);  //΢���ʱ
    int32_t (*timer_start)(util_timer_t *timerval);  //��ȡ��ʱ����ʼʱ��
    int32_t (*timer_differ)(util_timer_t *timerval);  //�Ƚ�ʱ���ֵ������Ƿ�ʱ
    int32_t (*chip_select)(HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, bool enable);  //Ƭѡ
    int32_t (*transmit)   (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, uint8_t *data, uint32_t  data_len);   //�������� 
    int32_t (*receive)    (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, uint8_t *data, uint32_t *data_len);   // �������� 
    int32_t (*control)    (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, uint32_t ctrlcode, uint8_t *data, uint32_t  *data_len);  //�շ���������
//spi_comm_param_pointer extra;
        void *extra;       //����ͻ��Զ���Ĳ��� 
};

/** ����SPI ����ʵ�������� */

#define SPI_PERIPHERAL_NAME(id) PERIPHERAL_NAME(PERIPHERAL_SPI, id)

/** ����SPI ���迪ʼ */

#define SPI_PERIPHERAL_DEFINE_BEGIN(id) PERIPHERAL_DEFINE_BEGIN(PERIPHERAL_SPI, id)

/** ����SPI ������� */
#define SPI_PERIPHERAL_DEFINE_END() PERIPHERAL_DEFINE_END()

/** ע��SPI ����*/
#define SPI_PERIPHERAL_REGISTER(id) PERIPHERAL_REGISTER(PERIPHERAL_SPI, id, PERIPHERAL_NAME(PERIPHERAL_SPI, id))

/** ����SPI ���� */
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
