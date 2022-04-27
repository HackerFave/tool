#include <QCoreApplication>
//#include "httpreq.h"
//#include "json/json.h"
#include "jsoncpp/json/json.h"
#include "HttpRequest.h"
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <curl/curl.h>
using namespace std;

// split function
vector<string> split(const string &str, const string &pattern)
{
    char *strc = new char[strlen(str.c_str()) + 1];
    strcpy(strc, str.c_str());
    vector<string> resultVec;
    char *tmpStr = strtok(strc, pattern.c_str());
    while (tmpStr != NULL)
    {
        resultVec.push_back(string(tmpStr));
        tmpStr = strtok(NULL, pattern.c_str());
    }
    delete[] strc;
    return resultVec;
}

// get path gps points
vector<string> getGps(string origin, string destination)
{
    vector<string> gps;
    HttpRequest *Http;
    char http_return[4964096] = {0};
    char http_msg[4096] = {0};
    char data[4096] = {0};
    string url = "http://192.168.30.137:8660/api/v1/certificate" ;
    string json = "{"
                  "\"Content-Type\": \"application/json\","
                  "\"userId\": \"123456\","
                  "\"csr\": \"MIIBsjCCAVkCAQAwWDEQMA4GA1UECAwHQmVpamluZzEQMA4GA1UEBwwHSGFpZGlhbjEMMAoGA1UECgwDT3JnMQ0wCwYDVQQLDARVbml0MRUwEwYDVQQDDAwxOTIuMTY4LjIuMjUwWTATBgcqhkjOPQIBBggqgRzPVQGCLQNCAATkYPvHXE1PZpESCu/wg4YA+iKLFcnX13AP8ainmgDv/dXrZ/e0hyNglz7pwvCyxUxDWT9wNY3gIf097aHY9en0oIGeMBgGCSqGSIb3DQEJBzELDAkxMjMxMjMxMjMwgYEGCSqGSIb3DQEJDjF0MHIwCQYDVR0TBAIwADAdBgNVHQ4EFgQU2jmj7l5rSw0yVb/vlWAYkK/YBwkwOQYDVR0RBDIwMKAuBggrBgEFBQcIBKAiBCAxMTExMTExMTExMTExMTExMTExMTExMTExMTExMTExMTALBgNVHQ8EBAMCBsAwCgYIKoZIzj0EAwIDRwAwRAIgSP/QaDetr8YJqQIG+S//JSs4ThMTrVMlshmGD2UQjz0CIEBgaR4lZNq7/Ae1iNRScHdl/noXUNBE8/OyuQO0nU8U\"}";
    string str = url+json/*+userId+csr*/;
    std::cout<<http_msg<<std::endl;
    strcpy(http_msg, str.c_str());
    strcpy(data,json.c_str());
    if (Http->/*HttpGet*/HttpPost(http_msg,/*json.c_str()*/data, http_return))
    {
        std::cout<<"JJJJJJJJJJJ"<</*response.c_str()*/http_return<<"UUUUUUUUUUUUUUUUUUUUUU"<<std::endl;
        char *resp = strstr(http_return, "\r\n\r\n") + 4;
        if (nullptr != resp)
        {
            string response = resp;
            //      Json::Reader reader;
            //      Json::Value rt;
            //      if (reader.parse(response, rt))
            //      {
            //        Json::Value paths_val = rt["route"]["paths"];
            //        if (paths_val.size() > 0)
            //        {
            //          Json::Value polylines_val = paths_val[0]["steps"];
            //          for (int j = 0; j < polylines_val.size(); j++)
            //          {
            //            vector<string> lines = split(polylines_val[j]["polyline"].asString(), ";");
            //            for (auto lin : lines)
            //            {
            //              if (gps.size() > 0)
            //              {
            //                if (gps.back() != lin)
            //                {
            //                  gps.push_back(lin);
            //                }
            //              }
            //              else
            //              {
            //                gps.push_back(lin);
            //              }
            //            }
            //          }
            //        }
            //      }
        }
    }
    return gps;
}

/* 编译
 * g++ -L. -o a.out main.cpp -ljsoncpp
 */
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //    string origin = "119.508988,32.356766";
    //    string destination = "119.497028,32.393829";
    //    vector<string> gps = getGps(origin, destination);
    //    for (auto lin : gps)
    //    {
    //        cout << lin << endl;
    //    }
    HttpRequest *Http;
    char http_return[4964096] = {0};
    char http_msg[4096] = {0};
    char data[4096] = {0};
