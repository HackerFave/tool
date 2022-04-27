/**@file  proto_spi.h
* @brief  proto_spi interface declearation 
* @author  liangww
* @date  2021-04-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/
#ifndef _PROTO_SPI_H_
#define _PROTO_SPI_H_

/***************************************************************************
* Include Header Files
***************************************************************************/
#include <stdint.h>
#include "types.h"
#include "config.h"
#include "peripheral.h"
#include "ial_spi.h"
#include "util.h"
	


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
//extern peripheral_bus_driver g_proto_spi;
PERIPHERAL_BUS_DRIVER_DECLARE(PERIPHERAL_SPI); 



/** @addtogroup SE_Driver
  * @{
  */

/** @addtogroup PROTO 
  * @brief link protocol layer.
  * @{
  */


/** @addtogroup PROTO_SPI 
  * @{
  */


/* Private constants --------------------------------------------------------*/
/** @defgroup Proto_Spi_Private_Constants Proto_Spi Private Constants
  * @{
  */

/** @defgroup HED_SPI_TIME_PARAMS    HED SPI Communication Protocol Time Params
  * @{
  */
#define SPI_SEND_CS_WAKEUP_TIME                  210   /*!< 唤醒时间: 210us */  
#define SPI_SEND_DATA_OVER_WAIT_TIME              200 /*!< 协议参数T3: 200us */ 
#define SPI_RESET_POLL_SLAVE_INTERVAL_TIME        20  /*!< 协议参数T4: 20us */ 
#define SPI_BEFORE_RECEIVE_DATA_WAIT_TIME         30/*!< 协议参数T5: 30us */ 
#define SPI_SEND_BGT_TIME                         200  /*!< 帧保护时间: 200us */ 
	
#define SPI_RECEVIE_FRAME_WAIT_TIME               700   /*!< 帧等待时间BWT: 700ms */ 
#define SPI_COMM_MUTEX_WAIT_TIME                  200   /*!< 通信锁标识等待超时时间: 200ms */ 
#define SPI_PROTO_SE_RST_DELAY                  10000   /*!< 延迟10ms确保SE唤醒 */   
/**
  * @}
  */

/** @defgroup HED_SPI_FRAME_DEFINE    HED SPI  Protocol Frame Define
  * @{
  */

//帧类型定义
#define PIB_ACTIVE_FRAME                0x03   /*!< 激活帧*/        
#define PIB_INFORMATION_FRAME    0x0E  /*!< 信息帧*/      
#define PIB_PROCESS_FRAME             0x09  /*!< 过程帧*/

//激活帧类型定义
#define PIB_ACTIVE_FRAME_RESET          0xD3  /*!<RESET 激活帧*/        
#define PIB_ACTIVE_FRAME_RATR           0xE2  /*!<RATR 激活帧*/   
#define PIB_ACTIVE_FRAME_RATR_RESPONSE  0x3B 

//过程帧类型定义
#define PIB_PROCESS_FRAME_NAK_CRC_INFO      0x3C   /*!<NAK 过程帧*/
#define PIB_PROCESS_FRAME_NAK_OTHER_INFO      0x3D   /*!<NAK 过程帧*/      

#define PIB_PROCESS_FRAME_WTX_INFO      0x60  /*!<WTX 过程帧*/     


//字段在buf中的偏移
#define PIB_OFFSET        0
#define LEN_OFFSET        1
#define DATA_OFFSET      3

//协议字段长度定义
#define  FRAME_HEAD_LEN                3   /*!<帧头长度*/    
#define  ACTIVE_FRAME_DATA_EDC_LEN     4  /*!<激活帧数据及EDC总长度*/  
#define  ACTIVE_REQ_FRAME_LEN          7  /*!<激活帧总长度*/  
#define  PROCESS_FRAME_LEN             6  /*!<过程帧总长度*/  
#define  EDC_LEN                       2  /*!< EDC 长度*/  
#define  WAKEUP_DATA_LEN               3 /*!<唤醒字符长度长度*/ 

#define FRAME_LEN_MAX                 4101   /*!< 帧格式字段的最大长度: 3+4096+2 */ 
//#define FRAME_LEN_MAX                 265   /*!< 帧格式字段的最大长度: 3+5+255+1+1 */ 
#define FRAME_DATA_LEN_MAX  (FRAME_LEN_MAX - EDC_LEN-FRAME_HEAD_LEN)


