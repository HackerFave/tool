//
// Created by DELL on 2022/4/14.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iwall_tbox_se_api.h"

//SE临时公钥KID(0xF0)
//SE临时私钥KID(0xF1)
//SE会话密钥KID(0xF2~0xF9)
//SE国密SM2协商后的SharedKey(0xFA)

#define DUMP_PRINT(d, l)\
{\
    unsigned char *p = (unsigned char *) d;\
    int i = 0;\
    do\
    {\
        printf("%02x", p[i]);\
        i++;\
    } while (i < l);\
    printf("\n");\
}

/**
 * 数字摘要建议通过软算法计算
 */
int digest_test()
{
    unsigned char *data = NULL;
    unsigned int dataLen = 256;
    unsigned char md[64] = {0};
    unsigned int mdLen = 64;

    int ret = 0;
    data = calloc(dataLen, 1);

    mdLen = 64;
    memset(md, 0, mdLen);
    ret = IWALL_digest(IW_ALG_SHA1, data, dataLen, md, &mdLen);
    printf("IWALL_digest(SHA1): ret=%x\n", ret);
    DUMP_PRINT(md, mdLen)

    mdLen = 64;
    memset(md, 0, mdLen);
    ret = IWALL_digest(IW_ALG_SHA256, data, dataLen, md, &mdLen);
    printf("IWALL_digest(SHA256): ret=%x\n", ret);
    DUMP_PRINT(md, mdLen)

    mdLen = 64;
    memset(md, 0, mdLen);
    ret = IWALL_digest(IW_ALG_SHA384, data, dataLen, md, &mdLen);
    printf("IWALL_digest(SHA384): ret=%x\n", ret);
    DUMP_PRINT(md, mdLen)

    mdLen = 64;
    memset(md, 0, mdLen);
    ret = IWALL_digest(IW_ALG_SHA512, data, dataLen, md, &mdLen);
    printf("IWALL_digest(SHA512): ret=%x\n", ret);
    DUMP_PRINT(md, mdLen)

    mdLen = 64;
    memset(md, 0, mdLen);
    ret = IWALL_digest(IW_ALG_SM3, data, dataLen, md, &mdLen);
    printf("IWALL_digest(SM3): ret=%x\n", ret);
    DUMP_PRINT(md, mdLen)

    if (data)
    {
        free(data);
    }

    return 0;
}

/**
 * 对称算法测试
 */
