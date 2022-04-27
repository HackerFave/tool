#include <QCoreApplication>
#include <QCoreApplication>
#include <iostream>
//#include "Socket.hpp"
#include <sys/socket.h>
#include <functional>
#include <bits/stdc++.h>
#include <openssl/sha.h>

#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>

#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <curl/curl.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
using namespace std;

//加密
std::string EncodeRSAKeyFile( const std::string& strPemFileName, const std::string& strData )
{
    if (strPemFileName.empty() || strData.empty())
    {
        assert(false);
        return "";
    }
    FILE* hPubKeyFile = fopen(strPemFileName.c_str(), "rb");
    if( hPubKeyFile == NULL )
    {
        assert(false);
        return "";
    }
    std::string strRet;
    RSA* pRSAPublicKey = RSA_new();
    if(PEM_read_RSA_PUBKEY(hPubKeyFile, &pRSAPublicKey, 0, 0) == NULL)
    {
        assert(false);
        return "";
    }

    int nLen = RSA_size(pRSAPublicKey);
    char* pEncode = new char[nLen + 1];
    int ret = RSA_public_encrypt(strData.length(), (const unsigned char*)strData.c_str(), (unsigned char*)pEncode, pRSAPublicKey, RSA_PKCS1_PADDING);
    if (ret >= 0)
    {
        strRet = std::string(pEncode, ret);
    }
    delete[] pEncode;
    RSA_free(pRSAPublicKey);
    fclose(hPubKeyFile);
    CRYPTO_cleanup_all_ex_data();
    return strRet;
}

//签名 use private key
std::string SignRSAKeyFile( const std::string& strPemFileName, std::string& strData )
{
        if (strPemFileName.empty() || strData.empty())
        {
                assert(false);
                return "";
        }
        FILE* hPriKeyFile = fopen(strPemFileName.c_str(), "rb");
        if( hPriKeyFile == NULL )
        {
                assert(false);
                return "";
        }
        std::string strRet;
        RSA* pRSAPriKey = RSA_new();
        if(PEM_read_RSAPrivateKey(hPriKeyFile, &pRSAPriKey, 0, 0) == NULL)
        {
                assert(false);
                return "";
        }

        int nLen = RSA_size(pRSAPriKey);
        char* pEncode = new char[nLen + 1];
    unsigned int outlen;
        int ret = RSA_sign(NID_sha1, (const unsigned char*)strData.c_str(), strData.length() , (unsigned char*)pEncode, &outlen, pRSAPriKey);
        if (ret >= 0)
        {
                strRet = std::string(pEncode);
        std::cout << "\n" << strRet << endl;
        //std::cout << "next \n" << pEncode << endl;
        std::cout << "critical length:\n" << outlen << endl;
        }
    if( ret != 1)
        std::cout << "sign failed\n";
        delete[] pEncode;
        RSA_free(pRSAPriKey);
        fclose(hPriKeyFile);
        CRYPTO_cleanup_all_ex_data();
        return strRet;
}

//解密
std::string DecodeRSAKeyFile( const std::string& strPemFileName, const std::string& strData )
{
    if (strPemFileName.empty() || strData.empty())
    {
        assert(false);
        return "";
    }
    FILE* hPriKeyFile = fopen(strPemFileName.c_str(),"rb");
    if( hPriKeyFile == NULL )
    {
        assert(false);
        return "";
    }
    std::string strRet;
    RSA* pRSAPriKey = RSA_new();
    if(PEM_read_RSAPrivateKey(hPriKeyFile, &pRSAPriKey, 0, 0) == NULL)
    {
        assert(false);
        return "";
    }
    int nLen = RSA_size(pRSAPriKey);
    char* pDecode = new char[nLen+1];

    int ret = RSA_private_decrypt(strData.length(), (const unsigned char*)strData.c_str(), (unsigned char*)pDecode, pRSAPriKey, RSA_PKCS1_PADDING);
    if(ret >= 0)
    {
        strRet = std::string((char*)pDecode, ret);
    }
    delete [] pDecode;
    RSA_free(pRSAPriKey);
    fclose(hPriKeyFile);
    CRYPTO_cleanup_all_ex_data();
    return strRet;
}

//验证签名 use pubkey
int VerifyRSAKeyFile( const std::string& strPemFileName, const std::string& strData , const std::string& sign_data)
{
        if (strPemFileName.empty() || strData.empty())
        {
                assert(false);
                return 0;
        }
        FILE* hPubKeyFile = fopen(strPemFileName.c_str(), "rb");
        if( hPubKeyFile == NULL )
        {
                assert(false);
                return 0;
        }
        std::string strRet;
        RSA* pRSAPublicKey = RSA_new();
        if(PEM_read_RSA_PUBKEY(hPubKeyFile, &pRSAPublicKey, 0, 0) == NULL)
        {
                assert(false);
                return 0;
        }

        int nLen = RSA_size(pRSAPublicKey);
        char* pEncode = new char[nLen + 1];
    unsigned int outlen;
        int ret = RSA_verify(NID_sha1, (const unsigned char*)strData.c_str(), strlen(strData.c_str()),  (const unsigned char*)sign_data.c_str(), 128,  pRSAPublicKey);
    if(ret != 1){
        std::cout << "verify error\n";
        unsigned long ulErr = ERR_get_error();
        char szErrMsg[1024] = {0};
        cout << "error number:" << ulErr << endl;
        char *pTmp = NULL;
        pTmp = ERR_error_string(ulErr,szErrMsg); // 格式：error:errId:库:函数:原因
        cout << szErrMsg << endl;
        return -1;
    }
    else
        std::cout << "verify success\n";
        delete[] pEncode;
        RSA_free(pRSAPublicKey);
        fclose(hPubKeyFile);
        CRYPTO_cleanup_all_ex_data();
        return 1;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    try
//    {
        //Socket::UDP sock;

        //sock.bind(3000);

    string m = "test";
    //hash SHA1
    unsigned char digest[SHA_DIGEST_LENGTH];

    SHA_CTX ctx;
    SHA1_Init(&ctx);
    SHA1_Update(&ctx, m.c_str(), strlen(m.c_str()));
    SHA1_Final(digest, &ctx);

    char mdString[SHA_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
    sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
    cout << "digest:" << digest << endl;
    std::cout << "SHA1 digest translate:" << mdString << endl;


    string digest_s((char*)digest);
    string mdString1(mdString);
    auto sign_ = SignRSAKeyFile("prikey.pem",mdString1);
    std::cout << "data:" << sign_ << std::endl;
    std::cout << "verify:" << VerifyRSAKeyFile("pubkey.pem",(const string)mdString,sign_);


    return a.exec();
}
