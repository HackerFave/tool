/**@file  apdu.h
* @brief  apdu interface declearation	 
* @author  liangww
* @date  2021-04-28
* @version	V1.0
* @copyright  Copyright(C),CEC Huada Electronic Design Co.,Ltd.
*/
#ifndef _APDU_H_
#define _APDU_H_

#include "config.h"
#include "util.h"
#include "error.h"
#include "tpdu.h"
#include "types.h"
#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _WIN32
#define SOAPI
#else
#define SOAPI __declspec(dllexport) __stdcall
#endif

/** @addtogroup SE_Service
  * @{
  */

/** @addtogroup CMD
  * @brief Command layer.
  * @{
  */

/** @defgroup APDU APDU
  * @brief apdu command pack , unpack.
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup APDU_Exported_Types APDU Exported Types
  * @{
  */
  
#define KEYUSAGE    0
#define KID         1
#define KALG        2
#define KMODEL_LEN  3
#define KAUTH       4
#define KLEN_HIGH   6
#define KLEN_LOW    7

/**
  * @brief  控制类别预留
  */
typedef enum ctrl_type
{
	HARD_CTRL_RST =		0x10,	///<复位控制
	HARD_CTRL_RFU =		0x11,	///<硬控制预留
	
	SOFT_CTRL_RFU =		0x20,	///<命令控制
}ctrl_type;


/**
  * @brief  配置类型
  */
typedef enum config_type
{
	ECC_CURVE               = 0x00,
	STANDBY_MODE            = 0x01,
	SPI_RE_STANDBY_TIME     = 0x02,    ///<SPI接口重新进入standby时间
	SPI_DELAY_POWERDWON_TIME= 0x03
}config_type;


/**
  * @brief  模式类型
  */
typedef enum work_mode
{
	STANDBY    = 0x00,   ///<StandBy低功耗模式
	//POWERDOWN  = 0x01,    ///<PowerDown下电模式
	//WAIT  = 0x01,    ///<WAIT模式式
}work_mode;



 /**
   * @brief  安全级别
   */
 typedef enum privilege_level
{
	PRIVILEGE_LEVEL_0    = 0x00,   ///<安全级别0 
	PRIVILEGE_LEVEL_1    = 0x01,   ///<安全级别1 
	PRIVILEGE_LEVEL_2    = 0x02,   ///<安全级别2  
	PRIVILEGE_LEVEL_3    = 0x03	   ///<安全级别3           
}privilege_level;


/**
  * @brief	对称算法类型
  */
typedef enum sym_alg 
{
	ALG_AES128		= 0x60,   ///<AES-128算法
	ALG_AES192		= 0x61,   ///<AES-192算法
	ALG_AES256		= 0x62,   ///<AES-256算法
	ALG_DES128		=0x00,    //<3DES-128算法
	ALG_SM4 		=0x40,    ///<SM4算法
}sym_alg;


/**
  * @brief	非对称算法类型
  */
typedef enum asym_alg 
{	
	ALG_RSA1024_STANDAND  = 0x30,    ///<RSA1024算法，N-D形式
    ALG_RSA1024_CRT       = 0x31,    ///<RSA1024算法，五元组形式
  	ALG_RSA2048_STANDAND  = 0x32,    ///<RSA2048算法，N-D形式
    ALG_RSA2048_CRT       = 0x33,    ///<RSA2048算法，五元组形式
	ALG_ECC256_NIST       = 0x70,    ///<ECC-256-NIST算法
	ALG_ECC256_BRAINPOOL  = 0x71,    ///<ECC-256-BRAINPOOL 算法
	ALG_ECC256_SM2        = 0x72,    ///<sm2曲线参数的ECC-256算法
	ALG_SM2               = 0x50,    ///<SM2算法
}asym_alg;


/**
  * @brief	哈希算法类型
  */
