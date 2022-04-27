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
* @brief ���ڸ����豸������Կ
* @param [in] mkey->value  �豸������Կֵ
* @param [in] mkey->value_len  �豸������Կ����
* @param [in] if_encrypt  �Ƿ�����Կ����
* @return �μ�error.h
* @note ʹ��ǰ��Ҫ����豸Ȩ��
* @see ����apdu_write_key
*/
se_error_t api_update_mkey (sym_key_t *mkey, bool if_encrypt)
{
	se_error_t ret = 0;
	//�������
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
	
	
	    //����cmd��apdu_write_key�ӿ�
		ret = apdu_write_key (mkey->val,mkey->val_len,if_encrypt,false,true);
		if(ret!=SE_SUCCESS)
		{	
			LOGE("failed to call apdu_write_key!\n");
			return ret;
		}

	return SE_SUCCESS;
}


/**
* @brief �����ǶԳƹ̶�/��ʱ��˽Կ��
* @param [in] pub_key->alg  �㷨����(ALG_RSA1024_STANDAND/ALG_RSA1024_CRT/ALG_RSA2048_STANDAND/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2)
* @param [in] pub_key->id  ��Կid (�̶���Կ��ԿID��ΧΪ��00-0xEF,��ʱ��ԿIDΪ��0xF0-0xFE)
* @param [in] pri_key->id  ˽Կid (�̶���Կ˽ԿID��ΧΪ��00-0xEF,��ʱ˽ԿIDΪ��0xF0-0xFE)
* @param [out] pub_key->val  ��Կֵ
* @param [out] pub_key->val_len  ��Կֵ����
* @return �μ�error.h
* @note ���Ϊ�̶���Կ����Ҫ���㰲ȫ�ļ���дȨ�ޡ�
* @see ����apdu_generate_keypair
*/

se_error_t  api_generate_keypair (pub_key_t *pub_key, pri_key_t *pri_key)
{
	se_error_t ret = 0;
	//�������
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
	//����cmd��apdu_generate_key�ӿ�
	ret = apdu_generate_key (pub_key,pri_key,NULL);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_generate_keypair!\n");
		return ret;
	}
	return SE_SUCCESS;
}



/**
* @brief ���ڳ�ʼ��������Կ
* @param [in] key->value  ������Կֵ
* @param [in] key->value_len  ������Կ����
* @param [in] key->id  ������Կid
* @return �μ�error.h
* @note ������ԿkidΪĬ��0x02
* @see ����apdu_generate_keypair
*/

se_error_t  api_set_transkey (sym_key_t *key)
{
	se_error_t ret = 0;
	uint8_t inbuf[19]={0};
	uint32_t inbuf_len = 19;
	//�������
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
	
    //�����Կ������Ϣ
	inbuf[0]=0x01;//��Կ��;
	inbuf[1]=key->id;//��Կ������������ԿĬ��ʹ��0x02����
	inbuf[2]=0x40;//�㷨���ͣ�SM4
	memcpy(inbuf+3,key->val,key->val_len);
	memcpy(trans_key,key->val,key->val_len );//����trans_keyΪ�µ���Կֵ
	//����cmd��apdu_write_key�ӿ�
	ret = apdu_write_key (inbuf,inbuf_len,false,false,false);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_generate_keypair!\n");
		return ret;
	}
	return SE_SUCCESS;
}



/**
* @brief ������������һ���Գ�Ӧ����Կ
* @param [in] symkey->alg �㷨����(ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] symkey->id  ��Կid (�̶���Կ��ԿԿID��ΧΪ��00-0xEF,��ʱ��ԿԿIDΪ��0xF0-0xFE)
* @return �μ�error.h
* @note ���Ϊ�̶���Կ����Ҫ���㰲ȫ�ļ���дȨ�ޡ�
* @see ����apdu_generate_keypair
*/

