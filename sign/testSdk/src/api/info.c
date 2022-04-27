/**@file  info.c
* @brief  info interface definition
* @author liangww
* @date  2021-05-12
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/

#include "info.h"

/** @addtogroup SE_Service
  * @{
  */

/** @addtogroup API 
  * @brief API layer.
  * @{
  */

/** @defgroup INFO INFO
  * @brief info interface api.
  * @{
  */



/* Exported functions --------------------------------------------------------*/

/** @defgroup INFO_Exported_Functions INFO Exported Functions
  * @{
  */


/**
* @brief 获取SE信息
* @param [in] type	信息类型(CHIP_ID/PRODUCT_INFO)
* @param [out] info->CHIP_ID  芯片8字节唯一序列号
* @param [out] info->PRODUCT_INFO   产品信息8字节
* @return 参见error.h
* @note no
* @see 函数apdu_get_info
*/
se_error_t  api_get_info (info_type type, se_info_t * info)
{
	se_error_t ret = 0;
	//参数检查
	if(info==NULL)
	{  
		LOGE("failed to api_get_info pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(type!=CHIP_ID&&type!=PRODUCT_INFO)
	{  
		LOGE("failed to api_get_info input params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	//调用cmd层apdu_get_info接口
	ret = apdu_get_info (type, info);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_get_info!\n");
		return ret;
	}

	return SE_SUCCESS;

}

/**
* @brief 获取SE ID
* @param [out] se_id->val  返回的SE的ID
* @param [out] se_id->val_len  返回的SE的ID长度
* @return 参见error.h
* @note no
* @see 函数apdu_get_id
*/
se_error_t  api_get_id (se_id_t *se_id ) 
{
	se_error_t ret = 0;
	//参数检查
	if(se_id==NULL)
	{  
		LOGE("failed to apdu_get_id pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	//调用cmd层apdu_get_id接口
	ret = apdu_get_id (se_id ) ;
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_get_id!\n");
		return ret;
	}
	return SE_SUCCESS;
}


/**
* @brief 写入SE ID
* -# 判断参数的合法性
* -# 调用命令层apdu_write_SEID接口
* -# 以上过程中发生错误会返回相应错误码
* @param [in] se_id->val  写入的SE的ID
* @param [in] se_id->val_len  写入的SE的ID长度
* @return 参见error.h
* @note 写入SEID之前需要获得设备权限
* @see 函数apdu_write_id
*/
se_error_t  api_write_id (se_id_t *se_id)
{
	se_error_t ret = 0;

	//参数检查
	if((se_id == NULL)||(se_id->val_len  == 0))
	{
		LOGE("failed to api_write_id params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	
	//调用cmd层apdu_write_SEID接口
	ret = apdu_write_SEID (se_id);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_write_SEID!\n");
		return ret;
	}

	return SE_SUCCESS;

}



/**
* @brief 获取SDK 4字节整型版本号
* @param [out] num  版本号
* @return 参见error.h
* @note no
* @see no
*/
se_error_t  api_sdk_version_number(uint32_t *num)
{

	//参数检查
	if(num==NULL)
	{  
		LOGE("failed to api_sdk_version_number pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	*num = SDK_VERSION_NUM;

	return SE_SUCCESS;
}

/**
* @brief 获取SDK字符串类型版本号
* @param [out] str  版本号
* @return 参见error.h
* @note no
* @see no
*/
se_error_t  api_sdk_version_string (char *str)
{

	//参数检查
	if(str==NULL)
	{  
		LOGE("failed to api_sdk_version_string pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	strcpy(str,SDK_VERSION_STRING);

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
