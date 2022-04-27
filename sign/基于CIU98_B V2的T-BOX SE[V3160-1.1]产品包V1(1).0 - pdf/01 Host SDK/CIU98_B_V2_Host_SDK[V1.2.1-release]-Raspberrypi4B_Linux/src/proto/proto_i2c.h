/******************************************************************************
 Copyright(C),CEC Huada Electronic Design Co.,Ltd.
 File name: 		proto_i2c.h
 Author:			zhengwd 
 Version:			V1.0	
 Date:			2021-04-26	
 Description:	        
 History:		

******************************************************************************/
#ifndef _PROTO_I2C_H_
#define _PROTO_I2C_H_ 

/***************************************************************************
* Include Header Files
***************************************************************************/
#include <stdint.h>
#include "types.h"
#include "config.h"
#include "peripheral.h"
#include "ial_i2c.h"
#include "util.h"


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
//extern peripheral_bus_driver g_proto_i2c;
PERIPHERAL_BUS_DRIVER_DECLARE(PERIPHERAL_I2C); 

/** @addtogroup SE_Driver
  * @{
  */

/** @addtogroup PROTO 
  * @brief link protocol layer.
  * @{
  */


/** @addtogroup PROTO_I2C 
  * @{
  */


/* Private constants --------------------------------------------------------*/
/** @defgroup Proto_I2c_Private_Constants Proto_I2c Private Constants
  * @{
  */

/** @defgroup HED_I2C_TIME_PARAMS	 HED I2C Communication Protocol Time Params
  * @{
  */
#define PROTO_I2C_RECEVIE_FRAME_WAIT_TIME     700    /*!< 帧等待时间FWT: 700ms */ 
#define PROTO_I2C_COMM_MUTEX_WAIT_TIME         200   /*!< 通信锁标识等待超时时间: 200ms */ 
	
#define PROTO_I2C_RECEVIE_POLL_TIME                          1000 /*!< 帧间隔时间: 1000us */ 
#define PROTO_I2C_RECEIVE_TO_SEND_BGT                     200  /*!< 帧保护时间: 200us */ 
#define PROTO_I2C_SE_RST_DELAY                  10000   /*!< 延迟10ms确保SE唤醒 */   

/**
  * @}
  */

/** @defgroup HED_I2C_FRAME_DEFINE    HED I2C Protocol Frame Define
  * @{
  */
  
//协议字段长度定义
#define PROTO_I2C_PIB_SIZE                       1   /*!<协议帧格式PIB 字段长度*/    
#define PROTO_I2C_LEN_SIZE                       2  /*!<协议帧格式LEN 字段长度*/       
#define PROTO_I2C_EDC_SIZE                       2 /*!<协议帧格式EDC 字段长度*/   
#define PROTO_I2C_PIB_LEN_SIZE               3 /*!<协议帧格式PIB、LEN 字段总长度*/  
#define PROTO_I2C_PIB_LEN_EDC_SIZE      5 /*!<协议帧格式PIB、LEN、EDC 字段总长度*/  
#define PROTO_I2C_RS_FRAME_SIZE           5 /*!<华大协议中，R帧或S 帧数据长度*/             
#define PROTO_I2C_RESET_RSP_SIZE          5 /*!<华大协议中，RESET请求响应数据的长度*/  
#define PROTO_I2C_ATR_RSP_SIZE             32 /*!<华大协议中，ATR请求响应数据的最大长度*/  
#define PROTO_I2C_FRONT_FRAME_SIZE     7  /*!<临时存储帧前面的字节数*/            
#define PROTO_I2C_FRAME_MAX_SIZE        0x4000 /*!<帧数据最大长度*/  
	
#define PROTO_I2C_TIMEOUT_RETRY_MAX_NUM               1    /*!<接收超时最多重发次数*/ 
#define PROTO_I2C_CRC_ERROR_RETRY_MAX_NUM          3   /*!<EDC校验错误最多重发次数*/ 
	
//协议帧偏移地址定义
#define PROTO_I2C_PIB_OFFSET                  (0)   /*!<协议帧格式PIB 偏移地址*/    
#define PROTO_I2C_LEN_OFFSET                  (1)   /*!<协议帧格式LEN偏移地址*/    
#define PROTO_I2C_DATA_OFFSET               (3)   /*!<协议帧格式DATA偏移地址*/    
		
//CRC 计算类型定义
#define	CRC_A 0  /*!<CRC_A格式*/ 
#define	CRC_B 1	/*!<所有不等于CRC_A的CRCType参数都认为是CRC_B*/ 


/**
  * @}
  */

/**
  * @}
  */


/* Exported types ------------------------------------------------------------*/
/** @defgroup Proto_I2c_Exported_Types Proto_I2c Exported Types
  * @{
  */

/**
  * @brief  I2C Frame Structure definition
  */
enum PROTO_I2C_FRAME_TYPE
{
	PROTO_I2C_S_FRAME_WTX  = 0xC0,   ///< S帧（延时请求帧）
	PROTO_I2C_S_FRAME_RESET  = 0xE0,   ///<S帧（RESET帧）
	PROTO_I2C_I_FRAME_NO_LINK = 0x20,   ///<信息帧（非链式）
	PROTO_I2C_I_FRAME_LINK = 0x00,   ///<信息帧（链式帧）
	PROTO_I2C_I_FRAME_ATR	= 0x30,   ///<信息帧（ATR请求帧）
	PROTO_I2C_R_FRAME_ACK = 0x80,   ///<R帧(ACK帧)
	PROTO_I2C_R_FRAME_NAK = 0x81,   ///<R帧(NAK帧)
};

/**
  * @brief  I2C Open SE Structure definition
  */
enum HED20_I2C_OPEN_TYPE
{
	HED20_I2C_OPEN_SE_RESET_NONE	= 0x00, 	///<无RESET 请求
	HED20_I2C_OPEN_SE_RESET_REQ = 0x01,    ///<有RESET 请求
};

/**
  * @brief  I2C Reset SE Structure definition
  */
enum HED20_I2C_RESET_TYPE
{
	HED20_I2C_RESET_SE_RESET_NONE = 0x00,	///<无RESET 请求
	HED20_I2C_RESET_SE_RESET_REQ = 0x01,   ///<有RESET 请求
};

/**
  * @brief  I2C Control Mode Structure definition
  */
enum  PROTO_I2C_CTRL
{
	PROTO_I2C_CTRL_RST =		0x00000010,    ///< RST 复位控制, 
	PROTO_I2C_CTRL_OTHER =		0x00000011	  ///< 其它控制
} ;


/**
  * @}
  */


/* Exported functions --------------------------------------------------------*/
/** @defgroup Proto_I2c_Exported_Functions Proto_I2c Exported Functions
  * @{
  */
extern se_error_t proto_i2c_init(peripheral *periph);
extern se_error_t proto_i2c_deinit(peripheral *periph);
extern se_error_t proto_i2c_open(peripheral *periph, uint8_t *rbuf, uint32_t *rlen);
extern se_error_t proto_i2c_close(peripheral *periph) ;
extern se_error_t proto_i2c_transceive(peripheral *periph, uint8_t *sbuf,    uint32_t  slen, uint8_t *rbuf, uint32_t *rlen);
extern se_error_t proto_i2c_reset(peripheral *periph, uint8_t *rbuf, uint32_t *rlen);
extern se_error_t proto_i2c_control(peripheral *periph , uint32_t ctrlcode, uint8_t *sbuf, uint32_t slen, uint8_t  *rbuf, uint32_t *rlen);
extern se_error_t proto_i2c_delay(peripheral *periph , uint32_t us); 

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