se_error_t  api_generate_symkey (sym_key_t * symkey)
{
	se_error_t ret = 0;
	//�������
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
	//����cmd��apdu_generate_key�ӿ�
	ret = apdu_generate_key (NULL,NULL,symkey);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_generate_keypair!\n");
		return ret;
	}
	return SE_SUCCESS;
}


/**
* @brief ����ECC/SM2Э����Կ
* @param [in] shared_key->alg �㷨����(ALG_ECDH_ECC256/ALG_ECDH_SM2/ALG_SM2_SM2)
* @param [in] shared_key->id  ���㷨����ΪALG_ECDH_ECC256/ALG_ECDH_SM2ʱSE��˽Կ��ʶ��KID
* @param [in] in_buf  ��������:���㷨����ΪALG_ECDH_ECC256/ALG_ECDH_SM2Ϊ�Է���Կ�����㷨����ΪALG_SM2_SM2ʱ���ο�������CIU98_B��SE[V3160]�û�ʹ���ֲᡷ
* @param [in] in_buf_len  �������ݳ���
* @param [in] if_return_key �Ƿ񷵻�shared key
* @param [in] if_return_s �Ƿ񷵻�sm2_sֵ
* @param [out] shared_key Э����Կ16�ֽ�
* @param [out] sm2_s У��ֵ64�ֽ�
* @return �μ�error.h
* @note no
* @see ����apdu_generate_shared_key
*/
se_error_t api_generate_shared_key (uint8_t *in_buf, uint32_t in_buf_len, unikey_t *shared_key, uint8_t *sm2_s, bool if_return_key, bool if_return_s)
{
	se_error_t ret = 0;
	//uint8_t key_info_tmp [2] = {0xF0,0x60};
	uint8_t int_buf_tmp[66] = {0xF0,0x60};//COS֧������Э�̳�ָ���㷨�ĶԳ���Կ���ù���SDK�ݲ����ţ�����Կid���㷨����д��Ϊ0xF0,0x60��
	uint32_t in_buf_len_tmp = 0;
	
	//�����ж�
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
  
	//���������apdu_generate_shared_key�ӿ�
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
* @brief ����ɾ��ָ����Կ��ʶ��KID��Ӧ����Կ
* @param [in] id ��ɾ����Կid
* @return �μ�error.h
* @note ɾ���̶�Ӧ����Կʱ��Ҫ��Ҫ��ð�ȫ�ļ���дȨ��
* @see ����apdu_del_key
*/
se_error_t api_del_key(uint8_t id)
{
	se_error_t ret = 0;
	
	//����cmd��apdu_delete_key�ӿ�
	ret = apdu_delete_key(id);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call api_del_key!\n");
		return ret;
	}

	return SE_SUCCESS;
}



/**
* @brief �ӿ�����Ӧ����Կ�����Ļ����ĵ���
* @param [in] ukey->alg �㷨����(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2/ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] ukey->id  Ӧ����Կid��������˽ԿKID������öԳ���Կkid��
* @param [in] inkey->alg �㷨����(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2/ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] inkey->id   ��Կ��ʶ������ΧΪ0x00-0xFF
* @param [in] inkey->type ��Կ����
* @param [in] inkey->val  ��Կֵ
* @param [in] inkey->val_len  �Ự��Կֵ����
* @param [in] if_cipher  true-���� false-����
* @param [in] if_trasns_key  �Ƿ���ô�����Կ���ܵ�����Կ
* @return �μ�error.h
* @note 1��	RSA����ʱ������PKCS1��λ��ʽ���в�λ��2��	�Գ��㷨����ʱ����������ǰ�����ֽڵ�Ld�����������ݳ��ȣ���Ȼ�����9797_M2��ʽ���Ϊ���鳤�ȡ�����ģʽ����ECBģʽ��3��	����̶�Ӧ����Կʱ��Ҫ��Ҫ��ð�ȫ�ļ���дȨ�ޡ�4��	��Կֵ��ʽ����Կֵ���Ȳμ�������CIU98_B��SE[V3160]�û�ʹ���ֲᡷ��5�����������ĵ������ô�����Կ���ܵ���ʱ��ukey��Ч��
* @see ����apdu_import_key
*/
se_error_t  api_import_key(unikey_t *ukey, unikey_t *inkey ,bool if_cipher,bool if_trasns_key)
{
	se_error_t ret = 0;
	//�������
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
	//�жϵ�����Կ����
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
	//����cmd��apdu_import_key�ӿ�
	ret = apdu_import_key (ukey,inkey,if_cipher,if_trasns_key);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_import_key!\n");
		return ret;
	}


	return SE_SUCCESS;


}


