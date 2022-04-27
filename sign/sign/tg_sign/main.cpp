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
#define PATH_PRI "/home/tc/work/DDS/work/sign/tg_sign/prikey.pem"
#define PATH_PUB "/home/tc/work/DDS/work/sign/tg_sign/pubkey.pem"
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
        //        assert(false);
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
    int ret = RSA_verify(NID_sha1, (const unsigned char*)strData.c_str(),
                         strlen(strData.c_str()),  (const unsigned char*)sign_data.c_str(), 256,  pRSAPublicKey);
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
// 通过公钥文件加密
std::vector<char> EncryptByPubkeyFile(const std::string& message,
                                      const std::string& pub_filename) {
    BIO* in = BIO_new(BIO_s_file());
    if (in == NULL) {
        std::cout << "BIO_new failed" << std::endl;
        return std::vector<char>();
    }
    BIO_read_filename(in, pub_filename.c_str());

    RSA* rsa = PEM_read_bio_RSA_PUBKEY(in, NULL, NULL, NULL);
    BIO_free(in);
    if (rsa == NULL) {
        std::cout << "PEM_read_bio_RSA_PUBKEY failed" << std::endl;
        return std::vector<char>();
    }
    int size = RSA_size(rsa);
    std::vector<char> encrypt_data;
    encrypt_data.resize(size);
    int ret = RSA_public_encrypt(
                message.length(), (unsigned char*)message.c_str(),
                (unsigned char*)encrypt_data.data(), rsa, RSA_PKCS1_PADDING);
    RSA_free(rsa);
    if (ret == -1) {
        std::cout << "RSA_public_encrypt failed" << std::endl;
        return std::vector<char>();
    }
    return encrypt_data;
}
// 通过私钥文件解密
std::string DecryptByPrikeyFile(char* cipher, uint32_t len,
                                const std::string& pri_file) {
    BIO* in = BIO_new(BIO_s_file());
    if (in == NULL) {
        std::cout << "BIO_new failed" << std::endl;
        return "";
    }
    BIO_read_filename(in, pri_file.c_str());

    RSA* rsa = PEM_read_bio_RSAPrivateKey(in, NULL, NULL, NULL);
    BIO_free(in);
    if (rsa == NULL) {
        std::cout << "PEM_read_bio_RSAPrivateKey failed" << std::endl;
        return "";
    }

    int size = RSA_size(rsa);
    std::vector<char> data;
    data.resize(size);
    int ret = RSA_private_decrypt(len, (unsigned char*)cipher,
                                  (unsigned char*)data.data(), rsa,
                                  RSA_PKCS1_PADDING);
    RSA_free(rsa);
    if (ret == -1) {
        std::cout << "RSA_private_decrypt failed" << std::endl;
        return "";
    }
    std::string decrypt_data(data.begin(), data.end());
    return decrypt_data;
}
// 通过公钥字符串进行加密
std::vector<char> EncryptByPubkeyString(const std::string& message,
                                        const std::string& pubkey) {
    BIO* in = BIO_new_mem_buf((void*)pubkey.c_str(), -1);
    if (in == NULL) {
        std::cout << "BIO_new_mem_buf failed" << std::endl;
        return std::vector<char>();
    }

    RSA* rsa = PEM_read_bio_RSA_PUBKEY(in, NULL, NULL, NULL);
    BIO_free(in);
    if (rsa == NULL) {
        std::cout << "PEM_read_bio_RSA_PUBKEY failed" << std::endl;
        return std::vector<char>();
    }

    int size = RSA_size(rsa);
    std::vector<char> encrypt_data;
    encrypt_data.resize(size);
    int ret = RSA_public_encrypt(
                message.length(), (unsigned char*)message.c_str(),
                (unsigned char*)encrypt_data.data(), rsa, RSA_PKCS1_PADDING);
    RSA_free(rsa);
    if (ret == -1) {
        std::cout << "RSA_public_encrypt failed" << std::endl;
        return std::vector<char>();
    }

    return encrypt_data;
}

// 通过私钥字符串进行解密
std::string DecryptByPrikeyString(char* cipher, uint32_t len,
                                  const std::string prikey) {
    BIO* in = BIO_new_mem_buf((void*)prikey.c_str(), -1);
    if (in == NULL) {
        std::cout << "BIO_new_mem_buf failed" << std::endl;
        return "";
    }

    RSA* rsa = PEM_read_bio_RSAPrivateKey(in, NULL, NULL, NULL);
    BIO_free(in);
    if (rsa == NULL) {
        std::cout << "PEM_read_bio_RSAPrivateKey failed" << std::endl;
        return "";
    }

    int size = RSA_size(rsa);
    std::vector<char> data;
    data.resize(size);
    int ret = RSA_private_decrypt(len, (unsigned char*)cipher,
                                  (unsigned char*)data.data(), rsa,
                                  RSA_PKCS1_PADDING);
    RSA_free(rsa);
    if (ret == -1) {
        std::cout << "RSA_private_decrypt failed" << std::endl;
        return "";
    }
    std::string decrypt_data(data.begin(), data.end());
    return decrypt_data;
}
// 通过私钥文件生成签名
std::vector<char> GenerateRsaSignByFile(const std::string& message,
                                        const std::string& pri_filename) {
    OpenSSL_add_all_algorithms();
    BIO* in = BIO_new(BIO_s_file());
    if (in == NULL) {
        std::cout << "BIO_new failed" << std::endl;
        return std::vector<char>();
    }
    BIO_read_filename(in, pri_filename.c_str());
    RSA* rsa = PEM_read_bio_RSAPrivateKey(in, NULL, NULL, NULL);
    BIO_free(in);

    if (rsa == NULL) {
        std::cout << "PEM_read_bio_RSAPrivateKey failed" << std::endl;
        return std::vector<char>();
    }
    unsigned int size = RSA_size(rsa);
    std::vector<char> sign;
    sign.resize(size);

    int ret =
            RSA_sign(NID_md5, (const unsigned char*)message.c_str(),
                     message.length(), (unsigned char*)sign.data(), &size, rsa);
    RSA_free(rsa);
    if (ret != 1) {
        std::cout << "RSA_sign failed" << std::endl;
        return std::vector<char>();
    }
    return sign;
}

