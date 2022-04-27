/**@file  auth_test.c
* @brief  auth_test interface definition
* @author liangww
* @date  2021-05-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/


/***************************************************************************
* Include Header Files
***************************************************************************/
#include "auth_test.h"
#include "crypto.h"
#include "soft_alg.h"

/** @addtogroup SE_APP_TEST
  * @{
  */


/** @defgroup AUTH_TEST AUTH_TEST
  * @brief auth_test interface api.
  * @{
  */



/* Exported functions --------------------------------------------------------*/
/** @defgroup AUTH_TEST_Exported_Functions AUTH_TEST Exported Functions
  * @{
  */


/**
* @brief ��ȫ��֤ʾ��
* @param no
* @return �μ�error.h
* @note no
* @see api_get_random api_ext_auth
*/
se_error_t auth_test(void)
{

	/*
	���²��Զ��ڱ��ػ�����ʾ�������ο�
	1����api_get_random������һ���������
	2�����豸��Կ�Դ���������ܣ�������֤���ݡ�����������Ȳ�������鳤�ȣ����ڽ�β��0�������㡣
	*/
	se_error_t ret = 0;
	uint8_t random[16]={0};
	uint8_t mkey[16]={0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F};
    uint8_t out_buf[16]={0};
	uint32_t out_buf_len;
	//��ȡ16�ֽ������
    api_get_random (0x10, random);
    //�����������SM4���ܻ����֤����
	ex_sm4_enc_dec(random,0x10,mkey,0x10,ECB,ENCRYPT,out_buf,&out_buf_len);
	//�����豸��֤�ӿ�
     ret = api_ext_auth(out_buf,out_buf_len);
    if ( ret != SE_SUCCESS)
	{
		LOGE("failed to auth_test!\n");
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


