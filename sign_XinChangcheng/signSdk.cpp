#include "signSdk.h"

#include <stdio.h>
#include <string.h>
#include "scm.h"
#include <iostream>
#include <fstream>
#include <string>
#include "httpMethord.h"
//#include <json-c/json.h>
//#include <jsoncpp/json/json.h>
#include <boost/property_tree/json_parser.hpp>
#include <stdlib.h>
#include "client_ssl.h"
using namespace std;
using namespace boost;
using namespace boost::property_tree;

void iprintf(unsigned char *p, size_t len)
{
    int i;
    for (i = 0; i < len; i++) {
        printf("%02x", p[i]);
    }
    printf("\n");
}
int write_string_to_file_append(const std::string & file_string, const std::string str )
{
    std::ofstream	OsWrite(file_string,std::ofstream::app);
    OsWrite<<str;
    OsWrite<<std::endl;
    OsWrite.close();
    return 0;
}
signSdk::signSdk()
{
    readKeyFile("./key.txt");
    _alg  = SCM_RSA;
    init_sdk();
    std::string ca= "./scm/certs/ca_chain.pem";
    std::string client_cert= "./scm/config/client_cert.pem";
    std::string privateKey= "./scm/config/private.pem";
    _client = new client_ssl(ca,client_cert,privateKey,_ip,_port);
    _client->init();
}
bool signSdk::isExistCert()
{
    int ret;
    
    unsigned char certificate[2048] = {0};
    unsigned int  cert_len = 2048;
    // 导出证书 (0: 签名证书，1: 加密证书)
    ret = IW_ExportCertificate(_userId.c_str(), 0, certificate, &cert_len);
    //    printf("IW_ExportCertificate: %d. Certificate: %s\n", ret, certificate);
    
    if(ret==0){
        std::cout<<"[已存在签名证书]"<<std::endl;
        return true;
    }
    std::cout<<"[正在申请签名证书]"<<std::endl;
    return false;
}
void signSdk::init_sdk() {
    
    int ret;
    printf("SDK Version: %s\n", IW_SDKVersion());
    char *path = "./scm/";
    //初始化SDK，用于指定SDK产生数据的存储位置
    ret = IW_InitSDK(path);
    printf("IW_InitSDK: %d\n", ret);
    //初始化TLS的证书链，用于校验后续的TLS证书的合法性
    IW_InitSSLCertChain(_certs.c_str());
    //初始化用户证书的证书链，用于校验后续的用户证书的合法新
    IW_InitCACertChain(_certs.c_str());
    std::string config = "./signConfig.ini";
    if(LoadConfig(config)>0){
        get_cert();
    }
    
}
//申请证书
void signSdk::get_cert()
{
    if(isExistCert()){//判断签名证书是否存在
        return;
    }
    int ret;
    int bits = 2048;
    int sbits  = 0, status = 0;
    //获取密钥状态，是否存在密钥
    ret = IW_GetKeyStatus(_userId.c_str(), &status);
    //    printf("IW_GetKeyStatus: %d  status:%d\n", ret, status);
    if(status == 0){// 判断是否存在密钥 0-- 不存在 1-- 存在
        std::cout<<"[密钥对不存在]"<<std::endl;
        //0-- 成功
        ret = IW_GenerateKey(_userId.c_str(), _pin.c_str(), _alg, bits, NULL, 0);
        if(ret == 0){
            std::cout<<"[密钥对创建成功]"<<ret<<std::endl;
        }else {
            std::cout<<"[密钥对创建失败]"<<ret<<std::endl;
            return;
        }
    }
    
    //构建CSR请求信息，commonName 可以包含tbox的序列号/VIN等用于业务平台校验
    csr_info_t csrInfo = {0};
    strcpy(csrInfo.subjectInfo.country,         "Beijing");
    strcpy(csrInfo.subjectInfo.stateOrProvince, "Beijing");
    strcpy(csrInfo.subjectInfo.locality,        "Haidian");
    strcpy(csrInfo.subjectInfo.organization,    "Org");
    strcpy(csrInfo.subjectInfo.organizationUnit,"Unit");
    strcpy(csrInfo.subjectInfo.commonName,      "192.168.2.25");
    strcpy(csrInfo.attributeInfo.challengePassword, "123123123");
    memset(csrInfo.attributeInfo.alertName, 0x31, 32);
    strcpy(csrInfo.attributeInfo.keyUsage, "nonRepudiation,digitalSignature");
    
    char csr[2048] = {0};
    unsigned int csr_len = 2048;
    //生成CSR请求
    ret = IW_GenerateCSR(_userId.c_str(), _pin.c_str(), csrInfo, csr, &csr_len);
    //    printf("IW_GenerateCSR: %x, CSR info: %s\n", ret, csr);
    if(ret == 0){
        std::cout<<"[生成CSR成功]"<<ret<<std::endl;
        std::string host = "http://192.168.30.137:8660/api/v1/certificate?";
        std::string user = "userId=123456&csr1=";
        std::string strURL = host+user+csr;
        string strResponse;
        CURLcode nRes = httpMethord::HttpGet(strURL, strResponse,300);
        /***********************
        *
        *
        * json 解析
        *
        * **********************************/
        std::string cert ;
        
        if(ParseJson(strResponse,cert)){
            //获取密钥长度
            ret = IW_GetKeyBits(_userId.c_str(), &sbits);
            printf("IW_GetKeyBits: %d  sbits: %d\n", ret, sbits);
            std::string certificates_str = /*DevJson["signCert"].asCString()*/cert;
            
            write_string_to_file_append("./scm/config/client_cert.pem",certificates_str);
            
            importexport_cert((unsigned char *)certificates_str.c_str(),certificates_str.length()) ;
        }
    }else {
        std::cout<<"[生成CSR失败]"<<ret<<std::endl;
        return;
    }
    
}
//导入导出证书
void signSdk::importexport_cert( unsigned char * client_cert,int length)
{
    
    int ret;
    // 导入证书 (0: 签名证书，1: 加密证书)
    ret = IW_ImportCertificate(_userId.c_str(), 0,client_cert, length);
    
    printf("IW_ImportCertificate: %d.\n", ret);
    unsigned char certificate[2048] = {0};
    unsigned int  cert_len = 2048;
    // 导出证书 (0: 签名证书，1: 加密证书)
    ret = IW_ExportCertificate(_userId.c_str(), 0, certificate, &cert_len);
    printf("IW_ExportCertificate: %d. Certificate:", ret);
    
    unsigned char cert_publicKey[2048] = {0};
    unsigned int  cert_pub_len = 2048;
    //获取公钥
    ret = IW_GetCertificatePublicKey(certificate, cert_len, cert_publicKey, &cert_pub_len);
    printf("IW_GetCertificatePublicKey: %d\n", ret);
    
    write_string_to_file_append("./scm/config/cert_publicKey.pem",(const char*)cert_publicKey);
    
    unsigned char privateKey[2048] = {0};
    unsigned int  pri_len = 2048;
    IW_ExportPrivateKey(_userId.c_str(),_pin.c_str(),privateKey,&pri_len);
    write_string_to_file_append("./scm/config/private.pem",(const char*)privateKey);
    
    unsigned char publicKey[2048] = {0};
    unsigned int  pub_len = 2048;
    IW_ExportPrivateKey(_userId.c_str(),_pin.c_str(),publicKey,&pub_len);
    write_string_to_file_append("./scm/config/public.pem",(const char*)publicKey);
    
    //    //    // 验证证书状态
    //    ret = IW_VerifyCertStatus((const char*)/*client_cert*/certificate);
    //    printf("IW_VerifyCertStatus: %d\n", ret);
    //        ret = IW_VerifyCertStatus((const char*)client_revoke_cert);
    //        printf("IW_VerifyCertStatus revoke: %d\n", ret);
    
    
    
}
#if 0
void signSdk::pwd_test() {
    int ret;
    char *userID = "admin";
    char *pin    = "12345678";
    unsigned char random[32] = {0};
    unsigned int  random_len = 32;
    // 获取随机数
    ret = IW_GenRandom(random, random_len);
    printf("IW_GenRandom: %d ", ret);
    iprintf(random, random_len);
    
    unsigned char hash[32] = {0};
    unsigned int  hash_len = 32;
    unsigned char data[3] = { 0x31, 0x32, 0x33 };
    // 哈希算法
    ret = IW_MessageDigest(SCM_SM3, data, sizeof(data), hash, &hash_len);
    printf("IW_MessageDigest: %d ", ret);
    iprintf(hash, hash_len);
    char cert[] = "-----BEGIN CERTIFICATE-----\n"
                  "MIICjzCCAjSgAwIBAgIER4t9dDAKBggqgRzPVQGDdTAjMQswCQYDVQ"
                  "QGEwJDTjEUMBIGA1UEAwwLU00yIFNJR04gQ0EwHhcNMjIwMzA4MjM0OTMzWhcNMjMwMzA4MjM0OTMzWjBWMRAwDgYDVQQIDAdCZWlqaW5nMRAwDgYDVQQHDAdIYWlkaWFuMQwwCgYDVQQKDANPcmcxDTALBgNVBAsMBFVuaXQxEzARBgNVBAMMCkNvbW1vbk5hbWUwWTATBgcqhkjOPQIBBggqgRzPVQGCLQNCAATbBcJTreq+YBS/HiVawQ0XDLDfMKUt6V4UC/ev6tpcS7TMSltM40K/1j0k/DZbtTO61aYfaTyV3cietBN8typZo4IBITCCAR0wOQYDVR0RBDIwMKAuBggrBgEFBQcIBKAiBCAxMTExMTExMTExMTExMTExMTExMTExMTExMTExMTExMTAJBgNVHRMEAjAAMDwGCCsGAQUFBwEBBDAwLjAsBggrBgEFBQcwAYYgaHR0cDovLzE5Mi4xNjguMS4yMTQ6MTgwODUvb2NzcC8wSgYDVR0fBEMwQTA/oD2gO4Y5aHR0cDovLzE5Mi4xNjguMS4yMTQ6MTgwODUvb2NzcC9kb3dubG9hZC8xMjAwMDQ5OTc0LzEuY3JsMB8GA1UdIwQYMBaAFA4wHcCva7/S61/T6tbUX/IitaGWMB0GA1UdDgQWBBSEl5BW2U7gOzFjUyVGPe786cM4RDALBgNVHQ8EBAMCB4AwCgYIKoEcz1UBg3UDSQAwRgIhAJ6+C6TVhbtHK1zU4AJ/76iSXRm4bhuSHgIgwbIfeZUwAiEA2IVUGZE5Nw/QhQotkMsM9r2H+YOHWJA4Fiec0vn4UFY=\n"
                  "-----END CERTIFICATE-----";
    symm_ctx_st *ctx = IW_SymmCtxNew();
    unsigned char key[16] = { 0 };
    memset(key, 0x31, 16);
    unsigned char iv[16] = { 0 };
    memset(iv, 0x31, 16);
    unsigned char input[16] = { 0 };
    memset(input, 0x31, 16);
    unsigned char out[32] = { 0 };
    int out_len = 32;
    int all_len = 0;
    ret = IW_SymmInit(ctx, SCM_SM4, SCM_MODE_CBC, key, 16, iv, 1);
    printf("IW_SymmInit enc: %d \n", ret);
    ret = IW_SymmUpdate(ctx, input, 16, out, &out_len);
    printf("IW_SymmUpdate enc: %d \n", ret);
    all_len += out_len;
    iprintf(out, all_len);
    ret = IW_SymmFinial(ctx, out + out_len, &out_len);
    all_len+= out_len;
    printf("IW_SymmFinial enc: %d \n", ret);
    iprintf(out, all_len);
    IW_SymmCtxFree(ctx);
    memset(key,   0x31, 16);
    memset(iv,    0x31, 16);
    memset(input, 0x31, 16);
    // 对称算法
    ret = IW_SymmCompute(SCM_SM4, SCM_MODE_CBC, key, 16, iv, 1, input, 16, out, &out_len);
    printf("IW_SymmCompute enc: %d \n", ret);
    iprintf(out, out_len);
    
    char base64_out[10] = {0};
    unsigned int base_out_len = 10;
    ret = IW_Base64Encode(data, sizeof(data), base64_out, &base_out_len);
    printf("IW_Base64Encode: %d ", ret);
    printf("%s\n", base64_out);
    
    unsigned char base64_info[10] = {0};
    unsigned int info_len = 10;
    ret = IW_Base64Decode(base64_out, out_len, base64_info, &info_len);
    printf("IW_Base64Decode: %d ", ret);
    iprintf(base64_info, info_len);
    
    unsigned char cipher[1024] = {0};
    unsigned int cipher_len = 1024;
    // 非对称加解密
    ret = IW_ASymmEncrypt((const unsigned char *)cert, strlen(cert), data, sizeof(data), cipher, &cipher_len);
    printf("IW_ASymmEncrypt: %d\n", ret);
    iprintf(cipher, cipher_len);
    unsigned char plain[1024] = {0};
    unsigned int plain_len = 1024;
    ret = IW_ASymmDecrypt(userID, pin, cipher, cipher_len, plain, &plain_len);
    printf("IW_ASymmDecrypt: %d\n", ret);
    iprintf(plain, plain_len);
    // 签名验签
    unsigned char sign[256] = {0};
    unsigned int  sign_len  = 256;
    ret = IW_Sign(userID, pin, data, sizeof(data), sign, &sign_len, SCM_SM3);
    printf("IW_Sign: %d\n", ret);
    iprintf(sign, sign_len);
    
    unsigned int cert_len = strlen(cert);
    ret = IW_VerifySignByCertificate(data, sizeof(data), sign, sign_len, (const unsigned char *)cert, cert_len, SCM_SM3);
    printf("IW_VerifySignByCertificate: %d\n", ret);
}
#endif

