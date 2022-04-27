/**@file  port_util.c
* @brief  �ṩϵͳ��ʼ������ʱ����ʱ���ϵ����Ƶȵײ���������
* @author  zhengwd
* @date  2021-4-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/


/***************************************************************************
* Include Header Files
***************************************************************************/
#include <stdarg.h>
#include "port_util.h"
#include "port_config.h"



/********************************************************************************* 
Function:      port_error_handler 
Description:  �����쳣ʱ��ʹ��LED��˸��ʾ
Input:           no
Output:         no  
Return:         no
Others:         no 
*********************************************************************************/
void port_error_handler(void)
{
	while(1)
	{
		/* Error LED is slowly blinking (1 sec. period) */
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_8);
		HAL_Delay(1000);		
	}
}


//_weak void port_printf(const char *format, ...)
/**
* @brief ��ӡ���log��Ϣ
* @param [in] format  ��Ϣ����Ĵ�ӡ��ʽ
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
* @brief ��SE �ϵ�
* @param no
* @return no	
* @note no
*/
void port_power_on(void)
{
	PORT_POWER_CTRL_ON();
}


/**
* @brief ��SE �µ�
* @param no
* @return no	
* @note no
*/
void port_power_off(void)
{
	PORT_POWER_CTRL_OFF();
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


