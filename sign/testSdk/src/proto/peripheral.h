/**@file  peripheral.h
* @brief  abstraction layer interface declearation    
* @author  zhengwd
* @date  2021-04-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/


#ifndef _PERIPHERAL_H_
#define _PERIPHERAL_H_

/**************************************************************************
* Global Macro Definition
***************************************************************************/

/** @addtogroup SE_Driver
  * @{
  */

/** @addtogroup PROTO 
  * @brief link protocol layer.
  * @{
  */


/** @addtogroup PERIPHERAL 
  * @{
  */



/* peripheral define --------------------------------------------------------*/
/** @defgroup peripheral_bus_driver_declearation peripheral bus driver declearation
  * @{
  */


#define MAX_PERIPHERAL_DEVICE                   4     /*!< ÿ��ͨ�Žӿ����֧�ֵ��豸��*/  
#define MAX_PERIPHERAL_BUS_DRIVER         4     /*!< ���֧�ֵ�ͨ�Žӿ���*/  



/**
  * @brief  Peripheral Type Structure definition
  */
typedef enum _peripheral_type {	
	PERIPHERAL_SPI,   ///< SPIͨ�Žӿڵ�����
	PERIPHERAL_I2C,   ///< I2Cͨ�Žӿڵ�����
	PERIPHERAL_UART, ///< UARTͨ�Žӿڵ�����
	PERIPHERAL_USB  ///< USBͨ�Žӿڵ�����
}peripheral_type;


/**
  * @brief  Peripheral Structure definition
  */
typedef struct _peripheral {
	peripheral_type type; ///< �����ͨ�Žӿ�����
	uint8_t id;	 ///< �����ʶ
}peripheral;

/** ��������ṹ������ */
#define PERIPHERAL_STRUCT_DEFINE(periph_type) struct peripheral_##periph_type  

/** ��������ʵ�������� */
#define PERIPHERAL_NAME(periph_type, id) g_peripheral_device_##periph_type##id

/** ����ָ������ʵ����ָ��*/
#define PERIPHERAL_POINTER(periph_type, id) p_peripheral_device_##periph_type##id


/** �������迪ʼ */
#define PERIPHERAL_DEFINE_BEGIN(periph_type, id) PERIPHERAL_STRUCT_DEFINE(periph_type) PERIPHERAL_NAME(periph_type, id) = {	\
	{periph_type, id},

/** ����������� */
#define PERIPHERAL_DEFINE_END()  };

/** ע������*/
//#define PERIPHERAL_REGISTER(periph_type, id, periph) const PERIPHERAL_STRUCT_DEFINE(periph_type) *PERIPHERAL_NAME(periph_type, id)_ = &periph
#define PERIPHERAL_REGISTER(periph_type, id, periph) PERIPHERAL_STRUCT_DEFINE(periph_type) *PERIPHERAL_POINTER(periph_type, id)= &periph


/** ��ȡ����*/
#define PERIPHERAL_GET(periph_type, id) PERIPHERAL_POINTER(periph_type, id)

/** �������� */
//#define PERIPHERAL_DECLARE(periph_type, id) extern const PERIPHERAL_STRUCT_DEFINE(periph_type) *PERIPHERAL_NAME(periph_type, id)_
#define PERIPHERAL_DECLARE(periph_type, id) extern PERIPHERAL_STRUCT_DEFINE(periph_type) *PERIPHERAL_POINTER(periph_type, id)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*�������� */


/**
  * @brief ����������������ӿ�
  */
typedef struct _peripheral_bus_driver {
	peripheral_type type;  ///< ��������
	peripheral    *periph[MAX_PERIPHERAL_DEVICE];  ///< ��������
	se_error_t (*init)		 (peripheral *periph);	  ///< ��ʼ�� 
	se_error_t (*deinit)	 (peripheral *periph);	  ///< ��ֹ��
	se_error_t (*open)      (peripheral *periph , uint8_t *rbuf, uint32_t *rlen);  ///< ��
	se_error_t (*close)     (peripheral *periph);	/* �ر�*/
	se_error_t (*transceive)(peripheral *periph ,  uint8_t *sbuf, uint32_t slen, uint8_t  *rbuf, uint32_t *rlen);  ///< �����շ�
	se_error_t (*reset)      (peripheral *periph , uint8_t *rbuf, uint32_t *rlen);  ///< ��λ
	se_error_t (*control)   (peripheral *periph , uint32_t ctrlcode, uint8_t *sbuf, uint32_t slen, uint8_t  *rbuf, uint32_t *rlen);	  ///< ��������
	se_error_t (*delay)   (peripheral *periph , uint32_t us);	  ///< ��ʱָ����΢��
	void *extra;  ///< Ԥ�����û�ʹ��
}peripheral_bus_driver;

/** ע���������� */
//#define PERIPHERAL_BUS_DRIVER_REGISTER(periph_type, driver) const  peripheral_bus_driver *g_peripheral_bus_driver_##periph_type = &driver
#define PERIPHERAL_BUS_DRIVER_REGISTER(periph_type, driver) peripheral_bus_driver *g_peripheral_bus_driver_##periph_type = &driver


/** ��ȡ������������ */
#define PERIPHERAL_BUS_DRIVER_GET(periph_type) g_peripheral_bus_driver_##periph_type

/** ����������������*/
//#define PERIPHERAL_BUS_DRIVER_DECLARE(periph_type) extern const  peripheral_bus_driver *g_peripheral_bus_driver_##periph_type
#define PERIPHERAL_BUS_DRIVER_DECLARE(periph_type) extern  peripheral_bus_driver *g_peripheral_bus_driver_##periph_type

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


#endif // _PERIPHERAL_DRIVER_H_

