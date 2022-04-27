/**@file  port_util.c
* @brief  提供系统初始化、延时、计时、上电电控制等底层驱动函数
* @author  zhengwd
* @date  2021-04-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/


/***************************************************************************
* Include Header Files
***************************************************************************/
#include "port_util.h"
#include "port_config.h"



//_weak void port_printf(const char *format, ...)
/**
* @brief 打印输出log信息
* @param [in] format  信息输出的打印格式
* @return no	
* @note no
*/
void port_printf(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}


/**
* @brief 对SE 上电
* @param no
* @return no	
* @note no
*/
void port_power_on(void)
{
	PORT_POWER_CTRL_ON();
}


/**
* @brief 对SE 下电
* @param no
* @return no	
* @note no
*/
void port_power_off(void)
{
	PORT_POWER_CTRL_OFF();
}


/**
* @brief 资源保护锁
* @param no
* @return no	
* @note no
*/
int port_lock(volatile int *lock)
{
//int status = BUSY;
if((*lock) == 0)
{
(*lock)++;
  if((*lock)==1)
   {
   return LOCK_UNBUSY;
   }
}
return LOCK_BUSY;
}


/**
* @brief 资源解锁
* @param no
* @return no	
* @note no
*/

int port_unlock(volatile int *lock)
{
  
(*lock) =0 ;

}



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


