/**@file  port_spi.c
* @brief  SPI 接口硬件适配实现驱动程序
* @author  zhengwd
* @date  2021-4-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/


/***************************************************************************
* Include Header Files
***************************************************************************/
#include "port_spi.h"
#include "error.h"
#include "util.h"
#include "log.h"
//#include "port_config.h"



/**************************************************************************
* Global Variable Declaration
***************************************************************************/
SPI_HandleTypeDef spi_comm_handle_se0={0};
spi_comm_param_t spi_comm_parm_se0={&spi_comm_handle_se0, SPI_PERIPHERAL_SE0,PORT_SPI_NSS_MODE};
static uint8_t g_spi_device_init[MAX_PERIPHERAL_DEVICE]={FALSE};

/** @addtogroup SE_Driver
  * @{
  */

/** @addtogroup PORT 
  * @brief hardware  portable layer .
  * @{
  */


/** @defgroup PORT_SPI PORT_SPI
  * @brief hardware portable layer spi interface driver.
  * @{
  */



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function  -----------------------------------------------*/
/** @defgroup Port_Spi_Private_Functions Port_Spi Private Functions
 * @{
 */


/**
* @brief This function configures the spi hardware resources
* @param [in] hspi  spi handle pointer
* @return no	
* @note no
*/
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};

	if(hspi->Instance == SPI2)
	{     
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		SPI2_CS_GPIO_CLK_ENABLE();
		SPI2_SCK_GPIO_CLK_ENABLE();
		SPI2_MISO_GPIO_CLK_ENABLE();
		SPI2_MOSI_GPIO_CLK_ENABLE();
		/* Enable SPI clock */
		SPI2_CLK_ENABLE(); 

		/*##-2- Configure peripheral GPIO ##########################################*/  
		/* SPI SCK GPIO pin configuration  */
		GPIO_InitStruct.Pin       = SPI2_SCK_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		//GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
		GPIO_InitStruct.Pull		= GPIO_PULLUP;	//debug  若不能进入powerdown时，使用control的io控制spi引脚拉高

		GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = SPI2_SCK_AF;
		HAL_GPIO_Init(SPI2_SCK_GPIO_PORT, &GPIO_InitStruct);

		/* SPI MISO GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI2_MISO_PIN;
		GPIO_InitStruct.Alternate = SPI2_MISO_AF;
		HAL_GPIO_Init(SPI2_MISO_GPIO_PORT, &GPIO_InitStruct);
		//GPIO_InitStruct.Pull		= GPIO_PULLUP;	//debug  若不能进入powerdown时，使用control的io控制spi引脚拉高

		/* SPI MOSI GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI2_MOSI_PIN;
		GPIO_InitStruct.Alternate = SPI2_MOSI_AF;
		HAL_GPIO_Init(SPI2_MOSI_GPIO_PORT, &GPIO_InitStruct);

		/* SPI CS GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI2_CS_PIN;
		GPIO_InitStruct.Alternate = SPI2_CS_AF;
		HAL_GPIO_Init(SPI2_CS_GPIO_PORT, &GPIO_InitStruct);
	}
}


/**
* @brief This function frees the spi hardware resources
* @param [in] hspi	spi handle pointer
* @return no	
* @note no
*/
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
	if(hspi->Instance == SPI2)
	{   
		/*##-1- Reset peripherals ##################################################*/
		SPI2_FORCE_RESET();
		SPI2_RELEASE_RESET();

		/*##-2- Disable peripherals and GPIO Clocks ################################*/
		/* Configure SPI SCK as alternate function  */
		HAL_GPIO_DeInit(SPI2_SCK_GPIO_PORT, SPI2_SCK_PIN);
		/* Configure SPI MISO as alternate function  */
		HAL_GPIO_DeInit(SPI2_MISO_GPIO_PORT, SPI2_MISO_PIN);
		/* Configure SPI MOSI as alternate function  */
		HAL_GPIO_DeInit(SPI2_MOSI_GPIO_PORT, SPI2_MOSI_PIN);
		/* Configure SPI CS as alternate function  */	
		HAL_GPIO_DeInit(SPI2_CS_GPIO_PORT, SPI2_CS_PIN);
	}
}


