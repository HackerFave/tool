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
* @brief ��ȡSE��Ϣ
* @param [in] type	��Ϣ����(CHIP_ID/PRODUCT_INFO)
* @param [out] info->CHIP_ID  оƬ8�ֽ�Ψһ���к�
* @param [out] info->PRODUCT_INFO   ��Ʒ��Ϣ8�ֽ�
* @return �μ�error.h
* @note no
* @see ����apdu_get_info
*/
se_error_t  api_get_info (info_type type, se_info_t * info)
{
	se_error_t ret = 0;
	//�������
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
	//����cmd��apdu_get_info�ӿ�
	ret = apdu_get_info (type, info);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_get_info!\n");
		return ret;
	}

	return SE_SUCCESS;

}

/**
* @brief ��ȡSE ID
* @param [out] se_id->val  ���ص�SE��ID
* @param [out] se_id->val_len  ���ص�SE��ID����
* @return �μ�error.h
* @note no
* @see ����apdu_get_id
*/
se_error_t  api_get_id (se_id_t *se_id ) 
{
	se_error_t ret = 0;
	//�������
	if(se_id==NULL)
	{  
		LOGE("failed to apdu_get_id pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	//����cmd��apdu_get_id�ӿ�
	ret = apdu_get_id (se_id ) ;
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_get_id!\n");
		return ret;
	}
	return SE_SUCCESS;
}


/**
* @brief д��SE ID
* -# �жϲ����ĺϷ���
* -# ���������apdu_write_SEID�ӿ�
* -# ���Ϲ����з�������᷵����Ӧ������
* @param [in] se_id->val  д���SE��ID
* @param [in] se_id->val_len  д���SE��ID����
* @return �μ�error.h
* @note д��SEID֮ǰ��Ҫ����豸Ȩ��
* @see ����apdu_write_id
*/
se_error_t  api_write_id (se_id_t *se_id)
{
	se_error_t ret = 0;

	//�������
	if((se_id == NULL)||(se_id->val_len  == 0))
	{
		LOGE("failed to api_write_id params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	
	//����cmd��apdu_write_SEID�ӿ�
	ret = apdu_write_SEID (se_id);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_write_SEID!\n");
		return ret;
	}

	return SE_SUCCESS;

}



/**
* @brief ��ȡSDK 4�ֽ����Ͱ汾��
* @param [out] num  �汾��
* @return �μ�error.h
* @note no
* @see no
*/
se_error_t  api_sdk_version_number(uint32_t *num)
{

	//�������
	if(num==NULL)
	{  
		LOGE("failed to api_sdk_version_number pointer params!\n");
		return SE_ERR_PARAM_INVALID;
	}
	*num = SDK_VERSION_NUM;

	return SE_SUCCESS;
}

/**
* @brief ��ȡSDK�ַ������Ͱ汾��
* @param [out] str  �汾��
* @return �μ�error.h
* @note no
* @see no
*/
se_error_t  api_sdk_version_string (char *str)
{

	//�������
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
