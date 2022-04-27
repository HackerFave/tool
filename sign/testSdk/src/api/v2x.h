/**@file  v2x.h
* @brief  extern v2x interface declearation	 
* @author  liangww
* @date  2021-10-21
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/


#ifndef _V2X_H_
#define _V2X_H_

#include "apdu.h"

/** @addtogroup SE_Service
  * @{
  */

/** @addtogroup API
  * @{
  */


/** @defgroup V2X
  * @brief v2x interface api.
  * @{
  */




/* Exported functions --------------------------------------------------------*/
/** @defgroup V2X_Exported_Functions V2X Exported Functions
  * @{
  */

se_error_t v2x_gen_key_derive_seed (uint8_t derive_type,derive_seed_t* out_buf);


se_error_t v2x_reconsitution_key (uint8_t *in_buf, uint32_t *in_buf_len,uint8_t *out_buf, uint32_t *out_buf_len);

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

#endif
