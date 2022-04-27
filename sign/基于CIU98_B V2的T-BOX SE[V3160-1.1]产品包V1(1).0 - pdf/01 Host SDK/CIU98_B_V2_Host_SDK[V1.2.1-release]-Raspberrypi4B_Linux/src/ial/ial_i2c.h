
#ifndef _HAL_I2C_H_
#define _HAL_I2C_H_

#include "types.h"
#include "peripheral.h"
#include "util.h"



#define HAL_I2C_PERIPHERAL_STRUCT_POINTER PERIPHERAL_STRUCT_DEFINE(PERIPHERAL_I2C)*

PERIPHERAL_STRUCT_DEFINE(PERIPHERAL_I2C) {
    peripheral periph;
    int32_t (*init)      (HAL_I2C_PERIPHERAL_STRUCT_POINTER periph);	/* 初始化*/
    int32_t (*deinit)	 (HAL_I2C_PERIPHERAL_STRUCT_POINTER periph);	/* 终止化*/    
    int32_t (*delay)(uint32_t us);  //微妙级延时
    int32_t (*timer_start)(util_timer_t *timerval);  //获取计时的起始时间
    int32_t (*timer_differ)(util_timer_t *timerval);  //比较时间差值，检查是否超时
    int32_t (*transmit)  (HAL_I2C_PERIPHERAL_STRUCT_POINTER periph, uint8_t *data, uint32_t  data_len);  /*发送数据 */
    int32_t (*receive)   (HAL_I2C_PERIPHERAL_STRUCT_POINTER periph, uint8_t *data, uint32_t *data_len);         /* 接收数据 */
    int32_t (*control)   (HAL_I2C_PERIPHERAL_STRUCT_POINTER periph, uint32_t ctrlcode, uint8_t *data, uint32_t  data_len);    /* 收发控制命令 */
    void *extra;                                               /* 保存客户自定义的参数 */
};

#define I2C_PERIPHERAL_NAME(id) PERIPHERAL_NAME(PERIPHERAL_I2C, id)

#define I2C_PERIPHERAL_DEFINE_BEGIN(id) PERIPHERAL_DEFINE_BEGIN(PERIPHERAL_I2C, id)

#define I2C_PERIPHERAL_DEFINE_END() PERIPHERAL_DEFINE_END()

#define I2C_PERIPHERAL_REGISTER(id) PERIPHERAL_REGISTER(PERIPHERAL_I2C, id, PERIPHERAL_NAME(PERIPHERAL_I2C, id))

#define I2C_PERIPHERAL_DECLARE(id) PERIPHERAL_DECLARE(PERIPHERAL_I2C, id)  

#endif