/**
* @brief 对外设SE 的gpio进行初始化
* @return no	
* @note no
*/
void port_spi_gpio_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};

	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; 
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	//---SE RST 控制IO----
	GPIO_InitStruct.Pin = PORT_SPI_SE0_RST_IO_PIN;
	PORT_SPI_SE0_RST_IO_CLK_ENABLE();
	HAL_GPIO_Init(PORT_SPI_SE0_RST_IO_PORT, &GPIO_InitStruct);
}


void port_spi_se0_cs_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct = {0};

	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; 
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	//---SE CS 控制IO----
	GPIO_InitStruct.Pin = SPI2_CS_PIN;
	PORT_SPI_SE0_CS_OFF();
	HAL_GPIO_Init(SPI2_CS_GPIO_PORT, &GPIO_InitStruct);
}


/**
* @brief SPI接口初始化
* @param [in] hspi  spi 接口句柄
* @param [in] spi_instance  接口实例
* @param [in] nss_mode  NSS模式
* @return 函数操作状态码
* @note no
*/
se_error_t port_spi_init(void *handle, void *spi_instance, uint8_t nss_mode)
{
	SPI_HandleTypeDef *HEDSEspiHandle = (SPI_HandleTypeDef *)handle;

	/* Set the SPI parameters */
	(*HEDSEspiHandle).Instance               = (SPI_TypeDef *)spi_instance;
	//(*HEDSEspiHandle).Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;   //4
	(*HEDSEspiHandle).Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;   
	(*HEDSEspiHandle).Init.Direction         = SPI_DIRECTION_2LINES;
	(*HEDSEspiHandle).Init.CLKPhase          = SPI_PHASE_1EDGE;
	(*HEDSEspiHandle).Init.CLKPolarity       = SPI_POLARITY_LOW;
	(*HEDSEspiHandle).Init.DataSize          = SPI_DATASIZE_8BIT;
	(*HEDSEspiHandle).Init.FirstBit          = SPI_FIRSTBIT_MSB;
	(*HEDSEspiHandle).Init.TIMode            = SPI_TIMODE_DISABLE;
	(*HEDSEspiHandle).Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	(*HEDSEspiHandle).Init.CRCPolynomial     = 7;
	(*HEDSEspiHandle).Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
	(*HEDSEspiHandle).Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
	(*HEDSEspiHandle).Init.Mode = SPI_MODE_MASTER;

	if(nss_mode == PORT_SPI_NSS_SOFT)
	{
		(*HEDSEspiHandle).Init.NSS               = SPI_NSS_SOFT;
		
	}
	else if(nss_mode == PORT_SPI_NSS_HARD_OUTPUT)
	{
		(*HEDSEspiHandle).Init.NSS               = SPI_NSS_HARD_OUTPUT;
	}

	if(HAL_SPI_Init(HEDSEspiHandle) != HAL_OK)
	{
		/* Initialization Error */
		return SE_ERR_INIT;
	}
		
	return SE_SUCCESS;
}


/**
* @brief SPI接口移除，获取设备句柄清除
* @param [in] hspi	spi 接口句柄
* @return 函数操作状态码
* @note no
*/
se_error_t  port_spi_deinit (void * handle)
{
    if(HAL_SPI_DeInit((SPI_HandleTypeDef *)handle) != HAL_OK)
    {
        /* Initialization Error */
         return SE_ERR_INIT;
    }

    return SE_SUCCESS;
}


/**
  * @}
  */





/* Exported functions --------------------------------------------------------*/

/** @defgroup Port_Spi_Exported_Functions Port_Spi Exported Functions
  * @{
  */



