/**@file  tpdu.h
* @brief  tpdu interface declearation	 
* @author  liangww
* @date  2021-04-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/


#ifndef _TPDU_H_
#define _TPDU_H_

#include "util.h"
#include "error.h"
#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup SE_Service
  * @{
  */

/** @addtogroup CMD
  * @brief Command layer.
  * @{
  */

/** @defgroup TPDU TPDU
  * @brief tpdu command pack , unpack, execute.
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup TPDU_Exported_Types TPDU Exported Types
  * @{
  */



/**
  * @brief  APDUЭ������
  */
enum 
{
	ISO_CASE_1  = 0x01,  ///<CLA INS P1 P2 00
	ISO_CASE_2S = 0x02,  ///<CLA INS P1 P2 Le
	ISO_CASE_3S = 0x03,  ///<CLA INS P1 P2 Lc Data
	ISO_CASE_4S = 0x04,   ///<CLA INS P1 P2 Lc Data Le
	ISO_CASE_2E = 0x12,  ///<CLA INS P1 P2 Le��Le��3�ֽڣ�
	ISO_CASE_3E = 0x13,  ///<CLA INS P1 P2 Lc Data��Lc��3�ֽڣ�
	ISO_CASE_4E = 0x14,  ///<CLA INS P1 P2 Lc Data Le��Lc��3�ֽ�; Le: 2�ֽڣ�

};

/**
  * @brief  APDU�������
  */
typedef struct
{
	uint8_t  isoCase;     ///<Э������
	uint8_t  classbyte;   ///<CLA
	uint8_t  instruction; ///<INS
	uint8_t  p1;          ///<P1
	uint8_t  p2;          ///<P2
	uint32_t lc;          ///<LC
	uint32_t le;          ///<LE
} iso_command_apdu_t;

/**
  * @brief  CLA ����
  */
enum
{ 
	CMD_CLA_CASE1 = 0x00,    
	CMD_CLA_CASE2 = 0x04,
	CMD_CLA_CASE3 = 0x80,
	CMD_CLA_CASE4 = 0x84,
};


/**
  * @brief  commandID
  */
enum 
{
	CMD_ENTER_LOWPOWER=0x00,  ///<����SE����͹���ģʽ���ȴ�����
	CMD_EXTTERN_AUTH, ///<����д����Կ����PIN����
	CMD_WRITE_KEY,///<�����豸��֤
    CMD_GENERATE_KEY,///<��������һ���Գ�Ӧ����Կ���������ķǶԳ�Ӧ����Կ��
	CMD_GEN_SHARED_KEY,///<���ڲ���shared key
	CMD_DEL_KEY,///<����ɾ����Կ
    CMD_EXPORT_KEY,///<���ڵ�����Կ
	CMD_IMPORT_KEY,///<���ڵ�����Կ
	CMD_GET_KEY_INFO,///<���ڻ�ȡд��SE����Կ��Ϣ
	CMD_CHANGE_RELOAD_PIN ,///<�����޸Ĺ���ԱPIN���û�PIN������ʹ�ù���ԱPIN�����û�PIN
	CMD_VERIFY_PIN,///<����PIN����֤
	CMD_CIPHER_DATA,///<���ڶԳƼӽ��ܡ�MAC���㡢MAC��֤
	CMD_PKI_ENCIPHER,///<���ڷǶԳƼ���
	CMD_PKI_DECIPHER,///<���ڷǶԳƽ���
	CMD_COMPUTE_SIGNATURE, ///<���ڷǶԳ�ǩ��
	CMD_VERIFY_SIGNATURE, ///<���ڷǶԳ���ǩ
    CMD_SM2_GET_ZA,///<���ڻ�ȡSM2 ZA
	CMD_DIGEST,///<���ڹ�ϣ����
    CMD_GET_RANDOM,///<���ڻ�ȡ�����
	CMD_SELECT_FILE,///<����ѡ���ļ�
    CMD_WRITE_FILE,///<����д�ļ�
	CMD_READ_FILE,///<���ڶ��ļ�
	CMD_WRITE_SEID,///<������дSEID
	CMD_GET_INFO,///<���ڻ�ȡSE��Ϣ
	CMD_GET_ID,///<���ڻ�ȡSEID
	CMD_GET_RESPONSE,///<���ڻ�ȡ�����ص����� 
	CMD_GET_FILE_INFO,///<���ڻ�ȡ�ļ���Ϣ 
	CMD_LOADER_INIT,     ///<loader���س�ʼ��
	CMD_LOADER_PROGRAM,     ///<loader����
	CMD_LOADER_CHECKPROGRAM,    ///<loader����У������image
	CMD_V2X_GENERATE_KEY_DERIVE_SEED,    ///<������Կ��������
	CMD_V2X_RECONSITUTION_KEY,    ///<��Կ�����������ع�˽Կ
};

/**
  * @}
  */



/* Exported functions --------------------------------------------------------*/
/** @defgroup TPDU_Exported_Functions TPDU Exported Functions
  * @{
  */


iso_command_apdu_t* tpdu_init(iso_command_apdu_t *command, int32_t isoCase, int32_t cla, int32_t ins,int32_t p1, int32_t p2,int32_t lc, int32_t le);


iso_command_apdu_t* tpdu_init_with_id	(iso_command_apdu_t *command, uint8_t commandID);


iso_command_apdu_t* tpdu_set_cla  (iso_command_apdu_t *command, uint32_t cla);


iso_command_apdu_t* tpdu_set_p1p2 (iso_command_apdu_t *command, uint8_t p1, uint8_t p2);


iso_command_apdu_t* tpdu_set_le(iso_command_apdu_t *command, uint32_t le);


se_error_t tpdu_pack(iso_command_apdu_t *command, uint8_t *output, uint32_t *output_len);


se_error_t tpdu_unpack(uint8_t *output, uint32_t *output_len);


se_error_t tpdu_send(uint8_t *input, uint32_t input_len,uint8_t *output, uint32_t *output_len);


se_error_t tpdu_execute	(iso_command_apdu_t *command, uint8_t *input, uint32_t input_len, uint8_t *output, uint32_t *output_len);


se_error_t tpdu_execute_no_response	(iso_command_apdu_t *command, uint8_t *input, uint32_t input_len);


se_error_t tpdu_change_error_code(uint16_t status_word);

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


#ifdef __cplusplus
}
#endif

#endif
