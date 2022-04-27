/**@file  fs.c
* @brief  fs interface definition
* @author liangww
* @date  2021-05-11
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/

#include "pin.h"

/** @addtogroup SE_Service
  * @{
  */

/** @addtogroup API 
  * @brief API layer.
  * @{
  */

/** @defgroup FS FS
  * @brief fs interface api.
  * @{
  */



/* Exported functions --------------------------------------------------------*/

/** @defgroup FS_Exported_Functions FS Exported Functions
  * @{
  */


/**
* @brief 接口用于写入PIN
* @param [in] pin->owner	PIN属主类型
* @param [in] pin->pin_value[16]  pin的值
* @param [in] pin->pin_len  取值为0x06-0x10
* @param [in] if_encrypt  是否利用传输密钥加密传输
* @return 参见error.h
* @note 需要获得安全文件的写权限
* @see 函数 apdu_write_key
*/ 
se_error_t  api_write_pin(pin_t *pin, bool if_encrypt)
{
	se_error_t ret = 0;
	uint8_t inbuf[24]={0};
	uint32_t inbuf_len = 0;
	
    //参数检查
	if(pin==NULL)
	{  
		LOGE("failed to api_write_pin input pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(pin->owner!=ADMIN_PIN&&pin->owner!=USER_PIN)
		{  
			LOGE("failed to api_write_pin pin owner params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	if(pin->pin_value==NULL)
		{  
			LOGE("failed to api_write_pin pin_value params!\n");
			return SE_ERR_PARAM_INVALID;
		}
   if(pin->pin_len<6||pin->pin_len>16)
		{  
			LOGE("failed to api_write_pin pin_len params!\n");
			return SE_ERR_PARAM_INVALID;
		}

   	inbuf_len = pin->pin_len+8;
	//添加密钥属性信息
	inbuf[0]=0x00;//密钥用途
	if(pin->owner==ADMIN_PIN)//密钥索(管理员PIN：‘00’;用户PIN：‘01’ )
	{
		inbuf[1]=0x00;
	}
	else if(pin->owner==USER_PIN)
	{
		inbuf[1]=0x01;
	}
	inbuf[2] = pin->limit;//pin验证次数限制
	inbuf[6]=0x00;//长度：高字节
	inbuf[7]=pin->pin_len;//长度：低字节
	memcpy(inbuf+8,pin->pin_value,pin->pin_len);
	//调用cmd层apdu_write_key接口
	ret = apdu_write_key(inbuf,inbuf_len,if_encrypt,true,false);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_write_key!\n");
		return ret;
	}

	return SE_SUCCESS;

}
/**
* @brief 接口用于修改管理员PIN和用户PIN
* @param [in] pin->owner  pin属主类型
* @param [in] pin->pin_value  旧pin的值
* @param [in] pin->pin_len  取值为0x06-0x10
* @param [in] in_buf  新的pin值
* @param [in] in_buf_len  新pin长度
* @return 参见error.h
* @note no
* @see 函数 apdu_change_reload_pin
*/ 
se_error_t api_change_pin (pin_t *pin, const uint8_t *in_buf, uint32_t in_buf_len)
{
	se_error_t ret = 0;
	//参数检查
	if(pin==NULL||in_buf==NULL)
	{  
		LOGE("failed to api_change_pin input pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(pin->owner!=ADMIN_PIN&&pin->owner!=USER_PIN)
	{  
		LOGE("failed to api_change_pin pin owner params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(pin->pin_value==NULL)
		{  
			LOGE("failed to api_change_pin pin_value params!\n");
			return SE_ERR_PARAM_INVALID;
		}
    if(pin->pin_len<6||pin->pin_len>16||in_buf_len<6||in_buf_len>16)
		{  
			LOGE("failed to api_change_pin pin_len params!\n");
			return SE_ERR_PARAM_INVALID;
		}

	//调用cmd层apdu_change_reload_pin接口
	ret = apdu_change_reload_pin (pin, in_buf, in_buf_len, true);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_change_reload_pin!\n");
		return ret;
	}

	return SE_SUCCESS;
}

/**
* @brief 用于重置用户PIN
* @param [in] pin->pin_value  管理员pin的值
* @param [in] pin->pin_len   取值为0x06-0x10
* @param [in] in_buf  用户pin新的值
* @param [in] in_buf_len  输入数据长度
* @return 参见error.h
* @note no
* @see 函数 apdu_change_reload_pin
*/
se_error_t api_reload_pin  (pin_t *pin, const uint8_t *in_buf, uint32_t in_buf_len)
{
	se_error_t ret = 0;
	//参数检查
	if(pin==NULL||in_buf==NULL)
	{  
		LOGE("failed to api_reload_pin input pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(pin->pin_value==NULL)
		{  
			LOGE("failed to api_reload_pin pin_value params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	if(pin->pin_len<6||pin->pin_len>16||in_buf_len<6||in_buf_len>16)
		{  
			LOGE("failed to api_reload_pin pin_len params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	//调用cmd层apdu_change_reload_pin接口
	ret = apdu_change_reload_pin (pin, in_buf, in_buf_len, false);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_change_reload_pin!\n");
		return ret;
	}

	return SE_SUCCESS;

}


/**
* @brief 用于验证PIN
* @param [in] pin->owner  pin的属主类型
* @param [in] pin->pin_value  待验证pin的值
* @param [in] pin->pin_len   取值为0x06-0x10
* @return 参见error.h
* @note no
* @see 函数 apdu_verify_pin
*/
se_error_t api_verify_pin  (pin_t *pin)
{
	se_error_t ret = 0;
	//参数检查
	if(pin==NULL)
	{  
		LOGE("failed to api_verify_pin input pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(pin->pin_value==NULL)
		{  
			LOGE("failed to api_verify_pin pin_value params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	if(pin->pin_len<6||pin->pin_len>16)
		{  
			LOGE("failed to api_verify_pin pin_len params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	//调用cmd层apdu_verify_pin接口
	ret = apdu_verify_pin (pin);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_verify_pin!\n");
		return ret;
	}

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

