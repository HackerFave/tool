/**@file  comm.c
* @brief  comm interface declearation	 
* @author  liangww
* @date  2021-04-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/


/***************************************************************************
* Include Header Files
***************************************************************************/
#include "comm.h"
#include "util.h"
#include "error.h"
#include "log.h"

/** @addtogroup SE_Service
  * @{
  */

/** @addtogroup API 
  * @brief API layer.
  * @{
  */

/** @defgroup COMM COMM
  * @brief comm interface api.
  * @{
  */




/**************************************************************************
* Global Variable Declaration
***************************************************************************/
static peripheral_bus_driver *g_drivers[MAX_PERIPHERAL_BUS_DRIVER] = {0};

/* Exported types ------------------------------------------------------------*/
/** @defgroup COMM_Exported_Types comm_driver_holder Exported Types
  * @{
  */

/**
  * @brief  driver_holder Structure definition
  */

struct driver_holder {
    peripheral_bus_driver *driver;
    peripheral *periph;
};

/**
  * @}
  */


#ifdef __MULTITASK
// 多任务环境
// 每个任务的TLS关联一个被选中peripheral_driver对象
#ifdef __FREERTOS
#endif

#ifdef __LINUX
#endif
#else
// 单任务环境
// 通过全局变量关联被选中的一个peripheral_driver对象
static struct driver_holder g_selected_driver = {NULL, NULL};

#define setSelectedDriver(driver) g_selected_driver.driver = driver
#define setSelectedPeriph(periph) g_selected_driver.periph = periph

#define getSelectedDriver() g_selected_driver.driver
#define getSelectedPeriph() g_selected_driver.periph
#endif

#define checkSelectedDriverAndDevice() \
    do {    \
        if (getSelectedDriver() == NULL || getSelectedDriver() == NULL)   \
            return NULL;    \
    }while(0)

/* Exported functions --------------------------------------------------------*/

/** @defgroup COMM_Exported_Functions COMM Exported Functions
  * @{
  */



		
/**
* @brief 增加设备句柄
* @param [in] driver	外设总线驱动句柄数据
* @param [in] periph  外设句柄
* @return 参见error.h
* @note no
* @see no
*/
static peripheral* add_periph(peripheral_bus_driver *driver, peripheral *periph) {
    int i=0;

    for (; i<MAX_PERIPHERAL_DEVICE; i++) {
        if (driver->periph[i] == periph)
            return periph;
        else
            if (driver->periph[i] == NULL) {
                driver->periph[i] = periph;
                return periph;
            }
    }

    return NULL;
}


/**
* @brief 增加外设总线驱动句柄和设备句柄
* @param [in] driver	外设总线驱动句柄数据
* @param [in] periph  外设句柄
* @return 参见error.h
* @note no
* @see 函数 add_periph
*/
static peripheral_bus_driver* add_driver(peripheral_bus_driver *driver, peripheral *periph) {
    int i=0;

    for (; i<MAX_PERIPHERAL_BUS_DRIVER; i++) { 
        if (g_drivers[i] == driver) {
            if (add_periph(driver, periph) != NULL)
                return driver;
            else
                return NULL;
        }
        else {
            if (g_drivers[i] == NULL) {
                g_drivers[i] = driver;
                if (add_periph(driver, periph) != NULL)
                    return driver;
                else
                    return NULL;
            }
        }
    }

    return NULL;
}



/**
* @brief 根据外设类型，从g_driver中查找外设总线驱动句柄
* @param [in] type	外设外设类型动句柄数据
* @return 外设总线驱动句柄
* @note no
* @see no
*/

static peripheral_bus_driver* find_driver(peripheral_type type) {
    int i=0;
	
    for (; i<MAX_PERIPHERAL_BUS_DRIVER; i++) { 
        if (g_drivers[i]->type == type)
            return g_drivers[i];
    }

    return NULL;
}



/**
* @brief 根据外设总线驱动句柄和外设ID值，查找外设句柄
* @param [in] driver	外设总线驱动句柄数据
* @param [in] dev_id  外设ID
* @return 外设句柄
* @note no
* @see no
*/
static peripheral* find_slave_device(peripheral_bus_driver *driver, uint32_t dev_id) {
    int i=0;
	
    for (; i<MAX_PERIPHERAL_DEVICE; i++) { 
        if (driver->periph[i]->id == dev_id)
            return driver->periph[i];
    }

    return NULL;
}




/**
* @brief 根据外设总线驱动类型和外设ID 值进行注册，即外设实例化
* @param [in] driver	外设总线驱动句柄
* @param [in] periph  外设
* @return 参见error.h
* @note no
* @see 函数 add_driver
*/
se_error_t _api_register(peripheral_bus_driver *driver, peripheral *periph) 
{
	if (driver == NULL || periph == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}
	if(add_driver(driver, periph) == NULL)
	{
		return SE_ERR_HANDLE_INVALID;
	}

	return SE_SUCCESS;
}




