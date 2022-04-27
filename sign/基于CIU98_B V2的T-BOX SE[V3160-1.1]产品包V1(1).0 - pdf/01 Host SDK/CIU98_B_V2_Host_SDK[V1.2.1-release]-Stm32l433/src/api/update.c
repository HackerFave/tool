/**@file  update.c
* @brief  update interface definition
* @author  ade
* @date  2020-06-02
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "comm.h"
#include "update.h"
#include "info.h"
#include "info.h"


/** @addtogroup SE_Service
  * @{
  */

/** @addtogroup API 
  * @brief API layer.
  * @{
  */

/** @defgroup UPDATE UPDATE
  * @brief update interface api.
  * @{
  */



/* Exported functions --------------------------------------------------------*/

/** @defgroup UPDATE_Exported_Functions UPDATE Exported Functions
  * @{
  */



/**
* @brief ��������
* @param [in] image_addr �������ݵ�ַ
* @param [in] image_len �������ݳ���
* @return �μ�error.h
* @note no
* @see ����  apdu_get_info  apdu_enter_loader  api_reset  apdu_loader_init  apdu_loader_init  apdu_loader_checkdata
*/
se_error_t api_loader_download(uint8_t *image_addr, uint32_t image_len)
{
	se_error_t ret = SE_SUCCESS;
	uint8_t atr[32] = {0};
	uint32_t atr_len = 0;
	uint32_t download_len = 0;
	uint32_t addr_off = 0;
	uint8_t cos_info_version[4] = {0};
	//info_type type = PRODUCT_INFO;//PRODUCT_INFO  CHIP_ID
	//se_info_t info = {0};
  //  uint8_t in_len = 5;
	se_info_t info;
	//�������
	if(image_addr==NULL||image_len==0)
	{  
		LOGE("failed to api_loader_download input params!\n");
		return SE_ERR_PARAM_INVALID;
	}

	
	//��image�У���ȡCOS��Ϣ�ͳ�ʼ���淶�汾��
	memcpy(cos_info_version, image_addr+image_len-4, 4);  //*********����COS��Ϣ�ͳ�ʼ���淶�汾��**********************************
   do
   {

	//����Ƿ���Ҫ����
	//1����ȡATR���ж����Ʒ״̬bit6 cos��Ч��ʶλ�����Ϊ1����ִ����������
		ret = api_reset(atr, &atr_len);
		if(ret != SE_SUCCESS)
		{
			LOGE("failed to api_reset!\n");
			return ret;
		}
		//���atr��״̬��Ϣbit6 cos��Ч��ʶλ
		if(atr[9]&0x20)
		{
			break;
		}
		
	//2���ж�ATR���Ʒ״̬bit5 ������Ч��ʶλ�����Ϊ0����ִ����������
		//���atr��״̬��Ϣbit5 ������Ч��ʶλ
		if(!atr[9]&0x10)
		{
			break;
		}
			
	//3�����ATR��Ʒ״̬bit6Ϊ0��bit5Ϊ1����ִ��api_get_info��ȡ��OS��Ϣ�ͳ�ʼ���淶�汾��
	    ret =  api_get_info (PRODUCT_INFO, &info);
		if(ret != SE_SUCCESS)
		{
			LOGE("failed to api_transceive!\n");
			return ret;
		}
	//4�����������image�е�COS��Ϣ�ͳ�ʼ���淶�汾����ִ����������
	    if(memcmp(cos_info_version, info.product_info, 4)!=0 )
	    {
	    	break;
	    }
		else
		{
		    LOGE("need not update!\n");
			return ret;
		}
		

	}while(1);
		
	//���������Ч��
	download_len = (uint16_t)(image_addr[addr_off]<<8 | image_addr[addr_off+1]) + 2;
	if(download_len > image_len)
	{
		LOGE("failed to api_loader_download image length!\n");
		return SE_ERR_PARAM_INVALID;
	}
	//���س�ʼ��
	ret = apdu_loader_init(image_addr);
	if(ret != SE_SUCCESS)
	{
		port_printf("failed to apdu_loader_init!\n");
		return ret;
	}
	addr_off = download_len;

	do
	{
		//���������Ч��
		download_len = download_len + (uint16_t)(image_addr[addr_off]<<8 | image_addr[addr_off+1]) + 2;
		if(download_len > image_len)
		{
			LOGE("failed to api_loader_download image length!\n");
			return SE_ERR_PARAM_INVALID;
		}

		//����Ϊ0x0025����ǰ��image ��ȡ������Ϊ����У������
		if((uint16_t)(image_addr[addr_off]<<8 | image_addr[addr_off+1])==0x0025 )
		{
			//��������У������
			ret = apdu_loader_checkprogram(image_addr+addr_off);
			if(ret != SE_SUCCESS)
			{
				port_printf("failed to apdu_loader_download!\n");
				return ret;
			}

			//��image�У���ȡCOS�汾��Ϣ
			addr_off = download_len;
			download_len = download_len + (uint16_t)(image_addr[addr_off]<<8 | image_addr[addr_off+1]) + 2;
			if(download_len > image_len)
			{
				LOGE("failed to api_loader_download image length!\n");
				return SE_ERR_PARAM_INVALID;
			}

			memcpy(cos_info_version, image_addr+addr_off+2, 4);  //����image�ļ���COS��Ϣ�ͳ�ʼ���淶�汾��
			
			break;  //���ؽ������˳�
		}
		
		//����image����
		ret = apdu_loader_program(image_addr+addr_off);
		if(ret != SE_SUCCESS)
		{
			port_printf("failed to apdu_loader_download!\n");
			return ret;
		}
		addr_off = download_len;

	}while(1);
    //�����ӳ�1��ȷ�������ɹ�
    ret =  api_delay (1000000);//�ӳ�1��
	if(ret!=SE_SUCCESS)
	 {		 
		  LOGE("failed to api_connect\n");
		  return ret;
	 } 	
	//����Ƿ������ɹ�
	//1����ȡATR���ж����Ʒ״̬bit6 cos��Ч��ʶλ�����Ϊ1���򷵻ش�����
		ret = api_reset(atr, &atr_len);
		if(ret != SE_SUCCESS)
		{
			LOGE("failed to api_reset!\n");
			return ret;
		}
		//���atr��״̬��Ϣbit6 cos��Ч��ʶλ
		if(atr[9]&0x20)
		{
			LOGE("failed to cos valid!\n");
			return SE_ERR_UPDATE;
		}
		
	//2���ж�ATR���Ʒ״̬bit5 ������Ч��ʶλ�����Ϊ0���򷵻ش�����
		//���atr��״̬��Ϣbit5 ������Ч��ʶλ
		if(!atr[9]&0x10)
		{
			LOGE("failed to patch valid!\n");
			return SE_ERR_UPDATE;
		}
			
	//3�����ATR��Ʒ״̬bit6Ϊ0��bit5Ϊ1����ִ��api_get_info��ȡ��COS��Ϣ�ͳ�ʼ���淶�汾��
	   ret =  api_get_info (PRODUCT_INFO, &info);
		if(ret != SE_SUCCESS)
		{
			LOGE("failed to api_transceive!\n");
			return ret;
		}
		
	//4�����������image�е�COS�汾�źͳ�ʼ���淶�汾�ţ��򷵻ش�����
	   if(memcmp(cos_info_version, info.product_info, 4)!=0 )
	    {
	    	LOGE("failed to patch version!\n");
			return SE_ERR_UPDATE;
	    }
		

	return ret;
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


