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
* @brief �ӿ�����д��PIN
* @param [in] pin->owner	PIN��������
* @param [in] pin->pin_value[16]  pin��ֵ
* @param [in] pin->pin_len  ȡֵΪ0x06-0x10
* @param [in] if_encrypt  �Ƿ����ô�����Կ���ܴ���
* @return �μ�error.h
* @note ��Ҫ��ð�ȫ�ļ���дȨ��
* @see ���� apdu_write_key
*/ 
se_error_t  api_write_pin(pin_t *pin, bool if_encrypt)
{
	se_error_t ret = 0;
	uint8_t inbuf[24]={0};
	uint32_t inbuf_len = 0;
	
    //�������
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
	//�����Կ������Ϣ
	inbuf[0]=0x00;//��Կ��;
	if(pin->owner==ADMIN_PIN)//��Կ��(����ԱPIN����00��;�û�PIN����01�� )
	{
		inbuf[1]=0x00;
	}
	else if(pin->owner==USER_PIN)
	{
		inbuf[1]=0x01;
	}
	inbuf[2] = pin->limit;//pin��֤��������
	inbuf[6]=0x00;//���ȣ����ֽ�
	inbuf[7]=pin->pin_len;//���ȣ����ֽ�
	memcpy(inbuf+8,pin->pin_value,pin->pin_len);
	//����cmd��apdu_write_key�ӿ�
	ret = apdu_write_key(inbuf,inbuf_len,if_encrypt,true,false);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_write_key!\n");
		return ret;
	}

	return SE_SUCCESS;

}
/**
* @brief �ӿ������޸Ĺ���ԱPIN���û�PIN
* @param [in] pin->owner  pin��������
* @param [in] pin->pin_value  ��pin��ֵ
* @param [in] pin->pin_len  ȡֵΪ0x06-0x10
* @param [in] in_buf  �µ�pinֵ
* @param [in] in_buf_len  ��pin����
* @return �μ�error.h
* @note no
* @see ���� apdu_change_reload_pin
*/ 
se_error_t api_change_pin (pin_t *pin, const uint8_t *in_buf, uint32_t in_buf_len)
{
	se_error_t ret = 0;
	//�������
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

	//����cmd��apdu_change_reload_pin�ӿ�
	ret = apdu_change_reload_pin (pin, in_buf, in_buf_len, true);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_change_reload_pin!\n");
		return ret;
	}

	return SE_SUCCESS;
}

/**
* @brief ���������û�PIN
* @param [in] pin->pin_value  ����Աpin��ֵ
* @param [in] pin->pin_len   ȡֵΪ0x06-0x10
* @param [in] in_buf  �û�pin�µ�ֵ
* @param [in] in_buf_len  �������ݳ���
* @return �μ�error.h
* @note no
* @see ���� apdu_change_reload_pin
*/
se_error_t api_reload_pin  (pin_t *pin, const uint8_t *in_buf, uint32_t in_buf_len)
{
	se_error_t ret = 0;
	//�������
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
	//����cmd��apdu_change_reload_pin�ӿ�
	ret = apdu_change_reload_pin (pin, in_buf, in_buf_len, false);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_change_reload_pin!\n");
		return ret;
	}

	return SE_SUCCESS;

}


/**
* @brief ������֤PIN
* @param [in] pin->owner  pin����������
* @param [in] pin->pin_value  ����֤pin��ֵ
* @param [in] pin->pin_len   ȡֵΪ0x06-0x10
* @return �μ�error.h
* @note no
* @see ���� apdu_verify_pin
*/
se_error_t api_verify_pin  (pin_t *pin)
{
	se_error_t ret = 0;
	//�������
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
	//����cmd��apdu_verify_pin�ӿ�
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