/**
* @brief 从设备periph的通信初始化
* -# 对外设SE的gpio进行初始化
* -# 初始化SPI接口
* -# 设置RST控制IO为高电平
* @param [in] periph  设备句柄
* @return 函数操作状态码	
* @note no
* @see 函数  port_spi_gpio_init  port_spi_init
*/
se_error_t port_spi_periph_init (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph) 
{	  
	se_error_t ret_code = SE_SUCCESS;
	spi_comm_param_pointer p_comm_param = (spi_comm_param_pointer)periph->extra;

	do
	{
		if(periph == NULL)
		{
			ret_code = SE_ERR_HANDLE_INVALID;
			break;
		}
		
		port_spi_gpio_init();
	
		if(g_spi_device_init[p_comm_param->slave_id] == FALSE)
		{
			ret_code = port_spi_init(p_comm_param->spi_handle,SPI2,p_comm_param->nss_mode);
			if(ret_code != SE_SUCCESS)
			{
				break;
			}
			g_spi_device_init[p_comm_param->slave_id] = TRUE;

		}

		if(p_comm_param->slave_id == SPI_PERIPHERAL_SE0)
		{
			PORT_SPI_SE0_RST_HIGH();   //高电平
			if(p_comm_param->nss_mode == PORT_SPI_NSS_SOFT)
			{
				PORT_SPI_SE0_CS_OFF();
				port_spi_se0_cs_init();
			}

			
		}	
	}while(0);

	return ret_code;
}


/**
* @brief 从设备periph 的通信终止化
* -# 终止化SPI接口
* -# 设置RST控制IO为低电平
* @param [in] periph  设备句柄
* @return 函数操作状态码	
* @note no
* @see 函数  port_spi_deinit
*/
se_error_t port_spi_periph_deinit (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph) 
{
	se_error_t ret_code = SE_SUCCESS;
	spi_comm_param_pointer p_comm_param = (spi_comm_param_pointer)periph->extra;

	do
	{
		if(periph == NULL)
		{
			ret_code = SE_ERR_HANDLE_INVALID;
			break;
		}

		if(p_comm_param->slave_id == SPI_PERIPHERAL_SE0)
		{
			if(g_spi_device_init[SPI_PERIPHERAL_SE0] == TRUE)
			{
				ret_code= port_spi_deinit(p_comm_param->spi_handle);
			}
			PORT_SPI_SE0_RST_LOW();   //低电平	
		}

		g_spi_device_init[p_comm_param->slave_id] = FALSE;

		HAL_GPIO_WritePin(SPI2_SCK_GPIO_PORT, SPI2_SCK_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SPI2_MISO_GPIO_PORT, SPI2_MISO_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SPI2_MOSI_GPIO_PORT, SPI2_MOSI_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SPI2_CS_GPIO_PORT, SPI2_CS_PIN, GPIO_PIN_RESET);  

	}while(0);

    return ret_code;
}


/**
* @brief 从设备periph选择使能或去使能
* @param [in] enable  使能: TRUE,  去使能: FALSE
* @return 函数操作状态码	
* @note 由于在STM32 Hal库的发送函数HAL_SPI_Transmit中，会调用函数__HAL_SPI_ENABLE，
*	      故使能时，可不用再调用此函数；由于STM32 Hal库的接收函数HAL_SPI_Receive
*	      未调用__HAL_SPI_DISABLE，故去使能时，需要调用此函数
*/
se_error_t port_spi_periph_chip_select (HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, bool enable) 
{
	se_error_t ret_code = SE_SUCCESS;
	spi_comm_param_pointer p_comm_param = (spi_comm_param_pointer)periph->extra;

	do
	{
		if(periph == NULL)
		{
			ret_code = SE_ERR_HANDLE_INVALID;
			break;
		}

		if(g_spi_device_init[p_comm_param->slave_id] == FALSE)
		{
			ret_code = SE_ERR_COMM;
			break;
		}
				
		if(enable == TRUE)
		{	__HAL_SPI_ENABLE(p_comm_param->spi_handle);
			if(p_comm_param->nss_mode == PORT_SPI_NSS_SOFT)
			{
			//	__HAL_SPI_ENABLE(p_comm_param->spi_handle);
				if(p_comm_param->slave_id == SPI_PERIPHERAL_SE0)
				{
					PORT_SPI_SE0_CS_ON();
				}	
			}
		}
		else
		{
			if(p_comm_param->nss_mode == PORT_SPI_NSS_SOFT)
			{
				if(p_comm_param->slave_id == SPI_PERIPHERAL_SE0)
				{
					PORT_SPI_SE0_CS_OFF();
				}	
			}
			__HAL_SPI_DISABLE (p_comm_param->spi_handle);  
		}
	}while(0);

	return ret_code;
}


