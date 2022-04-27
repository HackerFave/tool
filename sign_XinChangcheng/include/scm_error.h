//
// Created by kong on 2022/2/18.
//

#ifndef GMSCM_SCM_ERROR_H
#define GMSCM_SCM_ERROR_H


#define SCM_OK                             0           /* SCM OK */
#define SCM_ERR                            0x0D000001  /* SCM ERROR */
#define SCM_INVALID_PARAMETER              0x0D000002  /* 无效的参数 */
#define SCM_MEMORY_ERR                     0x0D000003  /* 内存开辟失败 */
#define SCM_BUFFER_TOO_SMALL               0x0D000004  /* 缓冲区不足 */
#define SCM_FOLDER_CREATE_ERR              0x0D000005  /* 文件夹创建失败 */
#define SCM_FILE_CREATE_ERR                0x0D000006  /* 文件创建失败 */
#define SCM_FILE_NOT_FOUND                 0x0D000007  /* 文件没找到 */
#define SCM_FILE_SIZE_ERRS                 0x0D000008  /* 文件大小错误 */

#define SCM_NOT_INIT                       0x0D000010  /* 密码模块未初始化 */

#define SCM_UNSUPPORT_ALG                  0x0D000020  /* 不支持的算法 */
#define SCM_UNSUPPORT_HASH_ALG             0x0D000021  /* 不支持的签名算法 */
#define SCM_UNSUPPORT_METHOD               0x0D000022  /* 不支持的 METHOD */
#define SCM_GEN_KEY_ERR                    0x0D000023  /* 密钥生成失败 */
#define SCM_DIGEST_ERR                     0x0D000024  /* 数据摘要计算失败 */
#define SCM_CA_CERT_NOT_EXIST              0x0D000025  /* 证书不存在 */
#define SCM_CERT_CHAIN_NOT_EXIST           0x0D000026  /* 证书链不存在 */
#define SCM_VERIFY_CERT_ERR                0x0D000027  /* 证书校验失败 */
#define SCM_CERT_FORMAT_ERR                0x0D000028  /* 证书格式错误 */
#define SCM_CERT_EXPIRATION                0x0D000029  /* 证书已过期 */
#define SCM_CERT_REVOKE                    0x0D00002A  /* 证书被吊销 */
#define SCM_CERT_UNABLE_GET_ISSUER         0x0D00002B  /* 未获取到颁发者*/
#define SCM_ENGINE_LOAD_ERR                0x0D00002C  /* 引擎加载失败 */
#define SCM_ENCRYPT_ERR                    0x0D00002D  /* 解密失败 */
#define SCM_DECRYPT_ERR                    0x0D00002E  /* 解密失败 */

#endif //GMSCM_SCM_ERROR_H
