
#ifndef _HAL_I2C_H_
#define _HAL_I2C_H_

#include "types.h"
#include "peripheral.h"
#include "util.h"



#define HAL_I2C_PERIPHERAL_STRUCT_POINTER PERIPHERAL_STRUCT_DEFINE(PERIPHERAL_I2C)*

PERIPHERAL_STRUCT_DEFINE(PERIPHERAL_I2C) {
    peripheral periph;
    int32_t (*init)      (HAL_I2C_PERIPHERAL_STRUCT_POINTER periph);	/* ��ʼ��*/
    int32_t (*deinit)	 (HAL_I2C_PERIPHERAL_STRUCT_POINTER periph);	/* ��ֹ��*/    
    int32_t (*delay)(uint32_t us);  //΢���ʱ
    int32_t (*timer_start)(util_timer_t *timerval);  //��ȡ��ʱ����ʼʱ��
    int32_t (*timer_differ)(util_timer_t *timerval);  //�Ƚ�ʱ���ֵ������Ƿ�ʱ
    int32_t (*transmit)  (HAL_I2C_PERIPHERAL_STRUCT_POINTER periph, uint8_t *data, uint32_t  data_len);  /*�������� */
    int32_t (*receive)   (HAL_I2C_PERIPHERAL_STRUCT_POINTER periph, uint8_t *data, uint32_t *data_len);         /* �������� */
    int32_t (*control)   (HAL_I2C_PERIPHERAL_STRUCT_POINTER periph, uint32_t ctrlcode, uint8_t *data, uint32_t  data_len);    /* �շ��������� */
    void *extra;                                               /* ����ͻ��Զ���Ĳ��� */
};

#define I2C_PERIPHERAL_NAME(id) PERIPHERAL_NAME(PERIPHERAL_I2C, id)

#define I2C_PERIPHERAL_DEFINE_BEGIN(id) PERIPHERAL_DEFINE_BEGIN(PERIPHERAL_I2C, id)

#define I2C_PERIPHERAL_DEFINE_END() PERIPHERAL_DEFINE_END()

#define I2C_PERIPHERAL_REGISTER(id) PERIPHERAL_REGISTER(PERIPHERAL_I2C, id, PERIPHERAL_NAME(PERIPHERAL_I2C, id))

#define I2C_PERIPHERAL_DECLARE(id) PERIPHERAL_DECLARE(PERIPHERAL_I2C, id)  

#endif