/**
* @brief 从已注册的多个外设中，选择需要操作的外设
* @param [in] type	外设类型
* @param [in] dev_id  外设ID值
* @return 参见error.h
* @note no
* @see 函数 find_driver find_slave_device setSelectedDriver setSelectedPeriph
*/
se_error_t api_select(peripheral_type type, uint32_t dev_id) 
{
	peripheral_bus_driver *driver = NULL;
	peripheral *periph = NULL;

	if (g_drivers[0] == NULL || g_drivers[0]->periph[0] == NULL)
	{
		return SE_ERR_PARAM_INVALID; //未执行acl_register 操作
	}

	driver = find_driver(type);
	if (driver == NULL)
	{
		return SE_ERR_HANDLE_INVALID; 
	}

	periph = find_slave_device(driver, dev_id);
	if (periph == NULL)
	{
		return SE_ERR_HANDLE_INVALID; 
	}

	setSelectedDriver(driver);
	setSelectedPeriph(periph);

	return SE_SUCCESS;
}



/**
* @brief 连接外设及获取外设ATR
* @param [out] out_buf	输出的起始地址
* @param [out] out_buf_len  输出数据长度的起始地址
* @return 参见error.h
* @note 1. 检查参数，加载选择外设的驱动 2. 根据加载的外设驱动，调用init 初始化函数和open函数
* @see 函数 getSelectedDriver getSelectedPeriph init open
*/

se_error_t api_connect (uint8_t *out_buf, uint32_t *out_buf_len)
{
	se_error_t ret_code = SE_SUCCESS;
	peripheral_bus_driver *driver = NULL;
	peripheral *periph = NULL;

	do
	{
		if (((out_buf == NULL) && (out_buf_len != NULL))||((out_buf != NULL) && (out_buf_len == NULL)))
		{
			ret_code = SE_ERR_PARAM_INVALID;
			break;
		}
	
		//1. 参数检查及加载驱动句柄
		if (g_drivers[0] == NULL || g_drivers[0]->periph[0] == NULL)
		{
			ret_code = SE_ERR_PARAM_INVALID; //未执行api_register 操作
			break;
		}
		
		if (getSelectedDriver() == NULL || getSelectedPeriph() == NULL) 
		{
			if (g_drivers[1] != NULL || g_drivers[0]->periph[1] != NULL)
			{
				ret_code = SE_ERR_NO_SELECT; //注册了多个外设，需先选择外设
				break;
			}
			//只注册了一个外设时, 无需选择，直接使用第1 个注册的外设
			driver = g_drivers[0];
			periph = g_drivers[0]->periph[0];
		}
		else
		{
			//执行了api_select 函数，选择了指定的设备
			driver = getSelectedDriver();
			periph = getSelectedPeriph();
		}

		//2.初始化设备
		ret_code = driver->init(periph);   
		if(ret_code != SE_SUCCESS)
		{
			break;
		}

		//3.打开设备
		ret_code = driver->open(periph, out_buf , out_buf_len); //打开设备,获取ATR
		if(ret_code != SE_SUCCESS)
		{
			break;
		}
	
	}while(0);
	
	return ret_code;
}


/**
* @brief 断开与外设的连接
* @param no
* @return 参见error.h
* @note no
* @see 函数 getSelectedDriver getSelectedPeriph deinit close
*/
se_error_t api_disconnect (void)
{
	peripheral_bus_driver *driver = NULL;
	peripheral *periph = NULL;

	se_error_t ret_code = SE_SUCCESS;

	do
	{
		if (g_drivers[0] == NULL || g_drivers[0]->periph[0] == NULL)
		{
			ret_code = SE_ERR_HANDLE_INVALID;//未执行acl_register 操作
			break;  
		}

		//1.加载驱动
		if (getSelectedDriver() == NULL || getSelectedPeriph() == NULL) 
		{
			//未执行acl_select 函数, 使用第1 个注册的设备
			driver = g_drivers[0];
			periph = g_drivers[0]->periph[0];
		}
		else
		{
			//执行了acl_select 函数，选择了指定的设备
			driver = getSelectedDriver();
			periph = getSelectedPeriph();
		}	

		//2.终止化设备
		ret_code = driver->deinit(periph);   
		if(ret_code != SE_SUCCESS)
		{
			break;
		}

		//3.关闭设备
		ret_code = driver->close(periph);   
		if(ret_code != SE_SUCCESS)
		{
			break;
		}
		
	}while(0);
	
	return ret_code;
}



