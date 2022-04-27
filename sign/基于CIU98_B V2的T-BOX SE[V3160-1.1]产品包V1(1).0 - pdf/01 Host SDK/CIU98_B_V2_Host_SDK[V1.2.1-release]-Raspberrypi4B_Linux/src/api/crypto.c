/**@file  crypto.c
* @brief  crypto interface definition
* @author liuch
* @date  2020-06-02
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "crypto.h"


/** @addtogroup SE_Service
  * @{
  */

/** @addtogroup API
  * @{
  */


/** @defgroup CPYPTO CPYPTO
  * @brief cpypto interface api.
  * @{
  */


/* Exported functions --------------------------------------------------------*/
/** @defgroup CPYPTO_Exported_Functions CPYPTO Exported Functions
  * @{
  */


/**
* @brief 对称加密
* @param [in] key->alg  对称算法类型(ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] key->id  密钥id值
* @param [in] sym_param->mode  对称加密模式
* @param [in] sym_param->iv  初始向量
* @param [in] sym_param->iv_len  初始向量长度
* @param [in] sym_param->padding_type 补位方式(PADDING_NOPADDING/PADDING_PKCS7)
* @param [in] in_buf  明文数据 
* @param [in] in_buf_len  明文数据长度
* @param [out] out_buf  密文数据
* @param [out] out_buf_len  密文数据长度
* @return 参见error.h
* @note no
* @see 函数apdu_sym_enc_dec
*/
se_error_t api_sym_encrypt (sym_key_t *key, alg_sym_param_t *sym_param, const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *out_buf, uint32_t *out_buf_len)
{

    se_error_t ret = 0;
	uint32_t block = DBLOCK;
	uint32_t count = 0;
	uint32_t padding_len = in_buf_len;
	uint32_t temp_len = in_buf_len;
	uint32_t seg_len = 0;
	uint32_t off = 0;
	uint32_t out_len = 0;
	bool if_last_block =false;
	bool if_first_block =true;
	//参数检查
	if(key==NULL||sym_param==NULL||in_buf==NULL||in_buf_len==0)
	{  
		LOGE("failed to api_sym_encrypt input params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(out_buf==NULL||out_buf_len==NULL)
	{  
		LOGE("failed to api_sym_encrypt output params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(key->alg!=ALG_AES128&&key->alg!=ALG_DES128&&key->alg!=ALG_SM4)
	{  
		LOGE("failed to api_sym_encrypt alg params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(sym_param->mode!=SYM_MODE_CBC&&sym_param->mode!=SYM_MODE_ECB)
	{  
		LOGE("failed to api_sym_encrypt mode params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(sym_param->mode==SYM_MODE_CBC)
	{
		if(key->alg==ALG_DES128)
		{
			if(sym_param->iv_len!=8)
            {
				LOGE("failed to api_sym_encrypt iv params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
		else
		{
			if(sym_param->iv_len!=16)
            {
				LOGE("failed to api_sym_encrypt iv params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
	}
	if(sym_param->padding_type!=PADDING_NOPADDING&&sym_param->padding_type!=PADDING_PKCS7)
	{  
		LOGE("failed to api_sym_encrypt padding params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(sym_param->padding_type==PADDING_NOPADDING)
	{
		if(key->alg==ALG_DES128)
		{
			if(in_buf_len%8!=0)
			{
				LOGE("failed to api_sym_encrypt input len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
		else
		{
			if(in_buf_len%16!=0)
			{
				LOGE("failed to api_sym_encrypt input len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
	}
	else//pkcs7计算补位后的长度
	{
		padding_len =  apdu_sym_padding_length(key->alg,PADDING_PKCS7,in_buf_len);
	}

	//判断个数，最少是1块，按照block 992字节进行分包，count为分包个数
	count = temp_len/block+1;
	if(temp_len%block==0)
		count--;

	//循环发送数据
	while(count>0)
	{			
		
		if(temp_len<=block)
		{
			seg_len = temp_len;
		}			
		else
			seg_len = block;
		//只有1包或者最后1包
		if(count==1)
		{
		    //判断为最后一包if_last_block置为true
			if_last_block = true;
			//最后一包若pkcs7计算补位长度，输入到out_buf_len中
			if(sym_param->padding_type==PADDING_PKCS7)
				out_len =  apdu_sym_padding_length(key->alg,PADDING_PKCS7,seg_len);
		}
		ret = apdu_sym_enc_dec (key,sym_param,in_buf+off, seg_len, out_buf+off, &out_len,if_first_block,if_last_block,true);
		if(ret!=SE_SUCCESS)
		{	
			LOGE("failed to call apdu_sym_enc_dec!\n");
			return ret;
		}
		if_first_block =false;
		off+=seg_len;
		count--;
		temp_len-=seg_len;
	}
	//加密数据长度
	*out_buf_len = padding_len;
	return SE_SUCCESS;
}


/**
* @brief 对称解密
* @param [in] key->alg  对称算法类型(ALG_AES128/ALG_DES128/ALG_SM1/ALG_SM4)
* @param [in] key->id  密钥id值
* @param [in] sym_param->mode  对称加密模式
* @param [in] sym_param->iv  初始向量
* @param [in] sym_param->iv_len  初始向量长度
* @param [in] sym_param->padding_type 补位方式(PADDING_NOPADDING/PADDING_PKCS7)
* @param [in] in_buf  密文数据 
* @param [in] in_buf_len  密文数据长度
* @param [out] out_buf  明文数据
* @param [out] out_buf_len  明文数据长度
* @return 参见error.h
* @note no
* @see 函数apdu_sym_enc_dec
*/
se_error_t api_sym_decrypt (sym_key_t *key, alg_sym_param_t *sym_param, const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *out_buf, uint32_t *out_buf_len)
{
	se_error_t ret = 0;
	uint32_t block = DBLOCK;
	uint32_t count = 0;
	uint32_t padding_len = 0;
	uint32_t temp_len = in_buf_len;
	uint32_t seg_len = 0;
	uint32_t off = 0;
	uint32_t out_len = 0;
	bool if_last_block =false;
	bool if_first_block =true;
	//参数检查
	if(key==NULL||sym_param==NULL||in_buf==NULL||in_buf_len==0)
	{  
		LOGE("failed to api_sym_decrypt input params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(out_buf==NULL||out_buf_len==NULL)
	{  
		LOGE("failed to api_sym_decrypt output params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(key->alg!=ALG_AES128&&key->alg!=ALG_DES128&&key->alg!=ALG_SM4)
	{  
		LOGE("failed to api_sym_decrypt alg params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(sym_param->mode!=SYM_MODE_CBC&&sym_param->mode!=SYM_MODE_ECB)
	{  
		LOGE("failed to api_sym_decrypt mode params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(sym_param->mode==SYM_MODE_CBC)
	{
		if(key->alg==ALG_DES128)
		{
			if(sym_param->iv_len!=8)
            {
				LOGE("failed to api_sym_decrypt iv params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
		else
		{
			if(sym_param->iv_len!=16)
            {
				LOGE("failed to api_sym_decrypt iv params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
	}
	if(sym_param->padding_type!=PADDING_NOPADDING&&sym_param->padding_type!=PADDING_PKCS7)
	{  
		LOGE("failed to api_sym_decrypt padding params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(sym_param->padding_type==PADDING_NOPADDING)
	{
		if(key->alg==ALG_DES128)
		{
			if(in_buf_len%8!=0)
			{
				LOGE("failed to api_sym_decrypt input len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
		else
		{
			if(in_buf_len%16!=0)
			{
				LOGE("failed to api_sym_decrypt input len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
	}
	//判断个数，最少是1块
	count = temp_len/block+1;

	if(temp_len%block==0)
		count--;

	//循环发送数据
	while(count>0)
	{				
		if(temp_len<=block)
		{
			seg_len = temp_len;
		}			
		else
			seg_len = block;
		//只有1包或者最后1包
		if(count==1)
		{
			//判断为最后一包if_last_block置为true
			if_last_block = true;
		}
		ret = apdu_sym_enc_dec (key,sym_param,in_buf+off, seg_len, out_buf+off, &out_len,if_first_block,if_last_block,false);
		if(ret!=SE_SUCCESS)
		{	
			LOGE("failed to call apdu_sym_enc_dec!\n");
			return ret;
		}
		if_first_block =false;
		off+=seg_len;
		count--;
		temp_len-=seg_len;
		padding_len += out_len;
	}
	*out_buf_len = padding_len;

	return SE_SUCCESS;
}


/**
* @brief MAC计算
* @param [in] key->alg  对称算法类型(ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] key->id  密钥id值
* @param [in] sym_param->iv  初始向量
* @param [in] sym_param->iv_len  初始向量长度
* @param [in] sym_param->padding_type 补位方式(PADDING_NOPADDING/PADDING_ISO9797_M1/PADDING_ISO9797_M2)
* @param [in] in_buf  MAC计算数据 
* @param [in] in_buf_len  MAC计算数据长度
* @param [out] out_buf  MAC数据
* @param [out] out_buf_len  MAC数据长度
* @return 参见error.h
* @note no
* @see 函数apdu_mac
*/
se_error_t api_mac (sym_key_t *key,  alg_sym_param_t *sym_param,const uint8_t *in_buf,uint32_t in_buf_len, uint8_t *mac, uint32_t *mac_len)
{
	se_error_t ret = 0;
	uint32_t block = DBLOCK;
	uint32_t count = 0;
	uint32_t temp_len = in_buf_len;
	uint32_t seg_len = 0;
	uint32_t off = 0;
	//uint32_t iv_len = 0;
	bool if_last_block =false;
	bool if_first_block =true;

	//参数检查
	if(key==NULL||sym_param==NULL||in_buf==NULL||in_buf_len==0)
	{  
		LOGE("failed to api_mac input params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(mac==NULL||mac_len==NULL)
	{  
		LOGE("failed to api_mac output params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(key->alg!=ALG_AES128&&key->alg!=ALG_DES128&&key->alg!=ALG_SM4)
	{  
		LOGE("failed to api_mac alg params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(key->alg==ALG_DES128)
	{
		if(sym_param->iv_len!=8)
        {
			LOGE("failed to api_mac iv params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	}
	else
	{
		if(sym_param->iv_len!=16)
        {
			LOGE("failed to api_mac iv params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	}

	if(sym_param->padding_type!=PADDING_NOPADDING&&sym_param->padding_type!=PADDING_ISO9797_M1&&sym_param->padding_type!=PADDING_ISO9797_M2)
	{  
		LOGE("failed to api_mac padding params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(sym_param->padding_type==PADDING_NOPADDING)
	{
		if(key->alg==ALG_DES128)
		{
			if(in_buf_len%8!=0)
			{
				LOGE("failed to api_mac input len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
		else
		{
			if(in_buf_len%16!=0)
			{
				LOGE("failed to api_mac input len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
	}
	

	//判断个数，最少是1块
	count = temp_len/block+1;

	if(temp_len%block==0)
		count--;

	//循环发送数据
	while(count>0)
	{			
		
		if(temp_len<=block)
		{
			seg_len = temp_len;
		}			
		else
			seg_len = block;
		///只有1包或者最后1包
		if(count==1)
		{
			if_last_block = true;
		}
		
		ret = apdu_mac (key,sym_param,in_buf+off, seg_len, mac, mac_len,if_first_block,if_last_block,true);
		if(ret!=SE_SUCCESS)
		{	
			LOGE("failed to call apdu_mac!\n");
			return ret;
		}
		if_first_block =false;
		off+=seg_len;
		count--;
		temp_len-=seg_len;
	}

	return SE_SUCCESS;	

}




/**
* @brief MAC验证
* @param [in] key->alg  对称算法类型(ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] key->id  密钥id值
* @param [in] sym_param->iv  初始向量
* @param [in] sym_param->iv_len  初始向量长度
* @param [in] sym_param->padding_type 补位方式(PADDING_NOPADDING/PADDING_ISO9797_M1/PADDING_ISO9797_M2)
* @param [in] in_buf  MAC计算数据 
* @param [in] in_buf_len  MAC计算数据长度
* @param [in] mac  MAC数据
* @param [in] mac_len  MAC数据长度
* @return 参见error.h
* @note no
* @see 函数apdu_mac
*/
se_error_t  api_mac_verify (sym_key_t *key, alg_sym_param_t *sym_param,const uint8_t *in_buf,uint32_t in_buf_len,const uint8_t *mac, uint32_t mac_len)
{
	se_error_t ret = 0;
	uint32_t block = DBLOCK-16;//防止数据域最后补上mac值之后超过992字节，block提前减去16
	int32_t count = 0;
	uint32_t temp_len = in_buf_len;
	uint32_t seg_len = 0;
	uint32_t off = 0;
	//uint32_t iv_len = 0;
	bool if_last_block =false;
	bool if_first_block =true;

	//参数检查
	if(key==NULL||sym_param==NULL||in_buf==NULL||in_buf_len==0)
	{  
		LOGE("failed to api_mac_verify input params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(mac==NULL||mac_len==0)
	{  
		LOGE("failed to api_mac_verify output params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(key->alg!=ALG_AES128&&key->alg!=ALG_DES128&&key->alg!=ALG_SM4)
	{  
		LOGE("failed to api_mac_verify alg params!\n");
		return SE_ERR_PARAM_INVALID;
	}


	if(key->alg==ALG_DES128)
	{
		if(sym_param->iv_len!=8||mac_len!=8)
        {
			LOGE("failed to api_mac_verify iv params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	}
	else
	{
		if(sym_param->iv_len!=16||mac_len!=16)
        {
			LOGE("failed to api_mac_verify iv params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	}

	if(sym_param->padding_type!=PADDING_NOPADDING&&sym_param->padding_type!=PADDING_ISO9797_M1&&sym_param->padding_type!=PADDING_ISO9797_M2)
	{  
		LOGE("failed to api_mac_verify padding params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(sym_param->padding_type==PADDING_NOPADDING)
	{
		if(key->alg==ALG_DES128)
		{
			if(in_buf_len%8!=0)
			{
				LOGE("failed to api_mac_verify input len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
		else
		{
			if(in_buf_len%16!=0)
			{
				LOGE("failed to api_mac input len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
	}
	

	//判断个数，最少是1块
	count = temp_len/block+1;

	if(temp_len%block==0)
		count--;

	//循环发送数据
	while(count>0)
	{			
		
		if(temp_len<=block)
		{
			seg_len = temp_len;
		}			
		else
			seg_len = block;
		//只有1包或者最后1包
		if(count==1)
		{
			if_last_block = true;
		}
		ret = apdu_mac (key,sym_param,in_buf+off, seg_len, (uint8_t*)mac, &mac_len,if_first_block,if_last_block,false);
		if(ret!=SE_SUCCESS)
		{	
			LOGE("failed to call apdu_mac!\n");
			return ret;
		}
		if_first_block =false;
		off+=seg_len;
		count--;
		if(temp_len!=0)
		{
			temp_len-=seg_len;
		}
	}

	return SE_SUCCESS;	
}


/**
* @brief 非对称加密
* @param [in] key->alg  非对称算法类型(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256/ALG_SM2)
* @param [in] key->id  公钥id值
* @param [in] asym_param-> padding_type  补位方式(PADDING_NOPADDING/PADDING_PKCS1)(仅RSA算法时有效)
* @param [in] in_buf  明文数据 
* @param [in] in_buf_len  明文数据长度
* @param [out] out_buf  密文数据
* @param [out] out_buf_len  密文数据长度
* @return 参见error.h
* @note no
* @see 函数apdu_asym_enc_dec
*/
se_error_t api_asym_encrypt(pub_key_t *key, alg_asym_param_t *asym_param, const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *out_buf, uint32_t *out_buf_len)
{
 	se_error_t ret = 0;
	uint32_t block = DBLOCK+16;
	uint32_t count = 0;
	uint32_t temp_len = in_buf_len;
	uint32_t seg_len = 0;
	uint32_t off = 0;
	bool if_last_block =false;


	//参数检查
	if(key==NULL||in_buf==NULL||in_buf_len==0)
	{  
		LOGE("failed to api_asym_encrypt input params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(out_buf==NULL||out_buf_len==NULL)
	{  
		LOGE("failed to api_asym_encrypt output params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	
	if(key->alg!=ALG_ECC256_NIST&&key->alg!=ALG_SM2&&key->alg!=ALG_RSA1024_CRT&&key->alg!=ALG_RSA2048_CRT)
	{  
		LOGE("failed to api_asym_encrypt alg params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	//补位方式，PADDING_NOPADDING-外部传入模长 PADDING_PKCS1-内部进行补位
	if(key->alg==ALG_RSA1024_CRT||key->alg==ALG_RSA2048_CRT)
	{
		if(asym_param->padding_type!=PADDING_NOPADDING&&asym_param->padding_type!=PADDING_PKCS1)
		{
			LOGE("failed to api_asym_encrypt padding params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	}

	if(key->alg==ALG_ECC256_NIST||key->alg==ALG_SM2)
	{
		//SM2、ECC算法，输入数据不超过4096字节
		if(in_buf_len>4096)
		{
			LOGE("failed to api_asym_encrypt in_buf_len params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	}
	else 
	{
		if(asym_param->padding_type==PADDING_NOPADDING)
		{
			//RSA算法，输入数据为模长
			if(in_buf_len!=128&&in_buf_len!=256)
			{
				LOGE("failed to api_asym_encrypt in_buf_len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
		else
		{
			//RSA算法，输入数据为不超过（模长-11）
			if((key->alg==ALG_RSA2048_CRT&&asym_param->padding_type==PADDING_PKCS1&&in_buf_len>245)||(key->alg==ALG_RSA1024_CRT&&asym_param->padding_type==PADDING_PKCS1&&in_buf_len>117))
			{
				LOGE("failed to api_asym_encrypt in_buf_len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
	}


	//判断个数，最少是1块
	count = temp_len/block+1;

	if(temp_len%block==0)
		count--;

	//循环发送数据
	while(count>0)
	{			
			
		if(temp_len<=block)
		{
			seg_len = temp_len;
		}			
		else
			seg_len = block;
		//只有1包或者最后1包
		if(count==1)
		{
			if_last_block = true;
		}
		ret = apdu_asym_enc (key,asym_param, in_buf+off, seg_len, out_buf, out_buf_len,if_last_block);
		if(ret!=SE_SUCCESS)
		{	
			LOGE("failed to call api_asym_encrypt!\n");
			return ret;
		}
		off+=seg_len;
		count--;
		temp_len-=seg_len;
	}
	
	return SE_SUCCESS;	

}


/**
* @brief 非对称解密
* @param [in] key->alg  非对称算法类型(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256/ALG_SM2)
* @param [in] key->id  私钥id值
* @param [in] asym_param->padding_type  补位方式(PADDING_NOPADDING/PADDING_PKCS1)(仅RSA算法时有效)
* @param [in] in_buf  密文数据 
* @param [in] in_buf_len  密文数据长度
* @param [out] out_buf  明文数据
* @param [out] out_buf_len  明文数据长度
* @return 参见error.h
* @note no
* @see 函数apdu_asym_enc_dec
*/
se_error_t api_asym_decrypt(pri_key_t *key, alg_asym_param_t *asym_param ,const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *out_buf, uint32_t *out_buf_len)
{
	se_error_t ret = 0;
	uint32_t block = DBLOCK+16;
	uint32_t count = 0;
	uint32_t temp_len = in_buf_len;
	uint32_t seg_len = 0;
	uint32_t off = 0;
	bool if_last_block =false;


	//参数检查
	if(key==NULL||in_buf==NULL||in_buf_len==0)
	{  
		LOGE("failed to api_asym_decrypt input params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(out_buf==NULL||out_buf_len==NULL)
	{  
		LOGE("failed to api_asym_decrypt output params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	
	if(key->alg!=ALG_ECC256_NIST&&key->alg!=ALG_SM2&&key->alg!=ALG_RSA1024_CRT&&key->alg!=ALG_RSA2048_CRT)
	{  
		LOGE("failed to api_asym_decrypt alg params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	//补位方式，PADDING_NOPADDING-外部传入模长 PADDING_PKCS1-内部进行补位
	if(key->alg==ALG_RSA1024_CRT||key->alg==ALG_RSA2048_CRT)
	{
		if(asym_param->padding_type!=PADDING_NOPADDING&&asym_param->padding_type!=PADDING_PKCS1)
		{
			LOGE("failed to api_asym_decrypt padding params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	}

	if(key->alg==ALG_ECC256_NIST||key->alg==ALG_SM2)
	{
		//SM2、ECC算法，输入数据不超过4096字节
		if(in_buf_len>4096+96)
		{
			LOGE("failed to api_asym_decrypt in_buf_len params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	}
	else 
	{
		//RSA算法，输入数据为模长
		if(in_buf_len!=128&&in_buf_len!=256)
		{
			LOGE("failed to api_asym_decrypt in_buf_len params!\n");
			return SE_ERR_PARAM_INVALID;
		}

	}

	//判断个数，最少是1块
	count = temp_len/block+1;

	if(temp_len%block==0)
		count--;

	//循环发送数据
	while(count>0)
	{			
		
		if(temp_len<=block)
		{
			seg_len = temp_len;
		}			
		else
			seg_len = block;
		//只有1包或者最后1包
		if(count==1)
		{
			if_last_block = true;
		}
		
		ret = apdu_asym_dec (key,asym_param, in_buf+off, seg_len, out_buf, out_buf_len,if_last_block);
		if(ret!=SE_SUCCESS)
		{	
			LOGE("failed to call api_asym_decrypt!\n");
			return ret;
		}
		off+=seg_len;
		count--;
		temp_len-=seg_len;
	}

	
	return SE_SUCCESS;

}

/**
* @brief 非对称签名
* @param [in] key->alg  非对称算法类型(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256/ALG_SM2)
* @param [in] key->id   密钥id，与SE中写入私钥id一致
* @param [in] asym_param->hash_type  哈希算法(ALG_SHA1/ALG_SHA256/ALG_SHA384/ALG_SHA512/ALG_SM3/ALG_NONE)
* @param [in] in_buf  签名原文数据 
* @param [in] in_buf_len  签名原文数据长度
* @param [out] sign_buf  签名数据
* @param [out] sign_buf_len  签名数据长度
* @return 参见error.h
* @note 1、待签名数据未哈希时，需要指明哈希算法类型。2、待签名数据已哈希时，哈希算法类型为ALG_NONE，若算法为RSA需按照PKCS1方式对哈希值进行补位。
* @see 函数apdu_asym_sign
*/
se_error_t api_asym_sign (pri_key_t *key, alg_asym_param_t *asym_param,const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *sign_buf, uint32_t *sign_buf_len)
{
	se_error_t ret = 0;
	uint32_t block = DBLOCK+16;
	uint32_t count = 0;
	uint32_t temp_len = in_buf_len;
	uint32_t seg_len = 0;
	uint32_t off = 0;
	bool if_last_block =false;


	//参数检查
	if(key==NULL||asym_param==NULL||in_buf==NULL||in_buf_len==0)
	{  
		LOGE("failed to api_asym_sign input params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(sign_buf==NULL||sign_buf_len==NULL)
	{  
		LOGE("failed to api_asym_sign output params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	
	if(key->alg!=ALG_ECC256_NIST&&key->alg!=ALG_SM2&&key->alg!=ALG_RSA1024_CRT&&key->alg!=ALG_RSA2048_CRT)
	{  
		LOGE("failed to api_asym_sign alg params!\n");
		return SE_ERR_PARAM_INVALID;
	}


	if(asym_param->hash_type!=ALG_NONE&&asym_param->hash_type!=ALG_SHA1&&asym_param->hash_type!=ALG_SHA256\
	&&asym_param->hash_type!=ALG_SHA384&&asym_param->hash_type!=ALG_SHA512&&asym_param->hash_type!=ALG_SM3&&asym_param->hash_type!=ALG_SHA224)
	{
		LOGE("failed to api_asym_sign hash_type params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if((key->alg==ALG_SM2||key->alg==ALG_ECC256_NIST)&&(asym_param->hash_type==ALG_NONE))
	{
		if(in_buf_len!=32)
		{
			LOGE("failed to api_asym_sign in_buf_len params!\n");
			return SE_ERR_PARAM_INVALID;
		}

	}
	if(key->alg==ALG_RSA1024_CRT&&asym_param->hash_type==ALG_NONE)
		{
			if(in_buf_len!=128)
			{
				LOGE("failed to api_asym_sign in_buf_len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
	
		}
	if(key->alg==ALG_RSA2048_CRT&&asym_param->hash_type==ALG_NONE)
			{
				if(in_buf_len!=256)
				{
					LOGE("failed to api_asym_sign in_buf_len params!\n");
					return SE_ERR_PARAM_INVALID;
				}
		
			}

	//判断个数，最少是1块
	count = temp_len/block+1;

	if(temp_len%block==0)
		count--;

	//循环发送数据
	while(count>0)
	{			
		if(temp_len<=block)
		{
			seg_len = temp_len;
		}			
		else
			seg_len = block;
		//只有1包或者最后1包
		if(count==1)
		{
			if_last_block = true;
		}
		ret = apdu_asym_sign (key,asym_param,in_buf+off, seg_len, sign_buf, sign_buf_len,if_last_block);
		if(ret!=SE_SUCCESS)
		{	
			LOGE("failed to call api_asym_sign!\n");
			return ret;
		}
		off+=seg_len;
		count--;
		temp_len-=seg_len;
	}
	
	return SE_SUCCESS;	

}

/**
* @brief 非对称验签
* @param [in] key->alg  非对称算法类型(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256/ALG_SM2)
* @param [in] key->id   密钥id，与SE中写入公钥id一致
* @param [in] asym_param->hash_type  哈希算法(ALG_SHA1/ALG_SHA256/ALG_SHA384/ALG_SHA512/ALG_SM3/ALG_NONE)
* @param [in] in_buf  签名原文数据 
* @param [in] in_buf_len  签名原文数据长度
* @param [in] sign_buf  签名数据
* @param [in] sign_buf_len  签名数据长度
* @return 参见error.h
* @note 1、签名原文数据未哈希时，需要指明哈希算法类型。2、签名原文已哈希时，哈希算法类型为ALG_NONE，若算法为RSA需按照PKCS1方式对哈希值进行补位。
* @see 函数api_asym_verify
*/
se_error_t api_asym_verify (pub_key_t *key, alg_asym_param_t *asym_param,const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *sign_buf, uint32_t *sign_buf_len)
{
	se_error_t ret = 0;
	uint32_t block = DBLOCK+16;
	uint32_t count = 0;
	uint32_t temp_len = in_buf_len;
	uint32_t seg_len = 0;
	uint32_t off = 0;
	bool if_last_block =false;
	bool if_first_block =true;


	//参数检查
	if(key==NULL||asym_param==NULL||in_buf==NULL||in_buf_len==0)
	{  
		LOGE("failed to api_asym_sign input params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(sign_buf==NULL||sign_buf_len==NULL)
	{  
		LOGE("failed to api_asym_sign output params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	
	if(key->alg!=ALG_ECC256_NIST&&key->alg!=ALG_SM2&&key->alg!=ALG_RSA1024_CRT&&key->alg!=ALG_RSA2048_CRT)
	{  
		LOGE("failed to api_asym_sign alg params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(asym_param->hash_type!=ALG_NONE&&asym_param->hash_type!=ALG_SHA1&&asym_param->hash_type!=ALG_SHA256\
	&&asym_param->hash_type!=ALG_SHA384&&asym_param->hash_type!=ALG_SHA512&&asym_param->hash_type!=ALG_SM3&&asym_param->hash_type!=ALG_SHA224)
	{
		LOGE("failed to api_asym_sign hash_type params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if((key->alg==ALG_SM2||key->alg==ALG_ECC256_NIST)&&(asym_param->hash_type==ALG_NONE))
	{
		if(in_buf_len!=32)
		{
			LOGE("failed to api_asym_sign in_buf_len params!\n");
			return SE_ERR_PARAM_INVALID;
		}

	}
	if(key->alg==ALG_RSA1024_CRT)
		{
			if(asym_param->hash_type==ALG_NONE&&in_buf_len!=128)
			{
				LOGE("failed to api_asym_sign in_buf_len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
			
	        if(*sign_buf_len!=128)
			{
				LOGE("failed to api_asym_sign in_buf_len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
	if(key->alg==ALG_RSA2048_CRT)
		{
			if(asym_param->hash_type==ALG_NONE&&in_buf_len!=256)
			{
				LOGE("failed to api_asym_sign in_buf_len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
			
	        if(*sign_buf_len!=256)
			{
				LOGE("failed to api_asym_sign in_buf_len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}

	//判断个数，最少是1块
	count = (temp_len+(*sign_buf_len))/block+1;

	if((temp_len+(*sign_buf_len))%block==0)
		count--;

	//循环发送数据
	while(count>0)
	{			
		if(if_first_block==true)
		{
			if((temp_len+(*sign_buf_len))<=block)
			{
				seg_len = temp_len;
			}			
			else 
			{
				seg_len = block-(*sign_buf_len);
			}

		}
		else
		{
			if(temp_len<=block)
			{
				seg_len = temp_len;
			}			
			else
				seg_len = block;
		}
		
		//只有1包或者最后1包
		if(count==1)
		{
			if_last_block = true;
		}
		ret = apdu_asym_verify (key,asym_param, in_buf+off, seg_len, sign_buf, *sign_buf_len,if_first_block,if_last_block);
		if(ret!=SE_SUCCESS)
		{	
			LOGE("failed to call api_asym_sign!\n");
			return ret;
		}
		if_first_block=false;
		off+=seg_len;
		count--;
		temp_len-=seg_len;
	}
	
	return SE_SUCCESS;	

}

/**
* @brief za计算
* @param [in] uid->val  用户ID
* @param [in] uid->val_len  用户ID长度
* @param [in] pub_key->val  SM2公钥值 X|Y 
* @param [in] pub_key->val_len  SM2公钥值长度，64字节
* @param [out] za  za值，32字节
* @return 参见error.h
* @note no
* @see 函数apdu_sm2_get_za
*/
se_error_t api_sm2_get_za (user_id_t* uid, pub_key_t *pub_key , uint8_t *za )
{
	se_error_t ret = 0;
	//参数检查
	if(uid==NULL||pub_key==NULL||za==NULL)
	{
		LOGE("failed to api_sm2_get_za pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(uid->val_len==0||uid->val_len>32||pub_key->val_len!=64)
	{
		LOGE("failed to api_sm2_get_za input params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	//调用cmd层 apdu_sm2_get_za接口
	ret = apdu_sm2_get_za (uid, pub_key , za );
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_sm2_get_za!\n");
		return ret;
	}

	return SE_SUCCESS;
	
}

/**
* @brief 哈希计算
* @param [in] alg  哈希算法(ALG_SHA1/ALG_SHA224/ALG_SHA256/ALG_SHA384/ALG_SHA512/ALG_SM3/ALG_NONE)
* @param [in] in_buf  输入数据 
* @param [in] in_buf_len  输入数据长度
* @param [out] out_buf  哈希数据
* @param [out] out_buf_len  哈希数据长度
* @return 参见error.h
* @note no
* @see 函数apdu_digest
*/
se_error_t api_digest (uint32_t alg, const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *out_buf, uint32_t *out_buf_len)
{
	se_error_t ret = 0;
	uint32_t block = 254;
	uint32_t count = 0;
	uint32_t temp_len = in_buf_len;
	uint32_t seg_len = 0;
	uint32_t off = 0;
	bool if_last_block =false;

	//参数检查
	if(in_buf==NULL||in_buf_len==0)
	{  
		LOGE("failed to api_digest input params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(out_buf==NULL||out_buf_len==NULL)
	{  
		LOGE("failed to api_digest output params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(alg!=ALG_SHA1&&alg!=ALG_SHA224&&alg!=ALG_SHA256&&alg!=ALG_SHA384&&alg!=ALG_SHA512&&alg!=ALG_SM3)
	{
		LOGE("failed to api_digest alg params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	

	//判断个数，最少是1块
	count = temp_len/block+1;

	if(temp_len%block==0)
		count--;

	//循环发送数据
	while(count>0)
	{			
		
		if(temp_len<=block)
		{
			seg_len = temp_len;
		}			
		else
			seg_len = block;
		//只有1包或者最后1包
		if(count==1)
		{
			if_last_block = true;
		}
		ret = apdu_digest (alg,in_buf+off, seg_len, out_buf, out_buf_len,if_last_block);
		if(ret!=SE_SUCCESS)
		{	
			LOGE("failed to call api_asym_encrypt!\n");
			return ret;
		}
		off+=seg_len;
		count--;
		temp_len-=seg_len;
	}
	
	return SE_SUCCESS;	

}


/**
* @brief 获取随机数
* @param [in] expected_len  期望随机数长度
* @param [out]random  随机数
* @return 参见error.h
* @note no
* @see 函数apdu_get_random
*/
se_error_t  api_get_random  (uint32_t expected_len, uint8_t *random)
{
	se_error_t ret = 0;
	//参数检查
	if(random==NULL)
	{
		LOGE("failed to api_get_random pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	
	if(expected_len<4||expected_len>16)
	{
		LOGE("failed to api_get_random input params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	//调用cmd层apdu_get_random接口
	ret = apdu_get_random  (expected_len,random);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to apdu_get_random!\n");
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
