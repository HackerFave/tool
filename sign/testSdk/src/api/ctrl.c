/**@file  ctrl.c
* @brief  ctrl interface definition
* @author  liuch
* @author  liangww
* @date  2021-04-28
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "ctrl.h"


/** @addtogroup SE_Service
  * @{
  */

/** @addtogroup API 
  * @brief API layer.
  * @{
  */

/** @defgroup CTRL CTRL
  * @brief ctrl interface api.
  * @{
  */



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/** @defgroup CTRL_Exported_Functions CTRL Exported Functions
  * @{
  */




/**
* @brief 切换工作模式应用
* @param [in] type	工作模式
* @return 参见error.h
* @note no
* @see 函数 apdu_switch_mode
*/
se_error_t api_switch_mode (work_mode  type)
{
	se_error_t ret = 0;
	//参数检查
	if(type!=STANDBY)
	{
		LOGE("failed to api_switch_mode input params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	//调用cmd层apdu_switch_mode接口
	ret = apdu_switch_mode (type);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_switch_mode!\n");
		return ret;
	}

	return SE_SUCCESS;

}

/**
* @brief 命令控制应用
* @param [in] ctrlcode	控制类型
* @param [in] in_buf  输入数据
* @param [in] in_buf_len 输入数据长度
* @param [out] out_buf  输出数据
* @param [out] out_buf_len 输出数据长度
* @return 参见error.h
* @note no
* @see 函数 _api_control  apdu_control
*/
se_error_t api_control(ctrl_type ctrlcode, const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *out_buf, uint32_t *out_buf_len)
{
	se_error_t ret = SE_SUCCESS;
	//参数检查	
	if(in_buf==NULL||in_buf_len==0)
	{
		LOGE("failed to api_control input params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(out_buf==NULL||out_buf_len==NULL)
	{
		LOGE("failed to api_control output params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(ctrlcode & 0x10)
	{
		ret = _api_control((uint32_t)ctrlcode, (uint8_t *)in_buf, in_buf_len,out_buf,out_buf_len);
		if(ret!=SE_SUCCESS)
		{	
			LOGE("failed to call _api_control !\n");
			return ret;
		}
	}
	else if(ctrlcode & 0x20)
	{
		//调用cmd层apdu_control接口
		ret = apdu_control(ctrlcode, in_buf, in_buf_len,out_buf,out_buf_len);
		if(ret!=SE_SUCCESS)
		{	
			LOGE("failed to call apdu_control!\n");
			return ret;
		}
	}
	else
	{
		ret = SE_ERR_PARAM_INVALID;
		LOGE("failed to api_control input params!\n");
	}

	return ret;
}


/**
* @brief SE上电
* @param 无
* @return 参见error.h
* @note no
* @see 函数 port_power_on
*/
se_error_t api_power_on (void)
{
	//port_power_on();
	
	return SE_SUCCESS;
}

/**
* @brief SE下电
* @param 无
* @return 参见error.h
* @note no
* @see 函数 port_power_off
*/
se_error_t api_power_off (void)
{
	//port_power_off();
	
	return SE_SUCCESS;
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