/**
* @brief 以双向队列格式，向从设备发送命令及接收响应
* @param [in] in_buf	输入双向队列的起始地址
* @param [in] in_buf_len  输入双向队列的输入数据长度
* @param [out] out_buf	输出双向队列的起始地址
* @param [out] out_buf_len  输出双向队列的输出数据长度的起始地址
* @return 参见error.h
* @note 1.检查参数有效性 2.加载总线驱动句柄和设备句柄 3.通过加载的句柄，调用proto的transceive函数，发送数据及接收数据响应
* @see 函数 getSelectedDriver getSelectedPeriph transceive
*/
se_error_t api_transceive_queue(uint8_t *in_buf, uint32_t in_buf_len, uint8_t *out_buf, uint32_t *out_buf_len)
{
	se_error_t ret_code = SE_SUCCESS;
	peripheral_bus_driver *driver = NULL;
	peripheral *periph = NULL;

	do
	{
		//检查参数
		if ((in_buf == NULL)||(in_buf_len == 0U) || (out_buf == NULL)||(out_buf_len==NULL))
		{
			ret_code = SE_ERR_PARAM_INVALID;
			break;
		}
		
		if (g_drivers[0] == NULL || g_drivers[0]->periph[0] == NULL)
		{
			ret_code = SE_ERR_HANDLE_INVALID;//未执行acl_register 操作
			break;  
		}

		//1.加载驱动
		if (getSelectedDriver() == NULL || getSelectedPeriph() == NULL) 
		{
			//未执行acl_select 函数, 使用第1 个注册的设备
			driver = g_drivers[0];
			periph = g_drivers[0]->periph[0];
		}
		else
		{
			//执行了acl_select 函数，选择了指定的设备
			driver = getSelectedDriver();
			periph = getSelectedPeriph();
		}	

		//---调用proto中的proto_spi_transceive或proto_i2c_transceive, 
		//向选择的设备发送命令，并接收命令响应数据
		ret_code = driver->transceive(periph, in_buf, in_buf_len, out_buf, out_buf_len); 
		if(ret_code != SE_SUCCESS)
		{
			break;
		}

	}while(0);	
	
	return ret_code;
}



/**
* @brief SE命令传输具体实现
* @param [in] in_buf	输入数据的起始地址
* @param [in] in_buf_len  输入数据长度
* @param [out] out_buf	输出的起始地址
* @param [out] out_buf_len  输出数据长度的起始地址
* @return 参见error.h
* @note 1.输入存储存档到输入双向队列 2.用api_transceive_queue 函数，以双向队列格式发送和接收数据 3.从输出双向队列取出响应数据
* @see 函数 util_queue_init util_queue_rear_push api_transceive_queue
*/
se_error_t api_transceive(const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *out_buf, uint32_t *out_buf_len)
{
	se_error_t ret_code = SE_SUCCESS;
	uint32_t out_len = 0;
	uint16_t front_node = 0;

	double_queue_node queue_in ={0} ;
	double_queue_node queue_out ={0} ;

	do
	{
		//检查参数
		if ((in_buf == NULL) || (out_buf == NULL)||(out_buf_len==NULL))
		{
			ret_code = SE_ERR_PARAM_INVALID;
			break;
		}

		if ((in_buf_len<COMM_DATA_LEN_MIN)||(in_buf_len>COMM_DATA_LEN_MAX))
		{
			ret_code = SE_ERR_PARAM_INVALID;
			break;
		}

		//双端队列初始化
		util_queue_init(&queue_in);
		util_queue_init(&queue_out);

		//输入数据存入双端队列
		util_queue_rear_push((uint8_t *)in_buf,in_buf_len, &queue_in);

		//将输入双向队列中的数据按协议格式发送给设备，并将响应数据存储到输出双向队列	
		ret_code = api_transceive_queue((uint8_t *)&queue_in, util_queue_size(&queue_in), (uint8_t *)&queue_out, &out_len);
		if(ret_code != SE_SUCCESS)
		{
			return ret_code;
		}

		//从双端队列拷贝回输出数据
		front_node = queue_out.front_node;
		memcpy(out_buf,&queue_out.q_buf[front_node],queue_out.q_buf_len);
		*out_buf_len = queue_out.q_buf_len;

	}while(0);

	return ret_code;
}