/**
* @brief 通过SPI接口从外设periph发送多字节数据
* -# 调用mcu hal库的HAL_SPI_Transmit函数发送多字节数据
* @param [in] periph  设备句柄
* @param [in] inbuf  待发送数据的起始地址
* @param [in] inbuf_len 待发送数据的长度    
* @return 函数操作状态码	
* @note no
*/
se_error_t port_spi_periph_transmit(HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, uint8_t *inbuf, uint32_t  inbuf_len) 
{
	se_error_t ret_code = SE_SUCCESS;
	spi_comm_param_pointer p_comm_param = (spi_comm_param_pointer)periph->extra;
	
	do
	{
		if(periph == NULL)
		{
			ret_code = SE_ERR_HANDLE_INVALID;
			break;
		}
		
		if((inbuf == NULL) || (inbuf_len == 0U))
		{
			ret_code = SE_ERR_PARAM_INVALID;
			break;
		}

		if(g_spi_device_init[p_comm_param->slave_id] == FALSE)
		{
			ret_code = SE_ERR_COMM;
			break;
		}
		
		ret_code = HAL_SPI_Transmit(p_comm_param->spi_handle, inbuf, inbuf_len, PORT_SPI_HAL_TIMEOUT);
		if(ret_code == HAL_BUSY)
		{
			ret_code = SE_ERR_BUSY;
			break;
		}
		else if(ret_code == HAL_TIMEOUT)
		{
			ret_code = SE_ERR_TIMEOUT;
			break;
		}
		else if(ret_code != HAL_OK)
		{
			ret_code = SE_ERR_COMM;
			break;
		}

	}while(0);
	
	return ret_code;
}


/**
* @brief 通过SPI接口从外设periph接收多字节数据
* -# 调用mcu hal库的HAL_SPI_Receive函数接收多字节数据
* @param [in] periph  设备句柄
* @param [out] outbuf  待接收数据的起始地址
* @param [out] outbuf_len 待接收数据的长度      
* @return 函数操作状态码	
* @note no
*/
se_error_t port_spi_periph_receive(HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, uint8_t *outbuf, uint32_t *outbuf_len) 
{
	se_error_t ret_code = SE_ERR_COMM;
	spi_comm_param_pointer p_comm_param = (spi_comm_param_pointer)periph->extra;

	do
	{
		if(periph == NULL)
		{
			ret_code = SE_ERR_HANDLE_INVALID;
			break;
		}
		
		if((outbuf == NULL) || (outbuf_len == NULL))
		{
			ret_code = SE_ERR_PARAM_INVALID;
			break;
		}

		if(g_spi_device_init[p_comm_param->slave_id] == FALSE)
		{
			ret_code = SE_ERR_COMM;
			break;
		}

		ret_code = HAL_SPI_Receive(p_comm_param->spi_handle, outbuf, (uint16_t)*outbuf_len, PORT_SPI_HAL_TIMEOUT);
		if(ret_code == HAL_BUSY)
		{
			ret_code = SE_ERR_BUSY;
			break;
		}
		else if(ret_code == HAL_TIMEOUT)
		{
			ret_code = SE_ERR_TIMEOUT;
			break;
		}
		else if(ret_code != HAL_OK)
		{
			ret_code = SE_ERR_COMM;
			break;
		}

	}while(0);
	
	return ret_code;
}


