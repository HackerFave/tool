/**@file  port_i2c.c
* @brief  I2C �ӿ�Ӳ������ʵ����������, ʹ���豸�������������ϵͳ����ʵ��
* @author  zhengwd
* @date  2021-4-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/


/***************************************************************************
* Include Header Files
***************************************************************************/



//#include "port_config.h"
#include "port_util.h"
#include "port_i2c.h"
#include "error.h"
#include "util.h"
#include "log.h"
#include "port_config.h"




/**************************************************************************
* Global Variable Declaration
***************************************************************************/
I2C_HandleTypeDef i2c_comm_handle_se0={0};
i2c_comm_param_t i2c_comm_parm_se0={&i2c_comm_handle_se0, PORT_I2C_ADDRESS_2A, I2C_PERIPHERAL_SE0};
static uint8_t g_i2c_device_init[MAX_PERIPHERAL_DEVICE]={FALSE};

/** @addtogroup SE_Driver
  * @{
  */

/** @addtogroup PORT 
  * @brief hardware  portable layer .
  * @{
  */


/** @defgroup PORT_I2C PORT_I2C
  * @brief hardware portable layer i2c interface driver.
  * @{
  */



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function  -----------------------------------------------*/
/** @defgroup Port_I2c_Private_Functions Port_I2c Private Functions
 * @{
 */



/********************************************************************************* 
Function:       HAL_I2C_MspInit
Description:    This function configures the i2c hardware resources
Input:          hi2c I2C handle pointer
Output:         no  
Return:         no
Others:         no 
*********************************************************************************/
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	GPIO_InitTypeDef  GPIO_InitStruct={0};
	RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct={0};

	/*##-1- Configure the I2C clock source. The clock is derived from the SYSCLK #*/
	RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2Cx;
	RCC_PeriphCLKInitStruct.I2c1ClockSelection = RCC_I2CxCLKSOURCE_SYSCLK;
	HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

	/*##-2- Enable peripherals and GPIO Clocks #################################*/
	/* Enable GPIO TX/RX clock */
	I2Cx_SCL_GPIO_CLK_ENABLE();
	I2Cx_SDA_GPIO_CLK_ENABLE();
	/* Enable I2Cx clock */
	I2Cx_CLK_ENABLE(); 

	/*##-3- Configure peripheral GPIO ##########################################*/  
	/* I2C TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       = I2Cx_SCL_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	//GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = I2Cx_SCL_SDA_AF;
	HAL_GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);

	/* I2C RX GPIO pin configuration  */
	GPIO_InitStruct.Pin       = I2Cx_SDA_PIN;
	GPIO_InitStruct.Alternate = I2Cx_SCL_SDA_AF;
	HAL_GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStruct);
}



/********************************************************************************* 
Function:       HAL_I2C_MspDeInit
Description:    This function frees the i2c hardware resources
Input:          hi2c  I2C handle pointer
Output:         no  
Return:         no
Others:         no 
*********************************************************************************/
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
	/*##-1- Reset peripherals ##################################################*/
	I2Cx_FORCE_RESET();
	I2Cx_RELEASE_RESET();

	/*##-2- Disable peripherals and GPIO Clocks #################################*/
	/* Configure I2C Tx as alternate function  */
	HAL_GPIO_DeInit(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_PIN);
	/* Configure I2C Rx as alternate function  */
	HAL_GPIO_DeInit(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_PIN);
}

/**
* @brief ������SE ��gpio���г�ʼ��
* @return no	
* @note no
*/
void port_i2c_gpio_init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStruct = {0};

	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; 
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

	//---SE RST ����IO----
	GPIO_InitStruct.Pin = PORT_I2C_SE0_RST_IO_PIN;
	PORT_I2C_SE0_RST_IO_CLK_ENABLE();
	HAL_GPIO_Init(PORT_I2C_SE0_RST_IO_PORT, &GPIO_InitStruct);
}



/*********************************************************************************
Function:       port_i2c_init
Description:    i2c�ӿڳ�ʼ������ȡ�豸���
Input:          i2c_handle_instance   I2C handle pointer
                i2c_addr ���豸��ַ
Output:         no
Return:         ��������״̬��
Others:         no
*********************************************************************************/
se_error_t  port_i2c_init(I2C_HandleTypeDef *i2c_handle_instance, uint8_t i2c_addr)
{
	//Configure the I2C peripheral 
	I2C_HandleTypeDef *HEDSEi2cHandle = i2c_handle_instance;

	(*HEDSEi2cHandle).Instance             = I2Cx;    
	(*HEDSEi2cHandle).Init.Timing          = PORT_I2C_TIMING_400K_16_9;
	(*HEDSEi2cHandle).Init.OwnAddress1     = i2c_addr;
	(*HEDSEi2cHandle).Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	(*HEDSEi2cHandle).Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	(*HEDSEi2cHandle).Init.OwnAddress2     = 0xFF;
	(*HEDSEi2cHandle).Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	(*HEDSEi2cHandle).Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

	if(HAL_I2C_Init(HEDSEi2cHandle) != HAL_OK)
	{
		return SE_ERR_INIT;
	}

	//Enable the Analog I2C Filter 
	HAL_I2CEx_ConfigAnalogFilter(HEDSEi2cHandle,I2C_ANALOGFILTER_ENABLE);

	return SE_SUCCESS;
}


