/**@file  update_test.c
* @brief  update_test interface definition
* @author liangww
* @date  2021-05-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

/***************************************************************************
* Include Header Files
***************************************************************************/
#include <stdint.h>

#include <stdio.h>

#include "auth.h"
#include "soft_alg.h"
#include "crypto.h"
#include "update.h"
#include "update_test.h"
#include "auth_test.h"
#include"string2byte.h"


extern const uint8_t update_image[];
extern uint32_t get_image_len(void);
/**
* @brief 程序升级测试
* @param no
* @return 参见error.h
* @note no
* @see no
*/
se_error_t update_test(void)
{
	
	se_error_t ret = 0;
	const uint8_t* pimage= update_image;
	uint32_t image_len = 0; 
		
	//loader数据
	image_len = get_image_len();
	ret = api_loader_download( (uint8_t* )pimage, image_len);
	if(ret!=SE_SUCCESS)
	{
		LOGE("Failed to api_loader_download\n");  
		return ret;
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