//    string url = "http://192.168.10.38:8660/api/v1/certificate" ;
//    string json = "{"
////                  "\"Content-Type\": \"application/json\","
//                  "\"userId\": \"123456\","
//                  "\"csr\": \"MIIBsjCCAVkCAQAwWDEQMA4GA1UECAwHQmVpamluZzEQMA4GA1UEBwwHSGFpZGlhbjEMMAoGA1UECgwDT3JnMQ0wCwYDVQQLDARVbml0MRUwEwYDVQQDDAwxOTIuMTY4LjIuMjUwWTATBgcqhkjOPQIBBggqgRzPVQGCLQNCAATkYPvHXE1PZpESCu/wg4YA+iKLFcnX13AP8ainmgDv/dXrZ/e0hyNglz7pwvCyxUxDWT9wNY3gIf097aHY9en0oIGeMBgGCSqGSIb3DQEJBzELDAkxMjMxMjMxMjMwgYEGCSqGSIb3DQEJDjF0MHIwCQYDVR0TBAIwADAdBgNVHQ4EFgQU2jmj7l5rSw0yVb/vlWAYkK/YBwkwOQYDVR0RBDIwMKAuBggrBgEFBQcIBKAiBCAxMTExMTExMTExMTExMTExMTExMTExMTExMTExMTExMTALBgNVHQ8EBAMCBsAwCgYIKoZIzj0EAwIDRwAwRAIgSP/QaDetr8YJqQIG+S//JSs4ThMTrVMlshmGD2UQjz0CIEBgaR4lZNq7/Ae1iNRScHdl/noXUNBE8/OyuQO0nU8U\"}";
   string url ="http://192.168.10.38:8660/api/v1/certificate?userId=123456&csr1=MIIBsjCCAVkCAQAwWDEQMA4GA1UECAwHQmVpamluZzEQMA4GA1UEBwwHSGFpZGlhbjEMMAoGA1UECgwDT3JnMQ0wCwYDVQQLDARVbml0MRUwEwYDVQQDDAwxOTIuMTY4LjIuMjUwWTATBgcqhkjOPQIBBggqgRzPVQGCLQNCAATkYPvHXE1PZpESCu/wg4YA+iKLFcnX13AP8ainmgDv/dXrZ/e0hyNglz7pwvCyxUxDWT9wNY3gIf097aHY9en0oIGeMBgGCSqGSIb3DQEJBzELDAkxMjMxMjMxMjMwgYEGCSqGSIb3DQEJDjF0MHIwCQYDVR0TBAIwADAdBgNVHQ4EFgQU2jmj7l5rSw0yVb/vlWAYkK/YBwkwOQYDVR0RBDIwMKAuBggrBgEFBQcIBKAiBCAxMTExMTExMTExMTExMTExMTExMTExMTExMTExMTExMTALBgNVHQ8EBAMCBsAwCgYIKoZIzj0EAwIDRwAwRAIgSP/QaDetr8YJqQIG+S//JSs4ThMTrVMlshmGD2UQjz0CIEBgaR4lZNq7/Ae1iNRScHdl/noXUNBE8/OyuQO0nU8U";
//    string str = url/*+json*//*+userId+csr*/;
    std::cout<<http_msg<<std::endl;
    strcpy(http_msg, url.c_str());
//    strcpy(data,json.c_str());
//    std::cout <<url<<"***********"<<json<<std::endl;
    if (Http->HttpGet/*HttpPost*/(http_msg,/*json.c_str(),*/ http_return))
    {

        char *resp = strstr(http_return, "\r\n\r\n") + 4;
        std::cout<<"返回\n"<</*response.c_str()*/http_return<<std::endl;
        if (nullptr != resp)
        {
            string response = resp;
            //      Json::Reader reader;
            //      Json::Value rt;
            //      if (reader.parse(response, rt))
            //      {
            //        Json::Value paths_val = rt["route"]["paths"];
            //        if (paths_val.size() > 0)
            //        {
            //          Json::Value polylines_val = paths_val[0]["steps"];
            //          for (int j = 0; j < polylines_val.size(); j++)
            //          {
            //            vector<string> lines = split(polylines_val[j]["polyline"].asString(), ";");
            //            for (auto lin : lines)
            //            {
            //              if (gps.size() > 0)
            //              {
            //                if (gps.back() != lin)
            //                {
            //                  gps.push_back(lin);
            //                }
            //              }
            //              else
            //              {
            //                gps.push_back(lin);
            //              }
            //            }
            //          }
            //        }
            //      }
        }
    }
    return a.exec();
}