/**
* @brief 复位外设及获取外设ATR
* @param [out] out_buf	输出的起始地址
* @param [out] out_buf_len  输出数据长度的起始地址
* @return 参见error.h
* @note 1.检查参数，加载选择外设的驱动，且未选择此外设，加载第一个注册的设备 2.根据加载的外设驱动，调用reset复位函数
* @see 函数 getSelectedDriver getSelectedPeriph reset
*/
se_error_t api_reset (uint8_t *out_buf, uint32_t *out_buf_len)
{
	se_error_t ret_code = SE_SUCCESS;
	peripheral_bus_driver *driver = NULL;
	peripheral *periph = NULL;

	do
	{
		if (((out_buf == NULL) && (out_buf_len != NULL))||((out_buf != NULL) && (out_buf_len == NULL)))
		{
			ret_code = SE_ERR_PARAM_INVALID;
			break;
		}

		//1. 参数检查及加载驱动句柄
		if (g_drivers[0] == NULL || g_drivers[0]->periph[0] == NULL)
		{
			ret_code = SE_ERR_PARAM_INVALID; //未执行acl_register 操作
			break;
		}
		
		if (getSelectedDriver() == NULL || getSelectedPeriph() == NULL) 
		{
			if (g_drivers[1] != NULL || g_drivers[0]->periph[1] != NULL)
			{
				ret_code = SE_ERR_NO_SELECT; //注册了多个外设，需先选择外设
				break;
			}
			//只注册了一个外设时, 无需选择，直接使用第1 个注册的外设
			driver = g_drivers[0];
			periph = g_drivers[0]->periph[0];
		}
		else
		{
			//执行了acl_select 函数，选择了指定的外设
			driver = getSelectedDriver();
			periph = getSelectedPeriph();
		}


		//2.复位外设
		ret_code = driver->reset(periph, out_buf , out_buf_len); //复位外设,获取ATR
		if(ret_code != SE_SUCCESS)
		{
			break;
		}

	}while(0);

	return ret_code;
}




/**
* @brief 微妙级延时接口
* @param [in] us	延时输入值，单位为微秒
* @return 参见error.h
* @see 函数 getSelectedDriver getSelectedPeriph delay 
*/
se_error_t api_delay (uint32_t us)
{
	se_error_t ret_code = SE_SUCCESS;
	peripheral_bus_driver *driver = NULL;
	peripheral *periph = NULL;

	do
	{
		if (us == 0)
		{
			ret_code = SE_ERR_PARAM_INVALID;
			break;
		}

		//1. 参数检查及加载驱动句柄
		if (g_drivers[0] == NULL || g_drivers[0]->periph[0] == NULL)
		{
			ret_code = SE_ERR_PARAM_INVALID; //未执行acl_register 操作
			break;
		}
		
		if (getSelectedDriver() == NULL || getSelectedPeriph() == NULL) 
		{
			if (g_drivers[1] != NULL || g_drivers[0]->periph[1] != NULL)
			{
				ret_code = SE_ERR_NO_SELECT; //注册了多个外设，需先选择外设
				break;
			}
			//只注册了一个外设时, 无需选择，直接使用第1 个注册的外设
			driver = g_drivers[0];
			periph = g_drivers[0]->periph[0];
		}
		else
		{
			//执行了acl_select 函数，选择了指定的外设
			driver = getSelectedDriver();
			periph = getSelectedPeriph();
		}


		//2.延时指定的微秒
		ret_code = driver->delay(periph, us); //延迟指定微秒数
		if(ret_code != SE_SUCCESS)
		{
			break;
		}

	}while(0);

	return ret_code;
}


/**
* @brief 发送控制命令，用于底层硬控制操作
* @param [in] ctrlcode	命令控制码
* @param [in] in_buf  输入的起始地址
* @param [in] in_buf_len  输入数据长度
* @param [out] out_buf	输出的起始地址
* @param [out] out_buf_len  输出数据长度的起始地址
* @return 参见error.h
* @note 1. 检查参数，加载选择外设的驱动 2. 根据加载的外设驱动，调用control控制函数
* @see 函数 getSelectedDriver getSelectedPeriph control
*/
se_error_t _api_control(uint32_t ctrlcode, uint8_t *in_buf, uint32_t in_buf_len, uint8_t *out_buf, uint32_t *out_buf_len)
{
	se_error_t retCode = SE_SUCCESS;

	peripheral_bus_driver *driver = NULL;
	peripheral *periph = NULL;

	if (g_drivers[0] == NULL || g_drivers[0]->periph[0] == NULL)
	{
		return SE_ERR_HANDLE_INVALID;  //未执行acl_register 操作
	}

	if (getSelectedDriver() == NULL || getSelectedPeriph() == NULL) 
	{
		//未执行acl_select 函数, 使用第1 个注册的设备
		driver = g_drivers[0];
		periph = g_drivers[0]->periph[0];
	}
	else
	{
		//执行了acl_select 函数，选择了指定的设备
		driver = getSelectedDriver();
		periph = getSelectedPeriph();
	}

	
	//向选择的设备发送控制命令，并接收控制命令的响应数据
	retCode = driver->control(periph, ctrlcode, in_buf, in_buf_len, out_buf, out_buf_len); 

	return retCode;
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
