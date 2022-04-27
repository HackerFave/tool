/**@file  auth.c
* @brief  auth interface definition
* @author  liangww
* @date  2021-04-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "auth.h"


/** @addtogroup SE_Service
  * @{
  */

/** @addtogroup API 
  * @brief API layer.
  * @{
  */


/** @defgroup AUTH AUTH
  * @brief auth interface api.
  * @{
  */



/* Exported functions --------------------------------------------------------*/

/** @defgroup AUTH_Exported_Functions AUTH Exported Functions
  * @{
  */


/**
* @brief �豸��֤Ӧ��
* @param [in] in_buf      ��֤����
* @param [in] in_buf_len  ��֤���ݳ���
* @return �μ� error.h
* @note no
* @see ����apdu_ext_auth
*/
se_error_t api_ext_auth (const uint8_t *in_buf, uint32_t in_buf_len)

{
	se_error_t ret = 0;
	//������
	if(in_buf==NULL||in_buf_len==0)
	{  
		LOGE("failed to api_ext_auth input params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	//����CMD���apdu_ext_auth
	ret = apdu_ext_auth(in_buf,in_buf_len);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_ext_auth!\n");
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
 
