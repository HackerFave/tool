//
// Created by DELL on 2022/4/14.
//

#ifndef CIU98B_IWALL_TBOX_SE_API_H
#define CIU98B_IWALL_TBOX_SE_API_H

#include "iwall_tbox_se_def.h"

#define PARAM_INPUT_ERROR      0x30000009

/*
*名称： 设备初始化
*描述： 设备初始化
*参数：
*       speed              [IN] SPI 速率,默认1M(10000000)。
*       devname            [IN] 设备名称,默认"/dev/spidev1.0"。
*       reset_pin_num      [IN] 复位引脚标号
*返回值：0————初始化成功；其他————错误。
*说明：
*首次启动，必须调用
*/
int IWALL_device_init(unsigned int speed, unsigned char *devname, unsigned int reset_pin_num);

/*
*名称： 获取芯片ID
*描述： 获取SE ID
*参数：
*       id              [OUT] ID 内容。
*       id_len          [OUT] id_len 长度。
*返回值：0————初始化成功；其他————错误。
*说明：
*
*/
int IWALL_get_se_id(unsigned char *id, int *id_len);

/*
*名称： 关闭spi设备
*描述： 关闭spi设备
*参数： 无
*返回值：0————成功关闭；其他————错误。
*说明：
*/
int IWALL_se_close(void);

/*
*名称： 写入公私钥密钥对文件
*描述： 根据keyType 及keyID 写入不同的公私钥文件
*参数： 
*       keyID                        [IN] 私钥对应使用的keyID。  
*       keyType                      [IN] 0 为 签名私钥 , 1 为 解密私钥 。  
*       keyprivate                   [IN] 私钥首地址。
*       key_private_len              [IN] 私钥文件长度，必须为32字节。
*       keypubkey                    [IN] 私钥首地址。
*       key_pubkey_len               [IN] 私钥文件长度，必须为64字节。
*返回值：0————写入文件成功；其他————错误。
*说明：
*    keyId合法的keyID 范围 00~EF。
*    keyType 预留，这里暂未使用。
*/
//int IWALL_write_asym_keypair_with_keyID(unsigned int keyID, unsigned int keyType,
//                                        unsigned char *keyprivate, unsigned int key_private_len,
//                                        unsigned char *keypubkey, unsigned int key_pubkey_len);

/*
*名称： 写入对称密钥
*描述： 根据keyID及symAlg算法类型，写入不同的对称密钥
*参数： 
*       keyID                        [IN] 密钥对应使用的keyID。  
*       symAlg                       [IN] 加密算法[AES128|SM4|SM1]。
*       key                          [IN] 对称密钥。
*       keyLen                       [IN] 对称密钥长度。
*返回值：0————写入文件成功；其他————错误。
*说明：合法的keyID 范围 00~EF
*   固定密钥范围： 00 - EF
*   临时密钥范围： F2 ~ F9，FA(仅限SM2 协商之后的SharedKey)
*/
int IWALL_write_sym_key_with_keyID(unsigned int keyID, iw_sym_alg symAlg,
                                   unsigned char *key, unsigned int keyLen);

/*
*名称： 生成固定密钥对
*描述： 指定密钥标号位置生成固定密钥对
*参数：
*       keyId              [IN] 密钥对标识。
*       asymAlg            [IN] 签名公钥算法[RSA1024|RSA2048|ECC256|SM2]。
*       pubkey             [OUT] 输出的公钥。
*       pubkeyLen          [OUT] 公钥长度。
*返回值：0————成功；其他————错误。
*说明：
*  每次生成的密钥对不相同。只能生成一次。
*
*/
int IWALL_generate_keypair(unsigned int keyId, iw_asym_alg asymAlg,
                           unsigned char *pubkey, unsigned int *pubkey_len);

/*
*名称： 导出公钥
*描述： SM2 导出公钥
*参数：
*       keyId              [IN] 密钥对标识。
*       asymAlg
*       pubkey             [OUT] 输出的公钥。
*       pubkeyLen          [OUT] 公钥长度。
*返回值：0————成功；其他————错误。
*说明：
*  用于导出指定keyId的公钥。
*
*/
int IWALL_export_publickey(unsigned int keyId, iw_asym_alg asymAlg,
                           unsigned char *pubkey, unsigned int *pubkey_len);

/*
*名称： 签名接口定义[普通]
*描述： 数字签名
*参数：
*       keyId                   [IN] 签名密钥标识；当有多个签名密钥时，用来指定使用某个特定的签名密钥。
*       asymAlg                 [IN] 签名公钥算法[RSA1024|RSA2048|ECC256|SM2]。
*       hashAlg                 [IN] 签名哈希算法[SHA1|SHA256|SHA384|SHA512|SM3|NONE]。
*       plain                   [IN] 签名原文数据缓冲区指针。
*       plainLen                [IN] 签名原文数据长度,小于1024。
*       signature               [OUT] 签名值。
*       signatureLen            [OUT] 签名值长度。
*返回值：0————签名成功；其他————验签失败。
*说明：  如需ZA,ZA 需要在外面计算。
*  keyId 内部自动加 1;
*/
int IWALL_asym_sign(int keyID, iw_asym_alg asymAlg, iw_hash_alg hashAlg,
                    unsigned char *data, unsigned int dataLen,
                    unsigned char *signature, unsigned int *signatureLen);