// 通过公钥文件验证签名
bool VerifyRsaSignByFile(char* sign, uint32_t sign_len,
                         const std::string& pub_filename,
                         const std::string& verify_str) {
    OpenSSL_add_all_algorithms();
    BIO* in = BIO_new(BIO_s_file());
    if (in == NULL) {
        std::cout << "BIO_new failed" << std::endl;
        return false;
    }

    BIO_read_filename(in, pub_filename.c_str());

    RSA* rsa = PEM_read_bio_RSA_PUBKEY(in, NULL, NULL, NULL);

    if (rsa == NULL) {
        std::cout << "PEM_read_bio_RSA_PUBKEY failed" << std::endl;
        return false;
    }
    BIO_free(in);

    int ret =
            RSA_verify(NID_md5, (const unsigned char*)verify_str.c_str(),
                       verify_str.length(), (unsigned char*)sign, sign_len, rsa);
    RSA_free(rsa);
    if (ret != 1) {
        std::cout << "RSA_verify failed" << std::endl;
        return false;
    }
    return true;
}
// 通过私钥字符串生成签名
std::vector<char> GenerateRsaSignByString(const std::string& message,
                                          const std::string& prikey) {
    OpenSSL_add_all_algorithms();
    BIO* in = BIO_new_mem_buf((void*)prikey.c_str(), -1);
    if (in == NULL) {
        std::cout << "BIO_new_mem_buf failed" << std::endl;
        return std::vector<char>();
    }

    RSA* rsa = PEM_read_bio_RSAPrivateKey(in, NULL, NULL, NULL);
    BIO_free(in);

    if (rsa == NULL) {
        std::cout << "PEM_read_bio_RSAPrivateKey failed" << std::endl;
        return std::vector<char>();
    }
    unsigned int size = RSA_size(rsa);
    std::vector<char> sign;
    sign.resize(size);

    int ret =
            RSA_sign(NID_md5, (const unsigned char*)message.c_str(),
                     message.length(), (unsigned char*)sign.data(), &size, rsa);
    RSA_free(rsa);
    if (ret != 1) {
        std::cout << "RSA_sign failed" << std::endl;
        return std::vector<char>();
    }
    return sign;
}

// 通过公钥字符串验证签名
bool VerifyRsaSignByString(char* sign, uint32_t sign_len,
                           const std::string& pubkey,
                           const std::string& verify_str) {
    BIO* in = BIO_new_mem_buf((void*)pubkey.c_str(), -1);
    if (in == NULL) {
        std::cout << "BIO_new_mem_buf failed" << std::endl;
        return false;
    }

    RSA* rsa = PEM_read_bio_RSA_PUBKEY(in, NULL, NULL, NULL);
    BIO_free(in);

    if (rsa == NULL) {
        std::cout << "PEM_read_bio_RSA_PUBKEY failed" << std::endl;
        return false;
    }

    int ret =
            RSA_verify(NID_md5, (const unsigned char*)verify_str.c_str(),
                       verify_str.length(), (unsigned char*)sign, sign_len, rsa);
    RSA_free(rsa);
    if (ret != 1) {
        std::cout << "RSA_verify failed" << std::endl;
        return false;
    }
    return true;
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
    SHA1_Init(&ctx);//是一个初始化参数，它用来初始化一个 SHA_CTX 结构，该结构存放弄了生成 SHA1 散列值的一些参数，在应用中可以不用关系该结构的内容。

    //函数正是可以处理大文件的关键。它可以反复调用，比如说我们要计算一个 5G 文件的散列值，我们可以将该文件分割成多个小的数据块，
    //对每个数据块分别调用一次该函数，这样在最后就能够应用 SHA1_Final() 函数正确计算出这个大文件的 sha1 散列值。
    SHA1_Update(&ctx, m.c_str(), strlen(m.c_str()));

    SHA1_Final(digest, &ctx);//SHA1_Final() 函数正确计算出这个大文件的 sha1 散列值。


    char mdString[SHA_DIGEST_LENGTH*2+1];
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    cout << "digest:" << digest << endl;
    std::cout << "SHA1 digest translate:" << mdString << endl;


    string digest_s((char*)digest);
    string mdString1(mdString);

//        auto sign_ = SignRSAKeyFile(PATH_PRI,mdString1);
    auto sign_ =GenerateRsaSignByFile("xxxx",PATH_PRI);
    std::cout <<"私钥签名" "data:" <<mdString1<<"-----"<< digest_s.c_str() << std::endl;
    std::string str;
    char signs[sign_.size()];
    for(int i=0;i<sign_.size();i++){
        signs[i] = sign_.at(i);
    }
    std::cout<<signs<<"KKKKKKKKKKKKKKKKKKKK";
//    char *sign;
    VerifyRsaSignByFile(signs,sign_.size(),PATH_PUB,str);
//    std::cout <<"公钥验证" "verify:" <<VerifyRSAKeyFile(PATH_PUB,(const string)mdString,sign_)<<std::endl;


    return a.exec();
}
