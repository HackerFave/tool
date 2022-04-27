/**@file  port_config.c
* @brief  提供系统初始化、延时、计时、上电电控制等底层驱动函数
* @author  zhengwd
* @date  2021-04-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/


/***************************************************************************
* Include Header Files
***************************************************************************/
#include "port_config.h"

/** @addtogroup SE_Driver
  * @{
  */

/** @addtogroup PORT 
  * @brief hardware  portable layer .
  * @{
  */


/** @defgroup PORT_CONFIG PORT_CONFIG
  * @brief hardware portable layer common interface driver.
  * @{
  */


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function  -----------------------------------------------*/


/* Exported functions --------------------------------------------------------*/

/** @defgroup Port_config_Exported_Functions Port_config Exported Functions
  * @{
  */

/**
* @brief 通知系统导出需要控制的 GPIO 引脚
* -# 引脚编号 = 控制引脚的寄存器基数 + 控制引脚寄存器位数，具体 GPIO 需要参考数据手册
* @param [in] pin  引脚编号
* @return 0 :success ;  -1: fail
* @note no
*/
int port_gpio_export(int pin)
{
	char buffer[BUFFER_MAX];
	int len;
	int fd;

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (fd < 0) 
	{
		//fprintf(stderr, "Failed to open export for writing!\n");
		return(-1);
	}  

	len = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, len);

	close(fd);
	return(0);
}

/**
* @brief 通知Linux系统隐藏需要控制的GPIO引脚
* -# 引脚编号 = 控制引脚的寄存器基数 + 控制引脚寄存器位数，具体 GPIO 需要参考数据手册
* @param [in] pin  引脚编号
* @return 0 :success ;  -1: fail
* @note no
*/
int port_gpio_unexport(int pin)
{
	char buffer[BUFFER_MAX];
	int len;
	int fd;

	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (fd < 0) 
	{
		//fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}

	len = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, len);

	close(fd);
	return(0);
}

/**
* @brief 配置GPIO为输入或输出模式。
* -# 引脚编号 = 控制引脚的寄存器基数 + 控制引脚寄存器位数，具体 GPIO 需要参考数据手册
* @param [in] pin  引脚编号
* @param [in] dir  配置引脚输出或输入模式。OUT:输出, IN:输入
* @return 0 :success ;  -1: fail
* @note no
*/
int port_gpio_direction(int pin, int dir)
{
	static const char dir_str[]  = "in\0out";
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (fd < 0) 
	{
		//fprintf(stderr, "failed to open gpio direction for writing!\n");
		return(-1);
	}

	if (write(fd, &dir_str[dir == IN ? 0 : 3], dir == IN ? 2 : 3) < 0) 
	{
		//fprintf(stderr, "failed to set direction!\n");
		return(-1);
	}

	close(fd);
	return(0);
}


/**
* @brief 控制GPIO输出高、低电平
* -# 引脚编号 = 控制引脚的寄存器基数 + 控制引脚寄存器位数，具体 GPIO 需要参考数据手册
* @param [in] pin  引脚编号
* @param [in] value  配置引脚输出高电平或输出低电平。HIGH:高电平, LOW:低电平
* @return 0 :success ;  -1: fail
* @note no
*/
int port_gpio_write(int pin, int value)
{
	static const char s_values_str[] = "01";
	char path[DIRECTION_MAX];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (fd < 0) 
	{
		//fprintf(stderr, "failed to open gpio value for writing!\n");
		return(-1);
	}

	if (write(fd, &s_values_str[value == LOW ? 0 : 1], 1) < 0)
	{
		//fprintf(stderr, "failed to write value!\n");
		return(-1);
	}

	close(fd);
	return(0);
}


/**
* @brief 获得GPIO输入电平状态
* -# 引脚编号 = 控制引脚的寄存器基数 + 控制引脚寄存器位数，具体 GPIO 需要参考数据手册
* @param [in] pin  引脚编号
* @return LOW :低电平 ;  HIGH:高电平;-1: fail
* @note no
*/
int port_gpio_read(int pin)
{
	char path[DIRECTION_MAX];
	char value_str[3];
	int fd;

	snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (fd < 0) 
	{
		//fprintf(stderr, "failed to open gpio value for reading!\n");
		return(-1);
	}

	if (read(fd, value_str, 3) < 0) 
	{
		//fprintf(stderr, "failed to read value!\n");
		return(-1);
	}

	close(fd);
	return(atoi(value_str));
}



/**
* @brief LED、调试 IO 等功能的GPIO 定义
* @param no
* @return no
* @note no
*/
void port_gpio_init(void)
{	
	port_gpio_export(PORT_POWER_CTRL_IO);
	usleep(30 * 1000);
	port_gpio_direction(PORT_POWER_CTRL_IO, OUT);

	PORT_POWER_CTRL_ON();
}



/**
* @brief mcu上电初始化
* @param no
* @return no
* @note no
*/
void port_mcu_init(void)
{
	port_gpio_init();
}

/**
* @brief mcu 去初始化
* @param no
* @return no
* @note no
*/
void port_mcu_deinit(void)
{

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


