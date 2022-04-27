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
  * @brief  APDU协议类型
  */
enum 
{
	ISO_CASE_1  = 0x01,  ///<CLA INS P1 P2 00
	ISO_CASE_2S = 0x02,  ///<CLA INS P1 P2 Le
	ISO_CASE_3S = 0x03,  ///<CLA INS P1 P2 Lc Data
	ISO_CASE_4S = 0x04,   ///<CLA INS P1 P2 Lc Data Le
	ISO_CASE_2E = 0x12,  ///<CLA INS P1 P2 Le（Le：3字节）
	ISO_CASE_3E = 0x13,  ///<CLA INS P1 P2 Lc Data（Lc：3字节）
	ISO_CASE_4E = 0x14,  ///<CLA INS P1 P2 Lc Data Le（Lc：3字节; Le: 2字节）

};

/**
  * @brief  APDU命令参数
  */
typedef struct
{
	uint8_t  isoCase;     ///<协议类型
	uint8_t  classbyte;   ///<CLA
	uint8_t  instruction; ///<INS
	uint8_t  p1;          ///<P1
	uint8_t  p2;          ///<P2
	uint32_t lc;          ///<LC
	uint32_t le;          ///<LE
} iso_command_apdu_t;

/**
  * @brief  CLA 类型
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
	CMD_ENTER_LOWPOWER=0x00,  ///<用于SE进入低功耗模式，等待唤醒
	CMD_EXTTERN_AUTH, ///<用于写入密钥或者PIN命令
	CMD_WRITE_KEY,///<用于设备认证
    CMD_GENERATE_KEY,///<用于生成一个对称应用密钥，或完整的非对称应用密钥对
	CMD_GEN_SHARED_KEY,///<用于产生shared key
	CMD_DEL_KEY,///<用于删除密钥
    CMD_EXPORT_KEY,///<用于导出密钥
	CMD_IMPORT_KEY,///<用于导入密钥
	CMD_GET_KEY_INFO,///<用于获取写入SE的密钥信息
	CMD_CHANGE_RELOAD_PIN ,///<用于修改管理员PIN和用户PIN，或者使用管理员PIN重置用户PIN
	CMD_VERIFY_PIN,///<用于PIN的验证
	CMD_CIPHER_DATA,///<用于对称加解密、MAC计算、MAC验证
	CMD_PKI_ENCIPHER,///<用于非对称加密
	CMD_PKI_DECIPHER,///<用于非对称解密
	CMD_COMPUTE_SIGNATURE, ///<用于非对称签名
	CMD_VERIFY_SIGNATURE, ///<用于非对称验签
    CMD_SM2_GET_ZA,///<用于获取SM2 ZA
	CMD_DIGEST,///<用于哈希计算
    CMD_GET_RANDOM,///<用于获取随机数
	CMD_SELECT_FILE,///<用于选择文件
    CMD_WRITE_FILE,///<用于写文件
	CMD_READ_FILE,///<用于读文件
	CMD_WRITE_SEID,///<用于设写SEID
	CMD_GET_INFO,///<用于获取SE信息
	CMD_GET_ID,///<用于获取SEID
	CMD_GET_RESPONSE,///<用于获取待返回的数据 
	CMD_GET_FILE_INFO,///<用于获取文件信息 
	CMD_LOADER_INIT,     ///<loader下载初始化
	CMD_LOADER_PROGRAM,     ///<loader下载
	CMD_LOADER_CHECKPROGRAM,    ///<loader下载校验下载image
	CMD_V2X_GENERATE_KEY_DERIVE_SEED,    ///<生成密钥衍生种子
	CMD_V2X_RECONSITUTION_KEY,    ///<密钥衍生流程中重构私钥
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