/**
* @brief I2C�ӿ��Ƴ�����ȡ�豸������
* @param [in] handle   i2c �ӿھ��
* @return ��������״̬��
* @note no
*/
se_error_t port_i2c_deinit(void *handle)
{
	if(HAL_I2C_DeInit((I2C_HandleTypeDef*)handle)!=HAL_OK)
	{
		return SE_ERR_INIT;
	}

	return SE_SUCCESS;
}


/**
  * @}
  */





/* Exported functions --------------------------------------------------------*/

/** (@defgroup Port_I2c_Exported_Functions Port_I2c Exported Functions
  * @{
  */


/**
* @brief ����΢���ʱ
* @param [in] us  ��ʱ����ֵ����λΪ΢��
* @return no	
* @note no
*/
se_error_t port_i2c_periph_delay(uint32_t us)
{
	uint32_t i = 0;
	uint32_t j = 0;
	
	for(i=0;i<us;i++)
	{
		//ϵͳʱ��Ϊ48Mhzʱ��forѭ����ֵֹΪ11��
		//ϵͳʱ��Ϊ80Mhzʱ��forѭ����ֵֹΪ18��
		for(j=0;j<18;j++);
	}
	return SE_SUCCESS;
}


/**
* @brief ������ʱ
* @param [out]  timer_start  ��ʼʱ��ֵ
* @return ��������״̬��	
* @note no
*/
se_error_t port_i2c_periph_timer_start(util_timer_t *timer_start) 
{
	timer_start->tv_msec = HAL_GetTick(); 
	return SE_SUCCESS;		
}


/**
* @brief �жϼ�ʱ�Ƿ�ʱ
* @param [in]  timer_differ �������ʼʱ��ֵ
* @return ��������״̬��	
* @note no
*/
se_error_t port_i2c_periph_timer_differ(util_timer_t *timer_differ) 
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


/**
* @brief ���豸periph��ͨ�ų�ʼ��
* -# ������SE��gpio���г�ʼ��
* -# ��ʼ��I2C�ӿ�
* -# ����RST����IOΪ�ߵ�ƽ
* @param [in] periph  �豸���
* @return ��������״̬��	
* @note no
* @see ����  port_i2c_gpio_init  port_i2c_init
*/
se_error_t port_i2c_periph_init (HAL_I2C_PERIPHERAL_STRUCT_POINTER periph) 
{	  
	se_error_t ret_code = SE_SUCCESS;
	i2c_comm_param_pointer p_comm_param = (i2c_comm_param_pointer)periph->extra;

	do
	{
		if(periph == NULL)
		{
			ret_code = SE_ERR_HANDLE_INVALID;
			break;
		}
		
		port_i2c_gpio_init();
	
		if(g_i2c_device_init[p_comm_param->slave_id] == FALSE)
		{
			ret_code = port_i2c_init(p_comm_param->i2c_handle, p_comm_param->slave_addr);
			if(ret_code != SE_SUCCESS)
			{
				break;
			}
			g_i2c_device_init[p_comm_param->slave_id] = TRUE;
		}

		if(p_comm_param->slave_id == I2C_PERIPHERAL_SE0)
		{
			PORT_I2C_SE0_RST_HIGH();   //�ߵ�ƽ
		}
	}while(0);

	return ret_code;
}


/**
* @brief ���豸periph ��ͨ����ֹ��
* -# ��ֹ��I2C�ӿ�
* -# ����RST����IOΪ�͵�ƽ
* @param [in] periph  �豸���
* @return ��������״̬��	
* @note no
* @see ����  port_i2c_deinit
*/
se_error_t port_i2c_periph_deinit (HAL_I2C_PERIPHERAL_STRUCT_POINTER periph) 
{
	se_error_t ret_code = SE_SUCCESS;
	i2c_comm_param_pointer p_comm_param = (i2c_comm_param_pointer)periph->extra;

	do
	{
		if(periph == NULL)
		{
			ret_code = SE_ERR_HANDLE_INVALID;
			break;
		}

		if(p_comm_param->slave_id == I2C_PERIPHERAL_SE0)
		{
			if(g_i2c_device_init[I2C_PERIPHERAL_SE0] == TRUE)
			{
				ret_code= port_i2c_deinit(p_comm_param->i2c_handle);
			}
			PORT_I2C_SE0_RST_LOW();   //�͵�ƽ	
		}

		g_i2c_device_init[p_comm_param->slave_id] = FALSE;

	}while(0);

    return ret_code;
}