int sym_test()
{
    unsigned char *data = NULL;
    unsigned int dataLen = 256;
    unsigned char *enc = NULL;
    unsigned int encLen = 256;
    unsigned char *dec = NULL;
    unsigned int decLen = 256;

    int ret = 0;
    //SE固定密钥KID(0x00~0xEF)
    int keyId = 0x60;
    unsigned char key[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                           0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    unsigned char iv[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                          0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    data = calloc(dataLen, 1);
    encLen = dataLen + 16;//PKCS7PADDING需要增加长度
    enc = calloc(encLen, 1);
    decLen = dataLen;
    dec = calloc(decLen, 1);

    memset(data, 0x31, dataLen);

    keyId++;
    ret = IWALL_write_sym_key_with_keyID(keyId, IW_ALG_AES128, key, sizeof(key));
    printf("IWALL_write_sym_key_with_keyID(AES): ret=%x\n", ret);
    encLen = dataLen + 16;
    decLen = dataLen;
    memset(enc, 0x00, encLen);
    memset(dec, 0x00, decLen);
    ret = IWALL_sym_encrypt(keyId, IW_ALG_AES128, IW_SYM_MODE_ECB, NULL, 0,
                            data, dataLen, enc, &encLen);
    printf("IWALL_sym_encrypt(AES): ret=%x\n", ret);
    DUMP_PRINT(enc, encLen)
    ret = IWALL_sym_decrypt(keyId, IW_ALG_AES128, IW_SYM_MODE_ECB, NULL, 0,
                            enc, encLen, dec, &decLen);
    printf("IWALL_sym_decrypt(AES): ret=%x\n", ret);
    DUMP_PRINT(dec, decLen)

    keyId++;
    ret = IWALL_write_sym_key_with_keyID(keyId, IW_ALG_SM4, key, sizeof(key));
    printf("IWALL_write_sym_key_with_keyID(SM4): ret=%x\n", ret);
    encLen = dataLen + 16;
    decLen = dataLen;
    memset(enc, 0x00, encLen);
    memset(dec, 0x00, decLen);
    ret = IWALL_sym_encrypt(keyId, IW_ALG_SM4, IW_SYM_MODE_CBC, iv, sizeof(iv),
                            data, dataLen, enc, &encLen);
    printf("IWALL_sym_encrypt(SM4): ret=%x\n", ret);
    DUMP_PRINT(enc, encLen)
    ret = IWALL_sym_decrypt(keyId, IW_ALG_SM4, IW_SYM_MODE_CBC, iv, sizeof(iv),
                            enc, encLen, dec, &decLen);
    printf("IWALL_sym_decrypt(SM4): ret=%x\n", ret);
    DUMP_PRINT(dec, decLen)

    if (data)
    {
        free(data);
    }
    if (enc)
    {
        free(enc);
    }
    if (dec)
    {
        free(dec);
    }
    return 0;
}

int asym_test()
{
    unsigned char publicKey[1024] = {0};
    unsigned int pubkeyLen = 1024;
    unsigned char data[256] = {0};
    unsigned int dataLen = 256 - 11;//RSA2048原文最大长度256-11，RSA1024原文最大长度128-11
    unsigned char signature[1024] = {0};
    unsigned int signatureLen = 1024;
    unsigned char encrypt[1024] = {0};
    unsigned int encLen = 1024;
    unsigned char decrypt[1024] = {0};
    unsigned decLen = 1024;

    int ret = 0;
    //SE固有公钥KID(0x00~0xEF)
    //SE固有私钥KID(0x00~0xEF)
    int keyId = 0x00;

    memset(data, 0x31, dataLen);

    keyId = 0x80;
    memset(publicKey, 0, pubkeyLen = sizeof(publicKey));
    memset(signature, 0, signatureLen = sizeof(signature));
    memset(encrypt, 0, encLen = sizeof(encrypt));
    memset(decrypt, 0, decLen = sizeof(decrypt));

    ret = IWALL_generate_keypair(keyId, IW_ALG_RSA2048_CRT, publicKey, &pubkeyLen);
    printf("\nIWALL_generate_keypair(RSA2048): ret=%x\n", ret);
    DUMP_PRINT(publicKey, pubkeyLen)

    ret = IWALL_asym_sign(keyId, IW_ALG_RSA2048_CRT, IW_ALG_SHA256,
                          data, dataLen, signature, &signatureLen);
    printf("IWALL_asym_sign(RSA2048): ret=%x\n", ret);
    DUMP_PRINT(signature, signatureLen)
    ret = IWALL_asym_verify(keyId, IW_ALG_RSA2048_CRT, IW_ALG_SHA256,
                            data, dataLen, signature, signatureLen);
    printf("IWALL_asym_verify(RSA2048): ret=%x\n", ret);
    ret = IWALL_asym_encrypt(keyId, IW_ALG_RSA2048_CRT, data, dataLen, encrypt, &encLen);
    printf("IWALL_asym_encrypt(RSA2048): ret=%x\n", ret);
    DUMP_PRINT(encrypt, encLen)
    ret = IWALL_asym_decrypt(keyId, IW_ALG_RSA2048_CRT, encrypt, encLen, decrypt, &decLen);
    printf("IWALL_asym_decrypt(RSA2048): ret=%x\n", ret);
    DUMP_PRINT(decrypt, decLen)


    keyId = 0x40;
    memset(publicKey, 0, pubkeyLen = sizeof(publicKey));
    memset(signature, 0, signatureLen = sizeof(signature));
    memset(encrypt, 0, encLen = sizeof(encrypt));
    memset(decrypt, 0, decLen = sizeof(decrypt));

    ret = IWALL_generate_keypair(keyId, IW_ALG_ECC256, publicKey, &pubkeyLen);
    printf("\nIWALL_generate_keypair(ECC256): ret=%x\n", ret);
    DUMP_PRINT(publicKey, pubkeyLen)

    ret = IWALL_asym_sign(keyId, IW_ALG_ECC256, IW_ALG_SHA256,
                          data, dataLen, signature, &signatureLen);
    printf("IWALL_asym_sign(ECC256): ret=%x\n", ret);
    DUMP_PRINT(signature, signatureLen)
    ret = IWALL_asym_verify(keyId, IW_ALG_ECC256, IW_ALG_SHA256,
                            data, dataLen, signature, signatureLen);
    printf("IWALL_asym_verify(ECC256): ret=%x\n", ret);
    ret = IWALL_asym_encrypt(keyId, IW_ALG_ECC256, data, dataLen, encrypt, &encLen);
    printf("IWALL_asym_encrypt(ECC256): ret=%x\n", ret);
    DUMP_PRINT(encrypt, encLen)
    ret = IWALL_asym_decrypt(keyId, IW_ALG_ECC256, encrypt, encLen, decrypt, &decLen);
    printf("IWALL_asym_decrypt(ECC256): ret=%x\n", ret);
    DUMP_PRINT(decrypt, decLen)

    keyId = 0x50;
    memset(publicKey, 0, pubkeyLen = sizeof(publicKey));
    memset(signature, 0, signatureLen = sizeof(signature));
    memset(encrypt, 0, encLen = sizeof(encrypt));
    memset(decrypt, 0, decLen = sizeof(decrypt));

    ret = IWALL_generate_keypair(keyId, IW_ALG_SM2, publicKey, &pubkeyLen);
    printf("\nIWALL_generate_keypair(SM2): ret=%x\n", ret);
    DUMP_PRINT(publicKey, pubkeyLen)

    ret = IWALL_asym_sign(keyId, IW_ALG_SM2, IW_ALG_SM3,
                          data, dataLen, signature, &signatureLen);
    printf("IWALL_asym_sign(SM2): ret=%x\n", ret);
    DUMP_PRINT(signature, signatureLen)
    ret = IWALL_asym_verify(keyId, IW_ALG_SM2, IW_ALG_SM3,
                            data, dataLen, signature, signatureLen);
    printf("IWALL_asym_verify(SM2): ret=%x\n", ret);
    ret = IWALL_asym_encrypt(keyId, IW_ALG_SM2, data, dataLen, encrypt, &encLen);
    printf("IWALL_asym_encrypt(SM2): ret=%x\n", ret);
    DUMP_PRINT(encrypt, encLen)
    ret = IWALL_asym_decrypt(keyId, IW_ALG_SM2, encrypt, encLen, decrypt, &decLen);
    printf("IWALL_asym_decrypt(SM2): ret=%x\n", ret);
    DUMP_PRINT(decrypt, decLen)
}

int main()
{
    int ret = 0;
    unsigned char seId[251] = {0};
    int seIdLen = 251;

    //连接安全芯片
    ret = IWALL_device_init(12000000, (unsigned char *) "/dev/spidev0.0", 5);
    printf("[FUNC:%s, LINE=%d], ret=%x\n", "IWALL_device_init", __LINE__, ret);

    //获取SE ID
    ret = IWALL_get_se_id(seId, &seIdLen);
    printf("[FUNC:%s, LINE=%d], ret=%x\n", "IWALL_get_se_id", __LINE__, ret);
    DUMP_PRINT(seId, seIdLen)

    //测试摘要算法
    digest_test();

    //测试对称算法
    sym_test();

    //测试公钥算法
    asym_test();

    //连接安全芯片
    ret = IWALL_se_close();
    printf("[FUNC:%s, LINE=%d], ret=%x\n", "IWALL_se_close", __LINE__, ret);
}