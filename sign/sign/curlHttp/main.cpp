#include <QCoreApplication>
#include "httpMethord.h"
#include <string>
using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // get 请求
    string strURL  ="http://192.168.30.137:8660/api/v1/certificate?"
                    "userId=123456&csr1=MIIBsjCCAVkCAQAwWDEQMA4GA1UECA"
                    "wHQmVpamluZzEQMA4GA1UEBwwHSGFpZGlhbjEMMAoGA1UECgwDT3JnMQ0wCw"
                    "YDVQQLDARVbml0MRUwEwYDVQQDDAwxOTIuMTY4LjIuMjUwWTATBgcqhkjOPQI"
                    "BBggqgRzPVQGCLQNCAATkYPvHXE1PZpESCu/wg4YA+iKLFcnX13AP8ainmgDv/d"
                    "XrZ/e0hyNglz7pwvCyxUxDWT9wNY3gIf097aHY9en0oIGeMBgGCSqGSIb3DQEJBzE"
                    "LDAkxMjMxMjMxMjMwgYEGCSqGSIb3DQEJDjF0MHIwCQYDVR0TBAIwADAdBgNVHQ4"
                    "EFgQU2jmj7l5rSw0yVb/vlWAYkK/YBwkwOQYDVR0RBDIwMKAuBggrBgEFBQcIBKA"
                    "iBCAxMTExMTExMTExMTExMTExMTExMTExMTExMTExMTExMTALBgNVHQ8EBAMCBsAw"
                    "CgYIKoZIzj0EAwIDRwAwRAIgSP/QaDetr8YJqQIG+S//JSs4ThMTrVMlshmGD2UQ"
                    "jz0CIEBgaR4lZNq7/Ae1iNRScHdl/noXUNBE8/OyuQO0nU8U" ;
    string strResponse;
    CURLcode nRes = httpMethord::HttpGet(strURL, strResponse,300);
    size_t nSrcLength = strResponse.length();
    std::cout << strResponse<<std::endl;
//    //下载文件
//    string strURL = "http://www.baidu.com/aaa.dat";
//    char local_file[50] = {0};
//    sprintf(local_file,"./pb_%d_%s.dat",1,"aaaa");
//    int iDownlaod_Success = CommonTools::download_file(url.c_str(),local_file);
//    if(iDownlaod_Success<0){
//        char download_failure_info[100] ={0};
//        sprintf(download_failure_info,"download file :%s ,failure,url:",local_file,url);
//        FATAL(download_failure_info);
//    }
    return a.exec();
}
