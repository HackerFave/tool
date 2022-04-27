/**@file  fs.c
* @brief  fs interface definition
* @author liangww
* @date  2021-05-12
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/

#include "fs.h"

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
* @brief 选择文件
* @param [in] fid	EF文件FID
* @return 参见error.h
* @note no
* @see 函数apdu_select_file
*/
se_error_t  api_select_file(uint32_t fid)
{
	se_error_t ret = 0;
	//调用cmd层apdu_select_file接口
	ret = apdu_select_file(fid);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_select_file!\n");
		return ret;
	}

	return SE_SUCCESS;

}
/**
* @brief 写文件
* @param [in] offset  偏移信息
* @param [in] in_buf  输入数据
* @param [in] in_buf_len  输入数据长度
* @param [in] if_encrypt  是否采用传输密钥加密
* @return 参见error.h
* @note no
* @see 函数apdu_write_file
*/
se_error_t api_write_file (uint32_t offset, const uint8_t *in_buf, uint32_t in_buf_len, bool if_encrypt)
{
	se_error_t ret = 0;
	//参数检查
	if(in_buf==NULL)
	{  
		LOGE("failed to api_write_file input params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(in_buf_len>(DBLOCK+16))
	{  
		LOGE("failed to api_write_file input params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	//调用cmd层apdu_write_file接口
	ret = apdu_write_file (offset, if_encrypt, in_buf, in_buf_len);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_write_file!\n");
		return ret;
	}

	return SE_SUCCESS;
}

/**
* @brief 读文件
* @param [in] offset  偏移信息
* @param [in] expect_len  期望返回长度
* @param [in] if_encrypt  是否采用传输密钥加密
* @param [out] out_buf  读出数据长度
* @param [out] out_buf_len  读出数据长度
* @return 参见error.h
* @note no
* @see 函数apdu_read_file
*/
se_error_t api_read_file  (uint32_t offset, uint32_t expect_len , bool if_encrypt,uint8_t *out_buf, uint32_t *out_buf_len)
{
	se_error_t ret = 0;
	//参数检查
	if(expect_len>(LE_MAX_LEN-16))
	{  
		LOGE("failed to api_read_file input params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	
	//调用cmd层apdu_read_file接口
	ret = apdu_read_file  (offset, if_encrypt, expect_len, out_buf, out_buf_len);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_read_file!\n");
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