/**
* @brief ͨ��I2C �ӿڴ�����periph���Ͷ��ֽ�����
* -# ����HAL_I2C_Master_Transmit �������Ͷ��ֽ�����
* @param [in] periph  �豸���
* @param [in] inbuf  ���������ݵ���ʼ��ַ
* @param [in] inbuf_len ���������ݵĳ���    
* @return ��������״̬��	
* @note no
*/
se_error_t port_i2c_periph_transmit(HAL_I2C_PERIPHERAL_STRUCT_POINTER periph, uint8_t *inbuf, uint32_t  inbuf_len) 
{
	se_error_t ret_code = SE_SUCCESS;
	i2c_comm_param_pointer p_comm_param = (i2c_comm_param_pointer)periph->extra;
	
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

		if(g_i2c_device_init[p_comm_param->slave_id] == FALSE)
		{
			ret_code = SE_ERR_COMM;
			break;
		}

		
		ret_code = HAL_I2C_Master_Transmit(p_comm_param->i2c_handle, (uint16_t)p_comm_param->slave_addr, inbuf, inbuf_len, PORT_I2C_HAL_TIMEOUT);
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
* @brief ͨ��I2C �ӿڴ�����periph���ն��ֽ�����
* -# ����HAL_I2C_Master_Receive �������ն��ֽ�����
* @param [in] periph  �豸���
* @param [out] outbuf  ���������ݵ���ʼ��ַ
* @param [out] outbuf_len ���������ݵĳ���      
* @return ��������״̬��	
* @note no
*/
se_error_t port_i2c_periph_receive(HAL_I2C_PERIPHERAL_STRUCT_POINTER periph, uint8_t *outbuf, uint32_t *outbuf_len) 
{
	se_error_t ret_code = SE_SUCCESS;
	i2c_comm_param_pointer p_comm_param = (i2c_comm_param_pointer)periph->extra;

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

		if(g_i2c_device_init[p_comm_param->slave_id] == FALSE)
		{
			ret_code = SE_ERR_COMM;
			break;
		}
		
	   ret_code = HAL_I2C_Master_Receive(p_comm_param->i2c_handle, (uint16_t)p_comm_param->slave_addr, outbuf, (uint16_t)*outbuf_len, PORT_I2C_HAL_TIMEOUT);
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
* @brief ���ݿ�������������ݣ�������periph ���и�λ���Ʋ���
* @param [in] periph  �豸���
* @param [in] ctrlcode  ������
* @param [in] inbuf  ���Ϳ������ݵ���ʼ��ַ
* @param [in] inbuf_len ���Ϳ������ݵĳ���         
* @return ��������״̬��	
* @note no
*/
se_error_t port_i2c_periph_control(HAL_I2C_PERIPHERAL_STRUCT_POINTER periph, uint32_t ctrlcode, uint8_t *inbuf, uint32_t  inbuf_len) 
{
	se_error_t ret_code = SE_SUCCESS;
	i2c_comm_param_pointer p_comm_param = (i2c_comm_param_pointer)periph->extra;

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

		if(ctrlcode == PORT_I2C_CTRL_RST)
		{
			if(p_comm_param->slave_id == I2C_PERIPHERAL_SE0)
			{
				PORT_I2C_SE0_RST_LOW();
				port_i2c_periph_delay(PORT_I2C_SE_RST_LOW_DELAY);  //��λʱ��RST�͵�ƽ����ʱ��
				PORT_I2C_SE0_RST_HIGH(); 
				port_i2c_periph_delay(PORT_I2C_SE_RST_HIGH_DELAY);  //��λ��RST�ߵ�ƽ����ʱ��	
			}
		}

	}while(0);

	return ret_code;
}


I2C_PERIPHERAL_DEFINE_BEGIN(I2C_PERIPHERAL_SE0)
     port_i2c_periph_init,
     port_i2c_periph_deinit,
     port_i2c_periph_delay,
     port_i2c_periph_timer_start,
     port_i2c_periph_timer_differ,
     port_i2c_periph_transmit,
     port_i2c_periph_receive,
     port_i2c_periph_control,
     &i2c_comm_parm_se0,
I2C_PERIPHERAL_DEFINE_END()

I2C_PERIPHERAL_REGISTER(I2C_PERIPHERAL_SE0);


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




