/**@file  comm_test.c
* @brief  comm_test interface declearation	 
* @author liangww
* @date  2021-05-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "comm_test.h"
#include "comm.h"


/** @addtogroup SE_APP_TEST
  * @{
  */



/** @defgroup COMM_TEST COMM_TEST
  * @brief comm_test interface api.
  * @{
  */


/* Exported functions --------------------------------------------------------*/
/** @defgroup COMM_TEST_Exported_Functions COMM_TEST Exported Functions
  * @{
  */


/**
* @brief ͨ��ʾ��
* @param no
* @return �μ�error.h
* @note no
* @see ���� api_register  api_select  api_connect  api_transceive  api_reset  api_disconnect
*/
se_error_t comm_test (void)
{

	
	uint8_t com_outbuf[300] = {0};
	uint32_t outlen =0;	
	uint8_t in_buf[5] = {0};
  uint32_t in_buf_len = 0;
	uint8_t out_buf[16] = {0};
  uint32_t out_buf_en = 0;	
	uint8_t atr[30] = {0};	
	uint32_t atr_len = 0;
	uint32_t i = 0;
	se_error_t ret = 0;
	
	
  /****************************************SEע�ᡢѡ�������*****************************************/	
	
	
	
	#if 1
	//---- 1. ע���豸----
	ret = api_register(PERIPHERAL_SPI, SPI_PERIPHERAL_SE0);
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to spi  api_register\n");
		 return ret;
	}
	
	//---- 2. ѡ���豸 ----
	ret = api_select(PERIPHERAL_SPI, SPI_PERIPHERAL_SE0);
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to  spi  api_select\n");
		 return ret;
	}

	#else
	//---- 1. ע�Ừ�� I2C ͨ��Э����豸----
	ret = api_register(PERIPHERAL_I2C, I2C_PERIPHERAL_SE0);
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to i2c api_register\n");
		 return ret;
	}
	
	//---- 2. ѡ���豸 ----
	ret = api_select(PERIPHERAL_I2C, I2C_PERIPHERAL_SE0);
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to i2c api_select\n");
		 return ret;
	}
	#endif

	
	
	//---- 3. ���ӣ���ȡatr ----
	ret = api_connect(com_outbuf, &outlen);
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to api_connect\n");
		 return ret;
	}
	port_printf("api_connect atr:\n");
	for(i = 0;i<outlen;i++)
	{
		port_printf("%02x",com_outbuf[i]);
	}
	port_printf("\n");
	
	
  /****************************************SE���ͽ���*****************************************/	
	
		//---- 4. ��ȡ8�ֽ������ ----
	in_buf_len = 0x05;
	memcpy(in_buf, "\x00\x84\x00\x00\x08", in_buf_len);
	
	ret = api_transceive(in_buf, in_buf_len, out_buf, &out_buf_en);
	if(ret != SE_SUCCESS)
	{
		return ret;
	}

	port_printf("api_transceive out_buf:\n");
	for(i = 0;i<out_buf_en;i++)
	{
		port_printf("%02x",out_buf[i]);
	}
	port_printf("\n");
	


  /****************************************SE�ȸ�λ������ATR*****************************************/	
	atr_len = 0;
	
	ret = api_reset(atr, &atr_len);
	if(ret != SE_SUCCESS)
	{
		LOGE("failed to api_reset\n");
		return ret;
	}
	
	
	
	 /****************************************SE�Ͽ�����*****************************************/	
	
	ret = api_disconnect ();
	if(ret!=SE_SUCCESS)
	{		
		 LOGE("failed to api_disconnect\n");
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




