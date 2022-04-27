//
// Created by DELL on 2022/4/15.
//

#ifndef CIU98B_IWALL_TBOX_SE_DEF_H
#define CIU98B_IWALL_TBOX_SE_DEF_H

/**
  * @brief	非对称算法类型
  */
typedef enum iw_asym_alg_t
{
    IW_ALG_RSA1024_CRT = 0x01,     ///<RSA1024算法，五元组形式
    IW_ALG_RSA2048_CRT = 0x03,     ///<RSA2048算法，五元组形式
    IW_ALG_ECC256 = 0x10,       ///<ECC-256算法
    IW_ALG_SM2 = 0x20,          ///<SM2算法
} iw_asym_alg;

/**
  * @brief	对称算法类型
  */
typedef enum iw_sym_alg_t
{
    IW_ALG_AES128 = 0x00,     ///<AES-128算法
    IW_ALG_SM4 = 0x20,        ///<SM4算法
} iw_sym_alg;

/**
  * @brief	对称加密模式
  */
typedef enum iw_sym_mode_t
{
    IW_SYM_MODE_CBC = 0x00,    ///<CBC模式
    IW_SYM_MODE_ECB = 0x01,    ///<ECB模式
} iw_sym_mode;

/**
  * @brief	哈希算法类型
  */
typedef enum iw_hash_alg_t
{
    IW_ALG_SHA1 = 0x00,    ///<SHA-1算法
    IW_ALG_SHA224 = 0x01,    ///<SHA-224算法
    IW_ALG_SHA256 = 0x02,    ///<SHA-256算法
    IW_ALG_SHA384 = 0x03,    ///<SHA-384算法
    IW_ALG_SHA512 = 0x04,    ///<SHA-512算法
    IW_ALG_SM3 = 0x05,    ///<SM3算法
    IW_ALG_NONE = 0xFF     ///<无哈希
} iw_hash_alg;

#endif //CIU98B_IWALL_TBOX_SE_DEF_H
