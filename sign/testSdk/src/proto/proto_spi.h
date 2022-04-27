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
#define SPI_SEND_CS_WAKEUP_TIME                  210   /*!< ����ʱ��: 210us */  
#define SPI_SEND_DATA_OVER_WAIT_TIME              200 /*!< Э�����T3: 200us */ 
#define SPI_RESET_POLL_SLAVE_INTERVAL_TIME        20  /*!< Э�����T4: 20us */ 
#define SPI_BEFORE_RECEIVE_DATA_WAIT_TIME         30/*!< Э�����T5: 30us */ 
#define SPI_SEND_BGT_TIME                         200  /*!< ֡����ʱ��: 200us */ 
	
#define SPI_RECEVIE_FRAME_WAIT_TIME               700   /*!< ֡�ȴ�ʱ��BWT: 700ms */ 
#define SPI_COMM_MUTEX_WAIT_TIME                  200   /*!< ͨ������ʶ�ȴ���ʱʱ��: 200ms */ 
#define SPI_PROTO_SE_RST_DELAY                  10000   /*!< �ӳ�10msȷ��SE���� */   
/**
  * @}
  */

/** @defgroup HED_SPI_FRAME_DEFINE    HED SPI  Protocol Frame Define
  * @{
  */

//֡���Ͷ���
#define PIB_ACTIVE_FRAME                0x03   /*!< ����֡*/        
#define PIB_INFORMATION_FRAME    0x0E  /*!< ��Ϣ֡*/      
#define PIB_PROCESS_FRAME             0x09  /*!< ����֡*/

//����֡���Ͷ���
#define PIB_ACTIVE_FRAME_RESET          0xD3  /*!<RESET ����֡*/        
#define PIB_ACTIVE_FRAME_RATR           0xE2  /*!<RATR ����֡*/   
#define PIB_ACTIVE_FRAME_RATR_RESPONSE  0x3B 

//����֡���Ͷ���
#define PIB_PROCESS_FRAME_NAK_CRC_INFO      0x3C   /*!<NAK ����֡*/
#define PIB_PROCESS_FRAME_NAK_OTHER_INFO      0x3D   /*!<NAK ����֡*/      

#define PIB_PROCESS_FRAME_WTX_INFO      0x60  /*!<WTX ����֡*/     


//�ֶ���buf�е�ƫ��
#define PIB_OFFSET        0
#define LEN_OFFSET        1
#define DATA_OFFSET      3

//Э���ֶγ��ȶ���
#define  FRAME_HEAD_LEN                3   /*!<֡ͷ����*/    
#define  ACTIVE_FRAME_DATA_EDC_LEN     4  /*!<����֡���ݼ�EDC�ܳ���*/  
#define  ACTIVE_REQ_FRAME_LEN          7  /*!<����֡�ܳ���*/  
#define  PROCESS_FRAME_LEN             6  /*!<����֡�ܳ���*/  
#define  EDC_LEN                       2  /*!< EDC ����*/  
#define  WAKEUP_DATA_LEN               3 /*!<�����ַ����ȳ���*/ 

#define FRAME_LEN_MAX                 4101   /*!< ֡��ʽ�ֶε���󳤶�: 3+4096+2 */ 
//#define FRAME_LEN_MAX                 265   /*!< ֡��ʽ�ֶε���󳤶�: 3+5+255+1+1 */ 
#define FRAME_DATA_LEN_MAX  (FRAME_LEN_MAX - EDC_LEN-FRAME_HEAD_LEN)


#define PROTO_SPI_PFSM_DEFAULT          0   /*!<PFSM Ĭ��ֵ*/ 
#define PROTO_SPI_PFSS_DEFAULT          0     /*!<PFSS Ĭ��ֵ*/ 
#define PROTO_SPI_HBSM_DEFAULT          0   /*!<HBSM Ĭ��ֵ*/ 
#define PROTO_SPI_HBSS_DEFAULT          0     /*!<HBSS Ĭ��ֵ*/ 


#define PROTO_SPI_ATR_MAX_LEN         32   /*!<ATR ��󳤶�ֵ*/ 
#define PROTO_SPI_RERESET_MAX_LEN      7  /*!<RESET֡����ֵ��󳤶�ֵ*/ 
#define PROTO_SPI_RETRY_NUM           3    /*!<EDCУ���������ط�����*/ 
#define PROTO_SPI_WTX_NUM               20   /*!<WTX����������*/ 
/********************CRC ��������*******************/
#define	CRC_A 0
#define	CRC_B 1	// ���в�����CRC_A��CRCType��������Ϊ��CRC_B

  
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
        SPI_WTX,              ///< WTX����֡
        SPI_NAK,              ///< NAK����֡
        SPI_INFO,            ///< ��Ϣ֡
        SPI_DEFAULT,     ///< RTU
}SPI_FRMAE_TYPE;

/**
  * @brief  SPI Param Structure definition
  */
typedef struct  {
	uint16_t pfsm;     ///< ����֧�ֵ����֡��С
	uint16_t pfss;      ///< �ӻ�֧�ֵ����֡��С
	uint16_t hbsm;     ///< �������������󳤶�
	uint16_t hbss;      ///< �ӻ����������С����
	SPI_FRMAE_TYPE type;  ///< ֡����
} spi_param_t;

/**
  * @brief  Proto SPI  Control Structure definition
  */
enum  PROTO_SPI_CTRL
{
	PROTO_SPI_CTRL_RST =		0x00000010,    ///< RST ��λ����
	PROTO_SPI_CTRL_OTHER =		0x00000011    ///< ��������
} ;

enum HED20_SPI_RESET_TYPE
 {
	 HED20_SPI_RESET_SE_RESET_NONE = 0x00,	 //<��RESET ����
	 HED20_SPI_RESET_SE_RESET_REQ = 0x01,	//<��RESET ����
 };

enum HED20_SPI_OPEN_TYPE
 {
	 HED20_SPI_OPEN_SE_RESET_NONE	 = 0x00,	 //<��RESET ����
	 HED20_SPI_OPEN_SE_RESET_REQ = 0x01,	//<��RESET ����
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

