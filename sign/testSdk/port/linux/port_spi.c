/**@file  port_spi.c
* @brief  SPI �ӿ�Ӳ������ʵ����������, ʹ���豸�������������ϵͳ����ʵ��
* @author  zhengwd
* @date  2021-04-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/


/***************************************************************************
* Include Header Files
***************************************************************************/
#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <asm/ioctl.h>
#include <linux/spi/spidev.h>
#include "port_spi.h"
#include "port_util.h"
#include "port_config.h"
#include "error.h"
#include "util.h"
#include "log.h"


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
SPI_SetDef spi_comm_handle_se0={0};
spi_comm_param_t spi_comm_parm_se0={&spi_comm_handle_se0, SPI_CHANNEL_SPI0, PORT_SPI_COMM_MODE0, SPI_SPEED, SPI_BPW,SPI_PERIPHERAL_SE0,PORT_SPI_CS_CTRL_MODE_HARD};
static uint8_t g_spi_device_init[MAX_PERIPHERAL_DEVICE]={FALSE};
volatile uint32_t locktag_spi=0;

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
static const char   *spiDev0  = "/dev/spidev0.0" ;
static const char   *spiDev1  = "/dev/spidev0.0" ;

/* Private function  -----------------------------------------------*/
/** @defgroup Port_Spi Private_Functions Port_Spi Private Functions
 * @{
 */




/**
* @brief ������SE ��gpio���г�ʼ��
* @return no	
* @note no
*/
void port_spi_gpio_init(HAL_SPI_PERIPHERAL_STRUCT_POINTER handle)
{
	spi_comm_param_pointer p_comm_param = (spi_comm_param_pointer)(handle->extra);

	port_gpio_export(PORT_SPI_SE0_RST_IO);
	if(p_comm_param->cs_mode==PORT_SPI_CS_CTRL_MODE_SOFT)
	{
		port_gpio_export(PORT_SPI_SE0_CS_IO);
	}
	usleep(30 * 1000);
	
	port_gpio_direction(PORT_SPI_SE0_RST_IO, OUT);
	if(p_comm_param->cs_mode==PORT_SPI_CS_CTRL_MODE_SOFT)
	{
		port_gpio_direction(PORT_SPI_SE0_CS_IO, OUT);
		port_gpio_write(PORT_SPI_SE0_CS_IO, HIGH);	
	}
}


