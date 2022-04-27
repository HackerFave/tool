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
  * @brief  �������Ԥ��
  */
typedef enum ctrl_type
{
	HARD_CTRL_RST =		0x10,	///<��λ����
	HARD_CTRL_RFU =		0x11,	///<Ӳ����Ԥ��
	
	SOFT_CTRL_RFU =		0x20,	///<�������
}ctrl_type;


/**
  * @brief  ��������
  */
typedef enum config_type
{
	ECC_CURVE               = 0x00,
	STANDBY_MODE            = 0x01,
	SPI_RE_STANDBY_TIME     = 0x02,    ///<SPI�ӿ����½���standbyʱ��
	SPI_DELAY_POWERDWON_TIME= 0x03
}config_type;


/**
  * @brief  ģʽ����
  */
typedef enum work_mode
{
	STANDBY    = 0x00,   ///<StandBy�͹���ģʽ
	//POWERDOWN  = 0x01,    ///<PowerDown�µ�ģʽ
	//WAIT  = 0x01,    ///<WAITģʽʽ
}work_mode;



 /**
   * @brief  ��ȫ����
   */
 typedef enum privilege_level
{
	PRIVILEGE_LEVEL_0    = 0x00,   ///<��ȫ����0 
	PRIVILEGE_LEVEL_1    = 0x01,   ///<��ȫ����1 
	PRIVILEGE_LEVEL_2    = 0x02,   ///<��ȫ����2  
	PRIVILEGE_LEVEL_3    = 0x03	   ///<��ȫ����3           
}privilege_level;


/**
  * @brief	�Գ��㷨����
  */
typedef enum sym_alg 
{
	ALG_AES128		= 0x60,   ///<AES-128�㷨
	ALG_AES192		= 0x61,   ///<AES-192�㷨
	ALG_AES256		= 0x62,   ///<AES-256�㷨
	ALG_DES128		=0x00,    //<3DES-128�㷨
	ALG_SM4 		=0x40,    ///<SM4�㷨
}sym_alg;


/**
  * @brief	�ǶԳ��㷨����
  */
typedef enum asym_alg 
{	
	ALG_RSA1024_STANDAND  = 0x30,    ///<RSA1024�㷨��N-D��ʽ
    ALG_RSA1024_CRT       = 0x31,    ///<RSA1024�㷨����Ԫ����ʽ
  	ALG_RSA2048_STANDAND  = 0x32,    ///<RSA2048�㷨��N-D��ʽ
    ALG_RSA2048_CRT       = 0x33,    ///<RSA2048�㷨����Ԫ����ʽ
	ALG_ECC256_NIST       = 0x70,    ///<ECC-256-NIST�㷨
	ALG_ECC256_BRAINPOOL  = 0x71,    ///<ECC-256-BRAINPOOL �㷨
	ALG_ECC256_SM2        = 0x72,    ///<sm2���߲�����ECC-256�㷨
	ALG_SM2               = 0x50,    ///<SM2�㷨
}asym_alg;


/**
  * @brief	��ϣ�㷨����
  */
typedef enum hash_alg
{
	ALG_SHA1       = 0x00,    ///<SHA-1�㷨
	ALG_SHA224     = 0x01,    ///<SHA-224�㷨
	ALG_SHA256     = 0x02,    ///<SHA-256�㷨
	ALG_SHA384     = 0x03,    ///<SHA-384�㷨
	ALG_SHA512     = 0x04,    ///<SHA-512�㷨
	ALG_SM3        = 0x05,    ///<SM3�㷨
	ALG_NONE       = 0xFF     ///<�޹�ϣ

}hash_alg;


/**
  * @brief	�ԳƼ���ģʽ
  */
typedef enum sym_mode 
{
	SYM_MODE_CBC    = 0x00,    ///<CBCģʽ
	SYM_MODE_ECB    = 0x01,    ///<ECBģʽ
	SYM_MODE_CFB    = 0x02,    ///<CFBģʽ
	SYM_MODE_OFB    = 0x03     ///<OFBģʽ

}sym_mode;