/**
* @brief �ӿ����������Ļ����ĵ���ʽ�����Գ���ʱӦ����Կ�ͷǶԳ���ʱӦ��˽Կ�������ĵ����̶�/��ʱ�ǶԳ�Ӧ�ù�Կ
* @param [in] ukey->alg �㷨����(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2/ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] ukey->id  Ӧ����Կid�������ù�ԿKID������öԳ���Կkid��
* @param [in] exkey->alg �㷨����(ALG_RSA1024_CRT/ALG_RSA2048_CRT/ALG_ECC256_NIST/ALG_SM2/ALG_AES128/ALG_DES128/ALG_SM4)
* @param [in] exkey->id   ��Կ��ʶ������ΧΪ0x00-0xFF
* @param [in] exkey->type ��Կ����
* @param [in] if_cipher  true-���� false-����
* @param [in] if_trasns_key  �Ƿ���ô�����Կ���ܵ�����Կ
* @param [out] exkey->val  ��Կֵ
* @param [out] exkey->val_len  �Ự��Կֵ����
* @return �μ�error.h
* @note 1��	RSA����ʱ������PKCS1��λ��ʽ���в�λ��2��	�Գ��㷨����ʱ����������ǰ�����ֽڵ�Ld�����������ݳ��ȣ���Ȼ�����9797_M2��ʽ���Ϊ���鳤�ȡ�����ģʽ����ECBģʽ��3�������ǶԳ���ʱӦ��˽Կʱ����Ҫ��ø�˽Կ��ʹ��Ȩ�ޡ�4��	��Կֵ��ʽ����Կֵ���Ȳμ�������CIU98_B��SE[V3160]�û�ʹ���ֲᡷ��5�����������ĵ������ô�����Կ���ܵ���ʱ��ukey��Ч��
* @see ����apdu_export_key
*/
se_error_t  api_export_key(unikey_t *ukey, unikey_t *exkey, bool if_cipher,bool if_trasns_key)
{
	se_error_t ret = 0;
	//�������
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
	//�жϵ�����Կ����
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

	if(exkey->type==PRI||exkey->type==SYM)//ֻ�ܵ�����ʱ˽Կ����ʱ�Գ���Կ
	{
	    if(exkey->id<0xF0)
		{
			LOGE("failed to api_export_key alg type!\n");
			return SE_ERR_PARAM_INVALID;
		}
	}
	
	//����cmd��apdu_export_key�ӿ�
	ret = apdu_export_key (ukey,exkey,if_cipher, if_trasns_key);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_export_key!\n");
		return ret;
	}

	return SE_SUCCESS;


}

/**
* @brief ��ȡ��Կ��Ϣ
* @param [in] if_app_key	�Ƿ��ȡӦ����Կ��Ϣ
* @param [out] out_buf  �������
* @param [out] out_buf_len  ������ݳ���
* @return �μ�error.h
* @note no
* @see ����apdu_get_key_info
*/
se_error_t  api_get_key_info (bool     if_app_key, uint8_t *out_buf,uint32_t *out_buf_len)
{
	se_error_t ret = 0;
	
	//����cmd��apdu_get_key_info�ӿ�
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
