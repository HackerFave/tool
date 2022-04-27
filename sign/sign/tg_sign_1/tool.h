#ifndef TOOL_H
#define TOOL_H

#include<openssl/rsa.h>
#include <openssl/bio.h>
#include<openssl/pem.h>
#include<openssl/err.h>
#include<iostream>
using namespace std;
/*
 *生成1024位rsa私钥，保存为pem格式：
 *  openssl genpkey -out prikey.pem -algorithm rsa


生成对应的公钥：
openssl pkey -in prikey.pem -pubout -out pubkey.pem

*/
static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}



static RSA* GetKeyRSA(std::string strKey,const int type)//type = 1 公钥 type = 2 私钥
{
    int nPublicKeyLen = strKey.size(); //strKey为base64编码的公钥字符串
    for(int i = 64; i < nPublicKeyLen; i+=64)
    {
        if(strKey[i] != '\n')
        {
            strKey.insert(i, "\n");
        }
        i++;
    }
    if(type == 1){
        strKey.insert(0, "-----BEGIN PUBLIC KEY-----\n");
        strKey.append("\n-----END PUBLIC KEY-----\n");
    }else{
        strKey.insert(0, "-----BEGIN PRIVATE KEY-----\n");
        strKey.append("\n-----END PRIVATE KEY-----\n");
    }

    BIO *bio = NULL;
    RSA *rsa = NULL;
    char *chPublicKey = const_cast<char *>(strKey.c_str());
    printf("chPublicKey===%s\n",chPublicKey);
    if ((bio = BIO_new_mem_buf(chPublicKey, -1)) == NULL) //从字符串读取RSA公钥
    {
        return NULL;
    }
    if(type == 1)
        rsa = PEM_read_bio_RSA_PUBKEY(bio, NULL, NULL, NULL); //从bio结构中得到rsa结构
    // rsa = PEM_read_bio_RSAPublicKey(bio,&rsa,NULL,NULL);
    else
        rsa = PEM_read_bio_RSAPrivateKey(bio,NULL,NULL,NULL);
    if (NULL == rsa)
    {
        BIO_free_all(bio);
        unsigned long ulErr = ERR_get_error(); // 获取错误号
        char szErrMsg[1024] = {0};
        char *pTmp = NULL;
        pTmp = ERR_error_string(ulErr,szErrMsg); // 格式：error:errId:库:函数:原因
        cout << szErrMsg;
        cout << "load public key fail error=" <<ulErr << " msg=" << szErrMsg;
    }
    else
    {
        cout<< "load public key ok "<<endl;
    }
    return rsa;
}


static std::string base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }
    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}


static char* base64Encode(char const* origSigned, unsigned origLength)
{
    unsigned int numOrig24BitValues = 0;
    unsigned int numResultBytes = 0;
    int havePadding = 0;
    int havePadding2 = 0;
    char* result = NULL;
    unsigned int i = 0;


    unsigned char const* orig = (unsigned char const*)origSigned; // in case any input bytes have the MSB set
    if (orig == NULL) return NULL;

    numOrig24BitValues = origLength / 3;
    havePadding = origLength > numOrig24BitValues * 3;
    havePadding2 = origLength == numOrig24BitValues * 3 + 2;
    numResultBytes = 4 * (numOrig24BitValues + havePadding);
    result = (char*)malloc(numResultBytes + 1); // allow for trailing '/0'

    // Map each full group of 3 input bytes into 4 output base-64 characters:

    for (i = 0; i < numOrig24BitValues; ++i)
    {
        result[4 * i + 0] = base64_chars[(orig[3 * i] >> 2) & 0x3F];
        result[4 * i + 1] = base64_chars[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
        result[4 * i + 2] = base64_chars[((orig[3 * i + 1] << 2) | (orig[3 * i + 2] >> 6)) & 0x3F];
        result[4 * i + 3] = base64_chars[orig[3 * i + 2] & 0x3F];
    }

    // Now, take padding into account. (Note: i == numOrig24BitValues)
    if (havePadding)
    {
        result[4 * i + 0] = base64_chars[(orig[3 * i] >> 2) & 0x3F];
        if (havePadding2)
        {
            result[4 * i + 1] = base64_chars[(((orig[3 * i] & 0x3) << 4) | (orig[3 * i + 1] >> 4)) & 0x3F];
            result[4 * i + 2] = base64_chars[(orig[3 * i + 1] << 2) & 0x3F];
        }
        else
        {
            result[4 * i + 1] = base64_chars[((orig[3 * i] & 0x3) << 4) & 0x3F];
            result[4 * i + 2] = '=';
        }
        result[4 * i + 3] = '=';
    }

    result[numResultBytes] = (char)('/0');
    return result;

}

#endif // TOOL_H