/*
* SPISetupMode:
*    Open the SPI device, and set it up, with the mode, etc.
*********************************************************************************
*********************************************************************************/
/**(
* @brief Open the SPI device, and set it up, with the mode, etc.
* @param [in] handle  spi �ӿھ��
* @param [in] channel	spi ͨ��
* @param [in] mode	  spi ͨ��ģʽ
* @param [in] speed	  spi ͨ���ٶ�
* @param [in] bpw	  spi ͨ������λ��
* @return ��������״̬��
* @note no
*/
int port_spi_set(void *handle, int channel, int mode, int speed, uint8_t bpw)
{
	int fd = 0 ;

	SPI_SetDef *spiSetHandle = (SPI_SetDef *)handle;

	if ((fd = open (channel == 0 ? spiDev0 : spiDev1, O_RDWR)) < 0)
	{
		printf("Unable to open SPI device: %s\n", strerror (errno)) ;
		return -1;
	}
	
	spiSetHandle->aiSpiSpeeds[channel] = speed;
	spiSetHandle->aiSpiFds[channel] = fd;

	/*
	* ����spi�Ķ�дģʽ��
	*  Mode 0�� CPOL=0, CPHA=0
	*  Mode 1�� CPOL=0, CPHA=1
	*  Mode 2�� CPOL=1, CPHA=0
	*  Mode 3�� CPOL=1, CPHA=1
	*/
	if (ioctl (fd, SPI_IOC_WR_MODE, &mode) < 0)                     
	{                                                               
		printf("Can't set spi mode: %s\n", strerror (errno)) ;         
		return -1;                                                    
	}                                                               

	if (ioctl (fd, SPI_IOC_RD_MODE, &mode) < 0)                     
	{                                                               
		printf("Can't get spi mode: %s\n", strerror (errno)) ;        
		return -1;                                                 
	}    

	/*
	* spi�Ķ�дbit/word���ÿ�д
	*/
	if (ioctl (fd, SPI_IOC_WR_BITS_PER_WORD, &bpw) < 0)          
	{                                                               
		printf("Can't set bits per word: %s\n", strerror (errno))  ;  
		return -1;                                                    
	}                                                              

	if (ioctl (fd, SPI_IOC_RD_BITS_PER_WORD, &bpw) < 0)          
	{                                                               
		printf("Can't get bits per word: %s\n", strerror (errno))  ;  
		return -1;                                                   
	}   

	/*
	* ����spi��д����
	*********************************************************************************
	*/
	if (ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
	{
		printf("Can't set max speed hz: %s\n", strerror (errno));
		return -1;
	}

	if (ioctl (fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0)
	{
		printf("Can't get max speed hz: %s\n", strerror (errno));
		return -1;
	}


	return fd ;
}


/**
* @brief ͨ��SPI�ӿڴ�����periph���Ͷ��ֽ�����
* -# ͨ��Linux ioctl��ʽ���Ͷ��ֽ�����
* @param [in] periph  �豸���
* @param [in] inbuf  ���������ݵ���ʼ��ַ
* @param [in] inbuf_len ���������ݵĳ���    
* @return ��������״̬��	
* @note no
*/
int port_spi_tx(void *handle, int channel, uint8_t *tx_data, int len)
{
	int fd ;
	int ret;
	struct spi_ioc_transfer spi ;
	SPI_SetDef *spiSetHandle = (SPI_SetDef *)handle;

	memset (&spi, 0, sizeof (spi)) ;

	fd = spiSetHandle->aiSpiFds[channel];

	spi.tx_buf        = (unsigned long)tx_data ;
	spi.rx_buf        = NULL ;
	spi.len           = len ;
	spi.delay_usecs   = spiSetHandle->sSpiDelay;
	spi.speed_hz      = spiSetHandle->aiSpiSpeeds [channel] ;
	spi.bits_per_word = spiSetHandle->bSpiBPW; 

	ret = ioctl (fd, SPI_IOC_MESSAGE(1), &spi) ; 
	if(ret<0)
	{
		return ret;
	}
	return SE_SUCCESS;

}



/**
* @brief ͨ��SPI�ӿڴ�����periph���ն��ֽ�����
* -# ͨ��Linux ioctl��ʽ���ն��ֽ�����
* @param [in] periph  �豸���
* @param [out] outbuf  ���������ݵ���ʼ��ַ
* @param [out] outbuf_len ���������ݵĳ���      
* @return ��������״̬��	
* @note no
*/
int port_spi_rx(void *handle, int channel, uint8_t *rx_data, int len)
{
	int fd ;
	int ret;
	struct spi_ioc_transfer spi ;
	SPI_SetDef *spiSetHandle = (SPI_SetDef *)handle;

	memset (&spi, 0, sizeof (spi)) ;

	fd = spiSetHandle->aiSpiFds[channel];

	spi.tx_buf        = NULL ;
	spi.rx_buf        = (unsigned long)rx_data ;
	spi.len           = len ;
	spi.delay_usecs   = spiSetHandle->sSpiDelay;
	spi.speed_hz      = spiSetHandle->aiSpiSpeeds [channel] ;
	spi.bits_per_word = spiSetHandle->bSpiBPW; ;

	ret = ioctl (fd, SPI_IOC_MESSAGE(1), &spi) ;
	if(ret<0)
	{
		return ret;
	}
	return SE_SUCCESS;

}




/**
* @brief SPI�ӿڳ�ʼ��
* @param [in] handle   spi �ӿھ��
* @param [in] channel spi ͨ��
* @param [in] mode	   spi ͨ��ģʽ
* @param [in] speed	   spi ͨ���ٶ�
* @param [in] bpw	   spi ͨ������λ��
* @return ��������״̬��
* @note no
*/
se_error_t port_spi_init(void *handle, int channel, int mode, int speed, uint8_t bpw)
{
	if(port_spi_set(handle, channel, mode, speed, bpw)<0)
	{
		return SE_ERR_PARAM_INVALID;
	}
	return SE_SUCCESS;
}


/**
* @brief SPI�ӿ��Ƴ�����ȡ�豸������
* @param [in] handle   spi �ӿھ��
* @param [in] channel spi ͨ��
* @return ��������״̬��
* @note no
*/
se_error_t  port_spi_deinit (void * handle, int channel)
{
	int fd ;
	SPI_SetDef *spiSetHandle = (SPI_SetDef *)handle;

	fd = spiSetHandle->aiSpiFds[channel];

	close(fd);

	return SE_SUCCESS;
}


/**
  * @}
  */





/* Exported functions --------------------------------------------------------*/

/** (@defgroup Port_Linux_Spi_Exported_Functions Port_Linux_Spi Exported Functions
  * @{
  */


/**
* @brief ����΢���ʱ
* @param [in] us  ��ʱ����ֵ����λΪ΢��
* @return no	
* @note no
*/
se_error_t port_spi_periph_delay(uint32_t us)
{
	struct timeval start;
	struct timeval end;

	gettimeofday(&start,NULL); 
	while(1)
	{
		gettimeofday(&end,NULL);
		if((uint32_t)((end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec))>=us)
		{
			break;	
		}	
	}

	return SE_SUCCESS;
}


/**
* @brief ������ʱ
* @param [out]  timer_start  ��ʼʱ��ֵ
* @return ��������״̬��	
* @note no
*/
se_error_t port_spi_periph_timer_start(util_timer_t *timer_start) 
{
	struct timeval timeofday;

	gettimeofday(&timeofday,NULL); 
	timer_start->tv_sec = timeofday.tv_sec; 
	timer_start->tv_usec = timeofday.tv_usec; 
	return SE_SUCCESS;		
}


/**
* @brief �жϼ�ʱ�Ƿ�ʱ
* @param [in] timer_differ �������ʼʱ��ֵ 
* @return ��������״̬��	
* @note no
*/
se_error_t port_spi_periph_timer_differ(util_timer_t *timer_differ) 
{
	struct timeval end;
	uint32_t time_use_ms;

	gettimeofday(&end,NULL);

	time_use_ms=(uint32_t)((end.tv_sec-timer_differ->tv_sec)*1000000+(end.tv_usec-timer_differ->tv_usec))/1000;
	if(time_use_ms>=timer_differ->interval)
	{
		return SE_ERR_TIMEOUT;		
	}
	return SE_SUCCESS;
}



/**
* @brief ���豸periph��ͨ�ų�ʼ��
* -# ������SE��gpio���г�ʼ��
* -# ��ʼ��SPI�ӿ�
* -# ����RST����IOΪ�ߵ�ƽ
* @param [in] periph  �豸���
* @return ��������״̬��	
* @note no
* @see ����  port_spi_gpio_init  port_spi_init
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
		
		port_spi_gpio_init(periph);
	
		if(g_spi_device_init[p_comm_param->slave_id] == FALSE)
		{
			ret_code = port_spi_init(p_comm_param->spi_handle,p_comm_param->channel,p_comm_param->mode,p_comm_param->speed,p_comm_param->bpw);
			if(ret_code != SE_SUCCESS)
			{
				break;
			}
			g_spi_device_init[p_comm_param->slave_id] = TRUE;
		}

		if(p_comm_param->slave_id == SPI_PERIPHERAL_SE0)
		{
			PORT_SPI_SE0_RST_HIGH();   //�ߵ�ƽ
		}
	}while(0);

	return ret_code;
}


/**
* @brief ���豸periph ��ͨ����ֹ��
* -# ��ֹ��SPI�ӿ�
* -# ����RST����IOΪ�͵�ƽ
* @param [in] periph  �豸���
* @return ��������״̬��	
* @note no
* @see ����  port_spi_deinit
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
				ret_code= port_spi_deinit(p_comm_param->spi_handle,p_comm_param->channel);
			}
			PORT_SPI_SE0_RST_LOW();   //�͵�ƽ	
		}

		g_spi_device_init[p_comm_param->slave_id] = FALSE;

	}while(0);

    return ret_code;
}


/**
* @brief ���豸periphѡ��ʹ�ܻ�ȥʹ��
* @param [in] enable  ʹ��: TRUE,  ȥʹ��: FALSE
* @return ��������״̬��	
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
		{
			if(p_comm_param->cs_mode==PORT_SPI_CS_CTRL_MODE_SOFT)
			{
				if(p_comm_param->slave_id == SPI_PERIPHERAL_SE0)
				{
					PORT_SPI_SE0_CS_HIGH();
				}	
			}
		}
		else
		{
			if(p_comm_param->cs_mode==PORT_SPI_CS_CTRL_MODE_SOFT)
			{
				if(p_comm_param->slave_id == SPI_PERIPHERAL_SE0)
				{
					PORT_SPI_SE0_CS_LOW();
				}	
			}
		}
	}while(0);

	return ret_code;
}


/**
* @brief ͨ��SPI�ӿڴ�����periph���Ͷ��ֽ�����
* -# ����port_spi_tx�������Ͷ��ֽ�����
* @param [in] periph  �豸���
* @param [in] inbuf  ���������ݵ���ʼ��ַ
* @param [in] inbuf_len ���������ݵĳ���    
* @return ��������״̬��	
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
		
		if(port_lock(&locktag_spi) == LOCK_UNBUSY)
		{
			ret_code = port_spi_tx(p_comm_param->spi_handle, p_comm_param->channel, inbuf, inbuf_len);
			port_unlock(&locktag_spi);
			if(ret_code != SE_SUCCESS)
			{
				ret_code = SE_ERR_COMM;
				break;
			}
		}
		else 
		{
             ret_code = SE_ERR_BUSY;
			 break;
		}
	}while(0);
	
	return ret_code;
}


/**
* @brief ͨ��SPI�ӿڴ�����periph���ն��ֽ�����
* -# ����port_spi_rx�������ն��ֽ�����
* @param [in] periph  �豸���
* @param [out] outbuf  ���������ݵ���ʼ��ַ
* @param [out] outbuf_len ���������ݵĳ���      
* @return ��������״̬��	
* @note no
*/
se_error_t port_spi_periph_receive(HAL_SPI_PERIPHERAL_STRUCT_POINTER periph, uint8_t *outbuf, uint32_t *outbuf_len) 
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
		
		if(port_lock(&locktag_spi) == LOCK_UNBUSY)
		{
			ret_code = port_spi_rx(p_comm_param->spi_handle, p_comm_param->channel, outbuf, *outbuf_len);
			port_unlock(&locktag_spi);

			if(ret_code != SE_SUCCESS)
			{
				ret_code = SE_ERR_COMM;
				break;
			}
	    }
		else 
		{
			 ret_code = SE_ERR_BUSY;
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
				port_spi_periph_delay(PORT_SPI_SE_RST_LOW_DELAY);  //��λʱ��RST�͵�ƽ����ʱ��
				PORT_SPI_SE0_RST_HIGH(); 
				port_spi_periph_delay(PORT_SPI_SE_RST_HIGH_DELAY);  //��λ��RST�ߵ�ƽ����ʱ��
			}
		}

	}while(0);

	return ret_code;
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




