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
#define PROTO_I2C_RECEVIE_FRAME_WAIT_TIME     700    /*!< ֡�ȴ�ʱ��FWT: 700ms */ 
#define PROTO_I2C_COMM_MUTEX_WAIT_TIME         200   /*!< ͨ������ʶ�ȴ���ʱʱ��: 200ms */ 
	
#define PROTO_I2C_RECEVIE_POLL_TIME                          1000 /*!< ֡���ʱ��: 1000us */ 
#define PROTO_I2C_RECEIVE_TO_SEND_BGT                     200  /*!< ֡����ʱ��: 200us */ 
#define PROTO_I2C_SE_RST_DELAY                  10000   /*!< �ӳ�10msȷ��SE���� */   

/**
  * @}
  */

/** @defgroup HED_I2C_FRAME_DEFINE    HED I2C Protocol Frame Define
  * @{
  */
  
//Э���ֶγ��ȶ���
#define PROTO_I2C_PIB_SIZE                       1   /*!<Э��֡��ʽPIB �ֶγ���*/    
#define PROTO_I2C_LEN_SIZE                       2  /*!<Э��֡��ʽLEN �ֶγ���*/       
#define PROTO_I2C_EDC_SIZE                       2 /*!<Э��֡��ʽEDC �ֶγ���*/   
#define PROTO_I2C_PIB_LEN_SIZE               3 /*!<Э��֡��ʽPIB��LEN �ֶ��ܳ���*/  
#define PROTO_I2C_PIB_LEN_EDC_SIZE      5 /*!<Э��֡��ʽPIB��LEN��EDC �ֶ��ܳ���*/  
#define PROTO_I2C_RS_FRAME_SIZE           5 /*!<����Э���У�R֡��S ֡���ݳ���*/             
#define PROTO_I2C_RESET_RSP_SIZE          5 /*!<����Э���У�RESET������Ӧ���ݵĳ���*/  
#define PROTO_I2C_ATR_RSP_SIZE             32 /*!<����Э���У�ATR������Ӧ���ݵ���󳤶�*/  
#define PROTO_I2C_FRONT_FRAME_SIZE     7  /*!<��ʱ�洢֡ǰ����ֽ���*/            
#define PROTO_I2C_FRAME_MAX_SIZE        0x4000 /*!<֡������󳤶�*/  
	
#define PROTO_I2C_TIMEOUT_RETRY_MAX_NUM               1    /*!<���ճ�ʱ����ط�����*/ 
#define PROTO_I2C_CRC_ERROR_RETRY_MAX_NUM          3   /*!<EDCУ���������ط�����*/ 
	
//Э��֡ƫ�Ƶ�ַ����
#define PROTO_I2C_PIB_OFFSET                  (0)   /*!<Э��֡��ʽPIB ƫ�Ƶ�ַ*/    
#define PROTO_I2C_LEN_OFFSET                  (1)   /*!<Э��֡��ʽLENƫ�Ƶ�ַ*/    
#define PROTO_I2C_DATA_OFFSET               (3)   /*!<Э��֡��ʽDATAƫ�Ƶ�ַ*/    
		
//CRC �������Ͷ���
#define	CRC_A 0  /*!<CRC_A��ʽ*/ 
#define	CRC_B 1	/*!<���в�����CRC_A��CRCType��������Ϊ��CRC_B*/ 


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
	PROTO_I2C_S_FRAME_WTX  = 0xC0,   ///< S֡����ʱ����֡��
	PROTO_I2C_S_FRAME_RESET  = 0xE0,   ///<S֡��RESET֡��
	PROTO_I2C_I_FRAME_NO_LINK = 0x20,   ///<��Ϣ֡������ʽ��
	PROTO_I2C_I_FRAME_LINK = 0x00,   ///<��Ϣ֡����ʽ֡��
	PROTO_I2C_I_FRAME_ATR	= 0x30,   ///<��Ϣ֡��ATR����֡��
	PROTO_I2C_R_FRAME_ACK = 0x80,   ///<R֡(ACK֡)
	PROTO_I2C_R_FRAME_NAK = 0x81,   ///<R֡(NAK֡)
};

/**
  * @brief  I2C Open SE Structure definition
  */
enum HED20_I2C_OPEN_TYPE
{
	HED20_I2C_OPEN_SE_RESET_NONE	= 0x00, 	///<��RESET ����
	HED20_I2C_OPEN_SE_RESET_REQ = 0x01,    ///<��RESET ����
};

/**
  * @brief  I2C Reset SE Structure definition
  */
enum HED20_I2C_RESET_TYPE
{
	HED20_I2C_RESET_SE_RESET_NONE = 0x00,	///<��RESET ����
	HED20_I2C_RESET_SE_RESET_REQ = 0x01,   ///<��RESET ����
};

/**
  * @brief  I2C Control Mode Structure definition
  */
enum  PROTO_I2C_CTRL
{
	PROTO_I2C_CTRL_RST =		0x00000010,    ///< RST ��λ����, 
	PROTO_I2C_CTRL_OTHER =		0x00000011	  ///< ��������
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