/**
  * @brief	ECC��������
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
  * @brief	��λ��ʽ
  */
typedef enum padding_type
{
	PADDING_NOPADDING	= 0x00,     ///<�����в�λ  
	PADDING_ISO9797_M1	= 0x01,   ///<ISO9797 M1��λ���ԳƼ���MACʱ�Ĳ�λģʽ
	PADDING_ISO9797_M2	= 0x02,   ///<ISO9797 M2��λ���ԳƼ���MACʱ�Ĳ�λģʽ      
	PADDING_PKCS7		= 0x03,       ///<PKCS7��λ���ԳƼ���ӽ��ܲ�λģʽ
	PADDING_PKCS1		= 0x04        ///<PKCS1��λ��RSAǩ����ǩ�Ĳ�λģʽ
}padding_type;


/**
  * @brief	SE��Ϣ����
  */
typedef enum info_type
{
	CHIP_ID	         = 0x00,    
	PRODUCT_INFO	 = 0x01,   ///<��Ʒ��Ϣ 	
}info_type;

/**
  * @brief	����������Ϣ
  */
typedef struct
{
	uint8_t SM4_KS[16];		 ///<SM4��ԿKS
	uint8_t SM4_KE[16];      ///<SM4��ԿKE
	uint8_t SM2_A[64];      ///<SM2��ԿA
	uint8_t SM2_P[64];      ///<SM2��ԿP
}derive_seed_t;


/**
  * @brief	�ڲ��Գ��㷨����
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
  * @brief	�ڲ��ǶԳ��㷨����
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
  * @brief	�ڲ��Ự��Կ�㷨����
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
  * @brief	PIN��������
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
  * @brief	������Կ��Ϣ
  */
typedef struct
{
	uint8_t  alg;              ///<�㷨����
	uint8_t  id;               ///<��ԿID
	uint32_t val_len;          ///<��Կ����
	uint8_t  val[MAX_KEY_LEN]; ///<��Կֵ
	uint8_t  type;             ///<��Կ����
} unikey_t;


/**
  * @brief	 PIN������Ϣ
  */

typedef struct {
uint8_t  owner;
uint8_t  pin_value[16];
uint8_t  pin_len;
uint8_t  limit;
}pin_t;


//��Կ��Կ��Ϣ
typedef  unikey_t  pub_key_t;

//˽Կ��Կ��Ϣ
typedef  unikey_t  pri_key_t;

//�Գ���Կ��Ϣ
typedef  unikey_t  sym_key_t;


#define MAX_IV_LEN 16

extern unsigned char trans_key[16];

/**
  * @brief	�Գ��㷨����
  */
typedef struct 
{      
	uint8_t mode;               ///<�ԳƼ���ģʽ                          
	uint8_t iv_len;             ///<��ʼ��������
	uint8_t iv[MAX_IV_LEN];      ///<��ʼ����      
	uint8_t padding_type;       ///<��λ��ʽ
	
} alg_sym_param_t;


/**
  * @brief	�ǶԳ��㷨����
  */
typedef struct
{  
	uint32_t hash_type;       ///<��ϣ�㷨
	uint32_t padding_type;    ///<��λ��ʽ 
   
} alg_asym_param_t;

#define MAX_ID_LEN 128

/**
  * @brief	�û�ID��Ϣ
  */
typedef struct
{
	uint32_t val_len;             ///<�û�idֵ����
    uint8_t val[MAX_ID_LEN];    ///<�û�idֵ
} user_id_t;
//SE ID��Ϣ
typedef  user_id_t  se_id_t;


/**
  * @brief	SE��Ϣ
  */
typedef struct
{
	uint8_t chip_id[8];
	uint8_t product_info[8];      ///<��Ʒ��Ϣ
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

