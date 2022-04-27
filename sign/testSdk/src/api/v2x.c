/**@file  v2x.c
* @brief  extern v2x interface definition
* @author  liangww
* @date  2021-10-21
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "v2x.h"


/** @addtogroup SE_Service
  * @{
  */

/** @addtogroup API 
  * @brief API layer.
  * @{
  */


/** @defgroup V2X V2X
  * @brief v2x interface api.
  * @{
  */



/* Exported functions --------------------------------------------------------*/

/** @defgroup V2X_Exported_Functions V2X Exported Functions
  * @{
  */

//V2x接口使用说明
//1、设备端调用v2x_gen_key_derive_seed接口生成密钥衍生种子，将(kS, kE, A, P)随EeRaCertRequest发送至PRA。
//2、设备端接收到SCTij之后，使用设备端的PCA证书公钥验证签名。
//3、若验签成功，则调用v2x_reconsitution_key接口获得私钥sij，并将该私钥存入SE指定kid，最终输出假名证书。

/**
* @brief 产生密钥衍生种子
* -# 判断参数的合法性
* -# 调用命令层apdu_v2x_gen_key_derive_seed接口
* -# 以上过程中发生错误会返回相应错误码
* @param [in]  derive_type    衍生类型，预留，默认0x00
* @param [out] out_buf    衍生数据信息，参见derive_seed_t
* @return 参见 error.h
* @note no
* @see 函数apdu_gen_key_derive_seed
*/
se_error_t v2x_gen_key_derive_seed (uint8_t derive_type,derive_seed_t* out_buf)
{
	se_error_t ret = 0;
	
	if(out_buf==NULL)
      return SE_ERR_PARAM_INVALID;

	//调用命令层apdu_v2x_gen_key_derive_seed接口
    ret =  apdu_v2x_gen_key_derive_seed(0x00, out_buf);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_v2x_gen_key_derive_seed!\n");
		return ret;
	}

	return SE_SUCCESS;	

}


/**
* @brief 重构私钥
* -# 判断参数的合法性
* -# 调用命令层apdu_v2x_reconsitution_key接口
* -# 以上过程中发生错误会返回相应错误码
* @param [in]  in_buf      重构数据信息(重构私钥ID-1B|i-4B｜j-4B|CTij)
* @param [in]  in_buf_len  重构数据信息长度
* @param [out] out_buf     假名证书
* @param [out] out_buf_len 假名证书长度
* @return 参见 error.h
* @note 必须先执行v2x_gen_key_derive_seed
* @see 函数apdu_reconsitution_key
*/
se_error_t v2x_reconsitution_key (uint8_t *in_buf, uint32_t *in_buf_len,uint8_t *out_buf, uint32_t *out_buf_len)
{
	se_error_t ret = 0;

	if(in_buf==NULL||*in_buf_len==0||out_buf==NULL||out_buf_len==NULL)
        return SE_ERR_PARAM_INVALID;

	
	//调用命令层apdu_v2x_reconsitution_key接口
    ret =  apdu_v2x_reconsitution_key(in_buf, in_buf_len, out_buf, out_buf_len);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_v2x_reconsitution_key!\n");
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
 