#define PROTO_SPI_PFSM_DEFAULT          0   /*!<PFSM 默认值*/ 
#define PROTO_SPI_PFSS_DEFAULT          0     /*!<PFSS 默认值*/ 
#define PROTO_SPI_HBSM_DEFAULT          0   /*!<HBSM 默认值*/ 
#define PROTO_SPI_HBSS_DEFAULT          0     /*!<HBSS 默认值*/ 


#define PROTO_SPI_ATR_MAX_LEN         32   /*!<ATR 最大长度值*/ 
#define PROTO_SPI_RERESET_MAX_LEN      7  /*!<RESET帧返回值最大长度值*/ 
#define PROTO_SPI_RETRY_NUM           3    /*!<EDC校验错误最多重发次数*/ 
#define PROTO_SPI_WTX_NUM               20   /*!<WTX最多请求次数*/ 
/********************CRC 计算类型*******************/
#define	CRC_A 0
#define	CRC_B 1	// 所有不等于CRC_A的CRCType参数都认为是CRC_B

  
/**
  * @}
  */




/**
  * @}
  */



/* Exported types ------------------------------------------------------------*/
/** @defgroup Proto_Spi_Impl_Exported_Types Proto_Spi_Impl Exported Types
  * @{
  */

typedef struct
{
    uint32_t  start;	
    uint32_t  interval;		
}spi_timer_t;

/**
  * @brief  SPI Frame Structure definition
  */
typedef enum _SPI_FRMAE_TYPE
{
        SPI_WTX,              ///< WTX请求帧
        SPI_NAK,              ///< NAK请求帧
        SPI_INFO,            ///< 信息帧
        SPI_DEFAULT,     ///< RTU
}SPI_FRMAE_TYPE;

/**
  * @brief  SPI Param Structure definition
  */
typedef struct  {
	uint16_t pfsm;     ///< 主机支持的最大帧大小
	uint16_t pfss;      ///< 从机支持的最大帧大小
	uint16_t hbsm;     ///< 主机单块接收最大长度
	uint16_t hbss;      ///< 从机单块接收最小长度
	SPI_FRMAE_TYPE type;  ///< 帧类型
} spi_param_t;

/**
  * @brief  Proto SPI  Control Structure definition
  */
enum  PROTO_SPI_CTRL
{
	PROTO_SPI_CTRL_RST =		0x00000010,    ///< RST 复位控制
	PROTO_SPI_CTRL_OTHER =		0x00000011    ///< 其它控制
} ;

enum HED20_SPI_RESET_TYPE
 {
	 HED20_SPI_RESET_SE_RESET_NONE = 0x00,	 //<无RESET 请求
	 HED20_SPI_RESET_SE_RESET_REQ = 0x01,	//<有RESET 请求
 };

enum HED20_SPI_OPEN_TYPE
 {
	 HED20_SPI_OPEN_SE_RESET_NONE	 = 0x00,	 //<无RESET 请求
	 HED20_SPI_OPEN_SE_RESET_REQ = 0x01,	//<有RESET 请求
 };

#define spi_time_get_diff(a, b)\
                ((a >= b) ? (a - b) : (0xffffffff -(b - a)))

/**
  * @}
  */



/* Exported functions --------------------------------------------------------*/
/** @defgroup Proto_Spi_Exported_Functions Proto_Spi Exported Functions
  * @{
  */

extern se_error_t proto_spi_init(peripheral *periph);
extern se_error_t proto_spi_deinit(peripheral *periph) ;
extern se_error_t proto_spi_open(peripheral *periph , uint8_t *rbuf, uint32_t *rlen) ;
extern se_error_t proto_spi_close(peripheral *periph) ;
//extern se_error_t proto_spi_power_on(peripheral *periph , uint8_t *rbuf, uint32_t *rlen) ;
extern se_error_t proto_spi_transceive(peripheral *periph, uint8_t *sbuf, uint32_t	slen, uint8_t *rbuf, uint32_t *rlen);
extern se_error_t proto_spi_reset(peripheral *periph , uint8_t *rbuf, uint32_t *rlen);
extern se_error_t proto_spi_control(peripheral *periph , uint32_t ctrlcode, uint8_t *sbuf, uint32_t slen, uint8_t  *rbuf, uint32_t *rlen);
extern se_error_t proto_spi_delay(peripheral *periph , uint32_t us) ;


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



#endif //_PROTO_SPI_H_

