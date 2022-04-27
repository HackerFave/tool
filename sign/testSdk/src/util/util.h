/**@file  util.h
* @brief  Header file of uitl module
* @author  zhengwd
* @date  2021-5-12
* @version  V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/
#ifndef _UTIL_H_
#define _UTIL_H_

/***************************************************************************
* Include Header Files
***************************************************************************/
#include <stdint.h>
#include <string.h>
#include "types.h"





/** @addtogroup SE_Driver
  * @{
  */

/** @addtogroup UTIL
  * @{
  */


/* Private constants --------------------------------------------------------*/
/** @defgroup UTIL_Queue_Private_Constants UTIL_Queue Private Constants
  * @{
  */

#define DEQUE_MAX_SIZE   1024 /*!< ˫�˶�����������洢�ռ� ,�ɸ�������MCU����Դ������̶Ƚ���������󲻳���1024*/

//apdu������ְ���С
#define DBLOCK DEQUE_MAX_SIZE-20-4-24-16

//Le��󳤶�
#define LE_MAX_LEN DEQUE_MAX_SIZE-20-5-2
//#define LE_MAX_LEN 52
/**
  * @}
  */



/* Exported types ------------------------------------------------------------*/
/** @defgroup UTIL_Queue_Exported_Types UTIL_Queue Exported Types
  * @{
  */


/**
  * @brief  Double Queue Structure definition
  */
	typedef struct 
	{
		uint16_t front_node;	///< ����ͷ
		uint16_t rear_node; 	///< ����β
		uint16_t q_buf_len; 	///< ���е�ǰ���ݳ���
		uint16_t capacity;		///< ��������
		uint8_t q_buf[DEQUE_MAX_SIZE];	///< ��������
	}double_queue_node,*double_queue;


/**
  * @}
  */



/** @defgroup UTIL_Timer_Exported_Types UTIL_Timer Exported Types
  * @{
  */


/**
  * @brief  Timer Structure definition
  */
	typedef struct
	{
		uint32_t  interval; ///< ʱ����
		uint32_t  tv_msec; 	  ///< ʱ�����ֵ: ����
		int64_t    tv_sec; 	  ///< ʱ��ֵ: ��
		int64_t    tv_usec; 	 ///< ʱ��ֵ: ΢��
	}util_timer_t;

/**
  * @}
  */



/* Exported functions --------------------------------------------------------*/
/** @defgroup UTIL_Queue_Exported_Functions UTIL_Queue Exported Functions
  * @{
  */

extern double_queue util_queue_init(double_queue d_queue);
extern void util_queue_front_push(uint8_t*in_buf,uint32_t in_buf_len, double_queue d_queue);
extern void util_queue_front_pop(uint32_t in_buf_len ,double_queue d_queue);
extern void util_queue_rear_push(uint8_t*in_buf,uint32_t in_buf_len, double_queue d_queue);
extern void util_queue_rear_pop(uint32_t in_buf_len ,double_queue d_queue);
extern uint32_t util_queue_size (double_queue d_queue);



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
