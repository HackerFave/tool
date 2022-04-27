#ifndef SIGNSDK_H
#define SIGNSDK_H
#include <iostream>
#include "client_ssl.h"
#include "mq_client.h"
#include <sstream>
//#include <boost/foreach.hpp>
//#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include<string>

using namespace boost::property_tree;
using namespace std;
class signSdk
{
public:
    signSdk();
    void init_sdk();//初始化sdk
    void pwd_test();
    void tls_test();
    void test_post();
    static size_t readData(void *contents, size_t size, size_t nmemb, void *stream);//读取数据
    bool ParseJson(const std::string &json,std::string &cert);//解析json
    int LoadConfig(string ConfigPath);//解析ini
    bool readFile(std::string path);
private:
    void get_cert();//获取证书
    void importexport_cert( unsigned char * client_cert,int length);//导入导出证书
    bool isExistCert();//判断是否存在证书
    string _userId ;
    string _pin ;
    int _alg ;// 算法
    std::shared_ptr<client_ssl> _client = nullptr;
    std::shared_ptr<mq_client> _mq_client =nullptr;
//    mq_client *_mq_client = nullptr;
    char *_CACerts = nullptr ;//证书链
    string _ip;
    string _port;
    string _certs="";
    string _imei;

};

#endif // SIGNSDK_H
