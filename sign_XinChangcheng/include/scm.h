//
// Created by kong on 2022/2/18.
//

#ifndef GMSCM_SCM_H
#define GMSCM_SCM_H

#include "scm_ext.h"
#include <curl/curl.h>

#ifdef _WIN32
#define DEVCTRLDLL __declspec(dllexport)
#else
#define DEVCTRLDLL
#endif

#ifdef __cplusplus
extern "C" {
#endif
/**************************************************
 * 初始化服务 *
 * IW_SDKVersion:        获取版本号
 * IW_InitSDK:           初始化SDK
 ***************************************************/
DEVCTRLDLL char *IW_SDKVersion(void);
DEVCTRLDLL int IW_InitSDK(const char *path);

/**************************************************
 * 密钥管理 *
 * IW_GenerateKey:         生成密钥对
 * IW_GenerateCSR:         生成 CSR
 * IW_DeleteKeyContainer:  清空容器
 * IW_DeleteAllContainer:  清空所有容器
 * IW_GetKeyBits:          导出密钥长度
 * IW_GetKeyStatus:        导出密钥状态
 * IW_ExportPublicKey:     导出公钥
 * IW_ExportPrivateKey:    导出私钥
 ***************************************************/
DEVCTRLDLL int IW_GenerateKey(const char *userID, const char *pin, int alg, int bits, unsigned char *pubkey, unsigned int *len);
DEVCTRLDLL int IW_DeleteKeyContainer(const char *userID);
DEVCTRLDLL int IW_DeleteAllKeyContainer(void);
DEVCTRLDLL int IW_ImportEncKey(const char *userID, const char *pin, char *encKey, unsigned int len);
DEVCTRLDLL int IW_GetKeyBits(const char *userID, int *bits);
DEVCTRLDLL int IW_GetKeyStatus(const char *userID, int *status);
DEVCTRLDLL int IW_ExportPublicKey(const char *userID, const char *pin, unsigned char *out, unsigned int *len);
DEVCTRLDLL int IW_ExportPrivateKey(const char *userID, const char *pin, unsigned char *out, unsigned int *len);

/**************************************************
 * 证书管理 *
 * IW_ImportCertificate: 导入证书，type: 0 签名证书， type: 1 加密证书 *
 * IW_ExportCertificate: 导出证书，type: 0 签名证书， type: 1 加密证书 *
 ***************************************************/
DEVCTRLDLL int IW_GenerateCSR(const char *userID, const char *pin, csr_info_t csrInfo,
                              char *out, unsigned int *len);

DEVCTRLDLL int IW_InitCACertChain(const char *CACerts);
DEVCTRLDLL int IW_InitOCSPCertChain(const char *OCSPCerts);
DEVCTRLDLL int IW_InitSSLCertChain(const char *SSLCerts);

DEVCTRLDLL int IW_ImportCertificate(const char* userID, int type, unsigned char* cert, unsigned int len);
DEVCTRLDLL int IW_ExportCertificate(const char* userID, int type, unsigned char* cert, unsigned int *len);
DEVCTRLDLL int IW_GetCertificatePublicKey(const unsigned char *cert, unsigned int len, unsigned char *pub, unsigned  int *pub_len);
DEVCTRLDLL int IW_VerifyCertStatus(const char *cert);

/**************************************************
 * 密码服务 *
 * IW_GenRandom:         生成随机数
 * IW_MessageDigest:     摘要算法
 * IW_SymmCompute:       对称加解密
 * IW_ASymmEncrypt:      非对称加密
 * IW_AsymmDecrypt:      非对称解密
 * IW_Sign:              数字签名
 * IW_VerifySignByCertificate 验证签名
 * return SCM_OK OR SCM_ERR_CODE
 ***************************************************/
DEVCTRLDLL int IW_GenRandom(unsigned char *random, unsigned int len);
DEVCTRLDLL int IW_SetRandomFunc(int (*bytes)(unsigned char *buf, int num));
DEVCTRLDLL void IW_RandomFuncFree(void);
DEVCTRLDLL int IW_Base64Encode(const unsigned char *in, unsigned int in_len, char *out, unsigned int *out_len);
DEVCTRLDLL int IW_Base64Decode(const char *in, unsigned int in_len, unsigned  char *out, unsigned int *out_len);
DEVCTRLDLL int IW_MessageDigest(int algo, const unsigned char *msg, unsigned int msg_len, unsigned char *hash, unsigned int *hash_len);

DEVCTRLDLL symm_ctx_st *IW_SymmCtxNew();
DEVCTRLDLL void IW_SymmCtxFree(symm_ctx_st *ctx);
DEVCTRLDLL int IW_SymmInit(symm_ctx_st *ctx, int alg, int mode, const unsigned char *key, int key_len, const unsigned char *iv, int enc);
DEVCTRLDLL int IW_SymmUpdate(symm_ctx_st *ctx, const unsigned char *in, int in_len, unsigned char *out, int *out_len);
DEVCTRLDLL int IW_SymmFinial(symm_ctx_st *ctx, const unsigned char *out, int *out_len);
DEVCTRLDLL int IW_SymmCompute(int alg, int mode, const unsigned char *key, int key_len, const unsigned char *iv, int enc,
                              const unsigned char *in, int in_len,
                              unsigned char *out, int *out_len);
DEVCTRLDLL int IW_ASymmEncrypt(const unsigned char *cert, unsigned int cert_len, const unsigned char *msg, unsigned int msg_len, unsigned char *cipher, unsigned int *cipher_len);
DEVCTRLDLL int IW_ASymmDecrypt(const char *userID, const char *pin, const unsigned char *cipher, unsigned int cipher_len, unsigned char *out, unsigned int *out_len);

DEVCTRLDLL int IW_Sign(const char *userID, char *pin, const unsigned char *msg, unsigned int msg_len, unsigned char *sign, unsigned int* sign_len, int hash_type);
DEVCTRLDLL int IW_VerifySignByCertificate(const unsigned char *msg, unsigned int msg_len, const unsigned char *sign, unsigned int sign_len, const unsigned char *cert, unsigned int cert_len, int hash_type);

DEVCTRLDLL CURLcode curl_easy_setopt_cert_and_prikey(CURL *curl, const char *userID, const char *pin);

#ifdef __cplusplus
};
#endif
#endif //GMSCM_SCM_H
