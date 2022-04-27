/**@file  port_i2c.c
* @brief  I2C �ӿ�Ӳ������ʵ����������, ʹ���豸�������������ϵͳ����ʵ��
* @author  zhengwd
* @date  2021-04-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/


/***************************************************************************
* Include Header Files
***************************************************************************/

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include "port_i2c.h"
#include "port_util.h"
#include "port_config.h"
#include "error.h"
#include "util.h"
#include "log.h"




/**************************************************************************
* Global Variable Declaration
***************************************************************************/
I2C_SetDef i2c_comm_handle_se0={0};
i2c_comm_param_t i2c_comm_parm_se0={&i2c_comm_handle_se0, I2C_CHANNEL_I2C0, I2C_SLAVE_ADRR, I2C_SPEED_DEFAULT, I2C_TIMEOUT_DEFAULT,I2C_RETRY_DEFAULT,I2C_PERIPHERAL_SE0};
static uint8_t g_i2c_device_init[MAX_PERIPHERAL_DEVICE]={FALSE};

volatile uint32_t g_i2c_lock=0;

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
static const char   *i2cDev0  = "/dev/i2c-0" ;
static const char   *i2cDev1  = "/dev/i2c-1" ;



/* Private function  -----------------------------------------------*/
/** @defgroup Port_I2c_Private_Functions Port_I2c Private Functions
 * @{
 */



/**
* @brief ������SE ��gpio���г�ʼ��
* @param [in] handle  �豸���
* @return no	
* @note no
*/
void port_i2c_gpio_init(void)
{	
	port_gpio_export(PORT_I2C_SE0_RST_IO);
	usleep(30 * 1000);
	port_gpio_direction(PORT_I2C_SE0_RST_IO, OUT);
}


/**(
* @brief Open the I2C device, and set param, etc.
* @param [in] handle   i2c �ӿھ��
* @param [in] channel  i2c ͨ��
* @param [in] slave	    i2c�ӵ�ַ
* @param [in] speed	    i2c ͨ���ٶ�
* @param [in] timeout  i2c ͨ�ų�ʱʱ��
* @param [in] retry	    i2c ͨ�����Դ���
* @return ��������״̬��
* @note no
*/
int port_i2c_set(void *handle, int channel, int slave, int speed, int timeout, int retry)
{
	int fd = 0;
	I2C_SetDef *i2cSetHandle = (I2C_SetDef *)handle;

	if ((fd = open (channel ==1 ? i2cDev0 : i2cDev1, O_RDWR)) < 0)
	{
		printf("Unable to open I2C device: %s\n", strerror (errno)) ;
		return -1;
	}

	i2cSetHandle->aiI2cFds[channel] = fd;

	/*
	* ����i2c ��д�����Ĵ��豸��ַ
	*/
	if (ioctl (fd, I2C_SLAVE, slave) < 0)     
	{                                                               
		printf("Can't set i2c slave address: %s\n", strerror (errno))  ;  
		return -1;                                                    
	}  

	i2cSetHandle->aiI2cSlave[channel] = slave;

	
	//ioctl(fd,I2C_TIMEOUT,20);//��ʱʱ��
	ioctl(fd,I2C_TIMEOUT,timeout);//��ʱʱ��
	ioctl(fd,I2C_RETRIES,retry);//�ظ�����

	return fd ;
}


/**
* @brief ͨ��I2C�ӿڷ��Ͷ��ֽ�����
* -# ͨ��Linux ioctl��ʽ���Ͷ��ֽ�����
* @param [in] handle   i2c �ӿھ��
* @param [in] channel  i2c ͨ��
* @param [in] tx_data  ���������ݵ���ʼ��ַ
* @param [in] len ���������ݵĳ���    
* @return ��������״̬��	
* @note no
*/
int port_i2c_tx(void *handle, int channel, uint8_t *tx_data, int len)
{
	int fd ;
	int ret = 0;
	struct i2c_rdwr_ioctl_data msg_rdwr;
	struct i2c_msg i2cmsg;


	I2C_SetDef *i2cSetHandle = (I2C_SetDef *)handle;

	fd = i2cSetHandle->aiI2cFds[channel];

	msg_rdwr.msgs = &i2cmsg;
	msg_rdwr.nmsgs = 1;

	i2cmsg.addr  =  i2cSetHandle->aiI2cSlave[channel];//�豸��ַ;
	i2cmsg.flags = 0;    //write
	i2cmsg.len	 = len;
	i2cmsg.buf = tx_data;

	if((ret=ioctl(fd,I2C_RDWR,&msg_rdwr))<0){
		//perror("ioctl()");
		//fprintf(stderr,"ioctl returned %d\n",ret);
		return -1;
	}

	return SE_SUCCESS;


}