/**
* @brief 根据控制码和输入数据，对外设periph 进行复位控制操作
* @param [in] periph  设备句柄
* @param [in] ctrlcode  控制码
* @param [in] inbuf  发送控制数据的起始地址
* @param [in] inbuf_len 发送控制数据的长度         
* @return 函数操作状态码	
* @note no
*/
se_error_t port_spi_periph_control(HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, uint32_t ctrlcode, uint8_t *inbuf, uint32_t  *inbuf_len) 
{
	se_error_t ret_code = SE_SUCCESS;
	spi_comm_param_pointer p_comm_param = (spi_comm_param_pointer)periph->extra;

	do
	{
		if(periph == NULL)
		{
			ret_code = SE_ERR_HANDLE_INVALID;
			break;
		}
		
		if(ctrlcode == 0U)
		{
			ret_code = SE_ERR_PARAM_INVALID;
			break;
		}

		if(ctrlcode == PORT_SPI_CTRL_RST)
		{
			if(p_comm_param->slave_id == SPI_PERIPHERAL_SE0)
			{
				PORT_SPI_SE0_RST_LOW();
				port_spi_periph_delay(PORT_SPI_SE_RST_LOW_DELAY);  //复位时，RST低电平持续时间
				PORT_SPI_SE0_RST_HIGH(); 
				port_spi_periph_delay(PORT_SPI_SE_RST_HIGH_DELAY);  //复位后，RST高电平持续时间
			}
		}

	}while(0);

	return ret_code;
}



/**
  * @}
  */





/* Exported functions --------------------------------------------------------*/

/** (@defgroup Port_Stm32l433_Spi_Exported_Functions Port_Stm32l433_Spi Exported Functions
  * @{
  */


/**
* @brief 用于微妙级延时
* @param [in] us  延时输入值，单位为微秒
* @return no	
* @note no
*/
se_error_t port_spi_periph_delay(uint32_t us)
{
	uint32_t i = 0;
	uint32_t j = 0;
	
	for(i=0;i<us;i++)
	{
		//系统时钟为48Mhz时，for循环终止值为11。
		//系统时钟为80Mhz时，for循环终止值为18。
		for(j=0;j<18;j++);
	}
	return SE_SUCCESS;
}



/**
* @brief 启动计时
* @param [out]  timer_start  起始时间值
* @return 函数操作状态码	
* @note no
*/
se_error_t port_spi_periph_timer_start(util_timer_t *timer_start) 
{
	timer_start->tv_msec = HAL_GetTick(); 
	return SE_SUCCESS;			
}


/**
* @brief 判断计时是否超时
* @param [in] timer_differ 传入的起始时间值 
* @return 函数操作状态码	
* @note no
*/
se_error_t port_spi_periph_timer_differ(util_timer_t *timer_differ) 
{
	
	uint32_t start, end;
	uint32_t time_use_ms;

	start = timer_differ->tv_msec;
	end = HAL_GetTick(); 

	if(end >= start)
	{
		time_use_ms = end - start;
	}
	else
	{
		time_use_ms = 0xffffffff -(start - end);
	}

	if(time_use_ms>=timer_differ->interval)
	{
		return SE_ERR_TIMEOUT;	
	}
	return SE_SUCCESS;
}

SPI_PERIPHERAL_DEFINE_BEGIN(SPI_PERIPHERAL_SE0)
    port_spi_periph_init,
    port_spi_periph_deinit,
    port_spi_periph_delay,
    port_spi_periph_timer_start,
    port_spi_periph_timer_differ,
    port_spi_periph_chip_select,
    port_spi_periph_transmit,
    port_spi_periph_receive,
    port_spi_periph_control,
    &spi_comm_parm_se0,
SPI_PERIPHERAL_DEFINE_END()

SPI_PERIPHERAL_REGISTER(SPI_PERIPHERAL_SE0);

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




