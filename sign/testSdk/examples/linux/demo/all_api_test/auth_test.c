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
* @brief 安全认证示例
* @param no
* @return 参见error.h
* @note no
* @see api_get_random api_ext_auth
*/
se_error_t auth_test(void)
{

	/*
	以下测试都在本地环境演示，仅供参考
	1、用api_get_random向卡申请一个随机数。
	2、用设备密钥对此随机数加密，产生认证数据。若随机数长度不满足分组长度，则在结尾补0参与运算。
	*/
	se_error_t ret = 0;
	uint8_t random[16]={0};
	uint8_t mkey[16]={0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F};
    uint8_t out_buf[16]={0};
	uint32_t out_buf_len;
	//获取16字节随机数
    api_get_random (0x10, random);
    //对随机数进行SM4加密获得验证数据
	ex_sm4_enc_dec(random,0x10,mkey,0x10,ECB,ENCRYPT,out_buf,&out_buf_len);
	//调用设备认证接口
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