size_t signSdk::readData(void *contents, size_t size, size_t nmemb, void *stream){
    string *str = (string*)stream;
    (*str).append((char*)contents, size*nmemb);
    return size * nmemb;
}
//解析Json
bool signSdk::ParseJson(const std::string &json,std::string &cert)
{
    stringstream stream(json);
    ptree strTree;
    try {
        read_json(stream, strTree);
    }
    catch (ptree_error & e) {
        return false;
    }
    
    try {
        cert = strTree.get<string>("signCert");
        //        ptree names = strTree.get_child("signCert");
        
        //        BOOST_FOREACH(ptree::value_type &name, names)
        //        {
        //            stringstream s;
        //            write_json(s, name.second);
        //            cert = s.str();
        //            std::cout<<cert<<"*****************************************"<<std::endl;
        //        }
    }
    catch (ptree_error & e)
    {
        return false;
    }
    return true;
}
/*----------------------------
*功能：加载配置文件参数
*-----------------------------
*输入：配置文件ini的路径
*输出：
*/
int signSdk::LoadConfig(string ConfigPath)
{
    boost::property_tree::ptree m_pt, tag_setting;
    try
    {
        read_ini(ConfigPath, m_pt);
        std::cout<<"------配置文件打开成功------------"<<std::endl;
    }
    catch (std::exception e)
    {
        std::cout<<"------配置文件打开错误，将使用默认设置------------"<<std::endl;
        return -1;
    }
    
    
    //Front Camera Config ini
    //Front_PointWidthRight1为宏定义参数，当配置文件加载失败时，将Front_PointWidthRight1赋值给Front_PointWidthRight
    
    tag_setting = m_pt.get_child("sign");
    _ip = tag_setting.get<string>("ip");
    _port = tag_setting.get<string>("port");
    _userId = tag_setting.get<string>("userid");
    _pin = tag_setting.get<string>("pin");
    if(_ip==""||_port==""||_userId==""||_pin==""){
        return -1;
    }
    
    return 1;
}
//读取证书链
bool signSdk::readKeyFile(std::string path)
{
    string stringLine;
    ifstream infile;
    string tempName;
    ofstream fout( "key.txt", ios::app);
    infile.open (path);
    while( !infile.eof() ) // To get you all the lines.
    {
        getline(infile,stringLine); // Saves the line in stringLine.
        _certs = _certs+"\n"+stringLine;
    }
    infile.close();
}
void signSdk::tls_test() {
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        /* what call to write: */
        curl_easy_setopt(curl, CURLOPT_URL, "https://192.168.10.99:8992");//SM2
        
        do { /* dummy loop, just to break out from */
            curl_easy_setopt_cert_and_prikey(curl,_userId.c_str() ,_pin.c_str());
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            /* Perform the request, res will get the return code */
            res = curl_easy_perform(curl);
            /* Check for errors */
            if (res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, readData);	//对返回的数据进行操作的函数地址
            /* we are done... */
        }
        while (0);
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    
    curl_global_cleanup();
    
}
