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
* @brief �ԳƼ���
* @param [in] key->alg  �Գ��㷨����(ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] key->id  ��Կidֵ
* @param [in] sym_param->mode  �ԳƼ���ģʽ
* @param [in] sym_param->iv  ��ʼ����
* @param [in] sym_param->iv_len  ��ʼ��������
* @param [in] sym_param->padding_type ��λ��ʽ(PADDING_NOPADDING/PADDING_PKCS7)
* @param [in] in_buf  �������� 
* @param [in] in_buf_len  �������ݳ���
* @param [out] out_buf  ��������
* @param [out] out_buf_len  �������ݳ���
* @return �μ�error.h
* @note no
* @see ����apdu_sym_enc_dec
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
	//�������
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
	else//pkcs7���㲹λ��ĳ���
	{
		padding_len =  apdu_sym_padding_length(key->alg,PADDING_PKCS7,in_buf_len);
	}

	//�жϸ�����������1�飬����block 992�ֽڽ��зְ���countΪ�ְ�����
	count = temp_len/block+1;
	if(temp_len%block==0)
		count--;

	//ѭ����������
	while(count>0)
	{			
		
		if(temp_len<=block)
		{
			seg_len = temp_len;
		}			
		else
			seg_len = block;
		//ֻ��1���������1��
		if(count==1)
		{
		    //�ж�Ϊ���һ��if_last_block��Ϊtrue
			if_last_block = true;
			//���һ����pkcs7���㲹λ���ȣ����뵽out_buf_len��
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
	//�������ݳ���
	*out_buf_len = padding_len;
	return SE_SUCCESS;
}


/**
* @brief �Գƽ���
* @param [in] key->alg  �Գ��㷨����(ALG_AES128/ALG_DES128/ALG_SM1/ALG_SM4)
* @param [in] key->id  ��Կidֵ
* @param [in] sym_param->mode  �ԳƼ���ģʽ
* @param [in] sym_param->iv  ��ʼ����
* @param [in] sym_param->iv_len  ��ʼ��������
* @param [in] sym_param->padding_type ��λ��ʽ(PADDING_NOPADDING/PADDING_PKCS7)
* @param [in] in_buf  �������� 
* @param [in] in_buf_len  �������ݳ���
* @param [out] out_buf  ��������
* @param [out] out_buf_len  �������ݳ���
* @return �μ�error.h
* @note no
* @see ����apdu_sym_enc_dec
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
	//�������
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
	//�жϸ�����������1��
	count = temp_len/block+1;

	if(temp_len%block==0)
		count--;

	//ѭ����������
	while(count>0)
	{				
		if(temp_len<=block)
		{
			seg_len = temp_len;
		}			
		else
			seg_len = block;
		//ֻ��1���������1��
		if(count==1)
		{
			//�ж�Ϊ���һ��if_last_block��Ϊtrue
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
* @brief MAC����
* @param [in] key->alg  �Գ��㷨����(ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] key->id  ��Կidֵ
* @param [in] sym_param->iv  ��ʼ����
* @param [in] sym_param->iv_len  ��ʼ��������
* @param [in] sym_param->padding_type ��λ��ʽ(PADDING_NOPADDING/PADDING_ISO9797_M1/PADDING_ISO9797_M2)
* @param [in] in_buf  MAC�������� 
* @param [in] in_buf_len  MAC�������ݳ���
* @param [out] out_buf  MAC����
* @param [out] out_buf_len  MAC���ݳ���
* @return �μ�error.h
* @note no
* @see ����apdu_mac
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

	//�������
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
	

	//�жϸ�����������1��
	count = temp_len/block+1;

	if(temp_len%block==0)
		count--;

	//ѭ����������
	while(count>0)
	{			
		
		if(temp_len<=block)
		{
			seg_len = temp_len;
		}			
		else
			seg_len = block;
		///ֻ��1���������1��
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
* @brief MAC��֤
* @param [in] key->alg  �Գ��㷨����(ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] key->id  ��Կidֵ
* @param [in] sym_param->iv  ��ʼ����
* @param [in] sym_param->iv_len  ��ʼ��������
* @param [in] sym_param->padding_type ��λ��ʽ(PADDING_NOPADDING/PADDING_ISO9797_M1/PADDING_ISO9797_M2)
* @param [in] in_buf  MAC�������� 
* @param [in] in_buf_len  MAC�������ݳ���
* @param [in] mac  MAC����
* @param [in] mac_len  MAC���ݳ���
* @return �μ�error.h
* @note no
* @see ����apdu_mac
*/
se_error_t  api_mac_verify (sym_key_t *key, alg_sym_param_t *sym_param,const uint8_t *in_buf,uint32_t in_buf_len,const uint8_t *mac, uint32_t mac_len)
{
	se_error_t ret = 0;
	uint32_t block = DBLOCK-16;//��ֹ�����������macֵ֮�󳬹�992�ֽڣ�block��ǰ��ȥ16
	int32_t count = 0;
	uint32_t temp_len = in_buf_len;
	uint32_t seg_len = 0;
	uint32_t off = 0;
	//uint32_t iv_len = 0;
	bool if_last_block =false;
	bool if_first_block =true;

	//�������
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
	

	//�жϸ�����������1��
	count = temp_len/block+1;

	if(temp_len%block==0)
		count--;

	//ѭ����������
	while(count>0)
	{			
		
		if(temp_len<=block)
		{
			seg_len = temp_len;
		}			
		else
			seg_len = block;
		//ֻ��1���������1��
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
* @brief �ǶԳƼ���
* @param [in] key->alg  �ǶԳ��㷨����(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256/ALG_SM2)
* @param [in] key->id  ��Կidֵ
* @param [in] asym_param-> padding_type  ��λ��ʽ(PADDING_NOPADDING/PADDING_PKCS1)(��RSA�㷨ʱ��Ч)
* @param [in] in_buf  �������� 
* @param [in] in_buf_len  �������ݳ���
* @param [out] out_buf  ��������
* @param [out] out_buf_len  �������ݳ���
* @return �μ�error.h
* @note no
* @see ����apdu_asym_enc_dec
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


	//�������
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

	//��λ��ʽ��PADDING_NOPADDING-�ⲿ����ģ�� PADDING_PKCS1-�ڲ����в�λ
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
		//SM2��ECC�㷨���������ݲ�����4096�ֽ�
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
			//RSA�㷨����������Ϊģ��
			if(in_buf_len!=128&&in_buf_len!=256)
			{
				LOGE("failed to api_asym_encrypt in_buf_len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
		else
		{
			//RSA�㷨����������Ϊ��������ģ��-11��
			if((key->alg==ALG_RSA2048_CRT&&asym_param->padding_type==PADDING_PKCS1&&in_buf_len>245)||(key->alg==ALG_RSA1024_CRT&&asym_param->padding_type==PADDING_PKCS1&&in_buf_len>117))
			{
				LOGE("failed to api_asym_encrypt in_buf_len params!\n");
				return SE_ERR_PARAM_INVALID;
			}
		}
	}


	//�жϸ�����������1��
	count = temp_len/block+1;

	if(temp_len%block==0)
		count--;

	//ѭ����������
	while(count>0)
	{			
			
		if(temp_len<=block)
		{
			seg_len = temp_len;
		}			
		else
			seg_len = block;
		//ֻ��1���������1��
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
* @brief �ǶԳƽ���
* @param [in] key->alg  �ǶԳ��㷨����(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256/ALG_SM2)
* @param [in] key->id  ˽Կidֵ
* @param [in] asym_param->padding_type  ��λ��ʽ(PADDING_NOPADDING/PADDING_PKCS1)(��RSA�㷨ʱ��Ч)
* @param [in] in_buf  �������� 
* @param [in] in_buf_len  �������ݳ���
* @param [out] out_buf  ��������
* @param [out] out_buf_len  �������ݳ���
* @return �μ�error.h
* @note no
* @see ����apdu_asym_enc_dec
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


	//�������
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

	//��λ��ʽ��PADDING_NOPADDING-�ⲿ����ģ�� PADDING_PKCS1-�ڲ����в�λ
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
		//SM2��ECC�㷨���������ݲ�����4096�ֽ�
		if(in_buf_len>4096+96)
		{
			LOGE("failed to api_asym_decrypt in_buf_len params!\n");
			return SE_ERR_PARAM_INVALID;
		}
	}
	else 
	{
		//RSA�㷨����������Ϊģ��
		if(in_buf_len!=128&&in_buf_len!=256)
		{
			LOGE("failed to api_asym_decrypt in_buf_len params!\n");
			return SE_ERR_PARAM_INVALID;
		}

	}

	//�жϸ�����������1��
	count = temp_len/block+1;

	if(temp_len%block==0)
		count--;

	//ѭ����������
	while(count>0)
	{			
		
		if(temp_len<=block)
		{
			seg_len = temp_len;
		}			
		else
			seg_len = block;
		//ֻ��1���������1��
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
* @brief �ǶԳ�ǩ��
* @param [in] key->alg  �ǶԳ��㷨����(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256/ALG_SM2)
* @param [in] key->id   ��Կid����SE��д��˽Կidһ��
* @param [in] asym_param->hash_type  ��ϣ�㷨(ALG_SHA1/ALG_SHA256/ALG_SHA384/ALG_SHA512/ALG_SM3/ALG_NONE)
* @param [in] in_buf  ǩ��ԭ������ 
* @param [in] in_buf_len  ǩ��ԭ�����ݳ���
* @param [out] sign_buf  ǩ������
* @param [out] sign_buf_len  ǩ�����ݳ���
* @return �μ�error.h
* @note 1����ǩ������δ��ϣʱ����Ҫָ����ϣ�㷨���͡�2����ǩ�������ѹ�ϣʱ����ϣ�㷨����ΪALG_NONE�����㷨ΪRSA�谴��PKCS1��ʽ�Թ�ϣֵ���в�λ��
* @see ����apdu_asym_sign
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


	//�������
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

	//�жϸ�����������1��
	count = temp_len/block+1;

	if(temp_len%block==0)
		count--;

	//ѭ����������
	while(count>0)
	{			
		if(temp_len<=block)
		{
			seg_len = temp_len;
		}			
		else
			seg_len = block;
		//ֻ��1���������1��
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
* @brief �ǶԳ���ǩ
* @param [in] key->alg  �ǶԳ��㷨����(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256/ALG_SM2)
* @param [in] key->id   ��Կid����SE��д�빫Կidһ��
* @param [in] asym_param->hash_type  ��ϣ�㷨(ALG_SHA1/ALG_SHA256/ALG_SHA384/ALG_SHA512/ALG_SM3/ALG_NONE)
* @param [in] in_buf  ǩ��ԭ������ 
* @param [in] in_buf_len  ǩ��ԭ�����ݳ���
* @param [in] sign_buf  ǩ������
* @param [in] sign_buf_len  ǩ�����ݳ���
* @return �μ�error.h
* @note 1��ǩ��ԭ������δ��ϣʱ����Ҫָ����ϣ�㷨���͡�2��ǩ��ԭ���ѹ�ϣʱ����ϣ�㷨����ΪALG_NONE�����㷨ΪRSA�谴��PKCS1��ʽ�Թ�ϣֵ���в�λ��
* @see ����api_asym_verify
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


	//�������
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

	//�жϸ�����������1��
	count = (temp_len+(*sign_buf_len))/block+1;

	if((temp_len+(*sign_buf_len))%block==0)
		count--;

	//ѭ����������
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
		
		//ֻ��1���������1��
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
* @brief za����
* @param [in] uid->val  �û�ID
* @param [in] uid->val_len  �û�ID����
* @param [in] pub_key->val  SM2��Կֵ X|Y 
* @param [in] pub_key->val_len  SM2��Կֵ���ȣ�64�ֽ�
* @param [out] za  zaֵ��32�ֽ�
* @return �μ�error.h
* @note no
* @see ����apdu_sm2_get_za
*/
se_error_t api_sm2_get_za (user_id_t* uid, pub_key_t *pub_key , uint8_t *za )
{
	se_error_t ret = 0;
	//�������
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
	//����cmd�� apdu_sm2_get_za�ӿ�
	ret = apdu_sm2_get_za (uid, pub_key , za );
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_sm2_get_za!\n");
		return ret;
	}

	return SE_SUCCESS;
	
}

/**
* @brief ��ϣ����
* @param [in] alg  ��ϣ�㷨(ALG_SHA1/ALG_SHA224/ALG_SHA256/ALG_SHA384/ALG_SHA512/ALG_SM3/ALG_NONE)
* @param [in] in_buf  �������� 
* @param [in] in_buf_len  �������ݳ���
* @param [out] out_buf  ��ϣ����
* @param [out] out_buf_len  ��ϣ���ݳ���
* @return �μ�error.h
* @note no
* @see ����apdu_digest
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

	//�������
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

	

	//�жϸ�����������1��
	count = temp_len/block+1;

	if(temp_len%block==0)
		count--;

	//ѭ����������
	while(count>0)
	{			
		
		if(temp_len<=block)
		{
			seg_len = temp_len;
		}			
		else
			seg_len = block;
		//ֻ��1���������1��
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
* @brief ��ȡ�����
* @param [in] expected_len  �������������
* @param [out]random  �����
* @return �μ�error.h
* @note no
* @see ����apdu_get_random
*/
se_error_t  api_get_random  (uint32_t expected_len, uint8_t *random)
{
	se_error_t ret = 0;
	//�������
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
	//����cmd��apdu_get_random�ӿ�
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
