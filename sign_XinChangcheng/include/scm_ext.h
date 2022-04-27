//
// Created by kong on 2022/2/25.
//

#ifndef GMSCM_SCM_EXT_H
#define GMSCM_SCM_EXT_H
#pragma pack(1)
typedef struct subject_info {
    char country[32];             /* Y 国家 */
    char stateOrProvince[32];     /* N 省份 */
    char locality[32];            /* N 地市 */
    char organization[64];        /* N 组织 */
    char organizationUnit[64];    /* N 机构 */
    char commonName[64];          /* Y  CN */
} subject_info_t;

/* PKCS9 5.4 */
typedef struct attribute_info {
    char challengePassword[32];         /* challengePassword */
    unsigned char alertName[32];          /* alertName */
    char keyUsage[128];                   /* nonRepudiation,digitalSignature */
} attribute_info_t;

typedef struct csr_info {
    subject_info_t subjectInfo;
    attribute_info_t attributeInfo;
} csr_info_t;

typedef struct symm_ctx {
    void *cipher;
} symm_ctx_st;

#pragma pack()

#define SCM_SHA1              0x00001001
#define SCM_SHA224            0x00002002
#define SCM_SHA256            0x00002003
#define SCM_SHA512            0x00002005
#define SCM_SM3               0x00003001
#define SCM_MD5               0x00004005

#define SCM_AES               0x10000010
#define SCM_SM4               0x10000020

#define SCM_MODE_ECB          0x10000001
#define SCM_MODE_CBC          0x10000002
#define SCM_MODE_CTR          0x10000003

#define SCM_RSA               0x20000010
#define SCM_ECC               0x20000020
#define SCM_SM2               0x20000030
#define SCM_ECC_prime256v1    0x20000100


/* 证书类型 */
#define FORMAT_UNKNOWN        0  /* UNKNOWN    ENCODE */
#define FORMAT_ASN1           1  /* ASN1       ENCODE */
#define FORMAT_PEM            2  /* PEM        ENCODE */
#define FORMAT_B64_PEM        3  /* BASE64 PEM ENCODE */

#endif //GMSCM_SCM_EXT_H