typedef enum hash_alg
{
	ALG_SHA1       = 0x00,    ///<SHA-1算法
	ALG_SHA224     = 0x01,    ///<SHA-224算法
	ALG_SHA256     = 0x02,    ///<SHA-256算法
	ALG_SHA384     = 0x03,    ///<SHA-384算法
	ALG_SHA512     = 0x04,    ///<SHA-512算法
	ALG_SM3        = 0x05,    ///<SM3算法
	ALG_NONE       = 0xFF     ///<无哈希

}hash_alg;


/**
  * @brief	对称加密模式
  */
typedef enum sym_mode 
{
	SYM_MODE_CBC    = 0x00,    ///<CBC模式
	SYM_MODE_ECB    = 0x01,    ///<ECB模式
	SYM_MODE_CFB    = 0x02,    ///<CFB模式
	SYM_MODE_OFB    = 0x03     ///<OFB模式

}sym_mode;


/**
  * @brief	ECC曲线类型
  */
typedef enum ecc_curve
{
	NISTP256_R1           = 0x00,
	BRAINPOOLP_256_R1     = 0x01
}ecc_curve;


typedef enum key_type
{
	PRI            = 0x00,    
	PUB            = 0x01,
    PRI_PUB_PAIR   = 0x02,
    SYM            = 0x03,
} key_type;


/**
  * @brief	补位方式
  */
typedef enum padding_type
{
	PADDING_NOPADDING	= 0x00,     ///<不进行补位  
	PADDING_ISO9797_M1	= 0x01,   ///<ISO9797 M1补位，对称计算MAC时的补位模式
	PADDING_ISO9797_M2	= 0x02,   ///<ISO9797 M2补位，对称计算MAC时的补位模式      
	PADDING_PKCS7		= 0x03,       ///<PKCS7补位，对称计算加解密补位模式
	PADDING_PKCS1		= 0x04        ///<PKCS1补位，RSA签名验签的补位模式
}padding_type;


/**
  * @brief	SE信息类型
  */
typedef enum info_type
{
	CHIP_ID	         = 0x00,    
	PRODUCT_INFO	 = 0x01,   ///<产品信息 	
}info_type;

/**
  * @brief	衍生种子信息
  */
typedef struct
{
	uint8_t SM4_KS[16];		 ///<SM4密钥KS
	uint8_t SM4_KE[16];      ///<SM4密钥KE
	uint8_t SM2_A[64];      ///<SM2公钥A
	uint8_t SM2_P[64];      ///<SM2公钥P
}derive_seed_t;


/**
  * @brief	内部对称算法类型
  */
typedef enum sym_code
{
	DES_CBC_NOPADDING = 0x00,
	DES_ECB_NOPADDING  =0x01,
  AES_CBC_NOPADDING  =0x02,
	AES_ECB_NOPADDING  =0x03,
	DES_CBC_ISO9797_M1 =0x04,
	DES_CBC_ISO9797_M2 =0x05,
	AES_CBC_ISO9797_M1 =0x06,
	AES_CBC_ISO9797_M2 =0x07,
	SM4_CBC_NOPADDING  =0x10,
	SM4_ECB_NOPADDING  =0x11,
	SM4_CBC_ISO9797_M1 =0x14,
	SM4_CBC_ISO9797_M2 =0x15,
	SM1_CBC_NOPADDING  =0x20,
	SM1_ECB_NOPADDING  =0x21,
	SM1_CBC_ISO9797_M1 =0x24,
	SM1_CBC_ISO9797_M2 =0x25
}sym_code;


/**
  * @brief	内部非对称算法类型
  */
typedef enum asym_code
{
	RSA_NOPADDING     =0x00,
	RSA_SHA1          =0x01,
	RSA_SHA256        =0x02,
	RSA_SHA384        =0x03,
	RSA_SHA512        =0x04,
	SM2_SM3           =0x05,
	SM2               =0x15,
  ECC256_SHA256     =0x06,
	ECC256            =0x16,
	SM9_SM3           =0x07,
	SM9_SM4           =0x08
}asym_code;


