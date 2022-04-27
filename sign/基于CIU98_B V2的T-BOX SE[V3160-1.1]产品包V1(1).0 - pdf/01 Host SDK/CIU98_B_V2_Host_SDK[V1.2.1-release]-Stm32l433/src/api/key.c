/**@file  key.c
* @brief  key interface definition
* @author  liangww
* @date  2021-04-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/

#include "key.h"
#include "sm4.h"
uint8_t trans_key[16] = {0x00};

/** @addtogroup SE_Service
  * @{
  */

/** @addtogroup API 
  * @brief API layer.
  * @{
  */

/** @defgroup KEY KEY
  * @brief key interface api.
  * @{
  */



/* Exported functions --------------------------------------------------------*/

/** @defgroup KEY_Exported_Functions KEY Exported Functions
  * @{
  */


/**
* @brief 用于更新设备主控密钥
* @param [in] mkey->value  设备主控密钥值
* @param [in] mkey->value_len  设备主控密钥长度
* @param [in] if_encrypt  是否传输密钥加密
* @return 参见error.h
* @note 使用前需要获得设备权限
* @see 函数apdu_write_key
*/
se_error_t api_update_mkey (sym_key_t *mkey, bool if_encrypt)
{
	se_error_t ret = 0;
	//参数检查
	if(mkey==NULL)
	{  
		LOGE("failed to api_write_key mkey params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(if_encrypt!=false&&if_encrypt!=true)
	{  
		LOGE("failed to api_update_mkey bool params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	
	
	    //调用cmd层apdu_write_key接口
		ret = apdu_write_key (mkey->val,mkey->val_len,if_encrypt,false,true);
		if(ret!=SE_SUCCESS)
		{	
			LOGE("failed to call apdu_write_key!\n");
			return ret;
		}

	return SE_SUCCESS;
}


/**
* @brief 产生非对称固定/临时公私钥对
* @param [in] pub_key->alg  算法类型(ALG_RSA1024_STANDAND/ALG_RSA1024_CRT/ALG_RSA2048_STANDAND/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2)
* @param [in] pub_key->id  公钥id (固定密钥公钥ID范围为：00-0xEF,临时公钥ID为：0xF0-0xFE)
* @param [in] pri_key->id  私钥id (固定密钥私钥ID范围为：00-0xEF,临时私钥ID为：0xF0-0xFE)
* @param [out] pub_key->val  公钥值
* @param [out] pub_key->val_len  公钥值长度
* @return 参见error.h
* @note 如果为固定密钥，需要满足安全文件的写权限。
* @see 函数apdu_generate_keypair
*/

se_error_t  api_generate_keypair (pub_key_t *pub_key, pri_key_t *pri_key)
{
	se_error_t ret = 0;
	//参数检查
	if(pub_key==NULL||pri_key==NULL)
	{  
		LOGE("failed to api_generate_keypair  pub_key/pri_key params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(pub_key->id==NULL||pri_key->id==NULL)
		{  
			LOGE("failed to api_generate_keypair id params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	if(pub_key->alg!=ALG_ECC256_NIST&&pub_key->alg!=ALG_SM2&&pub_key->alg!=ALG_RSA1024_CRT&&pub_key->alg!=ALG_RSA2048_CRT&&pub_key->alg!=ALG_RSA1024_STANDAND&&pub_key->alg!=ALG_RSA2048_STANDAND)
	{  
		LOGE("failed to api_generate_keypair id  params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	//调用cmd层apdu_generate_key接口
	ret = apdu_generate_key (pub_key,pri_key,NULL);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_generate_keypair!\n");
		return ret;
	}
	return SE_SUCCESS;
}



/**
* @brief 用于初始化传输密钥
* @param [in] key->value  传输密钥值
* @param [in] key->value_len  传输密钥长度
* @param [in] key->id  传输密钥id
* @return 参见error.h
* @note 传输密钥kid为默认0x02
* @see 函数apdu_generate_keypair
*/

se_error_t  api_set_transkey (sym_key_t *key)
{
	se_error_t ret = 0;
	uint8_t inbuf[19]={0};
	uint32_t inbuf_len = 19;
	//参数检查
	if(key==NULL)
	{  
		LOGE("failed to api_set_transkey input pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(key->val==NULL||key->val_len!=16||key->id!=0x02)
		{  
			LOGE("failed to api_set_transkey input params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	
    //添加密钥属性信息
	inbuf[0]=0x01;//密钥用途
	inbuf[1]=key->id;//密钥索引：传输密钥默认使用0x02索引
	inbuf[2]=0x40;//算法类型：SM4
	memcpy(inbuf+3,key->val,key->val_len);
	memcpy(trans_key,key->val,key->val_len );//设置trans_key为新的密钥值
	//调用cmd层apdu_write_key接口
	ret = apdu_write_key (inbuf,inbuf_len,false,false,false);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_generate_keypair!\n");
		return ret;
	}
	return SE_SUCCESS;
}



/**
* @brief 用于用于生成一个对称应用密钥
* @param [in] symkey->alg 算法类型(ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] symkey->id  密钥id (固定密钥密钥钥ID范围为：00-0xEF,临时密钥钥ID为：0xF0-0xFE)
* @return 参见error.h
* @note 如果为固定密钥，需要满足安全文件的写权限。
* @see 函数apdu_generate_keypair
*/

se_error_t  api_generate_symkey (sym_key_t * symkey)
{
	se_error_t ret = 0;
	//参数检查
	if(symkey==NULL)
	{  
		LOGE("failed to api_generate_symkey input pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	
	if(symkey->alg!=ALG_AES128&&symkey->alg!=ALG_DES128&symkey->alg!=ALG_SM4)
	{  
		LOGE("failed to api_generate_symkey input params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	//调用cmd层apdu_generate_key接口
	ret = apdu_generate_key (NULL,NULL,symkey);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_generate_keypair!\n");
		return ret;
	}
	return SE_SUCCESS;
}


/**
* @brief 产生ECC/SM2协商密钥
* @param [in] shared_key->alg 算法类型(ALG_ECDH_ECC256/ALG_ECDH_SM2/ALG_SM2_SM2)
* @param [in] shared_key->id  当算法类型为ALG_ECDH_ECC256/ALG_ECDH_SM2时SE内私钥标识符KID
* @param [in] in_buf  计算数据:当算法类型为ALG_ECDH_ECC256/ALG_ECDH_SM2为对方公钥；当算法类型为ALG_SM2_SM2时，参考《基于CIU98_B的SE[V3160]用户使用手册》
* @param [in] in_buf_len  计算数据长度
* @param [in] if_return_key 是否返回shared key
* @param [in] if_return_s 是否返回sm2_s值
* @param [out] shared_key 协商密钥16字节
* @param [out] sm2_s 校验值64字节
* @return 参见error.h
* @note no
* @see 函数apdu_generate_shared_key
*/
se_error_t api_generate_shared_key (uint8_t *in_buf, uint32_t in_buf_len, unikey_t *shared_key, uint8_t *sm2_s, bool if_return_key, bool if_return_s)
{
	se_error_t ret = 0;
	//uint8_t key_info_tmp [2] = {0xF0,0x60};
	uint8_t int_buf_tmp[66] = {0xF0,0x60};//COS支持生成协商出指定算法的对称密钥，该功能SDK暂不开放，固密钥id和算法类型写死为0xF0,0x60。
	uint32_t in_buf_len_tmp = 0;
	
	//参数判断
	if(shared_key->alg!=ALG_ECDH_ECC256 &&shared_key->alg!=ALG_ECDH_SM2&&shared_key->alg!=ALG_SM2_SM2)
	{  
		LOGE("failed to api_generate_shared_key params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(in_buf==NULL)
	{
		LOGE("failed to api_generate_shared_key input params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(shared_key->alg==ALG_ECDH_ECC256||shared_key->alg==ALG_ECDH_SM2)
	{
		if(in_buf_len>66)
		{
			LOGE("failed to api_generate_shared_key input len params!\n");
			return SE_ERR_PARAM_INVALID;
		}

	}
	else
	{
		if(in_buf_len<139||in_buf_len>201)
		{
			LOGE("failed to api_generate_shared_key input len params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	}
  
	//调用命令层apdu_generate_shared_key接口
	if(shared_key->alg==ALG_SM2_SM2)
	{
	  ret = apdu_generate_shared_key (in_buf, in_buf_len,shared_key,sm2_s, if_return_key, if_return_s);
	}
	else
	{
	  memcpy(int_buf_tmp+2, in_buf, in_buf_len);
		in_buf_len_tmp = in_buf_len+2 ;
		ret = apdu_generate_shared_key (int_buf_tmp, in_buf_len_tmp,shared_key,sm2_s, if_return_key, if_return_s);
	}
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_generate_shared_key!\n");
		return ret;
	}


	return SE_SUCCESS;
}


/**
* @brief 用于删除指定密钥标识符KID的应用密钥
* @param [in] id 被删除密钥id
* @return 参见error.h
* @note 删除固定应用密钥时，要需要获得安全文件的写权限
* @see 函数apdu_del_key
*/
se_error_t api_del_key(uint8_t id)
{
	se_error_t ret = 0;
	
	//调用cmd层apdu_delete_key接口
	ret = apdu_delete_key(id);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call api_del_key!\n");
		return ret;
	}

	return SE_SUCCESS;
}



/**
* @brief 接口用于应用密钥的明文或密文导入
* @param [in] ukey->alg 算法类型(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2/ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] ukey->id  应用密钥id（解密用私钥KID或解密用对称密钥kid）
* @param [in] inkey->alg 算法类型(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2/ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] inkey->id   密钥标识符，范围为0x00-0xFF
* @param [in] inkey->type 密钥类型
* @param [in] inkey->val  密钥值
* @param [in] inkey->val_len  会话密钥值长度
* @param [in] if_cipher  true-密文 false-明文
* @param [in] if_trasns_key  是否采用传输密钥加密导入密钥
* @return 参见error.h
* @note 1、	RSA加密时，按照PKCS1补位方式进行补位。2、	对称算法加密时首先在数据前加两字节的Ld（被加密数据长度），然后采用9797_M2方式填充为整块长度。加密模式采用ECB模式。3、	导入固定应用密钥时，要需要获得安全文件的写权限。4、	密钥值格式和密钥值长度参见《基于CIU98_B的SE[V3160]用户使用手册》。5、当采用明文导入或采用传输密钥加密导入时：ukey无效。
* @see 函数apdu_import_key
*/
se_error_t  api_import_key(unikey_t *ukey, unikey_t *inkey ,bool if_cipher,bool if_trasns_key)
{
	se_error_t ret = 0;
	//参数检查
	if(if_cipher==true)
	{ 	
         if(if_trasns_key == false)
         {
			 if(ukey->alg!=ALG_RSA1024_CRT &&ukey->alg!=ALG_RSA2048_CRT\
					  &&ukey->alg!=ALG_ECC256_NIST&&ukey->alg!=ALG_SM2&&ukey->alg!=ALG_AES128&&ukey->alg!=ALG_DES128&&ukey->alg!=ALG_SM4&&ukey->alg!=ALG_AES128&&ukey->alg!=ALG_RSA1024_STANDAND&&ukey->alg!=ALG_RSA2048_STANDAND)
			 {

				 LOGE("failed to api_import_key alg params!\n");
				 return SE_ERR_PARAM_INVALID;
			 }	 
		 }		 
	}
	else
	{
	   if(if_trasns_key == true)
		{
			LOGE("failed to api_import_key id params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	   
	}
	//判断导入密钥参数
	if(inkey==NULL)
	{
		LOGE("failed to api_import_key inkey params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(inkey->alg!=ALG_RSA1024_CRT &&inkey->alg!=ALG_RSA2048_CRT\
		 &&inkey->alg!=ALG_ECC256_NIST&&inkey->alg!=ALG_SM2&&inkey->alg!=ALG_AES128&&inkey->alg!=ALG_DES128&&inkey->alg!=ALG_SM4&&inkey->alg!=ALG_RSA1024_STANDAND&&inkey->alg!=ALG_RSA2048_STANDAND)
	{
		LOGE("failed to api_import_key alg params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(inkey->val_len<16||inkey->val_len>912)
	{
		LOGE("failed to api_import_key val params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(inkey->type!=PRI&&inkey->type!=PUB&&inkey->type!=PRI_PUB_PAIR&&inkey->type!=SYM)
	{
		LOGE("failed to api_import_key type params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	//调用cmd层apdu_import_key接口
	ret = apdu_import_key (ukey,inkey,if_cipher,if_trasns_key);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_import_key!\n");
		return ret;
	}


	return SE_SUCCESS;


}


/**
* @brief 接口用于以明文或密文的形式导出对称临时应用密钥和非对称临时应用私钥，及明文导出固定/临时非对称应用公钥
* @param [in] ukey->alg 算法类型(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2/ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] ukey->id  应用密钥id（加密用公钥KID或加密用对称密钥kid）
* @param [in] exkey->alg 算法类型(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2/ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] exkey->id   密钥标识符，范围为0x00-0xFF
* @param [in] exkey->type 密钥类型
* @param [in] if_cipher  true-密文 false-明文
* @param [in] if_trasns_key  是否采用传输密钥加密导入密钥
* @param [out] exkey->val  密钥值
* @param [out] exkey->val_len  会话密钥值长度
* @return 参见error.h
* @note 1、	RSA加密时，按照PKCS1补位方式进行补位。2、	对称算法加密时首先在数据前加两字节的Ld（被加密数据长度），然后采用9797_M2方式填充为整块长度。加密模式采用ECB模式。3、导出非对称临时应用私钥时，需要获得该私钥的使用权限。4、	密钥值格式和密钥值长度参见《基于CIU98_B的SE[V3160]用户使用手册》。5、当采用明文导入或采用传输密钥加密导入时：ukey无效。
* @see 函数apdu_export_key
*/
se_error_t  api_export_key(unikey_t *ukey, unikey_t *exkey, bool if_cipher,bool if_trasns_key)
{
	se_error_t ret = 0;
	//参数检查
	if(if_cipher==true)
	{
		if(if_trasns_key == false)
		{
	          if(ukey->alg!=ALG_RSA1024_CRT &&ukey->alg!=ALG_RSA2048_CRT\
			 &&ukey->alg!=ALG_ECC256_NIST&&ukey->alg!=ALG_SM2&&ukey->alg!=ALG_AES128&&ukey->alg!=ALG_DES128&&ukey->alg!=ALG_SM4)
			{

				LOGE("failed to api_import_key alg params!\n");
				return SE_ERR_PARAM_INVALID;
			}
			if((exkey->type==PRI &&ukey->alg==ALG_RSA1024_CRT)||(exkey->type==PRI &&ukey->alg==ALG_RSA2048_CRT)||(exkey->type==PRI &&ukey->alg==ALG_ECC256_NIST)||(exkey->type==PRI &&ukey->alg==ALG_SM2)||(exkey->type==PRI &&ukey->alg==ALG_RSA1024_STANDAND)||(exkey->type==PRI &&ukey->alg==ALG_RSA2048_STANDAND))
			{

				LOGE("failed to api_export_key alg params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
         
		if(exkey->type==PUB)
		{

			LOGE("failed to api_export_key type params!\n");
			return SE_ERR_PARAM_INVALID;
		}

	}
	else
	{
	    if(if_trasns_key == true)
		{
			LOGE("failed to api_import_key id params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	}
	//判断导出密钥参数
	if(exkey==NULL)
	{
		LOGE("failed to api_import_session_key exkey params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	if(exkey->alg!=ALG_RSA1024_CRT &&exkey->alg!=ALG_RSA2048_CRT\
		 &&exkey->alg!=ALG_ECC256_NIST&&exkey->alg!=ALG_SM2&&exkey->alg!=ALG_AES128&&exkey->alg!=ALG_DES128&&exkey->alg!=ALG_SM4&&exkey->alg!=ALG_RSA1024_STANDAND &&exkey->alg!=ALG_RSA2048_STANDAND)
	{
		LOGE("failed to api_export_key alg params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	
	if(exkey->type!=PRI&&exkey->type!=PUB&&exkey->type!=SYM)
	{
		LOGE("failed to api_export_key alg type!\n");
		return SE_ERR_PARAM_INVALID;
	}

	if(exkey->type==PRI||exkey->type==SYM)//只能导出临时私钥和临时对称密钥
	{
	    if(exkey->id<0xF0)
		{
			LOGE("failed to api_export_key alg type!\n");
			return SE_ERR_PARAM_INVALID;
		}
	}
	
	//调用cmd层apdu_export_key接口
	ret = apdu_export_key (ukey,exkey,if_cipher, if_trasns_key);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_export_key!\n");
		return ret;
	}

	return SE_SUCCESS;


}

/**
* @brief 读取密钥信息
* @param [in] if_app_key	是否读取应用密钥信息
* @param [out] out_buf  输出数据
* @param [out] out_buf_len  输出数据长度
* @return 参见error.h
* @note no
* @see 函数apdu_get_key_info
*/
se_error_t  api_get_key_info (bool     if_app_key, uint8_t *out_buf,uint32_t *out_buf_len)
{
	se_error_t ret = 0;
	
	//调用cmd层apdu_get_key_info接口
	ret = apdu_get_key_info (if_app_key, out_buf,out_buf_len);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_get_key_info!\n");
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
