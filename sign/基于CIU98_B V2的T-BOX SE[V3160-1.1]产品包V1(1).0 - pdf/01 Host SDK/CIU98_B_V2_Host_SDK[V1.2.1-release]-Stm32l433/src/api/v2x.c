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

//V2x�ӿ�ʹ��˵��
//1���豸�˵���v2x_gen_key_derive_seed�ӿ�������Կ�������ӣ���(kS, kE, A, P)��EeRaCertRequest������PRA��
//2���豸�˽��յ�SCTij֮��ʹ���豸�˵�PCA֤�鹫Կ��֤ǩ����
//3������ǩ�ɹ��������v2x_reconsitution_key�ӿڻ��˽Կsij��������˽Կ����SEָ��kid�������������֤�顣

/**
* @brief ������Կ��������
* -# �жϲ����ĺϷ���
* -# ���������apdu_v2x_gen_key_derive_seed�ӿ�
* -# ���Ϲ����з�������᷵����Ӧ������
* @param [in]  derive_type    �������ͣ�Ԥ����Ĭ��0x00
* @param [out] out_buf    ����������Ϣ���μ�derive_seed_t
* @return �μ� error.h
* @note no
* @see ����apdu_gen_key_derive_seed
*/
se_error_t v2x_gen_key_derive_seed (uint8_t derive_type,derive_seed_t* out_buf)
{
	se_error_t ret = 0;
	
	if(out_buf==NULL)
      return SE_ERR_PARAM_INVALID;

	//���������apdu_v2x_gen_key_derive_seed�ӿ�
    ret =  apdu_v2x_gen_key_derive_seed(0x00, out_buf);
	if(ret!=SE_SUCCESS)
	{	
		LOGE("failed to call apdu_v2x_gen_key_derive_seed!\n");
		return ret;
	}

	return SE_SUCCESS;	

}


/**
* @brief �ع�˽Կ
* -# �жϲ����ĺϷ���
* -# ���������apdu_v2x_reconsitution_key�ӿ�
* -# ���Ϲ����з�������᷵����Ӧ������
* @param [in]  in_buf      �ع�������Ϣ(�ع�˽ԿID-1B|i-4B��j-4B|CTij)
* @param [in]  in_buf_len  �ع�������Ϣ����
* @param [out] out_buf     ����֤��
* @param [out] out_buf_len ����֤�鳤��
* @return �μ� error.h
* @note ������ִ��v2x_gen_key_derive_seed
* @see ����apdu_reconsitution_key
*/
se_error_t v2x_reconsitution_key (uint8_t *in_buf, uint32_t *in_buf_len,uint8_t *out_buf, uint32_t *out_buf_len)
{
	se_error_t ret = 0;

	if(in_buf==NULL||*in_buf_len==0||out_buf==NULL||out_buf_len==NULL)
        return SE_ERR_PARAM_INVALID;

	
	//���������apdu_v2x_reconsitution_key�ӿ�
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
 