/**
  * @brief	内部会话密钥算法类型
  */
typedef enum session_key_code
{
	SESSION_RSA =     0x00,//7-6bits 
	SESSION_SM2 =     0x20,
	SESSION_ECC =     0x40,
	SESSION_SM4 =     0x02,
	SESSION_DES =     0x04,
	SESSION_DES_128 = 0x05,
	SESSION_SM1 =     0x07,
	SESSION_AES_128 = 0x08,
}session_key_code;

/**
  * @brief	PIN属主类型
  */
enum pin_owner_type{
		ADMIN_PIN   = 0x00,    
        USER_PIN     = 0x01,   
};


enum shared_key_type{
		ALG_ECDH_ECC256    = 0x00,    
		ALG_ECDH_SM2       = 0x01,
        ALG_SM2_SM2        = 0x02,
};


#define MAX_KEY_LEN  912

/**
  * @brief	基本密钥信息
  */
typedef struct
{
	uint8_t  alg;              ///<算法类型
	uint8_t  id;               ///<密钥ID
	uint32_t val_len;          ///<密钥长度
	uint8_t  val[MAX_KEY_LEN]; ///<密钥值
	uint8_t  type;             ///<密钥类型
} unikey_t;


/**
  * @brief	 PIN参数信息
  */

typedef struct {
uint8_t  owner;
uint8_t  pin_value[16];
uint8_t  pin_len;
uint8_t  limit;
}pin_t;


//公钥密钥信息
typedef  unikey_t  pub_key_t;

//私钥密钥信息
typedef  unikey_t  pri_key_t;

//对称密钥信息
typedef  unikey_t  sym_key_t;


#define MAX_IV_LEN 16

extern unsigned char trans_key[16];

/**
  * @brief	对称算法参数
  */
typedef struct 
{      
	uint8_t mode;               ///<对称加密模式                          
	uint8_t iv_len;             ///<初始向量长度
	uint8_t iv[MAX_IV_LEN];      ///<初始向量      
	uint8_t padding_type;       ///<补位方式
	
} alg_sym_param_t;


/**
  * @brief	非对称算法参数
  */
typedef struct
{  
	uint32_t hash_type;       ///<哈希算法
	uint32_t padding_type;    ///<补位方式 
   
} alg_asym_param_t;

#define MAX_ID_LEN 128

/**
  * @brief	用户ID信息
  */
typedef struct
{
	uint32_t val_len;             ///<用户id值长度
    uint8_t val[MAX_ID_LEN];    ///<用户id值
} user_id_t;
//SE ID信息
typedef  user_id_t  se_id_t;


/**
  * @brief	SE信息
  */
typedef struct
{
	uint8_t chip_id[8];
	uint8_t product_info[8];      ///<产品信息
}se_info_t;

/**
  * @}
  */



/* Exported functions --------------------------------------------------------*/

/** @defgroup APDU_Exported_Functions APDU Exported Functions
* @{
*/


se_error_t apdu_sym_padding	(uint32_t padding, uint8_t *input, uint32_t input_len, uint8_t *output, uint32_t *output_len);

se_error_t apdu_sym_unpadding(uint32_t padding, uint8_t *input, uint32_t input_len, uint8_t *output, uint32_t *output_len);

uint32_t apdu_sym_padding_length(uint32_t alg, uint32_t padding, uint32_t input_len);

se_error_t apdu_switch_mode (work_mode  type);

se_error_t apdu_control(ctrl_type ctrlcode, const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *out_buf, uint32_t *out_buf_len);

se_error_t apdu_ext_auth(const uint8_t *in_buf, uint32_t in_buf_len);

se_error_t apdu_write_key(const uint8_t *in_buf, uint32_t in_buf_len, bool if_encrypt, bool if_update, bool if_update_mkey );

se_error_t  apdu_generate_key(pub_key_t *pub_key,pri_key_t *pri_key, sym_key_t * symkey );