/*
*名称： 签名验证接口定义[普通]
*描述： 验证数字签名
*参数：
*       keyId                   [IN] 签名密钥标识；当有多个签名密钥时，用来指定使用某个特定的签名密钥。
*       asymAlg                 [IN] 签名公钥算法[RSA1024|RSA2048|ECC256|SM2]。
*       hashAlg                 [IN] 签名哈希算法[SHA1|SHA256|SHA384|SHA512|SM3|NONE]。
*       plain                   [IN] 签名原文数据缓冲区指针。
*       plainLen                [IN] 签名原文数据长度,小于1024。
*       signature               [IN] 签名值。
*       signatureLen            [IN] 签名值长度。
*返回值：0————验签成功；其他————验签失败。
*说明：  无
*/
int IWALL_asym_verify(int keyID, iw_asym_alg asymAlg, iw_hash_alg hashAlg,
                      unsigned char *data, unsigned int dataLen,
                      unsigned char *signature, unsigned int signatureLen);

/*
*名称： 非对称算法加密接口定义[普通]
*描述： 非对称加密
*参数：
*       keyId                   [IN] 加密密钥标识。
*       asymAlg                 [IN] 签名公钥算法[RSA1024|RSA2048|ECC256|SM2]。
*       plain                   [IN] 原文数据缓冲区指针。
*       plainLen                [IN] 原文数据长度,RSA小于([1024|2048] / 8 - 11)。
*       cipher                  [OUT] 密文。
*       cipherLen               [OUT] 密文长度。
*返回值：0————成功；其他————失败。
*说明：
*  计算结果 [SM2:C1||C3||C2]
*/
int IWALL_asym_encrypt(int keyID, iw_asym_alg asymAlg,
                       unsigned char *plain, unsigned int plainLen,
                       unsigned char *cipher, unsigned int *cipherLen);

/*
*名称： 非对称算法解密接口定义[普通]
*描述： 非对称解密
*参数：
*       keyId                   [IN] 解密密钥标识。
*       asymAlg                 [IN] 签名公钥算法[RSA1024|RSA2048|ECC256|SM2]。
*       cipher                  [IN] 密文。
*       cipherLen               [IN] 密文长度。
*       plain                   [IN] 原文数据缓冲区指针。
*       plainLen                [IN] 原文数据长度,小于1024。
*返回值：0————签名成功；其他————验签失败。
*说明：
*  keyId 内部自动加 1;
*  要注意传入的密文的格式，否则会报错0x10006582
*/
int IWALL_asym_decrypt(int keyID, iw_asym_alg asymAlg,
                       unsigned char *cipher, unsigned int cipherLen,
                       unsigned char *plain, unsigned int *plainLen);

/*
*名称： 数字摘要计算接口定义[普通]
*描述： 数字摘要
*参数：
*       hashAlg               [IN] 数字摘要算法[SHA1|SHA256|SHA384|SHA512|SM3]。
*       data                  [IN] 数据。
*       dataLen               [IN] 数据长度。
*       hash                  [OUT] 摘要。
*       hashLen               [OUT] 摘要的长度。
*返回值：0————成功；其他————失败。
*说明：
*
*/
int IWALL_digest(iw_hash_alg hashAlg,
                 unsigned char *data, unsigned int dataLen,
                 unsigned char *hash, unsigned int *hashLen);

/*
*名称： 对称加密接口定义[普通]
*描述：
*参数：
*       keyId                   [IN] 加密密钥标识。
*       symAlg                  [IN] 加密算法[AES128|SM4|SM1]。
*       mode                    [IN] 加密模式[ECB|CBC]。
*       plain                   [IN] 原文数据缓冲区指针。
*       plainLen                [IN] 原文数据长度,小于1024。
*       cipher                  [OUT] 密文。
*       cipherLen               [OUT] 密文长度。
*返回值：0————成功；其他————失败。
*说明：
**  填充模式 PADDING_PKCS7
*
*/
int IWALL_sym_encrypt(int keyID, iw_sym_alg symAlg, iw_sym_mode mode,
                      unsigned char *iv, unsigned int iv_len,
                      unsigned char *plain, unsigned int plainLen,
                      unsigned char *cipher, unsigned int *cipherLen);

/*
*名称： 对称解密接口定义[普通]
*描述：
*参数：
*       keyId                   [IN] 加密密钥标识。
*       symAlg                  [IN] 加密算法[AES128|SM4|SM1]。
*       mode                    [IN] 加密模式[ECB|CBC]。
*       cipher                  [IN] 密文。
*       cipherLen               [IN] 密文长度。
*       plain                   [OUT] 原文数据缓冲区指针。
*       plainLen                [OUT] 原文数据长度,小于1024。
*返回值：0————成功；其他————失败。
*说明：
*
*/
int IWALL_sym_decrypt(int keyID, iw_sym_alg symAlg, iw_sym_mode mode,
                      unsigned char *iv, unsigned int iv_len,
                      unsigned char *cipher, unsigned int cipherLen,
                      unsigned char *plain, unsigned int *plainLen);

#endif //CIU98B_IWALL_TBOX_SE_API_H
