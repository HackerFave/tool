/**@file  update_test.h
* @brief  update_test interface declearation  
* @author liangww
* @date  2021-05-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/

#ifndef _UPDATE_TEST_H_
#define _UPDATE_TEST_H_

#include "update_test.h"

/** @addtogroup SE_APP_TEST
  * @{
  */



/** @addtogroup UPDATE_TEST 
  * @brief update test interface functions.
  * @{
  */


/* Private constants --------------------------------------------------------*/
/** @defgroup UPDATE_TEST_Private_Constants UPDATE_TEST Private Constants
  * @{
  */

#define   SERVER_TYPE_SCP_AUTH                    0x00    /*!< 通道认证数据 */
#define   SERVER_TYPE_LOADER_AUTH             0x01   /*!< 回退loader授权指令*/    
#define   SE_COS_MAX_SIZE                               118784   /*!< SE COS最大存储空间*/ 


/**
  * @}
  */


/* Exported functions --------------------------------------------------------*/
/** @defgroup UPDATE_TEST_Exported_Functions UPDATE_TEST Exported Functions
  * @{
  */


//se_error_t scp02_server_get_auth_data(uint8_t auth_type, privilege_level level, scp_key *key, uint8_t *inbuf, uint32_t inbuf_len, uint8_t *outbuf, uint32_t *outbuf_len);

se_error_t update_test(void);


/**
  * @}
  */


/**
  * @}
  */


/**
  * @}
  */
	



#endif