/**
* @brief ͨ��I2C �ӿڽ��ն��ֽ�����
* -# ͨ��Linux ioctl��ʽ���ն��ֽ�����
* @param [in] handle   i2c �ӿھ��
* @param [in] channel  i2c ͨ��
* @param [out] rx_data  ���������ݵ���ʼ��ַ
* @param [in] len ���������ݵĳ���      
* @return ��������״̬��	
* @note no
*/
int port_i2c_rx(void *handle, int channel, uint8_t *rx_data, int len)
{
	int fd ;
	int ret = 0;
	struct i2c_rdwr_ioctl_data msg_rdwr;
	struct i2c_msg i2cmsg;


	I2C_SetDef *i2cSetHandle = (I2C_SetDef *)handle;

	fd = i2cSetHandle->aiI2cFds[channel];

	msg_rdwr.msgs = &i2cmsg;
	msg_rdwr.nmsgs = 1;

	i2cmsg.addr  =  i2cSetHandle->aiI2cSlave[channel];//�豸��ַ;
	i2cmsg.flags = I2C_M_RD;    //read
	i2cmsg.len	 = len;
	i2cmsg.buf = rx_data;

	if((ret=ioctl(fd,I2C_RDWR,&msg_rdwr))<0){
		//perror("ioctl()");
		//fprintf(stderr,"ioctl returned %d\n",ret);
		return -1;
	}

	return SE_SUCCESS;
}




/**
* @brief I2C�ӿڳ�ʼ��
* @param [in] handle    i2c�ӿھ��
* @param [in] channel  i2c ͨ��
* @param [in] slave	    i2c ���豸��ַ
* @param [in] speed	    i2c ͨ���ٶ�
* @param [in] timeout i2c ���ͳ�ʱʱ��
* @param [in] retry      i2c �ظ����ʹ���
* @return ��������״̬��
* @note no
*/
se_error_t port_i2c_init(void *handle, int channel, int slave, int speed, int timeout, int retry)
{
	if(port_i2c_set(handle, channel, slave, speed, timeout, retry)<0)
	{
		return SE_ERR_PARAM_INVALID;
	}
	return SE_SUCCESS;
}


/**
* @brief i2c�ӿ��Ƴ�����ȡ�豸������
* @param [in] handle   i2c �ӿھ��
* @param [in] channel i2cͨ��
* @return ��������״̬��
* @note no
*/
se_error_t  port_i2c_deinit (void * handle, int channel)
{
	int fd ;
	I2C_SetDef *i2cSetHandle = (I2C_SetDef *)handle;

	fd = i2cSetHandle->aiI2cFds[channel];

	close(fd);

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
se_error_t port_i2c_periph_timer_start(util_timer_t *timer_start) 
{
	struct timeval timeofday;

	gettimeofday(&timeofday,NULL); 
	timer_start->tv_sec = timeofday.tv_sec; 
	timer_start->tv_usec = timeofday.tv_usec; 
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
* -# ��ʼ��I2C �ӿ�
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
			ret_code = port_i2c_init(p_comm_param->i2c_handle,p_comm_param->channel,p_comm_param->slave,p_comm_param->speed,p_comm_param->timeout,p_comm_param->retry);
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
				ret_code= port_i2c_deinit(p_comm_param->i2c_handle,p_comm_param->channel);
			}
			PORT_I2C_SE0_RST_LOW();   //�͵�ƽ	
		}

		g_i2c_device_init[p_comm_param->slave_id] = FALSE;

	}while(0);

    return ret_code;
}


/**
* @brief ͨ��I2C �ӿڴ�����periph���Ͷ��ֽ�����
* -# ����port_i2c_tx �������Ͷ��ֽ�����
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

		if(port_lock(&g_i2c_lock)==LOCK_BUSY)
		{
			ret_code = SE_ERR_BUSY;
			break;
		}	

		ret_code = port_i2c_tx(p_comm_param->i2c_handle, p_comm_param->channel, inbuf, inbuf_len);
		port_unlock(&g_i2c_lock);
		if(ret_code != SE_SUCCESS)
		{
			ret_code = SE_ERR_COMM;
			break;
		}

	}while(0);
	//port_unlock(&g_i2c_lock);
	
	return ret_code;
}


/**
* @brief ͨ��I2C �ӿڴ�����periph���ն��ֽ�����
* -# ����port_i2c_rx �������ն��ֽ�����
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

		if(port_lock(&g_i2c_lock)==LOCK_BUSY)
		{
			ret_code = SE_ERR_BUSY;
			break;
		}
				
		ret_code = port_i2c_rx(p_comm_param->i2c_handle, p_comm_param->channel, outbuf, *outbuf_len);
		port_unlock(&g_i2c_lock);
		if(ret_code != SE_SUCCESS)
		{
			ret_code = SE_ERR_COMM;
			break;
		}
		//port_unlock(&g_i2c_lock);

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