se_error_t apdu_delete_key(uint8_t id);

se_error_t  apdu_import_key (unikey_t *ukey, unikey_t *inkey, bool if_cipher, bool if_trasns_key);

se_error_t  apdu_export_key (unikey_t *ukey, unikey_t *exkey, bool if_cipher, bool if_trasns_key);

se_error_t  apdu_get_key_info (bool if_app_key, uint8_t *out_buf,uint32_t *out_buf_len);

se_error_t apdu_sym_enc_dec (sym_key_t *key, alg_sym_param_t *sym_param, const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *out_buf, uint32_t *out_buf_len, bool if_first_block, bool if_last_block, bool if_enc);

se_error_t apdu_mac (sym_key_t *key, alg_sym_param_t *sym_param, const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *mac, uint32_t *mac_len, bool if_first_block, bool if_last_block, bool if_mac);

se_error_t apdu_asym_enc (pub_key_t *key, const alg_asym_param_t *asym_param,const uint8_t *in_buf,uint32_t in_buf_len, uint8_t * out_buf, uint32_t * out_buf_len, bool if_last_block);

se_error_t apdu_asym_dec (pri_key_t *key, const alg_asym_param_t *asym_param,const uint8_t *in_buf,uint32_t in_buf_len, uint8_t * out_buf, uint32_t * out_buf_len, bool if_last_block);

se_error_t apdu_asym_sign (pub_key_t *key, const alg_asym_param_t *asym_param,const uint8_t *in_buf,uint32_t in_buf_len, uint8_t *sign_buf, uint32_t * sign_buf_len , bool if_last_block);

se_error_t apdu_asym_verify (pub_key_t *key, const alg_asym_param_t *asym_param,const uint8_t *in_buf,uint32_t in_buf_len, uint8_t *sign_buf, uint32_t sign_buf_len , bool if_first_block, bool if_last_block);

se_error_t apdu_sm2_get_za (user_id_t* uid, pub_key_t *pub_key , uint8_t *za );

se_error_t apdu_digest (uint32_t alg, const uint8_t *in_buf, uint32_t in_buf_len, uint8_t *out_buf, uint32_t *out_buf_len, bool if_last_block);

se_error_t apdu_get_random  (uint32_t expected_len, uint8_t *random);

se_error_t  apdu_get_info (info_type type, se_info_t * info);

se_error_t  apdu_get_id (se_id_t *se_id );

se_error_t apdu_change_reload_pin(pin_t *pin, const uint8_t *in_buf, uint32_t in_buf_len, bool if_change_pin);

se_error_t apdu_verify_pin(pin_t *pin);

se_error_t  apdu_write_file(uint32_t offset, bool  if_encrypt, const uint8_t *in_buf, uint32_t in_buf_len);

se_error_t  apdu_read_file(uint32_t offset, bool  if_encrypt, uint32_t expect_len ,uint8_t *out_buf, uint32_t *out_buf_len);

se_error_t  apdu_get_file_info(bool  if_df,uint8_t *out_buf, uint32_t *out_buf_len);

se_error_t apdu_loader_init(uint8_t* image_addr);

se_error_t apdu_loader_program(uint8_t* image_addr);

se_error_t apdu_loader_checkprogram(uint8_t* image_addr);

se_error_t  apdu_select_file(uint32_t  fid);

se_error_t apdu_generate_shared_key (uint8_t *in_buf, uint32_t in_buf_len, unikey_t *shared_key, uint8_t *sm2_s, bool if_return_key, bool if_return_s);

se_error_t  apdu_v2x_gen_key_derive_seed (uint8_t derive_type,derive_seed_t* out_buf);

se_error_t  apdu_v2x_reconsitution_key (uint8_t *in_buf, uint32_t* in_buf_len,uint8_t *out_buf, uint32_t* out_buf_len);

se_error_t  apdu_write_SEID(se_id_t *se_id) ;



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

